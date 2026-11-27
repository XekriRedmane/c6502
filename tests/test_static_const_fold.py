"""Tests for the TAC-level scalar const-static fold +
const-array-subscript fold + Add reassociation.

Three composable optimizations:

  1. `cpp/src/passes/optimization/static_const_fold.h` replaces
     `Var(static_const_scalar)` USE positions with
     `Constant(value)` so downstream constant folding can collapse
     the resulting arithmetic.
  2. `_fold_indexed_load` in `cpp/src/passes/optimization/constant_folding.h`
     folds `IndexedLoad(static_const_array, Constant(byte_idx))` to
     a single Constant (the array element's value) when the index
     is element-aligned and the array's element type is const-
     qualified.
  3. `cpp/src/passes/optimization/reassoc_const.h` collapses
     `Constant(C1) + (Constant(C2) + V)` into `Constant(C1+C2) + V`
     so two nested 16-bit Adds become one.

Together they turn `hires_page1[interlace_p1_offsets[2] + col]`
(when both `hires_page1` and `interlace_p1_offsets` are
const-qualified statics) into a single 16-bit `Add(0x21D0, col)`
at runtime — half the address-arithmetic work, plus all the
intermediate temp slots freed up.

The `-S` text is checked in cpp/tests/test_static_const_fold.cpp.
"""
from __future__ import annotations

import unittest

from sim.harness import build_sim
from tests.cpp_build import needs_c6502cpp


@needs_c6502cpp
class TestStaticConstFoldCorrectness(unittest.TestCase):
    """End-to-end: optimized programs compute the same answers as
    unoptimized ones."""

    def test_static_const_int_value_returned(self) -> None:
        src = (
            "static const int magic = 0x1234;\n"
            "int main(void) { return magic + 1; }\n"
        )
        sim = build_sim(src, optimize=True)
        result = sim.run(max_cycles=5_000_000)
        self.assertFalse(result.timed_out)
        self.assertEqual(result.return_int() & 0xFFFF, 0x1235)

    def test_const_array_subscript_value_returned(self) -> None:
        src = (
            "#include <stdint.h>\n"
            "static const uint16_t arr[3] = {0x1234, 0x5678, 0x9ABC};\n"
            "int main(void) { return arr[2]; }\n"
        )
        sim = build_sim(src, optimize=True)
        result = sim.run(max_cycles=5_000_000)
        self.assertFalse(result.timed_out)
        self.assertEqual(result.return_int() & 0xFFFF, 0x9ABC)

    def test_pointer_const_static_indexed_write(self) -> None:
        # The headline case: a `static T * const` initialized to a
        # raw address, indexed by a const + runtime-1-byte sum.
        # Verifies the byte lands at the right memory address.
        src = (
            "#include <stdint.h>\n"
            "static uint8_t * const buf = (uint8_t * const)0x4000;\n"
            "static const uint16_t offsets[3] = {0x100, 0x200, 0x300};\n"
            "int main(void) { buf[offsets[1] + 5] = 0x42; return 0; }\n"
        )
        sim = build_sim(src, optimize=True)
        result = sim.run(max_cycles=5_000_000)
        self.assertFalse(result.timed_out)
        # Target address: 0x4000 + 0x200 + 5 = 0x4205.
        self.assertEqual(result.memory[0x4205], 0x42)

    def test_reassoc_runtime_correctness(self) -> None:
        # Verify that the reassoc rewrite preserves runtime
        # behavior — the answer must match what the unoptimized
        # arithmetic would compute.
        src = (
            "static const int a = 100;\n"
            "static const int b = 200;\n"
            "int main(int col) { return a + b + col; }\n"
        )
        sim = build_sim(src, optimize=True)
        # main is called with no args; the synthesizer puts a
        # default 0 in col, so result = 300.
        result = sim.run(max_cycles=5_000_000)
        self.assertFalse(result.timed_out)
        self.assertEqual(result.return_int() & 0xFFFF, 300)


if __name__ == "__main__":
    unittest.main()
