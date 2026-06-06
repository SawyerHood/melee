#!/usr/bin/env python3
"""check_fn.py - fast single-function match iteration loop for the Melee decomp.

Given a function name, this tool:
  1. Resolves the function -> translation unit (via campaign/backlog.json, with a
     fallback to build/GALE01/report.json).
  2. Rebuilds ONLY that unit's object file by invoking the compiler directly
     (extracted from build.ninja). This is ~8-10x faster than `ninja <obj>` and
     produces a byte-identical .o (verified). Pass --ninja to use ninja instead.
  3. Runs objdiff-cli one-shot and prints:
       (a) the current fuzzy / match percent for the function
       (b) a compact side-by-side instruction diff with mismatch markers
  4. Exits 0 iff the function is a 100.0% match, else 1 (2 on error).

stdlib only. Usage:
  python3 campaign/check_fn.py <function_name> [--ninja] [--full] [--quiet]

Run from anywhere; paths are resolved relative to the repo root (this file's
grandparent: campaign/ -> repo root).
"""
import argparse
import json
import os
import shlex
import subprocess
import sys
import tempfile
import time

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
BACKLOG = os.path.join(REPO, "campaign", "backlog.json")
REPORT = os.path.join(REPO, "build", "GALE01", "report.json")
BUILD_NINJA = os.path.join(REPO, "build.ninja")
OBJDIFF = os.path.join(REPO, "build", "tools", "objdiff-cli")

# Markers for diff_kind values emitted by objdiff-cli.
# wine + MoltenVK dumps a huge Vulkan-extension log to stderr on every launch.
# Capturing that via a subprocess PIPE causes pipe-buffer backpressure that stalls
# the writer, turning a ~0.55s compile into ~6s. The reliable fix is to redirect
# child stdout+stderr to a regular temp FILE (file writes don't block) and read it
# back only if we need to surface a compile error. Env-based log suppression
# (MVK_CONFIG_LOG_LEVEL / WINEDEBUG) is unreliable because wineserver caches state
# across runs. We still set them as a harmless best-effort.
QUIET_ENV = dict(os.environ)
QUIET_ENV.setdefault("MVK_CONFIG_LOG_LEVEL", "0")   # MoltenVK: errors only
QUIET_ENV.setdefault("WINEDEBUG", "-all")           # silence wine debug channels

_NOISE_PREFIXES = (
    "[mvk-info]", "VK_", "The following", "GPU ", "model:", "type:",
    "vendorID:", "deviceID:", "pipelineCacheUUID:", "Metal ",
    "supports the following", "Read-Write", "Created VkInstance",
)


def _clean(text):
    """Drop residual MoltenVK / Vulkan-extension noise from captured output."""
    out = []
    for ln in text.splitlines():
        s = ln.strip()
        if "Vulkan extension" in s or any(s.startswith(p) for p in _NOISE_PREFIXES):
            continue
        out.append(ln)
    return "\n".join(out)


def _run_to_tempfile(argv_or_cmd, shell):
    """Run a child with stdout+stderr -> temp file (no pipe stall).

    Returns (returncode, combined_output_text, elapsed_seconds).
    """
    t0 = time.time()
    with tempfile.TemporaryFile() as tf:
        p = subprocess.run(argv_or_cmd, shell=shell, cwd=REPO, stdout=tf,
                           stderr=subprocess.STDOUT, env=QUIET_ENV)
        dt = time.time() - t0
        tf.seek(0)
        out = tf.read().decode("utf-8", "replace")
    return p.returncode, out, dt


MARK = {
    None: "  ",
    "DIFF_NONE": "  ",
    "DIFF_ARG_MISMATCH": "~ ",   # same mnemonic, different argument/relocation
    "DIFF_OP_MISMATCH": "x ",    # different mnemonic
    "DIFF_REPLACE": "x ",
    "DIFF_INSERT": "+ ",
    "DIFF_DELETE": "- ",
}


def die(msg, code=2):
    sys.stderr.write("check_fn: %s\n" % msg)
    sys.exit(code)


def find_unit(fn):
    """Return (unit, recorded_fuzzy_or_None). Try backlog first, then report."""
    if os.path.exists(BACKLOG):
        try:
            data = json.load(open(BACKLOG))
            for f in data.get("functions", []):
                if f.get("name") == fn:
                    return f.get("unit"), f.get("fuzzy")
        except (ValueError, OSError):
            pass
    if os.path.exists(REPORT):
        try:
            data = json.load(open(REPORT))
            for u in data.get("units", []):
                for f in (u.get("functions") or []):
                    if f.get("name") == fn:
                        return u.get("name"), f.get("fuzzy_match_percent")
        except (ValueError, OSError):
            pass
    return None, None


def unit_to_obj_and_src(unit):
    """unit 'main/melee/X' -> (obj_path, src_path). Drops the leading 'main/'."""
    rel = unit
    if rel.startswith("main/"):
        rel = rel[len("main/"):]
    obj = os.path.join(REPO, "build", "GALE01", "src", rel + ".o")
    src = os.path.join(REPO, "src", rel + ".c")
    return obj, src


def objdiff_paths(unit):
    """Return (target_path, base_path) for a unit from objdiff.json."""
    cfg = os.path.join(REPO, "objdiff.json")
    try:
        data = json.load(open(cfg))
        for u in data.get("units", []):
            if u.get("name") == unit:
                return u.get("target_path"), u.get("base_path")
    except (ValueError, OSError):
        pass
    return None, None


def parse_build_command(obj_rel):
    """Extract the compile command for a given obj from build.ninja.

    Returns a shell-ready command string that compiles the unit directly
    (wine + sjiswrap + mwcceppc.exe). We reconstruct it from the build edge's
    rule (mwcc / mwcc_sjis / *_extab) and its bound variables (mw_version,
    cflags, basedir, in). The depfile-generation (-MMD) and transform_dep.py /
    extab post-steps are intentionally omitted: they do not affect the .o bytes
    (verified byte-identical) and only update the ninja .d depfile.
    """
    # obj_rel is the ninja build target, e.g.
    #   build/GALE01/src/melee/ft/chara/ftCommon/ftCo_Attack100.o
    text = open(BUILD_NINJA, encoding="utf-8", errors="replace").read()
    lines = text.split("\n")
    # Find the "build <obj>: <rule> <in> ..." edge (may span continuation lines).
    target_prefix = "build " + obj_rel + ":"
    start = None
    for i, ln in enumerate(lines):
        if ln.startswith(target_prefix):
            start = i
            break
    if start is None:
        return None
    # Join continuation ($ at EOL) for the build line + its indented var block.
    block = []
    i = start
    # First: the build statement (may continue with trailing $).
    stmt = []
    while i < len(lines):
        ln = lines[i]
        if ln.endswith("$"):
            stmt.append(ln[:-1])
            i += 1
        else:
            stmt.append(ln)
            i += 1
            break
    build_stmt = " ".join(s.strip() for s in stmt)
    # Now collect indented variable lines until a blank line / next 'build '.
    variables = {}
    while i < len(lines):
        ln = lines[i]
        if ln.strip() == "" or ln.startswith("build ") or not (ln.startswith(" ") or ln.startswith("\t")):
            break
        # var assignment may also span continuations.
        vstmt = []
        cur = ln
        while True:
            if cur.endswith("$"):
                vstmt.append(cur[:-1])
                i += 1
                cur = lines[i]
            else:
                vstmt.append(cur)
                i += 1
                break
        joined = " ".join(p.strip() for p in vstmt)
        if "=" in joined:
            k, v = joined.split("=", 1)
            variables[k.strip()] = v.strip()
    # Parse "build <obj>: <rule> <inputs...> | <implicit...>"
    after = build_stmt[len(target_prefix):].strip()
    # inputs end at '|' (order-only / implicit deps) or '||'.
    parts = after.split()
    if not parts:
        return None
    rule = parts[0]
    rest = parts[1:]
    inputs = []
    for tok in rest:
        if tok in ("|", "||"):
            break
        inputs.append(tok)
    in_file = inputs[0] if inputs else None
    if in_file is None:
        return None

    mw = variables.get("mw_version", "GC/1.2.5n")
    cflags = variables.get("cflags", "")
    basedir = variables.get("basedir", os.path.dirname(obj_rel))

    compiler = "build/compilers/%s/mwcceppc.exe" % mw
    sjiswrap = "build/tools/sjiswrap.exe"
    # Build a direct, no-depfile compile command.
    if rule in ("mwcc_sjis", "mwcc_sjis_extab"):
        prefix = "wine %s %s" % (shlex.quote(sjiswrap), shlex.quote(compiler))
    else:  # mwcc / mwcc_extab
        prefix = "wine %s" % shlex.quote(compiler)
    # CRITICAL: compile into a SCRATCH dir, never the canonical build path.
    # Writing build/GALE01/src/...o directly poisons ninja's incremental state
    # (the .o becomes newer than its source, so ninja skips the rebuild and the
    # extab-clean post-step never runs -> phantom .data/extab diffs).
    scratch_dir = os.path.join(REPO, "campaign", "scratch", os.path.dirname(obj_rel))
    os.makedirs(scratch_dir, exist_ok=True)
    cmd = "%s %s -c %s -o %s" % (prefix, cflags, shlex.quote(in_file), shlex.quote(scratch_dir))
    scratch_obj = os.path.join(scratch_dir, os.path.basename(obj_rel))
    is_extab = rule.endswith("_extab")
    return cmd, scratch_obj, is_extab


def rebuild_direct(obj_rel):
    parsed = parse_build_command(obj_rel)
    if parsed is None:
        return None, "could not parse build command from build.ninja for %s" % obj_rel, None, False
    cmd, scratch_obj, is_extab = parsed
    rc, out, dt = _run_to_tempfile(cmd, shell=True)
    if rc != 0:
        return dt, "compile failed (rc=%d):\n%s" % (rc, _clean(out)[-2000:]), None, is_extab
    return dt, None, scratch_obj, is_extab


def rebuild_ninja(obj_rel):
    rc, out, dt = _run_to_tempfile(["ninja", obj_rel], shell=False)
    if rc != 0:
        return dt, "ninja failed (rc=%d):\n%s" % (rc, _clean(out)[-2000:])
    return dt, None


def run_objdiff(unit, base_override=None):
    if base_override:
        target, _base = objdiff_paths(unit)
        if not target:
            return None, "unit %r not found in objdiff.json" % unit
        argv = [OBJDIFF, "diff", "-1", target, "-2", base_override,
                "-o", "-", "--format", "json"]
    else:
        argv = [OBJDIFF, "diff", "-p", REPO, "-u", unit, "-o", "-", "--format", "json"]
    p = subprocess.run(argv, cwd=REPO, stdout=subprocess.PIPE,
                       stderr=subprocess.DEVNULL, env=QUIET_ENV)
    if p.returncode != 0:
        return None, "objdiff-cli failed (rc=%d)" % p.returncode
    try:
        return json.loads(p.stdout.decode("utf-8", "replace")), None
    except ValueError as e:
        return None, "could not parse objdiff JSON: %s" % e


def sym(side, data, fn):
    for s in (data.get(side) or {}).get("symbols", []):
        if s.get("name") == fn:
            return s
    return None


def fmt_instr(ins):
    if not ins or "instruction" not in ins:
        return ""
    return ins["instruction"].get("formatted", "")


def render_diff(left_sym, right_sym, full=False, width=44):
    """Side-by-side: target (left) | our build (right), with mismatch markers."""
    L = left_sym.get("instructions", []) if left_sym else []
    R = right_sym.get("instructions", []) if right_sym else []
    n = max(len(L), len(R))
    out = []
    header = "%-*s %-*s %s" % (width, "TARGET (left)", width, "OURS (right)", "")
    out.append(header)
    out.append("-" * (width * 2 + 4))
    shown = 0
    for i in range(n):
        li = L[i] if i < len(L) else None
        ri = R[i] if i < len(R) else None
        ldk = (li or {}).get("diff_kind")
        rdk = (ri or {}).get("diff_kind")
        is_diff = (ldk not in (None, "DIFF_NONE")) or (rdk not in (None, "DIFF_NONE"))
        if not full and not is_diff:
            continue
        mark = MARK.get(ldk, "? ") if ldk not in (None, "DIFF_NONE") else MARK.get(rdk, "  ")
        ltxt = fmt_instr(li)
        rtxt = fmt_instr(ri)
        flag = "  " if not is_diff else mark
        out.append("%s%-*s %-*s" % (flag, width, ltxt[:width], width, rtxt[:width]))
        shown += 1
    if shown == 0:
        out.append("(no instruction-level differences)")
    return "\n".join(out)


def main():
    ap = argparse.ArgumentParser(description="Fast single-function match check.")
    ap.add_argument("function", help="function symbol name, e.g. ftCo_800D72A0")
    ap.add_argument("--ninja", action="store_true",
                    help="rebuild via `ninja <obj>` instead of direct compile")
    ap.add_argument("--full", action="store_true",
                    help="print all instructions, not just diff rows")
    ap.add_argument("--no-build", action="store_true",
                    help="skip rebuild; just diff the existing .o")
    ap.add_argument("--quiet", action="store_true",
                    help="only print fuzzy %% and exit code (no diff)")
    args = ap.parse_args()

    fn = args.function
    unit, recorded = find_unit(fn)
    if not unit:
        die("function %r not found in backlog.json or report.json" % fn)

    obj, src = unit_to_obj_and_src(unit)
    obj_rel = os.path.relpath(obj, REPO)
    print("function : %s" % fn)
    print("unit     : %s" % unit)
    print("source   : %s" % os.path.relpath(src, REPO))
    print("object   : %s" % obj_rel)

    base_override = None
    if not args.no_build:
        if args.ninja:
            dt, err = rebuild_ninja(obj_rel)
            mode = "ninja"
        else:
            dt, err, base_override, is_extab = rebuild_direct(obj_rel)
            mode = "direct(scratch)"
            if is_extab and not err:
                print("note     : extab unit — data/extab sections may show "
                      "phantom diffs in scratch mode; gate with ninja + land.py")
        if err:
            die("rebuild (%s) %s" % (mode, err), 2)
        print("rebuild  : %s in %.2fs" % (mode, dt))

    data, err = run_objdiff(unit, base_override=base_override)
    if err:
        die(err, 2)
    ls = sym("left", data, fn)
    rs = sym("right", data, fn)
    if ls is None:
        die("symbol %r not present in objdiff output (left)" % fn, 2)

    pct = ls.get("match_percent")
    if pct is None:
        pct = 0.0
    print("fuzzy    : %.4f%%  (recorded baseline: %s)" % (
        pct, "%.4f%%" % recorded if isinstance(recorded, (int, float)) else recorded))

    matched = abs(pct - 100.0) < 1e-6
    if not args.quiet:
        print()
        print(render_diff(ls, rs, full=args.full))
        print()
        print("legend: '~' arg/reloc mismatch  'x' mnemonic mismatch  "
              "'+' extra in ours  '-' missing in ours")

    if matched:
        print("RESULT   : MATCH (100%)")
        sys.exit(0)
    else:
        print("RESULT   : NOT MATCHED (%.4f%%)" % pct)
        sys.exit(1)


if __name__ == "__main__":
    main()
