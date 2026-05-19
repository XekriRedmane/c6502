# passes/optimization/CLAUDE.md

TAC-level optimizer. Active under `--optimize`. Runs after
`c99_to_tac` and before `tac_to_asm`. The root pipeline overview lives
in `/project/c6502/CLAUDE.md`; the asm-level SSA round-trip that runs
later lives in
[../optimization_asm/CLAUDE.md](../optimization_asm/CLAUDE.md);
peephole catalog lives in [../CLAUDE.md](../CLAUDE.md).

## Module roster

- `cfg.py` — control-flow graph helpers (basic-block construction,
  dominator tree, natural-loops detection).
- `interference.py` — TAC-level interference graph builder (used by
  register allocation).
- `pool.py` — ZP byte pool abstraction (caller- vs callee-saved
  partitioning, `allowed_range`).
- `register_allocation.py` — graph-coloring regalloc used by the asm-
  level optimizer; here for shared infrastructure with the TAC layer.
- `ssa_construction.py` — `to_ssa`. SSA construction (Cytron 1991).
- `ssa_destruction.py` — `from_ssa`. Parallel-copy ordering by
  topological sort fixes the "lost copy" problem; cycles break with a
  fresh `<funcname>.cycle_tmp@N`.
- `var_visit.py` — utility for visiting / rewriting TAC operands.
- `optimizer.py` — driver (`optimize_function`, `optimize_tac`).
- `loop_rotate.py` — `rotate_signed_countdown_loops`. Pre-SSA one-
  shot.
- `static_const_fold.py` — `fold_static_const_reads`. One-shot post-
  SSA.
- `constant_folding.py` — `constant_fold` (Unary / Binary / comparison
  / cast / conditional-jump-with-constant-cond / Phis with agreeing
  args). Width-correct wraparound at the operand's declared width.
  Const-array-subscript fold (`_fold_indexed_load`) is part of this
  pass.
- `strength_reduction.py` — `reduce_strength`. `Multiply(x, 2^k)` →
  `LeftShift`, unsigned `Divide(x, 2^k)` → `RightShift`, unsigned
  `Modulo(x, 2^k)` → `BitwiseAnd`. Signed Divide / Modulo skipped
  (C99 truncation differs from arithmetic shift).
- `cmp_zero_jump_fold.py` — `fold_cmp_zero_jump`. Fuses
  `Binary(cmp, ...); JumpIf*` to direct conditional jumps. `== 0` /
  `!= 0` traces through ZeroExtend; ordering ops emit
  `JumpIfCmp(op, src1, src2)` for the per-byte compare-chain lowering.
  Operand narrowing through ZeroExtend folds `(uint8_t)i < 105` to
  3-instr `LDA / CMP / BCS`.
- `and_zero_jump_fold.py` — `fold_narrow_and_jump`. Folds
  `(ZeroExtend(uchar); BitwiseAnd(_, 0x80); JumpIf*)` to
  `JumpIfMasked` when the operand can be narrowed to 1 byte —
  produces the direct `LDA / BPL/BMI` pattern at asm lowering instead
  of an 8-bit AND + 16-bit Z.
- `dead_loop_elimination.py` — `eliminate_dead_loops`. Detects natural
  loops (back-edges via `cfg.natural_loops()`) whose body has no
  `Call` / `Store` / `Ret` and whose every SSA def is loop-local. When
  the gates pass, rewrites the header to `[Label, Jump(exit)]` so UCE
  prunes the now-unreachable body on the next sweep. Single-exit-edge
  gate keeps Phi-arg retagging at the exit unambiguous. Collapses
  nested empty-loop shapes like `do { while(--y); } while(--d);` to
  nothing. NOTE: lacking volatile semantics (the parser silently drops
  the qualifier), this pass will currently delete loops the programmer
  marked volatile; preserving them requires plumbing volatile through
  the type system.
- `unreachable_code_elimination.py` — forward DFS from ENTRY; prunes
  dead Phi args; folds singleton Phis to Copies; drops useless jumps /
  labels.
- `copy_propagation.py` — `copy_propagate`. SSA-aware: replaces every
  use of a Copy's dst with its src; chains too.
- `dead_store_elimination.py` — SSA-aware: drops pure defs with no
  reads. Calls keep the call (side effects) but drop unused dst.
- `copy_folding.py` — `fold_copies`. See "Copy folding" below.
- `reassoc_const.py` — `reassoc_constants`. See "Add-with-Constant
  reassociation" below.
- `recognize_indexed_store.py` / `recognize_indexed_load.py` —
  collapse `ZeroExtend(uchar) + Binary(Add, C) + Store/Load` chains to
  `IndexedStore` / `IndexedLoad` (absolute,X addressing). See
  "IndexedStore recognizer" below.
- `recognize_indirect_indexed.py` — post-fixedpoint one-shot.
  Collapses `ZeroExtend(uchar) + Binary(Add, ptr) + Load` to
  `IndirectIndexed` for the `(zp),Y` lowering. Deliberately runs LAST
  so any pointer that's going to fold to a Constant (via the const-
  static fold path) has already done so — otherwise this pass would
  prematurely lock in (zp),Y for a chain that would have qualified for
  the cheaper absolute,X form.
- `short_circuit_jump_fold.py` — `fold_short_circuit_jump`. Post-
  destruction one-shot (iterated to a fixed point). Rewrites the
  canonical `&&` / `||` 0-or-1-materialize tail + adjacent
  `JumpIf{True,False}` consumer into direct conditional branches:
  the chain's short-circuit jumps retarget to the consumer's
  destination, the 5-instruction tail and 1-instruction consumer
  are deleted, and the "flipped" sub-case (consumer's branch
  direction routes the short-circuit value to fall-through) emits
  `Jump(T); Label(.<fn>@scfold@<N>)` to materialize the fall-
  through. Covers all four `(C_ft, C_sc) × consumer-kind`
  combinations and nested short-circuits (`(a && b) || c` style)
  via transitive closure on the retarget map.
- `sink_increment.py` — `sink_increments`. Moves `Y = X + c` past the
  last in-line use of `X` when `Y`'s only consumer follows, exposing
  `recognize_indexed_*` patterns the original ordering hid.
- `dispatch_pointer_array.py` — `dispatch_const_pointer_arrays`. Runs
  at the program level after `optimize_tac` (post-from_ssa).
  Recognizes the `Binary(LeftShift|Multiply, i, 1|2) + IndexedLoad(arr,
  _, %ptr) + IndirectIndexedLoad(%ptr, j, %v)` chain when `arr` is a
  file-scope `static const T * const[N]` with N ≤ 8 and all-
  AddressInit elements; rewrites to a CMP/BEQ dispatch on `i` with
  per-case direct `IndexedLoad(target_k, j, %v)`. Eliminates DPTR
  staging and (zp),Y indirection at the cost of a small dispatch
  chain; frees X and Y from the dual-index conflict so loop counters
  can stay pinned to X across the dispatch.

## Per-function pipeline shape

```
fn → rotate_signed_countdown_loops (one-shot, pre-SSA)
   → to_ssa
   → fold_static_const_reads (one-shot)
   → [constant_fold → reduce_strength → fold_cmp_zero_jump
      → fold_narrow_and_jump → eliminate_dead_loops → UCE
      → copy_propagate → DSE → fold_copies → reassoc_constants
      → recognize_indexed_store → recognize_indexed_load
      → sink_increments]*
   → recognize_indirect_indexed (post-fixedpoint, one-shot)
   → from_ssa
   → fold_copies                                  (post-from_ssa)
   → fold_short_circuit_jump*                     (post-from_ssa, fixedpoint)
   → fn'
```

`docs/optimization.md` is the from-scratch tour. Brief notes per
stage:

1. **`rotate_signed_countdown_loops`** (`loop_rotate.py`). Pre-SSA
   one-shot. Recognizes the canonical `c99_to_tac` for-loop shape
   where the test is at the top (`for (i = N; i >= 0; i--)`) and
   rotates to test-at-bottom (`do { ... } while (i >= 0);`), saving
   one unconditional jump per iteration. Operates pre-SSA because the
   rewrite is structural — Phis don't exist yet, so the rewrite needs
   only to fix up the def/use chain on the loop's single counter var,
   not parallel-copy a Phi web. `to_ssa` rebuilds Phis after.

2. **`to_ssa`** (`ssa_construction.py`). Renames promotable Vars
   (LocalAttr + scalar + non-address-taken) to `<orig>.<N>`; inserts
   pruned Phi nodes at iterated dominance frontiers (Cytron 1991).
   Address-taken locals, statics, aggregates pass through unchanged.
   SSA-minted labels are scoped per-function
   (`.<funcname>@ssa_block@N`).

3. **`fold_static_const_reads`** (`static_const_fold.py`). One-shot
   post-SSA. Replaces every USE-position `Var(name)` with
   `Constant(value)` when `name` is `static const` scalar with an
   `Initial(c)` initializer and a const-qualified type. See "Static-
   const reads + array-subscript folding" below.

4. **Fixed-point loop**. Twelve passes rotated to convergence (see
   the module roster above for the per-pass entry points).

5. **`recognize_indirect_indexed`** (`recognize_indirect_indexed.py`).
   Post-fixedpoint one-shot.

5a. **`dispatch_const_pointer_arrays`** (`dispatch_pointer_array.py`).
    Program-level post-from_ssa.

6. **`from_ssa`** (`ssa_destruction.py`). One Copy per PhiArg in the
   matching predecessor, before the terminator. Parallel-copy
   ordering by topological sort fixes the "lost copy" problem; cycles
   break with a fresh `<funcname>.cycle_tmp@N`.

7. **Post-destruction folds**.
   - **`fold_copies`** (`copy_folding.py`) — fuses the Copy round
     trip emitted at predecessor block ends to feed Phi sources
     into Phi dsts (covered above).
   - **`fold_short_circuit_jump`** (`short_circuit_jump_fold.py`,
     iterated to fixed point) — recognizes the 5-instruction
     short-circuit tail (`Copy(C_ft, %t); Jump(end);
     Label(branch); Copy(C_sc, %t); Label(end)`) with adjacent
     `JumpIf{True,False}(%t, T)` consumer (single-use %t,
     single-use end_label, `{C_ft, C_sc} == {0, 1}`), retargets
     the chain's short-circuit jumps to where the consumer
     would route `%t == C_sc`, and deletes the
     tail+consumer. Covers `&&` / `||` × `JumpIfFalse` /
     `JumpIfTrue` (4 cases). The "natural" sub-case (consumer's
     branch direction matches `C_sc`) is a clean delete; the
     "flipped" sub-case mints a fresh
     `.<funcname>@scfold@<N>` label and inserts `Jump(T);
     Label(.scfold@N)` to materialize the fall-through path.
     Nested patterns (`(a && b) || c` and similar) resolve in a
     single sweep via transitive closure on the retarget map.
     Sound only post-destruction: pre-destruction the tail is
     split across two SSA-renamed `%t` defs merged by a Phi,
     which complicates the pattern match.

8. After post-destruction folds the function is non-SSA TAC,
   ready for `tac_to_asm` in bare-exit mode.

The asm-level SSA round-trip that follows `tac_to_asm` is documented
in [../optimization_asm/CLAUDE.md](../optimization_asm/CLAUDE.md).

## Static-const reads + array-subscript folding

Three composable TAC-level passes that turn const-static reads and
const-array subscripts with constant indices into compile-time
Constants, exposing them to the rest of the constant folder.

### `static_const_fold.py` — scalar reads

One-shot pass that runs once after `to_ssa`, before the fixed-point
loop. Walks every TAC instruction; replaces every USE-position
`Var(name)` with `Constant(value)` when `name`'s symbol-table entry
is:

- `StaticAttr(initial_value=Initial(c))` with `c` being `int` or
  `float` (NOT `AddressInit` — link-time symbol; NOT a tuple —
  aggregate);
- type carries an outermost `Const(...)` wrapper (gates the fold on
  the C type system having already promised the storage's value is
  fixed at runtime);
- underlying type is a foldable scalar (Char/SChar/UChar /
  Int/UInt / Long/ULong / LongLong/ULongLong / Float / Double /
  Pointer — not Array, Structure, Union).

The asm-level `fold_const_statics` already drops the `StaticVariable`
storage when nothing references it; this TAC-level pass eliminates
the runtime reads upstream so the constant flows into the rest of the
constant folder.

### Const-array-subscript fold (`_fold_indexed_load` in `constant_folding.py`)

`IndexedLoad(name, Constant(byte_idx), dst)` collapses to
`Copy(Constant(value), dst)` when:

- `name` is `StaticAttr(Initial(tuple_value))`,
- the array's element type is const-qualified
  (`Array(Const(elem_t), N)`),
- `byte_idx` is element-aligned,
- the indexed element value is `int` or `float` (not a nested tuple,
  not `AddressInit`),
- the dst's c99 width matches the element's width.

### Add-with-Constant reassociation (`reassoc_const.py`)

Recognizes `Binary(Add, C2, V, %inner); Binary(Add, C1, %inner,
%outer)` (or any commutative variant) where `%inner` is single-use
and the two Constants share a const variant, and rewrites to
`Binary(Add, (C1+C2), V, %outer)` (dropping the inner def). Wraps
modulo the variant's bit width.

The headline composition: in code like

```c
static uint8_t * const buf = (uint8_t * const)0x2000;
static const uint16_t offsets[N] = {0x100, 0x200, ...};
buf[offsets[2] + col] = value;
```

the static-const reads turn `buf` into `Constant(0x2000)`, the const-
array fold turns `offsets[2]` into `Constant(0x300)`, and
reassociation collapses `0x2000 + (0x300 + col)` to `0x2300 + col` —
one runtime 16-bit Add instead of two. Then the IndexedStore
recognizer (next section) folds the whole thing into a single
absolute,X store.

## IndexedStore recognizer (`recognize_indexed_store.py`)

A TAC pass that runs in the fixed-point loop. Detects the canonical
absolute,X-store pattern and rewrites it to the new
`IndexedStore(int address, val index, val src)` instruction.

Pattern (three adjacent instructions, with single-use temps):

```
ZeroExtend(uchar_var, %ext)
Binary(Add, Constant(C), %ext, %addr)   # or commutative
Store(val, %addr)
```

Eligibility:

- `%ext` and `%addr` are single-use Pseudos.
- `uchar_var`'s c99 type is 1 byte (Char / SChar / UChar).
- `val` is 1-byte typed (Var or Constant).
- `0 ≤ C ≤ 0xFF00` so `C + 255` fits in the 16-bit address space (the
  6502's absolute,X addressing wraps modulo 0x10000; capping the base
  prevents an unintended wrap into page zero).

The replacement `IndexedStore(C, uchar_var, val)` lowers in
`tac_to_asm` to:

```
LDA val           # Mov(val, A)
LDX uchar_var     # Mov(uchar_var, X) via A
STA $C,X          # Mov(A, IndexedData(name="", offset=C, index=X))
```

The asm IR's `IndexedData` operand has been extended: when its `name`
field is empty, the address is read directly from `offset` (rendered
as `$XXXX,X` instead of `name+offset,X`). The existing static-array
load path uses `name`-keyed `IndexedData`; the IndexedStore lowering
uses the empty-name variant for raw numeric bases.

The end-to-end composition (`static T * const` + const subscript +
reassoc + recognize) turns

```c
static uint8_t * const buf = (uint8_t * const)0x2000;
buf[100 + col] = value;
```

(where `col` is uchar) into a single

```
LDA value
LDX col
STA $2064,X
```

— 7 bytes / 11 cycles, vs the original ~19 bytes / ~30 cycles with
separate 16-bit pointer arithmetic + DPTR-staged indirect-Y store.

## Copy folding (`copy_folding.py`)

TAC-level pass that fuses adjacent `<producer dst=%t>; Copy(%t, X)`
pairs into `<producer dst=X>` when `%t` is single-use across the
function. Runs inside the TAC fixed-point loop (alongside
constant_fold / reduce_strength / cmp_zero_jump_fold / UCE /
copy_propagate / DSE) AND once more after `from_ssa` (the SSA
destruction pass emits Copies at predecessor block ends to feed each
Phi's source into the Phi's dst — those Copies are the loop-counter
`i++` shape, fusable but not yet present during the fixed-point
loop).

The fusion handles two distinct cases:

1. **Non-SSA-promoted dst** (the unique contribution of this pass).
   c99_to_tac emits `Binary(Add, x, 1, %t); Copy(%t, x)` for `x += 1`
   where x is a static or address-taken local — names that aren't
   SSA-renamed. copy_propagation can't forward `Copy(%t, x)` because
   x isn't an SSA-renamed name; fusion redirects the producer's dst
   to x, eliminating the temp.
2. **SSA-renamed dst**. After `from_ssa` lowers each Phi to
   `Copy(%phi_arg, %phi_dst)` at predecessor block tails, those
   Copies have an SSA-renamed dst. Fusion redirects the producer's
   dst to `%phi_dst` directly, dropping the round trip. (Inside the
   fixed-point loop this case is also handled by copy_propagation +
   DSE — fusion is just a faster equivalent.)

Eligible producers (any TAC instruction with a single Var dst):
SignExtend, ZeroExtend, Truncate, the six FP-conversion casts
(IntToFloat, IntToDouble, FloatToInt, DoubleToInt, FloatToDouble,
DoubleToFloat), Unary, Binary, Copy (chained-copy elimination),
GetAddress, Load, IndexedLoad, FunctionCall (when its dst is non-
None), IndirectCall (same).

Phi is deliberately excluded — Phi.dst is always an SSA-renamed name
in the IR shape this pass sees, and SSA construction's invariant (one
def per renamed name) keeps it that way until SSA destruction.
Redirecting Phi.dst would let the SSA destruction emit Copies into a
non-renamed name, which complects a different concern with this
pass's job.

Soundness gates:

- The Copy is the immediately-next instruction (adjacency). Without
  intervening side effects, no other op observes `%t` or `X` between
  the producer and the Copy, so redirecting is semantically
  identical.
- `%t` is used exactly once across the function. The use-count check
  makes the fusion sound regardless of `%t`'s SSA status — multi-def
  `%t` (uncommon outside non-SSA) is fine, since after fusion any
  remaining def writes a name nothing reads (DSE picks them up next
  iteration).
- `X` doesn't have to be SSA-renamed. The fusion preserves SSA: if
  `X` was renamed, it had exactly one def (the Copy); after fusion it
  still has one def (the redirected producer). If `X` is non-renamed
  (static), it had multiple defs; after fusion it still has multiple
  defs (one redirected here).

The composition with the multi-byte INC peephole is the headline win
for the static-RMW case: `static int x; x += 1;` previously lowered
to ~25 bytes (read X to %t through ADC chain, then Copy %t back to
X). After copy folding it becomes in-place `Binary(Add, x, 1, x)`;
tac_to_asm emits `LDA x; CLC; ADC #1; STA x; LDA x+1; ADC #0; STA
x+1`; the INC peephole then collapses to `INC x; BNE done; INC x+1;
done:` — 8 bytes total.

What still doesn't fire:

- `Op(... %t); ...; Copy(%t, X)` with intervening instructions. Could
  be lifted with a more thorough aliasing/liveness check in the gate,
  deferred until a motivating case appears.
- Asm-SSA-internal Phi destruction copies. The TAC fusion fires
  before tac_to_asm, but tac_to_asm and the asm-level SSA round-trip
  introduce their OWN Phi destruction copies on Pseudos that asm
  regalloc didn't coalesce. Those would need an asm-level analog of
  this pass — backward_copy_propagation handles a related shape but
  explicitly defers Pseudo-to-Pseudo coalescing to regalloc.
