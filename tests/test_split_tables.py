"""Tests for passes/optimization/split_tables.py, which stores a table
of 2-byte (or wider) elements read by a byte index as its low bytes
and then its high bytes."""

from __future__ import annotations

import shutil
import unittest

from compile import _run_source
from sim.harness import build_sim

_HAVE_PCPP = shutil.which("pcpp") is not None


def _codegen(src: str) -> str:
    return _run_source("codegen", src, optimize=True)


@unittest.skipUnless(_HAVE_PCPP, "pcpp CLI not available")
class TestSplitTablesSim(unittest.TestCase):
    """Programs that read split tables, run in both pipelines."""

    def _check(self, src: str, expected: int) -> None:
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                got = build_sim(src, optimize=optimize).run().return_int()
                self.assertEqual(got, expected & 0xFFFF)

    def test_uint16_table(self):
        src = """
        #include <stdint.h>
        static const uint16_t T[8] = {
            0x0102, 0x0304, 0x1000, 0x2000, 0x1234, 0xFF00, 7, 0x8001,
        };
        uint16_t get(uint8_t i) { return T[i]; }
        uint16_t next(uint8_t i) { return T[i + 1]; }
        int main(void) {
            uint16_t s = T[4];
            for (uint8_t i = 0; i < 8; i++) s += get(i);
            return s + next(6) + next(0);
        }
        """
        values = [0x0102, 0x0304, 0x1000, 0x2000, 0x1234, 0xFF00, 7, 0x8001]
        self._check(src, 0x1234 + sum(values) + 0x8001 + 0x0304)
        body = _codegen(src).split("\nget:\n")[1].split("RTS")[0]
        self.assertIn("T+8,X", body)
        self.assertNotIn("ASL", body)

    def test_long_table(self):
        # 200 elements, 400 bytes: the 16-bit address path.
        src = """
        #include <stdint.h>
        static const uint16_t T[200] = {11, 0x1234, 0xBEEF};
        uint16_t get(uint8_t i) { return T[i]; }
        int main(void) { return get(0) + get(1) + get(2) + get(199); }
        """
        self._check(src, 11 + 0x1234 + 0xBEEF)
        body = _codegen(src).split("\nget:\n")[1].split("RTS")[0]
        self.assertIn("T+200,X", body)

    def test_pointer_table(self):
        # Nine pointers, one more than the dispatch on a small
        # pointer table takes.
        src = """
        #include <stdint.h>
        static const uint8_t a[2] = {1, 2}, b[2] = {3, 4}, c[2] = {5, 6};
        static const uint8_t *const P[9] = {a, b, c, a + 1, b + 1, c + 1,
                                            a, b, c};
        uint8_t get(uint8_t i, uint8_t j) { return P[i][j]; }
        int main(void) {
            uint8_t s = 0;
            for (uint8_t i = 0; i < 9; i++) s += get(i, 0) * (i + 1);
            return s + get(2, 1);
        }
        """
        firsts = [1, 3, 5, 2, 4, 6, 1, 3, 5]
        self._check(src, sum(v * (k + 1) for k, v in enumerate(firsts)) + 6)
        self.assertIn("DC.B  <(a+1)", _codegen(src))

    def test_uint32_table(self):
        src = """
        #include <stdint.h>
        static const uint32_t T[3] = {0x01020304, 0x0A0B0C0D, 0xFFFFFFFF};
        uint32_t get(uint8_t i) { return T[i]; }
        int main(void) {
            return (int)(get(1) >> 16) + (int)(get(0) & 0xFFFF)
                + (int)(get(2) >> 24);
        }
        """
        self._check(src, 0x0A0B + 0x0304 + 0xFF)
        self.assertIn("T+6,X", _codegen(src))


if __name__ == "__main__":
    unittest.main()
