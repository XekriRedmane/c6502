"""`short`, `unsigned short`, and unary `+`: parsing, type checking,
and programs run through the TAC simulator and the asm simulator
with and without `-O`."""

from __future__ import annotations

import unittest

from sim.harness import build_sim
from tests.test_sim_chapters import _run_program as _run_tac
from tests.cpp_build import needs_c6502cpp


# (name, C source, expected int return of main)
_PROGRAMS: list[tuple[str, str, int]] = [
    ("unary_plus", """
        unsigned char c = 200;
        signed char s = -5;
        int n = +7;
        double d = +1.5;
        int arr[+4];
        int main(void) {
            int a = +c;
            int b = +s;
            long l = +(long)s;
            switch (a) { case +200: a = a + 1; }
            return a + b + (int)l + n + 3 + (int)sizeof(arr)
                   + (d > 1.0) + sizeof(+c);
        }
    """, 212),
    ("short", """
        short s = -3;
        unsigned short us = 65535;
        short arr[3] = {1, -2, 30000};
        struct P { short a; unsigned short b; };
        struct P p = {-7, 40000};
        short twice(short x) { return x * 2; }
        unsigned short umax(void) { return us; }
        int main(void) {
            short int a = 100;
            signed short b = -50;
            unsigned short int c = 60000;
            long r = 0;
            r += a + b;
            r += (us + 1 == 0);
            r += (c > 0);
            r += sizeof(short) + sizeof(unsigned short) + sizeof s;
            r += twice(arr[2]) == -5536;
            r += p.a + (p.b == 40000);
            r += (unsigned short)70000;
            r += umax() == 65535;
            r += s;
            for (short i = 0; i < 3; i++) r += arr[i];
            return (int)(r & 0x7FFF);
        }
    """, 1746),
    ("unsigned_short_compare", """
        unsigned short big = 40000;
        int main(void) { return (big > 30000) + (big > -1) * 2; }
    """, 1),
]


@needs_c6502cpp
class TestPrograms(unittest.TestCase):

    def test_tac_sim(self) -> None:
        for name, src, expected in _PROGRAMS:
            with self.subTest(case=name):
                v = _run_tac(src) & 0xFFFF
                self.assertEqual(v - 0x10000 if v & 0x8000 else v, expected)

    def test_asm_sim(self) -> None:
        for name, src, expected in _PROGRAMS:
            for optimize in (False, True):
                with self.subTest(case=name, optimize=optimize):
                    result = build_sim(src, optimize=optimize).run()
                    self.assertFalse(result.timed_out)
                    self.assertEqual(result.return_int_signed(), expected)


if __name__ == "__main__":
    unittest.main()
