"""Tests for `cpp/src/passes/ret_slot_coalescing.h`: a local pair holding a
copy of `F@ret` becomes `F@ret`.

The tests that called the Python functions directly are doctest cases of
the C++ port now, each named for the test it came from
(docs/cpp_port.md, step 8, group G6).

The `-S` text is checked in cpp/tests/test_ret_slot_coalescing.cpp."""

from __future__ import annotations

import unittest

from sim.harness import build_sim
from tests.cpp_build import needs_c6502cpp


_SRC = """
#include <stdint.h>
static const uint8_t text[] = {2, 'a', 'b', 1, 'c', 0, 3, 'd', 'e', 'f', 0};
uint16_t total;
static const uint8_t *next(const uint8_t *s) { return s + s[0] + 1; }
static void add(uint8_t c) { total = total * 3 + c; }
static const uint8_t *group(uint8_t index) {
    const uint8_t *p = text;
    for (; index != 0; index--) {
        while (*p != 0)
            p = next(p);
        p++;
    }
    return p;
}
static void walk(const uint8_t *p) {
    while (*p != 0) {
        add(*p);
        add(p[1]);
        p = next(p);
        if (*p == 0)
            add(0xFF);
    }
}
uint16_t main(void) {
    walk(group(0));
    walk(group(1));
    return total;
}
"""


def _expected() -> int:
    total = 0

    def add(c):
        nonlocal total
        total = (total * 3 + c) & 0xFFFF
    text = [2, ord('a'), ord('b'), 1, ord('c'), 0, 3, ord('d'), ord('e'),
            ord('f'), 0]
    for start in (0, 6):
        p = start
        while text[p] != 0:
            add(text[p])
            add(text[p + 1])
            p = p + text[p] + 1
            if text[p] == 0:
                add(0xFF)
    return total


@needs_c6502cpp
class TestRuns(unittest.TestCase):

    def test_values(self):
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                r = build_sim(_SRC, optimize=optimize).run()
                self.assertEqual(r.return_int(), _expected())


if __name__ == "__main__":
    unittest.main()
