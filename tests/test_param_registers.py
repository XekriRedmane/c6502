"""Tests for naming the register a parameter arrives in:
`uint8_t lo [[c6502::in_y]]`, and for a 2-byte parameter a pair
such as `in_xy` (low byte in X, high byte in Y). `register` is
optional with the attribute, and the parameter may be any integer or
pointer with one byte for each register named. A `register` parameter
without the attribute keeps the positional rule (A, then X), skipping
the registers the named ones take, and can't be a pointer.

The tests of the errors and of the code are doctest cases in
cpp/tests/test_param_registers.cpp.
"""
from __future__ import annotations

import unittest

from compile import _run_source
from sim.harness import build_sim
from tests.cpp_build import needs_c6502cpp


def _codegen(src: str, optimize: bool = True) -> str:
    return _run_source("codegen", src, optimize=optimize)


@needs_c6502cpp
class TestSim(unittest.TestCase):
    """Each program runs through both pipelines."""

    def _check(self, src: str, expected: int, *, standin=None) -> None:
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                sim = build_sim(src, optimize=optimize)
                if standin is not None:
                    addr, code = standin
                    sim.mpu.memory[addr:addr + len(code)] = code
                self.assertEqual(sim.run().return_int(), expected)

    def test_compiled_function(self):
        src = (
            "unsigned char out[2];\n"
            "unsigned char g = 9;\n"
            "void put(register unsigned char hi [[c6502::in_a]],\n"
            "         register unsigned char lo [[c6502::in_y]]) {\n"
            "    out[0] = hi; out[1] = lo;\n"
            "}\n"
            "int main(void) {\n"
            "    unsigned char a = g + 1;\n"
            "    put(a, g);\n"
            "    return out[0] * 100 + out[1];\n"
            "}\n"
        )
        self._check(src, 1009)

    def test_pair_and_positional(self):
        # `w` takes X and Y, so `k`, declared plain `register`, gets A.
        src = (
            "unsigned int sum(register unsigned int w [[c6502::in_xy]],\n"
            "                 register unsigned char k) {\n"
            "    return w + k;\n"
            "}\n"
            "unsigned int swap(register unsigned int w [[c6502::in_ya]]) {\n"
            "    return (w << 8) | (w >> 8);\n"
            "}\n"
            "int main(void) {\n"
            "    unsigned int s = sum(0x1234, 3);\n"
            "    return (s == 0x1237) + (swap(0xABCD) == 0xCDAB) * 2;\n"
            "}\n"
        )
        self._check(src, 3)

    # A routine at $D000 that returns A + 2 * Y, as `STY $3100; ASL
    # $3100; CLC; ADC $3100; RTS`.
    _ROM = (
        "[[c6502::zp_abi, c6502::address(0xD000)]]\n"
        "unsigned char rom(register unsigned char a [[c6502::in_a]],\n"
        "                  register unsigned char y [[c6502::in_y]]);\n"
    )
    _ROM_CODE = (0xD000, bytes([0x8C, 0x00, 0x31, 0x0E, 0x00, 0x31, 0x18,
                                0x6D, 0x00, 0x31, 0x60]))

    def test_fixed_address_routine(self):
        src = self._ROM + (
            "unsigned char g = 20;\n"
            "int main(void) { return rom(g, 3) + rom(1, g); }\n"
        )
        self._check(src, (20 + 6) + (1 + 40), standin=self._ROM_CODE)

    def test_pointer_parameters(self):
        # `second` and `sum` read through pointers that arrive in
        # registers, without `register`.
        src = (
            "struct blk { unsigned char a, b, c; };\n"
            "struct blk blk = {1, 2, 3};\n"
            "unsigned char tab[4] = {10, 20, 30, 40};\n"
            "unsigned char second(const unsigned char *p "
            "[[c6502::in_ya]]) {\n"
            "    return p[1];\n"
            "}\n"
            "unsigned char sum(struct blk *b [[c6502::in_xy]],\n"
            "                  unsigned char k [[c6502::in_a]]) {\n"
            "    return b->a + b->c + k;\n"
            "}\n"
            "int main(void) { return second(tab) + sum(&blk, 100); }\n"
        )
        self._check(src, 20 + 1 + 3 + 100)

    # A routine at $D100 that returns the byte after the one its
    # pointer argument, low byte in Y and high byte in A, points at:
    # `STY $F0; STA $F1; LDY #1; LDA ($F0),Y; RTS`.
    _NEXT_CODE = (0xD100, bytes([0x84, 0xF0, 0x85, 0xF1, 0xA0, 0x01,
                                 0xB1, 0xF0, 0x60]))

    def test_fixed_address_routine_with_a_pointer(self):
        src = (
            "struct blk { unsigned char a, b, c; };\n"
            "struct blk blk = {7, 9, 11};\n"
            "[[c6502::zp_abi, c6502::address(0xD100)]]\n"
            "unsigned char next(struct blk *p [[c6502::in_ya]]);\n"
            "int main(void) { return next(&blk); }\n"
        )
        self._check(src, 9, standin=self._NEXT_CODE)

    def test_a_argument_from_a_frame_slot(self):
        # Without -O, `a` and `b` live on the soft stack, and
        # loading `a` into A is `LDY #off; LDA (FP),Y`, which would
        # destroy `b`, already in Y. So `a` is loaded first and kept
        # with PHA / PLA.
        src = self._ROM + (
            "int main(void) {\n"
            "    unsigned char a = 7, b = 5;\n"
            "    for (unsigned char i = 0; i < 3; i++) { a++; b += 2; }\n"
            "    return rom(a, b);\n"
            "}\n"
        )
        self.assertIn("PHA", _codegen(src, optimize=False))
        self._check(src, 10 + 2 * 11, standin=self._ROM_CODE)


if __name__ == "__main__":
    unittest.main()
