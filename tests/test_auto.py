"""Tests for C23's `auto` (§6.7.2, §6.7.10): with no type specifier it
infers an object's type from its initializer, and with one it's C99's
automatic storage class, which C23 allows only at block scope. Most
cases are the standard's own examples."""

from __future__ import annotations

import shutil
import unittest

from preprocessor import preprocess
from sim.harness import build_sim
from tests.test_sim_chapters import _run_program as _run_tac


_PROGRAM = """\
static auto a = 3.5;
auto p = &a;

int count(void) {
    static auto n = 0;
    return ++n;
}

int main(void) {
    double x = 7;
    double y = 9;
    {
        auto y = x * x;
        auto x = y;
        if (x != 49.0) return 1;
    }
    if (*p != 3.5) return 2;
    auto u = (unsigned char)250;
    u += 10;
    if (u != 4) return 3;
    auto s = 0L;
    for (auto i = 0; i < 300; i++)
        s += i;
    if (s != 44850L) return 4;
    count();
    count();
    if (count() != 3) return 5;
    auto int z = 8;
    return z - 8;
}
"""


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestPrograms(unittest.TestCase):
    """`u` is an `unsigned char`, so it wraps at 256, and `s` a `long`,
    so the sum of 0 to 299 fits."""

    def test_tac_sim(self) -> None:
        self.assertEqual(_run_tac(preprocess(_PROGRAM)), 0)

    def test_asm_sim(self) -> None:
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                result = build_sim(_PROGRAM, optimize=optimize).run(
                    max_cycles=4_000_000,
                )
                self.assertFalse(result.timed_out)
                self.assertEqual(result.return_int_signed(), 0)


if __name__ == "__main__":
    unittest.main()
