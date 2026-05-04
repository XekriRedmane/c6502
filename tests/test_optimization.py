"""Tests for the TAC-level optimizer driver (`passes.optimization`).

The driver wraps SSA-in / fixed-point cycle / SSA-out around a TAC
function. Per-pass behavioral tests live in their own modules
(`test_constant_folding.py`, `test_unreachable_code_elimination.py`,
`test_ssa.py`, `test_strength_reduction.py`,
`test_cmp_zero_jump_fold.py`, etc.). What this file pins:

  - the per-pass entry points accept arbitrary well-formed input
    without crashing;
  - the driver's fixed-point loop terminates (synthetic functions
    converge in zero or one iteration);
  - the program-level dispatch (Function entries get optimized,
    StaticVariable entries pass through);
  - end-to-end CLI plumbing (`--optimize` doesn't break --tac /
    --codegen).

Coloring decisions live entirely in the asm-level pipeline now —
this driver does NOT perform register allocation. See
`tests/test_asm_regalloc.py` and friends for coloring tests.
"""

from __future__ import annotations

import io
import unittest
from unittest.mock import patch

import tac_ast
from compile import main as compile_main
from passes.optimization import optimize_function, optimize_program
from passes.optimization.constant_folding import constant_fold
from passes.optimization.copy_propagation import copy_propagate
from passes.optimization.dead_store_elimination import (
    eliminate_dead_stores,
)
from passes.optimization.unreachable_code_elimination import (
    eliminate_unreachable_code,
)


def _ret(v: int = 0) -> tac_ast.Ret:
    return tac_ast.Ret(val=tac_ast.Constant(const=tac_ast.ConstInt(value=v)))


def _fn(*instrs, name: str = "main", params=()) -> tac_ast.Function:
    return tac_ast.Function(
        name=name,
        is_global=True,
        params=list(params),
        instructions=list(instrs),
    )


class TestPassesAcceptInputs(unittest.TestCase):
    """Each pass at minimum must accept arbitrary well-formed input
    without crashing. Behavioral correctness for each pass lives in
    its own test module."""

    def setUp(self) -> None:
        self.fn = _fn(
            tac_ast.Copy(
                src=tac_ast.Constant(const=tac_ast.ConstInt(value=1)),
                dst=tac_ast.Var(name="x"),
            ),
            tac_ast.Binary(
                op=tac_ast.Add(),
                src1=tac_ast.Var(name="x"),
                src2=tac_ast.Constant(const=tac_ast.ConstInt(value=2)),
                dst=tac_ast.Var(name="y"),
            ),
            _ret(),
        )

    def test_constant_folding_runs(self) -> None:
        out = constant_fold(self.fn)
        self.assertIsInstance(out, tac_ast.Function)

    def test_unreachable_code_runs(self) -> None:
        out = eliminate_unreachable_code(self.fn)
        self.assertIsInstance(out, tac_ast.Function)

    def test_copy_propagation_runs(self) -> None:
        out = copy_propagate(self.fn)
        self.assertIsInstance(out, tac_ast.Function)

    def test_dead_store_runs(self) -> None:
        out = eliminate_dead_stores(self.fn)
        self.assertIsInstance(out, tac_ast.Function)


class TestOptimizeFunction(unittest.TestCase):
    """Driver invariants — `optimize_function` terminates and the
    SSA-in/de-SSA bracket only kicks in when a SymbolTable is
    supplied (so legacy callers without one see the simple cycle)."""

    def test_terminates_on_empty_function(self) -> None:
        fn = _fn(_ret())
        out = optimize_function(fn)
        self.assertEqual(out, fn)

    def test_terminates_on_function_with_body(self) -> None:
        # Without a symbol table, SSA construction is skipped and
        # the SSA-aware passes (copy propagation, dead-store
        # elimination) become no-ops, since they have no safe way
        # to identify which Vars are SSA single-def. So the
        # function passes through structurally unchanged here —
        # what we're pinning is just that the driver's loop
        # terminates without exploding.
        fn = _fn(
            tac_ast.Copy(
                src=tac_ast.Constant(const=tac_ast.ConstInt(value=7)),
                dst=tac_ast.Var(name="t0"),
            ),
            tac_ast.Ret(val=tac_ast.Var(name="t0")),
        )
        out = optimize_function(fn)
        self.assertEqual(out, fn)


class TestOptimizeProgram(unittest.TestCase):
    """Top-level dispatch: Functions go through the per-function
    optimizer, StaticVariables pass through unchanged."""

    def test_function_and_static_round_trip(self) -> None:
        prog = tac_ast.Program(top_level=[
            tac_ast.StaticVariable(
                name="g",
                is_global=True,
                data_type=tac_ast.Int(),
                init=[tac_ast.IntInit(value=5)],
            ),
            _fn(_ret()),
        ])
        out = optimize_program(prog)
        self.assertEqual(out, prog)

    def test_static_only_program_passes_through(self) -> None:
        prog = tac_ast.Program(top_level=[
            tac_ast.StaticVariable(
                name="g",
                is_global=True,
                data_type=tac_ast.Long(),
                init=[tac_ast.LongInit(value=0)],
            ),
        ])
        out = optimize_program(prog)
        self.assertEqual(out, prog)

    def test_empty_program_passes_through(self) -> None:
        prog = tac_ast.Program(top_level=[])
        out = optimize_program(prog)
        self.assertEqual(out, prog)


class TestCliFlag(unittest.TestCase):
    """`--optimize` is orthogonal to --tac / --codegen — it changes
    the optimization level but the same stages still run."""

    SOURCE = "int main(void) { return 42; }"

    def _run(self, argv: list[str], stdin: str = "") -> tuple[int, str, str]:
        with patch("sys.stdin", io.StringIO(stdin)), \
             patch("sys.stdout", new_callable=io.StringIO) as out, \
             patch("sys.stderr", new_callable=io.StringIO) as err:
            rc = compile_main(argv)
        return rc, out.getvalue(), err.getvalue()

    def test_tac_with_optimize_compiles(self) -> None:
        # `return 42` is a trivial constant; the optimized TAC
        # may or may not match the unoptimized form (constant
        # folding could collapse it differently). Just verify
        # both invocations succeed.
        rc1, _, _ = self._run(
            ["compile.py", "-", "--tac"], stdin=self.SOURCE,
        )
        rc2, _, _ = self._run(
            ["compile.py", "-", "--tac", "--optimize"], stdin=self.SOURCE,
        )
        self.assertEqual(rc1, 0)
        self.assertEqual(rc2, 0)

    def test_codegen_with_optimize_compiles(self) -> None:
        rc1, _, _ = self._run(
            ["compile.py", "-", "--codegen"], stdin=self.SOURCE,
        )
        rc2, _, _ = self._run(
            ["compile.py", "-", "--codegen", "--optimize"],
            stdin=self.SOURCE,
        )
        self.assertEqual(rc1, 0)
        self.assertEqual(rc2, 0)


if __name__ == "__main__":
    unittest.main()
