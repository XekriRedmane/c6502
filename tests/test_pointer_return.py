"""A function returning a pointer returns it in A (low byte) and X
(high byte), as an `unsigned int` comes back, under both calling
conventions and through a function pointer."""

import unittest

from sim.harness import build_sim
from tests.cpp_build import needs_c6502cpp


@needs_c6502cpp
class TestPointerReturn(unittest.TestCase):

    _PROGRAM = """
    #include <stdint.h>
    uint8_t table[8] = {10, 20, 30, 40, 50, 60, 70, 80};
    uint8_t *gp;
    /* zp_abi under -O. */
    static uint8_t *entry(uint8_t i) { return table + i; }
    /* A soft-stack function in both pipelines, since it recurses. */
    static uint8_t *walk(uint8_t *p, uint8_t n) {
        if (n == 0) return p;
        return walk(p + 1, n - 1);
    }
    /* Called through a pointer. */
    static uint8_t *last(void) { return &table[7]; }
    static uint8_t *(*pick)(void) = last;
    int main(void) {
        uint8_t *a = entry(2);
        uint8_t *b = walk(table, 5);
        uint8_t *c = pick();
        gp = entry(1);
        *gp += 1;
        return *a + *b + *c + table[1] + (int)(b - a);
    }
    """

    def test_values(self):
        # *a = 30, *b = 60, *c = 80, table[1] = 21, b - a = 3.
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                out = build_sim(self._PROGRAM, optimize=optimize).run(
                    max_cycles=5_000_000,
                )
                self.assertFalse(out.timed_out)
                self.assertEqual(out.return_int(), 30 + 60 + 80 + 21 + 3)

    def test_no_hargs(self):
        from compile import _run_source
        asm = _run_source("codegen", self._PROGRAM, optimize=True)
        self.assertNotIn("HARGS+1", asm.split("@zp-link-meta-end")[1])


if __name__ == "__main__":
    unittest.main()
