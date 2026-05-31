"""TAC optimizer driver.

Wraps SSA-in / fixed-point cycle / SSA-out around the function. The
fixed-point cycle re-runs until the function's instruction list is
structurally unchanged from the start of an iteration; each cycle
sweeps every pass regardless of whether earlier passes converged,
since a pass already at fixed point is cheap to re-run and the
between-pass interleaving is part of the optimizer's contract.

Pipeline shape (now driven by PhaseDriver):
    fn → loop_rotate (one-shot, pre-SSA)
       → SSA construction
       → fold_static_const_reads (one-shot, pre-fixedpoint)
       → (CF → strength_reduce → cmp_zero_jump_fold → and_zero_jump_fold →
          lnot_jump_fold → dead_loop_elim → UCE → CopyProp → DSE →
          CopyFold → ...)*
       → recognize_indirect_indexed (one-shot, post-fixedpoint)
       → SSA destruction
       → CopyFold (post-destruction)
       → fold_short_circuit_jump* (post-destruction, until converged)
       → fn'

`loop_rotate` runs before SSA because the rewrite is a structural
shuffle of instruction ranges with no name updates — pre-SSA the
loop counter `x_var` carries one canonical name across init, body,
and post, so a name-preserving move suffices. After the rotation,
`to_ssa` rebuilds Phis for the new control flow.

Promotable Vars (block-scope locals, params, and TAC temps that are
never address-taken and have scalar type) are renamed and Phi'd
between SSA-in and SSA-out. Address-taken locals, statics, and
aggregates pass through unchanged.

Per-pass roles:
  - constant_fold: fold Unary / Binary / cast over Constant operands;
    fold a Phi whose every PhiArg.source agrees into a Copy.
  - strength_reduce: rewrite Multiply / unsigned Divide / unsigned
    Modulo by power-of-2 constants into LeftShift / RightShift /
    BitwiseAnd.
  - cmp_zero_jump_fold: rewrite `Binary(==/!=, x, 0, cond);
    JumpIfTrue/False(cond, t)` as a direct JumpIf on x (with sense
    flip), tracing through ZeroExtend defs to operate at the
    narrowest available width.
  - eliminate_dead_loops: detect natural loops whose body is pure
    (no Call / Store / Ret) and every SSA def is loop-local
    (no use outside the body); rewrite the header to jump past
    the body so UCE prunes it on the next sweep. Composes with
    DSE to collapse the nested empty-loop shape down to nothing.
  - UCE: prune unreachable blocks; fold singleton Phis to Copies;
    treat Phi pred_labels as label uses so SSA destruction can
    later locate predecessors.
  - copy_propagate, eliminate_dead_stores: SSA-aware versions.
  - fold_copies: fuse `<producer dst=%t>; Copy(%t, X)` adjacent
    pairs into `<producer dst=X>` when `%t` is single-use.
    Eliminates the temp round-trip when the Copy's dst isn't an
    SSA-renamed name (the case copy_prop + DSE can't reach,
    typically static-storage rmw like `static int x; x++;`).

Termination: each pass is a pure function on `tac_ast.Function`,
and dataclass `__eq__` compares structurally — so the loop exits
as soon as no pass in a cycle made a structural change.

Per-program shape: only `Function` top-levels get optimized;
`StaticVariable` entries pass through unchanged (their `init` is a
constant byte layout, not control flow).

Calling `optimize_function` without `symbols` (e.g. legacy unit
tests that exercise the driver on synthetic Functions) skips SSA
construction entirely — the symbol table is required to register
fresh SSA names with their types, and we'd rather no-op than
silently emit untyped temporaries that downstream passes can't size.

This driver does NOT perform register allocation. Coloring decisions
live in the asm-level optimizer (`passes/optimization_asm/`), which
operates on the post-`tac_to_asm` IR with byte-granular precision.
"""

from __future__ import annotations

import tac_ast
from passes.optimization.framework import PhaseDriver, PassContext, RuleSet
from passes.optimization.loop_rotate import RotateSignedCountdownLoops
from passes.optimization.static_const_fold import FoldStaticConstReads
from passes.optimization.constant_folding import ConstantFold
from passes.optimization.strength_reduction import STRENGTH_RULES
from passes.optimization.cmp_zero_jump_fold import CMP_ZERO_RULE
from passes.optimization.and_zero_jump_fold import AND_ZERO_RULE
from passes.optimization.lnot_jump_fold import LNOT_RULE
from passes.optimization.dead_loop_elimination import EliminateDeadLoops
from passes.optimization.unreachable_code_elimination import EliminateUnreachableCode
from passes.optimization.copy_propagation import CopyPropagate
from passes.optimization.dead_store_elimination import EliminateDeadStores
from passes.optimization.copy_folding import (
    FoldCopiesInFixedpoint,
    FoldCopiesPostDestruction,
)
from passes.optimization.reassoc_const import ReassocConstants
from passes.optimization.recognize_indexed_store import RecognizeIndexedStore
from passes.optimization.recognize_indexed_load import RecognizeIndexedLoad
from passes.optimization.truncate_extend_fold import FoldTruncateExtend
from passes.optimization.sink_increment import SinkIncrements
from passes.optimization.sink_and_past_branch import SinkAndPastBranch
from passes.optimization.narrow_widened_arith import NarrowWidenedArith
from passes.optimization.recognize_indirect_indexed import RecognizeIndirectIndexed
from passes.optimization.short_circuit_jump_fold import FoldShortCircuitJump


# Module-level driver instance, reused across all optimize_function calls.
_DRIVER = PhaseDriver(
    pre_ssa=[
        # Pre-SSA: rotate signed-countdown for-loops to test-at-bottom
        # shape. Operates on the canonical c99_to_tac for-loop layout
        # where x_var carries one name across init, body, and post.
        # After this, `to_ssa` rebuilds Phis for the rotated control flow.
        RotateSignedCountdownLoops(),
    ],
    pre_fixedpoint=[
        # One-shot: replace `Var(static_const_scalar)` USE-position
        # operands with `Constant(value)` so the fixed-point loop's
        # constant_fold can collapse downstream arithmetic. SSA
        # construction has already finished, so the substitution
        # doesn't disturb def/use chains (statics aren't promoted
        # in any case).
        FoldStaticConstReads(),
    ],
    fixedpoint=[
        ConstantFold(),
        # Strength reduction split into per-op rules (Multiply / Divide
        # / Modulo by powers of two) in one RuleSet.
        RuleSet(*STRENGTH_RULES, name="reduce_strength"),
        # Three disjoint jump-folds — comparison / BitwiseAnd /
        # LogicalNot producers each feeding a single-use JumpIf —
        # merged into one RuleSet. They share the producer+JumpIf
        # window and the single-use gate, and their producer opcodes
        # are disjoint, so one sweep applies whichever rule matches
        # each position instead of three separate sweeps.
        RuleSet(CMP_ZERO_RULE, AND_ZERO_RULE, LNOT_RULE,
                name="fold_producer_jumps"),
        EliminateDeadLoops(),
        EliminateUnreachableCode(),
        CopyPropagate(),
        EliminateDeadStores(),
        FoldCopiesInFixedpoint(),
        ReassocConstants(),
        RecognizeIndexedStore(),
        RecognizeIndexedLoad(),
        FoldTruncateExtend(),
        SinkIncrements(),
        SinkAndPastBranch(),
        # Run AFTER sink_and_past_branch so the latter sees the
        # canonical wide `ZeroExtend + BitwiseAnd + Truncate +
        # JumpIfMasked` trio it pattern-matches. The narrow form
        # this pass produces (Binary(BitwiseAnd, %x, ConstUChar(C),
        # %t)) wouldn't match sink's strict shape, which would
        # force the AND result to live across the branch and spill
        # to memory.
        NarrowWidenedArith(),
    ],
    post_fixedpoint=[
        # Run the indirect-indexed recognizer AFTER the fixed-point
        # loop has converged on constant folding. If we ran it inside
        # the loop, it could prematurely lock in an IndirectIndexed
        # form for an address chain whose pointer side is going to
        # fold to a Constant on the next iteration — preempting the
        # cheaper `recognize_indexed_store` lowering. Running it last
        # guarantees: every chain that COULD become absolute,X already
        # has (via recognize_indexed_store); only the genuine
        # runtime-pointer cases (zp_abi params, address-taken pointer
        # locals) remain for the (zp),Y lowering.
        RecognizeIndirectIndexed(),
    ],
    post_destruction=[
        # Post-from_ssa copy folding. SSA destruction emits a Copy at
        # the end of each predecessor block to feed each Phi's source
        # into the Phi's dst. For a loop-counter `i++`, that pattern
        # looks like `Binary(Add, i.vK, 1, %t); Copy(%t, i.vJ)` at the
        # end of the loop's continue block — which the fold pass
        # collapses to in-place `Binary(Add, i.vK, 1, i.vJ)`. Doing
        # this once post-destruction (rather than re-running the full
        # fixed-point loop) is enough because nothing later in the TAC
        # pipeline produces fresh fusable patterns.
        FoldCopiesPostDestruction(),
    ],
    post_destruction_fixedpoint=[
        # Post-destruction short-circuit fold: the `&&` / `||` 0-or-1
        # materialize tail + adjacent JumpIf consumer collapses to
        # direct conditional branches. Pre-destruction the tail is
        # split across two SSA-renamed defs of `%t` merged by a Phi;
        # post-destruction (and after the fold_copies above) it's the
        # canonical 5-instruction tail this pass matches. Loop until
        # convergence so nested patterns (`(a && b) || c` and similar)
        # peel off one short-circuit at a time.
        FoldShortCircuitJump(),
    ],
)


def optimize_program(
    prog: tac_ast.Program, symbols=None,
) -> tac_ast.Program:
    """Optimize each `Function` top-level in `prog`. `StaticVariable`
    top-levels pass through unchanged. `symbols` is the type
    checker's `SymbolTable`, threaded into per-pass calls that need
    it (constant folding for cast-node folds, SSA construction for
    fresh-name typing)."""
    new_top: list[tac_ast.Type_top_level] = []
    for t in prog.top_level:
        if isinstance(t, tac_ast.Function):
            new_top.append(optimize_function(t, symbols=symbols))
        else:
            new_top.append(t)
    return tac_ast.Program(top_level=new_top)


def optimize_function(
    fn: tac_ast.Function, *, symbols=None,
) -> tac_ast.Function:
    """SSA-in → fixed-point cycle → SSA-out. Without `symbols`, skip
    SSA conversion (the renaming pass needs the symbol table to
    register fresh SSA names with their types); the SSA-aware
    passes (copy propagation, dead-store elimination) become no-ops
    in that mode.

    Returns the optimized function."""
    ctx = PassContext(symbols=symbols)
    return _DRIVER.apply(fn, ctx)
