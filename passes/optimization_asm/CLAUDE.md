# passes/optimization_asm/CLAUDE.md

Asm-level SSA round-trip with byte-granular regalloc. Active under
`--optimize`. Sits between `tac_to_asm` (which produces `asm_ast`) and
`replace_pseudoregisters_bare_exit` (which resolves Pseudos to
concrete operands). The root pipeline overview lives in
`/project/c6502/CLAUDE.md`; the TAC-level optimizer that runs earlier
lives in [../optimization/CLAUDE.md](../optimization/CLAUDE.md).

## Module roster

- `optimizer.py` — driver (`optimize_program`, `optimize_function`).
  Orchestrates the prepasses, then per-function SSA construction,
  coalescing, fixed-point copy-prop / DCE bracket, regalloc, and SSA
  destruction.
- `const_static_fold.py` — `fold_const_statics`. Program-level prepass
  (see "Const-static fold" below).
- `dead_static.py` — drops any internal-linkage `StaticVariable` top-
  level nothing now references.
- `ssa_construction.py` — `to_ssa`. Byte-versions every promotable
  `(name, offset)` pair.
- `ssa_destruction.py` — `from_ssa`. Emits per-edge Movs with
  parallel-copy ordering.
- `hwreg_eligibility.py` — marks Pseudos that can live in X/Y across
  their intra-block live range (saves the `LDX / LDY` setup for
  absolute,X / (zp),Y accesses where the index is the pinned Pseudo).
  Eligibility is per-HwReg (separate `eligible_x` and `eligible_y`
  sets) — `Mov(IndexedData(...,X), P)` makes P Y-eligible only (via
  `LDY abs,X`, since `LDX abs,X` doesn't exist), and vice versa for
  `IndexedData(...,Y)`.
- `coalescing.py` — `coalesce_moves`. Aggressive (Chaitin-style) move
  coalescing. See "Move coalescing" below.
- `copy_propagation.py` — forward copy propagation on the asm-SSA
  form.
- `backward_copy_propagation.py` — backward copy propagation; the
  pass explicitly defers Pseudo-to-Pseudo coalescing to regalloc.
- `byte_dce.py` — byte-granular dead-code elimination.
- `regalloc.py` — byte-granular register allocation. Colors 1-byte
  SSA names to ZP from `Pool(start=0x80)` (default split: caller-
  saved `[0x80, 0xC0)`, callee-saved `[0xC0, 0x100)`); multi-byte
  names get contiguous width-N blocks.
- `apply_coloring.py` — applies the coloring map produced by
  `regalloc` to every operand in the function.
- `cfg.py` — asm-level control-flow graph (basic blocks, dominators,
  natural loops).
- `interference.py` — `build_interference`. Asm-level interference
  graph; runs before coalescing.
- `liveness.py` — byte-granular liveness used by interference / DCE.

## Per-function pipeline shape

```
fn → fold_const_statics (program-level prepass)
   → dead_static       (program-level prepass)
   → to_ssa
   → hwreg_eligibility
   → coalesce_moves
   → [copy_propagate → backward_copy_propagate → byte_dce]*
   → build_interference
   → color_graph (byte-granular regalloc)
   → from_ssa
   → fn'
```

Cross-call values prefer callee-saved; non-cross-call values caller-
saved. HwReg-eligible Pseudos are assigned `Reg(X)` / `Reg(Y)`
instead of a ZP byte. When the per-function private pool (see "Call-
graph-disjoint ZP allocation" in [../CLAUDE.md](../CLAUDE.md)) is in
effect for an eligible function, the regalloc draws colors
exclusively from that pool — the caller/callee partition collapses.

## Move coalescing (`coalescing.py`)

Asm-level SSA-era pass that merges move-related Pseudo pairs in the
interference graph when they don't interfere. Runs between
`build_interference` and `color_graph`. The point: ensure the two
ends of every `Mov(Pseudo a, Pseudo b)` and every
`(Phi.dst, PhiArg.source)` pair get the SAME ZP color whenever that's
safe. After SSA destruction the corresponding Mov becomes
`Mov(ZP($X), ZP($X))` — a self-Mov that asm_emit's self-Mov peephole
drops, eliminating the temp-routing round trip.

Move-related pairs come from two sources:

- `Mov(Pseudo a, Pseudo b)` — explicit Pseudo-to-Pseudo copy in the
  asm IR.
- `(Phi.dst, PhiArg.source)` — SSA destruction would emit a
  `Mov(source, dst)` for this pair at the predecessor block's tail.

Eligibility filters:

- Both names must be in the interference graph (statics, address-
  taken, params are excluded upstream).
- Same width (the coloring pool's slot search is width-aware;
  coalescing different widths would force one node into the other's
  slot layout).
- No interference edge between them (coalescing two interfering nodes
  would force them to share a color, which can't be correct).
- Both Pseudos have `offset == 0` (asm-SSA-renamed names; a non-zero
  offset marks an unrenamed multi-byte name needing contiguous bytes,
  not the same byte).

Algorithm: aggressive (Chaitin-style) — for each candidate pair in
instruction order, look up the union-find class representatives,
check eligibility, and merge by absorbing one node's edges and
`lives_across_call` flag into the other. The spill check is implicit
via the existing `Coloring.spilled` fallback: if a coalesced node
ends up with too-high degree to fit in any pool, spilling kicks in.
With the default 128-byte ZP pool this hasn't been observed in
practice on c6502 programs.

The `CoalesceResult.representative` map projects every coalesced non-
rep name to its rep. The optimizer driver expands the post-coloring
assignments through this map so `apply_coloring` sees every original
SSA name mapped to its merged color.

The headline win: a loop-counter `for (uint8_t i = 0; i < N; i++) ...`
previously routed the increment through a temp because asm-SSA Phi
destruction emitted a `Mov(.v_post_inc, .v_phi)` with the two SSA
names colored to different ZP slots. With coalescing, .v_phi /
.v_init / .v_post_inc all share one slot; the inserted Mov is a self-
Mov dropped at emit; and the in-place ADC chain that remains is
collapsed by the multi-byte INC peephole. End result: `i++` becomes a
single `INC $XX` (uchar) or `INC $XX; BNE done; INC $YY; done:`
(int).

## Const-static fold (`const_static_fold.py`)

Program-level prepass that runs first inside `optimize_program`. A
`static T const x = <const-init>` (file-scope, internal linkage,
const-qualified, single foldable scalar init) whose address is never
taken in the program is genuinely immutable in c6502's single-TU
model: `static` keeps the symbol invisible at link time, `const`
rejects writes to it, and "no address taken" means no runtime path
observes the storage location. Every reference to its bytes can
therefore be replaced with the corresponding immediate at compile
time, and the storage cells freed.

A `StaticVariable` top-level becomes a candidate when:

- `is_global` is False (internal linkage — `static` at file scope or
  any block-scope `static`),
- the symbol-table type carries an outermost `Const(...)` wrapper
  (not recursed — `Pointer(Const(Int))` is `const int *` pointee, not
  a `int * const` pointer; that wouldn't be us),
- `init` is a single CharInit / IntInit / LongInit / LongLongInit /
  FloatInit / DoubleInit (one foldable scalar — arrays, AddressInit,
  StringInit, ZeroInit are skipped).

A candidate is then disqualified if it appears as:

- `LoadAddress.src` — `&candidate` somewhere,
- the dst of any write atom (Mov / Add / Sub / And / Or / Xor / Inc /
  Dec / ASL / LSR / ROL / ROR / Pop) — defensive; the type checker
  rejects writes to a const lvalue, but we don't silently fold past
  one if it slipped through,
- an `IndexedData(name=candidate, ...)` operand (only relevant for
  static arrays in practice — defensive),
- an `AddressInit(name=candidate, ...)` in another static's
  initializer.

For surviving candidates: every `Pseudo(name=cand, offset=k)` USE in
every function is rewritten to `Imm(byte_at(init, k))`, and the
candidate's `StaticVariable` top-level is dropped. The asm-level
`Mov(Imm, Pseudo)` shapes the rewrite leaves behind get picked up by
the existing forward-copy-prop / DCE bracket — the fold is a setup
for downstream cleanup, not a standalone pass.

The canonical case is a memory-mapped device pointer:
`static uint8_t * const hires_page1 = (uint8_t * const)0x2000;` —
every `LDA hires_page1` (3 bytes) collapses to `LDA #$00` (2 bytes),
every `LDA hires_page1+1` to `LDA #$20`, and the 2-byte storage of
`hires_page1` itself disappears from the output. External-linkage
globals (without `static`) are skipped even when const, because
another TU might read the symbol.
