"""Tests for the return-tail merge (`cpp/src/passes/tail_merge.h`).

The tests that called the Python functions directly are doctest cases of
the C++ port now, each named for the test it came from
(docs/cpp_port.md, step 8, group G6)."""

import unittest

from sim.harness import build_sim
from tests.cpp_build import needs_c6502cpp


@needs_c6502cpp
class TestTailMergeSim(unittest.TestCase):

    def test_early_returns(self):
        src = (
            "unsigned int f(unsigned char a, unsigned char b) {\n"
            "    if (a < 3) return 0;\n"
            "    if (b < 3) return 0;\n"
            "    if (a == b) return 0;\n"
            "    return a + b;\n"
            "}\n"
            "int main(void) {\n"
            "    return f(1, 9) + f(9, 1) + f(5, 5) + f(4, 6) * 10;\n"
            "}\n"
        )
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                got = build_sim(src, optimize=optimize).run().return_int()
                self.assertEqual(got, 100)


if __name__ == "__main__":
    unittest.main()
