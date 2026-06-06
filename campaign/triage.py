#!/usr/bin/env python3
"""Phase 0 triage v2: classify every sub-100% function into a campaign backlog.

v2 incorporates the audit findings from the phase-0 verification workflow:
  - v1 compared objdiff's ALIGNED row counts, which are always equal; the
    structural class was driven entirely by gap rows and misfired badly
    (~31-41% misclassification across buckets).
  - structural now means REAL instruction counts differ (net inserts/deletes);
    balanced insert+delete pairs are local reorders -> codegen.
  - arg-mismatch rows are discriminated by token analysis: register-only
    diffs (regalloc), number diffs (layout displacement), symbol-name-only
    diffs (naming — fixable via config/GALE01/symbols.txt in this fork).

Classes:
  structural   - real instruction counts differ (missing/extra code)
  codegen      - mnemonic differences or balanced reorders
  regalloc     - same shape; only register operands differ
  layout       - same shape; address/offset constants differ
  naming_only  - same shape; only anonymous-vs-named symbols differ
                 (likely byte-correct already; config naming work)
  mixed_arg    - same shape; regalloc + layout rows both present
Severity: near_miss >=90 fuzzy, partial 50-90, hard <50.
"""
import json
import re
import subprocess
import sys
from collections import Counter, defaultdict
from pathlib import Path

REPO = Path("/Users/sawyerhood/melee")
OBJDIFF = REPO / "build/tools/objdiff-cli"
REPORT = REPO / "build/GALE01/report.json"
SCRATCHES = REPO / "config/GALE01/scratches.txt"
OUT = REPO / "campaign/backlog.json"

REG_RE = re.compile(r"\b(?:r\d{1,2}|f\d{1,2}|cr\d)\b")
NUM_RE = re.compile(r"-?0x[0-9a-fA-F]+|\b-?\d+\b")
ANON_RE = re.compile(r"@\d+\b|\.\.\.(?:bss|data|rodata|sdata2?|sbss2?)\.\d+")


def load_sub100():
    report = json.loads(REPORT.read_text())
    rows = []
    for unit in report["units"]:
        for fn in unit.get("functions", []):
            pct = float(fn.get("fuzzy_match_percent", 0))
            if pct >= 100.0:
                continue
            vaddr = int(fn["metadata"]["virtual_address"])
            rows.append(
                {
                    "name": fn["name"],
                    "unit": unit["name"],
                    "vaddr": f"0x{vaddr:08X}",
                    "size": int(fn["size"]),
                    "fuzzy": round(pct, 2),
                }
            )
    return rows


def diff_unit(unit_name):
    proc = subprocess.run(
        [str(OBJDIFF), "diff", "-p", str(REPO), "-u", unit_name, "-o", "-", "--format", "json"],
        capture_output=True,
        text=True,
        cwd=REPO,
    )
    if proc.returncode != 0:
        return None
    d = json.loads(proc.stdout)
    left = {s["name"]: s for s in d.get("left", {}).get("symbols", [])}
    right = {s["name"]: s for s in d.get("right", {}).get("symbols", [])}
    return {n: (ls, right.get(n)) for n, ls in left.items()}


def mnemonic(ins):
    if not ins:
        return None
    m = ins.get("mnemonic")
    if m:
        return m
    for part in ins.get("parts", []):
        op = part.get("opcode")
        if isinstance(op, dict) and op.get("mnemonic"):
            return op["mnemonic"]
    return None


def text_of(ins):
    if not ins:
        return ""
    t = ins.get("formatted") or ins.get("arguments") or ""
    if not t and ins.get("parts"):
        t = " ".join(str(p.get("text", "")) for p in ins["parts"])
    return str(t)


def classify(left_sym, right_sym):
    """Returns (class, row_counts dict)."""
    if not right_sym:
        return "missing_in_build", {}
    li = left_sym.get("instructions") or []
    ri = right_sym.get("instructions") or []
    n = max(len(li), len(ri))
    counts = Counter()
    real_l = real_r = 0
    for i in range(n):
        l = li[i] if i < len(li) else {}
        r = ri[i] if i < len(ri) else {}
        lin, rin = l.get("instruction"), r.get("instruction")
        if lin:
            real_l += 1
        if rin:
            real_r += 1
        lk, rk = l.get("diff_kind"), r.get("diff_kind")
        if not lk and not rk:
            continue
        if not lin or not rin:
            counts["gap"] += 1
            continue
        lm, rm = mnemonic(lin), mnemonic(rin)
        if lm != rm:
            counts["mnemonic"] += 1
            continue
        lt, rt = text_of(lin), text_of(rin)
        anon = bool(ANON_RE.search(lt) or ANON_RE.search(rt))
        # strip anonymous symbol tokens (@297, ...bss.0) BEFORE number
        # extraction so their digits don't masquerade as numeric operands
        lt2, rt2 = ANON_RE.sub("§", lt), ANON_RE.sub("§", rt)
        lregs, rregs = sorted(REG_RE.findall(lt2)), sorted(REG_RE.findall(rt2))
        lnums, rnums = sorted(NUM_RE.findall(lt2)), sorted(NUM_RE.findall(rt2))
        regs_eq, nums_eq = lregs == rregs, lnums == rnums
        if regs_eq and nums_eq:
            counts["naming" if anon else "other_arg"] += 1
        elif regs_eq:
            counts["layout"] += 1
        elif nums_eq:
            counts["regalloc"] += 1
        else:
            counts["mixed"] += 1

    if real_l != real_r:
        return "structural", dict(counts)
    if counts["mnemonic"] or counts["gap"]:
        # equal real counts but mnemonic changes or balanced insert/delete pairs
        return "codegen", dict(counts)
    arg_kinds = {k for k in ("naming", "layout", "regalloc", "mixed", "other_arg") if counts[k]}
    if arg_kinds <= {"naming"}:
        return "naming_only", dict(counts)
    if "mixed" in arg_kinds or ({"layout", "regalloc"} <= arg_kinds):
        return "mixed_arg", dict(counts)
    if "layout" in arg_kinds:
        return "layout", dict(counts)
    if "regalloc" in arg_kinds:
        return "regalloc", dict(counts)
    return "codegen", dict(counts)


def severity(fuzzy):
    if fuzzy >= 90:
        return "near_miss"
    if fuzzy >= 50:
        return "partial"
    return "hard"


ADDR_RE = re.compile(r"\b(80[0-9A-Fa-f]{6})\b")
LINE_RE = re.compile(r"^(.*?) = ([\d.]+)%:(\S+); // author:(\S+) id:(\S+)")


def load_scratches():
    by_name, by_addr = defaultdict(list), defaultdict(list)
    for line in SCRATCHES.read_text().splitlines():
        m = LINE_RE.match(line)
        if not m:
            continue
        name, pct, status, author, sid = m.groups()
        entry = {"name": name, "pct": float(pct), "status": status, "author": author, "id": sid}
        by_name[name].append(entry)
        for addr in ADDR_RE.findall(name):
            by_addr[addr.upper()].append(entry)
    return by_name, by_addr


def best_scratch(fn, by_name, by_addr):
    hits = list(by_name.get(fn["name"], []))
    hits += [h for h in by_addr.get(fn["vaddr"][2:].upper(), []) if h not in hits]
    if not hits:
        return None
    best = max(hits, key=lambda h: h["pct"])
    return {"pct": best["pct"], "id": best["id"], "author": best["author"], "n_scratches": len(hits)}


def main():
    rows = load_sub100()
    units = sorted({r["unit"] for r in rows})
    print(f"{len(rows)} sub-100% functions across {len(units)} units", file=sys.stderr)

    diffs, failed_units = {}, []
    for i, u in enumerate(units):
        d = diff_unit(u)
        if d is None:
            failed_units.append(u)
        else:
            diffs[u] = d
        if (i + 1) % 25 == 0:
            print(f"  diffed {i + 1}/{len(units)} units", file=sys.stderr)

    by_name, by_addr = load_scratches()
    for r in rows:
        pair = (diffs.get(r["unit"]) or {}).get(r["name"])
        if pair:
            r["class"], r["rows"] = classify(*pair)
        else:
            r["class"], r["rows"] = "diff_unavailable", {}
        r["severity"] = severity(r["fuzzy"])
        sc = best_scratch(r, by_name, by_addr)
        if sc:
            r["scratch"] = sc

    rows.sort(key=lambda r: (-r["fuzzy"], r["size"]))
    OUT.write_text(json.dumps({"triage_version": 2, "functions": rows, "failed_units": failed_units}, indent=1))

    cls = Counter(r["class"] for r in rows)
    combo = Counter((r["class"], r["severity"]) for r in rows)
    unit_owner = Counter(r["unit"] for r in rows)
    print(f"\nBy class: {dict(cls.most_common())}")
    print("By class x severity:")
    for (c, s), n in sorted(combo.items(), key=lambda kv: -kv[1]):
        print(f"  {c:>16} x {s:<9} {n}")
    print(f"\nUnits with most sub-100% functions: {unit_owner.most_common(8)}")
    print(f"Wrote {OUT}")


if __name__ == "__main__":
    main()
