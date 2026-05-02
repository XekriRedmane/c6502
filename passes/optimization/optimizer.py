"""TAC optimizer driver.

Wraps SSA-in / de-SSA around a fixed-point cycle of four TAC-level
passes — constant folding, unreachable-code elimination, copy
propagation, dead-store elimination. The cycle re-runs until the
function's instruction list is structurally unchanged from the
start of an iteration; each cycle sweeps all four passes regardless
of whether earlier passes converged, since a pass already at fixed
point is cheap to re-run and the between-pass interleaving is part
of the optimizer's contract.

Pipeline shape:
    fn → SSA construction → (CF → UCE → CopyProp → DSE)* → SSA destruction → fn'

Promotable Vars (block-scope locals, params, and TAC temps that are
never address-taken and have scalar type) are renamed and Phi'd
between SSA-in and de-SSA. Address-taken locals, statics, and
aggregates pass through unchanged. After de-SSA, every Phi has been
lowered to `Copy` instructions in predecessor blocks; the resulting
TAC is regular non-SSA form, ready for `tac_to_asm`.

The four cycle passes are SSA-aware:
  - constant folding folds a Phi whose every PhiArg.source agrees
    (same Constant, same Var) into a Copy from that source.
  - UCE prunes PhiArgs whose pred_label named a dropped block,
    folds singleton Phis to Copies, and treats Phi pred_labels as
    label uses so SSA destruction can later locate predecessors.
  - copy propagation and dead-store elimination are the SSA-aware
    versions (Milestone 2 — currently still stubs).

Termination: each pass is a pure function on tac_ast.Function, and
dataclass `__eq__` compares structurally — so the loop exits as
soon as no pass in a cycle made a structural change.

Per-program shape: only `Function` top-levels get optimized;
`StaticVariable` entries pass through unchanged (their `init` is a
constant byte layout, not control flow).

Calling `optimize_function` without `symbols` (e.g. legacy unit
tests that exercise the driver on synthetic Functions) skips SSA
construction entirely — the symbol table is required to register
fresh SSA names with their types, and we'd rather no-op than
silently emit untyped temporaries that downstream passes can't size.
"""

from __future__ import annotations

import tac_ast
from passes.optimization.constant_folding import constant_fold
from passes.optimization.copy_propagation import copy_propagate
from passes.optimization.dead_store_elimination import (
    eliminate_dead_stores,
)
from passes.optimization.ssa_construction import to_ssa
from passes.optimization.ssa_destruction import from_ssa
from passes.optimization.unreachable_code_elimination import (
    eliminate_unreachable_code,
)


def optimize_program(
    prog: tac_ast.Program, symbols=None,
) -> tac_ast.Program:
    """Optimize each function in `prog`. StaticVariable top-levels
    pass through unchanged. `symbols` is the type checker's
    SymbolTable, threaded into per-pass calls that need it (constant
    folding for cast-node folds, SSA construction for fresh-name
    typing)."""
    return tac_ast.Program(top_level=[
        _optimize_top_level(t, symbols) for t in prog.top_level
    ])


def _optimize_top_level(
    t: tac_ast.Type_top_level, symbols,
) -> tac_ast.Type_top_level:
    if isinstance(t, tac_ast.Function):
        return optimize_function(t, symbols=symbols)
    return t


def optimize_function(
    fn: tac_ast.Function, *, symbols=None,
) -> tac_ast.Function:
    """SSA-in → fixed-point cycle → de-SSA. Without `symbols`, skip
    SSA conversion (the renaming pass needs the symbol table to
    register fresh SSA names with their types); the SSA-aware passes
    (copy propagation, dead-store elimination) become no-ops in
    that mode since they have no safe way to identify SSA names."""
    ssa_dsts: set[str] | None = None
    if symbols is not None:
        fn, ssa_dsts = to_ssa(fn, symbols)
    while True:
        prev = fn
        fn = constant_fold(fn, symbols=symbols)
        fn = eliminate_unreachable_code(fn)
        fn = copy_propagate(fn, ssa_dsts=ssa_dsts)
        fn = eliminate_dead_stores(fn, ssa_dsts=ssa_dsts)
        if fn == prev:
            break
    if symbols is not None:
        fn = from_ssa(fn)
    return fn
