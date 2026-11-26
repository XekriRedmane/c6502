"""Tests for the const-pointer-array dispatch pass."""

import unittest

from tests.cpp_build import needs_c6502cpp


# The tables the tests below use, typed `static uint8_t * const
# name[N]`, so their elements are const.


@needs_c6502cpp
class TestDispatchSim(unittest.TestCase):
    def _both(self, src, expected):
        from sim.harness import build_sim
        for optimize in (False, True):
            r = build_sim(src, optimize=optimize).run(max_cycles=200_000)
            self.assertEqual(r.return_int(), expected, f"optimize={optimize}")

    def test_table_stored_into_by_another_function(self):
        # swap() repoints frames[0]. The dispatch read walk[1] = 2
        # from the initial contents in place of other[1] = 9.
        src = (
            "#include <stdint.h>\n"
            "static uint8_t walk[4] = {1, 2, 3, 4};\n"
            "static uint8_t jump[4] = {5, 6, 7, 8};\n"
            "static uint8_t other[4] = {9, 9, 9, 9};\n"
            "static uint8_t *frames[2] = {walk, jump};\n"
            "void swap(void) { frames[0] = other; }\n"
            "uint8_t frame(uint8_t anim, uint8_t step) {\n"
            "    return frames[anim][step];\n"
            "}\n"
            "int main(void) { swap(); return frame(0, 1) + frame(1, 2) * 16; }\n"
        )
        self._both(src, 9 + 7 * 16)

    def test_const_table(self):
        src = (
            "#include <stdint.h>\n"
            "static uint8_t walk[4] = {1, 2, 3, 4};\n"
            "static uint8_t jump[4] = {5, 6, 7, 8};\n"
            "static uint8_t * const frames[2] = {walk, jump};\n"
            "uint8_t frame(uint8_t anim, uint8_t step) {\n"
            "    return frames[anim][step];\n"
            "}\n"
            "int main(void) { walk[1] = 11; return frame(0, 1) + frame(1, 2) * 16; }\n"
        )
        self._both(src, 11 + 7 * 16)


if __name__ == "__main__":
    unittest.main()
