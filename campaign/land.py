#!/usr/bin/env python3
"""Land wins from a grind batch.

For every modified file under src/ (or extern/):
  - find its backlog functions and re-check each with the unit diff
  - report which improved / regressed vs the recorded baseline
Then run the full build and confirm the DOL byte check.

Usage: python3 campaign/land.py          # analyze only
       python3 campaign/land.py --build  # also run full ninja + DOL check
"""
import json
import subprocess
import sys
from pathlib import Path

REPO = Path("/Users/sawyerhood/melee")
OBJDIFF = REPO / "build/tools/objdiff-cli"
BACKLOG = REPO / "campaign/backlog.json"


def sh(cmd, **kw):
    return subprocess.run(cmd, capture_output=True, text=True, cwd=REPO, **kw)


def modified_sources():
    out = sh(["git", "diff", "--name-only"]).stdout.splitlines()
    return [f for f in out if f.startswith(("src/", "extern/")) and f.endswith(".c")]


def unit_for(path):
    return "main/" + path[len("src/"):-2] if path.startswith("src/") else None


def unit_fuzzy(unit):
    """Return {fn_name: fuzzy%} for all symbols in a unit (rebuild first)."""
    proc = sh([str(OBJDIFF), "diff", "-p", str(REPO), "-u", unit, "-o", "-", "--format", "json"])
    if proc.returncode != 0:
        return None
    d = json.loads(proc.stdout)
    return {
        s["name"]: float(s.get("match_percent", 0))
        for s in d.get("left", {}).get("symbols", [])
    }


def main():
    backlog = json.loads(BACKLOG.read_text())["functions"]
    by_unit = {}
    for f in backlog:
        by_unit.setdefault(f["unit"], []).append(f)

    mods = modified_sources()
    if not mods:
        print("No modified source files.")
    wins, regressions = [], []
    for path in mods:
        unit = unit_for(path)
        print(f"\n== {path} (unit {unit})")
        if not unit:
            print("   (extern/ file — check manually)")
            continue
        # rebuild this unit via check_fn on its first backlog fn
        fns = by_unit.get(unit, [])
        if fns:
            subprocess.run(
                ["python3", "campaign/check_fn.py", fns[0]["name"], "--quiet"],
                cwd=REPO, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL,
            )
        cur = unit_fuzzy(unit)
        if cur is None:
            print("   objdiff failed!")
            continue
        # check every backlog fn in the unit AND look for new regressions
        for f in fns:
            now = cur.get(f["name"])
            mark = "WIN " if now == 100.0 else ("UP  " if now and now > f["fuzzy"] else ("DOWN" if now and now < f["fuzzy"] else "same"))
            print(f"   {mark} {f['name']:36} {f['fuzzy']:7.2f} -> {now}")
            if now == 100.0:
                wins.append((path, f["name"]))
            elif now is not None and now < f["fuzzy"]:
                regressions.append((path, f["name"], f["fuzzy"], now))
        baseline_names = {f["name"] for f in fns}
        for name, pct in cur.items():
            if pct < 100.0 and name not in baseline_names:
                print(f"   REGRESSION (was 100): {name} -> {pct}")
                regressions.append((path, name, 100.0, pct))

    print(f"\nTotal: {len(wins)} wins, {len(regressions)} regressions")
    for w in wins:
        print(f"  WIN {w[1]} in {w[0]}")
    for r in regressions:
        print(f"  REGRESSION {r[1]} in {r[0]}: {r[2]} -> {r[3]}")

    if "--build" in sys.argv:
        print("\nRunning full ninja + DOL check…")
        proc = subprocess.run(["ninja"], cwd=REPO, capture_output=True, text=True)
        ok = "main.dol: OK" in proc.stdout + proc.stderr
        print("DOL CHECK:", "OK" if ok else "FAILED")
        if not ok:
            print((proc.stdout + proc.stderr)[-2000:])
            sys.exit(1)


if __name__ == "__main__":
    main()
