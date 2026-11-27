"""Tests for `apply_jump_tables` (cpp/src/passes/jump_table.h): a chain of compares
of A with constants becomes a dispatch through a table of the cases'
addresses.

The tests that called the Python functions directly are doctest cases of
the C++ port now, each named for the test it came from
(docs/cpp_port.md, step 8, group G6).

The `-S` text is checked in cpp/tests/test_jump_table_source.cpp."""

import unittest

from sim.harness import build_sim
from tests.cpp_build import needs_c6502cpp


_PROGRAM = """
#include <stdint.h>
uint8_t r[25];
static uint8_t pick(uint8_t n) {
    switch (n) {
    case 0: return 7;
    case 1: return 11;
    case 2: return 13;
    case 4: return 17;
    case 5: return 19;
    case 6: return 23;
    case 7: return 29;
    case 9: return 31;
    case 10: return 37;
    case 11: return 41;
    default: return 1;
    }
}
static uint8_t pick_high(uint8_t n) {
    switch (n) {
    case 100: return 2;
    case 101: return 3;
    case 102: return 5;
    case 103: return 7;
    case 104: return 11;
    case 105: return 13;
    case 106: return 17;
    case 107: return 19;
    case 108: return 23;
    default: return 0;
    }
}
int main(void) {
    for (uint8_t i = 0; i != 13; i++)
        r[i] = pick(i);
    r[13] = pick(200);
    for (uint8_t i = 0; i != 11; i++)
        r[14 + i] = pick_high((uint8_t)(99 + i));
    return 0;
}
"""


@needs_c6502cpp
class TestRuns(unittest.TestCase):

    def test_values(self):
        want = [7, 11, 13, 1, 17, 19, 23, 29, 1, 31, 37, 41, 1, 1,
                0, 2, 3, 5, 7, 11, 13, 17, 19, 23, 0]
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                sim = build_sim(_PROGRAM, optimize=optimize)
                sim.run()
                base = sim.symbols["r"]
                got = list(sim.mpu.memory[base:base + 25])
                self.assertEqual(got, want)


if __name__ == "__main__":
    unittest.main()
