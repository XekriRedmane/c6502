"""Tests for `passes.optimization.value_range`,
`passes.optimization.narrow_byte_range`, strength reduction's rule for
a nonnegative signed dividend, and the `tac_to_asm` lowerings they
lead to (`_inline_byte_divide`, `_shift_loop`, `_inline_byte_product`,
and the pairing of a divide with a remainder)."""

from __future__ import annotations

import unittest
from tests.cpp_build import needs_c6502cpp


def _asm(src: str, optimize: bool = True) -> str:
    from compile import _run_source
    return _run_source("codegen", src, optimize=optimize)


def _body(asm: str, fn: str) -> str:
    """The lines of function `fn`, up to the next function's label."""
    rest = asm[asm.index(f"\n{fn}:"):]
    nxt = rest.find(":\n   SUBROUTINE", len(fn) + 3)
    return rest if nxt < 0 else rest[:rest.rfind("\n", 0, nxt)]


@needs_c6502cpp
class TestLowerings(unittest.TestCase):

    def test_byte_divide_is_inline(self):
        asm = _asm("#include <stdint.h>\n"
                   "uint8_t f(uint8_t x) { return x / 20; }\n")
        body = _body(asm, "f")
        self.assertIn("SBC   #$14", body)
        self.assertNotIn("JSR", body)
        self.assertNotIn("JMP", body)

    def test_divide_and_remainder_share_one_loop(self):
        asm = _asm("#include <stdint.h>\n"
                   "uint16_t f(uint8_t x) { uint8_t c = x % 20;"
                   " uint8_t r = x / 20; return c | (uint16_t)r << 8; }\n")
        body = _body(asm, "f")
        self.assertEqual(body.count("SBC   #$14"), 1)
        self.assertIn("ADC   #$14", body)

    def test_word_divide_and_remainder_share_one_call(self):
        asm = _asm("#include <stdint.h>\n"
                   "uint16_t q;\n"
                   "uint16_t f(uint16_t v, uint16_t p) "
                   "{ q = v / p; v %= p; return v; }\n")
        self.assertEqual(_body(asm, "f").count("udivmod16"), 1)

    def test_variable_shifts_are_loops(self):
        src = ("#include <stdint.h>\n"
               "unsigned int f(unsigned int x, uint8_t n) { return x << n; }\n"
               "int g(int x, uint8_t n) { return x >> n; }\n"
               "uint8_t h(uint8_t x, uint8_t n) { return x >> n; }\n")
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                asm = _asm(src, optimize)
                self.assertNotIn("JSR   asl", asm)
                self.assertNotIn("JSR   asr", asm)
                self.assertNotIn("JSR   lsr", asm)
                self.assertIn("DEX", asm)

    def test_shift_of_promoted_byte_is_logical(self):
        # `(style >> 5)` inside a product had called `asr16`.
        asm = _asm("#include <stdint.h>\n"
                   "uint16_t f(uint8_t p, uint8_t s) "
                   "{ return p * (((s >> 5) & 3) + 1); }\n")
        body = _body(asm, "f")
        self.assertEqual(body.count("LSR\n"), 5)
        self.assertNotIn("JSR", body)
        self.assertNotIn("mul16", body)

    def test_byte_product_is_inline(self):
        asm = _asm("#include <stdint.h>\n"
                   "uint16_t f(uint8_t a, uint8_t b) { return a * b; }\n")
        body = _body(asm, "f")
        self.assertIn("ROR   HARGS", body)
        self.assertNotIn("mul16", body)

    def test_word_product_calls_mul16(self):
        asm = _asm("#include <stdint.h>\n"
                   "uint16_t f(uint16_t a, uint8_t b) { return a * b; }\n")
        self.assertIn("mul16", _body(asm, "f"))


if __name__ == "__main__":
    unittest.main()
