"""TAC optimizer driver.

Runs four TAC-level passes per cycle on each function — constant
folding, unreachable-code elimination, copy propagation, dead-store
elimination — and repeats the cycle until the function's instruction
list is unchanged from the start of the cycle. Each cycle is one full
sweep of all four passes; we don't probe for changes between passes,
since a pass that's already at fixed point is cheap to re-run and the
between-pass interleaving is part of the optimizer's contract.

Termination: each pass is a pure function on tac_ast.Function, and
dataclass `__eq__` compares structurally — so the loop exits as soon
as no pass in a cycle made a structural change.

Per-program shape: only `Function` top-levels get optimized;
`StaticVariable` entries pass through unchanged (their `init` is a
constant byte layout, not control flow).
"""

from __future__ import annotations

import tac_ast
from passes.optimization.constant_folding import constant_fold
from passes.optimization.copy_propagation import copy_propagate
from passes.optimization.dead_store_elimination import (
    eliminate_dead_stores,
)
from passes.optimization.unreachable_code_elimination import (
    eliminate_unreachable_code,
)


def optimize_program(
    prog: tac_ast.Program, symbols=None,
) -> tac_ast.Program:
    """Optimize each function in `prog`. StaticVariable top-levels
    pass through unchanged. `symbols` is the type checker's
    SymbolTable, threaded into per-pass calls that need it (today
    only constant folding, for the cast-node folds — see that
    module's docstring)."""
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
    """Run the four-pass cycle on `fn` to a fixed point."""
    while True:
        prev = fn
        fn = constant_fold(fn, symbols=symbols)
        fn = eliminate_unreachable_code(fn)
        fn = copy_propagate(fn)
        fn = eliminate_dead_stores(fn)
        if fn == prev:
            return fn
