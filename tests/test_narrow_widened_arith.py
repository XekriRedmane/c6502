"""Unit tests for cpp/src/passes/optimization/narrow_widened_arith.h.

Covers the `Truncate(Binary(safe_op, Extend(a), Extend(b) | Const),
u_n)` → `Binary(safe_op, a, b | narrowed_Const, u_n)` rewrite for
the wraparound-safe ops (Add / Subtract / Multiply / BitwiseAnd /
BitwiseOr / BitwiseXor), the soundness gates that exclude
non-wraparound-safe ops (Divide / Modulo / RightShift, and LeftShift
by anything but a constant below the narrow width) and non-SSA-renamed
names, the constant-narrowing logic that
masks Constants modulo the target width, and a truncated comparison
narrowing to its 1-byte result.

The `-S` text is checked in cpp/tests/test_narrow_widened_arith.cpp.
"""

import unittest
from tests.cpp_build import needs_c6502cpp


@needs_c6502cpp
class TestGlobalShiftOnTwoPathsSim(unittest.TestCase):

    _SRC = (
        "#include <stdint.h>\n"
        "uint8_t alive = 0x81, other = 6, key;\n"
        "void f(void) {\n"
        "    if (key == 3) {\n"
        "        other++;\n"
        "        alive >>= 1;\n"
        "        return;\n"
        "    }\n"
        "    if (key == 4)\n"
        "        alive >>= 1;\n"
        "}\n"
        "int main(void) {\n"
        "    key = 3; f();\n"
        "    key = 4; f();\n"
        "    key = 5; f();\n"
        "    return alive + (other << 8);\n"
        "}\n"
    )

    def test_runs(self):
        from sim.harness import build_sim
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                out = build_sim(self._SRC, optimize=optimize).run()
                self.assertEqual(out.return_int(), 0x0720)


@needs_c6502cpp
class TestVariableLeftShift(unittest.TestCase):
    """A byte shifted left by a variable count is a 1-byte shift, which
    `tac_to_asm` lowers as a loop of `ASL`s, where it had called
    `asl16`. A count of 8 or more gives 0 at either width."""

    _SRC = (
        "#include <stdint.h>\n"
        "uint8_t s(uint8_t v, uint8_t n) { v <<= n; return v; }\n"
        "int main(void) {\n"
        "    int bad = 0;\n"
        "    if (s(3, 2) != 12) bad |= 1;\n"
        "    if (s(0x81, 1) != 2) bad |= 2;\n"
        "    if (s(5, 8) != 0) bad |= 4;\n"
        "    if (s(5, 12) != 0) bad |= 8;\n"
        "    if (s(5, 0) != 5) bad |= 16;\n"
        "    return bad;\n"
        "}\n"
    )

    def test_runs(self):
        from sim.harness import build_sim
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                out = build_sim(self._SRC, optimize=optimize).run()
                self.assertEqual(out.return_int(), 0)

if __name__ == "__main__":
    unittest.main()
