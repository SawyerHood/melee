#!/usr/bin/env python3
"""Phase 0 triage: classify every sub-100% function into a campaign backlog.

Classes (derived from objdiff instruction-level diffs, not fuzzy % alone):
  reloc_layout - all mismatches are argument/relocation-only with identical
                 mnemonics; the C is right, data/bss layout is displaced
  codegen      - same instruction count but opcode/mnemonic differences;
                 classic register-allocation / expression-shape near-miss
  structural   - instruction counts differ (inserts/deletes); control flow
                 or inlining is wrong
Severity bands from fuzzy %: near_miss >=90, partial 50-90, hard <50.
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
    """One-shot objdiff for a unit; returns {symbol_name: (left_sym, right_sym)}."""
    proc = subprocess.run(
        [str(OBJDIFF), "diff", "-p", str(REPO), "-u", unit_name, "-o", "-", "--format", "json"],
        capture_output=True,
        text=True,
        cwd=REPO,
    )
    if proc.returncode != 0:
        return None
    d = json.loads(proc.stdout)
    out = {}
    left_syms = {s["name"]: s for s in d.get("left", {}).get("symbols", [])}
    right_syms = {s["name"]: s for s in d.get("right", {}).get("symbols", [])}
    for name, ls in left_syms.items():
        out[name] = (ls, right_syms.get(name))
    return out


def classify(left_sym, right_sym):
    if not right_sym:
        return "missing_in_build"
    li = left_sym.get("instructions") or []
    ri = right_sym.get("instructions") or []
    if len(li) != len(ri):
        return "structural"
    saw_arg_mismatch = False
    for l, r in zip(li, ri):
        lk, rk = l.get("diff_kind"), r.get("diff_kind")
        if not lk and not rk:
            continue
        lin, rin = l.get("instruction"), r.get("instruction")
        if lin is None or rin is None:
            return "structural"
        if lin.get("mnemonic") != rin.get("mnemonic"):
            return "codegen"
        if (lk or rk) in ("DIFF_ARG_MISMATCH", "DIFF_BRANCH_MISMATCH"):
            saw_arg_mismatch = True
        else:
            return "codegen"
    return "reloc_layout" if saw_arg_mismatch else "codegen"


def severity(fuzzy):
    if fuzzy >= 90:
        return "near_miss"
    if fuzzy >= 50:
        return "partial"
    return "hard"


ADDR_RE = re.compile(r"\b(80[0-9A-Fa-f]{6})\b")
LINE_RE = re.compile(r"^(.*?) = ([\d.]+)%:(\S+); // author:(\S+) id:(\S+)")


def load_scratches():
    by_name = defaultdict(list)
    by_addr = defaultdict(list)
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
    addr_suffix = fn["vaddr"][2:].upper()
    hits += [h for h in by_addr.get(addr_suffix, []) if h not in hits]
    if not hits:
        return None
    best = max(hits, key=lambda h: h["pct"])
    return {"pct": best["pct"], "id": best["id"], "author": best["author"], "n_scratches": len(hits)}


def main():
    rows = load_sub100()
    units = sorted({r["unit"] for r in rows})
    print(f"{len(rows)} sub-100% functions across {len(units)} units", file=sys.stderr)

    diffs = {}
    failed_units = []
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
        syms = diffs.get(r["unit"]) or {}
        pair = syms.get(r["name"])
        r["class"] = classify(*pair) if pair else "diff_unavailable"
        r["severity"] = severity(r["fuzzy"])
        sc = best_scratch(r, by_name, by_addr)
        if sc:
            r["scratch"] = sc

    rows.sort(key=lambda r: (-r["fuzzy"], r["size"]))
    OUT.write_text(json.dumps({"functions": rows, "failed_units": failed_units}, indent=1))

    cls = Counter(r["class"] for r in rows)
    sev = Counter(r["severity"] for r in rows)
    combo = Counter((r["class"], r["severity"]) for r in rows)
    with_scratch = sum(1 for r in rows if "scratch" in r)
    matched_scratch = sum(1 for r in rows if r.get("scratch", {}).get("pct", 0) >= 100)
    subsys = Counter(re.sub(r"^main/", "", r["unit"]).split("/")[1] if "/" in r["unit"] else r["unit"] for r in rows)

    print(f"\nBy class:    {dict(cls)}")
    print(f"By severity: {dict(sev)}")
    print("By class x severity:")
    for (c, s), n in sorted(combo.items(), key=lambda kv: -kv[1]):
        print(f"  {c:>16} x {s:<9} {n}")
    print(f"\nScratch hits: {with_scratch} functions have decomp.me scratches; {matched_scratch} have a 100% scratch")
    print(f"Top subsystems: {subsys.most_common(10)}")
    if failed_units:
        print(f"FAILED units ({len(failed_units)}): {failed_units[:5]}...")
    print(f"\nWrote {OUT}")


if __name__ == "__main__":
    main()
