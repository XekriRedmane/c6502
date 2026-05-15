# c6502 knowledge base

A running log of decisions, bug fixes, patterns, and general rules
extracted from coding sessions. Append, don't rewrite — each entry
should carry the date and a one-line headline so future sessions
can skim.

---

## 2026-05-15 — DSE: LoadAddress is precisely modeled, not opaque

### What happened

`examples/floor_enemy_advance.asm` (compiled with `--optimize`)
contained 20 `STA DPTR` writes, ~12 of them dead. The asm-level
SSA round-trip's `apply_indirect_base_prop` had successfully
rewritten the indirect-Y operands to `IndirectZp(<b0>)` (so the
`STA (b0),Y` accesses bypassed DPTR), but `apply_asm_dead_store`
left the dead `STA DPTR` / `STA DPTR+1` pairs in place.

Root cause: `passes/asm_dead_store.py`'s `_OPAQUE_TYPES` included
`asm_ast.LoadAddress`. The DSE walk treats opaque instructions as
"may read any memory" → returns LIVE for any target whose forward
path crosses one. In this file, every spawn-side and step-side
store sequence ends with another `LoadAddress(Data(enemy_*))`
overwriting `b0`, so the dead `STA DPTR` walk hit a `LoadAddress`
before its `STA DPTR` kill and bailed.

### Fix

`passes/asm_dead_store.py`:

- Removed `LoadAddress` from `_OPAQUE_TYPES`.
- Added a `LoadAddress` case to `_read_operands`: yields `Data("FP",
  0)` and `Data("FP", 1)` when src is `Frame` (the asm-emit lowering
  is `CLC; LDA FP; ADC #off; STA dst.lo; LDA FP+1; ADC #0; STA
  dst.hi`); yields nothing for `Data` src (lowers to `LDA #<name`
  immediates, no memory read).
- Added a `LoadAddress` case to `_write_operand`: returns `dst` so
  a same-byte upstream STA is recognized as killed.

Result: `examples/floor_enemy_advance.asm` went 289 → 266 lines
(~8% reduction); 8 dead DPTR stage pairs eliminated.

Tests: `tests/test_asm_dead_store.py` pins four behaviors —
DPTR-stage cleanup with intervening `LoadAddress`, FP being a live
read for `LoadAddress(Frame, _)`, no spurious FP read for
`LoadAddress(Data, _)`, and `LoadAddress` killing a same-byte
upstream write.

### Failed attempt — DON'T do this

Tried to also treat `Call` as a kill of `Data("DPTR", _)` on the
theory that "DPTR is caller-saved scratch." Surfaced 44 test
failures in the first cut (too broad: also covered callee-saved
ZP `$C0..$FF` where regalloc deliberately puts cross-call live
values). Narrowed to DPTR only → 2 failures, because the `icall`
trampoline IS the JSR target for indirect calls and `icall: JMP
(DPTR)` reads both bytes of DPTR. Reverted entirely.

### General rules extracted

1. **"Caller-saved" ≠ "dead-at-call."** Caller-saved means the
   callee may CLOBBER. It says nothing about whether the callee
   READS. Named callees (like `icall`) can and do read caller-saved
   cells. Don't optimize on the assumption that a Call kills DPTR
   without proving the specific callee doesn't read it.

2. **Pool partition matters.** `$80..$BF` is caller-saved (regalloc
   uses it for non-cross-call values); `$C0..$FF` is callee-saved
   (regalloc uses it for cross-call live values, with prologue
   save/restore). The private-pool allocator can shift addresses
   into either half, so DSE has no easy way to tell from an
   address alone whether a cell is dead-at-call. When in doubt,
   use the conservative `Call`-is-opaque treatment.

3. **Prefer precise modeling over opaque marking.** Whenever a
   compound instruction has bounded memory effects (read set +
   write set both enumerable from the operand shape), model them
   precisely in liveness/DSE rather than marking opaque. Opaque
   marking compounds across passes — one over-conservative opaque
   blocks a chain of downstream optimizations.

4. **Verify with the full suite for liveness-related changes.**
   Changes to `_OPAQUE_TYPES`, `_read_operands`, `_write_operand`,
   `_is_dead_at_exit`, or aliasing predicates touch dozens of
   downstream passes. Always run the full `uv run python -m
   unittest` before declaring success.

---

## 2026-05-15 — Differential opt-vs-unopt sim test pattern

### Pattern

For a function whose optimizer correctness needs verifying:

1. Inline the source into the test file (don't read from disk —
   self-contained tests bisect cleaner).
2. Provide stubs for any `extern` / zp_abi callees (counter +
   last-args globals make assertions easy).
3. Write a `main` that runs a battery of scenarios. After each
   scenario, copy the relevant post-state into a flat byte array
   (`result_log[N]`) at a sequential offset (`log_idx`).
4. Test methods:
   - `test_unoptimized_matches_expected` — compile with
     `optimize=False`, assert `result_log` bytes match a hand-
     derived expected layout.
   - `test_optimized_matches_expected` — same, `optimize=True`.
   - `test_opt_and_unopt_agree` — both pipelines must produce
     byte-identical `result_log` AND byte-identical HARGS+0..1
     return windows. This catches optimizer miscompiles even if
     both the expected layout and the opt path agree on the wrong
     answer.

### Helpers

- `sim.harness.build_sim(source, optimize=...)` builds a
  `Simulation`; call `.run(max_cycles=N)` for a `SimResult` with
  memory snapshot.
- `result.return_int() & 0xFFFF` reads the 2-byte int return from
  `HARGS+0..1`.
- `sim.symbols["<name>"]` resolves any user static/function name
  to its concrete address.
- `bytes(result.memory[addr:addr+N])` snapshots an arbitrary
  region for assertion.

### Parser gotcha

`result_log[16 * 8]` doesn't parse — array sizes must be integer
constant LITERALS, not constant expressions. Precompute and use
the literal (`result_log[128]`).

---

## Investigation workflow rules

These crystallized while debugging the DSE issue. Apply when
investigating "why doesn't pass X fire?":

1. **Construct a minimal inline repro first.** Build the smallest
   IR fragment that should trigger the pass, run the pass in
   isolation, observe behavior. If the minimal case passes but the
   real one doesn't, the difference IS the bug surface.

2. **Drive the full pipeline and dump IR at intermediate stages
   with index numbers.** When the minimal repro works but the full
   compile doesn't, run the pipeline by hand through `sim.harness.
   compile_to_asm` (or its constituent passes) and print each
   stage's IR with positional indices. The index gives you a
   coordinate for `_is_dead_cfg`-style queries.

3. **Call the predicate directly.** When a pass's high-level
   behavior is unclear, call its internal predicates (e.g. `_is_
   dead_cfg(instrs, idx, ...)`) directly with constructed inputs.
   That isolates "the predicate is wrong" from "the predicate's
   inputs are surprising."

4. **Trust the CFG walk's verdict, then ask why.** If `_is_dead_
   cfg` says LIVE, walk the CFG by hand from the instruction
   index. Identify which successor edge first returns LIVE — that
   instruction is the immediate cause.

---

## Project-wide rules (cumulative)

### Compilation

- Default flags for this project: `--optimize --unroll`. See user-
  memory `feedback_compile_optimize_default.md`.

### Editing CLAUDE.md

- It's loaded into every Claude Code session's context, so size
  matters. ~2300 lines is tolerable for a compiler with this much
  architecture; bloat it carefully.
- The "Pipeline at a glance" section at top (post-2026-05-15
  refresh) is the entry point for new readers. Keep it accurate
  if you change the pass order.
- The "Peephole catalog" enumerates all 19 peepholes in
  `_peephole_fixedpoint`. When adding a new peephole, add a one-
  line entry there; don't write a new top-level `## Section` for
  it unless the pass has unusual subtleties.
- The "Status" section is intentionally short: gaps and known
  imprecisions only. Per-feature feature-completeness lives in
  README.md's Status section and `tests/STATUS.md`.

### Asm IR + pass invariants

- `LoadAddress` is a compound IR node (expanded by
  `replace_pseudoregisters` for Pseudo dst, by `asm_to_asm2`
  for the final atomic form). Its memory effect is bounded:
  Data src reads nothing (immediates), Frame src reads FP/FP+1,
  always writes 2 bytes at dst.
- `Call` is opaque to DSE for all targets. Don't change this
  without analyzing every named callee's read set —
  `icall` reads DPTR, helper calls read HARGS, etc.
- `IndirectZp` / `IndirectZpY` operands don't alias `Data(<DPTR
  | SSP | FP | HARGS>)` runtime symbols (see `passes/asm_
  aliasing.py:_RUNTIME_ZP_NAMES`). User pointers don't point
  into runtime infrastructure by c6502 convention.

### Test file conventions

- New pass tests go to `tests/test_<pass_name>.py`. When the
  pass is asm-level, prefix with `test_asm_` if the pass module
  already does (e.g. `test_asm_dead_store.py`, `test_asm_byte_
  dce.py`).
- Self-contained sim tests use `sim.harness.build_sim(source,
  optimize=...)` with inline C source + stubs.
- Differential opt-vs-unopt tests go in their own file when
  they exercise one specific function; `tests/test_sim_
  differential.py` is the broad chapter-corpus sweep.

