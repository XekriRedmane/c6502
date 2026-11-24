"""Tests for `passes.optimization.narrow_byte_bitwise`."""

from __future__ import annotations

import shutil
import unittest


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestEndToEnd(unittest.TestCase):

    _SRC = (
        "#include <stdint.h>\n"
        "uint8_t g = 0xC5;\n"
        "int8_t sg = -3;\n"
        "uint8_t p[8] = {0, 0, 0, 0, 0, 0, 0x2A, 0};\n"
        "int below(uint8_t *q, uint8_t lim) { return (q[6] & 0x3F) < lim; }\n"
        "int main(void) {\n"
        "    int n = 0;\n"
        "    if ((g & 0x3F) < 6) n |= 1;\n"
        "    if ((g | 0x10) == 0xD5) n |= 2;\n"
        "    if ((g ^ 0x0F) > 0xC9) n |= 4;\n"
        "    if ((sg & 0xF0) == 0xF0) n |= 8;\n"
        "    if (below(p, 43)) n |= 16;\n"
        "    if (below(p, 42)) n |= 32;\n"
        "    int8_t d = (int8_t)(g - 0xC6);\n"
        "    if (d > 0) n |= 64;\n"
        "    if (d > -2) n |= 128;\n"
        "    return n;\n"
        "}\n"
    )

    def test_runs(self):
        from sim.harness import build_sim
        # g & 0x3F is 5, g | 0x10 is 0xD5, g ^ 0x0F is 0xCA, sg & 0xF0
        # is 0xF0, p[6] & 0x3F is 42, and d is -1.
        expected = 1 | 2 | 4 | 8 | 16 | 128
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                out = build_sim(self._SRC, optimize=optimize).run()
                self.assertEqual(out.return_int(), expected)

    def test_byte_compares(self):
        from compile import _run_stage
        from preprocessor import preprocess
        asm = _run_stage("codegen", preprocess(self._SRC), optimize=True)
        body = asm[asm.index("\nbelow:"):asm.index("\nmain:")]
        # A byte compare has no overflow test.
        self.assertNotIn("BVC", body)
        self.assertNotIn("EOR   #$80", body)


if __name__ == "__main__":
    unittest.main()
