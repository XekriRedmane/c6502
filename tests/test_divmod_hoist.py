"""Tests for `passes.optimization.divmod_hoist`, which computes a divide
and a remainder of the same operands next to each other, and runs of
the functions it rewrites in both pipelines."""

from __future__ import annotations

import shutil
import unittest


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestDivmodHoistSim(unittest.TestCase):

    _SRC = """
#include <stdint.h>
struct iob { uint8_t pad[4]; uint8_t track, sector; };
struct iob IOB;
struct iob *P = &IOB;
uint8_t V, Q, R, S;
uint8_t OUT[256 * 5];
void g(uint8_t s) { P->sector = s %% 13; P->track = 0x11 + s / 13; }
void f(void) { if (V >= 24) Q = V / 24; R = V %% 24; }
uint8_t h(uint8_t sel) {
    if (V >= 24)
        S = sel + V / 24;
    return V %% 24 * 4 + 0x20;
}
int main(void) {
    for (uint16_t v = 0; v < 256; v++) {
        g((uint8_t)v);
        V = (uint8_t)v;
        Q = 0xEE;
        f();
        S = 0xEE;
        uint8_t x = h(%d);
        OUT[v * 5] = IOB.sector;
        OUT[v * 5 + 1] = IOB.track;
        OUT[v * 5 + 2] = Q;
        OUT[v * 5 + 3] = R;
        OUT[v * 5 + 4] = x ^ S;
    }
    return 0;
}
"""

    def test_runs(self):
        from sim.harness import build_sim
        sel = 7
        want = []
        for v in range(256):
            q = v // 24 if v >= 24 else 0xEE
            s = (sel + v // 24) & 0xFF if v >= 24 else 0xEE
            x = (v % 24 * 4 + 0x20) & 0xFF
            want += [v % 13, (0x11 + v // 13) & 0xFF, q, v % 24, x ^ s]
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                sim = build_sim(self._SRC % sel, optimize=optimize)
                result = sim.run(max_cycles=20_000_000)
                self.assertFalse(result.timed_out)
                base = sim.symbols["OUT"]
                got = list(sim.mpu.memory[base:base + len(want)])
                self.assertEqual(got, want)


if __name__ == "__main__":
    unittest.main()
