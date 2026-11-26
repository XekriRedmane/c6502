"""cpp/src/passes/constant_store_order.h: a run of stores grouped by the value
stored, constant or loaded from memory, so each value is loaded once.

The tests that called the Python functions directly are doctest cases of
the C++ port now, each named for the test it came from
(docs/cpp_port.md, step 8, group G6)."""

import unittest

from sim.harness import build_sim
from tests.cpp_build import needs_c6502cpp


@needs_c6502cpp
class TestLoadedValueStoreOrderSim(unittest.TestCase):
    """Programs with the shape of `fill_hole`, run in both pipelines."""

    # Each brick whose timer runs out copies its row and column into
    # locals and globals, and the loop counter stays in X. The values
    # include 0, 128, and 255.
    _FILL = """
#include <stdint.h>
const uint8_t ROWS[4] = {3, 0, 255, 7};
const uint8_t COLS[4] = {9, 128, 1, 0};
uint8_t TIMERS[4] = {1, 2, 1, 1};
uint8_t MAP[8][10];
uint8_t GROW, GCOL;
uint16_t sum;

static void fill(uint8_t i) {
    uint8_t row = ROWS[i];
    uint8_t col = COLS[i];
    GROW = row;
    GCOL = col;
    uint8_t r = row & 7;
    uint8_t c = col % 10;
    MAP[r][c] += 1;
    sum += (uint16_t)(row * 3) + col + MAP[r][c];
}

int main(void) {
    for (uint8_t i = 3; (int8_t)i >= 0; i--) {
        if (--TIMERS[i] != 0)
            continue;
        fill(i);
    }
    return sum + GROW + GCOL;
}
"""

    # `T[1] = col` writes a byte that `T[i]` reads on the next iteration
    # of the loop, so the value of `row` can't be loaded again
    # after it.
    _ALIAS = """
#include <stdint.h>
uint8_t T[4] = {5, 6, 7, 8};
const uint8_t C[4] = {1, 2, 3, 4};
uint8_t G, H;

int main(void) {
    uint16_t s = 0;
    for (uint8_t i = 3; (int8_t)i >= 0; i--) {
        uint8_t row = T[i];
        uint8_t col = C[i];
        T[1] = col;
        G = row;
        H = col;
        s += (uint16_t)(row * 16) + col + T[i];
    }
    return s + G + H + T[1];
}
"""

    def test_fill_runs(self):
        # Bricks 3, 2, and 0 fill: 808 summed, then GROW = 3, GCOL = 9.
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                got = build_sim(self._FILL, optimize=optimize).run()
                self.assertEqual(got.return_int(), 820)

    def test_fill_loads_each_byte_once(self):
        from compile import _run_source
        asm = _run_source("codegen", self._FILL, optimize=True)
        lines = asm.split("\nmain:\n")[1].splitlines()
        # The first load of each table byte stores it into the local
        # and then into the global.
        for table, glob in (("ROWS", "GROW"), ("COLS", "GCOL")):
            load = f"   LDA   {table},X"
            self.assertEqual(lines.count(load), 1)
            at = lines.index(load)
            self.assertTrue(lines[at + 1].startswith("   STA   __local_main__"))
            self.assertEqual(lines[at + 2], f"   STA   {glob}")

    def test_store_into_source_runs(self):
        # The rows read are 8, 7, 3 (stored by the iteration before), and 5.
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                got = build_sim(self._ALIAS, optimize=optimize).run()
                self.assertEqual(got.return_int(), 407)


if __name__ == "__main__":
    unittest.main()
