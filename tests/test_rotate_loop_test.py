"""Tests for cpp/src/passes/rotate_loop_test.h, which moves a loop's test from
its top to its bottom, and drops the entry `JMP` when the test is
known to pass on entry.

The tests that called the Python functions directly are doctest cases of
the C++ port now, each named for the test it came from
(docs/cpp_port.md, step 8, group G6)."""

import unittest

from sim.harness import build_sim
from tests.cpp_build import needs_c6502cpp


@needs_c6502cpp
class TestRotatedLoopsRun(unittest.TestCase):
    """Loops whose test the pass moves, run in both pipelines."""

    _PROGRAM = """
    #include <stdint.h>
    uint8_t buf[8] = {5, 6, 7, 0x8D, 1, 2, 3, 4};
    uint8_t zero = 0;
    static uint8_t a_mod_3(register uint8_t a) {
        while (a >= 3) a -= 3;
        return a;
    }
    static uint8_t count(void) {
        uint8_t c = 0;
        while (buf[c] != 0x8D) c++;
        return c;
    }
    static uint8_t sum_skip(void) {
        uint8_t s = 0;
        for (uint8_t i = 0; i != 8; i++) {
            if (i == 2) continue;
            s += buf[i];
        }
        return s;
    }
    static uint8_t none(void) {
        uint8_t s = 1;
        for (uint8_t i = 0; i != zero; i++) s += i;
        return s;
    }
    int main(void) {
        for (uint8_t i = 0; i != 3; i++) buf[4 + i] = i;
        return a_mod_3(14) + 10 * count() + 100 * none()
            + 1000 * (sum_skip() & 0x0F);
    }
    """

    def test_values(self):
        # a_mod_3(14) = 2, count() = 3, none() = 1, and sum_skip() is
        # 5 + 6 + 0x8D + 0 + 1 + 2 + 4 = 159, 0x9F, whose low four
        # bits are 15.
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                out = build_sim(self._PROGRAM, optimize=optimize).run(
                    max_cycles=5_000_000,
                )
                self.assertFalse(out.timed_out)
                self.assertEqual(out.return_int(), 2 + 30 + 100 + 15000)


if __name__ == "__main__":
    unittest.main()
