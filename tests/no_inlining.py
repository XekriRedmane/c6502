"""Turns off the inlining of small `static` functions
(passes/optimization/inline_functions.py) for tests that check a
function's own code, its calling convention, or what a call graph
records about it. Such a test's `static` helper would otherwise be
copied into its caller and dropped.

`no_inlining()` patches `compile.inline_functions`, which
`compile.translate_for_codegen` calls for the CLI, `_run_stage`, the
link helpers, and the simulator harness, so it covers every compile
made in the test's process. It works as a decorator of a test class
or method, or as a context manager."""

from unittest import mock


def _leave_alone(tac, symbols, prog):
    return tac


def no_inlining():
    """A patcher that makes `compile.inline_functions` return the
    program unchanged."""
    return mock.patch("compile.inline_functions", new=_leave_alone)
