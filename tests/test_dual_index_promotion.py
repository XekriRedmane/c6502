"""Tests for the X→Y dual-index promotion pass.

The tests that called the Python functions directly are doctest cases of
the C++ port now, each named for the test it came from
(docs/cpp_port.md, step 8, group G6)."""

import shutil
import unittest

from sim.harness import build_sim


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not installed")
class TestDualIndexPromotionSim(unittest.TestCase):

    def _check(self, src, expected):
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                r = build_sim(src, optimize=optimize).run()
                self.assertFalse(r.timed_out)
                self.assertEqual(r.return_int(), expected)

    def test_store_after_branch(self):
        # `v[d] = a` sits after `if (a != 0)`. The optimized build
        # used to store to `v[k]`.
        self._check(r'''
        #include <stdint.h>
        uint8_t t[8], u[8] = {0,0,0,0,0,7,0,0}, v[8], w[8];
        uint8_t o, o2;
        void f(uint8_t k, uint8_t d)
        {
            o = t[k];
            uint8_t a = u[d];
            if (a != 0)
                v[d] = a;
            o = t[k];
            o2 = w[d];
        }
        int main(void) { f(2, 5); return v[5] * 10 + v[2]; }
        ''', 70)

    def test_pointer_read_between_uses(self):
        # `*p` through a zero-page pointer loads Y with 0, so the
        # optimized build used to read `w[0]` where it should read
        # `w[d]`.
        self._check(r'''
        #include <stdint.h>
        uint8_t t[8], u[8], w[8] = {0,0,0,0,0,9,0,0};
        uint8_t o, o2, o3, cell;
        void f(uint8_t k, uint8_t d, uint8_t *p)
        {
            o = t[k];
            o2 = u[d];
            o = t[k];
            o3 = *p;
            o2 = w[d];
        }
        int main(void) { f(2, 5, &cell); return o2; }
        ''', 9)


if __name__ == "__main__":
    unittest.main()
