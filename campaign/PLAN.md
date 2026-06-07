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

## ⭐ TU-SPLIT VERDICT (wave 9 experiment — PROVEN, ready to land)

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
with uniformly shifted d-offsets. **Split queue**: gm_1884 (ready),
gmregclear (3 cuts: TU bases lbl_804706C0/lbl_80472E48/lbl_80472ED8),
gm_1601 (≥2 cuts, +0x110 family), particle (CE3F8 AND D08E8 both demand
section offset 0), gm_18A5 (triad blueprint: 771C4/799B8; obsoletes the
wave-9 extern-flip package). DOL-safe for ALL NonMatching units. Caveat:
land splits.txt + src halves together — splits-only temporarily drops the
tail functions from report.json.

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
   after ANY local addition.
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
- **gm_1601 +0x110 anchor-fold family** (gm_8016A164/A22C/A4C8/95BC/9A84/
  AC44): unit merges ≥3 original TUs; constant-offset fns fold to merged
  .bss base. UNPARK PATH: TU-split verdict section (≥2 cuts queued).
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
  enumeration agent before retry.
- **ftCo_0A01 residuals**: B2790 webA/line_id r28↔r29 tie; B1478 inline-data
  r28/r29 (derived LIFO free-stack model — ours pops most-recently-freed,
  target implies different death order, lever unknown); AC5A0 dead 8B
  between y and f2i temps; A9904/A9CB4 dead-below-y + `fadds f1,f1,f31`
  canonical operand order (both source orders emit f31-first); A61D8
  loop-group-vs-invariant coloring; AE7AC r4-vs-r7 pick + inverted mr/addi
  param-homing pair.
- **gm_1832 residuals**: fn_801851C0 r30/r31 LICM-temp swap (8 forms);
  fn_80188550 + fn_80188644 TU-SPLIT-BLOCKED → UNPARK PATH proven (wave-9
  exp: 99.18/99.34 in split prototype; see verdict section); fn_80189B88
  tail zero-share (store-zero and return-zero same vreg, 4 forms
  const-propped); fn_80187CF4 st/jobj/gobj cyclic perm (identical source
  shape to matched fn_80187AB4 allocates differently).
- **lbaudio_ax unsolved**: bss-vs-data anchor callee-saved rank swap
  (26C04/27168/2838C); loop-counter-first volatile rotation (decl/def/init
  all no-ops); ud/sp param-copy homing swap (25FAC, 5 levers); 26EBC u64
  inline-result r3/r4 coalesce (expert inline-copy class).
- **it_8026F3D4 (95.56)**: preheader lfs-after-volatile-stw — scheduler
  canonicalizes every form to lfs-pairs-with-lis (~10 forms; idiom-15
  singleton). **fn_802FF218 (94.33)**: y↔thing callee-saved swap + r6
  arg-copy site, regalloc-resistant pair.
- **gm_18A5 wave-9 parks**: fn_8018E46C lwz/neg pair-order attractor (16
  rows, 6 levers — anonymous-temp attractor family; arg-reshuffles drift
  @991→@992); fn_801977AC single lwz/mr schedule row (6 levers; matched
  sibling fn_80197AF0 proves canonical source emits mr-first); lbzu-fusion
  vs add-operand-order are MUTUALLY EXCLUSIVE from C (fn_801913BC, 5
  forms: array-index fuses but orders (idx,base); ptr-arith orders
  (base,idx) but unfuses); fn_80191240 jobj2-copy reconstruction (cast-copy
  coalesces, plain copy over-splits; 98.09 stands).
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
- **ftCo_0A01 wave-9 parks**: AF290 residual (99.38) = pure 5-web rank
  rotation, target {r26,r27,r28,r30,r31} vs ours packed (B1478 LIFO-death
  family; decl/init probes no-ops) + tail instance-band internals (pads
  can't fill between instance items); AC5A0 +3 failed forms (depth-2 conv
  helper moves y not the gap) — keep parked; A3908/A4038 DIAGNOSED
  AF290-class (target splits &fp->x1A88 web into two addis, uses r20,
  frame remap 0x8c-vs-0x60) — needs a dedicated session WITH an @-id
  budget plan.
- **mplib wave-9 parks**: NextNon×8 / Get×8 / 534FC×4 / mpLinesConnected /
  800581DC / 80058614 / DrawMatchingLines-rotation / 8004ED5C — one
  volatile-web/inline-result rotation class (6+6 permutation sweeps,
  non-monotonic); mpCheckFloor (55 rows = single phantom-band shift);
  80055E9C loop-invariant FPR pair tie; 56C54 sqrtf instance-band;
  RespawnVtxIds 80% + 804D8150 66.7% are dtk pad-fold size rows —
  diff-policy, unreachable from C (like gap_07/gap_09 rows).
- **particle wave-9 parks**: TU-SPLIT-BLOCKED residual class (CE3F8 AND
  D08E8 both demand section offset 0 — see verdict section); @3751 dup
  0.5f .sdata2 per-original-TU copies (conversion-magic class); .data is
  ~0x2BA4 bytes short of target (dead string blobs of the original TUs) —
  queued for a dedicated reconstruction agent (idiom-24/51 style, with
  @id-shim accounting).

## Session log

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
