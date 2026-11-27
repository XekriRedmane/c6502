"""Tests for cpp/src/passes/branch_through_jump.h: a branch or `JMP` to a label
that holds only a `JMP` goes straight to that `JMP`'s target.

The tests that called the Python functions directly are doctest cases of
the C++ port now, each named for the test it came from
(docs/cpp_port.md, step 8, group G6).

The `-S` text is checked in cpp/tests/test_branch_through_jump.cpp."""
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

    def test_runs(self) -> None:
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                result = build_sim(self._SRC, optimize=optimize).run()
                self.assertEqual(result.return_int(), 5)


if __name__ == "__main__":
    unittest.main()
