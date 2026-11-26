"""End-to-end tests of the TAC-level optimizer (cpp/src/passes/optimization/optimizer.h)
through the command line: `-O` doesn't break --tac / -S.

The tests of the driver on synthetic functions (the per-pass entry
points accept well-formed input, the fixed-point loop terminates, and
the program-level dispatch optimizes each Function and passes each
StaticVariable through) became doctest cases of the C++ port in step 8
of the port (cpp/tests/test_optimization_calls.cpp).

Coloring decisions live entirely in the asm-level pipeline now —
this driver does NOT perform register allocation. See
`tests/test_asm_regalloc.py` and friends for coloring tests.
"""

from __future__ import annotations

import io
import unittest
from unittest.mock import patch

from compile import main as compile_main


class TestCliFlag(unittest.TestCase):
    """`-O` is orthogonal to --tac / -S — it changes
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
            ["compile.py", "-", "--tac", "-O"], stdin=self.SOURCE,
        )
        self.assertEqual(rc1, 0)
        self.assertEqual(rc2, 0)

    def test_codegen_with_optimize_compiles(self) -> None:
        rc1, _, _ = self._run(
            ["compile.py", "-", "-S"], stdin=self.SOURCE,
        )
        rc2, _, _ = self._run(
            ["compile.py", "-", "-S", "-O"],
            stdin=self.SOURCE,
        )
        self.assertEqual(rc1, 0)
        self.assertEqual(rc2, 0)


if __name__ == "__main__":
    unittest.main()
