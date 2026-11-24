"""Tests for the shared asm-level aliasing predicate.

The tests that called `may_alias` directly are doctest cases of the C++
port now, each named for the test it came from, in
cpp/tests/test_asm_aliasing.cpp (docs/cpp_port.md, step 8, group G6).
"""
from __future__ import annotations

import unittest

from sim.harness import build_sim


class TestZeroPagePointers(unittest.TestCase):
    """`(zp),Y` through a pointer kept in zero page. One base is one
    pointer, so its offsets decide; two bases are two pointers that
    may hold the same address."""

    _TWO_POINTERS = """
        unsigned char buf[4];
        unsigned char g(unsigned char *p, unsigned char *q) {
            unsigned char s = 0;
            for (unsigned char i = 0; i < 4; i++) {
                p[i] = i + 1;
                s += q[i];
                q[i] = 9;
                s += p[i];
            }
            return s;
        }
        int main(void) { return g(buf, buf); }
    """

    def test_two_pointers_to_one_buffer_run(self) -> None:
        # Under -O `p[i]` and `q[i]` are `(zp),Y` through two
        # bases; the read through `p` after the store through `q`
        # must see the 9. (1 + 2 + 3 + 4) + 4 * 9 = 46.
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                result = build_sim(
                    self._TWO_POINTERS, optimize=optimize,
                ).run()
                self.assertEqual(result.return_int(), 46)
