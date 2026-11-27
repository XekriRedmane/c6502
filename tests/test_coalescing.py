"""Tests for asm-level move coalescing.

The pass identifies move-related Pseudo pairs (connected by an
explicit Mov or by a Phi argument) and merges them in the
interference graph when they don't interfere. After coloring,
the merged class shares one ZP slot, and Phi-destruction Movs
between members become self-Movs that `asm_emit`'s self-Mov
peephole drops.

Coverage:
  * Direct unit tests on synthetic functions: simple Phi
    coalescing, Mov coalescing, interfering pair NOT coalesced,
    different-width pair NOT coalesced, chained merges
    (transitive), excluded-name (static) NOT coalesced.
  * End-to-end via the optimizer: a `for (uchar i; i < N; i++)`
    loop where the asm-SSA Phi destruction would otherwise route
    through a temp — coalescing eliminates the temp and the INC
    peephole collapses the increment to a bare `INC` (or
    `INC + BNE + INC` for multi-byte i).

The tests that called the Python functions directly are doctest cases of
the C++ port now, each named for the test it came from
(docs/cpp_port.md, step 8, group G6).

The `-S` text is checked in cpp/tests/test_coalescing.cpp.
"""
from __future__ import annotations

import unittest

from sim.harness import build_sim
from tests.cpp_build import needs_c6502cpp


@needs_c6502cpp
class TestCoalescingEndToEnd(unittest.TestCase):
    """Programs going through the full pipeline."""

    def test_uchar_loop_correct_sum(self) -> None:
        # Same loop as above, but verify the simulator actually
        # computes 0+1+2+3+4 = 10 with the coalesced code.
        src = (
            "#include <stdint.h>\n"
            "int main(void) {\n"
            "    uint8_t total = 0;\n"
            "    for (uint8_t i = 0; i < 5; i++) total = total + i;\n"
            "    return total;\n"
            "}\n"
        )
        sim = build_sim(src, optimize=True)
        result = sim.run(max_cycles=5_000_000)
        self.assertFalse(result.timed_out)
        self.assertEqual(result.return_int() & 0xFFFF, 10)


@needs_c6502cpp
class TestByteMergeSeesWholeClass(unittest.TestCase):
    """fuzz.stmts seed 1921: in `p *= 5` (`(p << 2) + p`) the shift
    temp was put in p's slot, though p is read after the shift. The
    byte-level check looked only at names still in the graph, which
    left out p's byte that the move pairs had already merged."""

    def test_shift_temp_not_merged_with_live_operand(self):
        src = (
            "int g2 = 0;\n"
            "signed char g4;\n"
            "void f(unsigned int p) {\n"
            "    unsigned char w;\n"
            "    if (g2 == 0) {\n"
            "        w = 0;\n"
            "        do {\n"
            "            if ((w | (p *= 5))) { }\n"
            "        } while (++w < 2);\n"
            "    }\n"
            "    g4 = (2 ^ (p -= 255));\n"
            "}\n"
            "int main(void) { f(6); return g4; }\n"
        )
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                got = build_sim(src, optimize=optimize).run()
                self.assertEqual(got.return_int_signed(), -107)


if __name__ == "__main__":
    unittest.main()
