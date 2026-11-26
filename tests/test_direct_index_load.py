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
"""
from __future__ import annotations

import shutil
import unittest

from sim.harness import build_sim


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestDirectIndexLoadAsmShape(unittest.TestCase):
    """Source-level checks: real C programs lowered through the
    full pipeline produce the expected `LDX` / `LDY` form."""

    def _compile(self, src: str) -> str:
        from compile import _run_source
        return _run_source("codegen", src, optimize=True)

    def test_indexed_load_uses_direct_ldx(self) -> None:
        # IndexedLoad's lowering stages the index via A→X. Under
        # -O, the loop counter `i` ends up either pinned to
        # the X register directly (HwReg coloring) or living in ZP
        # with a peephole-emitted direct `LDX $XX`. Either way the
        # `arr[i]` access uses absolute,X addressing — `LDA arr,X`
        # — without an `LDA <i>; TAX` pair.
        src = (
            "#include <stdint.h>\n"
            "static const uint8_t arr[10] ="
            " {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};\n"
            "int main(void) {\n"
            "    int s = 0;\n"
            "    for (uint8_t i = 0; i < 10; i++) s += arr[i];\n"
            "    return s;\n"
            "}\n"
        )
        asm = self._compile(src)
        # Direct absolute,X read, with the loop counter living in X
        # (incremented in place via INX) — no `LDA <i>; TAX` setup
        # pair feeding the indexed load. The read is the `ADC arr,X`
        # of `s += arr[i]` (cmp_through_tmp_fold).
        self.assertIn("ADC   arr,X", asm)
        self.assertIn("INX", asm)
        # The only register transfer into X is the return-value
        # staging (`TYA; TAX` for the 2-byte int return's high byte),
        # NOT an index setup. Assert no `LDA <slot>; TAX` index pair.
        self.assertNotIn("TAX\n   LDA   arr,X", asm)
        self.assertNotRegex(asm, r"LDA   __local[^\n]*\n   TAX")


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
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
