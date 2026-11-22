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

__all__ = ["optimize_function", "optimize_program"]


def __getattr__(name: str):
    """Returns `optimize_function` or `optimize_program` from the
    optimizer, which is imported on first use. A module that imports
    only one of this package's own modules, as passes/abi_selection.py
    imports `pool`, then loads neither the optimizer nor the type
    checker it reads, which the assembler's reading of the link
    metadata would otherwise import for every `-c`."""
    if name in __all__:
        from passes.optimization import optimizer
        return getattr(optimizer, name)
    raise AttributeError(f"module {__name__!r} has no attribute {name!r}")
