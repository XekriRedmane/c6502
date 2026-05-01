"""Scaffolding tests for `passes.optimization`.

Each individual pass is a stub today (identity), so the tests here
focus on:
  - the per-pass identity contract (a stub returns its input
    structurally unchanged),
  - the driver's fixed-point loop (terminates immediately when no
    pass reports a change),
  - the program-level dispatch (Function entries get optimized,
    StaticVariable entries pass through),
  - end-to-end plumbing (`--optimize` doesn't break --tac / --codegen).

When real folding / propagation / DCE land, each pass module gets
its own behavioral test file; this module stays focused on the
driver's invariants.
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
    return tac_ast.Ret(val=tac_ast.Constant(const=tac_ast.ConstInt(int=v)))


def _fn(*instrs, name: str = "main", params=()) -> tac_ast.Function:
    return tac_ast.Function(
        name=name,
        is_global=True,
        params=list(params),
        instructions=list(instrs),
    )


class TestStubPassesAreIdentity(unittest.TestCase):
    """Every pass currently returns its input verbatim. These tests
    pin that contract so a future implementation that breaks it
    surfaces here rather than silently corrupting the AST."""

    def setUp(self) -> None:
        self.fn = _fn(
            tac_ast.Copy(
                src=tac_ast.Constant(const=tac_ast.ConstInt(int=1)),
                dst=tac_ast.Var(name="x"),
            ),
            tac_ast.Binary(
                op=tac_ast.Add(),
                src1=tac_ast.Var(name="x"),
                src2=tac_ast.Constant(const=tac_ast.ConstInt(int=2)),
                dst=tac_ast.Var(name="y"),
            ),
            _ret(),
        )

    def test_constant_folding_identity(self) -> None:
        self.assertEqual(constant_fold(self.fn), self.fn)

    def test_unreachable_code_identity(self) -> None:
        self.assertEqual(eliminate_unreachable_code(self.fn), self.fn)

    def test_copy_propagation_identity(self) -> None:
        self.assertEqual(copy_propagate(self.fn), self.fn)

    def test_dead_store_identity(self) -> None:
        self.assertEqual(eliminate_dead_stores(self.fn), self.fn)


class TestOptimizeFunction(unittest.TestCase):
    """Driver invariants. Stub passes mean one cycle is enough to
    reach fixed point — what we're really pinning is that the loop
    terminates and that the output matches the input under structural
    equality."""

    def test_terminates_on_empty_function(self) -> None:
        fn = _fn(_ret())
        self.assertEqual(optimize_function(fn), fn)

    def test_terminates_on_function_with_body(self) -> None:
        fn = _fn(
            tac_ast.Copy(
                src=tac_ast.Constant(const=tac_ast.ConstInt(int=7)),
                dst=tac_ast.Var(name="t0"),
            ),
            tac_ast.Ret(val=tac_ast.Var(name="t0")),
        )
        self.assertEqual(optimize_function(fn), fn)


class TestOptimizeProgram(unittest.TestCase):
    """Top-level dispatch: Functions go through the per-function
    optimizer, StaticVariables pass through unchanged."""

    def test_function_and_static_round_trip(self) -> None:
        prog = tac_ast.Program(top_level=[
            tac_ast.StaticVariable(
                name="g",
                is_global=True,
                data_type=tac_ast.Int(),
                init=[tac_ast.IntInit(int=5)],
            ),
            _fn(_ret()),
        ])
        self.assertEqual(optimize_program(prog), prog)

    def test_static_only_program_passes_through(self) -> None:
        prog = tac_ast.Program(top_level=[
            tac_ast.StaticVariable(
                name="g",
                is_global=True,
                data_type=tac_ast.Long(),
                init=[tac_ast.LongInit(int=0)],
            ),
        ])
        self.assertEqual(optimize_program(prog), prog)

    def test_empty_program_passes_through(self) -> None:
        prog = tac_ast.Program(top_level=[])
        self.assertEqual(optimize_program(prog), prog)


class TestCliFlag(unittest.TestCase):
    """`--optimize` is orthogonal to --tac / --codegen and shouldn't
    change observable output while every pass is a stub."""

    SOURCE = "int main(void) { return 42; }"

    def _run(self, argv: list[str], stdin: str = "") -> tuple[int, str, str]:
        with patch("sys.stdin", io.StringIO(stdin)), \
             patch("sys.stdout", new_callable=io.StringIO) as out, \
             patch("sys.stderr", new_callable=io.StringIO) as err:
            rc = compile_main(argv)
        return rc, out.getvalue(), err.getvalue()

    def test_tac_with_optimize_matches_unoptimized_for_stubs(self) -> None:
        rc1, plain, _ = self._run(
            ["compile.py", "-", "--tac"], stdin=self.SOURCE,
        )
        rc2, opt, _ = self._run(
            ["compile.py", "-", "--tac", "--optimize"], stdin=self.SOURCE,
        )
        self.assertEqual(rc1, 0)
        self.assertEqual(rc2, 0)
        self.assertEqual(plain, opt)

    def test_codegen_with_optimize_matches_unoptimized_for_stubs(self) -> None:
        rc1, plain, _ = self._run(
            ["compile.py", "-", "--codegen"], stdin=self.SOURCE,
        )
        rc2, opt, _ = self._run(
            ["compile.py", "-", "--codegen", "--optimize"],
            stdin=self.SOURCE,
        )
        self.assertEqual(rc1, 0)
        self.assertEqual(rc2, 0)
        self.assertEqual(plain, opt)


if __name__ == "__main__":
    unittest.main()
