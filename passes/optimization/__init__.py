"""TAC-level optimization passes plus a fixed-point driver.

Public API:
  optimize_program(prog) — run the optimizer on every Function in a
                           tac_ast.Program. StaticVariable top-levels
                           pass through unchanged.
  optimize_function(fn)  — run the optimizer on a single tac_ast.Function.

The optimizer chains four passes per iteration (constant folding,
unreachable-code elimination, copy propagation, dead-store elimination),
re-running the cycle until the function's instruction list stops
changing. Each individual pass is a pure function on tac_ast.Function;
modules in this package own one pass each and are otherwise independent
of the driver.
"""

from passes.optimization.optimizer import (
    optimize_function,
    optimize_program,
)

__all__ = ["optimize_function", "optimize_program"]
