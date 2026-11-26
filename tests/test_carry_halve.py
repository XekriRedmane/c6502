"""Tests for `cpp/src/passes/carry_halve.h`: a halved 9-bit sum takes bit 8
from the carry of its low byte's add.

The tests that called the Python functions directly are doctest cases of
the C++ port now, each named for the test it came from
(docs/cpp_port.md, step 8, group G6)."""

from __future__ import annotations

import unittest

from sim.harness import build_sim
from tests.no_inlining import no_inlining
from tests.cpp_build import needs_c6502cpp


_SRC = """
#include <stdint.h>
uint8_t col;
static uint8_t half(uint8_t a, uint8_t b) { return (a + b) / 2; }
static uint8_t round_up(uint8_t s) { return (s + 1) >> 1; }
static void center(uint8_t s) { col += (s + 1) / 2; }
uint16_t main(void) {
    static const uint8_t v[] = {0, 1, 2, 127, 128, 200, 254, 255};
    uint16_t n = 0;
    for (uint8_t i = 0; i < 8; i++) {
        for (uint8_t j = 0; j < 8; j++)
            n = n * 7 + half(v[i], v[j]);
        col = 3;
        center(v[i]);
        n = n * 5 + round_up(v[i]) + col;
    }
    return n;
}
"""


def _expected() -> int:
    v = [0, 1, 2, 127, 128, 200, 254, 255]
    n = 0
    for a in v:
        for b in v:
            n = (n * 7 + (a + b) // 2) & 0xFFFF
        col = (3 + (a + 1) // 2) & 0xFF
        n = (n * 5 + ((a + 1) >> 1) + col) & 0xFFFF
    return n


@needs_c6502cpp
class TestRuns(unittest.TestCase):

    def test_values(self):
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                r = build_sim(_SRC, optimize=optimize).run()
                self.assertEqual(r.return_int(), _expected())

    @no_inlining()
    def test_no_high_byte(self):
        from compile import _run_source
        asm = _run_source("codegen", _SRC, optimize=True)
        body = asm[asm.index("\nhalf:"):asm.index("\nmain:")]
        self.assertNotIn("LSR", body)
        self.assertEqual(body.count("ROR"), 3)


if __name__ == "__main__":
    unittest.main()
