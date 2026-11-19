# passes/optimization/CLAUDE.md

TAC-level optimizer. Active under `-O`. Runs after
`c99_to_tac` and before `tac_to_asm`. The root pipeline overview lives
in `/project/c6502/CLAUDE.md`; the asm-level SSA round-trip that runs
later lives in
[../optimization_asm/CLAUDE.md](../optimization_asm/CLAUDE.md);
peephole catalog lives in [../CLAUDE.md](../CLAUDE.md).

## Module roster

- `framework/` — `Pass` ABC, phase ABCs, `PhaseDriver`, pattern
  combinator DSL, `WindowPass` / `DefUsePass` / `OperandRewritePass` /
  `SinkPass` bases. See [framework/CLAUDE.md](framework/CLAUDE.md).
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
  Canonical home for `uses_in` / `defs_in` (Var read / write operands
  of an instruction) and `count_uses(instrs)` (a `Counter` of
  USE-position Var names, built on `uses_in`). Passes that need
  use-counts or def/use enumeration call these rather than rolling
  their own — several stale local copies were consolidated here.
- `optimizer.py` — `optimize_function` / `optimize_program`. Constructs
  a module-level `PhaseDriver` (`_DRIVER`) and calls `_DRIVER.apply`
  per function. `PhaseDriver` is defined in `framework/driver.py`.
- `loop_rotate.py` — `rotate_signed_countdown_loops`
  (`RotateSignedCountdownLoops`, PreSsaPass). Pre-SSA one-shot. The
  constant init must be written after the last label before the
  loop, so it reaches the loop on every path, and the counter's
  address must never be taken. The test may only compute `%`
  temporaries (`_computes_temporary`), since the rotated loop skips
  the first test, and `i-- >= 0` or `(n++, i) >= 0` would lose a
  write.
- `noreturn_call.py` — `noreturn_calls` (`NoreturnCalls`, PreSsaPass).
  After a `FunctionCall` to a function with `FunAttr.noreturn` (from
  `[[noreturn]]` on any declaration), a `Ret(val=None)` replaces the
  instructions up to the next label, so values read after the `if`
  around the call needn't survive it. The `Ret` has no value in any
  function, so `JSR f; RTS` becomes `JMP f`. The call graph reads
  `FunctionCall.noreturn`, which `c99_to_tac` sets from the same
  attribute, and leaves out such a call when it has no arguments
  (`passes/call_edges.py`); every pass that rebuilds a
  `FunctionCall` copies the flag, as it does `hw_stack_save`.
- `static_const_fold.py` — `fold_static_const_reads`
  (`FoldStaticConstReads`, PreFixedpointPass wrapping an internal
  `OperandRewritePass`). One-shot post-SSA.
- `constant_folding.py` — `constant_fold` (`ConstantFold`, WindowPass).
  A packed BCD `Binary(BcdAdd | BcdSubtract)` of two constants folds
  at its dst's width with `builtin_functions.bcd_add` / `bcd_sub`
  (`_fold_bcd`). Every other pass treats those two ops as an opaque
  pure binary operation.
  Unary / Binary / comparison / cast / conditional-jump-with-constant-
  cond (including a `JumpIfCmp` of two constants) / Phis with agreeing
  args. Width-correct wraparound at the
  operand's declared width. Const-array-subscript fold
  (`_fold_indexed_load`) is part of this pass.
- `distribute_constant.py` — `DistributeConstant` (raw
  FixedpointPass, right after `ConstantFold`). `(x ± c) * k`, with
  the sum single-use and `x` an SSA `ZeroExtend` of a byte, becomes
  `x * k ± c * k` (a negative product subtracted, so its top byte is
  `SBC #0`, which `carry_inc` makes `BCS; DEC`). The product of one
  significant byte shifts in A or is an 8-by-8 loop. `&p->options[n -
  1]` with 9-byte options is `n * 9 - 9`.
- `phi_offset.py` — `PhiOffset` (PostFixedpointPass, first). A Phi of
  single-use `p + k_i` (one Var `p`, constants 0-255) whose result's
  one reader is a load or store through it becomes `o = Phi(k_i)`,
  `ZeroExtend(o)`, and `p + that` after the join's Phis, so the access
  is `(p),Y` with the offset in Y. `p` must be unchanged from the
  adds to the join: SSA-renamed, or no store, call, write of `p`, or
  jump out of the range from the first add to the Phi
  (`_unchanged_between`), and not `volatile`. `n < 3 ? &E->sword :
  &E->dagger` is `LDY #7` or `LDY #9` and `STA (E),Y`.
- `strength_reduction.py` — `reduce_strength` (three `RuleSet` rules
  `MULTIPLY_RULE` / `DIVIDE_RULE` / `MODULO_RULE`, bundled as
  `STRENGTH_RULES`; `optimizer.py` runs them as one merged RuleSet).
  `Multiply(x, 2^k)` → `LeftShift`, unsigned `Divide(x, 2^k)` →
  `RightShift`, unsigned `Modulo(x, 2^k)` → `BitwiseAnd`.
  `Multiply(x, 2^a ± 2^b)` → two `LeftShift`s and an `Add` /
  `Subtract` through fresh temps `<dst>.sr0` / `<dst>.sr1`
  (registered in the symbol table and `ctx.ssa_dsts`), made only
  when `passes/shift_cost.py` says every shift inlines at the dst's
  width, so `mul*` is never traded for an `asl*` call. A byte
  `Multiply` by a constant of three signed powers of two (69 = 64 +
  4 + 1) becomes three shifts and two adds
  (`_three_term_byte_multiply`). Signed
  Divide / Modulo skipped (C99 truncation differs from arithmetic
  shift), unless `value_range.max_value` shows the dividend can't
  be negative, which makes `(space + 1) / 2` on a byte a shift. Each
  `build` is computational (log2 / mask construction) —
  the table form splits the per-op dispatch into rules but keeps the
  arithmetic in the build, per "RuleSet" in
  [framework/CLAUDE.md](framework/CLAUDE.md).
- **Jump-fold family** (`cmp_zero_jump_fold.py`, `and_zero_jump_fold.py`,
  `lnot_jump_fold.py`) — three `RuleSet` rules (`CMP_ZERO_RULE`,
  `AND_ZERO_RULE`, `LNOT_RULE`) sharing the `producer_then_jump` window
  and the `single_use` gate. Each module keeps its `build` +
  computational helpers and exposes a standalone `fold_*` entry point
  as a one-rule `RuleSet`; `optimizer.py` runs all three as a single
  merged `RuleSet` (`fold_producer_jumps`) in the fixedpoint. See
  "RuleSet" in [framework/CLAUDE.md](framework/CLAUDE.md). The AND
  rule also takes a wide operand whose mask falls in one byte `k`
  (`x & 0x8000`), testing byte `k` through new `>> 8k` and `Truncate`
  temporaries (`_try_wide_byte`).
  - `fold_cmp_zero_jump` (`CMP_ZERO_RULE`): fuses `Binary(cmp, ...);
    JumpIf*` to direct conditional jumps. `== 0` / `!= 0` traces through
    ZeroExtend; ordering ops emit `JumpIfCmp(op, src1, src2)` for the
    per-byte compare-chain lowering. Operand narrowing through
    ZeroExtend folds `(uint8_t)i < 105` to 3-instr `LDA / CMP / BCS`.
    Every narrowing reads the extend's source at the compare, so
    `extend_source_unchanged` requires an SSA-renamed source, or else
    no label, jump, store, call, or write to the source between the
    extend and the compare. `AND_ZERO_RULE` uses the same check.
  - `NARROW_VALUE_COMPARE_RULE` (same module, right after
    `CMP_ZERO_RULE` in the rule set): a comparison whose result isn't
    a jump's condition gets the same operand narrowing, into the same
    dst, so `return x < 10;` on a `uint8_t x` is a 1-byte compare.
  - `NARROW_JUMP_COMPARE_RULE` (same module, after it): a `JumpIfCmp`
    whose operand reached a `ZeroExtend` only after the fold, through
    a `Copy` that copy propagation removed later (`i < 40U` converts
    the promoted `int` to `unsigned`), narrows the same way, for an
    SSA-renamed extend source. Every narrowing looks through such a
    `Copy` to the extend behind it, since copy propagation keeps one
    that changes the signedness while the extend has other readers
    (`i < sizeof a`, with `sizeof` an `unsigned int`).
  - `fold_narrow_and_jump` (`AND_ZERO_RULE`): folds `(ZeroExtend(uchar);
    BitwiseAnd(_, 0x80); JumpIf*)` to `JumpIfMasked` when the operand
    narrows to 1 byte — direct `LDA / BPL/BMI` at asm lowering instead
    of an 8-bit AND + 16-bit Z.
  - `fold_lnot_jump` (`LNOT_RULE`): folds `Unary(LogicalNot, src, %t);
    JumpIf{True,False}(%t, target)` to a sense-flipped direct JumpIf
    when %t is single-use.
  - `LNOT_OF_ZERO_OR_ONE_RULE` (same module, right after `LNOT_RULE`):
    a `!` that no jump tests, of a value that is 0 or 1 (an object
    whose `LocalAttr` / `StaticAttr.was_bool` says it was a `bool`,
    or an SSA name from a comparison, a `!`, or an extend or copy of
    one), is `x ^ 1` at x's width, then extended or truncated to the
    result's, so `A = !flag` is `LDA flag; EOR #$01; STA A`.
  - `NONNEGATIVE_COMPARE_RULE` (`nonnegative_compare.py`, last in the
    rule set): a signed ordering `JumpIfCmp` or comparison of two
    values `value_range.max_value` bounds (neither can be negative),
    each a signed integer of 2 bytes or more and no constant 0, is
    done unsigned: a constant becomes the unsigned constant of its
    width, or the first Var is copied into a new unsigned temporary,
    a copy copy propagation keeps. `row + 1 < WNDBTM` on bytes loses
    its overflow test.
- `dead_loop_elimination.py` — `eliminate_dead_loops`
  (`EliminateDeadLoops`, raw FixedpointPass — CFG-shaped back-edge
  detection). Detects natural loops (back-edges via
  `cfg.natural_loops()`) whose body has no `Call` / `Store` / `Ret` and
  whose every SSA def is loop-local. When the gates pass, rewrites the
  header to `[Label, Jump(exit)]` so UCE prunes the now-unreachable body
  on the next sweep. Single-exit-edge gate keeps Phi-arg retagging at
  the exit unambiguous. Collapses nested empty-loop shapes like
  `do { while(--y); } while(--d);` to nothing. NOTE: lacking volatile
  semantics (the parser silently drops the qualifier), this pass will
  currently delete loops the programmer marked volatile; preserving them
  requires plumbing volatile through the type system.
- `jump_thread.py` — `ThreadConstantJumps` (raw FixedpointPass, right
  before UCE). A block of its label, a `Phi` whose SSA result only a
  `JumpIfTrue` / `JumpIfFalse` right after it reads, and that jump: an
  edge whose `Phi` argument is an integer constant goes to the
  successor the constant picks (the predecessor's jump is retargeted,
  or a `Jump` added after its fall-through). An empty predecessor, or
  one that only jumps to the block, reached only by jumps, has those
  jumps retargeted instead, which is what `&&` / `||` leave. Each
  `Phi` at the successor that took a value from the block takes it
  from the new predecessor. Not when the predecessor already has an
  edge to the successor, or the new edge would enter a loop's body
  past its header. An inlined `return false;` tested by the caller no
  longer loads 0 and branches on it. See
  docs/optimizations/jump_thread.md.
- `unreachable_code_elimination.py` — `EliminateUnreachableCode` (raw
  FixedpointPass — forward DFS from ENTRY). Prunes dead Phi args; folds
  singleton Phis to Copies; drops useless jumps / labels.
- `phi_constant_fold.py` — `FoldIntoConstantPhi` (raw
  FixedpointPass, right after `ConstantFold`). A `Phi` of integer
  constants whose SSA dst has one reader, a `Unary`, a `Binary` with
  a constant other operand, or a `Truncate` that `constant_folding._fold`
  folds for every argument, takes the reader's operation in its
  arguments: the Phi defines the reader's dst (an SSA name) and the
  reader goes, or a new temporary that the reader becomes a copy of.
  An extend isn't taken, since a byte Phi of 0 and 1 is what
  `rotate_in` looks for. `B = (flag ? 3 : 2) + 1` loads 4 or 3.
- `cse.py` — `eliminate_common_subexpressions`
  (`EliminateCommonSubexpressions`, raw FixedpointPass — dominator-
  tree walk). Replaces a repeated pure computation (`Binary`, `Unary`,
  extends, `Truncate` on SSA operands) by a Copy from the first one
  whose block dominates it. A repeated non-volatile load is replaced
  only along a chain of single-predecessor blocks with no store, call,
  volatile access, or write to a non-SSA name in between. Keys include
  the dst type and the whole constant node (the FP constants keep
  their value in `bits`), except a constant shift count, which is
  keyed by its value alone, so `i << (uchar)1` from strength reduction
  matches C's `i << 1`.
- `licm.py` — `hoist_loop_invariants` (`HoistLoopInvariants`, raw
  FixedpointPass). Moves a loop-invariant extend or `Truncate` to the
  loop's preheader. Adds aren't moved, since one used once on a
  conditional path costs a store and a load to hold across the loop.
- `reassoc_offset.py` — `reassoc_offset_constant`
  (`ReassocOffsetConstant`, raw FixedpointPass). `p + (x + k)` →
  `(p + x) + k` when the offset has one use and every operand has the
  same width, so `dict[n + 1]`, `dict[n + 2]`, ... share `dict + n`
  (CSE) and each `+ k` becomes `LDY #k`. A Constant `p` is left to
  `reassoc_constants`.
- `copy_propagation.py` — `copy_propagate` (`CopyPropagate`, raw
  FixedpointPass — CFG-shaped forward dataflow). SSA-aware: replaces
  every use of a Copy's dst with its src; chains too. A Copy whose
  src and dst differ in signedness is the lowering of a same-width
  cast like `(unsigned)s`, and isn't propagated (a Constant src is
  propagated retagged to the dst's variant), because `/`, `%`, `>>`,
  and ordering compares take their signedness from the operand.
- `dead_store_elimination.py` — `EliminateDeadStores` (raw
  FixedpointPass — SSA-aware whole-def-use liveness). Drops pure defs
  with no reads. Calls keep the call (side effects) but drop unused dst.
- `copy_folding.py` — `fold_copies` (`FoldCopiesInFixedpoint` /
  `FoldCopiesPostDestruction`). See "Copy folding" below. Internally
  delegates to `_FoldCopiesWindow` (WindowPass).
- `reassoc_const.py` — `reassoc_constants` (`ReassocConstants`,
  DefUsePass). See "Add-with-Constant reassociation" below.
- `extend_add_chain.py` — shared recognizer core. `recognize_extend_add_
  chain(addr_var, env, ctx)` walks back `%addr → Binary(Add, base, %ext)
  → %ext := ZeroExtend(idx)` applying the gates all three
  addressing-mode recognizers share (symbols present; `%addr` / `%ext`
  SSA-renamed, `%addr` single-use; 1-byte `idx`). A SignExtend index
  doesn't fold, since X and Y are unsigned and `p[-1]` would address
  `p + 255`. The walk looks
  through a few `Copy`s to the extend (an `int` to `unsigned` cast is
  a Copy copy propagation keeps). A multi-use `%ext`, as CSE leaves
  when subscripts share an index, still folds, and the chain's
  `drop_defs()` leaves its def for the other uses. Plus `is_1_byte_var` /
  `is_1_byte_val` / `all_dsts`. Each recognizer post-checks `base`'s kind
  (Constant for absolute,X; pointer Var for (zp),Y) and emits its node.
- `direct_member_access.py` — `direct_member_access`
  (`DirectMemberAccess`, raw FixedpointPass). A `Load` / `Store` (or
  `IndirectIndexed*` with a constant index) through `GetAddress(obj)`
  plus a constant `k` becomes `IndexedLoad` / `IndexedSymbolStore` of
  `obj` at index `k`, which `tac_to_asm` lowers to `LDA g+k` / `STA
  g+k`, or to the local's own byte. Applies to statics and to
  automatic structs, unions, and arrays that aren't parameters (an
  automatic scalar could be SSA-renamed once its address is gone).
  A local struct whose every access is rewritten is no longer
  address-taken, so the asm regalloc gives it zero-page slots.
  `var_visit.uses_in` returns the name an `IndexedLoad` /
  `IndexedSymbolStore` accesses as a read, so copy folding and DSE
  keep the copy that fills such a struct.
- `recognize_indexed_store.py` / `recognize_indexed_load.py` —
  `RecognizeIndexedStore` / `RecognizeIndexedLoad` (DefUsePass, on
  `recognize_extend_add_chain`). Collapse `ZeroExtend(uchar) +
  Binary(Add, C) + Store/Load` chains to `IndexedStore` / `IndexedLoad`
  (absolute,X) when `base` is a Constant in 0..0xFF00. Drop the two
  intermediate defs via `Rewrite.drop_defs`. The standalone `fold_*`
  entry points delegate to the same class. See "IndexedStore recognizer"
  below.
- `truncate_extend_fold.py` — `fold_truncate_extend`
  (`FoldTruncateExtend`, DefUsePass). Folds `Truncate(SignExtend|
  ZeroExtend(x))` → `Copy(x)` (or narrower `Truncate`) when the
  round-trip is the identity at the destination's width, to the same
  extend of `x` when the destination is between the two widths, and
  `Truncate(Truncate(x))` → `Truncate(x)`. `FoldExtendExtend`, right
  after it, makes an extend of a widening extend one extend:
  `SignExtend(ZeroExtend(x))` is `ZeroExtend(x)`, so `i < sizeof a`
  on a `uint8_t i` narrows to a byte compare.
- `sink_increment.py` — `sink_increments` (`SinkIncrements`, SinkPass).
  Moves `Y = X + c` past the last in-line use of `X` when `Y`'s only
  consumer follows, exposing `recognize_indexed_*` patterns the original
  ordering hid. `X` must be SSA-renamed (a static could be written in
  between), and the scan stops at any write to `X`. A jump on `X`
  right after the step, `X`'s last use, tests `Y` against the
  constant moved by `c` instead (`_test_the_step`), so `while (x-- !=
  0)` is `DEX; CPX #$FF; BNE`.
- `sink_and_past_branch.py` — `SinkAndPastBranch` (raw FixedpointPass —
  duplicates a trio into multiple successors with SSA renaming; different
  shape from SinkPass). See "sink_and_past_branch" in the peephole
  catalog. The original ZeroExtend or AND stays when another
  instruction reads its result.
- `narrow_byte_bitwise.py` — `NarrowByteBitwise` (DefUsePass). A wide
  `&`, `|`, or `^` with an extended byte among its operands, whose
  result's high bytes are zero, becomes the operation on the low bytes
  into a new `uchar` and a `ZeroExtend` of it, so that a comparison of
  `(x & 0x3F)` narrows to one byte. Last in the fixed point.
- `narrow_widened_arith.py` — `narrow_widened_arith` (`NarrowWidenedArith`,
  DefUsePass). Folds `Truncate(Binary(safe_op, Extend(a), Extend(b) |
  Constant), u_n)` → `Binary(safe_op, a, b | narrowed_Constant, u_n)`
  at the narrow width. Wraparound-safe ops are Add / Subtract / Multiply
  / BitwiseAnd / BitwiseOr / BitwiseXor, plus LeftShift by a variable
  count (a loop of one-bit shifts at the narrow width) or by a constant
  below the narrow width (Divide / Modulo / RightShift excluded
  because high bytes contribute to low bytes of the result). An
  operand may also be any wider integer Var (a 16-bit global, say),
  whose low bytes come from a new `Truncate` before the narrow
  Binary, when the Truncate is the wide Binary's only reader (or the
  narrow Binary would repeat its work). The narrow Binary reads its operands at the Truncate, so a
  non-SSA one must be unchanged since the wide Binary or Extend read
  it (same block, no call, store, or write to it in between; not
  volatile), or `int t = g + 1; h(); (uint8_t)t` would read `g` after
  `h` changed it. The Truncate is found by identity
  (`_truncate_index`), since a global written by several Truncates
  has another one as its last definition. Eliminates the dead
  high-byte arithmetic that C99's `uint8_t op uint8_t`
  integer-promotion-then-truncate idiom otherwise leaves in the IR. MUST
  run AFTER `sink_and_past_branch` inside the fixed-point loop — sink's
  strict 4-instr trio (`ZE; AND; Trunc; JumpIfMasked`) wouldn't match
  the narrow form this pass produces, and the AND result would spill
  across the branch (apply_bobble regression observed at +1 instr).
  A `Truncate` of a comparison becomes the comparison with the narrow
  dst and its operands unchanged (`_narrow_compare`), since its result
  is 0 or 1, so `bool b = (x & 0x80) != 0;` gets a 1-byte result.
- `value_range.py` — `max_value(val, def_of, symbols, ssa_dsts)`:
  the largest value an operand can hold when it can't be negative,
  from its type (an unsigned type's largest value) and, for an SSA
  name, from its definition (an extend, a copy, `&`, `|`, `^`, `+`,
  `*`, `<<` that doesn't wrap, `>>`, `/`, `%`, a `Phi`), walking back
  at most 12 definitions. `narrow_byte_range` and strength reduction
  use it.
- `narrow_byte_range.py` — `NarrowByteRange` (DefUsePass), right
  after `narrow_widened_arith` in the fixed point. A wide `/` or `%`
  whose dividend `max_value` bounds by 255, by a constant from 1 to
  255 or a value bounded by 255, becomes the unsigned byte operation
  into a new `uchar` and a `ZeroExtend` of it, and so does a wide
  `>>` of such a value (by any count; a constant count of 8 or more
  is a copy of 0). A 2-byte `*` of two such values reads each operand
  as a `ZeroExtend` of a byte (a `Truncate` of a wide one), so that
  `tac_to_asm` lowers it as an 8-by-8 loop. `pos / 20` on a byte had
  called `sdivmod16`, and `price * (((style >> 5) & 3) + 1)` `asr16`
  and `mul16`. A `<<` by a constant from 0 to 7 of a value whose
  bound shifted that far is at most 255 is a byte shift too, so the
  high byte of `(row / 8) << 3` isn't computed. See
  docs/optimizations/narrow_byte_range.md.
- `recognize_indirect_indexed.py` — post-fixedpoint one-shot
  (`RecognizeIndirectIndexed`, PostFixedpointPass wrapping an internal
  `DefUsePass`, on `recognize_extend_add_chain`). Collapses
  `ZeroExtend(uchar) + Binary(Add, ptr) + Load` to `IndirectIndexed`
  for the `(zp),Y` lowering when `base` is a pointer Var that doesn't
  resolve to a Constant. A constant byte offset, `Binary(Add, ptr,
  k)` with `0 <= k <= 255` feeding the Load / Store, becomes the same
  node with index `ConstUChar(k)`, which lowers to `LDY #k` (the
  shape of `p[10]` and of a byte struct member through a pointer).
  With such a constant index, a wider value folds too when `k + w -
  1 <= 255` (`_fits_constant_offset`), as a 2-byte member `p->gold`
  does, and `tac_to_asm` reads or writes it a byte at a time at
  `k`, `k+1`, and so on.
  Such an address may have several readers when every one is a
  1-byte Load or Store and `p` is unchanged at each
  (`_all_byte_accesses`, with `narrow_widened_arith.var_unchanged`),
  as in `p[1]++`; the Add is then left for the asm byte DCE.
  An access at a constant offset `k >= 1` through `p + ZeroExtend(b)`,
  its one reader, is `(p),Y` with index `b + k` (a new byte
  temporary) when `b`'s largest value (`_byte_max`: a left or right
  shift or an AND by a constant) plus `k` fits in a byte
  (`_byte_index_sum`), so `p[i + 1]` after `p[i]` at an even `i` is
  `INY`.
  An address `p + ZeroExtend(i)` read by several 1-byte loads and
  stores (`p[i] &= m`) is `(p),Y` with Y = `i` in each, for an
  SSA-renamed `i` (`_shared_extend_add_chain`). A reader `(p +
  ZeroExtend(i)) + k` with `k` outside 0-255, read by one 1-byte
  access, is `(t),Y` with `t = p + k` made just before the access
  (`_offset_from_extend_chain`), as CSE leaves `background[x]` for
  `background = line + 0x2000`.
  Deliberately runs LAST so any pointer that's going to fold to a Constant (via the const-static
  fold path) has already done so — otherwise this pass would prematurely
  lock in (zp),Y for a chain that would have qualified for the cheaper
  absolute,X form.
- `static_indexed_access.py` — `static_indexed_access`
  (`StaticIndexedAccess`, PostFixedpointPass, before
  `recognize_indirect_indexed`). A `Load` / `Store` (or an
  `IndirectIndexed*` with a constant index) through `GetAddress(o) +
  ZeroExtend(b) [+ k]`, with `o` static and `b` one byte, becomes
  `IndexedLoad(o, b, offset=k)` / `IndexedSymbolStore(o, b, src,
  offset=k)` (`LDA o+k,X`); a pointer value `(GetAddress(o) + z) + k`
  becomes `(GetAddress(o) + k) + z`, whose constant part
  `label_offset_fold` makes one immediate. `c99_to_tac` scales an
  index into an array object of at most 255 bytes at byte width
  (`_byte_offset_scale`), which is what makes `b` a byte for
  `o[i].m`. The `offset` field of `IndexedLoad` / `IndexedSymbolStore`
  is kept by every pass that rebuilds one. `c99_to_tac` itself puts a
  constant added to an unsigned byte index of a static array there
  (`_split_index_addend`), so `t[x + 1]` is `LDA t+1,X`. A subscript
  of a cast of a static object's address, `((uint8_t *)&S)[i]` or
  `*((uint8_t *)&S + i)`, reads `S` as an array (`_object_view`) and
  lowers to `IndexedLoad(S, i)` / `IndexedSymbolStore(S, i, v)` the
  same way.
- `compare_subtract.py` — `hoist_subtract_to_compare`
  (`HoistSubtractToCompare`, PostFixedpointPass). Moves `t = a - b`
  from a successor (whose only predecessor is the compare's block) to
  just before the ordering `JumpIfCmp` that compares `a` with `b`,
  for operands of 2 bytes or more; `tac_to_asm._fused_subtract` then
  branches on the subtraction's flags instead of comparing, with any
  copies SSA destruction puts between them. An
  operand may be a second read of the same memory as the compare's
  (`ZeroExtend(g)` in both blocks), which it replaces with the
  compare's. 1-byte operands are left to the asm `cmp_sbc_fusion`.
  When `a` is a Phi of the compare's block (the value a loop carries)
  and only the other successor reads it after the compare, that
  successor gets `a.exit = t + b` instead (re-reading `b` from memory
  when it's a read of a global), so `a` dies at the subtract and the
  loop updates it in place.
- `divmod_hoist.py` — `hoist_divmod_pairs` (`HoistDivmodPairs`,
  PostFixedpointPass, after `hoist_subtract_to_compare`). A `/` and a
  `%` of the same operands with a store, a call, or a branch between
  them are computed next to each other, at the first when its block
  dominates the other's and otherwise at the end of their nearest
  common dominator, when every path from there to the exit passes one
  of them. A destination SSA didn't rename gets a new temporary and a
  `Copy` where the instruction was. No operand may be written on the
  way, and for a global one no pointer store, `IndexedStore`, or call
  may come either. `tac_to_asm._plan_divmod_pairs` then lowers the
  pair with one loop or one helper call. Ali Baba's
  `load_scene_from_disk` (`P->sector = s % 13; P->track = 0x11 + s /
  13`) and `render_font_char_direct` (a `/` in an `if`, the `%` after
  it). See docs/optimizations/divmod_hoist.md.
- `count_down.py` — `count_loops_down` (`CountLoopsDown`,
  PostFixedpointPass, after `hoist_divmod_pairs`). A loop's byte
  counter read only by its own test (the first instruction after its
  `Phi`, by `>=` / `==` to leave or `<` / `!=` to stay, with a constant
  bound at least the start, or a loop-invariant SSA bound and a start
  of 0) and by its `+ 1` step holds `n - i` instead: the `Phi` starts
  at `c1 - c0` (or the bound), the step subtracts 1, and the test is
  `== 0` or `!= 0`. The block the loop stays in must have the test's
  as its only predecessor and dominate the step. `for (uint8_t i = 0;
  i < 20; i++) out(0xA0);` ends `DEC i; BNE`. See
  docs/optimizations/count_down.md.
- `read_stored_global.py` — `read_stored_globals`
  (`ReadStoredGlobals`, FixedpointPass, right before
  `FoldCopiesInFixedpoint`). `Binary(..., T); Copy(T, G)` with `T` an
  SSA name and `G` a non-volatile global of the same type, and one
  more read of `T` later in the block (not in a `Phi`, an extend, or a
  `Truncate`, and only as the pointer of a load or store through one),
  with no write of `G`, pointer store, or call between: the read
  reads `G`, and copy folding then makes the `Binary` write `G`.
  `*++P` on a zero-page `P` is `INC P; BNE; INC P+1; LDA (P),Y`. See
  docs/optimizations/read_stored_global.md.
- `step_on_edge.py` — `move_steps_onto_edges` (`MoveStepsOntoEdges`,
  PostDestructionPass, right after the post-destruction
  `fold_copies`). `T = X op c` (`+`, `-`, `&`, `|`, `^`) whose `T` is
  written there alone and read by one `Copy(T, Z)` alone, in the block
  its block's jump goes to (with that jump its only way in) or falls
  through to, goes in place of the `Copy` as `Z = X op c`, when
  nothing on the way writes `X`. `X` and `T` must be SSA names.
  `do g(i); while (i-- != first);` steps with `DEC i` on the back
  edge. See docs/optimizations/step_on_edge.md.
- `three_way_compare.py` — `split_three_way_compares`
  (`SplitThreeWayCompares`, PostDestructionPass, after the
  post-destruction `fold_copies`). A 2-byte unsigned ordering
  `JumpIfCmp` and an equality `JumpIfCmp` of the same values, in a
  row or with the equality at the ordering one's target, become a
  `<` and a `!=` of the high bytes and then a compare of the low
  bytes, each outcome going where the two compares sent it. The bytes
  are a constant's, the `h` and `l` of a value built as
  `(ZeroExtend(h) << 8) | ZeroExtend(l)` in one block (a load of
  either moves down to its compare when nothing in between writes
  memory), or new `Truncate(a >> 8)` / `Truncate(a)` temporaries.
  When the equality compare is at the target, the less outcome's code
  must end in a `Ret` or `Jump`, and the low bytes branch over it so
  the asm `tail_merge` can share it. It runs after destruction
  because the new edges would require Phi arguments in SSA form. Zork's
  `match_dictionary_word` compares its word with each entry this way.
- `short_circuit_jump_fold.py` — `fold_short_circuit_jump`
  (`FoldShortCircuitJump`, raw PostDestructionFixedpointPass — multi-
  block retarget map with transitive closure). Post-destruction fixedpoint.
  Rewrites the canonical `&&` / `||` 0-or-1-materialize tail + a
  `JumpIf{True,False}` consumer (with phi copies that don't touch the
  temporary allowed in between, kept on both paths, the chain's
  through a stub after the last instruction) into direct conditional
  branches: the
  chain's short-circuit jumps retarget to the consumer's destination, the
  5-instruction tail and 1-instruction consumer are deleted, and the
  "flipped" sub-case emits `Jump(T); Label(.<fn>@scfold@<N>)` to
  materialize the fall-through. Copies on the short-circuit arm alone
  (before `Copy(C_sc, %t)`) go first on the chain's path. Covers all four `(C_ft, C_sc) ×
  consumer-kind` combinations and nested short-circuits (`(a && b) || c`
  style) via transitive closure on the retarget map. After each sweep,
  `merge_equality_jumps` makes two adjacent jumps to one target on
  `x == a` and `x == b`, with `a` and `b` one bit `p` apart, one jump on
  `(x & ~p) == min(a, b)` (a non-volatile integer `x`), so `e == 0 ||
  e == 1` is `AND #$FE; ORA hi; BEQ`.
- `known_compare_thread.py` — `thread_known_compares`
  (`ThreadKnownCompares`, PostDestructionFixedpointPass, after
  `FoldShortCircuitJump`). A scan follows the `==` / `!=` facts the
  conditional jumps it passes give about non-volatile integer Vars
  (`JumpIfCmp` with an integer constant, `JumpIfTrue` / `JumpIfFalse`
  against 0), dropping a Var's at a write of it and all of them at a
  label or at an instruction other than the jumps and those that
  write only their `dst`. A jump goes past the labels, the tests its
  edge decides, and the `Copy`s into locals dead where it lands
  (`_liveness`) at its target, to a `.<fn>@cthread@N` label if needed,
  and a conditional jump the code before it decided becomes a `Jump`
  or goes. Then the blocks nothing reaches go, and a conditional
  jump's target block is swapped in front of the block after the
  `Jump` that follows it, when that block ends in a `Jump` to where
  the target falls through, so `branch_invert` makes one branch.
  `if (t == 20 || t == 10) f(t == 20 ? a : b);` tests 20 once
  (Lode Runner's `handle_timers`). See
  docs/optimizations/known_compare_thread.md.
- `dispatch_pointer_array.py` — `dispatch_const_pointer_arrays`
  (`DispatchConstPointerArrays`, ProgramPass). Runs at the program level
  after `optimize_tac` (post-from_ssa). Recognizes the
  `Binary(LeftShift|Multiply, i, 1|2) + IndexedLoad(arr, _, %ptr) +
  IndirectIndexedLoad(%ptr, j, %v)` chain when `arr` is a file-scope
  `static T * const[N]` (const elements, checked in the symbol table)
  with N ≤ 8 and all-AddressInit elements;
  rewrites to a CMP/BEQ dispatch on `i` with per-case direct
  `IndexedLoad(target_k, j, %v)`. Eliminates DPTR staging and (zp),Y
  indirection at the cost of a small dispatch chain; frees X and Y from
  the dual-index conflict so loop counters can stay pinned to X across
  the dispatch.
- `split_tables.py` — `apply_split_tables`. Runs at the program level
  right after `dispatch_const_pointer_arrays`. A `static const` array
  of at most 256 elements of 2 bytes or more, with internal linkage,
  that the unit reads only by a byte index is laid out as the low
  bytes of every element, then the next bytes, and so on (`plane_init`,
  which `row_address_tables` uses too), and each read becomes
  `IndexedLoad(T, i, dst, offset=k, plane=N)`, `LDA T+k,X` and `LDA
  T+k+N,X`. A read is an `IndexedLoad` through `i << log2(w)` (or a
  constant offset), or a `Load` through `GetAddress(T) + (ZeroExtend(i)
  << log2(w))` for a table of more than 255 bytes, each with its chain
  in one block and `i` unchanged up to the load (no write of it, and
  no call or pointer store when it's a static or address-taken). Any
  other reference to the table, an `AddressInit` in another static
  included, leaves it alone. The table's symbol becomes an array of
  `const` bytes whose `Initial` tuple holds the bytes in the new order,
  integers or `AddressByteInit`s, which `constant_folding._fold_plane_load`
  reads. See docs/optimizations/split_tables.md.
- `inline_functions.py` — `inline_functions`. Runs at the program
  level in `compile.translate_for_codegen` (and the `--tac -O`
  stage), right after `c99_to_tac` and before
  `return_pointers_in_zero_page`, so every later pass reads only the
  functions left. A `static` function whose address isn't taken, on
  no call cycle, with no convention or placement attribute
  (`zp_abi`, `section`, `address`, `return_bool_in_carry`,
  `noreturn`, a `register`, `in_*`, or `zeropage(addr)` parameter, a
  `register` local), with scalar parameters and result, and with no
  indirect call, `save_hardware_stack` call, address-taken local, or
  block-scope `static` in its body, has all its calls inlined or
  none. Each call becomes copies of the arguments into renamed
  parameters (`@N.name` or `%N` with a fresh N, the symbols copied),
  the body with its labels prefixed `.ik_`, and each `Ret(v)` as
  `Copy(v, d); Jump(.ik@end)`. A call followed by a `Ret` of its
  result, or by a bare `Ret`, keeps the body's `Ret`s. Callers with a
  `register` local are skipped. Candidates go leaves first.
  `_Inliner._pays` compares `(n - 1) * BYTES_PER_COST * cost`, the
  cost of an optimized copy of the callee, plus `EARLY_RETURN_BYTES`
  for each early return at each call not in tail position, with
  `n * (CALL_BYTES + ARGUMENT_BYTE_COST * a + RESULT_BYTE_COST * r) +
  1`. A function with no call left is dropped. See
  docs/optimizations/inline_functions.md for the measurements behind
  the constants.
- `row_address_tables.py` — `apply_row_address_tables`. Runs at the
  program level at the start of `compile.optimized_asm`, before
  `optimize_tac`, on the TAC from `c99_to_tac`, since strength
  reduction would turn the multiply into shifts. `GetAddress(A) +
  (ZeroExtend(i) ± k...) * R`, with A a static array (or one at a fixed
  address) of N rows of R bytes, R not a power of two, `i` an
  unsigned non-volatile byte, `-255 <= k <= N`, and every temporary
  defined and read once, becomes `IndexedLoad(A@rows, i, p, offset=k,
  plane=N+1)` at the `ZeroExtend`'s place. `A@rows` holds the low and
  then the high bytes of the N+1 row addresses (numbers for a fixed
  address, `AddressByteInit`s otherwise), but only in the TAC: its
  symbol's `StaticAttr.planes` names the objects that hold the two
  halves, `A@lo` and `A@hi`, and `tac_to_asm` emits those in its
  place (`_planes`, `_plane_object`) and reads byte j of a lookup from
  the j-th. `_choose` keeps the set of arrays whose accesses save more
  than their tables cost (`_saving`: 40 bytes a lookup over two shifts
  and an add or subtract, 10 over a `mul16` call), and arrays with the
  same row count share a half with the same bytes, one object named
  after the first array, so `LEVEL_MAP` reads `LEVEL_BG@lo`. Each half's
  symbol is `StaticAttr.mergeable`, which puts `merge=true` on its
  `item` line, and the linker keeps one copy of the halves several
  units make with the same bytes (`linker._merge_identical`). See
  docs/optimizations/row_address_tables.md.

`IndexedLoad.plane` is 0 for an ordinary array. When it isn't, the
index is an element number, and byte k of the value is at `name +
offset + index + k * plane`. Every pass that rebuilds an
`IndexedLoad` keeps it, CSE keys on it, and `tac_to_asm`, `tac_sim`,
and the const-array fold (`_fold_plane_load`) read it.

## Per-function pipeline shape

The pipeline is run by `_DRIVER` (`PhaseDriver`) in `optimizer.py`.
`optimize_function` calls `_DRIVER.apply(fn, ctx)`.

```
fn → rotate_signed_countdown_loops (PreSsaPass, one-shot)
   → noreturn_calls                (PreSsaPass, one-shot)
   → to_ssa                        (owned by PhaseDriver)
   → fold_static_const_reads       (PreFixedpointPass, one-shot)
   → [constant_fold → distribute_constant → fold_into_constant_phi
      → reduce_strength → fold_producer_jumps
      (cmp_zero ∪ narrow_and ∪ lnot ∪ nonnegative_compare, one
      merged RuleSet)
      → eliminate_dead_loops → thread_constant_jumps → UCE → cse → licm
      → copy_propagate → DSE → read_stored_globals → fold_copies
      → reassoc_constants
      → reassoc_offset_constant → direct_member_access
      → recognize_indexed_store → recognize_indexed_load
      → fold_truncate_extend → fold_extend_extend → sink_increments
      → sink_and_past_branch → narrow_widened_arith
      → narrow_byte_range → narrow_byte_bitwise]*      (FixedpointPass loop)
   → phi_offset                    (PostFixedpointPass, one-shot)
   → static_indexed_access         (PostFixedpointPass, one-shot)
   → recognize_indirect_indexed    (PostFixedpointPass, one-shot)
   → hoist_subtract_to_compare     (PostFixedpointPass, one-shot)
   → hoist_divmod_pairs            (PostFixedpointPass, one-shot)
   → count_loops_down              (PostFixedpointPass, one-shot)
   → from_ssa                      (owned by PhaseDriver)
   → fold_copies                   (PostDestructionPass, one-shot)
   → move_steps_onto_edges         (PostDestructionPass, one-shot)
   → split_three_way_compares      (PostDestructionPass, one-shot)
   → [fold_short_circuit_jump
      → thread_known_compares]*     (PostDestructionFixedpointPass)
   → fn'
```

`docs/optimizations/README.md` describes how `-O` is organized,
with diagrams, and has a document for each pass. Brief notes per
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

4. **Fixed-point loop**. Passes rotated to convergence (see the module
   roster above for the per-pass entry points; the three jump-folds run
   as one merged `fold_producer_jumps` RuleSet).

5. **`recognize_indirect_indexed`** (`recognize_indirect_indexed.py`).
   Post-fixedpoint one-shot.

5a. **`dispatch_const_pointer_arrays`** (`dispatch_pointer_array.py`).
    Program-level post-from_ssa.

6. **`from_ssa`** (`ssa_destruction.py`). One Copy per PhiArg in the
   matching predecessor, before the terminator. Parallel-copy
   ordering by topological sort fixes the "lost copy" problem; cycles
   break with a fresh `<funcname>.cycle_tmp@N`. When the predecessor
   ends in a conditional jump, the copies can't go in front of it if
   they'd change something the jump or the other successor reads, or
   if the other edge has copies too (a do/while's bottom test feeds
   both the header's and the exit's Phis). Then they go on the edge:
   after the jump for the fall-through successor, or in a new
   `.<fn>@ssa_split@N` block the jump is retargeted to. Each edge
   finds its predecessor's jump by the index the jump had before any
   copies went in, since the fall-through edge's copies follow the
   jump once they're placed.

7. **Post-destruction folds**.
   - **`fold_copies`** (`copy_folding.py`) — fuses the Copy round
     trip emitted at predecessor block ends to feed Phi sources
     into Phi dsts (covered above).
   - **`split_three_way_compares`** (`three_way_compare.py`) — a
     2-byte `<` and `==` of the same values as byte compares
     (covered above).
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
   - **`thread_known_compares`** (`known_compare_thread.py`, in the
     same fixed point, after the fold) — a jump goes past the
     equality tests at its target that its own path decided, and the
     block a conditional jump goes to is placed after the `Jump` that
     follows it (covered above).

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

- `StaticAttr(initial_value=Initial(c))` with `c` being an `int`
  (NOT `AddressInit` — link-time symbol; NOT a tuple — aggregate),
  which for a `float` or `double` object holds its IEEE 754 bits, so
  the constant is `ConstFloat(bits=c)` / `ConstDouble(bits=c)`;
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
- `%t` is not a static and its address is never taken
  (`_observable_names`). Other functions read a static, and a `Load`
  through a pointer reads an address-taken local, and neither read
  appears in the use count. Without this gate, `g = 7; h = g;` with
  a global `g` fused into `h = 7` and never stored to `g`.
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
