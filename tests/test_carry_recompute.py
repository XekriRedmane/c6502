"""Tests for cpp/src/passes/carry_recompute.h: a shift that only sets C to what
C already holds goes, and for the lowering of adds and subtracts of a
constant whose low bytes are zero (`tac_to_asm._translate_add_sub`).

The tests that called the Python functions directly are doctest cases of
the C++ port now, each named for the test it came from
(docs/cpp_port.md, step 8, group G6)."""

import unittest

from compile import _run_source
from sim.harness import build_sim
from tests.cpp_build import needs_c6502cpp


@needs_c6502cpp
class TestZeroLowBytes(unittest.TestCase):

    def test_page_steps(self):
        src = """
        unsigned int p;
        unsigned long q;
        unsigned int r;
        void up(void) { p += 0x100; }
        void down(void) { p -= 0x100; }
        void far(void) { q += 0x20000; }
        void copy(void) { r = p + 0x300; }
        int main(void) {
            p = 0x12FF; up(); up(); down();
            q = 0x1FFFF; far();
            copy();
            p = 0x0050; down();
            return (int)(p ^ (unsigned)(q >> 16) ^ r);
        }
        """
        p = (0x12FF + 0x100) & 0xFFFF
        q = (0x1FFFF + 0x20000) & 0xFFFFFFFF
        r = (p + 0x300) & 0xFFFF
        p = (0x0050 - 0x100) & 0xFFFF
        want = (p ^ (q >> 16) ^ r) & 0xFFFF
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                got = build_sim(src, optimize=optimize).run().return_int()
                self.assertEqual(got, want)
        asm = _run_source("codegen", src, optimize=True)
        self.assertIn("INC   p+1", asm.split("\nup:\n")[1].split("RTS")[0])
        self.assertIn("DEC   p+1", asm.split("\ndown:\n")[1].split("RTS")[0])


if __name__ == "__main__":
    unittest.main()
