"""Tests for cpp/src/passes/optimization/count_down.h, which counts a loop down
to 0 when its counter is read only by its own test and step, and runs
of such loops in both pipelines."""

from __future__ import annotations

import unittest
from tests.cpp_build import needs_c6502cpp


@needs_c6502cpp
class TestCountDownSim(unittest.TestCase):

    _SRC = """
#include <stdint.h>
uint16_t ticks;
void tick(void) { ticks++; }
uint8_t K;
void a(void) { for (uint8_t n = 0; n < 20; n++) tick(); }
void b(uint8_t k) { for (uint8_t n = 0; n < k; n++) tick(); }
void c(void) { for (uint8_t n = 3; n != 3; n++) tick(); }
void d(void) { for (uint8_t n = 7; n <= 254; n++) tick(); }
int main(void) {
    a(); uint16_t ra = ticks; ticks = 0;
    b(K); uint16_t rb = ticks; ticks = 0;
    c(); uint16_t rc = ticks; ticks = 0;
    d();
    return ra + rb * 2 + rc * 1000 + ticks * 4;
}
"""

    def test_runs(self):
        from sim.harness import build_sim
        for k in (0, 1, 200, 255):
            for optimize in (False, True):
                with self.subTest(k=k, optimize=optimize):
                    sim = build_sim(self._SRC.replace(
                        "uint8_t K;", f"uint8_t K = {k};"),
                        optimize=optimize)
                    out = sim.run(max_cycles=5_000_000)
                    self.assertEqual(out.return_int() & 0xFFFF,
                                     (20 + 2 * k + 4 * 248) & 0xFFFF)


if __name__ == "__main__":
    unittest.main()
