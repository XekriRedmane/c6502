"""TAC optimizer driver.

Wraps SSA-in / fixed-point cycle / SSA-out around the function. The
fixed-point cycle re-runs until the function's instruction list is
structurally unchanged from the start of an iteration; each cycle
sweeps every pass regardless of whether earlier passes converged,
since a pass already at fixed point is cheap to re-run and the
between-pass interleaving is part of the optimizer's contract.

Pipeline shape:
    fn → loop_rotate (one-shot, pre-SSA)
       → SSA construction
       → (CF → strength_reduce → cmp_zero_jump_fold → and_zero_jump_fold →
          dead_loop_elim → UCE → CopyProp → DSE → CopyFold → ...)*
       → SSA destruction → fn'

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
from passes.optimization.and_zero_jump_fold import fold_narrow_and_jump
from passes.optimization.cmp_zero_jump_fold import fold_cmp_zero_jump
from passes.optimization.constant_folding import constant_fold
from passes.optimization.copy_folding import fold_copies
from passes.optimization.copy_propagation import copy_propagate
from passes.optimization.dead_loop_elimination import (
    eliminate_dead_loops,
)
from passes.optimization.dead_store_elimination import (
    eliminate_dead_stores,
)
from passes.optimization.loop_rotate import (
    rotate_signed_countdown_loops,
)
from passes.optimization.reassoc_const import reassoc_constants
from passes.optimization.recognize_indexed_load import (
    recognize_indexed_load,
)
from passes.optimization.recognize_indexed_store import (
    recognize_indexed_store,
)
from passes.optimization.recognize_indirect_indexed import (
    recognize_indirect_indexed,
)
from passes.optimization.sink_increment import sink_increments
from passes.optimization.ssa_construction import to_ssa
from passes.optimization.ssa_destruction import from_ssa
from passes.optimization.static_const_fold import (
    fold_static_const_reads,
)
from passes.optimization.strength_reduction import reduce_strength
from passes.optimization.unreachable_code_elimination import (
    eliminate_unreachable_code,
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
    ssa_dsts: set[str] | None = None
    if symbols is not None:
        # Pre-SSA: rotate signed-countdown for-loops to test-at-
        # bottom shape. Operates on the canonical c99_to_tac for-
        # loop layout where x_var carries one name across init,
        # body, and post. After this, `to_ssa` rebuilds Phis for
        # the rotated control flow.
        fn = rotate_signed_countdown_loops(fn, symbols)
        fn, ssa_dsts = to_ssa(fn, symbols)
        # One-shot: replace `Var(static_const_scalar)` USE-position
        # operands with `Constant(value)` so the fixed-point loop's
        # constant_fold can collapse downstream arithmetic. SSA
        # construction has already finished, so the substitution
        # doesn't disturb def/use chains (statics aren't promoted
        # in any case).
        fn = fold_static_const_reads(fn, symbols)
    while True:
        prev = fn
        fn = constant_fold(fn, symbols=symbols)
        fn = reduce_strength(fn, symbols=symbols)
        fn = fold_cmp_zero_jump(fn, symbols=symbols)
        fn = fold_narrow_and_jump(fn, symbols=symbols)
        fn = eliminate_dead_loops(fn, ssa_dsts=ssa_dsts)
        fn = eliminate_unreachable_code(fn)
        fn = copy_propagate(fn, ssa_dsts=ssa_dsts)
        fn = eliminate_dead_stores(fn, ssa_dsts=ssa_dsts)
        fn = fold_copies(fn)
        fn = reassoc_constants(fn)
        fn = recognize_indexed_store(fn, symbols=symbols)
        fn = recognize_indexed_load(fn, symbols=symbols)
        fn = sink_increments(fn)
        if fn == prev:
            break
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
    if symbols is not None:
        fn = recognize_indirect_indexed(fn, symbols=symbols)
    if symbols is not None:
        fn = from_ssa(fn, symbols=symbols)
    # Post-from_ssa copy folding. SSA destruction emits a Copy at
    # the end of each predecessor block to feed each Phi's source
    # into the Phi's dst. For a loop-counter `i++`, that pattern
    # looks like `Binary(Add, i.vK, 1, %t); Copy(%t, i.vJ)` at the
    # end of the loop's continue block — which the fold pass
    # collapses to in-place `Binary(Add, i.vK, 1, i.vJ)`. Doing
    # this once post-destruction (rather than re-running the full
    # fixed-point loop) is enough because nothing later in the TAC
    # pipeline produces fresh fusable patterns.
    fn = fold_copies(fn)
    return fn
