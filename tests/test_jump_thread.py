"""Tests for cpp/src/passes/optimization/jump_thread.h.

The pass sends an edge into a join of a `Phi` and a test of it straight
to the successor the test picks for the constant that the edge brings.
The unit tests run it on SSA functions built by hand, and the
simulator tests check programs whose inlined functions return a
constant on some paths, through both outcomes, in both pipelines.
"""
from __future__ import annotations

import unittest

from sim.harness import build_sim


class TestPrograms(unittest.TestCase):
    """`find` is inlined into `f`, which tests its result. Each case
    runs both the `return true;` and the `return false;` paths."""

    _SOURCE = """
        #include <stdint.h>
        #include <stdbool.h>
        uint8_t a[8], n, hits;
        static bool find(uint8_t v) {
            uint8_t i = n;
            do { if (a[i] == v) { hits++; return true; } } while (--i != 0);
            return false;
        }
        uint8_t f(uint8_t v) { if (find(v)) return 10; hits += 2; return 20; }
        int main(void) {
            n = 7;
            for (uint8_t k = 0; k < 8; k++) a[k] = k * 3;
            uint8_t r = 0;
            r += f(%d);
            return r * 256 + hits;
        }
    """

    def test_found_and_not_found(self):
        # 9 is a[3]; 4 isn't in the array; a[0] = 0 is never compared,
        # since the loop stops when i reaches 0; 21 is a[7], the first
        # compared.
        for v, expected in ((9, 10 * 256 + 1), (4, 20 * 256 + 2),
                            (0, 20 * 256 + 2), (21, 10 * 256 + 1)):
            for optimize in (False, True):
                with self.subTest(v=v, optimize=optimize):
                    result = build_sim(
                        self._SOURCE % v, optimize=optimize,
                    ).run()
                    self.assertFalse(result.timed_out)
                    self.assertEqual(result.return_int(), expected)

    def test_and_or_results(self):
        src = """
            #include <stdint.h>
            uint8_t x, y, z;
            static uint8_t pick(void) {
                if (x == 0 && (y != 0 || z != 0)) return 3;
                if (x == 5 || y == 6) return 4;
                return 5;
            }
            int main(void) {
                int r = 0;
                static const uint8_t xs[] = {0, 0, 0, 5, 1};
                static const uint8_t ys[] = {0, 1, 0, 0, 6};
                static const uint8_t zs[] = {0, 0, 1, 0, 0};
                for (uint8_t k = 0; k < 5; k++) {
                    x = xs[k]; y = ys[k]; z = zs[k];
                    r = r * 8 + pick();
                }
                return r;
            }
        """
        # x, y, z = 0, 0, 0 gives 5; 0, 1, 0 and 0, 0, 1 give 3; and
        # 5, 0, 0 and 1, 6, 0 give 4. One octal digit each.
        expected = 0o53344
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                result = build_sim(src, optimize=optimize).run()
                self.assertFalse(result.timed_out)
                self.assertEqual(result.return_int(), expected)


if __name__ == "__main__":
    unittest.main()
