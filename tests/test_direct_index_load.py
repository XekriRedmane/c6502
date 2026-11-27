"""Tests for the direct-into-X/Y peephole.

`apply_direct_index_load` recognizes `Mov(M, Reg(A));
Mov(Reg(A), Reg(X|Y))` where M is `Data` / `ZP` / `Imm` and `A`
is dead immediately after the second Mov. Rewrites to a single
`Mov(M, Reg(X|Y))` — `LDX` / `LDY` directly from memory or
immediate.

Coverage:
  * Asm shape: the canonical IndexedStore + IndexedLoad lowerings
    (which both stage their index through A) collapse to a
    direct LDX.
  * Disqualifications: A live after the pair (read by next
    instruction), Frame / Stack / Indirect operand (LDX/LDY don't
    support indirect-Y), the second Mov targeting A (not X/Y), no
    matching first Mov.
  * End-to-end correctness via the sim — the program writes to
    the right address and produces the right return value.

The tests that called the Python functions directly are doctest cases of
the C++ port now, each named for the test it came from
(docs/cpp_port.md, step 8, group G6).

The `-S` text is checked in cpp/tests/test_direct_index_load_source.cpp.
"""
from __future__ import annotations

import unittest

from sim.harness import build_sim
from tests.cpp_build import needs_c6502cpp


@needs_c6502cpp
class TestDirectIndexLoadCorrectness(unittest.TestCase):
    """End-to-end correctness."""

    def test_indexed_store_lands_correctly(self) -> None:
        src = (
            "#include <stdint.h>\n"
            "static uint8_t * const buf = (uint8_t * const)0x4100;\n"
            "void put(uint8_t value, uint8_t col) {\n"
            "    buf[col] = value;\n"
            "}\n"
            "int main(void) { put(0x55, 17); return 0; }\n"
        )
        sim = build_sim(src, optimize=True)
        result = sim.run(max_cycles=5_000_000)
        self.assertFalse(result.timed_out)
        self.assertEqual(result.memory[0x4100 + 17], 0x55)


if __name__ == "__main__":
    unittest.main()
