"""Enumerations (C99 §6.7.2.2): the parser's enumeration constants
and enum tags, and programs run through the TAC simulator and the asm
simulator with and without `-O`.

The TAC interpreter's results are checked in cpp/tests/test_enum.cpp."""

from __future__ import annotations

import unittest

from sim.harness import build_sim
from tests.cpp_build import needs_c6502cpp


# (name, C source, expected int return of main)
_PROGRAMS: list[tuple[str, str, int]] = [
    ("enum", """
        enum color { RED, GREEN = 5, BLUE, LAST = BLUE + 10 };
        typedef enum { OFF, ON } state;
        enum { N = 4, NEG = -3, BIG = 32767 };
        int table[N];
        struct thing { enum color c; state s; };
        struct thing things[2] = { { GREEN, ON }, { LAST, OFF } };
        enum color pick(enum color c) { return c == RED ? BLUE : RED; }
        int main(void) {
            enum color c = GREEN;
            long l = NEG;
            int r = 0;
            switch (c) {
            case RED: r = 1; break;
            case GREEN: r = 2; break;
            default: r = 3;
            }
            { int GREEN = 100; r += GREEN; }
            { enum { GREEN = 7 }; r += GREEN; }
            for (int i = 0; i < N; i++) table[i] = i * BLUE;
            r += table[3];
            r += things[1].c + things[0].s;
            r += pick(RED) + pick(c);
            r += (int)sizeof(enum color) + (int)sizeof(state)
                 + (int)sizeof RED;
            r += (int)l + (BIG == 32767) + (NEG < 0);
            r += (enum color)3;
            return r;
        }
    """, 158),
    ("enum_negative_static", """
        enum { M = -32768, P = 32767 };
        long lo = M;
        int hi = P;
        int main(void) { return (lo == -32768L) + (hi == 32767) * 2; }
    """, 3),
    ("enum_one_byte", """
        typedef enum : unsigned char { IDLE, RUN, STOP = 250 } state;
        enum dir : signed char { LEFT = -1, NONE, RIGHT };
        struct actor { state s; enum dir d; unsigned char x; };
        struct actor actors[3] = {
            { RUN, RIGHT, 10 }, { STOP, LEFT, 20 }, { IDLE, NONE, 30 },
        };
        state next(state s) { return s == IDLE ? RUN : s == RUN ? STOP : IDLE; }
        int main(void) {
            int r = 0;
            for (unsigned char i = 0; i < 3; i++) {
                struct actor *a = &actors[i];
                a->x += a->d;
                a->s = next(a->s);
                switch (a->s) {
                case IDLE: r += 1; break;
                case RUN: r += 2; break;
                case STOP: r += 4; break;
                }
                if (a->d < 0) r += 100;
            }
            r += sizeof(struct actor) * 1000;
            r += actors[0].x + actors[1].x + actors[2].x;
            r += (actors[1].s == IDLE) * 10000;
            return r;
        }
    """, 13167),
]


@needs_c6502cpp
class TestPrograms(unittest.TestCase):

    def test_asm_sim(self) -> None:
        for name, src, expected in _PROGRAMS:
            for optimize in (False, True):
                with self.subTest(case=name, optimize=optimize):
                    result = build_sim(src, optimize=optimize).run()
                    self.assertFalse(result.timed_out)
                    self.assertEqual(result.return_int_signed(), expected)


if __name__ == "__main__":
    unittest.main()
