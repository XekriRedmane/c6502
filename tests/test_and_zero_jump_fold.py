"""Tests for the TAC pass and_zero_jump_fold
(cpp/src/passes/optimization/and_zero_jump_fold.h).

The pass rewrites `Binary(BitwiseAnd, %ext, ConstInt(C), %res);
JumpIfTrue/False(%res, t)` — where `%ext` traces through a
`ZeroExtend` from a 1-byte unsigned var, `C` fits in 0..255, and
`%res` is single-use — as a single `JumpIfMasked` on the
narrow source. The motivating C idiom is `if (uchar & 0x80)`,
which previously emitted a 2-byte AND + zero-extend + reload
chain (~6 asm instructions) and now collapses through the
existing `and_sign_bit_branch` peephole to `LDA u; BPL t` (2
instructions).

The folds for both senses, other masks than 0x80, the constant on
either side of the AND, and the guards (a multi-use %res, a source that
isn't an unsigned char, a mask above 0xFF) are doctest cases in
cpp/tests/test_fold_producer_jumps_calls.cpp. The tests here run
programs with the idiom in the simulator under `-O` and check the
value each returns. `test_sim_differential` checks that the optimized
and unoptimized pipelines agree, and these catch a regression before
the full chapter sweep runs.
"""
from __future__ import annotations

import unittest

from sim.harness import build_sim
from tests.cpp_build import needs_c6502cpp


@needs_c6502cpp
class TestSimIntegration(unittest.TestCase):
    """End-to-end: a small C program exercising the (uchar & 0x80)
    idiom returns the right value under -O."""

    def _run(self, src: str) -> int:
        sim = build_sim(src, optimize=True)
        r = sim.run(max_cycles=200_000)
        self.assertFalse(r.timed_out)
        return r.return_int()

    def test_high_bit_set(self) -> None:
        src = """
        #include <stdint.h>
        int test(uint8_t flags) {
            if (flags & 0x80) return 1;
            return 0;
        }
        int main(void) { return test(0xFF); }
        """
        self.assertEqual(self._run(src), 1)

    def test_high_bit_clear(self) -> None:
        src = """
        #include <stdint.h>
        int test(uint8_t flags) {
            if (flags & 0x80) return 1;
            return 0;
        }
        int main(void) { return test(0x7F); }
        """
        self.assertEqual(self._run(src), 0)

    def test_arbitrary_mask(self) -> None:
        # Mask 0x10 — exercises a non-sign-bit fold path. Existing
        # peepholes don't collapse this further than `AND #$10;
        # BEQ`, but the high-byte zero stage is gone.
        src = """
        #include <stdint.h>
        int test(uint8_t flags) {
            if (flags & 0x10) return 7;
            return 3;
        }
        int main(void) { return test(0x10); }
        """
        self.assertEqual(self._run(src), 7)

    def test_store_between_extend_and_and(self) -> None:
        # The ZeroExtend reads g before the store of 0. The fold read
        # g at the jump instead, after the store, and the test never
        # passed.
        src = """
        #include <stdint.h>
        uint8_t g;
        uint8_t hits;
        void check(void) { int t = g; g = 0; if (t & 1) hits = 1; }
        int main(void) { g = 1; check(); return hits; }
        """
        sim = build_sim(src, optimize=False)
        self.assertEqual(sim.run(max_cycles=200_000).return_int(), 1)
        self.assertEqual(self._run(src), 1)


@needs_c6502cpp
class TestWideMaskInOneByte(unittest.TestCase):
    """A mask on a wide value whose set bits fall in one byte tests
    that byte: `if (zchars & 0x8000)` is `LDA zchars+1; BPL`."""

    _SRC = """
    unsigned int w;
    int s;
    unsigned long l;
    unsigned char hi(void) { return (w & 0x8000) ? 0xFF : 0; }
    unsigned char mid(void) { return (w & 0x0400) != 0; }
    unsigned char neg(void) { return (s & 0x8000) != 0; }
    unsigned char top(void) { return (l & 0x00800000UL) != 0; }
    int main(void) {
        int r = 0;
        w = 0x8400; r += hi() + mid();
        w = 0x7BFF; r += hi() * 3 + mid() * 5;
        s = -2; r += neg() * 7;
        l = 0x00812345UL; r += top() * 11;
        l = 0x007FFFFFUL; r += top() * 13;
        return r;
    }
    """

    def test_runs(self):
        want = 0xFF + 1 + 0 + 0 + 7 + 11
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                got = build_sim(self._SRC, optimize=optimize).run().return_int()
                self.assertEqual(got, want)

    def test_byte_tested(self):
        from compile import _run_source
        asm = _run_source("codegen", self._SRC, optimize=True)
        body = asm.split("\nhi:\n")[1].split("RTS")[0]
        self.assertIn("LDA   w+1", body)
        self.assertNotIn("ORA", body)

if __name__ == "__main__":
    unittest.main()
