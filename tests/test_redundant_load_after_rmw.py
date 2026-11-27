"""Tests for the asm-level `redundant_load_after_rmw` peephole.

The pass drops `LDA M` immediately following an in-place rmw on
the same M (DEC / INC / ASL / LSR / ROL / ROR), provided `Reg(A)`
is dead after the deletion point — every forward path kills A
before reading it. The motivating case is the rotated signed-
countdown loop tail `DEC m; LDA m; BPL .top`, which collapses to
the canonical `DEC m; BPL .top` 6502 idiom.

Coverage:
  * Direct unit tests on synthetic asm: drops the LDA when A is
    dead via fall-through kill, branch-target kill, or both;
    keeps the LDA when A is read on either path; refuses on
    non-rmw or address-mismatched cases; handles cycle (loop
    back-edge) without infinite recursion.
  * End-to-end via the optimizer pipeline: a signed-countdown
    `for` loop emits `DEC m; BPL .top` with no intervening LDA.

The tests that called the Python functions directly are doctest cases of
the C++ port now, each named for the test it came from
(docs/cpp_port.md, step 8, group G6).

The `-S` text is checked in
cpp/tests/test_redundant_load_after_rmw_source.cpp.
"""
from __future__ import annotations

import unittest
from tests.cpp_build import needs_c6502cpp


@needs_c6502cpp
class TestRedundantLoadAfterRmwE2E(unittest.TestCase):

    def test_simulator_returns_correct_value(self) -> None:
        # Verify semantics preserved end-to-end through the sim.
        src = (
            "int main(void) {\n"
            "    int sum = 0;\n"
            "    for (int x = 15; x >= 0; x--) sum += x;\n"
            "    return sum;\n"
            "}\n"
        )
        from sim.harness import build_sim
        sim = build_sim(src, optimize=True)
        result = sim.run(max_cycles=200_000)
        self.assertFalse(result.timed_out)
        self.assertEqual(result.return_int_signed(), 120)


if __name__ == "__main__":
    unittest.main()
