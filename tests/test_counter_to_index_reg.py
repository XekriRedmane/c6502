"""Tests for `passes.counter_to_index_reg`.

The tests that called the Python functions directly are doctest cases of
the C++ port now, each named for the test it came from
(docs/cpp_port.md, step 8, group G6)."""

from __future__ import annotations

import shutil
import unittest


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestDelayLoopSim(unittest.TestCase):

    _SRC = (
        "#include <stdint.h>\n"
        "uint16_t passes;\n"
        "void delay(uint8_t count) {\n"
        "    do {\n"
        "        [[c6502::keep]] for (uint8_t y = 3; --y;) {\n"
        "        }\n"
        "        passes++;\n"
        "    } while (--count);\n"
        "}\n"
        "int main(void) {\n"
        "    delay(5);\n"
        "    delay(0);\n"
        "    return passes;\n"
        "}\n"
    )

    def test_dey(self):
        from compile import _run_stage
        from preprocessor import preprocess
        asm = _run_stage("codegen", preprocess(self._SRC), optimize=True)
        body = asm[asm.index("\ndelay:"):asm.index("\nmain:")]
        self.assertIn("DEY", body)
        self.assertIn("DEX", body)

    def test_runs(self):
        from sim.harness import build_sim
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                out = build_sim(self._SRC, optimize=optimize).run()
                self.assertEqual(out.return_int(), 5 + 256)


if __name__ == "__main__":
    unittest.main()
