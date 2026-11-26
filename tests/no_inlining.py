"""Turns off the inlining of small `static` functions
(cpp/src/passes/optimization/inline_functions.h) for tests that check a
function's own code, its calling convention, or what a call graph
records about it. Such a test's `static` helper would otherwise be
copied into its caller and dropped.

`no_inlining()` sets `compile.inline_small_functions` to False, which
compile.py's `_inlining` reads. Every request that compile.py and the
simulator harness send to the C++ port for a compile carries the flag,
so it covers every compile made in the test's process, through the
CLI's `main`, `_run_stage`, `_run_source`, the link helpers, and the
harness. It works as a decorator of a test class or method, or as a
context manager."""

from unittest import mock


def no_inlining():
    """A patcher that sets `compile.inline_small_functions` to False."""
    return mock.patch("compile.inline_small_functions", new=False)
