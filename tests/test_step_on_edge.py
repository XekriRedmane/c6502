"""Tests for cpp/src/passes/optimization/step_on_edge.h, which moves a step
whose result is copied on one edge alone onto that edge, and runs of
the loops it rewrites in both pipelines."""

from __future__ import annotations

import unittest
from tests.cpp_build import needs_c6502cpp


@needs_c6502cpp
class TestStepOnEdgeSim(unittest.TestCase):

    _SRC = """
#include <stdint.h>
uint8_t seen[300];
uint16_t n;
void g(uint8_t i) { seen[n++] = i; }
void down(uint8_t first, uint8_t last) {
    uint8_t i = last;
    do g(i); while (i-- != first);
}
uint8_t up(uint8_t i, uint8_t last) {
    while (i++ != last)
        g(i);
    return i;
}
int main(void) {
    down(%d, %d);
    seen[n++] = up(%d, %d);
    return n;
}
"""

    def _model(self, a, b, c, d):
        out = []
        i = b
        while True:
            out.append(i)
            old = i
            i = (i - 1) & 0xFF
            if old == a:
                break
        i = c
        while True:
            old = i
            i = (i + 1) & 0xFF
            if old == d:
                break
            out.append(i)
        out.append(i)
        return out

    def test_runs(self):
        from sim.harness import build_sim
        for a, b, c, d in [(0, 0, 0, 0), (3, 7, 250, 255), (0, 5, 4, 9),
                           (255, 255, 254, 255), (10, 9, 0, 3)]:
            want = self._model(a, b, c, d)
            if len(want) > 300:
                continue
            for optimize in (False, True):
                with self.subTest(args=(a, b, c, d), optimize=optimize):
                    sim = build_sim(self._SRC % (a, b, c, d),
                                    optimize=optimize)
                    result = sim.run(max_cycles=5_000_000)
                    self.assertEqual(result.return_int(), len(want))
                    base = sim.symbols["seen"]
                    got = list(sim.mpu.memory[base:base + len(want)])
                    self.assertEqual(got, want)


if __name__ == "__main__":
    unittest.main()
