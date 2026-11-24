"""Tests for the multi-byte INC peephole.

The pass detects the per-byte ADC chain `tac_to_asm` emits for an
in-place add-1 (`Mov(M, A); CLC; Add(Imm(1), A); Mov(A, M)` for
the low byte; `Mov(M, A); Add(Imm(0), A); Mov(A, M)` for each
continuation byte) and rewrites it to `INC + BNE done` chains.
The rewrite is gated on the operand being `Data` or `ZP` (the
6502 has no `INC (ind),Y`) and on the per-byte LDA source
matching the STA destination (in-place RMW; if a temp routing
sneaks in we skip).

Coverage:
  * Asm shape: 16-bit static / ZP-resident loop counter increment
    becomes `INC + BNE + INC + label`. 1-byte case becomes a bare
    `INC`.
  * Disqualifications: non-in-place (LDA/STA target differ),
    Frame-resident operand (`(ind),Y` access), `+= 2` (not 1),
    `-= 1` (DEC family — not in this peephole's scope).
  * End-to-end correctness via the sim — INC chain produces the
    same final value as the ADC chain, including across byte
    overflow.

Tests are gated on the optimization pipeline being available
(`pcpp` for the preprocessor) and run both unoptimized and
optimized when the construct supports both.

The tests that called the Python functions directly are doctest cases of
the C++ port now, each named for the test it came from
(docs/cpp_port.md, step 8, group G6).
"""
from __future__ import annotations

import shutil
import unittest

from sim.harness import build_sim


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestIncPeepholeAsmShape(unittest.TestCase):
    """Source-level checks: the emitted asm uses INC + BNE."""

    def _compile_optimized(self, src: str) -> str:
        from compile import _run_stage
        from preprocessor import preprocess
        return _run_stage("codegen", preprocess(src), optimize=True)

    def test_loop_counter_16bit_inc_chain(self) -> None:
        # `for (int i = 0; i < N; i++)` — under -O the
        # loop counter `i` lives in two ZP bytes (regalloc colors
        # both bytes of the renamed SSA pseudos to ZP). The `i++`
        # increment is in-place on each byte and the peephole
        # collapses the ADC chain to INC + BNE + INC.
        src = (
            "int sum10(void) {\n"
            "    int s = 0;\n"
            "    for (int i = 0; i < 10; i++) s += i;\n"
            "    return s;\n"
            "}\n"
            "int main(void) { return sum10(); }\n"
        )
        asm = self._compile_optimized(src)
        # The loop_continue block should have `INC ...; BNE ...; INC ...`
        # for the 16-bit counter increment.
        # Find the .loop@N_continue label and look at what follows.
        self.assertIn(".loop@", asm)
        # Look for the pattern: an INC, then a BNE, then an INC, on
        # ZP slots.
        # Easiest: just check no `ADC   #$01` survives (the ADC #$01
        # only ever appears in this program for the i++ increment).
        self.assertNotIn("ADC   #$01", asm)
        # And the INC + BNE chain should appear. The INC target is
        # a body-local slot symbol (`__local_<fn>__<source>[_<byte>]`),
        # bound to a ZP address via the EQU directive emitted at the
        # top. The branch-through-jump peephole rewrites the BNE to
        # skip directly past the high-byte INC to the loop start,
        # so the original `.inc_done@N` label gets folded away by
        # the downstream dead-label drop; check for the simpler
        # INC + BNE shape instead.
        import re
        self.assertRegex(asm, r"INC\s+__local_\w+__\w+")
        self.assertRegex(
            asm, r"INC\s+__local_\w+__\w+\s*\n\s*BNE\s+",
        )


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestIncPeepholeCorrectness(unittest.TestCase):
    """End-to-end: the optimized program produces the same answer
    as the unoptimized one, including across byte-overflow
    boundaries (the BNE / fall-through into the high-byte INC has
    to fire correctly when the low byte wraps from $FF to $00)."""

    def test_loop_counter_16bit_correct_sum(self) -> None:
        # Sum 0..9 = 45. The loop-counter `i++` is the peephole
        # target.
        src = (
            "int sum10(void) {\n"
            "    int s = 0;\n"
            "    for (int i = 0; i < 10; i++) s += i;\n"
            "    return s;\n"
            "}\n"
            "int main(void) { return sum10(); }\n"
        )
        sim = build_sim(src, optimize=True)
        result = sim.run(max_cycles=5_000_000)
        self.assertFalse(result.timed_out)
        self.assertEqual(result.return_int() & 0xFFFF, 45)

    def test_byte_overflow_propagates_to_high_byte(self) -> None:
        # Loop runs 300 iterations. After 256 the low byte wraps
        # to 0 and the BNE in the INC chain falls through to the
        # high-byte INC — exercising the carry-propagation path.
        # `s` accumulates `i`, so the final value tests both that
        # the high byte got incremented (i must reach 300, an
        # impossible value if the high byte stayed 0) and that
        # the loop terminates correctly (so the `i < 300`
        # comparison sees the right multi-byte value).
        src = (
            "long sum300(void) {\n"
            "    long s = 0;\n"
            "    for (int i = 0; i < 300; i++) s += 1;\n"
            "    return s;\n"
            "}\n"
            "long main(void) { return sum300(); }\n"
        )
        sim = build_sim(src, optimize=True)
        result = sim.run(max_cycles=10_000_000)
        self.assertFalse(result.timed_out)
        # 300 iterations of `s += 1` → s == 300.
        self.assertEqual(result.return_long() & 0xFFFFFFFF, 300)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestIncDecLeavesA(unittest.TestCase):
    """The ADC / SBC form leaves the new value in A and INC / DEC
    don't, so when A is read afterwards the value is loaded. `return
    ++c;` for a static `unsigned char` used to return whatever A held
    before."""

    def _both(self, src, want):
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                got = build_sim(src, optimize=optimize).run().return_int()
                self.assertEqual(got, want)

    def test_return_preincrement_of_static_char(self):
        self._both(
            "unsigned char tick(void) {\n"
            "    static unsigned char c = 10;\n"
            "    return ++c;\n"
            "}\n"
            "int main(void) { tick(); tick(); return tick(); }\n",
            13,
        )

    def test_return_predecrement_of_static_char(self):
        self._both(
            "unsigned char tock(void) {\n"
            "    static unsigned char c = 10;\n"
            "    return --c;\n"
            "}\n"
            "int main(void) { tock(); return tock(); }\n",
            8,
        )

    def test_return_preincrement_of_static_int(self):
        self._both(
            "unsigned int tick(void) {\n"
            "    static unsigned int w = 0x01FF;\n"
            "    return ++w;\n"
            "}\n"
            "int main(void) { tick(); return tick(); }\n",
            0x0201,
        )


if __name__ == "__main__":
    unittest.main()
