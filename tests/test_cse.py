"""Tests for common-subexpression elimination
(`cpp/src/passes/optimization/cse.h`).

The unit tests build SSA-form functions directly and check which
instructions become Copies. The sim tests compile C whose repeated
subscripts and tests go through the pass, in both pipelines.
"""
from __future__ import annotations

import unittest

from sim.harness import build_sim
from tests.cpp_build import needs_c6502cpp


@needs_c6502cpp
class TestCseSim(unittest.TestCase):

    def _both(self, src: str, want: int) -> None:
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                got = build_sim(src, optimize=optimize).run().return_int()
                self.assertEqual(got, want)

    def test_repeated_subscripts_and_tests(self) -> None:
        src = (
            "unsigned char t[8] = {9, 3, 3, 7, 1, 2, 5, 4};\n"
            "unsigned char *p = t;\n"
            "int cmp(unsigned char k, unsigned char n) {\n"
            "    if (k < p[n + 1]) return -1;\n"
            "    if (k == p[n + 1]) return p[n + 2] + p[n + 3];\n"
            "    return 1;\n"
            "}\n"
            "int main(void) {\n"
            "    return cmp(1, 0) * 100 + cmp(3, 0) + cmp(9, 4) * 1000;\n"
            "}\n"
        )
        # cmp(1, 0): 1 < 3, -1. cmp(3, 0): 3 + 7 = 10. cmp(9, 4):
        # 9 > 2, 1.
        self._both(src, -100 + 10 + 1000)

    def test_store_between_loads(self) -> None:
        src = (
            "unsigned char t[4] = {1, 2, 3, 4};\n"
            "int f(unsigned char *p) {\n"
            "    int a = p[1];\n"
            "    p[1] = 9;\n"
            "    return a * 10 + p[1];\n"
            "}\n"
            "int main(void) { return f(t); }\n"
        )
        self._both(src, 29)

    def test_doubles(self) -> None:
        src = (
            "int f(double d) {\n"
            "    if (d - 1 != 5) return 1;\n"
            "    if (d - 2 != 4) return 2;\n"
            "    return 0;\n"
            "}\n"
            "int main(void) { return f(6.0); }\n"
        )
        self._both(src, 0)


if __name__ == "__main__":
    unittest.main()
