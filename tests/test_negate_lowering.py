"""Tests for the lowering of an integer negate, `0 - x` a byte at a
time (`tac_to_asm.Translator`, the `Negate` case of the unary
lowering)."""

from __future__ import annotations

import shutil
import unittest

from sim.harness import build_sim

# For each type, values at the edges of each byte.
_CASES = (
    ("int", 16, (0, 1, -1, 255, 256, -256, 32767, -32768)),
    ("unsigned int", 16, (0, 1, 0xFF, 0x100, 0x7FFF, 0x8000, 0xFFFF)),
    ("long", 32, (0, 1, -1, 0xFFFF, 0x10000, 2**31 - 1, -2**31)),
    ("long long", 64, (0, 1, -1, 2**32, 2**63 - 1, -2**63)),
)


def _program(ctype: str, bits: int, values) -> str:
    # Each value as its bit pattern, since C has no literal for the
    # most negative value of a type.
    mask = (1 << bits) - 1
    table = ", ".join(f"({ctype})0x{v & mask:X}ULL" for v in values)
    return (
        f"{ctype} t[{len(values)}] = {{{table}}};\n"
        f"{ctype} r[{len(values)}];\n"
        "int main(void) {\n"
        f"    for (unsigned char i = 0; i < {len(values)}; i++)\n"
        "        r[i] = -t[i];\n"
        "    return 0;\n"
        "}\n"
    )


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestNegate(unittest.TestCase):

    def test_values(self):
        for ctype, bits, values in _CASES:
            for optimize in (False, True):
                with self.subTest(type=ctype, optimize=optimize):
                    sim = build_sim(_program(ctype, bits, values),
                                    optimize=optimize)
                    sim.run()
                    size = bits // 8
                    base = sim.symbols["r"]
                    mem = sim.mpu.memory
                    for k, v in enumerate(values):
                        got = int.from_bytes(
                            bytes(mem[base + size * k:base + size * (k + 1)]),
                            "little",
                        )
                        self.assertEqual(got, (-v) & ((1 << bits) - 1),
                                         msg=f"-({v})")

    def test_subtracts_from_zero(self):
        from compile import _run_source
        src = ("unsigned int g, h;\n"
               "int main(void) { h = -g; return 0; }\n")
        asm = _run_source("codegen", src, optimize=True)
        body = asm[asm.index("\nmain:"):]
        self.assertNotIn("EOR", body)
        self.assertIn("SBC   g", body)


if __name__ == "__main__":
    unittest.main()
