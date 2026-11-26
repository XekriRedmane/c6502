"""Unit tests for `apply_remat` (cpp/src/passes/asm_remat.h).

Covers the single-atom mem-to-mem staging shape, the two-atom
`Mov(<recomp>, Reg(A)); Mov(Reg(A), Data(local))` shape (post-
SSA round-trip), and the dead-stage-dst collapse that converts a
no-longer-needed `Mov(_, Data(local))` to a bare `Mov(_, Reg(A))`
(or omits it entirely when the src is already `Reg(A)`).

The tests that called the Python functions directly are doctest cases of
the C++ port now, each named for the test it came from
(docs/cpp_port.md, step 8, group G6).
"""

import unittest
from tests import cpp_build


class TestRematPointerStore(unittest.TestCase):
    """A store through a pointer between the load and the read may
    write the static, unless the static is internal and its address
    is never formed. `uint8_t t = g; *p = 7; return t;` called with
    `p == &g` returned 7 instead of the old value of `g`."""

    def test_simulated_pointer_store_to_global(self):
        if not cpp_build.BUILT:
            self.skipTest(cpp_build.REASON)
        from sim.harness import build_sim
        src = (
            "unsigned char g;\n"
            "unsigned char f(unsigned char *p) {\n"
            "    unsigned char t = g;\n"
            "    *p = 7;\n"
            "    return t;\n"
            "}\n"
            "int main(void) { g = 3; return f(&g); }\n"
        )
        self.assertEqual(build_sim(src, optimize=True).run().return_int(), 3)


if __name__ == "__main__":
    unittest.main()


class TestRematCostSim(unittest.TestCase):
    """The shapes of `TestRematCost` and of Lode Runner's
    `enable_next_level_ladders` (a table byte copied into a local and
    a zero-page global, the local read again after a call) run to the
    same results in both pipelines."""

    _SRC = r'''
#include <stdint.h>
uint8_t ROWS[6] = {1, 4, 9, 16, 25, 36};
uint8_t COLS[6] = {2, 3, 5, 7, 11, 13};
uint8_t GR [[c6502::zeropage]];
uint8_t GC [[c6502::zeropage]];
uint16_t sum;
void touch(void) { sum += GR + GC; }
int main(void) {
    for (uint8_t i = 5; i != 0; i--) {
        uint8_t col = COLS[i];
        if (col == 5)
            continue;
        uint8_t row = ROWS[i];
        GR = row;
        GC = col;
        touch();
        sum += (uint16_t)(row * 3) + col;
    }
    return sum;
}
'''

    def test_runs(self):
        if not cpp_build.BUILT:
            self.skipTest(cpp_build.REASON)
        from sim.harness import build_sim
        rows = [1, 4, 9, 16, 25, 36]
        cols = [2, 3, 5, 7, 11, 13]
        want = 0
        for i in range(5, 0, -1):
            if cols[i] == 5:
                continue
            want += rows[i] + cols[i] + rows[i] * 3 + cols[i]
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                got = build_sim(self._SRC, optimize=optimize).run()
                self.assertEqual(got.return_int(), want)
