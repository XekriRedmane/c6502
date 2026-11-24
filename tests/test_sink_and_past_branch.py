"""Tests for the TAC sinker that moves
`ZeroExtend; BitwiseAnd; Truncate` past a `JumpIfMasked`."""

import unittest


class TestSinkAndPastBranchKeepsSharedDefs(unittest.TestCase):
    """A ZeroExtend whose result another instruction reads. The pass
    dropped it with the rest of the trio, so `g16 = w` read a name
    that nothing wrote."""

    _SRC = (
        "#include <stdint.h>\n"
        "uint8_t bob[8] = {0x85, 3, 0, 0, 0, 0, 0, 0};\n"
        "uint8_t y = 10;\n"
        "int g16;\n"
        "void step(uint8_t i) {\n"
        "    uint8_t b = bob[i];\n"
        "    int w = b;\n"
        "    uint8_t mag = b & 0x7F;\n"
        "    if (b & 0x80) y -= mag; else y += mag;\n"
        "    g16 = w;\n"
        "}\n"
        "int main(void) { step(0); step(1); return g16 + y * 256; }\n"
    )

    def test_shared_zero_extend_sim(self):
        import shutil
        if not shutil.which("pcpp"):
            self.skipTest("pcpp CLI not available")
        from sim.harness import build_sim
        # y = 10 - 5 + 3 = 8, and g16 = 3 from the second call.
        for optimize in (False, True):
            r = build_sim(self._SRC, optimize=optimize).run(
                max_cycles=200_000,
            )
            self.assertEqual(
                r.return_int(), 8 * 256 + 3, f"optimize={optimize}",
            )


if __name__ == "__main__":
    unittest.main()
