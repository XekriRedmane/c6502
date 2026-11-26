"""Tests for `passes.branch_through_jump`: a branch or `JMP` to a label
that holds only a `JMP` goes straight to that `JMP`'s target.

The tests that called the Python functions directly are doctest cases of
the C++ port now, each named for the test it came from
(docs/cpp_port.md, step 8, group G6)."""
from __future__ import annotations

import unittest

from sim.harness import build_sim


class TestGotoBackwards(unittest.TestCase):
    _SRC = """
        int main(void) {
            if (0)
            label:
                return 5;
            goto label;
            return 0;
        }
    """

    def test_optimized_is_straight_line(self) -> None:
        from compile import _run_source
        asm = _run_source("codegen", self._SRC, optimize=True)
        body = asm[asm.index("main:"):]
        self.assertNotIn("JMP", body)
        self.assertRegex(body, r"LDX\s+#\$00\s+LDA\s+#\$05\s+RTS")

    def test_runs(self) -> None:
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                result = build_sim(self._SRC, optimize=optimize).run()
                self.assertEqual(result.return_int(), 5)


if __name__ == "__main__":
    unittest.main()
