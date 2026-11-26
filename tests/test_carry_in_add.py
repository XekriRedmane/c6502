"""Tests for `passes/carry_in_add.py`: `p + b + 1` as one add with the
carry set.

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
static const uint8_t table[] = {3, 'a', 'b', 'c', 0, 255};
static const uint8_t *after(const uint8_t *s) { return s + s[0] + 1; }
static uint16_t sum(uint16_t p, uint8_t b) { return p + b + 1; }
uint16_t main(void) {
    static const uint16_t ps[] = {0, 1, 0xFF, 0x1FF, 0xFFFE, 0xFFFF};
    static const uint8_t bs[] = {0, 1, 0x7F, 0xFE, 0xFF};
    uint16_t n = (uint16_t)(after(table) - table);
    n = n * 3 + (uint16_t)(after(table + 4) - table);
    for (uint8_t i = 0; i < 6; i++)
        for (uint8_t j = 0; j < 5; j++)
            n = n * 7 + sum(ps[i], bs[j]);
    return n;
}
"""


def _expected() -> int:
    n = 4
    n = (n * 3 + 5) & 0xFFFF
    for p in [0, 1, 0xFF, 0x1FF, 0xFFFE, 0xFFFF]:
        for b in [0, 1, 0x7F, 0xFE, 0xFF]:
            n = (n * 7 + ((p + b + 1) & 0xFFFF)) & 0xFFFF
    return n


@needs_c6502cpp
class TestRuns(unittest.TestCase):

    def test_values(self):
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                r = build_sim(_SRC, optimize=optimize).run()
                self.assertEqual(r.return_int(), _expected())

    @no_inlining()
    def test_one_add(self):
        from compile import _run_source
        asm = _run_source("codegen", _SRC, optimize=True)
        body = asm[asm.index("\nafter:"):asm.index("\nsum:")]
        self.assertIn("SEC", body)
        self.assertNotIn("#$01", body)


if __name__ == "__main__":
    unittest.main()
