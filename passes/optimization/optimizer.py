"""TAC optimizer driver.

Wraps SSA-in / regalloc / de-SSA around a fixed-point cycle of four
TAC-level passes — constant folding, unreachable-code elimination,
copy propagation, dead-store elimination. The cycle re-runs until
the function's instruction list is structurally unchanged from the
start of an iteration; each cycle sweeps all four passes regardless
of whether earlier passes converged, since a pass already at fixed
point is cheap to re-run and the between-pass interleaving is part
of the optimizer's contract.

Pipeline shape:
    fn → SSA construction → (CF → UCE → CopyProp → DSE)*
       → register allocation (still in SSA form)
       → SSA destruction → fn'

Promotable Vars (block-scope locals, params, and TAC temps that are
never address-taken and have scalar type) are renamed and Phi'd
between SSA-in and de-SSA. Address-taken locals, statics, and
aggregates pass through unchanged.

Register allocation runs WHILE the function is in SSA form because
the chordal property of SSA interference graphs is what makes the
greedy coloring optimal. The resulting `Coloring` is returned
alongside the function and is consumed by `replace_pseudoregisters`
downstream — colored names lower to `ZP(addr, offset)` operands;
spilled / never-colored names continue to flow through the existing
Frame allocation. After de-SSA, every Phi has been lowered to
`Copy` instructions in predecessor blocks; the resulting TAC is
regular non-SSA form, ready for `tac_to_asm`.

The four cycle passes are SSA-aware:
  - constant folding folds a Phi whose every PhiArg.source agrees
    (same Constant, same Var) into a Copy from that source.
  - UCE prunes PhiArgs whose pred_label named a dropped block,
    folds singleton Phis to Copies, and treats Phi pred_labels as
    label uses so SSA destruction can later locate predecessors.
  - copy propagation and dead-store elimination are the SSA-aware
    versions.

Termination: each pass is a pure function on tac_ast.Function, and
dataclass `__eq__` compares structurally — so the loop exits as
soon as no pass in a cycle made a structural change.

Per-program shape: only `Function` top-levels get optimized;
`StaticVariable` entries pass through unchanged (their `init` is a
constant byte layout, not control flow). `optimize_program` returns
`(prog, colorings)` where `colorings: dict[str, Coloring]` keys per
optimized function name. `StaticVariable` top-levels are absent
from the dict.

Calling `optimize_function` without `symbols` (e.g. legacy unit
tests that exercise the driver on synthetic Functions) skips SSA
construction entirely — the symbol table is required to register
fresh SSA names with their types, and we'd rather no-op than
silently emit untyped temporaries that downstream passes can't size.
In that mode regalloc is also skipped; the returned Coloring is
`None`.
"""

from __future__ import annotations

import tac_ast
from passes.optimization.constant_folding import constant_fold
from passes.optimization.copy_propagation import copy_propagate
from passes.optimization.dead_store_elimination import (
    eliminate_dead_stores,
)
from passes.optimization.interference import build_interference
from passes.optimization.liveness import compute_liveness
from passes.optimization.register_allocation import Coloring, color_graph
from passes.optimization.ssa_construction import to_ssa
from passes.optimization.ssa_destruction import from_ssa
from passes.optimization.unreachable_code_elimination import (
    eliminate_unreachable_code,
)


def optimize_program(
    prog: tac_ast.Program, symbols=None, *,
    do_regalloc: bool = True,
) -> tuple[tac_ast.Program, dict[str, Coloring]]:
    """Optimize each function in `prog`. StaticVariable top-levels
    pass through unchanged. `symbols` is the type checker's
    SymbolTable, threaded into per-pass calls that need it (constant
    folding for cast-node folds, SSA construction for fresh-name
    typing).

    `do_regalloc=False` (the `--optimize-asm` path) runs the SSA
    fixed-point optimizations and `from_ssa` but skips register
    allocation — the asm-level pipeline does its own byte-granular
    regalloc later, so duplicating the work at TAC level is wasted.
    The returned `colorings` dict is empty in that mode.

    Returns `(optimized_program, colorings)` where `colorings` maps
    each optimized function's name to its `Coloring` (empty when
    `symbols=None` or `do_regalloc=False`). `StaticVariable`
    top-levels do not appear in the dict."""
    new_top: list[tac_ast.Type_top_level] = []
    colorings: dict[str, Coloring] = {}
    for t in prog.top_level:
        if isinstance(t, tac_ast.Function):
            new_fn, coloring = optimize_function(
                t, symbols=symbols, do_regalloc=do_regalloc,
            )
            new_top.append(new_fn)
            if coloring is not None:
                colorings[new_fn.name] = coloring
        else:
            new_top.append(t)
    return tac_ast.Program(top_level=new_top), colorings


def optimize_function(
    fn: tac_ast.Function, *, symbols=None,
    do_regalloc: bool = True,
) -> tuple[tac_ast.Function, Coloring | None]:
    """SSA-in → fixed-point cycle → register allocation → de-SSA.
    Without `symbols`, skip SSA conversion (the renaming pass needs
    the symbol table to register fresh SSA names with their types);
    the SSA-aware passes (copy propagation, dead-store elimination)
    become no-ops in that mode, and regalloc is skipped.

    `do_regalloc=False` runs SSA + fixed-point + from_ssa but skips
    register allocation. Returned coloring is `None` in that mode.

    Returns `(optimized_fn, coloring)` where `coloring` is the
    register-allocation result (or `None` when skipped)."""
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
    coloring: Coloring | None = None
    if symbols is not None:
        if do_regalloc:
            # Regalloc runs on the still-SSA function — chordal
            # interference graphs admit optimal greedy coloring in
            # dom-tree-PEO.
            liveness = compute_liveness(fn)
            graph = build_interference(fn, liveness, symbols)
            coloring = color_graph(fn, graph)
        # `from_ssa` runs whether or not we colored. Without regalloc
        # the function is just regular post-SSA TAC — Phis lowered
        # to Copies, no ZP coloring decisions yet.
        fn = from_ssa(fn, symbols=symbols)
    return fn, coloring
