"""Tests for `passes/slot_coalescing.py`: a zero-page local holding a
copy of a parameter takes the parameter's own slot.

The tests that called the Python functions directly are doctest cases of
the C++ port now, each named for the test it came from
(docs/cpp_port.md, step 8, group G6)."""

from __future__ import annotations

import shutil
import unittest

from sim.harness import build_sim
from tests.no_inlining import no_inlining


# `mul` takes the starting product first, which arrives in A and X, so
# that `a` and `b` arrive in their slots, where `a <<= 1` and `b >>= 1`
# shift them in place.
_MULU16 = """
#include <stdint.h>
uint16_t t[6] = {0, 1, 7, 255, 300, 65535};
static uint16_t mul(uint16_t product, uint16_t a, uint16_t b) {
    while (b != 0) {
        if (b & 1)
            product += a;
        a <<= 1;
        b >>= 1;
    }
    return product;
}
static uint16_t div(uint16_t dividend, uint16_t divisor) {
    uint16_t quotient = dividend;
    uint16_t partial = 0;
    for (uint8_t i = 17; i != 0; i--) {
        uint8_t fits = partial >= divisor;
        if (fits)
            partial -= divisor;
        uint8_t out = (quotient & 0x8000) != 0;
        quotient = (quotient << 1) | fits;
        partial = (partial << 1) | out;
    }
    return quotient;
}
int main(void) {
    uint16_t n = 0;
    for (uint8_t i = 0; i < 6; i++)
        for (uint8_t j = 0; j < 6; j++)
            n = (uint16_t)(n * 3u + mul(0, t[i], t[j])
                           + (t[j] ? div(t[i], t[j]) : 0));
    return n;
}
"""


def _expected() -> int:
    t = [0, 1, 7, 255, 300, 65535]
    n = 0
    for a in t:
        for b in t:
            q = a // b if b else 0
            n = (n * 3 + (a * b) + q) & 0xFFFF
    return n


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
@no_inlining()
class TestRuns(unittest.TestCase):

    def test_multiply_and_divide(self):
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                r = build_sim(_MULU16, optimize=optimize).run()
                self.assertEqual(r.return_int(), _expected())

    def test_no_copy_of_the_parameters(self):
        from compile import _run_source
        asm = _run_source("codegen", _MULU16, optimize=True)
        body = asm[asm.index("\nmul:"):asm.index("\ndiv:")]
        self.assertNotIn("__local_mul__a", body)
        self.assertIn("ASL   __zpabi_mul__a_0", body)
        self.assertIn("changes=__zpabi_mul__a_0", asm)


if __name__ == "__main__":
    unittest.main()
