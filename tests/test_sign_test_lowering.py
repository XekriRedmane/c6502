"""Tests for the lowering of `x < 0` and `x >= 0` kept as a value, for
a signed `x` (`tac_to_asm.Translator._translate_sign_test`): the sign
bit of the high byte, moved into C by `ASL` and into a zero by `ROL`.
And for the unsigned orderings kept as a value
(`_translate_unsigned_ordering`), where `a < b` is the carry of `b - a
- 1`, `CLC` and `SBC`."""

from __future__ import annotations

import unittest

from sim.harness import build_sim
from tests.cpp_build import needs_c6502cpp

# For each type, values at the edges of the sign bit.
_CASES = (
    ("signed char", 8, (0, 1, -1, 127, -128)),
    ("int", 16, (0, 1, -1, 255, 256, -256, 32767, -32768)),
    ("long", 32, (0, 1, -1, 0xFFFF, 2**31 - 1, -2**31)),
)


def _program(ctype: str, bits: int, values) -> str:
    # Each value as its bit pattern, since C has no literal for the
    # most negative value of a type. `lt` and `ge` are stored through
    # globals, so the compares are kept as values.
    mask = (1 << bits) - 1
    table = ", ".join(f"({ctype})0x{v & mask:X}UL" for v in values)
    return (
        f"{ctype} t[{len(values)}] = {{{table}}};\n"
        f"int lt[{len(values)}];\n"
        f"bool ge[{len(values)}];\n"
        "int main(void) {\n"
        f"    for (unsigned char i = 0; i < {len(values)}; i++) {{\n"
        "        lt[i] = t[i] < 0;\n"
        "        ge[i] = 0 <= t[i];\n"
        "    }\n"
        "    return 0;\n"
        "}\n"
    )


@needs_c6502cpp
class TestSignTest(unittest.TestCase):

    def test_values(self):
        for ctype, bits, values in _CASES:
            for optimize in (False, True):
                with self.subTest(type=ctype, optimize=optimize):
                    sim = build_sim(_program(ctype, bits, values),
                                    optimize=optimize)
                    sim.run()
                    mem = sim.mpu.memory
                    lt, ge = sim.symbols["lt"], sim.symbols["ge"]
                    for k, v in enumerate(values):
                        got_lt = mem[lt + 2 * k] | mem[lt + 2 * k + 1] << 8
                        self.assertEqual(got_lt, int(v < 0), msg=f"{v} < 0")
                        self.assertEqual(mem[ge + k], int(v >= 0),
                                         msg=f"{v} >= 0")

    def test_no_subtract(self):
        from compile import _run_source
        src = ("#include <stdint.h>\n"
               "#include <stdbool.h>\n"
               "uint16_t a, b;\n"
               "bool n, p;\n"
               "int main(void) {\n"
               "    n = (int16_t)(a ^ b) < 0;\n"
               "    p = (int16_t)a >= 0;\n"
               "    return 0;\n"
               "}\n")
        asm = _run_source("codegen", src, optimize=True)
        body = asm[asm.index("\nmain:"):]
        self.assertNotIn("SBC", body)
        self.assertNotIn("BVC", body)
        self.assertEqual(body.count("ROL"), 2)


_UNSIGNED_PAIRS = ((0, 0), (0, 1), (1, 0), (5, 5), (0x7F, 0x80),
                   (0x80, 0x7F), (0xFF, 0), (0, 0xFF), (0xFF, 0xFF))


def _unsigned_values(bits: int, x: int, y: int) -> tuple[int, int]:
    """The pair `x`, `y` in the top bytes of a type of `bits` bits,
    with equal low bytes, so the high bytes decide for the wider
    types."""
    if bits == 8:
        return x, y
    scale = 1 << (bits - 8)
    return x * scale + x, y * scale + x


def _unsigned_program(ctype: str, bits: int) -> str:
    pairs = [_unsigned_values(bits, x, y) for x, y in _UNSIGNED_PAIRS]
    a = ", ".join(f"({ctype}){va}U" for va, _ in pairs)
    b = ", ".join(f"({ctype}){vb}U" for _, vb in pairs)
    n = len(_UNSIGNED_PAIRS)
    return (
        f"{ctype} a[{n}] = {{{a}}};\n"
        f"{ctype} b[{n}] = {{{b}}};\n"
        f"unsigned char r[{n}][4];\n"
        "int main(void) {\n"
        f"    for (unsigned char i = 0; i < {n}; i++) {{\n"
        "        r[i][0] = a[i] < b[i];\n"
        "        r[i][1] = a[i] > b[i];\n"
        "        r[i][2] = a[i] <= b[i];\n"
        "        r[i][3] = a[i] >= b[i];\n"
        "    }\n"
        "    return 0;\n"
        "}\n"
    )


@needs_c6502cpp
class TestUnsignedOrdering(unittest.TestCase):

    def test_values(self):
        for ctype, bits in (("unsigned char", 8), ("unsigned int", 16),
                            ("unsigned long", 32)):
            for optimize in (False, True):
                with self.subTest(type=ctype, optimize=optimize):
                    sim = build_sim(_unsigned_program(ctype, bits),
                                    optimize=optimize)
                    sim.run()
                    mem, r = sim.mpu.memory, sim.symbols["r"]
                    for k, (x, y) in enumerate(_UNSIGNED_PAIRS):
                        va, vb = _unsigned_values(bits, x, y)
                        got = list(mem[r + 4 * k:r + 4 * k + 4])
                        self.assertEqual(
                            got,
                            [int(va < vb), int(va > vb), int(va <= vb),
                             int(va >= vb)],
                            msg=f"{va:#x} vs {vb:#x}",
                        )

    _CARRY = (
        "#include <stdbool.h>\n"
        "#include <stdint.h>\n"
        "uint8_t level, score;\n"
        "[[c6502::return_bool_in_carry]]\n"
        "static bool beats(uint8_t l, uint8_t s) {\n"
        "    if (level != l)\n"
        "        return level > l;\n"
        "    return score > s;\n"
        "}\n"
        "int main(void) {\n"
        "    int n = 0;\n"
        "    level = 3; score = 7;\n"
        "    if (beats(2, 9)) n += 1;\n"
        "    if (!beats(4, 0)) n += 2;\n"
        "    if (beats(3, 6)) n += 4;\n"
        "    if (!beats(3, 7)) n += 8;\n"
        "    return n;\n"
        "}\n"
    )

    def test_carry_return_needs_no_rol(self):
        # `return level > l;` from a function that returns its `bool`
        # in C is the subtract alone, `LDA level; CLC; SBC l`.
        from compile import _run_source
        asm = _run_source("codegen", self._CARRY, optimize=True)
        body = asm[asm.index("\nbeats:"):asm.index("\nmain:")]
        self.assertNotIn("ROL", body)
        self.assertNotIn("EOR", body)

    def test_carry_return_runs(self):
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                out = build_sim(self._CARRY, optimize=optimize).run()
                self.assertEqual(out.return_int(), 15)


if __name__ == "__main__":
    unittest.main()
