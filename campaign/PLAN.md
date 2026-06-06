# Melee Decomp Completion Campaign

Fork-based experiment to drive doldecomp/melee from ~97.4% fuzzy / 64% matched
bytes toward 100%. This file is the persistent campaign state — update it at
the end of every session.

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
  ⚠ particle BSS: target order is CEB40 *before* CF740; our build inverts
  declaration order (phase-0 layout strategy stated the fix backwards).
- **S3 structural near-miss** (419): root causes per phase-0 analysis:
  anon-bss cascade ~137 (depends on S5), small 1-fix ~97, type
  sign/zero-extension (extsb/extsh/clrlwi from wrong types), inline-vs-call.
- **S4 codegen** (197): mnemonic-level fixes; includes balanced-reorder
  scheduling cases. MIN-macro operand flips, mr-vs-addi, fmr-chain ordering.
- **S5 config/naming** (cuts across ~150+ fns): name anonymous `@NNN` sdata2
  literals and `...bss.N` symbols in `config/GALE01/symbols.txt`. Fork-legal.
  OPEN QUESTION (answer before scaling): are naming-only functions already
  byte-correct in a linked Matching unit? If yes, S5 is bookkeeping that
  unlocks fuzzy-100 gates; if no, it changes layout. Test on one unit first.
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
2. **Naming-only byte-correctness**: pick a unit whose only remaining diffs
   are naming rows; flip to Matching in configure.py, rebuild, see if the
   DOL check still passes. Settles the S5 question and the real termination
   criterion.
3. Re-validate one S1 win end-to-end (edit → 100% → unit clean → DOL OK →
   commit) to prove the full loop before fanning out.

## Session log

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
