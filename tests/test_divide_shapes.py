"""Tests for the pieces that make a shift-and-subtract divide loop come
out as the 6502 code a person writes, as Zork's `divu16`
(examples/zork/numbers.c) does:

- `tac_to_asm._fused_subtract` branches on the flags of a subtract
  with copies between it and the compare.
- `tac_to_asm._rotate_in` lowers `(x << 1) | b`, for a byte `b`
  that's 0 or 1, as `LDA b; LSR` and a `ROL` of each byte, and
  drops the `LDA b; LSR` when C already holds `b`.
- `memory_value_propagation` reads a local array through `name,Y`
  when the pointer holds the array's address.
- `asm_liveness.sets_flags` and `asm_aliasing.may_alias` for the
  stores and indexed accesses those leave.

The `-S` text is checked in cpp/tests/test_divide_shapes.cpp."""

from __future__ import annotations

import unittest

from sim.harness import build_sim
from tests.no_inlining import no_inlining
from tests.cpp_build import needs_c6502cpp

_DIVIDE = """
#include <stdint.h>
uint16_t remainder;
uint16_t t[7] = {0, 1, 7, 10, 255, 1000, 65535};
static uint16_t divide(uint16_t dividend, uint16_t divisor) {
    uint16_t quotient = dividend;
    uint16_t partial = 0;
    for (uint8_t i = 17; i != 0; i--) {
        uint8_t fits = 0;
        if (partial >= divisor) {
            partial -= divisor;
            fits = 1;
        }
        uint8_t out = (uint8_t)(quotient >> 15);
        quotient = (quotient << 1) | fits;
        partial = (partial << 1) | out;
    }
    remainder = partial >> 1;
    return quotient;
}
int main(void) {
    uint16_t n = 0;
    for (uint8_t i = 0; i < 7; i++)
        for (uint8_t j = 1; j < 7; j++)
            n = (uint16_t)(n * 3u + divide(t[i], t[j]) + remainder);
    return n;
}
"""

_DIGITS = """
#include <stdint.h>
uint8_t out[6];
uint8_t n_out;
static void put(uint8_t c) { out[n_out++] = c; }
static void digits(uint16_t n) {
    uint8_t d[5];
    uint8_t count = 0;
    do {
        d[count] = (uint8_t)(n % 10);
        n /= 10;
        count++;
    } while (n != 0);
    do {
        count--;
        put(d[count] + '0');
    } while (count != 0);
}
int main(void) {
    digits(40321);
    return (out[0] - '0') * 10 + (out[4] - '0');
}
"""


def _expected_divide() -> int:
    t = [0, 1, 7, 10, 255, 1000, 65535]
    n = 0
    for a in t:
        for b in t[1:]:
            n = (n * 3 + a // b + a % b) & 0xFFFF
    return n


@needs_c6502cpp
@no_inlining()
class TestDivideLoop(unittest.TestCase):

    def test_runs(self):
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                r = build_sim(_DIVIDE, optimize=optimize).run()
                self.assertEqual(r.return_int(), _expected_divide())


@needs_c6502cpp
class TestLocalArrayIndexed(unittest.TestCase):

    def test_runs(self):
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                r = build_sim(_DIGITS, optimize=optimize).run()
                self.assertEqual(r.return_int(), 41)


if __name__ == "__main__":
    unittest.main()
