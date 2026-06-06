#!/usr/bin/env python3
"""Land wins from a grind batch — A/B attribution edition.

For every modified file under src/:
  A) build its unit via ninja WITH the edit, record per-symbol match %
  B) stash the file, rebuild, record per-symbol match % WITHOUT the edit
  C) restore, rebuild, and report the per-symbol deltas B -> A

This compares diff-output to diff-output (same instrument), avoiding the
stale-snapshot problem of comparing against backlog/report numbers, and
attributes every change to the edit itself.

Usage: python3 campaign/land.py            # A/B all modified src files
       python3 campaign/land.py --dol      # then full ninja + DOL check
"""
import json
import subprocess
import sys
from pathlib import Path

REPO = Path("/Users/sawyerhood/melee")
OBJDIFF = REPO / "build/tools/objdiff-cli"


def sh(cmd, **kw):
    return subprocess.run(cmd, capture_output=True, text=True, cwd=REPO, **kw)


def modified_sources():
    out = sh(["git", "diff", "--name-only"]).stdout.splitlines()
    return [f for f in out if f.startswith("src/") and f.endswith(".c")]


def unit_for(path):
    return "main/" + path[len("src/"):-2]


def obj_for(path):
    return "build/GALE01/src/" + path[len("src/"):-2] + ".o"


def unit_symbols(unit):
    proc = sh([str(OBJDIFF), "diff", "-p", str(REPO), "-u", unit, "-o", "-", "--format", "json"])
    if proc.returncode != 0:
        return None
    d = json.loads(proc.stdout)
    return {
        s["name"]: float(s.get("match_percent", 0))
        for s in d.get("left", {}).get("symbols", [])
        if s.get("match_percent") is not None
    }


def ninja(target):
    p = subprocess.run(["ninja", target], cwd=REPO, stdout=subprocess.DEVNULL,
                       stderr=subprocess.DEVNULL)
    return p.returncode == 0


def main():
    mods = modified_sources()
    if not mods:
        print("No modified source files.")
    all_wins, all_regs = [], []
    for path in mods:
        unit, obj = unit_for(path), obj_for(path)
        print(f"\n== {path}")
        if not ninja(obj):
            print("   BUILD FAILED with edit — file is broken, fix or revert")
            all_regs.append((path, "<build failure>", 0, 0))
            continue
        with_edit = unit_symbols(unit)
        sh(["git", "stash", "push", "--", path])
        try:
            ok = ninja(obj)
            baseline = unit_symbols(unit) if ok else None
        finally:
            sh(["git", "stash", "pop"])
        ninja(obj)  # restore with-edit object as the canonical state
        if with_edit is None or baseline is None:
            print("   could not measure (objdiff/build failure)")
            continue
        changed = False
        for name in sorted(set(with_edit) | set(baseline)):
            a, b = with_edit.get(name), baseline.get(name)
            if a is None or b is None or abs(a - b) < 1e-9:
                continue
            changed = True
            if a > b:
                tag = "WIN " if a == 100.0 else "UP  "
                if a == 100.0:
                    all_wins.append((path, name))
                print(f"   {tag}{name:40} {b:9.4f} -> {a:9.4f}")
            else:
                all_regs.append((path, name, b, a))
                print(f"   REG {name:40} {b:9.4f} -> {a:9.4f}")
        if not changed:
            print("   edit has NO effect on this unit's match (revert it?)")

    print(f"\nTotal: {len(all_wins)} new 100% matches, {len(all_regs)} regressions")
    for p, n in all_wins:
        print(f"  WIN {n} ({p})")
    for p, n, b, a in all_regs:
        print(f"  REG {n} ({p}): {b} -> {a}")

    if "--dol" in sys.argv:
        print("\nFull ninja + DOL check…")
        proc = subprocess.run(["ninja"], cwd=REPO, capture_output=True, text=True)
        ok = "main.dol: OK" in proc.stdout + proc.stderr
        print("DOL CHECK:", "OK" if ok else "FAILED")
        if not ok:
            print((proc.stdout + proc.stderr)[-2000:])
            sys.exit(1)
    sys.exit(1 if all_regs else 0)


if __name__ == "__main__":
    main()
