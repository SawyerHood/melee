# Melee Decomp Completion Campaign

Fork-based experiment to drive doldecomp/melee from ~97.4% fuzzy / 64% matched
bytes toward 100%. This file is the persistent campaign state — update it at
the end of every session.

> **⭐⭐ ENDGAME METRIC (live since wave 18): LINKED Matching file count.**
> Currently **842/1041 linked (42.80% code-complete)**, matched 72.20%,
> fuzzy 97.59, HEAD beb3bdc77. Flips #1-3 wave 18, #4 (hsd_40FF,
> data-only) wave 19, **#5 (itdrop) at the wave-19 boundary
> (d4fee40d1)**, **#6 (bytecode, ZERO src work) wave 20 (b6674faf6)** —
> surfaced only by the en-masse importer-closure ranker (idiom 132).
> **THREE units sit FLIP-READY at the wave-21 close (uncommitted src)**:
> ftPr_SpecialN + itlinkbomb (linkcheck hard rows NONE — pool-order
> cracked, idioms 146/147) and MSL/math (linkcheck `[]` — logf →100,
> idiom 143). Next tier: gm_1BFA 20B · grgreatbay 31B · itdraw 37B —
> every stream ranks its work by "what unblocks the next flip".

## Ground rules

- This is a fork: `config/` and `configure.py` edits ARE allowed (unlike
  upstream policy). We voluntarily keep the no-fake-match standard: C should
  be plausible original code, not byte-coercing contortions.
- The DOL byte check (`ninja` → `build/GALE01/main.dol: OK`) must pass before
  every commit. One commit per won function (or per unit-level fix).
- **One owner per source file.** MWCC register allocation is TU-global and
  edits in one function can regress siblings. Never run two agents on the
  same .c concurrently. (Phase 0 found a real collision: cobj.c was claimed
  by two streams.)
- **BUILD-RACE LAW (wave 19, live-incident-proven)**: two ninjas racing in
  the shared build dir CORRUPT main.elf/main.dol (transient garbled DOL,
  sha 9cdf54d4). `pgrep -fl 'ninja|mwcceppc'` and confirm idle before
  EVERY full-ninja gate — mandatory for flip gates. Corollary: flips must
  never go live mid-wave on shared trees (a mid-flight Matching flip
  transiently broke the tree-wide DOL for a sibling stream this wave).

## Infrastructure

- `campaign/check_fn.py <function>` — rebuild one unit + diff one function.
  ~0.75s warm. Exit 0 iff 100% fuzzy. `--full` for whole listing, `--ninja`
  for ninja-mediated rebuild (~6s). Direct compile verified byte-identical
  to ninja's output.
- **Wine gotcha:** never PIPE-capture wine compile stderr (MoltenVK banner
  causes pipe backpressure, 10x slowdown). check_fn.py already redirects to
  temp files; any new tooling must too.
- Concurrency: parallel compiles on *different* units are safe (verified).
- `campaign/triage.py` — regenerates `backlog.json` from a fresh build's
  report.json. Re-run after each session; classes are audit-validated (v2.1).
- Per-function verification is NOT sufficient: gate = function at 100% →
  whole-unit objdiff clean (no sibling regressions) → full ninja DOL check.
- **Durable reports (wave-10 lesson)**: agents MUST stream findings to
  `campaign/scratch/<agent>/REPORT.md` incrementally as they work — the
  orchestrator return path is NOT reliable. Wave 10 lost all six agent
  reports when the wave died; the only surviving artifacts were baseline
  snapshots, so every probe result from ~75 min of agent time was lost.

## Backlog (triage v2.1, audit-validated)

1115 functions, 182 units. Classes:

| class | count | meaning | stream |
|---|---|---|---|
| structural | 714 (419 nm / 284 p / 11 hard) | real instruction counts differ | S3/S6 |
| codegen | 197 | mnemonic diffs or balanced reorders | S4 |
| regalloc | 79 | register operands only | S1 |
| layout | 69 | address/offset displacement | S2 |
| mixed_arg | 56 | regalloc + layout both | S2 after S1 idioms |

Per-function `rows` field counts naming/layout/regalloc/mnemonic/gap diff rows.
Functions whose mismatches are partly `naming` rows need the S5 config fix to
reach 100% — check `rows` before assuming a C edit can finish a function.

## ⭐ TU-SPLIT VERDICT (wave 9 experiment — PROVEN, ready to land)

> **EXECUTION STATUS (wave 11, 2026-06-06): ✅ LANDED — SUCCESS, ZERO
> deviations from the wave-9 proof; committed 268945dce.** The
> gm_1832/gm_1884 trio (splits.txt 7 cuts +
> configure.py:972 `Object(NonMatching, "melee/gm/gm_1884.c")` +
> gm_1832.c trimmed to lines 1–1894 + gm_1884.c = wave-9 prototype
> verbatim; NO header edits needed). All four gates PASS: DOL OK + dtk
> shasum at baseline/post-split/idempotence rebuild; report.json 69 fns
> preserved (46+23, none dropped), project matched 71.96→**71.99%**, unit
> fuzzy +0.033; anchors fn_80188550 95.28→**99.18** (residual = 8 pure S1
> regalloc rows) and fn_80188644 99.0→**99.34** (→100 pends the
> TrainingModeState +0x114 header split — now its ONLY blocker);
> controlled gate 203 SAME / 8 UP / 11 DOWN / 1 GONE / 2 NEW with every
> DOWN byte-identical to the wave-9 prototype — all @NNN-repartition
> naming rows except fn_801891F4 (−0.016 web reshape, queued S1).
> **Naming-8 renumber map (recovers all 10 naming DOWNs)**: gm_1884 pool
> @304←@697, @452←@913, @702←@1905, @695←@1898, @445←@515, @446←@516;
> gm_1832-side orphan @1843@0x804DA5F8 needs repair. Gate artifacts:
> `campaign/scratch/tu-split-exec-w11/`. The seven cuts (each a verified
> symbols.txt object boundary): .text gm_80188454 / .rodata lbl_803B7C68 /
> .data lbl_803D9828 / .bss lbl_80473700 / .sdata @515 / .sbss
> lbl_804D6628 / .sdata2 lbl_804DA608.

splits.txt CAN express target-side TU splits and CANNOT break the DOL
(`tools/project.py add_unit()` links the *extracted* object for NonMatching/
absent units; verified live: split applied → ninja → `main.dol: OK`, shasum
PASS). But splits.txt alone does NOT fix the anchor rows — the mismatch is
OUR codegen: MWCC folds multi-static constant-offset webs to the merged TU's
`.bss` base, shifting the lis/addi reloc AND every folded d-form immediate
(instruction bytes, not naming). objdiff also pairs exactly one base object
per unit, so a target-only split just unmeasures the tail functions. The
complete fix is a TRIO landed TOGETHER:
  1. **splits.txt cuts** — every cut must land on a symbols.txt object
     boundary; detect a true TU boundary by partitioning all per-function
     symbol refs per section: part1-max < part2-min must hold in EVERY
     section simultaneously (script from the exp run).
  2. **configure.py** — add `Object(NonMatching, "melee/gm/gm_1884.c")`
     after gm_1832.c (line ~971).
  3. **src split** — new file from prototype + trim gm_1832.c to lines
     1–1894; then post-land @-pin renumber for BOTH TUs (idiom 23).
End-to-end proof (scratch compile, zero src/ edits): gm_1832 lines 1895–2807
compiled as gm_1884.c vs the split target — ALL anchor rows vanish;
fn_80188550 95.44→99.18 (residual = plain S1 regalloc), fn_80188644
99.00→99.34 (→100 after splitting TrainingModeState at +0x114 =
gm_80473814 boundary, header queue). One web reshape to re-solve post-split
(fn_801891F4 97.08→96.31). Ready prototype + exact 7-cut split spec:
`campaign/scratch/tu-split-1884/{gm_1884.c,gm_1884.o,diff.json}` + wave-9
exp report. **Detection tell**: target ha/lo fold-anchor = unit-internal
.bss/.data symbol at NONZERO section offset while ours anchors `...bss.0`
with uniformly shifted d-offsets. **Split queue**: ✅ gm_1884 LANDED
(wave 11, committed 268945dce); ✅ **gmregclear LANDED + COMMITTED
(wave 12)** — 3 splits / 4 TUs, commits 64f0dedfb (gm_182F.c) +
e079c36ff (gm_181A.c) + 831a72891 (gm_180A.c), every-section constraint
0 violations over 245 refs, all gates PASS each split (DOL OK + dtk
shasum + deps#3 + 107 fns preserved 80+10+15+2 + idempotence), project
matched 72.028→**72.042**, gm_182F unit fuzzy 100.0; anchors
gm_80181998 99.46→**100.00 EXACT**, fn_80180C60 98.07 (E48-fold addi
removed), fn_80181E18 ~16 d-forms fixed (display dip 92.66→92.08 is
all-or-nothing row scoring over byte-strictly-better code; residual =
pre-existing rotation park), gm_80182174 +3 d-forms, all other moved fns
byte-IDENTICAL; @-pin renumber map (address-keyed) in
`campaign/scratch/tu-split-gmregclear/REPORT.md` → naming-9 queue.
**Skipped deliberately**: gmregclear part0 merges ≥4 more original TUs
(@847/@849/@294/@295/@550 dup-literal evidence) — anchors already match
via offset-0 shadow/hoist idioms; new cuts would move named-anchor
statics to offset 0 (idiom-46 shadow) and REGRESS 100% fns; interior
boundaries underdetermined. ✅ **particle LANDED + COMMITTED (wave
13)** — 2 splits / 3 TUs, TU identity proven by DOL strings (head =
debugconsole_main.c TU 89 fns; hsd_3983.c = original particle.c TU 15
fns; hsd_39D1.c = generator.c TU 15 fns); commits 11be7f00c
(hsd_39D1) + 2e522ed29 (hsd_3983) + 572614b9c/b701858cc
(binary-proven reader-slot/literal/assert fixes: hsd_8039F05C
78.41→**96.33**, psInitDataBankLoad 96.04→**99.86**, 9930C slot fixes
×5 + 100.0F conditional-kill, DAD4 eps 0.001F, generator.c/psCamera
assert strings via idiom 27); ALL gates PASS at EVERY step (DOL OK +
dtk shasum + deps#3 + no-op idempotence; 119 fns preserved 89+15+15),
project matched 72.062→**72.100** / fuzzy 97.537; check_cuts.py 682
ok + 22 dup-ok / 0 violations; cuts: A|B .text 0x803983A4 / .bss
0x804D08E8 (the demanded offset-0 base; head gets NO extab), B|C
.text 0x8039D1EC / .bss 0x804D0F90 (full 8-section tables in
tu-split-particle scratch). End-to-end gate 12 UP / 7 DOWN / 0
GONE/NEW — every DOWN = @-pin renumber drift; address-keyed,
value-verified maps in `campaign/scratch/tu-split-particle/
progress.txt` → naming-10 queue (@2723 + lbl_804DE98C BLOCKED; head
dup copies keep pins per idiom 54). NOTE: required a 5-symbol
scope:local→global flip in symbols.txt — dtk-shasum byte-PROVEN safe
(narrows idiom-75's negative; see idiom 80). The wave-9 CE3F8/D08E8
both-demand-offset-0 paradox is RESOLVED: D08E8 got its own TU base;
13-symbol bss block + CF7E8 extern siblings verified intact.
✅ **gm_1601 LANDED + COMMITTED (wave 15)** — 2 splits / 3 TUs,
reverse address order, baseline 2cd6e2ad1: commit 3504f3639
(gm_16A9.c, split B|C: .text 0x8016A92C / .bss 0x8046B668 / .sdata2
0x804DA294 — all object boundaries; the 4B "gap_11" end is a real
cut, dtk re-displays gap→pad; lbl_8046B668 → own TU at .bss offset
0; gate 456 SAME / 2 UP / 0 DOWN) + commit 2226073a1 (gm_1693.c,
split A|B: .text 0x80169364 — primary cut used, 0x801692E8 fallback
NOT needed — / .bss 0x8046B488 / .sdata2 0x804DA288; the
byte-confirmed +0x110 fold anchor lbl_8046B488 lands at offset 0;
gate 455 SAME / 4 UP / 0 DOWN / 0 GONE/NEW). ALL gates PASS ×5
checkpoints (DOL sha1 08e0bf20 + dtk OK + deps#3 + idempotence ×2 +
ZERO @-pin drift + symbols.txt untouched, 371-line canary slice
identical to baseline); 226 fns preserved (184+35+7).
**gm_8016A164 99.50→100.00 EXACT**, gm_8016A22C →76.80, fn_8016A4C8
→89.53, gm_8016AC44 →66.77, [.sdata2-0]→100 (TU-B pool byte-EXACT:
1.0f/0.0f/−1.0f at 0/4/8); unit matched_code 63.369→63.659.
PREDICTION CORRECTION: fn_801695BC 70.22 + fn_80169A84 74.20 did
NOT recover — byte-diff shows pure S1 residuals with NO anchor rows
(wave-13 recovery list overstated them). Src pattern: gm_1693.c
carries its own lbl_8046B488_t struct copy — head TU field-accesses
through fn_80169364()'s getter pointer so the def must be visible
to BOTH TUs (idiom 89) — plus moved gm_80164840/gm_80169238
noinline wrapper chains (idiom-83 per-TU clones). Artifacts:
campaign/scratch/tu-split-gm1601/progress.txt.
✅ **gm_18A5 LANDED + COMMITTED (wave 16)** — 2 splits / 3 TUs
(gm_18A5 49 fns + gm_190A 41 + gm_1965 39), reverse address order,
baseline d5eb5b810: commits 486077b8f (gm_1965.c, B|C) → 437d3432d
(gm_190A.c, A|B) → daedf29c7 (ASCII fixup). Both wave-12 pend-flags
RESOLVED: simultaneous-partition 0 local cross-part refs (469 ok +
80 dup-ok + 288 legal scope:global externs; scope-aware checker
tu-split-gm18a5/check_cuts.py); every cut = a symbols.txt object
start. **FINAL cuts — 3 corrections vs the w12 LOG, all
binary-proven**: .text 0x80190ABC | 0x80196510; .data 0x803D9F80 |
0x803DA0D0; **.bss 0x804799B8 | 0x804799D8** (BOTH w12 candidates
wrong — TU-A owns {473AB8, 771B8, 771C4, 77738}: the
fn_8018FBE0/FA24 anchor gm_804771C4 sits at lbl_80473AB8+0x370C =
defining-TU fold fingerprint, idiom 97; TU-B owns ONLY lbl_804799B8
at offset 0); **.sdata 0x804D4170 | 0x804D4190** (triple @354/@355
dup copies = one per TU); .sbss 0x804D6638 | none (TU-C empty);
.sdata2 0x804DA6E8 | 0x804DA7E0 (gap_11 ends TU-B). ALL gates PASS
at every checkpoint ×5 (DOL sha1 08e0bf20 + dtk OK + deps#3 +
idempotence ×2 + 322-line symbols.txt range slice byte-IDENTICAL,
idiom-79 clean; TU-A @-table strict subset of baseline, 0 adds);
129 fns preserved (49+41+39). Final controlled gate: **276 SAME /
29 UP / 39 DOWN / 0 GONE / 0 NEW** — every DOWN ≤0.97pp,
row-verified @-pool renumber drift (fn_80193308 = 100% @NNN reloc
rows); byte canary 116/129 identical, all 13 diffs score-positive
heals: fn_8018FA24 +4.8, fn_801953C8 +4.6, fn_80191FD4 +4.0,
fn_8018F888 +1.8 (the parked "+0x20 artifact" family HEALED),
fn_8019A158 +3.0, fn_8019AF50 +1.4, fn_80194F30 →99.98, [.sdata-0]
64→96, dup-pins @354/@355/@357/@785/@991/@1620 →100. Project
72.103% matched (sibling flux in-flight; per-symbol gates
authoritative). ✅ **naming-13 COMMITTED d17be7f73 (wave 17,
+61/−61): 26 fns→100** (16 TU-B + 10 TU-C) + 19 partial UPs —
the 25-30 prediction CONFIRMED; the queued TU-C magic pair was
VALUE-CROSSED (positional transcription) — applied corrected
@357→@416 / @1620→@320, inverse of the queue (idiom 103). TU-C
extras @3554/@3555 deferred to naming-14 (sibling recon was
mid-flight). Type-size deviations surfaced →
header queue: gm/types.h TmData 0x56C→0x574 (+8) / MatchEnd
0x227C→0x2280 (+4) tail fields — **wave-17 adjudication: TmData
APPLIED (gm_804771C4 →100); MatchEnd +4 REFUTED** (dtk
gap-inclusive pad, idiom 101). **SPLIT QUEUE now**: lb_00F9
low-prio (fn_80013614 SECFOLD evidence) is the only remaining
candidate — no verified front; serialize any split vs naming-13
AND the dirty wave-16 trees at the wave boundary.
DOL-safe for ALL
NonMatching units. Caveat: land splits.txt + src halves together —
splits-only temporarily drops the tail functions from report.json.
**Recipe refinement (wave 12)**: land multi-cut splits ONE AT A TIME in
REVERSE address order so each new TU's .bss base finalizes immediately
(no transient wrong-base states; head keeps its base throughout).

**EXECUTION RECIPE (wave-11 validated end-to-end on gm_1884 — use for
the rest of the queue):**
1. Find the boundary via the detection tell above; the new TU's .bss base
   symbol is the cut (gm_1884: lbl_80473700 = old base+0x158).
2. Derive ALL section cuts: partition every per-function symbol ref per
   section; part1-max < part2-min must hold in EVERY section
   simultaneously; every cut must land on a symbols.txt object boundary
   (grep-verify). If a scratch-split diff.json exists, target-side
   section sizes give the cuts directly (end − size).
3. Land the TRIO together: (a) splits.txt two entries; (b)
   `Object(NonMatching, ...)` in configure.py preserving link order;
   (c) src split at the exact static-decl line of the new TU's .bss
   base — new file = head includes + extern decls for head-declared
   statics the tail reads (externs are @id-free if already extern) +
   tail verbatim.
4. Rebuild: `python3 configure.py --no-warn-error && touch
   config/GALE01/config.yml && ninja`; VERIFY `ninja -t deps
   build/GALE01/config.json` #deps ≠ 0, `main.dol: OK`, `dtk shasum -c`
   PASS.
5. Gate with controlled per-symbol objdiff-cli snapshots before/after —
   **gate on `match_percent`; `fuzzy_match_percent` is null in this
   objdiff build's JSON and gives a false all-SAME**. Expect: anchor
   rows vanish; @NNN naming churn on functions whose literals
   repartition into the new pool — enumerate each as reloc-name-only and
   queue the symbols.txt renumber (idiom 23/54); duplicate-@ pins
   (idiom 54) may split across the two units — recount per unit.
6. NEVER run a split concurrently with a naming round touching the same
   unit's address ranges (wave 11 was safe only because naming-7's edits
   were all mplib-range).

## ⭐ DATA-RECON VERDICT (w16 pilot PROVEN; w17 ×2 TUs; w18 ×2 more + FLIP-ENABLER)

Dead/placeholder .data IS reconstructable from target bytes at near-zero
@-id cost: numeric brace-init arrays consume **0 @-ids at any position**
(89KB EOF + 7.7KB top-of-file both proven — idiom 98; only STRING-literal
initializers tick +1, refining idiom 90). Pilot results (wave 16, trees
UNCOMMITTED, no config edits, symbols.txt md5 unchanged):
- **sislib [.data-0] 6.43→98.47** (.data BYTE-IDENTICAL, 0x1ec→0x3af0);
  CB00/CD40/C680/C8C0 →100 ×4; @698/@1197 50→92.6/81.5; analyzer
  NAMEPAIR_LC+MIXED cleared ({SHADOW:6, OK:158, LIT2EXT:1} = by-design
  parks); bytegate 32/34 byte-identical (only the 2 fix sites differ);
  21/21 @-syms zero drift. Binary-proven type fix: CB00 is a **u8
  kerning table** (target lbzx + 0x4330 magic; our f32 deref emitted
  lfsx at ×8 scale); 84BC `(u32)pass == 2U` →91.20 (header verify
  queued); 8134 size target-EXACT 0x388, reloc debt 0 (display −0.49 on
  byte-strictly-better code — S1-rotation row-scoring class).
- **particle [.data-0] 33.31→78.75** (head 0x1e88 byte-identical; .data
  0xddc→0x2c64): lbl_80408630 u8[0x268] (was `static u8[1]` sbss
  placeholder), lbl_80408898 GlyphEntry[4], lbl_804088B8 non-static
  u8[0x1c00]; 267/267 @-syms zero drift; DrawASCII →90.34, 21B8/22FC
  NAMEPAIR_LC leftovers cleared; .sbss → target sizes both units.
Recipe: placeholders → `extern u8[N]` decls + EOF brace-init defs from
target bytes; entry-bound named pointer locals pin materialization rank
(8134 gets target's r30 exactly); ≤8B zero objects CANNOT reach .data
from C (`static u32 x=0`→.sbss, `x[1]={0}`→.sdata) — inter-TU pads must
materialize via ALIGNMENT of the next object (u64 element type consumed
sislib's C564 pad byte-exactly); defining a previously-extern GLOBAL
keeps named relocs (refines idiom 46 — the fold law is statics/anchors
only); .sbss→.data table conversion flips sda21 refs into entry-hoisted
lis/addi (= target shape). Tooling:
`campaign/scratch/datarecon-w16/{elf,align_data,gate,unit_state}.py` +
baselines under `base/`; durable record datarecon-w16/progress.txt.
DOL gate was DEFERRED to the wave boundary (siblings live mid-wave;
idiom-79 re-split risk) — full ninja + shasum over the dirty trees
before ANY commit. **SCALE QUEUE**: particle E80 string package (9
invented texts + decoded true texts + `reset`-helper inline hypothesis
proven by pool order — land as ONE unit) → dead cluster AB00..ADC4
(ADC4 0xaac/200 relocs REQUIRES the .sdata pass: tgt 0x2e8 vs ours
0xb9, ~40 key-name strings, Fix-A dup deletions 62D8../BF10) →
BAF0/BC3C .bss→initialized-.data structs → BF70 content + BF48/BF5C
(u64-align trick) → @901-3 order swap + USB-loader strings + invented
texts @1181/83/@1198/@1794/@1931/32/@1991 (A540-cluster +4 alignment
PARK; fn_80397814 VIGetRetraceCount ×22 ours-only → S1); ✅ **gm_18A5
TU packages EXECUTED wave 17 (both UNCOMMITTED)** — gm_190A [.data-0]
54.24→**94.84** (content == target; 5B dtk gap_07 tail park) +
[.sdata2-0] →86.18 + lbl_803D9F80 + 14 positioned const defs →100 +
**gm_801963B4_OnEnter →100.00 MATCH**; gm_1965 [.data-0]
3.72→**99.91 byte-IDENTICAL** + 39 syms→100 (lbl_803DA0D0 0x184 +
6 dead strings + 32 const f32 EOF defs) + @697 10000.0f pool →100
— **@3554 UNBLOCKED**; binary-proven f32 truths flagged in-source
(738=12.800008f/414cccd5 NOT 12.8f, 73C=2.6200008f, 754=7.1899995f,
78C=0x46DC46FF s32); scale law: Fix-B const scalar defs are 0-id at
any position but must emit AFTER all uses (idiom 104), and some
target stream heads are C-UNREACHABLE — dead/deleted early pool
creators (0.0f pool-head, both TUs; idiom 105) — park, don't fight;
gmresult idiom-69 package (ours 0x198 vs tgt
0x580 + positioned .sdata pass); ✅ lbaudio package LANDED wave 16;
tydisplay B1FC/B328/B460 idiom-69 map (tydisplay-w14/REPORT.md);
ftcoll [.sdata-0] dtk-tail park stands.

**⭐ WAVE-18 SCALE ×2 (ftcoll + gmregclear family; trees UNCOMMITTED
except gm_182F.c which rode flip commit 51951826b)**:
- **ftcoll** (durable: datarecon-ftcoll-w18/REPORT.md): [.sdata2-0]
  81.25→**100.00 byte-IDENTICAL** — the w14 permutation park CRACKED:
  the u32→f32 magic (4330... +0x28) is pooled by a fully-inlined,
  linker-DEADSTRIPPED static helper; an UNCALLED `static f32` helper
  redefined at the original lexical slot pools the orphan at its
  DEFINITION slot — zero text change to all 77 fns (idiom 111, extends
  105). Bonus **ftColl_8007861C 79.44→100.00 MATCH** (entry-read
  `victim->dmg.x18c4_source_ply` semantic-bug fix + ternary x18C0 +
  arg6 int→u16 in ftcoll.h + diamond-init/ternary-demotion webs +
  PAD_STACK(8)); getEnvDmg ours-only park DISSOLVED — w14's
  "structural" park was a missing `static inline` (idiom 112).
  Unreachable-bytes ledger CLOSED (.data 4B + .sdata 6B = dtk tail
  pads only; .sdata2 zero). 17 value-verified renames → naming-15
  (SERIALIZE vs any ftcoll edit). .text leftovers enumerated → S1
  (77C60 frame+8, 7BE3C missing u32 conversion, decay FPR rotations).
- **gmregclear family** (gm_182F/gm_181A/gm_180A/head; durable:
  datarecon-gmregclear-w18/progress.txt): gm_182F made whole-byte-exact
  (math_ppc.h ballast kill, idiom 114a + in-TU `u8 lbl_804D65E8;` .sbss
  def) → **FLIP-ENABLED and FLIPPED**; gm_181A [.data-0]
  missing→**99.81** (u8[0x480] table + 7 byte-proven-dead kumite
  strings — reached ONLY via `addi rX,rBase,0x480..`, ZERO relocs;
  .text byte-IDENTICAL); gm_180A [.sdata2-0] 80→**100** + lbl_804D65C8
  family .sbss ×4→100; head [.data-0] 6.28→**96.15** (2 typed stage
  tables, 0 relocs) + .rodata defined (93.33) + **whole .bss layout
  target-exact 10/10** via 2 binary-proven size fixes (gmClassic_
  80472AF8 u8[0x110]→StartMeleeData 0x138; lbl_80472CB0 →0x78 prefix
  struct). Head's single DOWN (fn_801803FC −0.85) root-caused
  C-unreachable (idiom 63; baseline "match" was an offset-0 shadow).
  Head gate 171 SAME / 10 UP / 1 DOWN; full ninja DOL sha1 EXACT.
- **SCALE QUEUE re-rank (flip-aligned, front first)**: ✅ **hsd_40FF
  DATA-ONLY recon EXECUTED + FLIPPED wave 19 (ab2a4870d — first
  data-only flip; needed idiom-116 force_active)**; head NAMING+SRC paired
  package (early-inline @1427-29 string-trio reconstruction,
  particle-E80 class — recovers most of [.data-0]'s last 3.85pp);
  particle E80 string package; dead cluster AB00..ADC4; BAF0/BC3C;
  gmresult idiom-69 package; tydisplay map. gmallstar future flip
  needs gmClassic_80472AF8 static→global (noted in-source).

## ⭐⭐ FLIP VERDICT (w18 ×3 + w19 ×2 + w20 ×1 = 6 FLIPS COMMITTED; THE ENDGAME METRIC)

**The flip recipe is PROVEN.** Wave 18 flipped the first three units to
`Object(Matching, ...)` — ninja links OUR compiled objects into the DOL,
byte-proven: sha1 08e0bf20… IDENTICAL + dtk OK through 6 full ninja
passes including a mid-gate naming-14 symbols.txt re-split (idiom-79
rewrite observed, none in flipped ranges); main.elf link-edge verified
consuming `build/GALE01/src/<unit>.o` for all three; PASS ×3 +
idempotence ×3. Commits: **51951826b** (melee/gm/gm_182F.c — one
recon-supplied .sbss def — + melee/pl/plattack.c, ZERO src work) +
**4715a41d8** (melee/ft/ftchangeparam.c, ZERO src work). Project
836→**839/1041 linked (42.35→42.62%)**, matched 72.13→**72.15%**, fuzzy
97.58→97.59.

**Settled questions (byte-proven at link; full pack = idiom 110)**:
- ⭐ **S5 / pre-session-2 experiment #2 ANSWERED: naming-only functions
  ARE byte-correct in a linked Matching unit** — ftchangeparam linked
  byte-exact carrying 219 LIT2EXT + 6 SHADOW rows. AT_RENUM/NAMEPAIR/
  LIT2EXT rows are link-IRRELEVANT at equal resolved offsets; Grand
  Inversion / naming is SCORE work, not byte work, for flips.
- dtk gap-tail parks SURVIVE a Matching link: short .sbss tails restore
  via the next contribution's align=8 (gm_182F, plattack) and short zero
  .data tails via the next object's alignment zero-fill (ftchangeparam)
  — answers the queued gm_190A question: its 5B/4B parks are NOT flip
  blockers.
- `_half/_three` weak .sdata2 surplus is linker-stripped AND compacted
  (extends idiom 49 to link-proven).
- Renamed/missing target-named local exports are link-neutral iff zero
  importers across all 1041 link inputs (verified per-symbol).

**WAVE-19 ADDITION — flip #4 COMMITTED ab2a4870d**: hsd_40FF
NonMatching→Matching, the first DATA-ONLY flip (11199-line u8[0x20BC0]
brace-init recon, idiom-98 class, zero @-ids; .data/.symtab/.comment
byte-IDENTICAL). 839→**840/1041 linked**. First attempt FAILED — **mwld
DEADSTRIPS compiled zero-importer objects** (DOL shrank exactly 0x20bc0);
dtk-extracted objects survive via the MW .comment per-symbol force-active
flag 0x08, MWCC emits 0x00; cure = `#pragma force_active on` (idiom 116).
**Wave-19-boundary — flip #5 COMMITTED d4fee40d1**: itdrop (rode the
wave-19 src commit; it_8026F3D4 →100 via idiom 117). 840→**841/1041**.
W19 recon-verification (w20): hsd_40FF re-proven end-to-end — full
0x20bc0 target-byte readback, fresh-compile sha identical, linkcheck
`[]`, touch-forced relink DOL EXACT; do NOT re-flip.

**WAVE-20 ADDITION — flip #6 COMMITTED b6674faf6**: sysdolphin/baselib/
bytecode NonMatching→Matching, **ZERO src work**. 841→**842/1041 linked
(42.80%)**. It had sat flip-ready ~2 waves buried under 26 zero-importer
EXPORT-MISSING rows — surfaced only by the new FULL-RANK sweep with
en-masse importer closure over all 1041 link inputs
(`campaign/scratch/file-flip-w20/rank.py`, idiom 132; sweep covered all
199 NonMatching units — bytecode was the ONLY flip-ready). Deadstrip
audit PASSED WITHOUT force_active: .text kept via HSD_ByteCodeEval
(imported by robj.o), .data/.sdata/.sdata2 reachable via intra-object
.text relocs (idiom 134, narrows 116). Plus 2 ALIGNFIX tails + weak
`_half/_three` surplus (idioms 110/49). NEW FLIP LAW (idiom 133,
inverse of 88): unit fuzzy 100.00 does NOT imply link-clean — two
fuzzy-100.000 units fail linkcheck on .sdata2 POOL ORDER alone
(ftPr_SpecialN, itlinkbomb — now the cheapest flip-unblocks in the
queue, zero .text bytes).

**GATE INSTRUMENT**: `campaign/scratch/file-flip-w18/linkcheck.py`
(resolved-reloc equality + weak-exempt + ALIGNFIX + export-importer
closure) — gate flips with it, NOT fuzzy/analyzer counts (extends the
idiom-88 hard rule to flips). ⚠ linkcheck.py does NOT model mwld
deadstrip — run the idiom-116 force-active audit (zero-importer exports
need the .comment keep bit) before gating every flip. Then `pgrep` for
sibling builds (build-race law), full ninja + `main.dol: OK` + dtk
shasum + idempotence. **FLIP-GATE LAW (all streams)**: a flipped unit's
ours-object is LOAD-BEARING — naming/recon/S1 gates must re-baseline the
DOL sha, and any edit to a flipped TU must keep it whole-byte-exact.

### ⭐ Ranked flip queue (wave-21 results, live-linkcheck; flips #1-6 COMMITTED)

**Flipped (6)**: gm_182F + plattack (51951826b) · ftchangeparam
(4715a41d8) · hsd_40FF (ab2a4870d, data-only) · itdrop (d4fee40d1) ·
bytecode (b6674faf6, zero src work). Importer closures all re-verified
at flip time. Re-rank after every recon/S1 wave with
`file-flip-w20/rank.py` (en-masse importer closure mandatory, idiom
132 — top-10 cutoffs buried bytecode for ~2 waves; w20b re-run
confirmed COMPLETE coverage: 197 ranked + 2 src-less data containers
= all 199 NonMatching link inputs, zero latent zero-work flips left).

1. **ftPr_SpecialN — FLIP-READY (w21, uncommitted src)**: linkcheck
   hard rows **NONE**; whole-object .text AND .sdata2 byte-exact vs
   target. Pool swap cured src-side via named-def slot interleaving +
   `*(f32*)&` cast reads + EOF 0.0F gap def (idioms 146/147/122).
   Residual = 1 ALIGNFIX + 19 zero-importer EXPORT-MISSING (link-
   neutral, idiom 110d). Flip gate to run: rank.py re-run + idiom
   116/134 deadstrip audit + pgrep + full ninja + DOL sha + idempotence.
2. **itlinkbomb — FLIP-READY (w21, uncommitted src)**: linkcheck hard
   rows **NONE**; .text untouched byte-for-byte — top-of-file 2.0F/0.5F
   defs un-rotated all 10 pool entries (47 relocs in one shot) + EOF
   0.0F tail = size 0x30 EXACT. Residual = 3 ALIGNFIX + 14
   zero-importer EXPORT-MISSING. Same flip gate as #1.
3. **MSL/math — FLIP-READY (w21, uncommitted src)**: linkcheck **`[]`
   LINK-CLEAN** — logf 99.80→**100.0000 MATCH** (the w19 ~50-probe park
   SOLVED via the scope-overlay frame law, idiom 143: inline switch
   head + outer decl band + case-block kept verbatim + lone `float H` +
   `(float)` contraction blocker + `F32_BIT_CAST` reads). logf has
   importers ⇒ idiom-134 covers data sections, no force_active. SIDE
   ITEM still open (math_data.c owner): delete lines 82-83 — dead
   `__logf_C0/C1_bits` defs, C1 hex 0x3EAA9F44 = binary-proven
   transcription error for 0x3EAAAA36 — makes math_data.o
   byte-identical.
4. **gm_1BFA** — **20B .text, now SINGLE hard row** (w21:
   EXPORT-MISSING gm_8049E558 cleared via static→global, idiom 139;
   .text/.data/.bss byte-identical). gm_801BFCFC 99.28 residual = ONE
   r27↔r28 transposition; w21 resolved ALL 4 queued ideas (A worse,
   B/B2 no-op, C deadlocked, D falsified) + 23 decl perms — but
   solve2.py proves the target map IS pure-L1-reachable (226 orders,
   anchor AFTER w1): blocker is the processing SLOT (idiom 140); pends
   a slot-inverter (p09/p10 anomaly = only observed mechanism).
5. **grgreatbay** — 44B→**31B** (w21, uncommitted src):
   **801F63F4 →100.00 MATCH** (SR-rover recon + predicted decl reorder
   + chain-assign; idioms 149/150/33); 801F5460 97.30→**98.73** (idiom
   34 self-reads; residual = ONE jobj/gp r29↔r31 transposition,
   slot-law park) + 499C 2 rows (helper structure right, idiom-153 8B
   charge blocks) + 4300 synthetic-reloc 0B (idiom 45).
6. **itdraw** — 37B .text (src reverted byte-identical): the lost w19
   99.26 uniform state REBUILT + recipe saved
   (cs-law-w21/itdraw_V7_99.2628.c) — pure global r28↔r29; ALL
   idiom-130 levers inert; target = comp1 processed LAST (slot law,
   idiom 140) ⇒ pends the same slot-inverter as gm_1BFA. GPR-side: L1
   applies (NOT the FPR variant).
7. **sobjlib** — 39B, HSD_SObjLib_803A4A68 99.89 — parked w21 with
   mechanism gains: target frame = one descending per-site stream of
   [arg-copy above staging] groups, reproduced EXACTLY by 11 local
   static-inline GX wrappers (prototype in s1-flip-w21 scratch) but the
   stream bottom-pins at 0xc — the 0x28 dead-bottom fill is the
   cobj/lbColl parked family; FPR trio = idiom-121 rotation.
8. **quatlib** — 25B .text ONLY, **DEMOTED below the workable tier**
   (w21): EC4C FPR pair swap is C-UNREACHABLE-class with mechanism
   proof (idiom 145; E1/E2 fingerprint probes in
   cslaw-parks-w21/progress.txt) — every C path to the cross-statement
   anonymous y-half lifetime is closed. Do not re-attempt from C.
9. Tail: itpikachuthunder 21B + 10B .sdata2 + 3 reloc · lbmemory 79B +
   2 reloc · ftmetal −0x10 + 20 reloc · 3-hard tier (vi0501, ifprize,
   lbshadow, leak, itkyasarin). **lb_00CE stays DEMOTED** (24B
   but dead-creator pool head, C-unreachable — idioms 118/148; D148
   12-block shape 99.06 → S1 rotation). **gm_16A9** refuted-easy (w19:
   real codegen + non-restorable .sdata2 shift). **ifprize** 34B + 12
   reloc + 4B .data tail = REAL content. **ftCo_Shouldered** 8B
   mid-.text insertion + non-weak .sdata2 surplus = real.

Family follow-ups (wave-17 list re-ranked by wave-18 results): gm_180A
next-best in the gmregclear family (only fn_80180C60 98.17 blocks);
gm_181A pends 3 S1 fns; hsd_39D1 pends 2 S1 rotation parks (DAD4/9F05C);
gm_190A's enumerated set shrank (935B8 →100 wave 18; gap-tail parks
cleared by ALIGNFIX — re-rank with linkcheck.py: E6C byte-load-bearing
park + 90ABC/91240 remain); gm_1965 a wave behind; gmallstar pends
gmClassic_80472AF8 static→global. Re-run `file-flip-w20/rank.py`
(supersedes the w18 rank_raw sweep) after each recon/S1 wave.

## ⭐ ROTATION-LAW VERDICT (wave 20 — the allocator CHOICE rule is SOLVED)

After 8 falsified models (waves 12-19) plus ~12 more falsified this
wave, the rotation-enum agent derived the law pack via a synthetic-probe
harness (`campaign/scratch/rotation-enum-w20/`: probe.sh, probegen*.py,
search*.py, solve*.py, 70-entry search_log.json — real cflags + dtk
disasm, ~1.5s/probe; solve2.py reconstructs processing orders from any
new reg map). Validated against 7 independent colorings by exhaustive
order-search; full text in rotation-enum-w20/REPORT.md.

- **L1 — CHOICE RULE (SOLVED, unique fit; = idiom 128)**: at each web,
  scan registers CIRCULAR-DESCENDING starting AT the cursor (the reg
  assigned to the previously-processed web), DIRTY (already-used) regs
  first; take the first whose holders all don't interfere; else the
  highest VIRGIN reg. "Chains" = consecutively-processed disjoint webs;
  the campaign's "rotation parks" are cursor artifacts.
- **L2 — LOW PRESSURE (≤7 CS webs)**: processing order = anonymous LICM
  temps (creation order) then named locals in DECL ORDER — re-validates
  and EXPLAINS idiom 1.
- **L3 — HIGH PRESSURE structural promotions (= idiom 131)**: whole-fn
  multi-def webs first (decl tiebreak), loop-LICM consts ~3rd, per-BB
  local pass for call-result temps; explains the old 24-perm cap.
- **L3c — ANCHOR-PAIR LAW (= idiom 129, extends 126)**: the anonymous
  @ha/@l anchor allocates immediately BEFORE its first copy-derived
  named web (one-above-the-walker in every perm); anchor-below-walker
  targets are decl-UNREACHABLE — the exact gm_801BFCFC blocker
  (deadlock proof in the report).
- **L4 — CHAIN STEERING (= idiom 130, the actionable S1 lever)**:
  declaring the LATER loop's idx first rebinds the earlier idx onto its
  register chain (won +0.22 real on gm_801BFCFC; +1.44 total with the
  reorder).
- **L5 — TU-CONTEXT SENSITIVITY (hard probe limit)**: an identical
  standalone replica colors DIFFERENTLY from the in-TU original
  (static-vs-extern flips maps; PAD_STACK inert) — probes give LAWS,
  not byte answers; final probing must run on the real unit.
  **W21 SHARPENING (idiom 142)**: L5 sensitivity is linkage/symbol
  context ONLY — function TU-position and vreg counters falsified
  (1-3 dummy fns incl. a 15-CS-web monster prepended → coloring
  byte-identical; extern-buf reshape is what moves maps).

**⭐ W21 SLOT-LAW REFINEMENT (idiom 140, supersedes the "anchor-pair
decl-UNREACHABLE" framing of L3c/129)**: solve2.py order-reconstruction
shows one-transposition park targets ARE pure-L1-consistent — gm_801BFCFC
has 226 valid orders with the anchor processed AFTER w1; itdraw's target
processes comp1 LAST. The blocker is the processing SLOT, not the choice
rule. Decl order, comma/for-init, block scope, variable splits, inline
decl/definition order, and TU position are ALL slot-inert (23+ perms +
4 queued ideas resolved w21). The class pends a SLOT-INVERTER; the
p09/p10 probe anomaly is the only observed mechanism — next probe:
vary statement forms between walker init and loop head on a
real-fn-shaped probe. The 226-order corpus (cs-law-w21/solve2.py) is
the spec — cross-check new reg maps against it before burning compiles.

**⭐ FPR-LAW VERDICT (w21 — L1 does NOT transfer to FPRs; idiom 145)**:
volatile FPR expression temps = fresh VIRGIN regs DESCENDING from the
highest free volatile, in IR creation (statement) order — NO cursor, NO
dirty-first. Adjacent result fadds coalesces dest=src2. Named
single-def multi-use f32 webs → ascending band f7+ (overflow CS f31);
named MULTI-def f32 webs → L3-promoted ahead of loads (idiom 131
transfers to FPR named webs, closing the staging route). Consequence:
quatlib EC4C's cross-statement anonymous y-half lifetime is
C-UNREACHABLE (E1/E2 fingerprint proof, cslaw-parks-w21/progress.txt) —
single-use copies VN-forward (121), multi-use/multi-def named webs
band/promote. FPR pair parks are a DIFFERENT mechanism from GPR cursor
parks: itdraw's r28↔r29 tie is GPR-side (L1 + slot law apply); DAD4 /
sobjlib FPR trio / EC4C are the FPR family.

**Practice**: re-judge every parked "rotation family" under L1-L5 + the
slot law + the FPR variant before burning compiles. Rotation queue:
slot-inverter mechanism probe (p09/p10, real-fn-shaped) — unlocks BOTH
gm_801BFCFC AND itdraw (V7 recipe = one copy once it exists) ·
grgreatbay 5460 r29↔r31 transposition (same class) · lb_8000D148 ·
lbaudio 27488 entry-exchange (exact target map in
lbaudio-s1-w20/progress.txt) + 27DF8 r0/r4 · 69A84/gmregclear
carryovers. CURSOR-SIM FIRST at ≤7 webs (idiom 150: hand-simulate
L1+L2 before probing — first fully-predicted S1 win at grgreatbay
63F4). Falsified-on-real-fn list (do not re-probe): named-base
packages (94.28/97.90/97.65), variable merges (96.75/98.03), quatlib
EC4C any-C-form, gm_801BFCFC decl-perm space.

## Validated playbook (update after every session)

1. **Declaration-order rule (VALIDATED, win #1)**: MWCC assigns callee-saved
   registers to local variables in pure LEXICAL DECLARATION ORDER, descending
   from the highest free register (after params/earlier decls consume theirs).
   Inner-block decls just continue the sequence. To fix a register permutation:
   read the target's register assignment per variable, derive the declaration
   order that produces it, reorder/move declarations (hoisting inner-block
   decls to the outer block is legal and was the original style in win #1).
   Evidence: fn_803ACD58 (hsd_3AA7.c) — target wanted retries,buf,offset,size
   = r25,r24,r23,r22; hoisting buf+offset above size matched 100%.
2. **check_fn scratch mode**: direct compiles go to campaign/scratch/ and diff
   via explicit -1/-2. NEVER write build/GALE01/src/*.o outside ninja — it
   poisons incremental state (ninja skips rebuild, extab-clean never runs,
   phantom .data regressions appear). If canonical objects are suspect:
   `find build/GALE01/src -name '*.o' -delete && ninja`.
3. **extab caveat**: units compiled with mwcc_extab rules get a `dtk extab
   clean` post-step under ninja. Scratch compiles of those units may show
   phantom extab/data diffs — judge CODE rows only; gate with ninja+land.py.
4. **Float-rounding artifacts**: backlog stores fuzzy to 2 decimals; a "drop"
   from 92.26 to 92.25676 is the same value. Only sub-rounding deltas are real.
5. **Schedule-move tell**: if an edit changes instruction ORDER (not just
   registers), it changed IR shape — usually wrong; revert unless the target
   also shows that order.

6. **ARG-PRECOLOR rule (won it_802A850C, it_802C53F0)**: when a condition's
   CSE temp lands one volatile register too high (r4 vs r3), check whether
   the target shows NO arg-setup before the branch's first `bl` for that
   slot — the original passed the CSE'd expression as a (possibly unused)
   extra argument, precoloring the temp. Codebase precedent:
   it_802B5CBC(gobj, unused) in itsamuschargeshot.c. Needs header edit
   (orchestrator applies).
7. **STATIC-INLINE-HELPER demotion (won gm_8016EDDC)**: a named local pinned
   wrong can be demoted to an anonymous inline-result temp by routing its
   expression through a `static inline` helper called in argument position
   — MWCC hoists the call before the enclosing statement (schedule intact)
   and ranks the anonymous temp differently than a named local.
8. **CAST-VS-MASK (won fn_8016FFD4)**: `(u8) expr` used 2+ times is
   front-end CSE'd EARLY (temp gets a high callee-saved reg);
   `(expr & 0xFF)` defers to loop-invariant hoisting (low reg). Use to
   demote a clrlwi-of-param temp that sits too high.
9. **Definition-vs-declaration decoupling (gm_1601 evidence)**: local-copy
   inits emit in DEFINITION order while registers assign in DECLARATION
   order — splitting `T x = init;` into decl + assignment decouples them.
   Also: the first-read order of two params inside the first switch case
   can globally permute the function's callee-saved assignment.
10. **Copy-prop blocking (itcoll evidence)**: MWCC copy-propagation is
   per-variable and blocked ONLY by a second assignment that READS the
   variable (`x = E op x`); dead re-inits are DCE'd first and don't block.

11. **CHAINED FOR-INIT (won it_80294364)**: 'for (i = count = 0; ...)'
   defeats MWCC's zero-trip-guard constant fold. Tell: target guard
   'cmpw rI,rN; bge' vs ours 'cmpwi rN,0; ble'.
12. **EXTERN ARRAY SIZING (won un_80301964, fn_800F53AC)**: declare externs
   with their true array size from symbols.txt ('extern char x[8];'), not
   pointer/unsized — changes addressing shape.
13. **INLINE-HELPER SUPPRESSES CONST-PROP (won fn_801A94BC)**: var-to-var
   copy-init folds to 'li' in caller body but survives as 'addi' inside an
   inline helper expansion. Mirror matched siblings' helper structure.
14. **LICM-SUNK INVARIANT (won ft_800852B0)**: invariant assignment written
   INSIDE the loop sinks to preheader with different allocation than
   written-outside.
15. **mr-vs-addi singleton (mpLib_80057BC0, codegen-resistant)**: per-site
   compiler-internal; 10+ idioms tested, none flip a singleton propagatable
   int arg move. Classify and skip.

16. **SWITCH IDIOMS (won grOnett_801E40E4, gm_801BE638)**: lone dead
   'b end' between compare tree and first case = empty trailing
   'case X: break;' (X outside tested values); TWO consecutive 'b end'
   before first case = empty case just above dispatch range, placed
   lexically at the dead b. Empty 'default:' is DCE'd — only real case
   values survive.
17. **DEAD 'b epilogue' after call = literal 'return;' in else branch**
   (won it_802B64FC — nested-if shape, not m2c flat gotos).
18. **SELF-READ MULTI-DEF pointer bump** 'p = (T*)((u8*)p + N);' blocks
   coalescing, forces temp+mr init (won fn_80175D34).
19. **ASSIGNMENT-IN-CONDITION** relocates a def site (won ftCo_800D0CBC
   — but REVERTED: broke the sibling that inlines it. SIBLING GATE is
   now mandatory: same-TU functions inline each other; check the whole
   unit after every win).
20. **Label-ID/@NNN shared counter**: labels and literal-pool names share
   one file-wide counter; added control flow shifts all later '@N' names
   (cosmetic naming-row churn — re-verify naming maps after src edits).
21. **THPComponent-class struct fixes**: when lha/sth offsets are uniformly
   +N vs ours, the TYPE layout is wrong (predDC at +6, u8 triple first) —
   fix the header struct, not the code (won Y/U/V decoders).

22. **RELOC-MATCH LAW (gm_18A5 proof: fn_80190520, fn_8018F71C)**: objdiff
   pairs a reloc row iff name-equal OR resolved-section+offset-equal. ALL
   layout/naming work reduces to satisfying one of the two.
23. **@-ID CONSERVATION LAW (refines 20; gm_18A5/particle/lbcollision/mplib)**:
   per-TU @ costs — extern f32 first-ref +1; sbss-int extern ref 0; named
   static 0; CFG reshape ±N; literal→extern swap is id-safe only if another
   user keeps the pool entry alive. Any net change BEFORE the last
   symbols.txt-pinned @id breaks all later pins; drift past already-broken
   rows is free. @NNN drift is recoverable at land time by renumbering the
   unit's pins (tools/replace-symbols) — compute renumbers AFTER src lands.
24. **CONST-FOLD KILLS NAMED .sdata2 RELOCS (lbcollision, byte-proven)**:
   file-scope const float/f64 with visible initializer NEVER yields a named
   reloc — refs pool anonymously, the defs are dead data (MWCC also drops
   unreferenced `static const`, so no padding placeholders). Fix A: delete
   defs, use literals, rename target syms → @NNN scope:local in symbols.txt.
   Fix B (camera, 14 matches): extern-declare in the .static.h and DEFINE at
   END of .c after all uses — refs then emit named relocs. Pick per-site.
25. **EXTERN-CONST vs POOL-LITERAL DICHOTOMY (camera)**: extern f32 loads are
   GCSE/hoisted (entry, high reg, CSE across BBs/calls, no LICM remat); pool
   literals reload per-use into f0, LICM-cache in loops, remat after stores.
   Read the target's load pattern per site. Multi-read extern-f32 swaps
   regress where target reloads (gm_18A5 −2/−3 each); 1–2-read swaps safe.
   NEGATIVE (itdrop F3D4): direct extern-f32 use INSIDE a loop kills the
   LICM hoist (per-use f0 loads) — keep a preheader local copy
   (`f32 zero; zero = ext;`); straight-line uses are byte-identical.
26. **NAMED-ANCHOR vs SECTION-FOLD (extends BSS rule; gm_18A5/particle/
   gm_1601)**: multi-access constant-offset statics fold to merged-section
   base (`...bss.0+N`). Named anchors via: (a) extern-flip the symbol at the
   original TU's .bss base — must be UNDEFINED extern; a defined global still
   folds (particle hsd_804CF7E8/D08E8); per-symbol net-judged: siblings
   reading nearby statics through that anchor regress (lwzu-fold), partially
   recoverable with base-ptr arithmetic off the extern; (b) struct-ptr hoist
   `T* d8 = &sym;` for variable-indexed multi-field access ONLY — hoisting
   constant-offset/single-&field sites regresses (gm_18A5); (c) variable-
   indexed/address-taken access materializes the name for free. Units merging
   several original TUs (gm_1601 +0x110 family) need a configure-level TU
   split — out of agent scope.
27. **HEADER-INLINE ASSERT-STRING DUP (particle, 4 matches)**: target wants a
   named lbl_* string equal to a header inline's HSD_ASSERT literals ⇒ the
   original TU had its own copy. File-local `static char lbl_X[]="...";` +
   `static inline foo_dup()` clone calling `__assert(lbl_file,line,lbl_cond)`.
   Zero @ids consumed — drift-free anywhere. Works only as pool REUSER; if
   the swap removes the TU's pool-emitting literal it shifts ids (blocked).
28. **CLAMP/TERNARY FAMILY (gm_1601 ×3, mplib ×4)**: u32 saturate =
   `(x+1 > 0xFFFFFFFFU) ? 0xFFFFFFFFU : x+1` (preloads else-value; MIN()/
   if-else misallocate). Do NOT CSE repeated `g()` calls a ternary
   re-evaluates — target shows the extra `bl`s. `return x != -1 ? x : -1;`
   moves the var off the r3 precolor and coalesces inline-result defs.
   static-inline bool helpers: if/return body = no frame temp, ternary body
   = +8; u64-typed named locals/returns cost 0x10/0x8 frame.
29. **DECL/ORDER EXTENSIONS (extends 1/9/18)**: volatile scratch webs bind in
   lexical decl order ascending from r3 (gm_1601); adjacent dependency-free
   STATEMENT order drives fpr vreg assignment even with identical schedule
   (lbColl 77A0); mutating a PARAM (vs `int copy = arg`) homes the entry copy
   into the callee-saved reg so the first use reads it (mplib 53DA4/53ECC);
   assignment-in-condition `(r5 = p->f) != -1` kills the lha+mr split before
   a backedge; ~~self-read index-as-address `i = (s32)(base + i*6)` rebinds
   the loop counter to r3~~ DISPROVEN at gm_80164A0C (wave 9: verified no-op
   both ways; true blocker is allocator never reusing just-freed r3 — parked
   rotation family); a dead small-typed local in an
   auto-inlined CALLEE reserves stack words in the CALLER's frame — fold it
   into its use, standalone callee stays byte-identical (mpJointListAdd).
30. **BITFIELD/INDEXING TELLS**: lone `rlwimi rD,rS,0,28,31` = u8:4 bitfield
   write; `(x&0xF0)|(v&0xF)` always pre-masks (fn_80165FA4). Displacement-
   only diffs on indexed loads (0x330 vs 0x338) = 1-indexed table
   `tbl[idx-1].f`, not layout (gm_80167BC8). lhzu/lbzu/stbu fusion needs
   direct indexed exprs `s->arr[i] op=`, not a hoisted element ptr (gm_18A5).
31. **MSL sqrtf FINGERPRINT (lbcollision/camera)**: the inline's `volatile
   float y` takes a 4-byte caller-frame slot per instance, below named
   locals, in instantiation order — slot offsets fingerprint instance count/
   order. `_half/_three` localstatics are const-folded dead data; named-const
   reconstruction misallocates f4/f5. Naming-class, unreachable from C.
32. **GXColor BY-VALUE (mplib)**: passed as pointer-to-copy; literal-init
   locals pass their own address, but locals assigned from globals/array
   elements get fresh compiler temps — named-local reconstruction of such
   call sites fails. Our build pools arg-copy temps function-wide descending
   below user locals; target shows per-block ascending — UNSOLVED gap.
33. **CAMERA MICRO-SHAPES (win-evidenced)**: chained `a=b=c=LIT` loads into
   the first assignee then fmr-chains (extern Z routes via temp); re-read
   local right after a store schedules lbz+cmpwi early; block-scope
   `T* t = &g.sub;` mixed with direct `g.sub.f` stores reproduces split
   r30-direct/rN-pointer addressing; write zero/const on the LEFT of fcmpu
   compares to match operand order.
34. **PLUS-ZERO / CAST-ONLY PROP BLOCKER (gm_1832, 4 fn wins)**: `int q =
   val + 0;` (init) or self-read `p = (T*)((u8*)p + 0);` / cast-only
   `p = (T*)(u8*)p;` survives folding long enough to block copy-prop and
   front-end CSE; emits ZERO instructions in low-pressure contexts, one
   addi-0 under pressure. Companion DIV-RECOMPUTE TELL: target sharing ONLY
   the mulhw between `x/K` in a condition and `(f32)(x/K)` in the branch
   (duplicated srawi/srwi/add) ⇒ two div nodes reached ISel — feed the
   second division a +0-copied operand (won 738/910/B3C).
35. **EMBEDDED-LHS ASSIGN (ftCo_0A01 ×2, gm_1832 A000)**: `*(p = &X) =
   call();` emits [bl; addi p; stw] with the store refolded to a
   base-displacement; `X = call(); p = &X;` emits [bl; stw; addi].
36. **NAMED-ANCHOR ADDENDA (extends 26)**: (d) blocked pointer `T* st =
   &static_sym;` + idiom-34 blocker materializes sym@ha/@l in a
   callee-saved reg with object-relative displacements (gm_1832 ×6 fns);
   (e) struct-ptr hoist is an EXACT win when target shows small
   displacements off one callee-saved anchor across calls — anchor reloc
   pairs by resolved offset even while section-folded (gmregclear
   fn_8017EE40 92.66→100). Neither form reaches true TU-split
   constant-offset-only functions (fn_80188644 class, gm_1601 +0x110).
37. **INLINE-HELPER EXTENSIONS (extends 6/7/28)**: (a) helper param feeding
   the callee's arg slot precolors it — pass the loaded-pointer EXPR as the
   param and the lwzx lands directly in r5, no addi copy (fn_80187AB4 100);
   (b) a local assigned from a static-inline ADDRESS-RETURNING helper
   (`return &arr[i];`) allocates ABOVE the strength-reduction roving IV;
   plain `&arr[i]` allocates below in every decl/def permutation (ifcoget
   un_802FF4FC/620 both →100); (c) inside an inline, `result = f; return
   result;` reserves a caller frame slot, `return f;` doesn't (B3C −8
   frame); (d) hand-expanded duplicate branch bodies = auto-inlined call to
   a shared sibling defined before `#pragma dont_inline on` (lbaudio
   fn_800253D8/56BC both →100); (e) replacing an open-coded section with
   the TU's existing static-inline helper frees a named-local rank and
   fixes inline-data color ties (ftCo AF78C).
38. **PARENT-STRUCT MEMBER PATH (toy un_80307F64 +2.6)**: raw `*(T*)((u8*)
   base + BIG + i*4)` arithmetic reserves a dead 4-byte frame slot per
   distinct address temp AND ranks webs differently; a parent-struct cast
   `((Parent*)sym)->sub.field[i]` emits identical instructions, zero
   slots, and re-ranks param homes to the top. Struct-cast exprs through an
   EXISTING pointer var's value CSE into that var regardless of statement
   order (value numbering is position-independent) — only the
   no-shared-node member-path form avoids it.
39. **WEB-RANK RULES (extends 29; Toy_LoadLObjList 100, lbaudio 25E38)**:
   long-web named locals take r31.. descending in decl order; ALL param
   homed copies (incl. `T copy = param;` — coalesced, decl position
   ignored) rank AFTER them, later-param first — to sink a pointer walk
   below other homes, delete the cursor local and mutate the param.
   Deleting named int locals in favor of direct field reads lets anonymous
   CSE temps bind r8..r5 in creation order.
40. **POINTER-WALK / SCHEDULE PINS (gmregclear)**: else-if chain over
   consecutive bytes showing `addi rT,base,1 / lbz 0x1(base) / lbzu
   0x1(rT) / lbz 0x1(rT)` = `*++q` walk; the walk var needs ≥2 defs incl. a
   self-read or LICM hoists it pre-loop (tell: extra addi + li before the
   loop). Reg-prop folding is BB-local: first post-increment load folds to
   base-displacement, later ones read the register. Adjacent addi/lwz order
   flips when an address expr is written fresh vs routed through the
   already-declared pointer var (fn_80181598 →100); guarded-indexed
   multi-pointer loops reserve 4 phantom frame words — bump-form
   (`*a=-1; a++`) removes them (lbaudio 27AB0).
41. **@-ID SHIM MENU + FRAME BANDS (extends 23/31; ftCo_0A01, measured)**:
   per-TU @ costs — `do{}while(0)` (PAD_STACK) = 4 ids; `if (0) {}` = 2;
   `goto L; L:` = 1; removing switch/goto/labels subtracts; the same shim
   can measure differently inside dense control flow — re-verify on a
   downstream canary @pin. Frame = three bands: named aggregates (decl
   order, top) / scalar homes (4B each, position-independent) / instance
   band (sqrtf volatile y, bottom). `UNUSED u8 pad[N]` decls fill the
   aggregate band, PAD_STACK fills lower; NOTHING pads below instance
   slots (dead-below-y targets unreachable — parked family).
42. **CROSS-BB COPY-PROP LIMIT (ftCo AF78C 97.8→99.1)**: `T* o = *ptr;` in
   a different BB from ptr's def is NOT folded to a direct displacement —
   open-coding an auto-inlined callee through a caller-held pointer
   reproduces `lwz rX, 0x0(rPtr)` and can re-rank the whole callee-saved
   map.
43. **BINARY TELLS (wave 8 pack)**: bgelr/blr with r3 clobbered and no mr =
   prototype should be void (gm_80189CDC); not-taken branch jumping INTO
   the lwz/addi/stw increment, returning pre-increment = unconditional
   `return field++;` (fn_80181C80, semantic fix); caller-side clrlwi of a
   callback result + `li r0,0` else-arm = callback returns int not u8
   (gm_8017DB88); 4330 pattern WITHOUT xoris 0x8000 = (f32)(u32) source
   (fn_80188EE8 ×6 — suspect struct field really u32); `if (++s->bf >= K)`
   tests the just-stored byte reg, statement-then-test reloads
   (fn_80187910); word-copy trichotomy: 24×-unroll = block-ptr indexed
   loop / bl = memcpy / lwzu 8-byte loop = struct assign (lbaudio 27168);
   empty-if/else carry = ternary with preloaded else value (27DF8);
   `int x = (s8) call()` = one extsb web vs s8-typed remat (2785C);
   ≥9-byte string literal forces .data pooling ("/audio/\0"). NEGATIVE:
   never replace string literals with named char[] externs — literals pair
   by address (ifcoget 364 100→87.3, reverted).
44. **WEB-SPLIT SEARCH VAR (idiom-10 application; ifcoget 218 +13.7)**:
   target `li r0,N` scratch web + `mr rC,r0` at the join = two C vars
   (`found` scratch + `y = found;`); the copy survives copy-prop only
   because a later `y = f(y)` reads y.
45. **dtk SYNTHETIC-RELOC ROWS (toy)**: base+offset arithmetic landing on
   another named symbol (`addi r5, r30, 0x438` → un_803FE150 =
   un_803FDD18+0x438) gets a target-side-only synthetic reloc — unpairable
   from C (compiled addi has no reloc); referencing the named extern
   directly kills the base var and reshapes the fn (−20%). Diff-policy/
   naming class, like conversion magics.
46. **TU-MERGE ANCHOR TRIAD + MERGED-TU BSS LAW (extends 26/36; gm_18A5 +
   particle, wave 9)**: target RELA bss relocs are ALL addend-0/symbol-
   anchored (offsets live in instruction immediates). MWCC per-TU: extern
   ref → named + stwu/lbzu fusion; defined + single-symbol fn → named
   anchor; defined + multi-symbol fn → `...bss.0` anchor + big disps (bytes
   match only at section offset 0). Per-function target styles map to
   ORIGINAL-TU membership: defining-TU-at-0 (hoisted anchor, small disps),
   defining-TU-fold (cached `&field` addi-temps — arise ONLY from constant
   addressing; any register-based ptr form folds them away), consumer-TU
   (per-site named lis/addi). Levers: static-at-section-0, extern-flip
   (per-site named; PACKAGE deal — folding siblings regress), blocked-ptr
   hoist. One TU cannot satisfy all three styles for one symbol → TU split
   (see verdict section). **ANCHOR-REBIND BLOCKER** (particle ×2 wins, 0
   @ids): `T* sp = &static_sym; sp = (T*)(u8*)sp;` + total access mediation
   flips section-fold → named anchor. **OFFSET-0 SHADOW**: our static at
   section offset 0 displays as `...bss.0`/`...data.0` — name-pairing
   impossible at 0, free at any nonzero offset. addi-position follows the
   blocker STATEMENT position (extends 40/42): same-BB loads through p fold
   to base-disp, cross-BB stores keep `0x0(rP)`.
47. **INTERPROCEDURAL CONST-RETURN PROP (gm_18A5 +18/+11/+8)**: MWCC folds
   `return &global;` from a same-TU `#pragma dont_inline` fn into callers
   (`bl` + section-folded access). Extern-flipping the global converts
   those call sites to named materializations.
48. **@-ID COST TABLE v2 (extends 23/41; measured, 4 units)**: block-scope
   named local in a conditional arm = +2 file-wide ids with NO CFG change
   (broke 4 pinned fns, particle); second-def cast-copy `p2=(T*)(u8*)p;`
   = +1; init-form `(T*)((u8*)&s+0)`, self-read `p=(u8*)p+0;`, cast-only
   self-read = 0; `UNUSED u8 pad[N]` = 0; unused scalar decl `f32 x;` = 0
   ids AND a 4B mid-band home — cheapest id-reclaim is swapping
   PAD_STACK(4) (4 ids) ↔ unused decl (0 ids); `for` = +1 id vs `while`;
   assignment-into-condition = −1; SR-rover creation = +1; new inline
   expansion = +1..+12 (nested). The @-debt ledger is CROSS-FUNCTION:
   debts payable by shim removal anywhere before the first downstream pin
   (ftCo paid +13 with do-while/PAD swaps, pins clean). Canary-check @pins
   after ANY local addition. WAVE-11 ADDITIONS (measured): shim table
   re-validated to ±1 on a −13 repayment (ftCo AF290: 6×if(0) + goto);
   moving static-inline DEFINITIONS within a file = 0 ids (ids bind at
   expansion); SR-rover removal via explicit walking ptr = −1 id/loop;
   cured re-def split = −2 ids/loop (idiom 64); discarded-inline
   expansion = +2 (idiom 60); statement-position ternary = +1 vs
   arg-position (idiom 61); block-decl hoisting measured id- AND
   frame-neutral once (gm_18A5) — idiom-50 frame claim needs refinement.
49. **SQRTF DEPTH-2 HOLE-KILL (extends 31/41; ftCo AF78C win)**: depth-1
   sqrtf instantiation reserves {4B dead hole + volatile y}; routing
   through a one-line `static inline` wrapper re-instantiates at depth 2
   and DROPS the hole, sinking y. Call-bearing top-level inline instance
   = 8B; call-free inlines and _dontinline wrappers = 0B; PAD_STACK inside
   an inline allocates nothing; nothing pads below instance slots (still).
   Also (mplib): `_half/_three$localstatic$sqrtf__Ff` weak .sdata2 syms
   emit at offset 0 in EVERY sqrtf TU, stripped at link — the universal
   +0x10 object-level sdata2 shift is invisible in the DOL and harmless.
50. **gm_1601 MICRO-PACK (4 wins)**: PARAM-WEB SPLIT + BLOCKER — target
   `addi rH,r3,0` AND `addi rC,rH,0` with first store through rH = keep
   ONE use through the param + cast/+0 blocker on the local copy (both
   required; B40→100). POST-INCREMENT INDEX COPY — `arr[idx++] = v` emits
   `addi rT,rIdx,0` + stbx; `arr[idx]=v; idx++;` strength-reduces to a
   walking pointer (8710→100). `i != j` vs `j != i`: cmpw operand order
   follows SOURCE order even between locals — zero-cost row fix. SHARED-
   RET PHI COALESCE — `li rCS,0`+`mr r3,rCS` after early `return arg0;` =
   single-exit `s32 ret = arg0;` + `ret = 0;` in else (multi-def phi
   defeats const-prop; plain `arg0=0`/`x&0` fold). VALUE-`&&`/BLOCK-BOOL
   frame cost: each value-materialized `a && b` or block-scope bool = +4
   frame bytes; condition-position `&&`/fn-scope scalars = 0; `(u8)` cast
   of an int param in a test = +4/site (u8 param = 0) — frame tuner
   without PAD_STACK. TERNARY ARM-PLACEMENT: write saturates expanded
   `(sum > MAX) ? MAX : sum` with calls re-evaluated per arm (don't fight
   polarity, fix the duplication first).
51. **RELOC-PAIRING REFINEMENT + .DATA RECONSTRUCTION (refines 22/24;
   mplib [.data-0]→100)**: with named object symbols on BOTH sides,
   objdiff pairs by name+addend ONLY — equal resolved section offsets do
   NOT pair (the offset-equal clause applies to section-symbol
   resolution). Unreferenced non-const statics WITH initializers SURVIVE
   MWCC and emit at decl point, 0 @ids — usable as .data pads and dead
   strings (DOL-recoverable bytes); .data emission is plain declaration
   order. mplib's VtxIds +0x4C: EOF-moved defs + dead Vec3[2] pad + dead
   format string = every .data offset byte-exact.
52. **WEB-SPLIT CURES (extends 18/29; mplib)**: the `x = load; … loop
   re-defs x` lha/lwz→rTEMP + mr split is cured by assignment-in-
   condition (DrawSnapping 99.42→99.98) or guard-read loop form
   `while (count > 0) { …; count--; }` (mpLibLoad; the `for` equivalent
   also matches but costs +1 @id/loop); NOT curable for address
   materialization (r0+mr attractor class). NEGATIVE (particle):
   assignment-in-condition on a recursive 3-level walker explodes
   regalloc (97520 → 38%, reverted).
53. **ftCo MICRO-PACK**: NAMED CALL-RESULT LOCAL blocks FPR acc-coalesce —
   `dist = HSD_Randf();` + left-assoc `dist*A*B*C` gives fresh-f0
   accumulators (anonymous nested form coalesces acc into f1; id-neutral,
   unlike `(f32)x` self-assign +1 id). PRE-NULL-CHECK ADDI (extends 9):
   pointer init-at-decl before `if (p == NULL) return;` schedules the
   addi into the `mr.` shadow slot. dx-stmt-first + `dx*dx+dy*dy`
   reproduces [y-loads-first, fmuls dy², fmadds]. `a == NULL ||
   (x = load) == NULL` emits cmp/beqlr + lwz/cmp/bne/**blr** literal-
   return tail (particle DF4 at 2 rows; comma form `(hp = &s->xD0,
   (h = *hp) == NULL)` is the best-known next shape).
54. **DUPLICATE-@ PINNING (naming-6, validated, 22 pins)**: per-original-
   TU duplicate literals fix by renaming EVERY target dup copy to our
   single @N — pairing is by name and dtk accepts intra-unit duplicate
   local names (precedent: HEAD's @298 ×2). Fixes rows no single rename
   could (gmregclear @295/@298/@550/@674/@847/@849×3; gm_1832 @515/@516/
   @913/@697/@1074). Caveat: when two target names map to ONE our-side @
   (gm_18A5 lbl_804DA710 AND lbl_804DA7F8 → @357), at most one lands.
55. **INLINE-RETURN `b` + RESULT DEMOTION (lbaudio B24 ×5 rows, 233EC
   ×7)**: `return x;` inside a static-inline expansion emits an
   unfoldable `b end` (vs `goto label` which folds into the cond branch),
   AND the result local ranks BELOW param homes (B24 slot→r26 under
   arg0-home r27). One shared helper (FindSlot/BusyWait) fixed both
   signatures in two callers (74.28→95.72, 85.81→94.91).
56. **COPY-INIT addi LIVES ONLY IN EXPANSIONS (confirms 13 at scale;
   lbaudio)**: `j = total` / `n = accum` copy-inits emit as `addi rD,rS,0`
   ONLY inside inline expansions — caller-body copies and `+0` forms fold.
57. **INLINE-EXPANSION DECL RANK = FIRST-DECL-HIGHEST (lbaudio BusyWait)**:
   expansion locals rank opposite idiom-29's ascending rule (decl order
   n,accum,j,k,total ⇒ r7,r6,r5,r4,r3 — exact target map).
58. **AFFINE-MERGE vs DISPLACEMENT-FOLD (lbaudio)**: `base + CONST + expr`
   merges CONST into the index when the index is an EXPRESSION
   (`(u32)slot*4`, `i<<3`); plain `(var)` indexes fold CONST into the
   load displacement. Cure for loads: array-typed cast
   `((int(*)[2])(base+0x13A8))[i][0]` (233EC). No cure known for
   non-memory `+0x40` args (B24, ~4 rows open).
59. **ANCHOR RANK IS WEB-COMPOSITION-SENSITIVE (negative; lbaudio)**:
   PAD_STACK(8) (not 4!) flips the bss-vs-data anchor r30/r31 rank at +8
   frame cost; a direct symbol ref (`lbl_803BB340`) also flips it — not
   decl-driven (26C04/27168/2838C park-family context).
60. **DISCARDED-INLINE-CALL (gm_18A5 fn_8019249C ×2, won rows)**: a
   target-side lone `cmplwi` with no consuming branch = a checked
   static-inline called with its result discarded
   (`HSD_JObjGetNext(child);`) — arms DCE'd, branch folded, the test
   survives. Costs +2 @ids per expansion (measured).
61. **TERNARY COSTS + ARG PRECOLOR (extends 6/28/48; gm_18A5 ×5)**:
   statement-position ternary (`x = c ? a : b;`) costs +1 file-wide @id
   vs the SAME ternary in argument position; `call(p==NULL ? NULL :
   p->child, …)` lands the phi directly in the arg reg — the named-local
   form emits phi-in-r0 + `mr`.
62. **PTR-REASSOC NEGATIVE (gm_18A5, 8 forms)**: MWCC canonicalizes
   `base + intA + intB` by grouping the ints — invariant to &arr[],
   scaled-ptr casts, u32 domain, inline helpers. Only a self-read second
   def blocks it, at scratch-web rotation cost (parked).
63. **MERGED-TU BSS d-FORM FOLD (byte-proven, extends 46; gm_18A5)**: a
   defined static's section offset folds into d-form IMMEDIATES (reloc
   addend stays 0) — symbol at offset 0 ⇒ matching d-forms + 2 unfixable
   anchor rows/fn; nonzero offset ⇒ every d-form wrong. Decl-reorder
   probes net-negative (7up/11down). TU split is the only complete fix.
64. **RE-DEF SPLIT CURE = PER-LOOP FRESH VARS (extends 52; mpLibLoad
   99.05→99.92)**: per-loop FRESH fn-scope variables (count2..5) make
   single-def webs coalesce into r3 like loop 1; guard-read form alone
   only fixes the first def. Each cured loop FREES 2 file-wide @ids
   (repay with if(0) shims — pins byte-exact). Does NOT transfer to
   schedule-sink cases (80059554: fresh-web = exact no-op) nor
   address-materialization r0+mr (DrawCrosses).
65. **DATA2-WEB MATERIALIZATION LAW (extends 14/42/46; ftCo A3908)**: a
   second same-value `&fp->xNNN` pointer var materializes its own addi
   web IFF its def is in a DIFFERENT BB from the first's init (LICM then
   hoists it to the target's exact preheader slot); every same-BB
   init/blocker form VN-folds away. 12TH-WEB WRAPAROUND TELL: pushing
   callee-saved demand 11→12 flips an invariant addi web (stage_info@l)
   from rank-9 to rank-1 (r31) — attractor-rotation family instance.
66. **HELPER-SWAP RE-RANKS DOWNSTREAM WEBS (extends 37e; ftCo B1478→100,
   AF290→100)**: swapping an open-coded region for the TU's EXISTING
   static inline flips POST-helper anonymous web color ties (r28→r29)
   with bytes otherwise identical; open-coding the *consuming* inline is
   byte-neutral — the lever is the PRECEDING region's web structure.
   AF290 cracked the parked 5-web rotation by rewriting to the matched
   sibling's exact source shape (helper defs moved above, fn-scope vec,
   dropped named ranks, `UNUSED u8 pad[0x18]` + unused f32 ballast).
   NEGATIVE (B2790/A3908): MWCC folds `if(c) goto L; stmt; L:`,
   do-while-break, and two-goto forms back to inverted-bne — the
   beq/beq/b dead-b layout is unreachable from these shapes.
67. **VARIABLE-BOUND GUARD FOLD (lbaudio B24)**: an inner loop with a
   variable bound (`s32 bound; bound = 9; … n < bound`) folds the
   redundant zero-trip top guard while keeping register unroll guards; a
   literal bound emits the guard; do-while retargets the unroller to the
   outer loop.
68. **IDIOM-59 MECHANISM (lbaudio B24, resolves the rank-swap mystery)**:
   a SECOND same-section symbol ref in a function causes section-fold +
   anchor r30/r31 rank flip; removing the second ref (route through a
   fn-scope self-read ptr, idiom 62-style blocker) restores the named
   single-symbol anchor AND the target rank. Decl/def order are no-ops.
69. **.DATA = ONE SOURCE-POSITION STREAM (extends 51; lbaudio, byte-
   proven)**: named statics, non-static globals, string pool, jumptables
   and localstatics all emit into .data interleaved in SOURCE position —
   entire sections are byte-reconstructable by decl placement alone
   (lbaudio 0x1818-byte .data byte-identical under the w12 package).
70. **TENTATIVE-ARRAY TELL (lbaudio)**: MWCC rejects `static T x[];`
   tentative + later initialized def — any early-visible late-emitted
   array in the target was a NON-STATIC GLOBAL (def in .c + extern in
   .static.h).
71. **ZERO-REGION = POINTER TABLE (lbaudio)**: all-zero target .data
   regions covered by addend-0 relocs are pointer tables (reloc storage),
   not padding — reconstruct as fnptr/objptr tables placed by source
   position.
72. **⭐ RELOC-FORM DICTATES SOURCE FORM — "GRAND INVERSION" (gm_18A5,
   byte-proven via .rela.text index resolution, tool tusplit2.py)**:
   target reloc form per site is LAW — @-target ⇒ pool literal in C;
   lbl-target ⇒ named extern ref in C. The naming-8 SRC-FIRST direction
   (extern→literal) was wrong for every lbl-target case; literal→extern
   pairs rows IMMEDIATELY with no symbols.txt change and deletes
   artifact @-dups. Dup @-names break objdump name-keying — resolve
   targets by .rela symbol INDEX, not name. @-costs measured: extern-f32
   single-ref/stmt = 0 ids (idiom-23 "+1" REFUTED for this shape);
   double-ref in one stmt = +1 CSE temp; single ref in loop = +1 LICM
   temp iff bound at loop entry before the loop's first pool creation
   (multi-BB webs = 0 — not fully modeled, ALWAYS canary); for→while =
   −1 (j-loop) yet 0 (i-loop) — idiom-41 context-dependence reconfirmed;
   literal→extern where the value dies = −1 at the old creation site.
   **⭐ WAVE-13 CODEBASE SWEEP (analysis-only, top-20 units)**: tool
   `campaign/scratch/inversion-sweep/analyze.py <unit>` (ELF32-BE
   .rela.text on both objects, symbol-INDEX resolution, dup-@ safe) +
   per-unit worklists `campaign/scratch/inversion-sweep/
   <unit>.worklist.txt` (post-naming-9 baseline). High-conf totals:
   **LIT2EXT 361 (355 actionable) vs EXT2LIT 4 (~90:1)** + AT_RENUM 15 +
   SHADOW ~190 (park) + NAMEPAIR ~20 — the naming-8 SRC-FIRST direction
   is conclusively dead. ✅ **Wave-14 EXECUTED the first five** (one
   owner per unit, src-only, all gated, trees UNCOMMITTED): ftcoll
   DONE (root cause = Fix-A not B; 9 fns→100, 24 UP / 0 DOWN; 14-
   rename naming-11 map) → camera DONE (14 UP / 0 DOWN, ~32/81 rows
   paired; no pure-LIT2EXT fns existed so no new 100s; 3 Fix-A
   renames + 1 NAMEPAIR queued) → toy DONE (5 fns→100; the EXT2LIT 4
   = fadds swap-pair rows, C-UNREACHABLE — Fix-A framing was wrong;
   address-keyed renumber map) → tydisplay DONE (ALL 49 rows cleared,
   un_8031C1D0→100, ZERO @-drift) → grvenom DONE (28/30, 3 fns→100;
   2 donor-keeper leftovers BY DESIGN, idiom 86). ✅ **Wave-15
   EXECUTED the tail — sweep worklist heads ALL DONE**: grbigblue
   (3 fns→100 + 9 UP, OK 432→439, LIT2EXT 20→13 all park-enumerated;
   Fix-A renames UNAVAILABLE there — sqrtf weak-sym +0x10 shift
   kills offset-pairing and ~40 @-pins name-pair on our numbering)
   → sislib (3 fns→100, OK 148→158, LIT2EXT 11→1 donor-residue BY
   DESIGN; the dup-640 split resolved per donor-keeper: kept the
   611C+0xfc creation literal, 6754+0x18→ABC, and @264 RECLAIMED
   its own number — no renumber queue) → lb_00F9 (3 fns→100, OK
   377→382, LIT2EXT 8→3; @328 kept alive at .sdata2+0x14; unit
   merges ≥4 TUs — lb_803BA1C0 = lbspdisplay.c string AND its
   .data fold base) → gmresult (5→1 forced donor-keeper; target
   lbl_804D3FB0 was the assert string "0", not an invented s8) →
   gm_1832 (correct NULL result: its 3 rows are 0x4330 conversion
   magic already objdiff-paired — analyzer-only, idiom 91; zero
   compiles against its pins). Jumptable rows (ftCo jtbl_803C5C70/BE8,
   grmutecity) excluded as naming park. camera cm_80452C68 ×74 SHADOW
   = the old camera.static.h extern-flip package; gm_18A5 SECFOLD 6
   re-confirms its TU split. Re-run analyze.py on main/melee/lb/
   lbaudio_ax after the lbaudio .data package lands.
73. **PRAGMA-AT-CALLEE (ftCo A2718 →100)**: `#pragma dont_inline on`
   wrapped around a CALLEE's definition suppresses its auto-inline at
   all call sites while leaving the caller free to expand its own static
   inlines — the per-original-TU tool when one merged-TU function needs
   both a `bl` sibling call and same-TU inline expansions. Loop-bearing
   functions never auto-inline (pragma on them is dead weight).
74. **EXPANSION r0-DIAMOND (extends 55; ftCo A2718)**: lone
   `if (static_inline_returning_bool(...))` materializes li r0,0/1 per
   return site + `cmpwi r0,0`; early-return body (`if (!a) return false;
   if (b && c) return true; return false;`) puts a separate li-0 on the
   !a path; `&&`-to-named-variable instead lowers incrementally into a
   callee-saved reg (never the diamond); two expansion results combined
   with `||` produce the shared `li r3,1` cross-jump tail. @-costs:
   2-if helper expansion ≈ +4; while+`&&`-condition vs for+continue-ifs
   = −3; nested-if vs `&&` inside an inline body = +3; idiom-50's
   "+2/conditional-arm local" appears REVERSE-SIGNED for hoisted
   comma-init arm locals (2nd counterexample — refine before relying).
75. **NEGATIVES (wave 12)**: idiom-37b address-returning inline helper
   does NOT reach r0+mr address-materialization attractors (mplib
   DrawCrosses, exact no-op); idiom-66 scope limit — CFG-form
   restructuring of preceding/consuming regions does NOT flip
   schedule-def-order rotations (mpLibLoad ×3 loop-form flips; the lever
   is web-STRUCTURE change, not loop form; rotation invariant to decl
   order, def order, grouping, +0 blockers, and decl-order permutations
   — new axis, all exact no-ops); changing scope:local↔global on
   split-range symbols.txt entries CHANGES LINKED DOL BYTES (dtk shasum
   FAIL) — dtk's cross-unit promotion suffix `@NNN_ADDR` rows are
   config-unreachable PERMANENT accepted drops (fn_80188EE8 ×8);
   renaming a target lbl_* that our src still extern-references breaks
   every paired row through it — convert src first (Grand Inversion).
76. **SDA21 RELOC-OFFSET CONVENTION SPLIT (inversion sweep)**: dtk
   records r_offset at the insn START, MWCC at +2 — any reloc tooling
   comparing the two sides must normalize `off & ~3`.
77. **`...sec.N` = NAMED MWCC ANCHOR OBJECT (inversion sweep)**: our-side
   `...sec.N` relocs are named anchor objects, NOT section symbols —
   discriminate SHADOW (resolved-equal ⇒ offset-0 park) vs SECFOLD
   (unequal ⇒ real fold/TU-split evidence) by resolved offset.
78. **POOL-ORDER SWAP (inversion sweep; mncharsel 62648, gm_18A5
   90ABC)**: two adjacent-site literals created in REVERSED order = a
   src statement-ordering fix, not a renumber.
79. **dtk RE-SPLIT AUTO-RENUMBER (header-tmunk w13)**: a config.yml-
   triggered re-split can AUTO-APPLY queued @-renumbers by rewriting
   symbols.txt (one-time, address-keyed, DOL-neutral — it byte-derived
   the exact gmregclear naming-9 map unprompted). Gate `git status` on
   symbols.txt after EVERY re-splitting ninja; the saved diff doubles
   as a machine-derived renumber map
   (`campaign/scratch/header-tmunk-w13/symbols_txt_dtk_rewrite.diff`).
80. **PARTICLE-SPLIT PACK (wave 13)**: (a) `-Cpp_exceptions` is a large
   @-id consumer — dropping it renumbered the head TU's whole pool
   (−8 early..−108 late); compiler-flag changes = TU-wide renumber;
   (b) scope:local→global flips on WHOLE-OBJECT addend-0 symbols are
   byte-safe (dtk shasum-proven ×5) — idiom-75's negative is narrower
   than stated (split-RANGE entries only); dtk renders local data names
   with an address suffix (psTexGroupArray_804D0B50), so flips change
   target-side names and src must rename to match; (c) the address-
   materialization attractor extends to const-array base+const-elem
   reads (`(base+bank)[0x18]` → lwzx; direct/local-base/+0 all no-op).
81. **ARG0-LOAD HOIST + FRAME QUANTIFICATION (extends 9/40/53/56;
   lbaudio w13, both won)**: named block-local + idiom-34 self-read
   (`s32 id = ext; id = id + 0;`) relocates an arg-position sda load
   ABOVE intervening clamp branches and still coalesces into the arg
   reg (without the blocker, load-prop folds it back). Idiom-56
   escalation: the `i=0; z=i;` copy-init `addi rZ,rI,0` is reachable
   ONLY via inline expansion — the identical caller-body form RESHAPES
   the web. Idiom-40 quantified: 4-pointer guarded-indexed loop =
   exactly +0x10 phantom frame; bump form = 0; `UNUSED u8 pad[8]`
   ballast tunes between (28690 frame 0x28 EXACT).
82. **u8-PARAM RANK LAW (extends 39; gm_80166A98, measured)**: a
   u8-typed param's home ranks at the TOP of the callee-saved map,
   above ALL s32 param homes (which run [p1, p10..p2] from r31 down);
   s32→u8 on one param rotated a 10-param map exactly into place. A
   named u8 local copy gets the same rank but costs clrlwi + 4B frame;
   an s32 copy coalesces (rank lost) — param TYPE is the only
   zero-cost lever found.
83. **gm_1601 WAVE-13 PACK**: CTR/bdnz conversion requires the counted-
   `for` form (`for (j=3; j!=0; j--)`) — do-while-with-decrement stays
   subic./bne even when legal; loop-bearing `static inline` NEVER
   inlines (bl emitted; extends 73) — per-TU duplicate helper clones
   (check_bit) are the mechanism behind r0-diamond regions;
   UNINITIALIZED-RETURN TELL: no li-0 anywhere + return-var sharing a
   dead pointer's r3 per-path = the decomp's `t = 0;` is an invented
   init (delete it; u8 t blocks coalescing, s32 required); UNUSED s64
   decl = 8B frame each, used = 0x10 (confirms 28/48); CAVEAT: three
   idiom-48 cost entries (cast-copy +1, assignment-in-condition −1,
   goto-label +1) measured ZERO @-drift here — always canary, never
   precompute from the table.
84. **FIX-A RESOLVE-PAIRING + POOL-MASS CONSERVATION (extends 22/24;
   ftcoll 9 instant 100s, camera)**: when target "named" .sdata2 syms
   are names ON anonymous pool entries, delete the const defs and use
   literals — our pool lands on the target's exact offsets and objdiff
   resolve-pairs immediately with ZERO config (beats renames; renames
   become optional cleanup). Corollary: OFFSET-PAIRING IS LOAD-BEARING
   — pool byte-mass above a paired entry must be conserved (camera
   0.5f@+0x30 pairs A0C0×3 + CF8×6 rows; do-not-kill literal list in
   inversion-camera-w14/REPORT.md). Conversion magics (0x4330…) pool
   ONLY from live codegen (dead inline / if(0) / dead-init local all
   fail) while STRING literals in never-expanded inlines DO pool; MWCC
   dedups identical strings TU-wide (a 1-char content typo = dup pool
   entry — binary tell, 3 fixed); zero-init static float → .sbss; dtk
   tail-pad gaps are unreconstructable residue.
85. **CONVERTIBLE-SITE TAXONOMY + NAMED-REMAT DRIFT (refines 25;
   camera, per-site measured)**: literal→extern converts byte-exact at
   compare-operand and call-arg sites (even across calls/diamonds);
   BREAKS at store-value temps, dominated re-reads (CSE), const-prop'd
   compares, and loop reads — attractor/CSE-locked. A literal site can
   emit a NAMED reloc via its value-number representative and FLIP
   under unrelated edits — pin the choice in source. MSL-inline clone
   recipe: byte-exact sqrtf clone needs EXPANSION-LOCAL `double
   half/three` copies (direct extern reads rotate the web) plus one
   plain-sqrtf expansion kept alive — the dead weak _half/_three 0x10
   .sdata2 bytes are load-bearing ballast.
86. **EXTERN-ALIAS RELOAD CURES + RANK FLIP + DONOR-KEEPER (extends
   25/33, refines 23/72; toy, grvenom)**: extern f32 reads do NOT CSE
   across pointer stores (alias barrier) — per-use reloads where pool
   literals CSE; cures: per-region block-local copy (byte-exact,
   frame-neutral; +1 @id) or reversed chain-assign
   `xE0 = xE4 = xE8 = xDC = ext;` (innermost-first store order =
   target, single lfs, 0 ids); bulk loop-fn conversions all regress.
   EXTERN-RANK FLIP: an extern zero takes the HIGHER FPR vs a literal
   in unrolled multi-use compares — extern is byte-exact only
   straight-line/arg-position. fadds operand order canonicalizes
   lit-first (invariant to source order/volatile/local routing — the
   4 EXT2LIT rows are swap-pairs, C-unreachable). DONOR-KEEPER: when
   the pool creation site is itself in LIT2EXT scope, keep the FIRST
   occurrence literal — preserves creation order, pool offsets, every
   downstream pin, zero shims.
87. **LOCAL-TABLE COPY TELL + POSITIONED Fix-B (extends 24/69;
   tydisplay)**: target head `stwu + lis/addi + li N + mtctr +
   lwzu/stwu` = a local array copied from a named .rodata template —
   convert as `T table = <named>;` copy-then-index ONLY (direct
   indexing collapses to 4%); dtk sizes the copy-loop align pad into
   the template (0xB0 object = 0xAC + pad, NOT an extra entry). Fix-B
   defs are POSITION tools: legal anywhere after seen uses; def
   placement reconstructs the .sdata2 emission stream around pool
   creations. Idiom-24 extends to s32 (const int folds to lis/subi
   immediates); SDA21 rA (r13 vs r2) diffs are real rows.
88. **INSTRUMENT SPLIT (tydisplay, hard rule)**: report.json fuzzy is
   reloc-name-BLIND — a 49-row reloc debt scored 100 there. Gate all
   inversion/naming work on inversion-sweep analyze.py + check_fn /
   objdiff match_percent only.
89. **gm_1601 SPLIT-EXEC PACK (wave 15)**: SHARED-STATIC.H SPLIT
   PATTERN — when the head TU field-accesses a moved static through a
   getter pointer, the struct def must stay visible to BOTH TUs
   (restore in .static.h with a sync note + duplicate in the new TU;
   incomplete-type compile error is the tell). Noinline wrapper
   chains MOVE with their only caller (idiom-83 per-TU clones).
   A 4-byte .sdata2 "gap_NN" can be a real object boundary — cutting
   at its end is legal (dtk re-displays gap→pad, same bytes).
   Re-verify wave-N "expected recovery" lists per-fn at byte level
   before promising: 95BC/9A84 residuals were pure S1 (no anchor
   rows) despite living in the +0x110 family. Project-level
   report.json delta mid-wave is sibling in-flight flux — per-symbol
   gates authoritative (extends 88).
90. **⭐ LABEL IDS TICK AT FUNCTION ENTRY (refines 41/48; sislib +
   gmresult, independently measured)**: label-shim @-ids bind at
   function-label-numbering time, NOT lexical position — a goto-shim
   placed AFTER two literal creations still renumbers them (sislib:
   shim in 5798 shifted pinned @67/@68, moved to 594C → restored;
   gmresult: shim stole id 880, not 882). Corollaries: a shim can
   NEVER repay an id slot bracketed by two same-function literals →
   donor-keeper is the only pin-safe option there (the residual row
   is permanent; renaming the target sym → @N forbidden once src
   extern-refs the name, idiom 75); place shims only in fns with NO
   pinned creations; donor-keeper at the creation site + fn-entry
   shim repayment of a same-statement sibling death reclaims the
   entry's EXACT original @number (no renumber queue needed).
   File-scope `char[8] = ""` init blob ticks +1 id — cure =
   positioned Fix-B EOF def (idiom 87 extends to .sdata objects).
91. **@-SYMS ARE RESOLVABLE-ANONYMOUS (refines 51; gmresult,
   byte-proven)**: objdiff pairs our-side `@N` pool syms against
   target NAMED syms by equal RESOLVED OFFSET (our @697 .sdata2+0x28
   == tgt lbl_804DA560+0x28 ⇒ fn_8018504C scores 100.00 with 2
   "mismatched" analyzer rows); the name+addend-only rule applies to
   lbl-vs-lbl pairs. Such rows are ANALYZER-ONLY — extend
   analyze.py's pairing emulation. Companion (lb_00F9): check_fn
   flags identical-text `yellow@sda21` rows `~` that the analyzer
   pairs — never count check_fn `~` without analyzer confirmation.
92. **LIT2EXT COST TAXONOMY + GX-FIFO RANK + SOURCE-ORDER COMPARES
   (extends 72/85/86; grbigblue/lb_00F9/sislib, measured)**:
   inline-expansion arg-position extern f32 = +1 @id PER EXPANSION
   (caller-body only — refines idiom-72's "0"; grbigblue +6 uniform
   drift on 6 expansions, + GCSE f30). 0-id byte-exact classes:
   compare-operand, call-arg, raw-ptr-store-after-call, chain-assign
   (re-validated ×3 at 100; innermost-first store order, single
   lfs), const-LEFT equality, direct extern in 2-use straight-line
   GX args, multiply-operands feeding (u8)(s32) store conversions
   (×12), triple same-value call-arg fans (one extern lfs, CSE
   preserved). Extern compares follow SOURCE operand order while
   literals canonicalize const-first — write the extern LEFT when
   the target row is const-first. EXTERN-RANK FLIP extends to
   GX-fifo multi-use: across GXWGFifo volatile stores a literal CSEs
   into f2 (target-exact) while an extern mis-ranks in EVERY form
   (direct=f3; named local stmt/in-arg=f4 — named webs rank after
   ALL anonymous CSE webs; pre-branch def=f4+hoist), and pointer
   loads DO CSE across fifo stores ⇒ the flip is pure RA rank, not
   reload. Donor-keeper parks remain the standing cure (idiom 86).
93. **COMMA-ARG ASSIGNMENTS SPILL (lbaudio w16)**: assignments in
   comma/arg position SPILL — and the +0 blocker does NOT block in
   comma/arg position.
94. **ARG-POSITION CHAIN-PRECOLOR vs BASE-GROUPING (extends 62;
   lbaudio 233EC)**: arg-position chain-precolor (r3 at depth 3) and
   statement base-grouping are MUTUALLY EXCLUSIVE from C —
   member-offset `->field` and scaled-const array-cast forms both
   reassoc-group.
95. **FOR-INCREMENT addi ORDER FOLLOWS COMMA SOURCE ORDER (lbaudio)**:
   `p += 2, i++` vs `i++, p += 2` flips the increment addi emission —
   zero-cost row fix.
96. **LITERAL-RETURN BLOCKS BRANCH-INVERSION FOLD (mechanism for
   idiom 17; lbaudio 233EC ×2)**: a literal `return arg0;` at a
   dead-b fold site blocks the fold at +1 dup mr per site (+ a
   label/beq where needed); MWCC never tail-merges [mr;b] return BBs
   (G7 negative).
97. **gm_18A5 SPLIT-EXEC PACK (wave 16)**: (a) **BSS-cut
   disambiguation is EMPIRICAL, not layout-order** — extern-flip a
   candidate and gate; the defining-TU fold fingerprint = anchor at
   the TU's bss BASE symbol with a big d-form displacement
   (lbl_80473AB8+0x370C); dtk's RELA addend-0 display masks ownership
   (both styles display "named"). (b) small pads cannot pad .bss:
   unreferenced `static u8 pad[≤8]` lands in **.sbss** — block-offset
   reconstruction needs >8B pads or correctly-sized typed defs.
   (c) defining an extra referenced bss static in a TU reshapes
   SIBLING refs (per-site named materializations flip to
   shared-anchor+lbzu fusion; 31 fns crashed, reverted). (d)
   tentative non-static defs flush at end-of-TU in REVERSE decl order
   (sbss, measured — same rule as unreferenced externs); unreferenced
   static struct/array bss defs DO emit at decl point. (e) commit
   hygiene: a sibling's STAGED files get swept into `git commit` even
   with explicit pathspecs — amend out via
   `git reset HEAD~1 -- <paths>`; verify staging before every commit.
98. **NUMERIC BRACE-INIT = 0 @-IDS + DATA-RECON LAWS (refines 90/46;
   datarecon-w16, proven 89KB EOF + 7.7KB top-of-file)**: the
   init-blob +1 @-id is STRING-LITERAL-initializer-specific; numeric
   brace-init arrays consume ZERO ids at any position — dead-data
   reconstruction needs no shim accounting. Companions: ≤8B zero
   objects can't reach .data from C (`static u32 x=0`→.sbss,
   `x[1]={0}`→.sdata) — pad via ALIGNMENT of the next object (u64
   element type); defining a previously-extern GLOBAL keeps named
   relocs (fold law = statics/anchors only; multi-.data-symbol fns
   can still fold ONE site, 97814); .sbss→.data table conversion
   flips sda21 1-insn refs into loop-invariant lis/addi hoisted to a
   callee-saved reg at entry — an entry-bound named pointer local
   pins the materialization rank.
99. **STALE-ID HAZARD + EOF-DEF DELETION (camera paired-fix w16; all
   3 queued ids were +2-drifted)**: Fix-A/naming queue ids from old
   wave reports MUST be re-derived value+address-keyed from a fresh
   object dump at land time — @-pin canaries do NOT detect early-id
   drift when a later shim rebalanced (w14 cam_sqrtf clone +2 ticks;
   @717 stayed exact). Companions: EOF-def deletion is
   pool-offset-FREE (defs emit after all pool creations — mid-block
   Fix-B def deletions never shift offset-paired entries; only the
   [.sdata2-0] display row pays, section-alignment artifact);
   EXT2LIT single-arm f64 assign is byte-neutral (extends idiom-85
   safe classes to the reverse direction).
100. **hsd_3983 S1 PACK (wave 16, byte-proven)**: (a) ⭐ auto-unroll
   zero-trip guard NEVER folds for signed var-left `i < N` but FOLDS
   for u32 IV or const-LEFT `0x41 > i` (signed cmpwi preserved —
   extends 92 to loop guards); (b) CTR guard menu:
   `for(j=num;j<num2;j++)` → subf+mtctr+one cmpw/bge;
   `for(k=1;k<=n;k++)` → mtctr-n-RAW + cmpwi n,1/blt (1-based);
   if-wrapped counted-fors double-guard; (c) out-of-line `b end;
   end: slwi/add; b back` residual = unroller output ⇒ rolled source
   (layout tell); (d) SR stream separation requires distinct STATIC
   symbols — interior offsets/struct fields/pointer locals all
   VN-merge (5 forms); (e) lwzx/stwx via idx-var + duplicated full
   deref expressions (extends 58); (f) `num2 = num` copy survives
   iff num is live past it; NEGATIVES: +0 never blocks const-prop of
   0-stores; `store i` always folds (A08 zero-web partition = new
   park family); idiom-48 block-local cost measured 0 here.
101. **dtk OBJECT SIZES ARE GAP-INCLUSIVE (header-gmtypes-w17,
   refuted MatchEnd +4)**: a symbols.txt size delta at a TU boundary
   whose next .bss base is 8-aligned is ALIGNMENT, not content —
   cross-check EVERY struct-size claim against adjacent-symbol caps
   on EMBEDDINGS before growing (MatchEnd had 3 embedding caps at
   0x227C; the live +4 experiment rewrote instruction bytes in 5
   matched fns).
102. **HEADER SIZE-FIX VERIFICATION RECIPE (w17)**: embedding-span
   caps → access-pattern scan for tail typing → live grow-experiment
   under masked-reloc bytegate. TmData +8 passed all three (the +8
   IS content: next obj 4-aligned at an already-8-aligned address,
   no target accesses in the tail).
103. **SHARED-TREE GATE PACK (naming-13, live-incident-proven)**:
   (a) per-rename DOL gates on a shared tree need a sibling-churn
   md5 canary + my-edit-revert attribution — a DOL FAIL isn't yours
   until proven on a settled tree (one spurious FAIL was a sibling's
   live size probe); (b) sibling-isolated controlled gate = HEAD
   worktree dtk split (target objects depend only on DOL+config) +
   direct mwcc of the units (~25s, no full build); (c) same-valued
   dup-pin magics must map by BYTE VALUE, never listing order
   (extends 91/99 — the queued gm_1965 pair was value-crossed).
104. **f32/s32 CONST-PROP LAW + Fix-B PLACEMENT WINDOW (datarecon
   gm_190A, hardens 87/98)**: a const def BEFORE a use const-props
   it into a duplicate pool literal (+1 id, rows break; probe broke
   7 fns); def-after-all-uses = byte-identical named lfs — Fix-B
   slots are reachable only in the [last-use, next-pool-creation]
   window. Companions: pool/def namespaces are DISJOINT (literals
   never dedupe onto equal-valued named defs); const scalar defs =
   0 @-ids at ANY position (extends 98 beyond brace-init arrays).
105. **MWCC CONST-DEF/EXTERN CONFLICT + UNREACHABLE STREAM HEADS
   (recon gm_1965)**: extern-f32 vs const-f32 redecl ERRORS, and a
   def followed by a conflicting extern is SILENTLY DISCARDED;
   idiom-24's fold is visibility-only — an extern-const decl does
   not protect a later def. Since Fix-B defs can never emit before
   later users' pool creations, a target .sdata2 stream whose HEAD
   precedes first-use order had a dead/deleted early creator and is
   C-UNREACHABLE (gm_1965 +0x00..0x14 group + magic order; 0.0f
   pool-head both recon TUs) — park with evidence.
106. **OFFSET-0 .DATA DEF FLIPS FN-NAMING AND AUTO-INLINE (recon
   gm_1965)**: an offset-0 .data def can flip a multi-symbol fn to
   `...data.0` AND flip auto-inline of an UNROLLED constant-loop
   callee (loop-unroll defeats "loop-bearing never inlines",
   refines 73/83) — pragma-at-callee cures both. Companions:
   extern-f32 in an inline store-arg = 0 ids (narrows idiom-92's
   +1 to the GCSE class); goto-shim id binding at fn entry
   re-confirmed (took @696 exactly).
107. **BOOL-OF-CALL-IN-INDEX (gm_18A5 S1, ECA8 →100)**:
   `a[!!f()][k] = rhs;` — MWCC evaluates the store RHS BEFORE the
   index call; div/sub temps become anonymous CS-reg temps with
   ZERO frame homes. Decomp-invented staging locals (`is_us`,
   `c7/c8`) rotate the whole web map and cost +4 frame each;
   deleting them freed exactly 11 @-ids (cross-checks the 41/48
   shim table: 5×if(0) + 1×goto = 11).
108. **CS-CHOICE LAW FOR CALL-FREE HIGH-PRESSURE FNS (gm_18A5 S1,
   5 measured probes)**: with ≥3 named webs the LAST three in decl
   order take r31,r30,r29 in decl order — idiom-1's
   first-decl-highest holds only for call-BEARING fns; with 2 named
   webs both get CS + one invariant is promoted (promoted
   clrlwi-LICM webs rank ABOVE named, promoted addi-anchor webs
   rank BELOW first-named). Idiom-66 negative extended: empty-then
   `if(c){}else` folds back to inverted branch and costs +1 id.
109. **hsd_39D1 S1 PACK (wave 17, byte-proven)**: (a) named f64
   local = 8-12B phantom home at decl position — "lfd const / stfd
   / lfd-in-loop" means a compiler-hoisted CONSTANT, not a
   variable; (b) volatile-y inside an inline lands in a separate
   inline-temp zone (+8-12B/site) — fn-scope user decls pack like
   the DOL; fabsf_bitwise costs +12B/site vs raw `*(s32*)&` masks;
   (c) `if ((v = expr) > k)` lands expr in v's web (kills the
   post-compare fmr); (d) `v = c ? v : k` keeps the ble/b
   empty-then shape that `if(c){}else` folds away; (e) beq-median/
   bge compare trees = source `switch` (if-chains emit
   subi/cmplwi collapses); (f) ⭐ OFFSET-PAIRING BEATS RENUMBERING —
   removing .sdata2 ballast (math_ppc.h weak sqrtf localstatics,
   +0x10/TU) let objdiff pair pool relocs positionally with ZERO
   symbols.txt edits; (g) a jumptable entry past the default block
   = explicit empty `case N: break;`. LAW: never sed shared tokens
   TU-wide (a sibling's `sed PAD_STACK(8)→(4)` clobbered a matched
   fn's pad — self-caught by the unit gate).
110. **⭐ FLIP-LINK LAW PACK (file-flip w18; byte-proven through 6
   full ninja passes)**: (a) WEAK-STRIP — `_half/_three` weak .sdata2
   surplus is linker-stripped AND compacted (GXPixel ours=tgt+0x20
   Matching precedent + fresh flip; extends 49 to link-proven); (b)
   ALIGNFIX-NOBITS — short .sbss tail (0x1/0x2 vs 0x8) restores via
   the next contribution's align=8; (c) ALIGNFIX-PROGBITS — short
   zero .data tail (0x75 vs 0x78) restores via the next object's
   alignment zero-fill ⇒ dtk gap-tail parks SURVIVE a Matching link;
   (d) EXPORT-NEUTRALITY — renamed/missing target-named local exports
   are link-neutral iff ZERO importers across all 1041 link inputs;
   (e) AT_RENUM/NAMEPAIR/LIT2EXT/SHADOW rows are link-IRRELEVANT at
   equal resolved offsets (ftchangeparam linked byte-exact carrying
   219 LIT2EXT + 6 SHADOW rows) — naming-only fns ARE byte-correct in
   a linked Matching unit; gate flips with linkcheck.py
   (resolved-reloc equality + weak-exempt + ALIGNFIX + export
   closure), never fuzzy/analyzer counts.
111. **DEADSTRIPPED-INLINE POOL-ORPHAN RECON (extends 105; ftcoll
   [.sdata2-0] →100)**: a target pool literal first reloc'd LATE but
   pooled EARLY = created by a fully-inlined, linker-deadstripped
   static helper — reconstruct as an UNCALLED static helper at the
   original lexical slot containing ONLY the orphan literal (flag
   in-source as permanent ours-only ballast; zero text change to all
   sibling fns). Laws: pool/def namespaces are DISJOINT (closes the
   Fix-B route — +0x28 is pool, not def); MWCC has NO post-codegen
   branch folding (multiply-assigned and single-assigned-zero guards
   both EMIT); post-return code dies PRE-pooling.
112. **MISSING-`inline` PARK DISSOLVER + DIAMOND-INIT RANK (ftcoll)**:
   an ours-only out-of-line copy of a helper = missing `static
   inline` (w14's "structural" getEnvDmg park dissolved; all 8
   expansion sites byte-identical, zero gate movement). Diamond-init
   webs outrank first-decl webs; a ternary init DEMOTES a web to
   volatile-class rank (new S1 lever); decl/init decoupling composes;
   one-call-at-end fns take the idiom-108 call-free CS-choice.
113. **S1 ROTATION LEVER PACK (w18: gm_190A 935B8 →100 in 1 probe;
   gm_1693 695BC →100 full rewrite)**: (a) idiom-98 entry capture
   `u8* table = lbl_X;` works as an S1 lever — flips a data/bss
   anchor r29↔r30 tie + the coupled tail counter/walker pair; (b)
   rolled fill `for` auto-unrolls ×8 (do-while KILLS the unroller);
   (c) inline-identity-helper `n = helper(raw)` lands `clrlwi.`
   directly in the CS home fused with the loop guard (plain assign =
   r0+mr attractor); (d) NO return statement — m2c invents them; any
   returned var costs a CS home + mr; (e) call-result redefine
   `q=(u8*)f(); if((s32)q!=0)` kills call-crossing (cast gives signed
   cmpwi); (f) `switch(u8)` = exact beq/bge median tree incl. the
   dead <0 branch — if-chains range-fold (extends 109e); (g)
   `*arr/*++arr/arr++` lbzu pair-walk + counted-while → mtctr/bdnz;
   per-loop `p = arr` anchor copies; (h) `(s8)pa[0]` typed load kills
   a dead extsb; `((s8*)sp)[i] = -1` = li −1; (i) blocked-pointer-in-
   condition reproduces add/lbz-disp + single extsb. Residual class:
   just-freed-reg chain rotations (69A84 parked-at-best 94.26 →
   rotation-enumeration agent).
114. **gmregclear-FAMILY RECON PACK (w18)**: (a) MERE INCLUSION of
   math_ppc.h emits the 16B _half/_three .sdata2 ballast even with
   ZERO sqrtf use — removal = −4 @-ids (safe ONLY in pin-free/
   offset-pairing TUs; KEPT in head deliberately, 60 pins); (b) head
   .data = 4 original-TU blocks, each 8-aligned — inter-TU 4B pads
   are C-unreachable (struct align 2, zero filler possible); (c) dtk
   pad-inclusive sizing extends to .sdata @-strings (content
   byte-IDENTICAL rows are display-class); (d) typed struct-array
   brace-init = 0 @-ids (extends 98 to typed aggregates; MWCC aligns
   struct arrays by member, string-pool entries by 4); (e)
   byte-proven-dead data tell: target reaches a region ONLY via
   `addi rX,rBase,BIG` with ZERO relocs ⇒ dead strings/table tail;
   (f) an undefined extern cannot link Matching — in-TU def required
   (the gm_182F flip enabler).
115. **NAMING-CHAIN ACCOUNTING (naming-14)**: (a) stale-id 3rd live
   catch — queue tail ids drift by land time; ONLY same-session live
   derivation is safe (extends 99/103); (b) pairwise-colliding rename
   sets apply collision-free in section-address-ASCENDING order
   (.sdata2 before .data — each new name freed by an earlier pin;
   20/20 zero transient dups); (c) name-keyed snapshot accounting
   LIES on rename chains (8 phantom rows incl. a None→100 shown as
   DOWN) — use value/address-keyed accounting when names are reused.
116. **⭐ FORCE-ACTIVE KEEP BIT GATES ZERO-IMPORTER FLIPS (hsd_40FF,
   binary-proven)**: mwld deadstrips a compiled object whose only
   global has zero importers (DOL shrank EXACTLY 0x20bc0, all
   downstream addresses shifted); dtk-extracted objects survive via
   MW .comment per-symbol flag byte 0x08 (force-active) — MWCC emits
   0x00; ldscript FORCEACTIVE has only 4 CRT entries. Cure:
   `#pragma force_active on` (codebase precedent mobj.c:379) — makes
   .comment byte-IDENTICAL. linkcheck.py does NOT model deadstrip —
   audit zero-importer exports before every flip gate.
117. **VOLATILE-CAST EXTERN-F32 READ (extends 25; won it_8026F3D4
   →100, the itdrop flip enabler)**: scheduler lfs-pairs-with-lis
   canonicalization is breakable ONLY via a volatile-qualified
   extern-f32 read — `*(volatile f32*)&ext` still emits the plain
   `lfs sym@sda21` (no address materialization) but gains a hard
   ordering edge against a preceding volatile store (target
   lis/addi/stw/lfs order). Negatives: in-loop copy-assign hoists to
   a post-`b` slot, not preheader end (idiom-14 route fails); inline
   f32 helper costs a 4B inline-temp frame slot (consistent w/ 109b).
118. **FLIP-grade vs SCORE-grade RECON SPLIT (extends 105/111;
   lb_00CE, byte-measured)**: dead-creator pool heads CANNOT flip —
   idiom-111's uncalled-static device pools correctly but EMITS its
   own .text (0xac, verified in ftcoll.o too — 111 is SCORE-grade
   only); `if (0)` emits nothing AND pools nothing (dies pre-pooling,
   +2 @ids); called-once static inline pools at CALL SITE, not
   definition; Fix-B defs can't reach mid-batch entries (2.0f inside
   the expf batch) and true pool entries are not defs (pool/def
   namespaces disjoint). Bonus: backend conversion-magic pools at
   function END, after frontend + inline literals.
119. **INVENTED-LOCAL FRAME HOME (extends 107; quatlib MatToQuat
   →100)**: a decomp-invented staging local (`tr`) = +4 byte-invisible
   phantom scalar home; deletion-only fix — original reuses the
   surviving var via self-read (`s = sum; if (s > 0.0F) { s =
   sqrtf(1.0F + s); … }`), frame 0x50→0x48.
120. **POOL-ORDER INTRA-FN LEVER + dtk @-BY-APPEARANCE TRAP (extends
   78; quatlib 8037EF28)**: an embedded assign inside a call arg
   (`sinf((f32)(M_PI_2 * (1.0F - (t2 = 2.0F * t))))`) reorders pool
   CREATION (pi/2 before 2.0f) with .text byte-identical. TRAP: dtk
   numbers @ids by .text APPEARANCE order, not address — a creation-
   order fix can show as a fuzzy DROP that is a pure @N↔@M name swap;
   judge by resolved offsets (extends 91).
121. **MWCC VN-FORWARDING LAW (quatlib EC4C, 6 formulations)**:
   single-use scalar copies re-anchor at the use site through EVERY
   sequencing construct (statement swap, staging local, goto shim,
   if(0) second use, comma sequencing) — FPR temp-pair creation order
   is value-numbering-canonical, unreachable from C sequencing
   (rotation-park family).
122. **F32 ZERO CONST EMITS .SDATA2 (refines the ≤8B-zero law of 98;
   quatlib)**: `const f32 x = 0.0F;` at EOF DOES emit 4B .sdata2
   (unlike zero u32/.sbss classes) — one-line gap-tail filler +
   export-closure flip enabler (restored section size AND the
   gap_11 export byte-EQUAL post weak-shift).
123. **DEPTH-0 AUTO-INLINE (refines 73/83; itdraw EBC8/EC54)**: under
   `-inline auto`, earlier-defined plain GLOBAL callees auto-inline at
   expansion depth 0 EVEN WHEN LOOP-BEARING — the never-inlines law
   holds only at depth ≥1. Corollary: target `bl`s to earlier-defined
   helpers prove the original used depth-1 static inlines —
   macro/open-code rewrites there are fake-match territory.
124. **INT→U16 FIELD STORE (extends 43; gm_801BFC60 →100)**: assigning
   int to a u16 field emits bare `sth`; an s16 field forces
   `extsh r0` + `sth`. Caller-side: target passing narrow args as
   plain `addi rD,rS,0` with NO extsh = 32-bit params (unreachable
   from s16 params) — the int-params + unsigned-fields package fixed
   4 call sites + callee in one edit.
125. **INLINE-PARAM == INLINE-LOCAL EQUIVALENCE (itdraw P9)**:
   `T* p = gobj->user_data;` as an inline's local vs the same
   expression fed as the inline's param is byte-identical incl.
   colors — never probe both.
126. **LEFTOVER-CS ANONYMOUS ADDRESS-WEB (extends 29/39; gm_1BFA)**:
   the anonymous address-web takes the LEFTOVER callee-saved reg
   after all named webs are assigned, every run; copy-coalesce into
   it happens only when the named var's own turn picks that reg.
127. **MSL-MATH PACK (mwcc 1.2.5 -O4,p MSL flags; byte-proven)**:
   (a) `(float)`-cast contraction blocker — `c + (float)(a*b)` →
   fmuls+fadds, `c + a*b` → fmadds (either-side mul; zero cost);
   (b) visible-init const u32 refs const-prop to lis/addi —
   `*(u32*)&sym` defeats it and emits the named lwz @sda21 (extends
   24 to integers, 26c to non-statics); top-of-file defs emit at
   .sdata2 head BEFORE pool; (c) visible-init const FLOAT refs fold
   to anonymous pool DUPS even under `-str pool,readonly` (24 holds);
   (d) named float locals reserve 4B frame homes in branched fns but
   NOT straight-line — trigger is STRUCTURAL, not count; `register`/
   pragmas/flags don't remove homes; (e) frame bands: scalar
   memory-homed pairs ascend in decl order; aggregates stack ABOVE
   scalars; multiple arrays reverse-decl; aggregates ≥8B 8-align;
   (f) GPR base-shift tell: deleting an address-taken scalar pair
   shifts unrelated int-web maps down one reg; (g) mwld strips
   unreferenced NON-weak .sdata2 objects (extends WEAK-STRIP).
128. **⭐ MWCC CS CHOICE LAW (rotation-enum w20, solver-proven unique
   fit over 7 colorings)**: scan circular-DESCENDING from the cursor
   (= the previous web's register), DIRTY regs first, take the first
   non-interfering; else the highest VIRGIN reg. Same-reg "chains" =
   consecutively-processed disjoint webs; rotation parks are cursor
   artifacts. See the ROTATION-LAW VERDICT section (L1).
129. **ANCHOR-PAIR LAW (extends 126; rotation-enum w20)**: the
   anonymous @ha/@l address-anchor web allocates immediately BEFORE
   its first copy-derived named web — one-above-the-walker in every
   permutation; anchor-below-walker targets are decl-UNREACHABLE
   (gm_801BFCFC residual's deadlock proof in rotation-enum-w20/
   REPORT.md).
130. **IDX-CHAIN JOIN LEVER (extends 1/57; won +0.22 real,
   gm_801BFCFC)**: declare the LATER loop's index first to rebind the
   earlier index onto its register chain (cursor-join steering).
131. **DECL-ORDER REGIME SPLIT (refines 1; rotation-enum w20)**:
   decl-order-direct processing holds only at ≤7 CS webs; above that,
   structural promotion (whole-fn multi-def webs first with decl
   tiebreak; LICM consts ~3rd; per-BB local pass for call temps) —
   explains the old 24-perm cap. TU-CONTEXT caveat: identical replicas
   color differently out-of-TU; probes give laws, not byte answers.
132. **EN-MASSE IMPORTER-CLOSURE RANKER LAW (file-flip w20; flip #6)**:
   importer closure must be computed across ALL link inputs inside the
   ranker itself — bytecode sat flip-ready ~2 waves buried under 26
   zero-importer EXPORT-MISSING rows below every top-10 cutoff.
   Automated in `campaign/scratch/file-flip-w20/rank.py`.
133. **FUZZY-100 ≠ LINK-CLEAN (inverse of 88; file-flip w20)**: unit
   fuzzy 100.000 does NOT imply flippable — ftPr_SpecialN and
   itlinkbomb both fail linkcheck on .sdata2 POOL ORDER alone (pool
   swap/rotation + tracking relocs, ZERO .text bytes; idiom-120/122
   class). Always linkcheck, never score-gate flips.
134. **INTRA-OBJECT RELOC REACHABILITY (narrows 116; byte-proven at
   link, bytecode)**: an object whose .text has ≥1 imported export
   keeps ALL its data sections via intra-object .text→data reloc
   reachability — `#pragma force_active` is needed only for
   zero-importer SECTIONS (data-only units like hsd_40FF).
135. **SAME-SECTION @ha LIS-FOLD IS DIRECTION-GATED (lbaudio w20)**:
   two same-section symbol materializations fold onto one lis only
   for ASCENDING-address def order; descending blocks the fold
   (zero-cost two-lis row fix). +0 blockers do NOT block the fold.
136. **DESTRUCTIVE LAST-LOOP PTR-WALK (extends 100d/121/125; lbaudio
   27488 +3pp, 27648 →100)**: a final loop walking `*p/p++` makes
   earlier loops' streams copy out and the finals coalesce in-place
   into the materialization regs (preserved-base shape). All
   pointer-local walker aliases (copy, +0, ++-walk, identity-inline,
   const re-init) VN-canonicalize; identity static-inline result
   copies VN-forward.
137. **CROSS-CALL ADDRESS-NODE REPEAT LAW (lbaudio w20)**: repeat
   occurrences of a constant-address node across calls route through
   a CS addi temp (first occurrence per node = direct d-form);
   cast-blocked second-pointer mediation splits one site per +1 id,
   two mediations spill. NEGATIVE on idiom 117: a volatile-cast read
   pins the LOAD, not the address web.
138. **DEPTH-0 AUTO-INLINE IS SIZE-GATED (refines 123; lbaudio
   27648)**: large loop-bearing plain globals do NOT depth-0
   auto-inline (bl emitted) — the depth-0 exception holds only for
   small callees.
139. **EXPORT-SCOPE SOURCE FIX (extends 120/133; gm_1BFA w21,
   byte-proven)**: EXPORT-MISSING row + symbols.txt scope:global ⇒
   the decomp wrongly made it `static` — drop the static; sections
   stay byte-identical (only .comment moves) and the row clears free.
   Audit ALL importer-cleared EXPORT-MISSING rows against symbols.txt
   scope.
140. **ANCHOR-PAIR IS A SLOT LAW (refines 129; cs-law w21,
   solve2.py)**: one-transposition rotation parks are pure-L1-REACHABLE
   (gm_801BFCFC: 226 consistent orders with anchor AFTER w1; itdraw:
   comp1 processed last) — the blocker is the processing SLOT. Decl
   order, comma/for-init, block scope, variable splits, inline
   decl/definition order, TU position are ALL slot-inert. Pends a
   slot-inverter (p09/p10 anomaly = only observed mechanism).
141. **TAIL-MERGE TRAP (extends 73/83; itdraw V6 47.59)**:
   byte-identical sibling expansions from ONE static inline get
   cross-jumped (MWCC tail-merges them) — duplicated target code must
   stay duplicated in source.
142. **TU-POSITION FALSIFIED FOR REGALLOC (sharpens L5; cs-law w21)**:
   L5 sensitivity is linkage/symbol context, never function position
   or vreg counters — prepending 1-3 dummy fns (incl. a 15-CS-web
   monster) leaves coloring byte-identical; an extern-buf reshape
   moves it.
143. **SCOPE-OVERLAY FRAME LAW (extends 127d/e; logf →100, real+
   synthetic probes)**: sibling-scope locals OVERLAY one unified
   lexical top-down frame band (scalars+aggregates together — refines
   127e's separate-bands claim); a case-block local hides 12B (cast-
   init: 4B home + 8B cast staging) / 4B (copy-class init) / 0B
   (inline use) of frame depending on init class — frame diffs can
   come from a DIFFERENT scope than the visible slots. An inline
   switch-head cast read front-end-CSEs a case-local cast-init down
   to copy-class (12B→4B). Companion: address-taken u32 scalar pair
   pins int webs (127f).
144. **ALIAS-CLASS SCHEDULING EDGES (extends 127; logf probes)**:
   local ARRAY stores carry may-alias edges against global loads
   (scheduler orders them); scalar and cast-pointer-to-local stores
   do NOT — array-vs-scalar staging choice is a zero-byte scheduling
   lever.
145. **FPR TEMP-POOL LAW — L1 DOES NOT TRANSFER (quatlib EC4C
   mechanism proof; closes the park)**: volatile FPR expression temps
   = fresh VIRGIN regs descending from highest free volatile in IR
   creation (statement) order, no cursor/dirty-first; adjacent result
   fadds coalesces dest=src2; named single-def multi-use f32 →
   ascending band f7+ (overflow CS f31); named multi-def f32 →
   L3-promoted ahead of loads (131 transfers to FPR named webs).
   Cross-statement anonymous y-half lifetimes are C-unreachable
   (single-use copies VN-forward per 121; named webs band/promote).
   See FPR-LAW VERDICT in the rotation section.
146. **NAMED-DEF POOL-SLOT INTERLEAVING (extends 122/127b, closes
   118's mid-pool gap; ftPr_SpecialN + itlinkbomb, byte-proven)**:
   file-scope `const f32` defs emit into .sdata2 at their LEXICAL
   position INTERLEAVED with anonymous pool literals in creation
   order — between-fns def lands between those fns' pool windows,
   top-of-file at HEAD before all pool literals, EOF at tail.
   FLIP-grade pool-position creator: zero .text, named export
   restored free.
147. **FLOAT CONST-PROP CAST CURE (extends 127b to floats, narrows
   127c)**: a visible-init const f32 referenced by NAME folds to an
   anonymous pool DUP under melee `-str reuse` too; `*(f32*)&sym`
   re-type read defeats the fold with byte-identical lfs (volatile
   NOT required — both probed). 146+147 together retarget any
   literal's pool slot to any lexical point.
148. **DEAD-CREATOR DEATH IS UNIVERSAL PRE-POOLING (extends 118)**:
   fully-dead locals AND param-gated inline branches folded
   post-substitution both die PRE-pooling — no anonymous dead creator
   exists from C; idiom 146/147 is the only flip-grade pool-position
   route (mechanism behind lb_00CE's C-unreachable verdict).
149. **SR-ROVER RECON TELL (grgreatbay 63F4 →100)**: decomp loops of
   the form `arr[off/K]` + `off += K` are decompiler-INVENTED —
   direct `arr[i]` lets MWCC create the target's strength-reduction
   rovers, incl. the `addi rR,rI,0` rover-init of a known-zero i
   (not a user copy) and hit-branch `slwi` recomputes.
150. **CURSOR-SIM PREDICTIVE AT ≤7 WEBS (grgreatbay 63F4)**:
   hand-simulating idiom-128 L1+L2 over the decl order predicted a
   full 4-CS-reg map BEFORE compiling (first fully-predicted S1 win)
   — simulate before probing.
151. **MUTATION-DEMOTION IS NON-COMPOSITIONAL (refines 34; grgreatbay
   5460)**: idiom-34 self-read demotions do NOT compose across
   subsets — a `+0` form dissolves where the cast form acts; probe
   the exact subset, never extrapolate from singles.
152. **PER-SITE INLINE-AGGREGATE STREAM (extends 31/41/49; sobjlib
   mechanism)**: expanded-inline aggregates + arg copies form
   bottom-band per-site groups — one descending stream of [arg-copy
   above staging] per call site in instantiation order; reproducible
   with local static-inline wrappers, but the stream bottom-pins
   (dead-bottom fill below it = the cobj/lbColl parked family).
153. **CALL-BEARING INLINE 8B CHARGE — FIELD-CONFIRMED (extends 49;
   grgreatbay 499C)**: a call-bearing helper reproduced the target's
   two Vec3 slots exactly but paid +0x10 frame; depth-2 re-routing
   does NOT drop the 8B charge (the depth-2 hole-kill of idiom 49 is
   sqrtf-hole-specific).

### Experiment results (wave 3)

- **BSS/sbss ordering rule (8-compile evidence)**: statics allocate at their
  DECLARATION point; extern .bss (>8B) at FIRST CODE REFERENCE (sub-function
  granular); unreferenced externs flush at end-of-TU in REVERSE decl order;
  extern .sbss (<=8B) always flushes reverse-decl regardless of use; size
  and alignment never matter. Lever for wrong order: make the early-target
  symbol `static` (if only locally referenced) or reorder first uses.
- **Naming rows**: objdiff pairs relocs by exact name OR identical
  section-relative address — so naming rows appear ONLY when offsets drift
  (e.g. duplicated sqrtf localstatic literals shift .sdata2). Fix: name the
  target symbols in config/GALE01/symbols.txt (split re-runs via ninja,
  ~1.2s). Procedure documented in wave-3 report; FRAGILE: '@N' indices
  shift when src edits change literal pools — re-verify after src changes.
  Big targets: mnCharSel_802640A0 (154 naming rows), toy.c fns (65-146).

### Known-hard regalloc patterns (decl-order rule does NOT reach these)

- **Induction-temp pairs**: when the swap is between a user variable and a
  compiler-created strength-reduction temp (roving pointer), declaration
  shuffles shift OTHER registers and usually make fuzzy worse. Example:
  lbDvd_CachePreloadedFile (99.62) — failed: i-first, i-last, init-at-decl,
  loop-shadowed block-local (98.1-99.4). Matched siblings in the same file
  use the identical loop shape, so the lever is in this function's pre-loop
  variable mix. Needs systematic enumeration (wave-3 dedicated agent).
- **Inline-copy pairs**: under #pragma inline_depth(N), inlined callees
  instantiate their locals per level; a swap between level-1 and level-2
  copies is controlled by the CALLEE's body, not the caller's. Editing the
  callee risks breaking its own (matched) function + other inline sites.
  Example: lb_80011C18 (99.69) — mismatch entirely inside inlined
  lb_80011B74 tail. Failed: init-at-decl. Treat as expert tier.

## Work streams

- **S1 regalloc sweep** (79 fns + regalloc-dominant mixed): declaration-order
  and temp-variable idioms. Dominant pattern GPR_ALLOC_SWAP (callee-save
  numbering). Known idioms: declaration reorder, temp introduction/removal,
  expression reassociation. Watch non-monotonicity (sibling regressions).
- **S2 layout per-unit** (69 + 56 mixed; clusters): cross-reference
  `nm --format=sysv build/GALE01/src/<unit>.o` against
  `config/GALE01/symbols.txt` (DOL ground truth) to find displaced/missized
  statics. Verified first target: **cobj.c stack-frame** (PAD_STACK at lines
  796 AND 803; 6 fns from one fix — critic-confirmed). gmregclear struct
  size (has unexplained 0x110 gap — investigate, don't assume).
  ✅ particle BSS RESOLVED (wave 9): full 13-symbol ordered static block
  reconstructed in particle.c at exact target sizes/offsets; CF7E8 kept
  extern deliberately (3 sibling 100s depend on extern fusion shape).
- **S3 structural near-miss** (419): root causes per phase-0 analysis:
  anon-bss cascade ~137 (depends on S5), small 1-fix ~97, type
  sign/zero-extension (extsb/extsh/clrlwi from wrong types), inline-vs-call.
- **S4 codegen** (197): mnemonic-level fixes; includes balanced-reorder
  scheduling cases. MIN-macro operand flips, mr-vs-addi, fmr-chain ordering.
- **S5 config/naming** (cuts across ~150+ fns): name anonymous `@NNN` sdata2
  literals and `...bss.N` symbols in `config/GALE01/symbols.txt`. Fork-legal.
  ✅ OPEN QUESTION ANSWERED (wave 18, byte-proven at link): naming-only
  functions ARE byte-correct in a linked Matching unit (ftchangeparam flip
  carried 219 LIT2EXT rows) — S5 is bookkeeping/score work that unlocks
  gates, NEVER byte work for flips (idiom 110e).
- **S6 hard tail** (11 fns): THPDec.c cluster (5, in extern/, sbss-vs-bss
  placement + Huffman struct access), grZebes_801DA0C4 (static table copy —
  mechanical), HSD_CObjGetLeftVector (de-inline roll2upvec; 104 extra rows
  from over-inlining). cobj.c belongs to ONE owner with S2's stack fix.
- **S7 scratch harvest** (4 claimed-100% scratches): decomp.me API is
  Cloudflare-blocked; use browser automation to fetch. Attribution required
  (authors in scratches.txt). ⚠ grOldYoshi_8020F31C: our source (79.7%)
  BEATS the 58% scratch — do not transplant.

## Pre-session-2 experiments (cheap, high-information)

1. **MWCC BSS ordering rule**: empirically swap two statics in a scratch
   copy of particle.c, rebuild, observe .o symbol order. Declaration order
   alone does NOT explain observed order (verified). Blocks ~137 anon_bss fns.
2. ✅ **Naming-only byte-correctness — SETTLED wave 18**: three flips
   committed (51951826b + 4715a41d8); naming rows are link-irrelevant at
   equal resolved offsets. See FIRST FLIP VERDICT + idiom 110.
3. Re-validate one S1 win end-to-end (edit → 100% → unit clean → DOL OK →
   commit) to prove the full loop before fanning out.

### Parked (expert-tier, do not re-attempt without new theory)

- **cobj.c cluster (6 fns)**: NOT a PAD_STACK bug — target frames have temp
  pools +4/+24 bytes higher with dead space at frame BOTTOM, unreachable
  from user C under this config. ~20 forms tested. Instructions identical.
- **HSD_TExpSimplify2 (99.97)**: one instruction; ~50 total compiles across
  two agents; every loop form/pointer style/decl permutation enumerated.
  Loop-1 roving init derives from homed copy, target derives from raw r3.
- **gm_1601 pair**: first-read-order vs callee-saved-perm coupling unsolved;
  see wave-3 report for the empirical law.
- **itcoll it_80270CD8**: FP attractor analysis complete; likely needs a
  shared static-inline helper for the stale-damage formula (cf. matched
  sibling it_80270E30). Worth ONE retry with the helper approach.
- **gm_1601 +0x110 anchor-fold family — RESOLVED by the wave-15 split**:
  gm_8016A164 →100 EXACT; A22C/A4C8/AC44 anchor rows gone (residuals now
  ordinary S1/structural, see split report); 95BC/9A84 were never
  anchor-blocked — pure S1 rotation, now in clean TU context (S1 queue).
  Wave 18: **fn_801695BC →100 WON** (idiom-113 full rewrite, 16 compiles);
  fn_80169A84 74.20→**94.26 parked-at-best** (shape-exact; ~18 residual
  rows = entry lis-temp r6-vs-r3, loop-B i2/p2 2-cycle, dead IV
  kept-with-CTR, loop-D just-freed-reg chain → rotation-enumeration agent).
- **lbColl_800077A0 residual (4 rows)**: target has a dead 4-byte temp at
  frame 0x34 before sqrtf instance 1; 6 forms failed (PAD_STACK/trailing
  local/block volatile/warm-up call all wrong or shift @ids).
- **Conversion magics (0x4330…) & jumptable_* names**: per-TU duplicate
  copies / compiler-generated names, unreachable from C (re-confirmed
  gm_18A5, particle, camera S32_TO_F32). Naming/diff-policy problem only.
- **mplib arg-copy temp placement** (mpLib_80059E60): ours pools fn-wide
  descending, target per-block ascending — emission rule uncracked.
- **f31/r30 ANONYMOUS-TEMP ATTRACTOR (toy ×3: un_803087F4/80310324/803109A0;
  same family as mplib arg-copy)**: call-result copies consumed inside
  inline expansions, merged multi-def FP vars, and SR roving pointers grab
  the TOP free callee-saved reg; target wants BOTTOM (f27/r26). ~8 levers
  failed (named temps, inline-body locals, wrapper, decl/def order,
  init-at-decl, source-redef, assignment-in-condition).
- **WEB-ASSIGNMENT-ORDER ROTATION (gmregclear gm_8017DB88/fn_8017D9C0/
  fn_8017FF1C/fn_80181C80)**: param-block vs local-block placement/rotation
  invariant to all decl/def/statement permutations (~20 probes). Decl order
  only controls relative order WITHIN local groups. Needs a dedicated
  enumeration agent before retry. Post-split (wave 12): fn_80181E18's
  residual is now ONLY this family — d-form/fold rows all fixed by the
  gm_181A split.
- **ftCo FY-PROP family (NEW wave-12 park; caps A6A98 ≈96.1 / A6D2C
  ≈95.6)**: a float copy of an escaped stack aggregate's field
  (`fy = floor_pos.y` after `&floor_pos` passed to a call) is
  copy-propagated to per-use reloads. NOT blocked by separate-statement
  def, (f32) self-cast (folded), (f32)(f64) round-trip (emits code), or
  inline-return getter. Only proven in-unit blocker = A4768's shape: the
  source aggregate is REDEFINED after the copy. Target holds fx/fy in
  f28/f27 (+1 FPR save, frame +0x10). Wave-13 theory: add a second def
  of the aggregate.
- **ftCo_0A01 residuals**: B2790 webA/line_id r28↔r29 tie (wave 11: +4
  more failed levers — helper-param direct pass, single-var web,
  result-named, line_id-decl-first; stays parked); ~~B1478 inline-data
  r28/r29~~ **WON wave 11 →100** (idiom 66 helper swap; the LIFO
  free-stack model's lever was the preceding region); ~~A2718~~ **WON
  wave 12 →100 EXACT** (idiom 73/74 package); A6A98/A6D2C → FY-PROP
  park family (above); A6FC4 root cause = full FPR map rotation (@471
  at f31 vs our f21) + 1 extra GPR; AC5A0 dead 8B
  between y and f2i temps; A9904/A9CB4 dead-below-y + `fadds f1,f1,f31`
  canonical operand order (both source orders emit f31-first); A61D8
  loop-group-vs-invariant coloring; AE7AC r4-vs-r7 pick + inverted mr/addi
  param-homing pair.
- **gm_1832 residuals**: fn_801851C0 r30/r31 LICM-temp swap (8 forms);
  fn_80188550 + fn_80188644 TU split **LANDED wave 11** — now 99.18
  (8 pure S1 regalloc rows: r28↔r25/r31↔r26 swaps) and 99.34 (→100 pends
  TrainingModeState +0x114 header split); fn_801891F4 96.31 web reshape
  queued for S1 (−0.016 to recover); fn_80189B88
  tail zero-share (store-zero and return-zero same vreg, 4 forms
  const-propped); fn_80187CF4 st/jobj/gobj cyclic perm (identical source
  shape to matched fn_80187AB4 allocates differently).
- **lbaudio_ax unsolved**: bss-vs-data anchor callee-saved rank swap
  (26C04/27168/2838C); loop-counter-first volatile rotation (decl/def/init
  all no-ops); ud/sp param-copy homing swap (25FAC, 5 levers); 26EBC u64
  inline-result r3/r4 coalesce (expert inline-copy class).
- ~~it_8026F3D4 (95.56)~~ **WON wave 19 →100.00** — the volatile-cast
  extern read (idiom 117) breaks lfs-pairs-with-lis; itdrop flip enabler.
  **fn_802FF218 (94.33)**: y↔thing callee-saved swap + r6
  arg-copy site, regalloc-resistant pair.
- **Wave-19 parks (updated wave 21)**: gm_801BFCFC residual r27↔r28
  transposition = slot-law park (idiom 140; ALL 4 queued ideas + 23
  decl perms resolved w21 — pends a slot-inverter); it_8026EECC
  allocator-tie ONE-transposition state — V7 99.26 recipe REBUILT +
  saved (cs-law-w21/itdraw_V7_99.2628.c), all idiom-130 levers inert,
  same slot-inverter class (GPR-side, L1 applies); ~~quatlib 8037EC4C~~
  PARKED DEFINITIVE w21 with mechanism proof (idiom 145 — do NOT
  re-attempt from C); lb_8000D148 fresh-reg rotation (true shape at
  99.06 documented, reverted); ~~MSL logf 18B~~ **WON wave 21 →100.0000
  MATCH** (scope-overlay frame law, idiom 143 — MSL/math link-clean).
  lb_00CE pool head = DEFINITIVE dead-creator park (idioms 118/148; do
  not re-attempt for flips).
- **gm_18A5 wave-9 parks**: fn_8018E46C lwz/neg pair-order attractor (16
  rows, 6 levers — anonymous-temp attractor family; arg-reshuffles drift
  @991→@992); fn_801977AC single lwz/mr schedule row (6 levers; matched
  sibling fn_80197AF0 proves canonical source emits mr-first); lbzu-fusion
  vs add-operand-order are MUTUALLY EXCLUSIVE from C (fn_801913BC, 5
  forms: array-index fuses but orders (idx,base); ptr-arith orders
  (base,idx) but unfuses); fn_80191240 jobj2-copy reconstruction (cast-copy
  coalesces, plain copy over-splits; 98.09 stands — wave-17 NOTE: both
  91240 and the 90ABC reg-web/pool-order item live in gm_190A.c (TU-B)
  post-split, NOT gm_18A5.c — routed to the gm_190A.c owner).
- **gm_1601 wave-9 parks**: gm_80164A0C/gm_80164910 pair ROOT-CAUSED —
  allocator never gives a fresh scratch web the just-freed r3 where target
  does (15 probes; auto-inline reconstruction moves i r6→r4 not r3;
  gmregclear-rotation family; idiom-29 r3-rebind claim disproven there);
  fn_801652D8 lone lis one slot early (9 probes, idiom-15 singleton; NB
  idx=1-before-call forces idx into r28 across the call); gm_801674C4
  temp_ptr r3→r30 residency split point (10 forms; blockers invert the
  whole map); fn_80169F50 reverted (r3-avoidance family); residuals on
  fn_80161154 (spC walker split, 4 probes) / fn_80167638 ((idx,ret,arg2)
  3-cycle, decl-invariant) / fn_80162170 (ck cmp-temp r0+mr vs precolored
  r3) / fn_80161C90 (count vs SR-roving r3/r4 swap) — all rotation family.
- **ftCo_0A01 wave-9 parks**: ~~AF290 residual (99.38)~~ **WON wave 11
  →100** (sibling-shape rewrite, idiom 66; −13 @-debt repaid exactly);
  AC5A0 +3 failed forms (depth-2 conv helper moves y not the gap) — keep
  parked; A3908/A4038 needs a dedicated session WITH an @-id budget plan —
  wave-11 diagnosis upgrade (intel in ftco0a01_w11/REPORT.md): (1) mixed
  addressing byte-verified — first blast-read fp-direct, rest via a
  SECOND anchor, win-2 stores via anchor2; (2) data2 web materializes
  ONLY via a cross-BB def (idiom 65); (3) materializing the 12th web
  flips stage_info@l rank-9→rank-1 (wraparound blocker); (4) target has
  PER-ARM block-scope locals (+2 @ids each × 8-12, result/valid colors
  SWAP between arms); (5) `valid=0` li-before-NULL-stw fusion and (6)
  beq/beq/b dead-b guard both fold-resistant (idiom 66 negative).
- **mplib wave-9 parks**: NextNon×8 / Get×8 / 534FC×4 / mpLinesConnected /
  800581DC / 80058614 / DrawMatchingLines-rotation / 8004ED5C — one
  volatile-web/inline-result rotation class (6+6 permutation sweeps,
  non-monotonic); mpCheckFloor (55 rows = single phantom-band shift);
  80055E9C loop-invariant FPR pair tie; 56C54 sqrtf instance-band;
  RespawnVtxIds 80% + 804D8150 66.7% are dtk pad-fold size rows —
  diff-policy, unreachable from C (like gap_07/gap_09 rows).
- **particle wave-9 parks (updated wave 13)**: ~~TU-SPLIT-BLOCKED
  residual class~~ **RESOLVED by the landed split** (D08E8 now its own TU
  base at offset 0; 9F05C/9930C/extab rows healed); @3751 dup 0.5f
  .sdata2 per-original-TU copies now naming-10 renumber rows; .data
  shortfall localized to HEAD's debugconsole dead strings (~0x2BA4) +
  hsd_3983's extra @1491/@1492 object.h pool pair — still queued for the
  reconstruction agent (idiom-24/51 style, with @id-shim accounting).
  Post-split S1 residuals — **hsd_3983 S1 RUN wave 16** (~55 compiles,
  3 binary-proven deviation fixes): 98A08 62.63→**97.52** (void*[146]
  = 3 original statics [16]+[65]+[65] + rolled 65-iteration loop),
  psInitDataBankLocate 75.74→**91.11** (phase-2 mask
  0xF000000F→0xF1FFFFFF rlwinm-proven; case-0 num=0 — phase-2 fixes
  kind bits on ALL v0 entries; phase-4 deref-before-NULL-check
  restored), 9930C →94.21 frame re-EXACT — **case 0xBD `mag > 1e-10F`
  = the un-located 1e-10 guard FOUND** (lbl_804DE98C, spill 0x268
  byte-proof; the blocked naming-10 pin is now UNBLOCKABLE);
  [.sdata2-0] →92.31; remaining parks: A08 zero-web partition (7
  forms), Locate named-web rotation + 2 mr/addi + v40 LICM addr,
  9930C broad regalloc + remap attractor, 98F8C FPR rotation, D0A0
  lwzu-fusion; gate DOWNs (hsd_804D0908 100→50, psInitDataBankLoad
  →99.86, [extabindex-0] −0.39) heal via the queued hsd_804D0908
  subdivision. ~~hsd_39D1 DAD4 86.18 + 9F05C~~ **RESOLVED wave 17**
  (S1 run: DAD4 →99.34, 9F05C →99.54, [.sdata2-0]→100, the f64
  2.0/π÷2 pins UNBLOCKED — residuals = DAD4 FPR 7-web rotation park
  + 9F05C frame-slack/fmr singleton; see wave-17 log + idiom 109).
- **gm_1601 wave-13 park — ROVER-INTO-BASE COPY COALESCE family** (3
  sites: fn_80164B48 block-6, fn_80169C54 bp, fn_80169000 p-rover):
  target folds the final walker copy into the dead base var's
  callee-saved reg; ours always emits `addi rX,rBase,0`; mutation form
  flips base ranks; fresh-var/recompute/decl-position all no-ops.
  lbaudio 28690's leftover-rover + zero-web-return fork (3 fresh failed
  forms, wave 13) strengthens the same family and the gm_1832
  fn_80189B88 park.
- **gm_18A5 wave-11 parks**: fn_801949B4 (95.00) / fn_80194BC4 (94.11)
  ptr-reassoc attractor (idiom 62, 8 forms enumerated); **fn_80195AF0
  98.07 ACHIEVABLE but reverted to 94.79** — winning package costs +3
  @ids breaking 9 landed pins — **wave-17: blocker CLEARED**
  (naming-13 landed @3025/@3027→@1679/@1681 + the gm_1965 recon
  unblocked @3554) — re-apply is FRONT of the gm_190A.c S1 queue
  (the fn now lives in TU-B gm_190A.c, NOT gm_18A5.c); exact text
  preserved in
  `campaign/scratch/gm18a5_w11/my_gm_18A5.c` + LOG; wave-12 park:
  fn_80192E6C residual = lbl_804DA738/75C rows — extern-swap
  blocked; **wave-17 re-probed post-split: STILL regresses (97.71)
  — loop LICM/CSE shape change, not just ids; the park is
  byte-load-bearing** (naming-13 also cost it lucky offset-pairing,
  99.979→99.96 — accepted; @768/@773 DO-NOT-RENAME stands,
  idiom 75); fn_8019175C residual
  jobjs 0x10-vs-0xc dead-word placement (instance-band hole family);
  gm_801905F0 residual inlined fn_8018F410 addi emission order
  (inline-copy class); fn_80192758/fn_8019249C post-call lwz/mr schedule
  singleton (fn_801977AC family) + r0-avoidance web; fn_8018F888 bss
  +0x20 artifact rows **HEALED by the wave-16 split** (+1.8; NB residual:
  target's two identical loops use DIFFERENT i/p reg pairs — {r3,r4}
  then {r4,r3}).
- **mplib wave-11 parks**: mpLib_DrawCrosses (98.47) r0+mr addi attractor
  — 16 forms enumerated incl. the re-run wave-10 matrix (a–l) + 3 new
  walker/indexed/direct-static probes; proves no cross-BB CSE of static
  addresses (rover re-materializes locally); ~~re-test after the
  mplib.static.h [0x80] header lands~~ wave-12 post-header retest DONE:
  NO change; +2 more probes (idiom-37b chained = exact no-op,
  helper+split = 97.29 reshape) — attractor now ~18 forms strong, park
  FINAL. mpLibLoad rotation park strengthened wave 12: 13 more probes
  (loop-form flips, +0 blockers, def grouping, decl-order permutations)
  all exact no-ops or ±1 @id; named-anchor form 97.95 REJECT; assignment
  = final-schedule def-order descending. All 38 remaining mplib <100 are
  parked classes; no naming rows left in unit. mpLib_DrawSnapping (99.98) = 2
  GXColor arg-copy slots 0xC low; 10-pt PAD_STACK sweep confirms idiom-41
  "nothing pads below instance slots" — current (0x30,0x40) is optimal.
  mpLib_80059554 (99.26) NEW schedule-sink sub-family: target hoists
  `lha count` to BB top interleaved with GXColor loads, ours sinks
  [lha;cmpwi;mtctr]; fresh-web = exact no-op ⇒ NOT a re-def split.
  mpLibLoad residual 9 rows = temp_r28/29/30 stack-ptr rotation (B1478
  LIFO-death family; def-reorder verified no-op).
- **lbaudio_ax wave-11 residuals (updated wave 12)**: B24 →98.79 (idiom
  58/67/68 cures landed); residual = synthetic-reloc symbol-subdivision
  rows ONLY (naming class: lbl_80433A64/B44, lbl_804D3870/74, gap_07_* —
  needs the bss 6-sym merge); 233EC 95.00 — ~50-row r3/r4 web swap =
  rotation family, parked (8 probes, all exact no-ops); 28690 97.81 —
  frame +8 + leftover-rover anchor parked (idiom-40 phantom family, ~14
  probes, bisection-proven owner = x2C/x70 loop); fn_80023254 LICM-hoist
  unsolved; fn_800268B4 rover r0+mr; 269AC rotation / 27488 u64-coalesce
  / 2785C — probes reverted; pooled-string offset rows now resolved by
  the wave-12 .data package (land `lbaudio_w12/full_tree_with_pkg.patch`
  WHOLE — flags_arr alone is net-negative). WAVE-13 UPDATES: 27DF8
  →98.70 / 28690 →97.84 frame-EXACT (both won, idiom 81); parks
  CONFIRMED with fresh probes — 233EC r3/r4 rotation + 2 dead-b sites,
  27648 {i,ptrA,ptrB} rotation (2 probes), 27DF8 preheader two-3-cycle
  rotation, 2785C u64 mr (fn-scope vs block-scope of a call-consumed
  local = byte-identical); B24 98.79 = SRC-COMPLETE — final 1.2% pends
  (a) the w12 package, (b) the symbols.txt bss 6-sym merge (objdiff
  resolves synthetic relocs against each object's own layout); re-run
  the B24 tail-web coloring AFTER those land (analysis in
  lbaudio_w13/progress.txt). NEGATIVES: idiom-34 +0 does NOT block
  const-prop (ret=0 fork) nor arg-position int-grouping reassoc.
  WAVE-16 UPDATES: ✅ **w12 .data package LANDED WHOLE** (git apply
  --3way + 27168 dead[10] stand-in removed; [.data-0] 46.95→**96.79**,
  2838C→97.81, 26C04→92.79, 267B0→99.15, 27AB0→98.49, 10 UP / 0 DOWN;
  whole-unit @-renumber benign per idiom 91; offsets_arr 59.89 +
  str_last_ssm 51.75 = display artifacts on byte-identical data —
  investigate objdiff data scoring); **233EC UN-PARKED →98.53**
  (FindSlot decl swap ranges-before-slot + fn-scope p hoist + comma
  order + 2 literal-return fold cures, idioms 93-96; w12 if(0) shim
  removed = exact −2 repayment; residual 2 mr rows = fold ceiling);
  B24 →**98.90** src CEILING (5-row web C-unreachable; rest pends
  ONLY the bss 6-sym merge); 28690 →97.93 (rover+i entangled ~19
  probes, park confirmed; @722 naming row). lbaudio unit-map
  recompute + idiom-72 analyze.py re-run trigger now ACTIONABLE.
- **Wave-14 inversion parks**: ftcoll .sdata2 3-entry pool PERMUTATION
  (pends the true shape of 7861C — prime suspect frame 0x48 vs 0x40 =
  one dead u32→f32 conversion temp pair; cracking it frees the 3
  near-100s 78A2C/7BC90/7ABD0 + [.sdata2-0]→100) + 77970 rotation web
  + 79EA8 FPR rotation + getEnvDmg ours-only + 7 size-mismatched
  structural fns (all improved); camera ~45 permanent style-conflict
  rows (zero/0.125/0.85/0.5/deg2rad/±2^31 — CSE-, attractor-,
  const-prop- and loop-LICM-locked classes; enumerated in
  inversion-camera-w14/REPORT.md) + 29CF8 extra callee-saved f26 web
  (needs a dedicated S1 session); toy @288 ×13 conversion-magic (park
  class re-confirmed binary-side) + 8F04 add-order ×8 + clamp ×3 +
  DB0/EB4/FB8 extern-rank-flip ×3 (stay literal, idiom 86) +
  un_804DDCFC .sdata-vs-.sdata2 row + big-fn structural gaps;
  tydisplay BF34 95.57 frame+4 + r30/r31 mr-vs-addi park (naming rows
  all paired), un_804D6F10 ×2 + un_804DDFB0 structural S3, quicksort
  ours-only; grvenom 80205AD4/80206874 rotation residuals → S1,
  802040F0 +0x28/+0x30 permanent src-side (idiom-75: renaming
  grVe_804DB740→@452 forbidden — extern-referenced from converted
  sites); SHADOW offset-0 parks throughout (camera cm_80452C68 ×78,
  ftcoll, tydisplay ×4 incl. B1FC/B328 .data layout d-forms — a full
  idiom-69 .data reconstruction would also fix B460; target byte map
  in tydisplay-w14/REPORT.md).
- **Wave-15 inversion parks**: sislib SHADOW offset-0 ×6 + S1 parks
  (5798 r3/r4, 594C coalesce, 67EC/70A0/7684/84BC structural) +
  803A611C 99.95 donor residue BY DESIGN (its baseline 100 was
  coincidental offset-pairing); lb_00F9 14638 f2/f3 swap
  (pre-existing) + 14770 extern-rank singleton (4-probe ledger in
  inversion-lb00F9-w15/progress.txt); gmresult fn_801857C4 li 0/1
  reg swap (S1 rotation) + 1 permanent donor-keeper row (idiom-90
  bracket; lbl_804D3FB0→@882 rename forbidden, idiom 75); grbigblue
  13 enumerated rows — 2 donors BY DESIGN (801E57C0 = @204/@205
  creation), 8 inline-setter args (+1 @id each + GCSE f30, idiom
  92), 1 sqrtf-internal (math_ppc.h `x > 0.0f`), 2 extern-rank-flips
  (8794 zero/range f29-f31 rotation, EF424 zero-above-LICM-doubles)
  — plus LC parks (store-group alias-barrier load counts, bulk
  multi-ref GCSE net-negative BAF8/D694/EEF00, B004 fval +1 id, CB50
  header-inline arg) and S1/S3 carryovers (8A1C r29/r30, B004
  bitfield+frame, CB50 stmw/rlwimi, EACE8 fnmsubs, structural
  6C60/E398/EEF00); re-run analyze.py after 93D8/A05C structural
  work. ftcoll [.sdata-0] stuck at 40.0 (naming-11 deviation):
  sections byte-match at 100 but dtk tail pads (.sdata 8 vs 2,
  .data 280 vs 276) + the 12B .sdata2 permutation penalize bracket
  rows — unreachable from symbols.txt, pends the 7861C S1 crack.

## Session log

- **2026-06-07 — Wave 21 (⭐⭐ THREE UNITS MADE FLIP-READY: ftPr_SpecialN
  + itlinkbomb pool-order CRACKED + MSL/math logf →100; ⭐ FPR-LAW +
  SLOT-LAW verdicts; grgreatbay 44B→31B w/ 63F4 →100; quatlib EC4C
  parked DEFINITIVE) — 0 commits (no grant); HEAD beb3bdc77 (= the
  wave-20 settle commit absorbing naming-15 + gm_1BFA + lbaudio);
  project unchanged 842/1041 (42.80%) but the NEXT THREE FLIPS sit
  gated-and-ready in uncommitted src; 2 NEW exact matches (logf,
  grGreatBay_801F63F4) + gm_1BFA EXPORT row cleared; ~145+ unit
  compiles + ~5 full-ninja gates, all DOL sha1 08e0bf20 EXACT.**
  ⭐⭐ **POOL-ORDER FLIP-UNBLOCKER**: ftPr_SpecialN AND itlinkbomb
  linkcheck hard rows **NONE** — whole-object .text + .sdata2
  byte-exact vs target (itlinkbomb .text untouched byte-for-byte).
  Mechanism: file-scope `const f32` defs emit at their LEXICAL slot
  interleaved with the anonymous pool (⭐ idiom 146) + `*(f32*)&sym`
  cast reads defeat the float const-prop pool-dup fold, volatile NOT
  required (⭐ idiom 147) + EOF 0.0F gap defs (122); dead-creator death
  proven universal pre-pooling (148 — mechanism behind lb_00CE).
  Residual EXPORT-MISSING ×33 importer-closure-cleared across ALL link
  inputs; 10 probes + 4 check_fn canaries; durable
  pool-order-w20/progress.txt. ⭐⭐ **logf SOLVED → MSL/math LINK-CLEAN**
  (the w19 ~50-probe park; ~49 compiles): root cause = SCOPE-OVERLAY
  FRAME LAW (⭐ idiom 143) — the case-block's local hides 12B/4B/0B of
  frame by init class, absorbed invisibly by baseline's float band;
  winning shape = inline switch head + outer decl band + case-EXP_MASK
  verbatim + lone `float H` + `(float)` contraction blocker +
  F32_BIT_CAST reads; check_fn direct AND --ninja 100.0000, linkcheck
  `[]`, DOL EXACT ×2 + idempotence. Companions: unified lexical frame
  band refines 127e; alias-class scheduling edges (144); synthetic
  harness w/ real MSL cflags in cslaw-parks-w21/. ⭐ **FPR-LAW VERDICT
  (idiom 145, new rotation-section subsection)**: L1 does NOT transfer
  to FPRs — volatile FPR temps are virgin-DESCENDING in creation order
  (no cursor/dirty-first), fadds coalesces dest=src2, named single-def
  multi-use f32 → ascending band f7+, multi-def → L3-promoted (131
  transfers); quatlib EC4C thereby C-UNREACHABLE with mechanism proof
  (E1/E2 fingerprints) — **parked DEFINITIVE, demoted in the flip
  queue, file reverted byte-exact**. ⭐ **SLOT-LAW VERDICT (idiom 140,
  refines 129)**: solve2.py proves one-transposition park targets ARE
  pure-L1-reachable (gm_801BFCFC 226 consistent orders, anchor AFTER
  w1; itdraw comp1-last) — blocker is the processing SLOT; ALL 4
  queued gm ideas resolved (A worse / B no-op / C deadlocked / D
  TU-position FALSIFIED, idiom 142) + 23 decl perms capped 99.2812;
  pends a slot-inverter (p09/p10 = only observed mechanism, probe
  queued). **gm_1BFA now SINGLE-hard-row 20B**: EXPORT-MISSING
  gm_8049E558 cleared via static→global (⭐ idiom 139 — audit all
  importer-cleared EXPORT rows against symbols.txt scope; sections
  byte-identical). **itdraw**: lost w19 99.26 uniform state REBUILT +
  recipe saved (itdraw_V7_99.2628.c — one copy once a slot-inverter
  exists); V6 shared-switch inline 47.59 = tail-merge trap (141); src
  reverted byte-identical. ⭐ **S1 grgreatbay 44B→31B**:
  **801F63F4 96.72→100.00 MATCH** — decompiler-invented `off/4` loops
  → direct `arr[i]` SR-rovers (⭐ idiom 149) + an idiom-128
  hand-simulation that PREDICTED the full 4-CS map pre-compile (⭐
  idiom 150, first predictive S1 win) + chain-assign (33); 801F5460
  97.30→**98.73** (idiom-34 self-reads; residual r29↔r31 = slot-law
  class; mutation demotion non-compositional, 151). **sobjlib parked
  w/ mechanism**: target frame = per-site inline-aggregate stream
  (152), 11-helper prototype reproduces it but bottom-pins at 0xc —
  cobj/lbColl dead-bottom family; reverted byte-identical;
  call-bearing-inline 8B charge field-confirmed (153, blocks 499C).
  **Flip-exec settle pass (w20b)**: zero flips (pre-unblocker
  linkcheck), rank.py coverage COMPLETE (197 + 2 src-less = all 199),
  flip-ready set was EMPTY — the latent zero-work pool is exhausted;
  every future flip needs src work, exactly what this wave delivered.
  **STALE-BASELINE FIND (s1 gate)**: grGreatBay_801F4300 99.9231 +
  801F499C 99.9898 are PRE-EXISTING at HEAD (stash-proven) — recorded
  baselines stale, backlog re-triage queued. **QUEUES**: flip executor
  = commit the 3 src wins, then full flip gates #7-9 (ftPr_SpecialN,
  itlinkbomb, MSL/math: rank.py re-run + 116/134 deadstrip audit +
  pgrep + ninja + DOL sha + idempotence ×2, ONE AT A TIME at the
  boundary per build-race law); rotation/slot = p09/p10 slot-inverter
  probe (unlocks gm_1BFA + itdraw + grgreatbay 5460); math_data.c:82-83
  deletion (side item, DOL-neutral); naming: quatlib @165↔@166 +
  lbaudio 6-sym merge carryovers; backlog re-triage (triage.py +
  grgreatbay pair). **WAVE BOUNDARY**: dirty = ftPr_SpecialN.c,
  itlinkbomb.c, MSL/math.c, gm_1BFA.c, grgreatbay.c (wins kept,
  uncommitted — no grant) + stale backlog.json; serialize commits →
  triage.py → full configure + ninja + dtk shasum over the settled
  tree → THEN flips. **WARNINGS**: (1) idiom-number collision ×4
  streams AGAIN (all self-assigned in the 137-143 range) — final
  numbers 139-153 assigned here; agents MUST cite candidates only;
  (2) build-race law HELD ×5 streams (pgrep idle before every gate;
  no full ninja for NonMatching-only edits); (3) sibling-dirt
  attribution clean ×5 (DOL byte-exact over dirt on every gate);
  (4) durable records held ×5 (pool-order-w20/, cslaw-parks-w21/,
  cs-law-w21/, s1-flip-w21/, file-flip-w20b/); (5) stale recorded
  baselines are a real hazard — two grgreatbay "100s" were not
  (idiom-103-style re-derive at gate time applies to BASELINES too).
  Idioms 139-153 added; FPR-LAW + SLOT-LAW verdict subsections opened
  in the rotation section; flip queue re-ranked (3 FLIP-READY at top,
  quatlib demoted to #8).
- **2026-06-07 — Wave 20 (⭐⭐ FLIP #6 bytecode ZERO-src-work + full-rank
  flip sweep + ⭐ ROTATION CHOICE-LAW SOLVED (gm_1BFA 67B→20B) + lbaudio
  S1 close + hsd_40FF recon verification; naming-15 IN FLIGHT at close)
  — 1 commit this wave (b6674faf6) atop the boundary commit d4fee40d1
  (= **flip #5 itdrop**, rode the w19 src commit); HEAD b6674faf6;
  project 840→**842/1041 linked (42.80%)**, matched 72.20%, fuzzy
  97.59; 1 NEW exact match (lbAudioAx_80027648) + gm_801BFCFC
  97.84→**99.28**; ~110 unit compiles + ~6 full-ninja gates.**
  ⭐⭐ **FLIP #6 COMMITTED b6674faf6**: sysdolphin/baselib/bytecode
  NonMatching→Matching, ZERO src work, 1 configure.py line — buried ~2
  waves under 26 zero-importer EXPORT rows; surfaced only by the new
  en-masse importer-closure full-rank sweep (`file-flip-w20/rank.py`,
  ⭐ idiom 132) over all 199 NonMatching units (bytecode the ONLY
  flip-ready). Deadstrip audit PASS WITHOUT force_active — intra-object
  reloc reachability (idiom 134 narrows 116); ALIGNFIX ×2 + weak
  surplus exempt; gates ALL PASS (pgrep idle each, sha1 08e0bf20 EXACT
  ×3, link edge consumes our bytecode.o, idempotence, 1-line commit
  isolation). NEW LAW: fuzzy-100 ≠ link-clean (⭐ idiom 133) — two
  fuzzy-100.000 units fail on .sdata2 POOL ORDER only (**ftPr_SpecialN,
  itlinkbomb** = new queue #1/#2, 0B .text); quatlib/MSL re-checked
  unchanged; flip queue re-ranked (sobjlib/grgreatbay new S1 leads).
  ⭐ **ROTATION-ENUM SPECIALIST — the allocator law (8 falsified
  models) is CRACKED** (new ROTATION-LAW VERDICT section; idioms
  128-131): L1 choice rule SOLVED (cursor circular-descending,
  dirty-first, else highest-virgin; solver-proven unique fit over 7
  colorings), L2/L3 processing-order regimes (explains idiom 1 AND the
  24-perm cap), L3c anchor-pair law (extends 126), L4 chain-steering
  lever, L5 TU-context probe limit. Synthetic harness + 70-entry
  search_log + solve2.py order-reconstructor banked in
  rotation-enum-w20/. **WIN KEPT in gm_1BFA.c**: load-bearing 13-decl
  reorder + i2-before-i1 → gm_801BFCFC 99.28, linkcheck 67B→**20B** =
  flip rank #2; residual ONE r27↔r28 transposition PROVEN
  model-unreachable (anchor-pair deadlock; 4 untried 1-compile
  inverter ideas queued); 5 siblings 100.00, bytegate confined to fn
  range, DOL EXACT ×2; it_8026EECC re-queued with the idiom-130 lever.
  ⭐ **lbaudio S1 close** (lbaudio_ax.c, ~28 compiles, src diff = 2 fns,
  all other probes reverted byte-clean): **27648 98.76→100.00 MATCH**
  (goto skeleton + both loops in `static inline` scan helper, idioms
  57/55) + **27488 94.68→97.72** (split decl/def + def-order b-first +
  destructive last-loop ptr-walk); idioms 135-138 (lis-fold direction
  gate, destructive ptr-walk coalesce, cross-call address-node law +
  117 negative, 123 size gate); gate 2 UP / 0 DOWN / 0 GONE/NEW (only
  @1731→@1737/@1887→@1893 tail-pool renames, already-broken-pin
  family), DOL EXACT over sibling dirt; 26C04 out-of-band recipe +
  27488/27DF8 rotation items queued. **hsd_40FF recon-completion
  VERIFIED, zero edits**: task narrative was STALE (backout cured
  in-wave w19; flip #4 already committed) — full 0x20bc0 target-byte
  readback, fresh-compile sha identical, linkcheck `[]`, force-active
  audit, touch-forced relink DOL EXACT; **do not re-flip**. **NAMING-15
  IN FLIGHT at close** (symbols.txt mid-gate): 24 pins live-derived +
  value-verified 24/24 (ftcoll 17 confirmed zero-drift + gm_182F −4
  family @245/246/247/249→@241/242/243/245 link-MOOT + gm_1693 Fix-A
  trio @571/@663/@664); plattack unk_804D6480 re-routed to plattack
  SRC owner (flipped byte-frozen TU — .symtab strings change, needs
  flip-gate re-baseline); per-rename DOL gate loop 12/24 PASSED (sha
  EXACT each) at close — commit pends post-round gates (value-verify
  --new, byte-gate, idempotence); durable record naming15/progress.txt.
  **QUEUES**: flip exec front = ftPr_SpecialN + itlinkbomb pool-order
  src fixes; rotation queue in the verdict section (gm_801BFCFC
  inverter, it_8026EECC, quatlib EC4C, D148, 27488/27DF8, 69A84/DAD4/
  gmregclear); S1 leads sobjlib 803A4A68 / grgreatbay; naming after
  n15: quatlib @165↔@166 swap, lbaudio 6-sym bss merge + unit @-map
  recompute (now incl. @1737/@1893); math_data.c:82-83 deletion
  (DOL-neutral, src owner); linkcheck.py .comment-0x08 extension
  STILL unimplemented (w19 carryover). **WAVE BOUNDARY**: dirty =
  gm_1BFA.c + lbaudio_ax.c (wins kept, uncommitted — no grant) +
  symbols.txt (naming-15 mid-round) + stale backlog.json; serialize:
  naming-15 finishes + commits FIRST, then triage.py, then full
  configure + ninja + dtk shasum over the settled tree. **WARNINGS**:
  (1) idiom-number collision AGAIN — rotation and lbaudio streams both
  self-assigned "128-131" (exec stream correctly cited CANDIDATE-A/B/C);
  final numbers assigned here: rotation=128-131, flips=132-134,
  lbaudio=135-138 — agents MUST cite candidates only; (2) build-race
  law HELD: pgrep idle before every gate ×3 streams, and the flip
  executor correctly did NOT re-ninja after post-commit sibling dirt
  appeared; (3) sibling-dirt attribution clean ×3 (DOL byte-exact over
  dirt on every gate); (4) durable records held ×5 (REPORT.md ×2 +
  progress.txt ×3). Idioms 128-138 added; ROTATION-LAW VERDICT section
  opened; flip queue re-ranked ×2 (rotation + sweep merged); FLIP
  VERDICT now ×6.
- **2026-06-07 — Wave 19 (⭐⭐ FLIP #4 hsd_40FF DATA-ONLY + flip-unblock
  sweep ×4 streams: itdrop FLIP-READY, quatlib→25B, MSL/math→18B,
  gm_1BFA→67B, lb_00CE/gm_16A9/ifprize/Shouldered adjudicated) — 1
  commit (ab2a4870d), HEAD ab2a4870d; project 839→**840/1041 linked**
  (code-complete % unchanged 42.62 — the flip is data-only), matched
  72.15→**72.20%**, fuzzy 97.59; 3 NEW exact matches (it_8026F3D4,
  gm_801BFC60, quatlib MatToQuat) + .sdata2 byte-EQUAL ×2 (quatlib,
  MSL/math); ~190 compiles + ~12 full ninjas.** ⭐⭐ **FLIP #4 COMMITTED
  ab2a4870d**: hsd_40FF NonMatching→Matching riding the 11199-line
  brace-init recon package (u8[0x20BC0] 4bpp glyph sheet, idiom-98
  class, ZERO @-ids, symbols/splits md5 unchanged); linkcheck [],
  .data/.symtab/.comment byte-IDENTICAL, DOL sha 08e0bf20 EXACT +
  idempotence ×2. **First gate attempt FAILED twice, both lessons
  banked**: (1) mwld deadstrips compiled zero-importer objects — cure
  `#pragma force_active on` (⭐ idiom 116; linkcheck.py blind to it,
  audit added to the flip recipe); (2) a sibling ninja racing in the
  shared build dir corrupted main.dol (BUILD-RACE LAW → ground rules:
  pgrep idle before every flip gate). ⭐ **itdrop UNBLOCKED →
  FLIP-READY** (queue #1 cleared, ~10 compiles): it_8026F3D4
  95.56→**100.00 MATCH** via `*(volatile f32*)&ext` (⭐ idiom 117 —
  the only breaker of the lfs-pairs-with-lis park; LICM/helper routes
  refuted at 95.5); all gates PASS (6 siblings byte-identical, ninja
  canonical, DOL EXACT ×2, zero @-ids); flip executor re-gates over
  the settled merged tree at the boundary. ⭐ **quatlib → 25B-only**
  (~30 compiles, only winning edits kept): MatToQuat →**100** (idiom
  119 — delete invented `tr`, self-read `s`), .sdata2 byte-EQUAL
  (embedded-assign pool-order idiom 120 — the 99.81 "drop" is a pure
  @165↔@166 name swap, dtk-@-by-appearance trap) + EOF
  `const f32 gap_11 = 0.0F` (idiom 122) closing size + export; sole
  blocker EC4C FPR pair swap = VN-canonical rotation park (idiom 121).
  **lb_00CE PARKED DEFINITIVE + DEMOTED** (src reverted): pool head
  needs a dead early creator — idiom 118 FLIP-grade proof (uncalled
  static emits .text; if(0) dies pre-pooling; called-once inline pools
  at call site; Fix-B refuted ×2); gm_1965/particle 0.0f-pool-heads =
  flip blockers by the same proof; D148 true shape (99.06) → S1.
  ⭐ **MSL/math 7→1 hard issue** (~74 compiles): positioned `const u32`
  defs + `*(u32*)&` reads (idiom 127) rebuilt .sdata2 0x20
  byte-IDENTICAL (pins intact), 15/15 relocs, 5/5 exports, cross-TU
  flip-blocker removed; **binary-proven deviation flagged in-source:
  math_data.c:83 C1 hex 0x3EAA9F44 is a transcription error for
  0x3EAAAA36** (its own comment + the DOL agree); also MWCC 0.0/0.0 =
  0xFFC00000 ≠ target 0x7FFFFFFF — float-typed NaN initializers can't
  reproduce the bytes. Residual 18B = logf frame layout (slot map
  fully known, fM/fm truth inverted vs source; 2 best lines queued).
  ⭐ **gm_1BFA 80B→67B** (~70 compiles): gm_801BFC60 →**100.00 EXACT**
  via s16→int params + s16→u16 fields (idiom 124; 4 call sites prove
  it); residual gm_801BFCFC ~50-row CS permutation park (target reg
  map + LOAD-BEARING 2-stmt r26 init documented). **itdraw reverted
  byte-identical** (~20 probes; one-transposition tie-locked state
  documented); intel: depth-0 auto-inline (idiom 123), inline-param
  ==inline-local (125), leftover-CS web (126). **QUEUES**:
  rotation-enumeration agent now owns gm_801BFCFC + it_8026EECC +
  quatlib EC4C + lb_8000D148 (+ 69A84/DAD4/gmregclear carryovers);
  naming: quatlib @165↔@166 value-verified swap (post-flip,
  score-only); math_data.c lines 82-83 deletion (DOL-neutral, makes
  its .o byte-identical); ZERO naming debt from itdrop/hsd_40FF.
  **WAVE BOUNDARY**: 5 src files dirty (math.c, gm_1BFA.c/.h,
  itdrop.c, quatlib.c) + stale backlog.json (triage.py — itdrop
  95.56→100); full configure + ninja + dtk shasum over the merged
  settled tree, THEN itdrop flip #5 (executor monitors armed).
  **WARNINGS**: (1) mutex pressure persists — 4 unblocker streams +
  flip executor live concurrently; mid-wave the hsd_40FF pre-flip
  transiently broke the tree-wide DOL for the gm_1BFA stream (later
  landed clean) and configure.py/itdraw/itdrop/math.c sibling dirt
  was observed by every stream — DOL stayed byte-exact over it on 4
  independent gates, but flips mid-wave on shared trees are now
  BANNED (build-race law); (2) durable records held (progress.txt ×4
  + flip-unblock-w19/REPORT.md); (3) idiom-number collision in agent
  reports (two streams both claimed "116") — playbook editor assigns
  final numbers; agents must cite candidates as CANDIDATE-N. Idioms
  116-127 added; flip queue re-ranked (lb_00CE demoted, gm_16A9
  refuted-easy, hsd_40FF retired); FLIP VERDICT section now ×4.
  ftcoll + gmregclear family + naming round 14 + S1 rotation ×2 wins)
  — 3 commits (633eda23d naming-14 → 51951826b flips 1+2 →
  4715a41d8 flip 3), HEAD 4715a41d8; project 836→**839/1041 linked
  (42.35→42.62%)**, matched 72.13→**72.15%**, fuzzy 97.58→97.59;
  3 NEW exact matches (ftColl_8007861C, fn_801935B8, fn_801695BC) +
  2 section rows →100 byte-IDENTICAL; ~225 compiles + ~30 ninja
  gates.** ⭐⭐ **FIRST FLIPS — THE ENDGAME METRIC IS LIVE** (verdict
  section rewritten at top): gm_182F (recon-supplied 1-line .sbss
  def) + plattack (ZERO work) + ftchangeparam (ZERO work) flipped
  `Object(Matching)`; DOL sha1 08e0bf20… IDENTICAL + dtk OK through
  6 full ninja passes incl. a mid-gate naming-14 symbols.txt
  re-split; PASS ×3 + idempotence ×3; main.elf link-edge verified
  consuming our objects ×3. **Settles S5 / pre-session-2 experiment
  #2**: naming-only fns ARE byte-correct in a linked Matching unit
  (ftchangeparam carried 219 LIT2EXT + 6 SHADOW rows); dtk gap-tail
  parks SURVIVE the link (ALIGNFIX — gm_190A 5B/4B parks are NOT
  blockers); weak _half/_three stripped+compacted; export-neutrality
  iff zero importers (idiom 110). New instrument
  `file-flip-w18/linkcheck.py` — flips gate on it, not fuzzy.
  Next-10 flip queue ranked (itdrop front — its F3D4 2-row park now
  GATES a flip; hsd_40FF = data-only +1 file). ⭐ **DATA-RECON
  ftcoll** (UNCOMMITTED; ~22 compiles): [.sdata2-0] 81.25→**100.00
  byte-IDENTICAL** — w14 permutation park CRACKED via the
  deadstripped-inline pool-orphan helper (idiom 111); bonus
  **ftColl_8007861C 79.44→100.00 MATCH** (entry-read source_ply
  semantic bug + ternary x18C0 + arg6 int→u16 ftcoll.h + diamond-
  init webs + PAD_STACK(8)); getEnvDmg park DISSOLVED (missing
  `static inline`, idiom 112); unreachable-bytes ledger CLOSED;
  gate 2 UP / 0 DOWN over 91 rows; 17 value-verified renames →
  naming-15 (serialize vs ftcoll edits); .text leftovers → S1.
  ⭐ **DATA-RECON gmregclear family** (~18 compiles; gm_182F rode the
  flip commit, rest UNCOMMITTED): gm_182F whole-byte-exact
  (math_ppc.h ballast kill + u8 lbl_804D65E8 def) → FLIP-ENABLED;
  gm_181A [.data-0] →**99.81** (u8[0x480] table + 7 proven-dead
  strings; .text byte-IDENTICAL); gm_180A [.sdata2-0] →**100** +
  .sbss ×4→100; head [.data-0] 6.28→**96.15** + .rodata 93.33 +
  **.bss layout 10/10 target-exact** (2 binary-proven size fixes:
  gmClassic_80472AF8→StartMeleeData 0x138, lbl_80472CB0→0x78);
  head gate 171 SAME / 10 UP / 1 DOWN (root-caused C-unreachable,
  idiom 63); idiom 114. **NAMING-14 COMMITTED 633eda23d**
  (symbols.txt only, +22/−22): gm_1965 extras CONFIRMED
  @3554→@697 / @3555→@698 (stale in-flight @701/@703 AND the n13-era
  @696 guess both REFUTED live); hsd_39D1 hygiene 20 incl. UNBLOCKED
  **@2723→@678 (f64 2.0)** + **lbl_804DE9E8→@681 (π/2)**, 2 live
  stale-id corrections (@849/@850→@850/@851); gm_190A re-check:
  ZERO renames needed (61/61 pins re-verified); gm_1965 6 UP,
  hsd_39D1 1 UP; ALL gates PASS (per-rename DOL ×22 + value-verify
  22/22 ×2 + byte-gate + idempotence 22/22); idiom 115. **S1
  ROTATION** (gm_190A.c + gm_1693.c UNCOMMITTED, ~42 compiles):
  **fn_801935B8 →100.00 MATCH** (1 probe — idiom-98 entry capture
  as S1 lever) + **fn_801695BC 70.22→100.00 MATCH** (16-compile
  rewrite: rolled fill for + inline-identity-helper + no-returns +
  call-result redefine + indexed shuffle) + fn_80169A84
  74.20→**94.26 parked-at-best**; idiom 113; gates 0 sibling drift
  both units, gm_190A symtab byte-identical; gm_190A's "only gap to
  100" CLOSED — 95AF0/90ABC/91240 were outside the grant.
  **NAMING-15 QUEUE** (stale-id policy: re-derive ALL ids live at
  land): ftcoll 17 value-verified renames (@338→@336 family +
  @1520→@773 magic — SERIALIZE vs any ftcoll edit); gm_1693 Fix-A
  trio RE-DERIVED **@571/@663/@664** (pool drift −17 from the S1
  rewrite; supersedes the naming-12 @588/@680/@681 values); gm_182F
  AT_RENUM 12 (@245→@241 family — link-MOOT post-flip, renumber at
  leisure, must keep the flipped TU byte-exact); plattack
  unk_804D6480→lbl_804D6480 optional display fix; carryovers
  unworked (hsd_3983 map, hsd_804D0908 subdivision, lbaudio B24
  6-sym merge + 28690 @722→@721, sislib C564/@69↔@70/@264
  paired-edits, particle lbl_8040A93C, naming-12 remainder minus
  the superseded gm_1693 trio, toy/tydisplay/gm_1601 maps, camera
  NAMEPAIR). **SRC/RETRY QUEUE re-rank (flip-aligned)**: itdrop
  F3D4 park retry GATES flip #1 (S1 front); gm_180A fn_80180C60
  98.17 = last blocker of the family's next flip; gm_181A 3 S1 fns;
  hsd_39D1 DAD4/9F05C rotation parks; gm_190A remainder
  (95AF0 re-apply, 90ABC, 91240); ftcoll .text leftovers (77C60
  frame+8 early-ply-load family, 7BE3C missing u32 conversion,
  decay FPR rotations ×4 + 8 more); rotation-enumeration agent now
  owns 69A84 + DAD4 + gmregclear-family parks; data-recon front =
  hsd_40FF data-only flip package, then head NAMING+SRC paired
  package. **WAVE BOUNDARY**: 7 files dirty (ftcoll.c/.h, gm_1693.c,
  gm_180A.c, gm_181A.c, gm_190A.c, gmregclear.c + stale
  backlog.json — triage.py before wave 19); full configure + ninja +
  dtk shasum over the dirty trees before ANY commit. **WARNINGS**:
  (1) **mutex violated #8** — 5 agents live on shared trees again;
  flip gates survived only via per-gate file re-hashing +
  hash-stable sibling verification; S1's end-of-session full ninja
  rebuilt the shared build dir under siblings (DOL still exact) —
  wave-boundary serialization STILL unenforced, and post-flip the
  stakes are higher (ours objects are load-bearing); (2) REPORT.md
  harness-block ×4 (gmregclear-w18 — progress.txt is the durable
  record); (3) name-keyed snapshot accounting lies on rename chains
  (idiom 115c — one sibling mid-flight edit nearly mis-attributed as
  a naming UP, correctly excluded); (4) commit hygiene held:
  explicit pathspec + staging-verified-empty (idiom 97e applied).
  Idioms 110-115 added; FIRST FLIP VERDICT section opened (replaces
  flip-candidates); endgame-metric banner added at top.
- **2026-06-07 — Wave 17 (naming round 13 + data-recon SCALE ×2 TUs +
  types.h tails adjudicated + gm_18A5 S1 + hsd_39D1 S1) — 1 commit
  (naming-13 d17be7f73), HEAD d17be7f73; 2 NEW exact matches + 26
  naming recoveries + 1 data sym→100; ~230 compiles + 2 full ninjas;
  flip-candidates section opened.** ⭐ **NAMING-13 COMMITTED
  d17be7f73** (symbols.txt only, +61/−61): TU-B gm_190A .sdata2
  43-pin renumber + .sdata @354/@355/@3025/@3027→@742/@743/@1679/
  @1681 + .data @1582→@230 + 7 Fix-A strings (lbl_803DA040 family →
  @1680..@1687, all grep-clean); TU-C gm_1965 6 pins with
  binary-proven VALUE CORRECTION — the queued magic pair was
  VALUE-CROSSED, applied @357→@416 / @1620→@320 (inverse of queue);
  **26 fns→100** (16 TU-B incl. fn_80193308/gm_80190EA4 + 10 TU-C
  incl. fn_80196DBC/973F8) + 19 partial UPs; ALL gates PASS
  (per-rename DOL ×61 sha 08e0bf20 EXACT, value-verify 61/61 ×2 vs
  sibling-isolated HEAD-worktree objects, controlled 471 SAME / 90 UP
  / 0 DOWN with GONE=NEW=61 exactly 1:1, byte-gate identical,
  idempotence 61/61, idiom-79 clean); SKIPPED: TU-C extras
  @3554/@3555 (sibling recon mid-flight → naming-14 re-derive),
  sislib @264 (paired src deletion still live, sislib.c:2362);
  DO-NOT-RENAME @768/@773 respected; lbl_804DA808→@697 candidate
  REJECTED (src-referenced, idiom 75); idiom 103 (sibling-churn
  canary + sibling-isolated gate + value-keyed dup-pin maps).
  ⭐ **DATA-RECON SCALED ×2** (gm_190A.c + gm_1965.c, both
  UNCOMMITTED, src-only, ~28 compiles; verdict section updated):
  **gm_190A** — [.data-0] 54.24→**94.84** + [.sdata2-0] →86.18 (both
  content == target, 5B/4B dtk gap-tail parks), lbl_803D9F80 +
  14 positioned const defs →100, **gm_801963B4_OnEnter →100.00 NEW
  MATCH**, fn_801935B8 →98.59 (residual = r28-r30 decl rotation →
  S1); gate 135 SAME / 19 UP / 1 DOWN (E6C −0.02 lucky-pairing loss,
  accepted park); binary-proven values flagged in-source
  (738=12.800008f, 73C=2.6200008f, 754=7.1899995f, 78C=0x46DC46FF
  s32); mid-session naming-13 rebase handled clean (103/140 at 100
  vs fresh target). **gm_1965** — [.data-0] 3.72→**99.91
  byte-IDENTICAL** + [.sdata2-0] 26.14→85.33, 39 syms→100
  (lbl_803DA0D0 0x184 + 6 dead strings + 32 const f32 EOF defs),
  @697 10000.0f →100 — **@3554 UNBLOCKED**; fn_8019B458 →91.43
  (pragma dont_inline, idiom-73 bonus); gate 33 SAME / 59 UP /
  0 DOWN / 1 GONE-by-design; analyzer OK 343→380, AT_RENUM 37→1,
  every residual enumerated. Idioms 104-106 (Fix-B placement
  window, C-unreachable stream heads, offset-0 auto-inline flip).
  ⭐ **HEADER types.h TAILS ADJUDICATED** (types.h +16 lines
  UNCOMMITTED): **TmData 0x56C→0x574 APPLIED** (idiom-102 recipe
  passed; **gm_804771C4 50→100**, fn_8018FA24 →98.99, [.bss]→81.84;
  gate 234 SAME / 6 UP / 0 DOWN over 28 units; full ninja DOL OK);
  **MatchEnd +4 REFUTED + RETIRED** (3 embedding caps + live
  grow-experiment broke 5 matched fns — dtk gap-inclusive pad,
  idiom 101; guard comment + 0x227C assert left in types.h);
  TrainingModeState +0x114 REAL but ownership-BLOCKED → reassigned
  (see header queue); TmUnkMenuData confirmed landed. **gm_18A5 S1**
  (TU-A head, ~60 compiles, UNCOMMITTED +47/−35): **fn_8018ECA8
  →100.00 MATCH** (invented `is_us` staging deleted →
  bool-of-call-in-index form, idiom 107; exact −11 id repayment);
  fn_8018FBE0 →93.64 (+12.6, u32 IV zero-trip fold); 2 binary-proven
  stale "+0xC" fixes (FA24 tmdata anchor base+0xC→base; F888
  .x37->x8→.x33); gate 113 SAME / 5 UP / 0 DOWN, 32/32 @-pins
  byte-identical, DOL sha EXACT; idiom 108 (CS-choice law); QUEUE
  CONFLICT corrected: 90ABC + 91240 live in gm_190A.c → re-routed.
  **hsd_39D1 S1** (generator.c TU, ~116 compiles, UNCOMMITTED):
  hsd_8039DAD4 86.29→**99.34**, hsd_8039F05C →**99.54**,
  **[.sdata2-0]→100.00**, [.data-0]→97.91, extab→100; **30
  binary-proven deviations fixed** (headliners: entry guard
  `count < 1.0F`, default case calls GLOBAL hsd_804D78E8 not
  gen->callback, case-8 sphere branch SWAP, M_TAU→`2.0*(M_PI*rnd)`
  + cone math on M_PI_2 — **naming-10 blocked pins @2723 f64 2.0 +
  π/2 BOTH UNBLOCKED**, open-coded Newton sqrt ×8, math_ppc.h
  removal = the offset-pairing win, idiom 109f); gate 10 UP /
  0 DOWN, DOL sha unchanged; parks: DAD4 FPR 7-web rotation
  (gmregclear family), 9F05C frame +8 + fmr copy-survival singleton
  (@859 caps 94.44).
  **NAMING-14 QUEUE** (STALE-ID POLICY: re-derive ALL ids
  value+address-keyed at land time — the gm_1965 recon land WILL
  renumber TU-C ids, in-flight ours-pool already moved +5):
  gm_1965 extras **0x804DA828 (@3554)→@697 + 0x803DA254
  (@3555)→@698** (pairs 12 rows; in-flight ours-ids @701/@703 —
  re-derive, do NOT pre-apply); re-check TU-B ids against the
  land-time map; hsd_39D1 hygiene 20 renames (address-keyed, names
  collide pairwise — apply by address): .sdata2 0x804DE9A8→@250,
  9AC→@671, 9B0→@672, 9B8→@673, 9C0→@674, 9C4→@675, 9C8→@676,
  9CC→@677, **9D0 @2723→@678 (2.0, UNBLOCKED)**, 9D8→@679,
  9E0→@680, **9E8 lbl_804DE9E8→@681 (π/2, UNBLOCKED)**, 9F0→@682,
  9F8→@683, A00→@686, A08→@849, A0C→@850; .data 0x8040C260→@688,
  C280→@687, C2D8→@852; carryovers unworked from the wave-16 queue:
  hsd_3983 map (lbl_804DE98C→@1496 family + 0x8040C044) +
  hsd_804D0908 0x248→0x40/0x104/0x104 subdivision (paired-edit),
  lbaudio unit-map recompute + B24 bss 6-sym merge + 28690
  @722→@721, sislib C564 retire + @69↔@70 decision + @264
  paired-edit, particle lbl_8040A93C subdivision, naming-12
  remainder (gm_1693 Fix-A trio, gmresult lbl_804DA560→@697,
  lb_00F9 lbl_803BA1C0→@1084 + @176→@328), toy/tydisplay/gm_1601
  maps, camera NAMEPAIR cm_803B73B8→cm_WorldForward (pends
  idiom-91 pairing emulation), optional jtbl→@ probe.
  **HEADER QUEUE**: ✅ TmData 0x574 LANDED (uncommitted types.h);
  MatchEnd +4 RETIRED (refuted, guard in place); TrainingModeState
  +0x114 → REASSIGN to a gm_1884.c owner with NEW constraint: the
  split css part must fit ≤0x204 span (0x80473814+0x20C would
  overlap gm_80473A18, a real gmallstar-shared object —
  CssSubStruct pad200 must shrink) + needs gm_1884.c variable split
  + `.css` rewrite (types.h-only breaks compile); OPTIONAL
  gm_18A5.static.h trailing 4B pad object after gm_80477738
  (cosmetic [.bss]→100); carryovers: sislib.h 84BC `int pass`→u32,
  gm_1601.h:53/:157-159/:168, grvenom 80204F20 s32-field verify.
  **SRC/RETRY QUEUE**: **gm_190A.c owner package** (one owner; tree
  already dirty with the recon work): 95AF0 re-apply FRONT (blocker
  CLEARED; text in gm18a5_w11/my_gm_18A5.c, re-fit to TU-B), 935B8
  r28-r30 rotation (only gap to 100), 90ABC reg-web + pool-order,
  91240 jobj2-copy, E6C park stands (byte-load-bearing); gm_1965 S1
  queue: 967E0/96FFC/9A86C/9A158/9AF50 (+ lbl_80473AB8 consumer
  materializations) + structural tails 985D4/98824/98EBC/99AF0/
  9B458; hsd_3983.c owner: try the same math_ppc.h removal
  ([.sdata2-0] 92.31, identical weak ballast); gm_18A5.c FA24/FBE0
  residuals (FBE0 4-cycle rotation s32-args theory — re-probe ONLY
  with header ownership + caller byte-gate; FA24 [bge .+8; b]
  fold-back family); dedicated rotation-family session (hsd DAD4
  7-web + gmregclear family parks); data-recon scale queue
  (particle E80 string package front); **NEW: gm_182F flip probe**
  (flip-candidates section — analyze.py + Matching-flip experiment,
  near-free information). **SPLIT QUEUE**: unchanged (lb_00F9
  low-prio only). **WAVE BOUNDARY**: full configure + ninja + dtk
  shasum over ALL dirty trees before ANY commit (5 files dirty:
  gm_18A5.c, gm_190A.c, gm_1965.c, types.h, hsd_39D1.c + stale
  backlog.json — triage.py before wave 18).
  **WARNINGS**: (1) **mutex violated #6 AND #7** — 5 agents live on
  shared trees again: types-tails final snapshots contaminated by
  S1+naming siblings (disentangled via mtimes + bytegates);
  naming-13 took 1 spurious pin DOL FAIL from a sibling's live
  MatchEnd size probe (attributed + re-gated — a DOL FAIL isn't
  yours until proven on a settled tree, idiom 103); a sibling
  full-ninja OVERWROTE gm_1965's canonical .o mid-session (gate
  ONLY via -1/-2 scratch objects); hsd_39D1's unit gate self-caught
  a sibling TU-wide `sed PAD_STACK` clobber — **law: never sed
  shared tokens TU-wide**; (2) stale-id hazard ESCALATED to live
  collisions: queued TU-C ids were value-crossed AND the w16 map
  "@357→@320" was value-wrong — value+address keying at land time
  is the only safe protocol; (3) REPORT.md harness-block ×3 more
  (gm18a5-s1 3rd occurrence — progress.txt is the durable record:
  naming13-gm18a5/REPORT.md, datarecon-gm190A/, recon-gm1965/,
  header-gmtypes-w17/, gm18a5-s1-w17/, hsd39d1-s1/); (4) failed
  compile leaves a stale scratch .o — require the fuzzy line before
  reading any diff; (5) em-dash sjiswrap gotcha re-hit + scrubbed
  (ASCII-only comments, again); (6) recon DOL gates deferred BY
  DESIGN (NonMatching units are DOL-neutral; full gate at the wave
  boundary). Idioms 101-109 added; flip-candidates section opened
  (gm_182F → hsd_39D1 → gm_190A → gm_1965).
- **2026-06-07 — Wave 16 (gm_18A5 TU-split EXECUTED + camera
  PAIRED-EDIT Fix-A + data-recon PILOT + lbaudio w12-package land +
  hsd_3983 S1) — 3 commits landed, HEAD daedf29c7; data-recon
  verdict PROVEN (new section above); ~160 compiles + 5 full
  ninjas.** ⭐ **TU-SPLIT gm_18A5 LANDED + COMMITTED** (2 splits / 3
  TUs reverse-address: 486077b8f gm_1965.c B|C → 437d3432d gm_190A.c
  A|B → daedf29c7 ASCII fixup; verdict section updated with the
  3 binary-proven cut corrections — notably .bss 0x804799B8 |
  0x804799D8, BOTH w12 candidates wrong, and .sdata 0x804D4170 |
  0x804D4190): all gates PASS ×5 checkpoints, 129 fns preserved
  (49+41+39), symbols.txt slice byte-identical (idiom-79 clean),
  final gate 276 SAME / 29 UP / 39 DOWN / 0 GONE/NEW — every DOWN
  row-verified renumber drift; 13 byte diffs ALL score-positive
  heals (FA24 +4.8, 953C8 +4.6, 91FD4 +4.0, 8F888 +1.8 "+0x20
  artifact" park HEALED, 9A158 +3.0, F30→99.98, [.sdata-0] 64→96,
  6 dup-pins →100); project 72.103% matched; idiom 97; TmData/
  MatchEnd type-size deviations → header queue. ⭐ **camera
  PAIRED-EDIT Fix-A ×3 COMPLETE** (tree UNCOMMITTED: camera.c +
  camera.static.h + symbols.txt lines 29184-29186, exactly 6-line
  diff): **5 fns→100** (29124/928C/2DFE4/2E158/2E234) + 6 more UP,
  end-to-end 11 UP / 0 DOWN, canary A4AC untripped ×3; ⚠
  BINARY-PROVEN DEVIATION fixed+flagged — the naming-12 queue ids
  were STALE +2 (idiom 99); landed value-verified
  cm_804D7E18(1.0)→@274 / cm_804D7E20(magic)→@276 /
  cm_804D7E28(0.65f)→@299 + live-ref :3982 → `= 1.0;`; [.sdata2-0]
  80.98→78.93 accepted (EOF-def deletion display artifact, idiom
  99); WARNING list respected (928C/958C/29AAC creation literals +
  BAA8 ballast untouched, 188 SAME); NAMEPAIR cm_803B73B8→
  cm_WorldForward still pends idiom-91 pairing-emulation; report
  paired-camera-fixA-w16/REPORT.md. ⭐ **DATA-RECON PILOT PROVEN**
  (sislib DONE + particle head-tables LANDED, both UNCOMMITTED;
  dedicated verdict section above): sislib [.data-0] 6.43→**98.47**
  (.data BYTE-IDENTICAL) + 4 syms→100 + NAMEPAIR_LC/MIXED cleared +
  binary-proven CB00 u8-kerning-table type fix; particle [.data-0]
  33.31→**78.75**; 0 @-drift both units (idiom 98 — numeric
  brace-init = 0 ids); DOL gate DEFERRED to wave boundary; scale
  queue in the verdict section. **lbaudio_ax S1 + package** (tree
  UNCOMMITTED): ✅ w12 .data package LANDED WHOLE (header-queue item
  + idiom-72 analyze.py trigger now satisfied; [.data-0]→96.79, 10
  UP / 0 DOWN); 233EC UN-PARKED 95.00→**98.53** (idioms 93-96, net
  ZERO @-drift, exact −2 shim repayment); B24→98.90 src CEILING;
  28690 park confirmed; final gate 158 SAME / 10 UP / 0 DOWN, DOL
  OK. **hsd_3983 S1** (tree UNCOMMITTED): 98A08 62.63→**97.52**,
  Locate 75.74→**91.11**, 9930C→94.21 frame re-EXACT; 3
  binary-proven fixes — phase-2 mask 0xF1FFFFFF, case-0 num=0,
  **`mag > 1e-10F` = the un-located 1e-10 guard FOUND**
  (lbl_804DE98C; naming-10 blocked pin UNBLOCKED); gate 84 SAME / 6
  UP / 3 DOWN all healing via the queued hsd_804D0908 subdivision;
  @-pins held under a deliberate +1 pool insertion (idiom-91
  offset pairing); idiom 100; DOL PASS.
  **NAMING-13 QUEUE** (STALE-ID POLICY per idiom 99: re-derive ALL
  ids value+address-keyed from fresh objects at land time):
  gm_18A5 split renumber maps (tu-split-gm18a5/progress.txt) —
  TU-B .sdata2 41 renames (@1578→@226 … @2183→@837 incl. dup-pins
  @357→@232, @1620→@270, @785→@310, @991→@833), .sdata @354→@742 /
  @355→@743 / @3025→@1679 / @3027→@1681, .data @1582→@230 + 7
  Fix-A string renames (lbl_803DA040→@1680 family); TU-C .sdata
  @354→@413 / @355→@414 / @3556→@699 / @3652→@795 + .sdata2
  @357→@320 / @1620→@416 candidates; TU-A none; DO-NOT-RENAME
  (idiom 75): lbl_804DA738 (our @768), lbl_804DA75C (our @773) —
  E6C park stands. hsd_3983 (address-keyed): lbl_804DE98C→@1496
  (UNBLOCKED) + 0x804DE990 @1496→@1497 + 0x804DE998 @1498→@1499 +
  0x804DE9A0 @1500→@1501 + 0x8040C044 @1499→@1500; PAIRED-EDIT:
  subdivide .bss hsd_804D0908 0x248 → 0x40/0x104/0x104 (heals all
  3 gate DOWNs, A08→~99.5; lbl_8040BFB0 precedent). lbaudio
  unit-map recompute NOW UNBLOCKED (w12 map @1921/3/5/2→str_*_ssm;
  B24 bss 6-sym merge lbl_80433A64/B44 + lbl_804D3870/74 + gap_07;
  28690 @722→@721). sislib C564 row (retire as padding) + @69↔tgt
  @70 dup-pin decision; particle lbl_8040A93C 0x94→0x80+@832
  subdivision. Carryovers: ✅ camera PAIRED-EDIT DONE (above);
  naming-12 unapplied remainder (gm_1693 Fix-A trio, gmresult
  lbl_804DA560→@697, lb_00F9 lbl_803BA1C0→@1084 + @176→@328);
  naming-10 blocked pair remainder (@2723 f64 2.0 / π÷2 — pend
  hsd_39D1 S1); toy renumber map (VALUE-KEYED), tydisplay @1021
  re-pin + un_80319EF0 map, gm_1601 gm_80166378 dup-pins, optional
  jtbl→@ probe.
  **HEADER QUEUE**: NEW gm/types.h TmData +8 / MatchEnd +4 tail
  fields (binary-proven sizes 0x574/0x2280 — no pads remain after);
  NEW sislib.h 84BC `int pass`→u32 signature verify; ✅ lbaudio
  full_tree_with_pkg.patch LANDED (item retired); carryovers
  unchanged: TrainingModeState +0x114, gm_1601.h:53/:157-159/:168,
  grvenom 80204F20 s32-field verify.
  **SRC/RETRY QUEUE**: gm_18A5 95AF0 package post-naming-13 (now
  eligible); gm_18A5 post-split S1 on clean TUs (90ABC reg-web +
  pool-order, 91240 residual); B24 5-row web re-score AFTER the bss
  merge; objdiff data-scoring investigation (byte-identical
  offsets_arr/str_last_ssm display artifacts); data-recon SCALE
  QUEUE (verdict section — particle E80 string package front);
  hsd_39D1 S1 (DAD4 f64 literals + 9F05C); carryovers unchanged
  (ftcoll 7861C probe, camera 29CF8 f26 web, gm_1601 post-split S1,
  gmresult/grbigblue parks).
  **SPLIT QUEUE**: gm_18A5 DONE — queue EMPTY except low-prio
  lb_00F9 (fn_80013614); next split pends new SECFOLD evidence.
  **WARNINGS**: (1) **mutex violated a FIFTH time** — 5 agents live
  mid-wave on shared trees (all range-disjoint, verified) AND one
  staged-file sweep incident: a sibling's STAGED lbaudio files were
  swept into the first split commit despite explicit pathspecs —
  amended out via `git reset HEAD~1 -- <paths>` (idiom 97e; verify
  staging before EVERY commit; wave-boundary serialization still
  unenforced); (2) STALE-ID hazard is systemic (idiom 99) — @-pin
  canaries do NOT catch early-id drift; re-derive every queued id
  at land time; (3) REPORT.md harness-block persists ×2 (datarecon
  + hsd_3983 — progress.txt is the durable record); (4) mwcc_sjis
  ASCII-only re-hit (em-dash → commit daedf29c7); (5) datarecon DOL
  gate DEFERRED — run full ninja + dtk shasum over ALL dirty trees
  at the wave boundary before any commit; (6) display-DOWN classes
  reconfirmed: [.sdata2-0] section-size/alignment artifacts (camera
  −2.05, sislib 8134 −0.49) + S1-rotation row scoring on
  byte-strictly-better code — byte-gate before judging. Idioms
  93–100 added. Uncommitted src: camera.c/.static.h (+ 3 paired
  symbols.txt renames), lbaudio_ax.c/.static.h, hsd_3983.c,
  sislib.c/.h, particle.c (+ stale backlog.json — triage.py before
  wave 17).
- **2026-06-07 — Wave 15 (gm_1601 TU-split EXECUTED + inversion-tail
  ×4 units + naming round 11) — 3 commits landed, HEAD 2226073a1;
  idiom-72 sweep worklist heads ALL DONE; 13 fns→100 (1 split + 9
  inversion + 3 naming).** ⭐ **TU-SPLIT gm_1601 LANDED + COMMITTED**
  (2 splits / 3 TUs reverse-address: 3504f3639 gm_16A9.c B|C +
  2226073a1 gm_1693.c A|B; verdict section updated with cuts/gates):
  all gates PASS ×5 checkpoints, 226 fns preserved (184+35+7), zero
  @-pin drift, symbols.txt canary-identical; **gm_8016A164 →100
  EXACT**, [.sdata2-0]→100 (TU-B pool byte-EXACT), A22C→76.8 /
  A4C8→89.5 / AC44→66.8; unit matched 63.369→63.659; PREDICTION
  CORRECTION: 95BC/9A84 were pure S1 all along (no anchor rows);
  idiom 89 (shared-static.h struct visibility, moved noinline
  wrapper chains, gap→pad cut, sibling-flux gating). ⭐ **Inversion
  tail EXECUTED (one owner per unit, src-only, all bytegated, trees
  UNCOMMITTED)**: **sislib** — LIT2EXT 11→1 (residue BY DESIGN), OK
  148→158, **3 fns→100** (803A62A0/6754/7548), 5798→99.54; all 4 LC
  rows cleared; .sdata HSD_SisLib_804D6390 char[8] Fix-B
  target-EXACT; dup-640 donor-keeper split — @264 RECLAIMED its own
  number, @-pins ZERO net drift (4 pool deaths repaid by 3
  goto-shims + 1 organic CSE tick); bytegate 34/34 byte-identical;
  ⭐ idiom 90 discovered (label ids tick at FUNCTION ENTRY).
  **lb_00F9** — LIT2EXT 8→3, OK 377→382, **3 fns→100**
  (13C18/13D68/14014 via reversed chain-assign); 14638/149E0 rows
  paired; 14770 REVERTED → donor-keeper park (extern-rank flip);
  121FC pair queued naming-12; extern refs consumed 0 @ids ×5
  (idiom 72 re-confirmed); 45-sym @-canary zero drift; unit merges
  ≥4 TUs (fn_80013614 SECFOLD = low-prio split evidence).
  **gmresult** — LIT2EXT 5→1 (forced donor-keeper, idiom-90
  bracket); binary-proven: target lbl_804D3FB0 = assert string "0"
  (.sdata, 0x2), ours had invented `static s8 = 0x30` — fixed via
  static.h `char[] = "0"` + 4 sites through __assert/HSD_ASSERTMSG;
  fn_80176A6C→99.85, OnEnter→99.25, [lbl_804D3FB0]→100; bytegate +
  @-canary PASS; **gm_1832 = correct NULL result** (3 rows = 0x4330
  magic, already objdiff-paired ⇒ ⭐ idiom 91; its load-bearing
  @-pins never touched). **grbigblue** — LIT2EXT 20→13 (all
  park-enumerated) + ~17 LC rows paired, OK 432→439, **3 fns→100**
  (801E6904/801EAB50/801EF7D8) + 9 UP incl. binary-proven 801EC58C
  `max_y = 0.0F`→grBb_804DB310 (-FLT_MAX sentinel); 71/71 @-syms
  zero drift; Fix-A unavailable (sqrtf weak-sym +0x10 shift); idiom
  92 cost taxonomy. **Naming-11 COMMITTED 10cffa882** (symbols.txt
  only, +14/−14): ftcoll queue ALL 14 applied scope:local
  (pin-verified pass1+pass2, 0 fails; cross-TU grep clean); gate
  PASS at baseline/post-apply/no-op idempotence; per-unit snapshot
  180 SAME / 42 UP / 0 DOWN / 0 GONE/NEW; **3 fns→100**
  (ftColl_80078A2C/7ABD0/7BC90 — pool-permutation trio recovered by
  name+addend pairing WITHOUT the 7861C crack) + 79AB0/79C70/79EA8/
  7A06C up; all 12 w14 drops + @340 + @655 recovered; DEVIATION:
  [.sdata-0] stayed 40.0 (dtk tail pads + 12B permutation — parked,
  see wave-15 parks); camera Fix-A 3 SKIPPED — live src coordination
  missing (EOF defs camera.c:4956-4958 + decls camera.static.h:
  102-104 + live ref :3982) → PAIRED-EDIT queue; particle: nothing
  unapplied (naming-10 complete; 3 blocked pins re-verified still
  source-blocked → S1, sources untouched since b701858cc).
  **NAMING-12 QUEUE**: gm_1693 Fix-A (address-keyed, value-verified,
  cross-TU grep-clean; re-verify ids at land): lbl_804DA288→@588
  (1.0f), lbl_804DA28C→@680 (0.0f), lbl_804DA290→@681 (−1.0f), all
  scope:local; gmresult lbl_804DA560 (symbols.txt:31102) → @697
  scope:local (idiom-54 dup-pin, 0x4330000080000000 ×3 value-
  verified, grep-clean, idiom-80 byte-safe class; hygiene-grade —
  clears 3 analyzer rows + 7 leftovers in 84138×1/84AB8×3/86634×3;
  bonus verify lbl_804DA568 10.0f/+0x30 vs unpinned @793); lb_00F9
  lbl_803BA1C0 (.data:0x803BA1C0) → @1084 (name-on-pool-string,
  idiom 84/54; pairs lb_800121FC ×2) + @176 (.sdata2:0x804D7BA4) →
  @328 dup-pin carryover (pairs 4 AT_RENUM + lb_8000FD48 ×2).
  **PAIRED-EDIT items (symbols.txt + src must land as ONE
  coordinated change each)**: camera cm_804D7E20→@274 (13 rows) /
  cm_804D7E18→@272 (2 rows + camera.c:3982 `rand_dir =` ref
  conversion) / cm_804D7E28→@297 (2 rows) — each = rename + EOF-def
  delete + header-decl delete (✅ **DONE wave 16** — queue ids were
  STALE +2, idiom 99; landed value-verified as E18→@274 / E20→@276 /
  E28→@299); + NAMEPAIR cm_803B73B8→
  cm_WorldForward (pends pairing-emulation check, cf. idiom 91);
  sislib OPTIONAL 1-row: HSD_SisLib_804DEAB4 (.sdata2:0x804DEAB4) →
  @264 dup-pin + 1-line src deletion of the then-orphaned AB4 EOF
  def (pairs 611C's last row →100; else residue stands by design).
  **DO-NOT-RENAME (idiom 75, src-referenced/load-bearing)**:
  lb_804D7C50/lb_804D7C70; grBb_804DB2F0/2F4/30C/310 (~30 OK rows).
  Unworked naming-11 carryovers: toy renumber map (VALUE-KEYED
  re-verify at land), tydisplay @1021 re-pin + un_80319EF0 map,
  naming-10 blocked trio (pends particle S1), lbaudio unit-map
  recompute, gm_1601 gm_80166378 idiom-54 dup-literal pins (now
  separable post-split), optional jtbl→@ probe.
  **SRC/RETRY QUEUE**: gm_1601 post-split S1 (clean TU context):
  fn_801695BC + fn_80169A84 rotation class, gm_8016A22C 76.8 /
  fn_8016A4C8 89.5 partials, gm_8016AC44 66.8 structural (TU-C);
  .data-reconstruction agent now owns FOUR packages — sislib
  8040CB00/8040CD40 (.sbss placeholders vs target initialized
  0x240+0x3240 .data tables; fixes 84BC/8134 NAMEPAIR_LC), gmresult
  idiom-69 (ours 0x198 vs tgt 0x580; lbl_803D6488/6558/6858
  undefined our-side; anchor+0x410 displacement rows across 8
  size-mismatched fns; + positioned .sdata pass — ours lacks
  "P"/"%"/lbl_804D3FA8, lbl_804D3F8C wrongly .bss), particle head
  strings + lbaudio package (carryovers); lb_00F9 low-prio TU-split
  (fn_80013614 + dead @715 dup-string + lbl_803BA248 table at
  +0xF8); carryovers unchanged: ftcoll 7861C pool-permutation probe,
  camera 29CF8 f26 web, grvenom 80204F20 s32-field header verify,
  particle S1 trio (1e-10 / f64 2.0 / π÷2), gm_18A5 95AF0
  post-renumber; header queue: gm_1601.h:53/:157-159/:168,
  TrainingModeState +0x114, lbaudio full_tree_with_pkg.patch WHOLE.
  **SPLIT QUEUE**: gm_18A5 FRONT (cuts proposed gm18a5_w12/LOG.txt;
  pends object-boundary + simultaneous-partition verification —
  deliberately NOT executed wave 15, outside the executor's
  ownership grant); serialize vs naming-12 + the four dirty
  inversion trees at the wave boundary.
  **WARNINGS**: (1) **mutex violated a FOURTH time** — naming-11
  committed mid-wave under the live gm_1601 split executor
  (range-disjoint, canary-proven no damage, but the wave-boundary
  serialization rule remains unenforced — orchestrator MUST gate
  it); (2) shared-build-dir refinement (grbigblue): a sibling's
  ninja rebuilds build/GALE01/src/<unit>.o from YOUR in-flight
  source — gate ONLY on campaign/scratch/build snapshots (cost one
  false +6-drift diagnosis); (3) sibling-flux gating: with a broken
  in-flight sibling source, `ninja <my .o>s build/GALE01/main.dol`
  still gates correctly (NonMatching units link extracted objects);
  full ninja once healed; (4) mwcc_sjis rejects non-ASCII comment
  bytes (em-dash); (5) REPORT.md harness-block persists —
  progress.txt is the durable record (tu-split-gm1601/,
  sislib-inversion-w15/, inversion-lb00F9-w15/,
  inversion-gmres-1832-w15/, grbigblue-inversion-w14/, naming11/);
  (6) per-symbol gates authoritative over mid-wave report.json flux
  (idioms 88/89). Idioms 89–92 added; +0x110 park RESOLVED. ~120
  compiles + 4 full ninjas. Uncommitted src: sislib.c + sislib.h,
  lb_00F9.c, gmresult.c + gmresult.static.h, grbigblue.c (+ stale
  backlog.json — triage.py before wave 16); wave-14 trees were
  committed pre-wave in 2cd6e2ad1.
- **2026-06-07 — Wave 14 (grand-inversion EXECUTION ×5 units + naming
  round 10) — idiom-72's worklist head fully executed; 23 fns→100
  (18 src + 5 naming).** ⭐ Five inversion agents (one owner per unit,
  src-only, every gate DOL+shasum PASS; trees UNCOMMITTED):
  **ftcoll** — root cause was idiom-24 **Fix A**, not B (target
  .sdata2 IS the anonymous pool; ftColl_804D82E0-family = names ON
  pool entries incl. 2 C-unreachable conversion magics): deleted 12
  const defs + 3 extern redecls, pool landed on target's exact
  offsets → resolve-paired instantly (idiom 84); **9 fns→100**
  (76640/699C/6CBC/7464/AB80/AD18/B128/GetWindOffsetVec/BBCC),
  fns<100 27→18, [.data-0] 54.1→**99.3** (idiom-69 decl-order
  reconstruction — byte-identical .data fixing anchor-folded d-forms
  in 4 fns; residual = unreachable dtk tail pad), [.sdata-0]
  15.4→40.0 (content exact, →100 pends @340 rename), .sdata2/.bss
  sizes target-EXACT (0x40/0x640); binary-proven: assert `NULL`→`0`
  ×12, 3 string-content typos (TU-dedup killed dup pool entries),
  7B320 asserts print assert_msg_1/2, phantom hit_sfx[20] + dead
  lbl_803C0C40 deleted; gate 24 UP / 0 DOWN / 0 GONE (12 sym-level
  drops = target data names pending rename, enumerated); 14-rename
  naming-11 map in ftcoll-inversion-w14/progress.txt. **camera** —
  14 fns UP / 0 DOWN / 0 GONE (~32 of 81 LIT2EXT+LC rows paired;
  29CF8 +4, BD88 +5, C5B4 +6 via cam_sqrtf clone, C010/B0E0/A0C0
  +2 each…); no pure-LIT2EXT fns existed so no new 100s (A4AC held);
  canary @717 tripped twice and rebalanced EXACTLY (idiom-48
  counterexample: inline DEFINITION consumed 2 ids); cm_804D7E20 =
  0x4330 magic confirmed, 13 rows C-unreachable → naming-11 Fix-A;
  report inversion-camera-w14/REPORT.md. **toy** — **5 fns→100**
  (un_80305D00/803062EC/80306BB8/80307018/803083D8); 8+18 rows fixed
  (+76 BONUS clone-pack rows in fn_8030B530) + @656/@895-7/@1188/
  setters ×4/@938 LC; [.sdata-0] 46.8→72.2; bytegate (masked-reloc
  .text vs pristine baseline_toy.o) proved ALL functions
  byte-identical — every gate DOWN (9) is reloc-name-only @-drift;
  address-keyed renumber map in toy-inversion-w14/REPORT.md; the
  only EXT2LIT 4 (un_804D6E80-8C) = fadds swap-pair rows,
  C-UNREACHABLE. **tydisplay** — **ALL 49 LIT2EXT rows CLEARED**
  (analyzer OK 390→439, LIT2EXT+LC 52→0), un_8031C1D0→**100**,
  un_8031BC54/B328/BF34 up; ZERO @-drift (@682..@1693 byte-verified;
  3 blob deaths repaid with exactly 3 goto-shims); binary-proven
  un_804D5AC0 empty-string (not "0") deviation fixed; 6 .rodata/
  .sdata2 objects defined via POSITIONED Fix-B (idiom 87; copy-then-
  index local tables); tree INCLUDES a 4-line symbols.txt rename
  (un_804D5AAC/B4→@699/@700, un_804DE01C/020→@1675/@1676,
  global→local shasum-proven, extends idiom 80) — must land WITH
  tydisplay.c; report tydisplay-w14/REPORT.md. **grvenom** — 28/30
  rows (LIT2EXT 30→2, OK 237→265), **3 fns→100** (80204428,
  802056B0 via reversed chain-assign, 80206BF0 via expanded
  __assert("wgobj") + zero-code label shim); 2 donor-keeper
  leftovers BY DESIGN (idiom 86 resolved the worklist's
  "order-swapped" note with NO decl/emission work); gate 6 UP / 0 fn
  DOWN ([.data-0] −0.37 = dead @1264 display artifact, accepted);
  NEW header tell: 80204F20 stores 1/0 through f32 xD4/xD8 → fields
  likely s32 (tgt has 2 extra grVe_803E5348 refs, size +0xC);
  progress.txt in grvenom-inversion-w14/. **Naming-10 COMMITTED
  f9229dac1** (symbols.txt only, +39/−37): 36/39 particle-split pins
  applied (all value-verified pin-by-pin, 0 fails; hsd_3983 18 +
  hsd_39D1 18) + lbl_8040BFB0 SUBDIVIDED → @236/@237/@238 (exact
  NUL-terminated sizes per lbl_8040C248 precedent, all 3 pair 100);
  net 23 UP / 0 DOWN (psInitDataBankLoad/98C04/991D8/D3AC/EE24→100);
  3 BLOCKED pins honored (lbl_804DE98C 1e-10 / @2723 f64 2.0 /
  lbl_804DE9E8 π÷2 — need particle S1 shape fixes first); DOL+shasum
  at baseline/post-apply/no-op idempotence; post-apply concurrency
  re-check: 39/39 re-verified on then-current objects, all 5
  inversion units' split ranges script-proven DISJOINT, symbols.txt
  cmp-intact through commit — no naming-9-style clobber of the
  committed file.
  **NAMING-11 QUEUE**: ftcoll 14 address-keyed (ftColl_804D82E0→@312,
  E4→@337, E8/EC/F0→@463/@464/@465, F8→@468, 8300→@771, 8308→@1520,
  8310→@1102, 8314→@1518, 8318/831C→@1778/@1779, ftColl_804D3A68→
  @340, assert_msg_0→@655; all scope:local, cross-TU grep-clean —
  recovers all 12 drops + 83 rows + [.sdata-0]→100); camera
  cm_804D7E20→@274 ×13 + cm_804D7E18→@272 + cm_804D7E28→@297 (each
  with coordinated EOF-def + header-decl deletion) + NAMEPAIR
  cm_803B73B8→cm_WorldForward (A768 ×2); toy renumber map (@626→@627,
  @1182-87→@1198-1203, @1380-87→@1395-1402, @1899-1915→@1913-1929,
  @3537/8→@3571/2; kill target-true @535/@656/@895-7/@938/@1062/3/
  @1188/@1391) — VALUE-KEYED re-verify at land time is policy
  (precomputed shim ledgers hopeless, toy empirics); tydisplay
  re-pin float @1021 (.sdata2 0x804DDF64, false-pairs with our .data
  string) FIRST, then un_80319EF0 @1021→un_803FF074 ×2 (+ naming-11
  re-apply map in tydisplay-w14 REPORT addendum); naming-10 BLOCKED
  trio after particle S1; carryovers: lbaudio unit-map recompute,
  gm_1601 dup-literal pins post-split, optional jtbl→@ probe.
  **SRC/RETRY QUEUE**: remaining inversion worklists (idiom 72):
  grbigblue 20 → sislib 11 (@264 dup-literal split) → lb_00F9 8
  (keep @328 alive) → gmresult 5 → gm_1832 3 (@-budget); ftcoll
  7861C pool-permutation probe (frame 0x48 vs 0x40 = one dead
  u32→f32 temp pair; frees 3 more 100s + [.sdata2-0]→100); camera
  29CF8 dedicated S1 (extra callee-saved f26 web); grvenom 80204F20
  s32-field header verify; particle S1 shape fixes (1e-10 / f64 2.0
  / π÷2) unblock the 3 naming pins; wave-13 carryovers unchanged
  (gm_1601 gm_80166378 + fn_8016588C, .data reconstruction agent,
  gm_18A5 95AF0 post-renumber; header queue: gm_1601.h:53/:157-159/
  :168, TrainingModeState +0x114, lbaudio full_tree_with_pkg.patch
  WHOLE).
  **SPLIT QUEUE (unblocked for wave 15)**: gm_1601 (spec READY,
  gm1601_w13/cuts_gm1601.txt — front) → gm_18A5 (cuts proposed in
  gm18a5_w12/LOG.txt, SECFOLD 6 re-confirmed; pends object-boundary +
  simultaneous-partition verification) — serialize vs naming-11 and
  the six dirty trees at the wave boundary.
  **WARNINGS**: (1) mutex HELD for the committed file this round
  (naming-10 proved range-disjointness pre- AND post-apply) BUT a
  sibling clobbered tydisplay's working-tree symbols.txt at 02:24
  (re-applied + fully re-gated) — src agents' working-tree config
  edits need the same wave-boundary serialization; (2) do NOT kill
  camera's 0.65/0.0/1.0/1e-4 pool literals (928C/958C/29AAC) nor
  BAA8's plain sqrtf expansion — offset-pairing + weak-sym ballast
  depend on them (idioms 84/85); (3) idiom-79 dtk symbols.txt
  auto-rewrite RECURRED on every re-splitting ninja in the toy tree
  (4 lines, tylist/tydisplay ranges, NOT toy — saved diff + reverted;
  `git status` symbols.txt after each ninja); (4) REPORT.md harness
  block persists for 2 agents (ftcoll + grvenom — progress.txt is
  the durable record, naming-7 precedent); (5) report.json fuzzy is
  reloc-name-BLIND (idiom 88) — never gate inversion work on it;
  (6) shared build dir: one agent's ninja rebuilds siblings'
  in-flight objects (harmless — gates are unit-scoped; snapshot
  before judging). Idioms 84–88 added; idiom-23/41/48 cost tables
  re-flagged context-dependent (grvenom measured goto-shim +1 vs
  gm_1601's zero — ALWAYS canary). ~110+ compiles reported
  (16/45/35/14 + camera/naming uncounted). Uncommitted src:
  ftcoll.c, camera.c, toy.c, tydisplay.c (+ 4-line symbols.txt),
  grvenom.c, plus wave-13 carryovers (gm/types.h, gm_1601.c,
  lbaudio_ax.c; stale backlog.json — triage.py before wave 15).
- **2026-06-07 — Wave 13 (particle TU-split + naming round 9 +
  grand-inversion sweep + types.h header land + 2 unit campaigns) —
  five commits landed, HEAD b701858cc.** ⭐ **TU-SPLIT particle LANDED
  + COMMITTED** (2 splits / 3 TUs: 11be7f00c hsd_39D1.c/generator.c +
  2e522ed29 hsd_3983.c/particle.c + 572614b9c/b701858cc binary-proven
  fixes; verdict section updated): all gates PASS at every step, 119
  fns preserved (89+15+15), project matched 72.062→**72.100** / fuzzy
  97.537; 9F05C 78.41→96.33, psInitDataBankLoad→99.86, extab/extabindex
  rows →99+; 5 binary-proven deviations fixed (reader-slot count/table,
  spawn-remap +0x60, 100.0F kill, 0.001F eps, assert strings); required
  5 byte-safe scope:local→global flips (idiom 80 narrows idiom-75);
  gate 12 UP / 7 DOWN all = @-drift with address-keyed maps →
  naming-10; ~55 compiles. **Naming-9 COMMITTED 1f9458ec3**
  (symbols.txt only, 43 address-keyed renames): 35 syms UP / 0 DOWN,
  24 fns→100 (fn_80182F40, fn_80181708, ftCo 34-pin @N→@N+4 recovering
  all 22 dropped 100s incl. canaries B0918/B101C), project matched
  72.06→72.09 pre-particle; DOL+shasum+deps#3 at every checkpoint;
  skips honored (gm_18A5 inversion cancels, @2089 bulk, lbaudio str_*,
  fn_80188EE8 permanent). ⭐ **GRAND-INVERSION SWEEP (analysis-only,
  zero edits)**: analyze.py + 27 worklists in
  `campaign/scratch/inversion-sweep/` — **LIT2EXT 361 vs EXT2LIT 4
  (~90:1), SRC-FIRST conclusively dead**; wave-14 src owner order
  ftcoll→camera→toy→tydisplay→grvenom→grbigblue→sislib→lb_00F9→
  gmresult→gm_1832 (details in idiom 72); naming-9 queue AUDITED
  (gmregclear + ftCo healed-verified; 4 missed ftCo .data pins →
  naming-10; gm_18A5 cancels confirmed; mechanics: idioms 76–78).
  ✅ **HEADER LANDED: gm/types.h:802 TmUnkMenuData → `u16 x9; u16
  xB;`** (uncommitted src): fn_80194F30 99.76→**99.97** (beats the
  99.90 prediction; sth/stb row gone), 3 unit gates PASS
  zero-regression, DOL ×3; →100 now pends ONLY the gm_18A5 TU split
  (lbl_804799B8 anchor pair). INCIDENT: post-edit dtk re-split
  AUTO-REWROTE symbols.txt with the queued gmregclear renumber —
  reverted, did not recur (idiom 79; saved diff = machine-derived map).
  Unit wins (UNCOMMITTED src, one file per agent): **lbaudio_ax** —
  27DF8 96.81→**98.70** (single-use inline expansion + arg0-load hoist
  ×2) + 28690 97.81→**97.84** with frame 0x28 EXACT (idiom-40
  quantified); gate 2 UP / 0 DOWN / 266 SAME, only free-territory
  @-churn; ~45 compiles; B24 declared SRC-COMPLETE. **gm_1601** — 4
  fns up, gate 4 UP / 0 DOWN, pins byte-stable through ~85 compiles:
  gm_80166A98 80.04→83.55 via binary-proven x3-bitfield fix (= **100.00
  EXACT** verified under gm_1601.h:168 arg1 s32→u8 — NEW header item,
  also lifts caller gm_801B0DD0 72.73→78.76 with zero other changes),
  fn_80164B48 85.46→**99.28**, fn_80169000 89.03→**95.80**
  (uninitialized-return tell — invented `t = 0` deleted), fn_80169C54
  92.88→**96.10**; + gm_1601 TU-SPLIT SPEC DERIVED report-only (2 cuts
  / 3 TUs, verdict section; gm1601_w13/cuts_gm1601.txt) — front of
  split queue. **HEADER QUEUE**: ✅ TmUnkMenuData LANDED; NEW
  gm_1601.h:168 s32→u8 (verified-then-reverted, →100 + caller +6);
  carry-overs unchanged: gm_1601.h:53, gm_1601.h:157-159,
  TrainingModeState +0x114, lbaudio full_tree_with_pkg.patch WHOLE.
  **NAMING-10 QUEUE**: particle renumber maps (address-keyed,
  value-verified, tu-split-particle/progress.txt; hsd_3983 +
  hsd_39D1; @2723 + lbl_804DE98C/9E8 BLOCKED; head dups keep pins;
  re-verify ids against then-current objects) + subdivide lbl_8040BFB0
  → @236/@237/@238 (psInitDataBankLoad last 2 rows →100); ftCo 4
  missed .data pins (@1929→@1933 @0x803C5B5C, @3040→@3044, @5861→@5865,
  @6168→@6172); lb_00F9 @176→@328; grgreens @1017→@828; lbaudio
  @721→@722 + @848→@849 + recompute the unit map AFTER this wave's src
  (+11 ids past line 2503, @1877→@1888); gm_1601 gm_80166378 idiom-54
  dup-literal pins AFTER its split (they then separate naturally);
  optional jtbl→@ rename probe (ftCo ×2 — settles jumptable-park
  config-recoverability). **SRC/RETRY QUEUE**: wave-14 inversion
  conversions (order in idiom 72, one owner per unit); particle S1
  agents (hsd_3983 + hsd_39D1 residual lists in parks); gm_1601 next:
  gm_80166378 (+0-copy struct var, r30 web) + fn_8016588C
  (binary-proven x74 lwz/s32 vs our lbz/u8 + switch→if-else-chain);
  .data reconstruction agent (head debugconsole strings + hsd_3983
  @1491/@1492 + lbaudio package); gm_18A5 95AF0 package still blocked
  pending its renumber. **WARNINGS**: (1) **naming/split-vs-src MUTEX
  violated a THIRD time** — naming-9 committed mid-wave under 5 live
  agents AND its first application was clobbered by a concurrent
  agent's git-restore of symbols.txt (recovered by atomic idempotent
  re-apply with cmp-verification); types.h also landed under running
  src agents (verified harmless via snapshots). Agents must NEVER
  git-restore shared config for "clean baselines"; the orchestrator
  must serialize naming/split/header lands at wave boundaries. (2) dtk
  re-split can rewrite symbols.txt unprompted — `git status`
  symbols.txt after every re-splitting ninja (idiom 79). (3) REPORT.md
  remains harness-blocked for several agents — progress.txt is the
  durable record (naming-7 precedent, 4 agents this wave). Idioms
  76–83 added; idiom-48 cost table flagged context-dependent (3
  zero-measurements); idiom-75 narrowed. ~230 compiles total.
  Uncommitted src: gm/types.h, gm_1601.c, lbaudio_ax.c (+ stale
  backlog.json — regenerate via triage.py before wave 14).
- **2026-06-06/07 — Wave 12 (gmregclear TU-split + 4 unit campaigns +
  naming round 8) — first wave with TWO config commits landed
  mid-flight.** ⭐ **TU-SPLIT gmregclear LANDED + COMMITTED** (3 splits /
  4 TUs: 64f0dedfb gm_182F.c, e079c36ff gm_181A.c, 831a72891 gm_180A.c —
  landed one at a time in REVERSE address order, recipe refined; verdict
  section updated): every-section constraint 0 violations/245 refs, all
  gates PASS per split (DOL+shasum+deps#3+107 fns preserved+idempotence;
  s1 242S/1U/1D, s2 235/2/1, s3 199/4/2, 0 GONE/NEW), project matched
  72.028→**72.042**, gm_80181998 →**100 EXACT**, fn_80180C60 98.07,
  fn_80181E18 ~16 d-forms fixed (rotation park is now its only
  blocker); part0 further cuts deliberately SKIPPED (would regress
  offset-0-shadow 100s). **Naming-8 COMMITTED d47c7b618** (symbols.txt
  only): 80 rows eliminated, 16 fns→fuzzy-100 (gm_1884 renumber map
  applied+extended — 8 of 10 split DOWNs back to exactly 100; lbaudio
  2392C; gm_18A5 +7), zero regressions, DOL+shasum every checkpoint;
  project 97.52% fuzzy / 72.04% matched. Unit wins (UNCOMMITTED src,
  one file per agent): **ftCo_0A01 ×1 EXACT** — A2718 79.81→**100**
  (NEW idioms 73/74: pragma moved to CALLEE A1F98 + ONETT static-inline
  r0-diamond + while-walk −3 ids + pad swap); A6A98 89.5→96.1 / A6D2C
  88.9→95.6 (FY-PROP park found), A2C80 +0.6; gate 4 UP / 29 "DOWN"
  ALL verified pure @N→@N+4 reloc drift (~65 compiles). **gm_18A5** —
  ⭐ GRAND INVERSION law (idiom 72): naming-8's SRC-FIRST direction was
  backwards for every lbl-target site; literal→extern conversions won
  gm_80190EA4→**100** + fn_80198C60→**100** + 7 more UP (9 UP/0 fn
  DOWN, 173→175@100, sdata2 −36B toward target; CANCELS most of the
  gm_18A5 naming-9 queue; TU-split triad cuts proposed; mid-wave rebase
  forced by the two commits landing under it; ~35 compiles).
  **lbaudio_ax** — B24 95.72→98.79 (idioms 67/68; residual now naming-
  class only), 233EC→95.00, 28690→97.81 (+2 binary-proven HSD_ASSERT
  literal fixes), 2392C→**100** (wave-11 accepted drop healed in src),
  27168/27AB0/[.data-0] up via dead-static pool stand-in; ⭐ **.data
  RECONSTRUCTION PACKAGE byte-PERFECT** — whole 0x1818-byte .data
  byte-identical to DOL, syms@100 246→290, DOL-gated, then REVERTED per
  header rule; land `lbaudio_w12/full_tree_with_pkg.patch` WHOLE
  (flags_arr alone is net-negative); gate 8 UP / 0 fn DOWN (~78
  compiles; idioms 69–71). **mplib** — approved header edit LANDED
  (mplib.static.h:35 [0x200]→[0x80]; mpLib_80458888 50→**100**, exactly
  1 move/0 drift over 338 syms — queue item CLEARED); DrawCrosses
  post-header retest no-change (park FINAL ~18 forms), mpLibLoad +13
  probes all no-ops (park strengthened, idiom-75 negatives); unit has
  NO naming rows left, all 38 <100 parked (~35 compiles).
  **HEADER QUEUE (updates)**: ✅ mplib.static.h [0x80] LANDED;
  gm/types.h:802 TmUnkMenuData `u16 x9; u16 xB;` RE-VERIFIED on current
  tree (fn_80194F30 99.76→99.90; x37[].xC zero users) — still pending;
  lbaudio: land full_tree_with_pkg.patch whole — SUPERSEDES the
  offsets_arr `={0}`+decl-order-surgery item AND byte-PROVES the
  wave-8 sfx_remap [0x4A]→[0x4B] sentinel; TrainingModeState +0x114
  split still pending; optional ftCo_800A1F98 prototype swap
  (float y, int x) NOT needed (A2718 is 100 without it).
  **NAMING-9 QUEUE**: gmregclear split renumber (address-keyed map in
  tu-split-gmregclear/REPORT.md: gm_182F @2166-68→@245-247 + @298→@249;
  gm_180A @1170→@449 @674→@450 @1684/85→@451/452 @298→@454; gm_181A
  empty; head pins unchanged; idiom-54 dup copies at head addresses
  keep existing pins); ftCo_0A01 uniform @N→@N+4 renumber for the 34
  listed pins from @997 (recovers all 22 dropped 100s incl. canaries
  B0918@5205/B101C@5350 — verify per-pin, sdata2 shared across units);
  gm_18A5 CANCEL 6D8-E4→@1601-04, 82C-840 six-pack, 734/738/75C
  (Grand Inversion landed these src-side; @1896 gone); only the
  @2089+2 bulk renumber remains (contingent on the 95AF0 package);
  lbaudio @1921/@1923/@1925/@1922 → str_main/pokemon/nr_title/
  nr_name_ssm + lbl_803BCA24 pairing decision when the package lands;
  fn_80188EE8 ×8 promotion-suffix rows = PERMANENT accepted drops
  (idiom-75: scope flips change DOL bytes).
  **RETRY QUEUE**: gm_18A5 fn_80195AF0 package (STILL blocked —
  naming-8 touched nothing at @3025+); ftCo A3908/A4038 dedicated
  @-budget session (post-renumber, now more attractive); ftCo wave-13:
  A6A98/A6D2C aggregate-redef probe + A6FC4 FPR-rotation; particle
  fn_80394DF4 comma-form (4th carry-over); particle .data
  reconstruction agent — lbaudio package PROVES the method (idioms
  69–71); SPLIT QUEUE NEXT: gm_1601, particle, gm_18A5 (cuts proposed).
  **WARNINGS**: (1) **naming/split-vs-src MUTEX violated TWICE** —
  naming-8 AND the gmregclear split committed mid-wave under running
  src agents (gm_18A5 rebased via controlled HEAD-src-vs-new-target
  gate; split agent re-verified all gates post-naming-8; no damage, by
  luck of range-disjointness) — wave-level mutex MUST be enforced by
  the orchestrator, not agent vigilance; (2) ftCo-style all-DOWN gates
  can be 100% benign @-drift — enumerate per-row before reacting,
  renumber before the next wave's baselines; (3) `match_percent`-only
  gating reconfirmed (3rd wave); (4) mplib harness still blocks
  REPORT.md (progress.txt is the durable record, naming-7 precedent);
  (5) display scores can DROP on byte-strictly-better code
  (fn_80181E18 92.66→92.08, all-or-nothing row scoring) — byte-compare
  anchors with cmpfn.py before judging splits. Idioms 67–75 added;
  idiom-23/41/48/50 cost-model caveats logged in 72/74. ~250 compiles
  total. Uncommitted src: ftCo_0A01.c, gm_18A5.c, lbaudio_ax.c,
  mplib.static.h (+ stale backlog.json).
- **2026-06-06 — Wave 11 (TU-split execution + 4 unit campaigns + naming
  round 7) — the wave-10 queue, fully recovered.** ⭐ **TU-SPLIT
  gm_1832/gm_1884 trio LANDED** (uncommitted working tree; verdict
  section updated with status, 7-cut table, renumber map, and the
  validated 6-step execution recipe): zero deviations from the wave-9
  proof, 4/4 gates PASS, 69 fns preserved (46+23), anchors 95.28→99.18 /
  99.0→99.34, project matched 71.96→**71.99%**, 11 DOWNs all
  prototype-identical @-repartition rows. Unit wins (UNCOMMITTED src,
  one file per agent): **ftCo_0A01 ×2 EXACT** — B1478→100 (idiom-66
  helper swap) + AF290→100 (parked 5-web rotation CRACKED via
  sibling-shape rewrite; −13 @-debt repaid exactly, canaries
  @5205/@5350 stable; ~40 compiles); **lbaudio_ax ×1 EXACT**
  (fn_80025E38→100, idiom 52) + 5 big climbs (B24 74.3→95.7 via
  FindSlot/BusyWait inlines, 233EC 85.8→94.9, 28690 92.4→97.8, 27648
  96.8→98.8, 27DF8 95.2→96.8) + 5 binary-proven deviations fixed (read-
  through-garbage pointer load → direct array, strcpy/SynthSFXLoad
  +0x40, ssm string literals → name-table reads ×5, duplicated saturate
  arm; ~85 compiles; gate PASS w/ 3 enumerated drops — 2392C 100→98.67
  pure @-drift pends renumber, "/audio/\0" .sdata→.data matches target);
  **gm_18A5** 6 fns improved, 12 UP / 0 DOWN (fn_8019175C 96.6→99.9
  binary-proven GET_JOBJ(gobj) fix, 801905F0 99.3→99.9, 92758 96.8→98.3,
  9249C 94.8→97.5 idiom-60, 8F888 91.7→95.7) with the object @-table
  kept BYTE-IDENTICAL to HEAD via shims (~80 compiles); **mplib**
  mpLibLoad 99.05→99.92 (idiom 64; only edit), wave-10 lost probe matrix
  re-run AND recorded, 3 parks confirmed with full enumeration (~35
  compiles). **Naming round 7 COMMITTED 3afa5a67c** (symbols.txt only):
  89 rows eliminated, **16 fns→fuzzy-100** (mplib 20→0 rows incl.
  VtxIds 4-way split, DrawEcbs/DrawSpecialPoints 100; gm_18A5 168→102,
  12 fns 100 incl. all 5 queued near-100s; particle dup-@/dead-data
  pins, baselib_mfspr + fn_80392A08 100), zero fns left 100, DOL PASS
  every checkpoint; empirical net-judging caught a TRUE regression
  (fn_80198D18 100 extern-uses the 82C-840 six-pack — applied-then-
  reverted, fn_80198C60's would-be 100 sacrificed); 6 pure-reloc drops
  accepted for +27 rows. Idioms 55–66 added; idiom-48 cost table
  extended; idiom-50 frame claim flagged (one id+frame-neutral
  hoisting counterexample); naming-7's 100-predictions partly wrong
  (80059554/DrawMatchingLines kept real codegen rows).
  **HEADER QUEUE (updates)**: TrainingModeState +0x114 split is now
  fn_80188644's ONLY blocker (→100); mplib.static.h:35 [0x200]→[0x80]
  STILL PENDING from wave 9 (re-test DrawCrosses after); lbaudio
  offsets_arr_803BC4E4 `= { 0 }` verified again (+0.4..+5.3 on 4 fns
  incl. 2838C→97.74) but land ONLY with .static.h decl-order surgery
  (target order lbl_803BC314 → unk_arr_803BC4A0 → offsets_arr →
  sfx_remap; as-is breaks 23F28/24030/243F4 + lbl_803BC314); NEW:
  missing 0xD4 .data object flags_arr_803BB800 (symbols.txt:20428,
  between s32_arr_803BB6B0 and s32_arr_803BB8D4) aligns lbaudio fold
  offsets (+0x500→+0x5d4). gm_18A5: no header requests this wave.
  **NAMING-8 QUEUE**: gm_1832/gm_1884 @-pin renumber (map in verdict
  section — recovers all 10 split DOWNs + orphan @1843); lbaudio
  renumber +10 head/−2 tail (recovers 2392C); gm_18A5 leftovers
  (lbl_804DA738/734 → fn_80192E6C; 6D8-E4→@1601-04 SRC-FIRST;
  lbl_804D4170/178→@354/@355; 82C-840 six-pack SRC-FIRST; 7F8→@357
  dup-pin → fn_80197AF0 100; @2089..@3553 uniform +2 bulk-renumber
  candidate); particle .data dead-string reconstruction agent still
  owed (now also blocks lbaudio B24/233EC/28690 pooled-string rows).
  **RETRY QUEUE**: gm_18A5 fn_80195AF0 package re-apply AFTER renumber
  (98.07 proven, text preserved); ftCo A3908 dedicated session with
  @-budget (diagnosis upgraded, see parks); particle fn_80394DF4
  comma-form (still pending, 3rd carry-over).
  **WARNINGS**: (1) **naming-7 landed mid-wave UNDER running src
  agents** — gm_18A5 + mplib had to rebase gates mid-flight (handled
  via controlled HEAD-src-vs-new-target re-baselines); adopt a
  wave-level MUTEX between naming and src/split agents per unit; (2)
  gate objdiff-cli JSON on `match_percent` — `fuzzy_match_percent` is
  null in this build and yields a false all-SAME; (3) build.ninja had
  LOST --no-warn-error at session start (ifstatus.c aborts builds) —
  always `python3 configure.py --no-warn-error` before baselining; (4)
  gm_18A5 extern-vs-pool dup conflicts (lbl_804DA6D8/DC/E0/E4, 6E8,
  734/738/744/75C, 82C-840) are NET-NEGATIVE config-side — convert the
  src externs first (src-agent queue, idiom-24 style); (5) **HARD
  CONSTRAINT: gm_18A5 src agents must keep the @-table byte-identical
  to HEAD numbering** (~20 pins landed in-unit; verification recipe in
  gm18a5_w11/LOG.txt); (6) piped-wine-stderr "User break" gotcha re-hit
  once (self-inflicted); (7) durable-report rule WORKED — every agent
  left incremental scratch artifacts (tu-split-exec-w11/, gm18a5_w11/,
  mplib_w11/, ftco0a01_w11/, lbaudio_w11/, naming7/progress.txt; note
  naming-7's harness blocked REPORT.md — progress.txt is the record).
- **2026-06-06 — Wave 10 (ABORTED — zero results, zero losses).** 6 agents
  launched ~20:21 (tu-split exec gm_1884; unit campaigns gm_18A5, mplib,
  ftCo_0A01, lbaudio_ax; naming round 7); ALL six reports returned null —
  the wave died by ~20:35 before any agent reported. Forensics from scratch:
  tu-split exec snapped `tu-split-exec/pre_gm_1832.json` only (split never
  applied; gm_1884.c never created); mplib agent got furthest — ~14 probe
  variants on mpLibLoad in `mplib_w10/v/` (split/rev/cast-self/+0/comma/
  init-decl/chain forms), outcomes unrecorded; gm_18A5 + ftCo_0A01 +
  lbaudio_ax + naming-7 only snapped baselines (`agent_gm18A5_w10/`,
  `ftco0a01_w10/`, `lbaudio_start_w10.txt`, `naming7/`). All src/config
  touches (configure.py, splits.txt, gm_1832.c, mplib.c+.static.h,
  ftCo_0A01.c, lbaudio_ax.c) were restored — tree verified clean at
  7fc5d64d5, DOL sha1 PASS (08e0bf20). NO wins, NO new idioms, NO commits;
  parked list unchanged. ENTIRE wave-9 queue carries over unworked:
  TU-split gm_1884 trio land (front of queue) → gmregclear → gm_1601 →
  particle → gm_18A5; wave-9 header-edit queue (TmUnkMenuData, gm_1601.h
  ×4, mplib.static.h [0x80], TrainingModeState +0x114 split) still
  pending; naming-7 queue (mplib renumber @4595/@4596 + VtxIds 4-way +
  D80D4/D8 locals; gm_18A5 lbl_804DAxxx→@NNN incl. @357 conflict pick;
  particle renames + dead-string .data) still pending; retry queue
  (particle fn_80394DF4 comma-form) still pending. WARNINGS: (1) null-
  report failure mode is real — durable-report rule added to
  Infrastructure; mandate `campaign/scratch/<agent>/REPORT.md` written
  incrementally; (2) touched-then-reverted files leave misleading mtimes —
  gate any "what changed" tooling on `git status`, not mtime; (3) mplib
  mpLibLoad probe matrix must be re-run — variants preserved in
  `mplib_w10/v/` + harness `probe2.py`, only the OUTPUTS were lost.
- **2026-06-06 — Wave 9 (5 unit campaigns + TU-split experiment + naming
  round 6).** 9 fn wins + ~12 data/section symbols (UNCOMMITTED src; only
  each agent's own unit files touched, all headers verified-then-reverted):
  particle ×3 fn + 6 data (whole-unit .bss reconstruction lands the
  13-symbol block; 196→205 syms@100; gate PASS with 5 enumerated drops
  incl. a LOAD-BEARING `if(0)` shim in hsd_80392E80); gm_1601 ×4 fn
  (param-web blocker, post-increment, loser-nibble union, DE8 rewrite;
  +2 MORE verified 100 under pending header edits; unit 30→26 unmatched;
  gate CLEAN); ftCo_0A01 ×2 fn (sqrtf depth-2 wrapper, x1A88 data-ptr web;
  AF290 96.6→99.4; +13 @-id debt fully repaid via shim arithmetic, pins
  @5205/@5350 verified; unit 30→28); mplib data sweep — [.data-0] →100.0,
  273→278 syms@100, VtxIds +0x4C cracked (idiom 51), zero true
  regressions (2 @drift rows pend the renumber below); gm_18A5 +20 UP /
  −10 DOWN net via the gm_804771C4 extern-flip PACKAGE (fn_80191240
  79.9→98.1, no exact wins; reversible only as a package; OBSOLETED by TU
  split when it lands). **TU-SPLIT EXPERIMENT: VERDICT PROVEN** (dedicated
  section above): splits.txt DOL-safe but insufficient alone; trio recipe
  (splits+configure+src) proven end-to-end in scratch; gm_1884 prototype
  ready at campaign/scratch/tu-split-1884/; queue gm_1884 → gmregclear →
  gm_1601 → particle → gm_18A5. Naming round 6 COMMITTED 4ae9faf9d
  (symbols.txt only): 124 rows eliminated (gm_1832 79→7, gmregclear 61→12
  recovering the round-5 accepted drops), 20 fns→fuzzy-100, 66 lines, DOL
  PASS at every checkpoint; duplicate-@ pinning VALIDATED (idiom 54); toy
  13 renames SRC-BLOCKED (extern refs in toy.c → src-agent queue,
  idiom-24 Fix-B; un_803FE150/E1E0 demotion OBSOLETE); lbaudio 6-sym bss
  merge blocked on the queued .static.h surgery. ~8 binary-proven
  deviations fixed toward DOL: mplib mpLib_803BF738 initializer (s16-
  truncated floats) + mpLib_803BDCB8 missing 20th entry; gm_1601
  fn_80160DE8 swapped args, fn_80165548 first-winner break, fn_80161C90
  dead 0xFFFFFFFFU clamp; gm_18A5 fn_80192938 7-field offset pack (x3
  self-inconsistency), gm_801905F0 x0_0 3-bit mode field, fn_80191B5C
  phantom _data array. Header-edit queue (each verified via temp edit,
  reverted): gm/types.h:~802 TmUnkMenuData `u16 x9; u8 xB; u8 xC;` →
  `u16 x9; u16 xB;` (F30→99.97, no other users grep-verified);
  gm_1601.h:53 bool→u8 arg2 (gm_80160C90→100); gm_1601.h:157-159 u8→s32
  returns ×3 (fn_80165548→100; 418/4A0 stay 100; no external callers);
  mplib.static.h:35 mpLib_80458888 [0x200]→[0x80] (→100); plus TU-split
  follow-up: split TrainingModeState at +0x114 (fn_80188644→100).
  Config/naming-7 queue: mplib renumber @4595/@4596→@4594/@4595 +
  VtxIds 4-way symbols.txt split + mpLib_804D80D4/D8→@4259/@4260
  scope:local (nets DrawSpecialPoints/DrawEcbs/MatchingLines/80059554
  100s); gm_18A5 S5 lbl_804DAxxx→@NNN queue (≥12 fns; near-100s
  fn_80191D38/1678/2690/1CA4/10E0 reach 100 on renames alone; @357
  conflict: lbl_804DA710 vs lbl_804DA7F8 — pick one); particle S5
  baselib_mfspr/fn_80392A08 renames + .data dead-string reconstruction
  family (dedicated agent, ~0x2BA4 bytes). Retry queue: particle
  fn_80394DF4 comma-form `(headp = &sp->xD0, (head = *headp) == NULL)`.
  WARNINGS: ftCo_0A01's shim ledger CHANGED (AE7AC do-whiles + AF78C
  PAD_STACK/goto are GONE, replaced by expansions+var_f4 — any future ±id
  edit there must re-verify canaries B0918(@5205)/B101C(@5350));
  re-confirmed never trust `check_fn --no-build` after edits; splits agent
  transiently split gm_1832 mid-wave (naming agent unaffected — renames
  address-keyed; coordinate splits vs naming next time); idiom-29's
  r3-rebind claim DISPROVEN (corrected in place); block-scope named
  locals cost +2 file-wide @ids with no CFG change (idiom 48) — canary
  @pins after any local addition. Idioms 46–54 added; S2 particle-BSS
  warning resolved.
- **2026-06-06 — Wave 8 (6 unit campaigns + naming round 5).** 19 fn + 1
  data matches (UNCOMMITTED src, 7 files dirty): gm_1832 ×10fn+1data
  (plus-zero blockers, inline play helpers, @-pin realignment collateral;
  unit 97.41→97.60), gmregclear ×3 (decl reorder, load-through-ptr,
  struct-ptr hoist), ifcoget ×3 (un_804A1F58 repack to true 0x80 layout
  {hdr 8B + 6×0x14 slots} — root-cause fix, resolves file TODO + memzero
  hack), lbaudio_ax ×2 (auto-inline reconstruction of duplicated bodies),
  toy ×1 (Toy_LoadLObjList — FIRST ty-module match). ftCo_0A01: 0 wins but
  5 fns improved ~150 rows; its do{}while(0)/if(0)/goto shims are @-pin
  LOAD-BEARING for 4 pinned siblings (replace only with a symbols.txt
  renumber pass). Gates: 5 CLEAN/PASS zero-regression; gmregclear PASS
  with 2 enumerated accepted drops (fn_801803FC/fn_80180630, bit-identical
  to HEAD, pure @-drift pairing — renumber unit pins AFTER src lands per
  idiom 23). 5 binary-proven deviations fixed toward DOL: gm_80189CDC
  zero loop 25→27 words (cmpwi 0x1b proof); fn_80188EE8 (f32)(u32) convs;
  fn_80181C80 unconditional `return x0++`; lbAudioAx_80027168 arr274[i] +
  OOB descending [55]-scan → ascending prefix scan; fn_802FF218 OOB slot-0
  header read → slot flag byte/score word. Naming round 5 committed
  8b85bcfe0 (branch campaign): 608 lines, ~1032 rows, ~113 fns to
  fuzzy-100, 47 units gated, DOL OK; lbcollision 157→0 rows (+10 fns),
  synth 60→1, mnmain 38→1, mplib 50→2; project matched code
  71.66%→71.72%. INFRA FINDING: ninja deps DB had LOST the split depfile
  (`ninja -t deps build/GALE01/config.json` → #deps 0) — symbols.txt edits
  silently never re-split and the DOL gate passed trivially; fix `touch
  config/GALE01/config.yml && ninja`; VERIFY #deps ≠ 0 before every naming
  round. Header/config queue (orchestrator): gm_1832.h:66 gm_80189CDC
  return → void (bgelr-proven; check callers); gm/types.h:1037
  char_data[25]+pad_6C[2] → char_data[27]; types.h anim_frames[39]/
  menu_values[7] s32→u32 candidate; lbaudio_ax.static.h:242
  offsets_arr_803BC4E4 add `= { 0 }` (.bss→.data, HIGH VALUE: measured
  +0.4–+4.2 on 3 fns, 2838C 93.5→97.7); lbaudio sfx_remap [0x4A]→[0x4B]
  low-confidence; gmregclear.h gm_8017DB88 arg6-8 callback ptrs u8→int
  returns (+ gmclassic.c:707 cast); gmregclear struct merge lbl_80473594
  into lbl_80472ED8+0x6BC (layout stream; bss layout unaffected);
  splits.txt add `.sdata2 0x804DDC00–0x804DDC20` to melee/if/ifcoget.c
  (fn_802FED14 → 100 with zero C changes). Naming round 6 queue: gm_1832
  tail @-drift −1 (@1651-54/@1901-07 family) + @913→lbl_804DA648 +
  @515/516 + @1074 TU-dup literals; gmregclear pin renumber (recovers the
  2 accepted drops) + lbl_804DAxxx rows + .data layout restoration
  (lbl_803D7AC0/85F0/8D08, layout stream); toy un_803FE150/E1E0
  synthetic-reloc demotion policy + @288/@535/@938 vs un_804DDCxx
  offset-pairing; lbaudio merge 6 bss syms into one 0x1F554 + .static.h
  .data decl-order surgery; src-agent queue from skip-class diagnoses:
  itdosei it_804DC878 extern (53 rows), ftcoll 13 names, ftCo dup-literal
  units need idiom-24 Fix-B in src. Retry queue: gm_1832 fn_80185F5C
  (same arg-reg signature as solved cluster); gmregclear web-rotation
  needs enumeration agent first. WARNINGS: check_fn.py `--no-build` diffs
  the stale CANONICAL object — never trust it after an edit (bit 2
  agents); /tmp scripts clobbered between concurrent agents — use
  campaign/scratch/<agent>/; report.json remains lenient (ED14 "100" was
  really 99.79 — controlled scratch baselines only); backlog.json
  stale-modified, left uncommitted. Idioms 34–45 + idiom-25 negative
  added above.
- **2026-06-06 — Wave 7 (6 unit campaigns + naming round 4).** 12 new fn
  matches in working tree (UNCOMMITTED src): gm_18A5 ×1, gm_1601 ×6,
  particle ×8 (… see wave-7 report), mplib ×3, camera 14 restored-to-100 vs
  controlled HEAD gate + 26 improved; lbcollision source half of const-fold
  conversion landed byte-proven (needs 15 symbols.txt renames → +10 fns,
  simulated). Naming round 4 committed 46e9da45b (branch campaign): 693
  lines, ~931 rows, ~202 fns to fuzzy-100, 64 units, DOL OK; mpcoll 165→9
  rows/60 fns. NEW idioms 22–33 added above. 6 binary-proven behavior fixes
  toward DOL (GXColor yellow, memset 0x24, 1-indexed handicap tbl, mplib
  line_id update, camera *1.0f no-op + C010 linear factor). Header-edit
  queue: gm_18A5 ×3, ground.h Ground_801C5774 s16→s32. INCIDENTS: whole-tree
  `git stash` race clobbered concurrent agents (stash@{0} 16:10:43 preserved
  — verify before drop); run_unit.sh false-negative reverts good renames
  (use campaign/scratch/run_unit2.sh, untracked); report.json confirmed
  stale/lenient (no reloc-name penalty) — gate only with controlled
  scratch compiles. RULE: never whole-tree git stash during a wave.
- **2026-06-06 — Wave 6 (unit campaigns).** 8 agents, 1.87M tokens, 81min.
  16 fn wins + 2 data symbols: gm_1832 (8, BSS repack + include removal +
  literal-pool archaeology), gmregclear (5, proven >=4 merged original TUs,
  bss 0->98.2), ftCo_800AB224, grBigBlue_801EACE8, ftCo_800D0CBC (sibling-
  safe retry with anti-inline ballast). Naming round 3: 718 renames, ~1963
  rows, 234 fns to fuzzy-100. THPDec header fix landed (Y/U/V + ifstatus
  uninit-read + --no-warn-error reconfigure). Two binary-proven game bugs
  fixed (resetText 15.0, grbigblue blob offset). NOTE: commit 61b1a52a3
  includes symbols.txt + ftchangeparam.c (land.py stash-pop staged them).
  Totals: 18761/19829 fns (94.61%), code 71.43%, data 41.71%, backlog 1068.
  NEW IDIOM CLASS: anchor-by-address-taken (BSS section-fold vs named
  anchor); extern-float caching trap; @ID realignment via goto early-outs.
  Orchestrator src-fix queue: camera.static.h(53 rows), ftcoll.c(13),
  lbcollision consts (needs dedicated sibling-gated agent).
- **2026-06-06 — Wave 5 + followups.** 14 agents, 1.38M tokens, 62min.
  Landed: 6 src wins (gronett, gm_1BA8, ft_0892, pltrick, itsamusmissile,
  gmresult) + THP Huffman Y/U/V via THPComponent header fix (hard-tail
  cluster opened) + naming round 2 (~1429 rows, 663 lines, 14 units) +
  ifstatus uninit-read fix with --no-warn-error reconfigure (upstream-
  anticipated). ftCo_800D0CBC win REVERTED by sibling gate (-27.9 on
  inlining sibling) -> retry queued. Binary-deviation fix: grBb -FLT_MAX.
  Total: 36 fns, 18750/19829 (94.56%). Switch idioms discovered.
- **2026-06-06 — Wave 4.** 16 agents, 1.88M tokens, 51min. 13 wins landed:
  6 camera.c fns (inline s64 bit-test helpers), fn_800F53AC (KIRBY'S FIRST
  — extern sizing + over-decompiled vararg), ftCo_800978D4, ft_800852B0,
  fn_801A94BC, if_802F7C30, un_80301964, it_80294364. Naming stream round 1:
  833 symbol lines renamed, 8 units. 4 new idioms. mplib .data emission-order
  finding → data-layout stream candidate. Total: 23 fns, 18737/19829 (94.49%).
- **2026-06-06 — Wave 3 (fruitcake fleet).** 19 agents, 2.46M tokens, 71min.
  7 wins landed: mn_8022FB88, itLinkbomb_UnkMotion3_Anim, grYorster_80202428,
  gm_8016EDDC (inline-helper), fn_8016FFD4 (cast-vs-mask), it_802A850C +
  it_802C53F0 (arg-precolor, orchestrator-applied header edits). Plus
  symbols.txt naming batch (+104 data bytes) and particle.c static fix.
  Both experiments SOLVED (BSS rule, naming mechanism). cobj re-diagnosed
  and parked. Running total: 9 functions matched, 18723/19829 (94.42%).
- **2026-06-06 — Session 2 (partial).** First match: fn_803ACD58 → 100%
  (declaration-order rule discovered + validated). check_fn.py converted to
  scratch-compile mode after the canonical-path poisoning bug bit us. Full
  clean rebuild performed. HARNESS CONSTRAINT: subagents (Workflow AND Agent
  tool) cannot run the session model — the runner sends thinking.type.disabled
  which adaptive-thinking models reject; MAX_THINKING_TOKENS=10000 added to
  ~/.claude/settings.json as a restart-time fix hypothesis (untested). Wave-1
  fan-out (sonnet/opus) was stopped mid-flight per user; all agent edits
  reverted. Wave 2 to re-launch post-rebuild.
- **2026-06-06 — Phase 0.** Build verified on macOS (wine-stable 11; note:
  README's wine-crossover cask is dead upstream). Triage v1 → audited by 3
  agents (caught aligned-row-count bug) → v2.1 validated against 12
  hand-verified functions. 4 strategy reports + infra loop + critic pass
  (9 agents, ~656k tokens). check_fn.py delivered. Backlog committed.
  Baseline: 97.43% fuzzy / 70.80% code bytes / 836/1031 units linked.
