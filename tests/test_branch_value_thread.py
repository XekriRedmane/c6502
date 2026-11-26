"""Tests for `passes.branch_value_thread.apply_branch_value_thread`: a
branch goes past the loads, compares, and branches at its target that
its path has already decided.

The tests that called the Python functions directly are doctest cases of
the C++ port now, each named for the test it came from
(docs/cpp_port.md, step 8, group G6)."""

import unittest

from sim.harness import build_sim
from tests.cpp_build import needs_c6502cpp


_PROGRAM = """
#include <stdint.h>
uint16_t t[8] = {0, 2, 4, 6, 0x102, 0x104, 0x200, 0xFFFF};
int r[8];
static int scale(uint16_t a, uint16_t b) {
    if (b == 2)
        return a << 1;
    else if (b == 4)
        return a << 2;
    return a + b;
}
int main(void) {
    for (uint8_t i = 0; i != 8; i++)
        r[i] = scale(3, t[i]);
    return 0;
}
"""


@needs_c6502cpp
class TestRuns(unittest.TestCase):

    def test_values(self):
        want = [3, 6, 12, 9, 0x105, 0x107, 0x203, 2]
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                sim = build_sim(_PROGRAM, optimize=optimize)
                sim.run()
                base = sim.symbols["r"]
                mem = sim.mpu.memory
                got = [mem[base + 2 * k] | mem[base + 2 * k + 1] << 8
                       for k in range(8)]
                self.assertEqual(got, want)


if __name__ == "__main__":
    unittest.main()
