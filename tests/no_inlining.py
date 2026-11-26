"""Turns off the inlining of small `static` functions
(passes/optimization/inline_functions.py) for tests that check a
function's own code, its calling convention, or what a call graph
records about it. Such a test's `static` helper would otherwise be
copied into its caller and dropped.

`no_inlining()` sets `compile.inline_small_functions` to False, which
compile.py's `_inlining` reads. Without `C6502_CPP_BACKEND=1`,
`translate_for_codegen` and `_run_stage` then skip the Python inliner,
and with it, every request that compile.py and the simulator harness
send for a compile carries the flag, so it covers every compile made in
the test's process, through the CLI's `main`, `_run_stage`, the link
helpers, and the harness. compile.py defines the flag before it imports
any of the compiler, so the patch works on the shim that compile.py
becomes once the Python compiler is removed. It works as a decorator
of a test class or method, or as a context manager."""

from unittest import mock


def no_inlining():
    """A patcher that sets `compile.inline_small_functions` to False."""
    return mock.patch("compile.inline_small_functions", new=False)
