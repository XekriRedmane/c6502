"""Tests for `passes.optimization.read_stored_global`, which reads a
global just stored from a temporary in place of the temporary, and runs
of `*++P` in both pipelines."""

from __future__ import annotations

import unittest
from tests.cpp_build import needs_c6502cpp


@needs_c6502cpp
class TestReadStoredGlobalSim(unittest.TestCase):

    _SRC = """
#include <stdint.h>
const uint8_t T[] = {3, 1, 4, 1, 5, 9, 2, 6};
extern const uint8_t *P [[c6502::zeropage(0xEC)]];
const uint8_t *Q;
uint8_t next(void) { return *++P; }
uint8_t nextq(void) { return *++Q + *Q; }
int main(void) {
    P = T; Q = T;
    uint16_t sum = 0;
    for (uint8_t k = 0; k < 7; k++)
        sum = sum * 3 + next() + nextq();
    return sum + (P - T) * 1000;
}
"""

    def test_runs(self):
        from sim.harness import build_sim
        want, p = 0, 0
        t = [3, 1, 4, 1, 5, 9, 2, 6]
        for _ in range(7):
            p += 1
            want = (want * 3 + t[p] + 2 * t[p]) & 0xFFFF
        want = (want + p * 1000) & 0xFFFF
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                out = build_sim(self._SRC, optimize=optimize).run()
                self.assertEqual(out.return_int() & 0xFFFF, want)


if __name__ == "__main__":
    unittest.main()
