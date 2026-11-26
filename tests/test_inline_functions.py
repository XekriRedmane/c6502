"""cpp/src/passes/optimization/inline_functions.h: the calls of small `static`
functions are replaced by copies of their bodies under `-O`.

`TestRewrite` translates small programs to TAC, inlines them, and
checks the instructions written, the names given to the copies, and
which functions are left alone. `TestPrograms` runs programs whose
calls are inlined, in the simulator with and without `-O`, and checks
the values they compute, including the order of an argument's side
effects and volatile accesses."""

from __future__ import annotations

import unittest

from sim.harness import build_sim
from tests.cpp_build import needs_c6502cpp


# (name, C source, expected int return of main)
_CASES: list[tuple[str, str, int]] = [
    # Each argument's side effects happen once, in order, before the
    # body, which reads its parameter twice.
    ("argument_side_effects", """
        unsigned char log[4];
        unsigned char n;
        static unsigned char note(unsigned char v) { log[n++] = v; return v; }
        static int pair(int a, int b) { return a * 10 + b + a; }
        int main(void) {
            int r = pair(note(3), note(5));
            return r * 100 + log[0] * 10 + log[1] + n * 1000;
        }
    """, 2000 + 3800 + 30 + 5),
    # Several returns, used in an expression and in tail position.
    ("returns", """
        static int sign(int x) {
            if (x < 0) return -1;
            if (x == 0) return 0;
            return 1;
        }
        static int tail(int x) { return sign(x - 5); }
        int main(void) {
            return (sign(-7) + 2) * 100 + (sign(0) + 2) * 10
                + sign(9) + tail(2) * 1000;
        }
    """, -1000 + 100 + 20 + 1),
    # A void function with an early return, and a pointer result read
    # through, as Ali Baba's next_string is.
    ("void_and_pointer", """
        unsigned char text[] = { 2, 'a', 'b', 3, 'c', 'd', 'e', 1, 'f' };
        unsigned char g;
        static void bump(unsigned char by) { if (by == 0) return; g += by; }
        static const unsigned char *next(const unsigned char *s) {
            return s + s[0] + 1;
        }
        int main(void) {
            const unsigned char *m = text;
            for (unsigned char i = 0; i < 2; i++) m = next(m);
            bump(0);
            bump(4);
            bump(5);
            return m[1] + g * 1000;
        }
    """, ord('f') + 9000),
    # A copy inside a loop: each pass gets the parameter's new value,
    # and a local of the copy starts from its initializer each time.
    ("in_a_loop", """
        static unsigned int tri(unsigned char k) {
            unsigned int s = 0;
            while (k) s += k--;
            return s;
        }
        int main(void) {
            unsigned int t = 0;
            for (unsigned char i = 1; i <= 4; i++) t += tri(i);
            return t;
        }
    """, 1 + 3 + 6 + 10),
    # Volatile reads in the body happen once per call.
    ("volatile", """
        volatile unsigned char port;
        unsigned char reads;
        static unsigned char get(void) { reads++; return port; }
        int main(void) {
            port = 7;
            unsigned char a = get();
            unsigned char b = get();
            return a + b + reads * 100;
        }
    """, 214),
    # Nested candidates, with a wide and a floating parameter.
    ("nested", """
        static long sq(long x) { return x * x; }
        static long sum_sq(long a, long b) { return sq(a) + sq(b); }
        static int trunc(double d) { return (int)d; }
        int main(void) {
            return (int)(sum_sq(3, 4) + sum_sq(100, 200) / 1000) + trunc(2.75);
        }
    """, 25 + 50 + 2),
]


@needs_c6502cpp
class TestPrograms(unittest.TestCase):

    def test_programs(self):
        for name, src, expected in _CASES:
            for optimize in (False, True):
                with self.subTest(case=name, optimize=optimize):
                    result = build_sim(src, optimize=optimize).run()
                    self.assertFalse(result.timed_out)
                    self.assertEqual(result.return_int_signed(), expected)


if __name__ == "__main__":
    unittest.main()
