"""Tests for `passes.cross_jump.apply_cross_jump`: of two blocks that
end in the same instructions and a jump to the same place, the later
jumps into the earlier one's copy.

The tests that called the Python functions directly are doctest cases of
the C++ port now, each named for the test it came from
(docs/cpp_port.md, step 8, group G6)."""

import unittest

from sim.harness import build_sim
from tests.cpp_build import needs_c6502cpp


_PROGRAM = """
#include <stdint.h>
uint16_t t[5] = {1, 2, 4, 7, 0x300};
uint16_t r[5];
static uint16_t scale(uint16_t a, uint16_t b) {
    uint16_t p;
    if (b == 2)
        p = a << 1;
    else if (b == 4)
        p = a << 2;
    else
        p = a + b;
    return p;
}
int main(void) {
    for (uint8_t i = 0; i != 5; i++)
        r[i] = scale(0x81, t[i]);
    return 0;
}
"""


@needs_c6502cpp
class TestRuns(unittest.TestCase):

    def test_values(self):
        want = [0x82, 0x102, 0x204, 0x88, 0x381]
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                sim = build_sim(_PROGRAM, optimize=optimize)
                sim.run()
                base = sim.symbols["r"]
                mem = sim.mpu.memory
                got = [mem[base + 2 * k] | mem[base + 2 * k + 1] << 8
                       for k in range(5)]
                self.assertEqual(got, want)


if __name__ == "__main__":
    unittest.main()
