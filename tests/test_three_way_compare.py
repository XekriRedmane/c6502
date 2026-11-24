"""Tests for `passes.optimization.three_way_compare`: a 2-byte unsigned
ordering compare and an equality compare of the same two values
split into a compare of the high bytes and then the low ones."""

from __future__ import annotations

import shutil
import unittest

from sim.harness import build_sim
from tests.no_inlining import no_inlining

# Values on both sides of each byte's edges, so every outcome of each
# byte's compare comes up.
_VALUES = (0x0000, 0x0001, 0x00FF, 0x0100, 0x0101, 0x01FF, 0x1233, 0x1234,
           0x1235, 0x1134, 0x1334, 0x8000, 0xFFFE, 0xFFFF)


def _classify(a: int, b: int) -> int:
    return 1 if a < b else 2 if a == b else 3


# Each body returns 1 for a < b, 2 for a == b, and 3 for a > b, given
# `uint16_t a, b`.
_SHAPES = {
    # The ordering compare's target starts with the equality compare,
    # which jumps when they differ.
    "less_then_equal": """
        if (a < b) return 1;
        if (a == b) return 2;
        return 3;""",
    # The same, with the equality compare jumping when they're equal.
    "less_then_not_equal": """
        if (a < b) return 1;
        if (a != b) return 3;
        return 2;""",
    # `<=` at the first compare, so the operands swap.
    "greater_then_equal": """
        if (a > b) return 3;
        if (a == b) return 2;
        return 1;""",
    # The two compares in a row.
    "adjacent": """
        if (a < b) goto less;
        if (a != b) goto greater;
        return 2;
    less:
        return 1;
    greater:
        return 3;""",
    # A constant operand.
    "constant": """
        (void)b;
        if (a < 0x1234u) return 1;
        if (a == 0x1234u) return 2;
        return 3;""",
    # Operands built from bytes, as in Zork's match_dictionary_word.
    "built_from_bytes": """
        uint8_t ka[2], kb[2];
        ka[0] = (uint8_t)a; ka[1] = (uint8_t)(a >> 8);
        kb[0] = (uint8_t)b; kb[1] = (uint8_t)(b >> 8);
        uint16_t x = ((uint16_t)ka[1] << 8) | ka[0];
        uint16_t y = ((uint16_t)kb[1] << 8) | kb[0];
        if (x < y) return 1;
        if (x == y) return 2;
        return 3;""",
    # A byte widened to 2 bytes, whose high byte is 0.
    "widened_byte": """
        uint8_t c = (uint8_t)a;
        if (c < b) return 1;
        if (c == b) return 2;
        return 3;""",
}

_EXPECTED = {
    "constant": lambda a, b: _classify(a, 0x1234),
    "widened_byte": lambda a, b: _classify(a & 0xFF, b),
}


def _program(body: str) -> str:
    table = ", ".join(f"0x{v:04X}u" for v in _VALUES)
    n = len(_VALUES)
    return (
        "#include <stdint.h>\n"
        f"uint16_t t[{n}] = {{{table}}};\n"
        "static uint8_t f(uint16_t a, uint16_t b) {" + body + "\n}\n"
        "int main(void) {\n"
        "    uint16_t n = 0;\n"
        f"    for (uint8_t i = 0; i < {n}; i++)\n"
        f"        for (uint8_t j = 0; j < {n}; j++)\n"
        "            n = (uint16_t)(n * 5u + f(t[i], t[j]));\n"
        "    return n;\n"
        "}\n"
    )


def _expected(name: str) -> int:
    pred = _EXPECTED.get(name, _classify)
    n = 0
    for a in _VALUES:
        for b in _VALUES:
            n = (n * 5 + pred(a, b)) & 0xFFFF
    return n


def _function_asm(src: str, name: str) -> str:
    from compile import _run_stage
    from preprocessor import preprocess
    asm = _run_stage("codegen", preprocess(src), optimize=True)
    start = asm.index(f"\n{name}:")
    return asm[start:asm.index("\nmain:", start)]


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
@no_inlining()
class TestRuns(unittest.TestCase):

    def test_shapes(self):
        for name, body in _SHAPES.items():
            for optimize in (False, True):
                with self.subTest(shape=name, optimize=optimize):
                    r = build_sim(_program(body), optimize=optimize).run()
                    self.assertEqual(r.return_int(), _expected(name))

    def test_no_two_byte_subtract(self):
        # A 2-byte ordering compare lowers to `SBC`, and a split one
        # to a `CMP` of each byte.
        for name, body in _SHAPES.items():
            with self.subTest(shape=name):
                self.assertNotIn("SBC", _function_asm(_program(body), "f"))

    def test_signed_compare_left_alone(self):
        body = _SHAPES["less_then_equal"].replace("a < b", "(int)a < (int)b")
        self.assertIn("SBC", _function_asm(_program(body), "f"))


if __name__ == "__main__":
    unittest.main()
