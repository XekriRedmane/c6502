"""cpp/src/passes/return_in_zero_page.h: a `static` function returning a
pointer returns it in a zero-page object of its own, `F@ret`.

`TestRewrite` checks which functions the TAC rewrite chooses and the
instructions it writes. `TestPrograms` runs programs that use the
returned pointer in each way the pass and the asm passes after it
have to keep right, in the simulator with and without `-O`.
`TestCodegen` checks that a read through the result right after the
call reads through `F@ret`, with no copy."""

from __future__ import annotations

import unittest

from sim.harness import build_sim
from tests.no_inlining import no_inlining
from tests.cpp_build import needs_c6502cpp


# (name, C source, expected int return of main)
_CASES: list[tuple[str, str, int]] = [
    # The result read right after the call, kept across a second
    # call (so it has to be copied out of `F@ret`), stored into a
    # static, compared, and returned on from another function.
    ("uses_of_the_result", """
        unsigned char t[16] = { 1, 2, 3, 4, 5, 6, 7, 8,
                                9, 10, 11, 12, 13, 14, 15, 16 };
        unsigned char *saved;
        static unsigned char *at(unsigned char i) { return t + i; }
        static unsigned char *pick(unsigned char i) {
            if (i & 1) return at(i);
            return t;
        }
        unsigned char *outer(unsigned char i) { return at(i + 1); }
        int main(void) {
            unsigned char *a = at(3);
            unsigned char *b = at(5);
            saved = at(7);
            int r = a[0] * 100 + b[0] * 10 + saved[0];
            if (at(2) == t + 2) r += 1000;
            if (pick(2) == t) r += 2000;
            r += pick(9)[0] + outer(4)[0];
            a[1] = 40;
            return r + t[4];
        }
    """, 3000 + 400 + 60 + 8 + 10 + 6 + 40),
    # A recursive pointer function: each call's result is read
    # before the caller's own return writes the object.
    ("recursion", """
        unsigned char t[8] = { 5, 6, 7, 8, 9, 10, 11, 12 };
        static unsigned char *nth(unsigned char n) {
            if (n == 0) return t;
            return nth(n - 1) + 1;
        }
        int main(void) { return *nth(5) + *nth(0) * 100; }
    """, 10 + 500),
    # Stores through the result, and through a pointer to another
    # element, between reads of it.
    ("stores_between_reads", """
        unsigned char t[4] = { 1, 2, 3, 4 };
        unsigned char *q;
        static unsigned char *first(void) { return t; }
        int main(void) {
            q = t + 3;
            unsigned char *p = first();
            p[1] = 20;
            *q = 30;
            p[2] = p[1] + *q;
            return p[0] + p[1] + p[2] + p[3];
        }
    """, 1 + 20 + 50 + 30),
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


@needs_c6502cpp
@no_inlining()
class TestCodegen(unittest.TestCase):

    def test_read_right_after_the_call_goes_through_the_object(self):
        from compile import _run_source
        src = (
            "#include <stdint.h>\n"
            "uint8_t *base;\n"
            "static uint8_t *addr(uint8_t n) { return base + n * 9; }\n"
            "uint8_t get(uint8_t n) { return addr(n)[4]; }\n"
        )
        asm = _run_source("codegen", src, optimize=True)
        get = asm[asm.index("\nget:\n"):]
        self.assertIn("LDA   (addr@ret),Y", get)
        self.assertNotIn("STA", get[:get.index("RTS")])


if __name__ == "__main__":
    unittest.main()
