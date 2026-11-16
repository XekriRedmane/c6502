"""Tests for the lowering of `x < 0` and `x >= 0` kept as a value, for
a signed `x` (`tac_to_asm.Translator._translate_sign_test`): the sign
bit of the high byte, moved into C by `ASL` and into a zero by `ROL`."""

from __future__ import annotations

import shutil
import unittest

from sim.harness import build_sim

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
        f"_Bool ge[{len(values)}];\n"
        "int main(void) {\n"
        f"    for (unsigned char i = 0; i < {len(values)}; i++) {{\n"
        "        lt[i] = t[i] < 0;\n"
        "        ge[i] = 0 <= t[i];\n"
        "    }\n"
        "    return 0;\n"
        "}\n"
    )


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
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
        from compile import _run_stage
        from preprocessor import preprocess
        src = ("#include <stdint.h>\n"
               "#include <stdbool.h>\n"
               "uint16_t a, b;\n"
               "bool n, p;\n"
               "int main(void) {\n"
               "    n = (int16_t)(a ^ b) < 0;\n"
               "    p = (int16_t)a >= 0;\n"
               "    return 0;\n"
               "}\n")
        asm = _run_stage("codegen", preprocess(src), optimize=True)
        body = asm[asm.index("\nmain:"):]
        self.assertNotIn("SBC", body)
        self.assertNotIn("BVC", body)
        self.assertEqual(body.count("ROL"), 2)


if __name__ == "__main__":
    unittest.main()
