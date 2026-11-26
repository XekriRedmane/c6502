"""Tests for `passes.optimization.sink_increment`, which moves `Y = X
+ c` past the last read of `X` in its block."""

from __future__ import annotations

import unittest

from sim.harness import build_sim
from tests.cpp_build import needs_c6502cpp


@needs_c6502cpp
class TestSinkIncrementSim(unittest.TestCase):
    def _both(self, src: str, expected: int) -> None:
        for optimize in (False, True):
            r = build_sim(src, optimize=optimize).run(max_cycles=200_000)
            self.assertFalse(r.timed_out, f"optimize={optimize}")
            self.assertEqual(r.return_int(), expected, f"optimize={optimize}")

    def test_static_written_before_last_read(self) -> None:
        # The add of the static `s` moved past `s = 5`, so h got 6.
        src = (
            "static unsigned s;\n"
            "unsigned g, h;\n"
            "void f(void) { unsigned t = s + 1; s = 5; g = s; h = t; }\n"
            "int main(void) { s = 1; f(); return h * 10 + g; }\n"
        )
        self._both(src, 25)

    def test_src_y_postincrement_chain(self) -> None:
        src = (
            "#include <stdint.h>\n"
            "uint8_t tab[4] = {3, 5, 7, 9};\n"
            "uint8_t out[3];\n"
            "void copy3(uint8_t y) {\n"
            "    out[0] = tab[y++];\n"
            "    out[1] = tab[y++];\n"
            "    out[2] = tab[y];\n"
            "}\n"
            "int main(void) { copy3(1); return out[0] + out[1] * 16; }\n"
        )
        self._both(src, 5 + 7 * 16)

    def test_post_decrement_tests(self) -> None:
        # `x-- != 0` tests the decremented value against -1, for each
        # width and signedness, with the count running out at 0 and
        # at a match.
        src = (
            "#include <stdint.h>\n"
            "uint8_t tab[8] = {9, 8, 7, 6, 5, 4, 3, 2};\n"
            "uint8_t find(uint8_t c) {\n"
            "    uint8_t x = 7;\n"
            "    do { if (tab[x] == c) return x + 10; } while (x-- != 0);\n"
            "    return 0;\n"
            "}\n"
            "int8_t sdown(int8_t n) { int8_t k = 0;\n"
            "    while (n-- != -3) k++; return k; }\n"
            "uint16_t wdown(uint16_t n) { uint16_t k = 0;\n"
            "    while (n--) k += 2; return k; }\n"
            "int main(void) {\n"
            "    return find(5) + find(1) * 100 + sdown(4) * 1000\n"
            "        + (int)wdown(300);\n"
            "}\n"
        )
        self._both(src, (4 + 10) + 0 + 7 * 1000 + 600)

    def test_post_decrement_becomes_dex(self) -> None:
        from compile import _run_source
        src = (
            "#include <stdint.h>\n"
            "uint8_t tab[8];\n"
            "uint8_t find(uint8_t c) {\n"
            "    uint8_t x = 7;\n"
            "    do { if (tab[x] == c) return x + 8; } while (x-- != 0);\n"
            "    return 0;\n"
            "}\n"
        )
        asm = _run_source("codegen", src, optimize=True)
        self.assertIn("CPX   #$FF", asm)
        self.assertNotIn("SBC", asm)


if __name__ == "__main__":
    unittest.main()
