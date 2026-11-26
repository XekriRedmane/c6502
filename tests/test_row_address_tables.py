"""Tests for passes/optimization/row_address_tables.py, which looks up
the address of a row of a 2-D array in a table of row addresses."""

from __future__ import annotations

import shutil
import unittest

from compile import _run_source
from sim.harness import build_sim

_HAVE_PCPP = shutil.which("pcpp") is not None


def _codegen(src: str) -> str:
    return _run_source("codegen", src, optimize=True)


def _cell(r: int, c: int) -> int:
    """What `_FILL` stores in row `r`, column `c`."""
    return (r * 16 + c) & 0xFF


@unittest.skipUnless(_HAVE_PCPP, "pcpp CLI not available")
class TestRowAddressTablesSim(unittest.TestCase):
    """Programs that index 2-D arrays through row tables, run in both
    pipelines. Each program reads its array often enough that the
    accesses save more than the tables cost, so that `-O` makes them.
    (Until step 8 of the port, these tests replaced the Python pass's
    estimate to force the tables, which the C++ pass's doctest cases now
    do with `fixed_saving`.)"""

    def _check(self, src: str, expected: int, table: str) -> None:
        self.assertIn(f"\n{table}@lo:\n", _codegen(src))
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                got = build_sim(src, optimize=optimize).run().return_int()
                self.assertEqual(got, expected & 0xFFFF)

    _FILL = """
        for (uint8_t r = 0; r < ROWS; r++)
            for (uint8_t c = 0; c < COLS; c++)
                A[r][c] = (uint8_t)(r * 16 + c);
    """

    def test_fixed_address_array(self):
        src = """
        #include <stdint.h>
        #define ROWS 16
        #define COLS 28
        extern uint8_t A [[c6502::address(0x6000)]][ROWS][COLS];
        uint8_t get(uint8_t r, uint8_t c) { return A[r][c]; }
        uint8_t above(uint8_t r, uint8_t c) { return A[r - 1][c]; }
        uint8_t below(uint8_t r, uint8_t c) { return A[r + 1][c]; }
        void put(uint8_t r, uint8_t c, uint8_t v) { A[r][c] = v; }
        uint8_t *row(uint8_t r) { return A[r]; }
        uint8_t *cell(uint8_t r, uint8_t c) { return &A[r][c]; }
        uint8_t (*whole(uint8_t r))[COLS] { return &A[r]; }
        int main(void) {
            """ + self._FILL + """
            put(5, 6, 200);
            int s = get(5, 6) + get(15, 27) + above(1, 3) + below(14, 0);
            s += row(2)[4] + *cell(3, 7) + (*whole(4))[1];
            // One past the last row.
            if ((uint8_t *)whole(ROWS) != &A[0][0] + ROWS * COLS) return -1;
            if (row(ROWS) != &A[0][0] + ROWS * COLS) return -2;
            return s;
        }
        """
        self._check(
            src,
            200 + _cell(15, 27) + _cell(0, 3) + _cell(15, 0) + _cell(2, 4)
            + _cell(3, 7) + _cell(4, 1),
            "A",
        )

    def test_static_array(self):
        src = """
        #include <stdint.h>
        #define ROWS 20
        #define COLS 14
        static uint8_t A[ROWS][COLS];
        uint8_t get(uint8_t r, uint8_t c) { return A[r][c]; }
        uint8_t above(uint8_t r, uint8_t c) { return A[r - 1][c]; }
        uint8_t *end(uint8_t r) { return A[r]; }
        int main(void) {
            """ + self._FILL + """
            if (end(ROWS) != &A[0][0] + ROWS * COLS) return -1;
            return get(19, 13) + get(0, 0) + above(1, 2) + get(7, 1);
        }
        """
        self._check(
            src, _cell(19, 13) + _cell(0, 0) + _cell(0, 2) + _cell(7, 1), "A",
        )

    def test_two_byte_elements(self):
        # Rows of 7 uint16_t, 14 bytes.
        src = """
        #include <stdint.h>
        static uint16_t W[30][7];
        uint16_t get(uint8_t r, uint8_t c) { return W[r][c]; }
        void put(uint8_t r, uint8_t c, uint16_t v) { W[r][c] = v; }
        int main(void) {
            for (uint8_t r = 0; r < 30; r++)
                for (uint8_t c = 0; c < 7; c++)
                    put(r, c, (uint16_t)(r * 1000 + c));
            return get(29, 6) - get(3, 2) + get(0, 1);
        }
        """
        self._check(src, (29 * 1000 + 6) - (3 * 1000 + 2) + 1, "W")

    def test_array_of_structs(self):
        src = """
        #include <stdint.h>
        struct cell { uint8_t a, b, c; };
        static struct cell S[90];
        uint8_t a_of(uint8_t i) { return S[i].a; }
        uint8_t b_of(uint8_t i) { return S[i].b; }
        uint8_t c_of(uint8_t i) { return S[i].c; }
        void set_a(uint8_t i, uint8_t v) { S[i].a = v; }
        void set_b(uint8_t i, uint8_t v) { S[i].b = v; }
        void set_c(uint8_t i, uint8_t v) { S[i].c = v; }
        int main(void) {
            for (uint8_t i = 0; i < 90; i++) {
                set_a(i, i); set_b(i, i + 1); set_c(i, i + 2);
            }
            return a_of(89) + b_of(0) + c_of(45);
        }
        """
        self._check(src, 89 + 1 + 47, "S")

    def test_shared_low_bytes(self):
        src = """
        #include <stdint.h>
        extern uint8_t M [[c6502::address(0x6000)]][16][28];
        extern uint8_t B [[c6502::address(0x6200)]][16][28];
        uint8_t both(uint8_t r, uint8_t c) { return M[r][c] + B[r][c]; }
        int main(void) {
            M[3][4] = 10; B[3][4] = 20; M[15][27] = 1; B[15][27] = 2;
            return both(3, 4) * 10 + both(15, 27);
        }
        """
        self._check(src, 300 + 3, "B")
        text = _codegen(src)
        self.assertNotIn("M@lo", text)
        tables = text.split("\nB@lo:\n")[1]
        self.assertIn("\nB@hi:\n", tables)
        self.assertIn("\nM@hi:\n", tables)
        self.assertEqual(tables.count("$"), 51)


if __name__ == "__main__":
    unittest.main()
