"""Tests for moving a constant out of an address offset
(`passes/optimization/reassoc_offset.py`) and for hoisting loop-
invariant extends (`passes/optimization/licm.py`)."""
from __future__ import annotations

import shutil
import unittest

from sim.harness import build_sim


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestReassocLicmSim(unittest.TestCase):

    def _both(self, src: str, want: int) -> None:
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                got = build_sim(src, optimize=optimize).run().return_int()
                self.assertEqual(got, want)

    def test_header_fields_after_variable_skip(self):
        src = (
            "unsigned char t[12] = {2, 9, 9, 7, 1, 4, 0, 0, 0, 0, 0, 0};\n"
            "int f(unsigned char *d) {\n"
            "    unsigned char n = d[0];\n"
            "    return d[n + 1] * 100 + d[n + 2] * 10 + d[n + 3];\n"
            "}\n"
            "int main(void) { return f(t); }\n"
        )
        self._both(src, 714)

    def test_pointer_advanced_by_byte_in_loop(self):
        src = (
            "unsigned char t[40];\n"
            "int f(unsigned char *p, unsigned char step, unsigned char n) {\n"
            "    int sum = 0;\n"
            "    while (n--) { sum += *p; p += step; }\n"
            "    return sum;\n"
            "}\n"
            "int main(void) {\n"
            "    for (unsigned char i = 0; i < 40; i++) t[i] = i;\n"
            "    return f(t + 1, 3, 12);\n"
            "}\n"
        )
        # 1 + 4 + ... + 34, twelve terms.
        self._both(src, sum(1 + 3 * k for k in range(12)))


if __name__ == "__main__":
    unittest.main()
