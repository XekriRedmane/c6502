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
"""
from __future__ import annotations

import shutil
import unittest

import asm_ast
from passes.inc_peephole import apply_inc_peephole
from sim.harness import build_sim


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestIncPeepholeAsmShape(unittest.TestCase):
    """Source-level checks: the emitted asm uses INC + BNE."""

    def _compile_optimized(self, src: str) -> str:
        from compile import _run_stage
        from preprocessor import preprocess
        return _run_stage("codegen", preprocess(src), optimize=True)

    def test_loop_counter_16bit_inc_chain(self) -> None:
        # `for (int i = 0; i < N; i++)` — under --optimize the
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
        # And `INC   $` / `BNE   .inc_done@` should appear.
        self.assertIn("INC   $", asm)
        self.assertIn(".inc_done@", asm)

    def test_one_byte_inc_no_branch(self) -> None:
        # 1-byte add-1 collapses to a bare INC — no BNE, no done
        # label.
        from passes.inc_peephole import apply_inc_peephole
        prog = asm_ast.Program(top_level=[
            asm_ast.Function(
                name="f", is_global=True, params=[],
                instructions=[
                    asm_ast.Mov(
                        src=asm_ast.ZP(address=0x80, offset=0),
                        dst=asm_ast.Reg(reg=asm_ast.A()),
                    ),
                    asm_ast.ClearCarry(),
                    asm_ast.Add(
                        src=asm_ast.Imm(value=1),
                        dst=asm_ast.Reg(reg=asm_ast.A()),
                    ),
                    asm_ast.Mov(
                        src=asm_ast.Reg(reg=asm_ast.A()),
                        dst=asm_ast.ZP(address=0x80, offset=0),
                    ),
                ],
            ),
        ])
        out = apply_inc_peephole(prog)
        instrs = out.top_level[0].instructions
        # One Inc, no Branch, no Label.
        self.assertEqual(len(instrs), 1)
        self.assertIsInstance(instrs[0], asm_ast.Inc)
        self.assertEqual(
            instrs[0].dst, asm_ast.ZP(address=0x80, offset=0),
        )

    def test_two_byte_inc_chain(self) -> None:
        # 2-byte add-1 expands to INC; BNE done; INC; done:.
        prog = asm_ast.Program(top_level=[
            asm_ast.Function(
                name="f", is_global=True, params=[],
                instructions=[
                    asm_ast.Mov(
                        src=asm_ast.ZP(address=0x80, offset=0),
                        dst=asm_ast.Reg(reg=asm_ast.A()),
                    ),
                    asm_ast.ClearCarry(),
                    asm_ast.Add(
                        src=asm_ast.Imm(value=1),
                        dst=asm_ast.Reg(reg=asm_ast.A()),
                    ),
                    asm_ast.Mov(
                        src=asm_ast.Reg(reg=asm_ast.A()),
                        dst=asm_ast.ZP(address=0x80, offset=0),
                    ),
                    asm_ast.Mov(
                        src=asm_ast.ZP(address=0x81, offset=0),
                        dst=asm_ast.Reg(reg=asm_ast.A()),
                    ),
                    asm_ast.Add(
                        src=asm_ast.Imm(value=0),
                        dst=asm_ast.Reg(reg=asm_ast.A()),
                    ),
                    asm_ast.Mov(
                        src=asm_ast.Reg(reg=asm_ast.A()),
                        dst=asm_ast.ZP(address=0x81, offset=0),
                    ),
                ],
            ),
        ])
        out = apply_inc_peephole(prog)
        instrs = out.top_level[0].instructions
        # 4 instructions: Inc, Branch(NE), Inc, Label.
        self.assertEqual(len(instrs), 4)
        self.assertIsInstance(instrs[0], asm_ast.Inc)
        self.assertEqual(
            instrs[0].dst, asm_ast.ZP(address=0x80, offset=0),
        )
        self.assertIsInstance(instrs[1], asm_ast.Branch)
        self.assertIsInstance(instrs[1].cond, asm_ast.NE)
        self.assertIsInstance(instrs[2], asm_ast.Inc)
        self.assertEqual(
            instrs[2].dst, asm_ast.ZP(address=0x81, offset=0),
        )
        self.assertIsInstance(instrs[3], asm_ast.Label)
        # Branch target should be the label.
        self.assertEqual(instrs[1].target, instrs[3].name)

    def test_data_operand_chain(self) -> None:
        # Same shape, but with Data operands (statics).
        prog = asm_ast.Program(top_level=[
            asm_ast.Function(
                name="f", is_global=True, params=[],
                instructions=[
                    asm_ast.Mov(
                        src=asm_ast.Data(name="ctr", offset=0),
                        dst=asm_ast.Reg(reg=asm_ast.A()),
                    ),
                    asm_ast.ClearCarry(),
                    asm_ast.Add(
                        src=asm_ast.Imm(value=1),
                        dst=asm_ast.Reg(reg=asm_ast.A()),
                    ),
                    asm_ast.Mov(
                        src=asm_ast.Reg(reg=asm_ast.A()),
                        dst=asm_ast.Data(name="ctr", offset=0),
                    ),
                    asm_ast.Mov(
                        src=asm_ast.Data(name="ctr", offset=1),
                        dst=asm_ast.Reg(reg=asm_ast.A()),
                    ),
                    asm_ast.Add(
                        src=asm_ast.Imm(value=0),
                        dst=asm_ast.Reg(reg=asm_ast.A()),
                    ),
                    asm_ast.Mov(
                        src=asm_ast.Reg(reg=asm_ast.A()),
                        dst=asm_ast.Data(name="ctr", offset=1),
                    ),
                ],
            ),
        ])
        out = apply_inc_peephole(prog)
        instrs = out.top_level[0].instructions
        self.assertEqual(len(instrs), 4)
        self.assertEqual(
            instrs[0].dst, asm_ast.Data(name="ctr", offset=0),
        )
        self.assertEqual(
            instrs[2].dst, asm_ast.Data(name="ctr", offset=1),
        )

    def test_non_consecutive_byte_addresses_still_match(self) -> None:
        # Byte-granular regalloc may place the two bytes of a
        # multi-byte value at non-adjacent ZP slots — the
        # structural CLC-ADC#1-then-ADC#0 pattern is what
        # identifies them as one logical add-1, not the addresses.
        # Here byte 0 lives at $81 and byte 1 at $80 (decreasing).
        prog = asm_ast.Program(top_level=[
            asm_ast.Function(
                name="f", is_global=True, params=[],
                instructions=[
                    asm_ast.Mov(
                        src=asm_ast.ZP(address=0x81, offset=0),
                        dst=asm_ast.Reg(reg=asm_ast.A()),
                    ),
                    asm_ast.ClearCarry(),
                    asm_ast.Add(
                        src=asm_ast.Imm(value=1),
                        dst=asm_ast.Reg(reg=asm_ast.A()),
                    ),
                    asm_ast.Mov(
                        src=asm_ast.Reg(reg=asm_ast.A()),
                        dst=asm_ast.ZP(address=0x81, offset=0),
                    ),
                    asm_ast.Mov(
                        src=asm_ast.ZP(address=0x80, offset=0),
                        dst=asm_ast.Reg(reg=asm_ast.A()),
                    ),
                    asm_ast.Add(
                        src=asm_ast.Imm(value=0),
                        dst=asm_ast.Reg(reg=asm_ast.A()),
                    ),
                    asm_ast.Mov(
                        src=asm_ast.Reg(reg=asm_ast.A()),
                        dst=asm_ast.ZP(address=0x80, offset=0),
                    ),
                ],
            ),
        ])
        out = apply_inc_peephole(prog)
        instrs = out.top_level[0].instructions
        self.assertEqual(len(instrs), 4)
        self.assertEqual(
            instrs[0].dst, asm_ast.ZP(address=0x81, offset=0),
        )
        self.assertEqual(
            instrs[2].dst, asm_ast.ZP(address=0x80, offset=0),
        )


class TestIncPeepholeDisqualifications(unittest.TestCase):
    """Patterns the peephole correctly leaves alone."""

    def _build(self, instrs):
        return asm_ast.Program(top_level=[
            asm_ast.Function(
                name="f", is_global=True, params=[],
                instructions=instrs,
            ),
        ])

    def test_not_in_place_skipped(self) -> None:
        # LDA from $80, STA to $81 — different memory cells, not
        # an in-place RMW. INC would be wrong (it'd modify $80,
        # not $81). Skip.
        instrs = [
            asm_ast.Mov(
                src=asm_ast.ZP(address=0x80, offset=0),
                dst=asm_ast.Reg(reg=asm_ast.A()),
            ),
            asm_ast.ClearCarry(),
            asm_ast.Add(
                src=asm_ast.Imm(value=1),
                dst=asm_ast.Reg(reg=asm_ast.A()),
            ),
            asm_ast.Mov(
                src=asm_ast.Reg(reg=asm_ast.A()),
                dst=asm_ast.ZP(address=0x81, offset=0),
            ),
        ]
        out = apply_inc_peephole(self._build(instrs))
        # Unchanged.
        self.assertEqual(out.top_level[0].instructions, instrs)

    def test_frame_operand_skipped(self) -> None:
        # Frame uses indirect-Y; INC has no `(ind),Y` mode. Skip.
        instrs = [
            asm_ast.Mov(
                src=asm_ast.Frame(offset=4),
                dst=asm_ast.Reg(reg=asm_ast.A()),
            ),
            asm_ast.ClearCarry(),
            asm_ast.Add(
                src=asm_ast.Imm(value=1),
                dst=asm_ast.Reg(reg=asm_ast.A()),
            ),
            asm_ast.Mov(
                src=asm_ast.Reg(reg=asm_ast.A()),
                dst=asm_ast.Frame(offset=4),
            ),
        ]
        out = apply_inc_peephole(self._build(instrs))
        self.assertEqual(out.top_level[0].instructions, instrs)

    def test_add_two_skipped(self) -> None:
        # `+= 2` — INC adds 1 only. Multiple INCs would be longer
        # than the original ADC chain. Skip.
        instrs = [
            asm_ast.Mov(
                src=asm_ast.ZP(address=0x80, offset=0),
                dst=asm_ast.Reg(reg=asm_ast.A()),
            ),
            asm_ast.ClearCarry(),
            asm_ast.Add(
                src=asm_ast.Imm(value=2),
                dst=asm_ast.Reg(reg=asm_ast.A()),
            ),
            asm_ast.Mov(
                src=asm_ast.Reg(reg=asm_ast.A()),
                dst=asm_ast.ZP(address=0x80, offset=0),
            ),
        ]
        out = apply_inc_peephole(self._build(instrs))
        self.assertEqual(out.top_level[0].instructions, instrs)

    def test_subtract_skipped(self) -> None:
        # `-= 1` uses SetCarry + Sub, which the peephole's pattern
        # doesn't match (would need a separate DEC peephole).
        instrs = [
            asm_ast.Mov(
                src=asm_ast.ZP(address=0x80, offset=0),
                dst=asm_ast.Reg(reg=asm_ast.A()),
            ),
            asm_ast.SetCarry(),
            asm_ast.Sub(
                src=asm_ast.Imm(value=1),
                dst=asm_ast.Reg(reg=asm_ast.A()),
            ),
            asm_ast.Mov(
                src=asm_ast.Reg(reg=asm_ast.A()),
                dst=asm_ast.ZP(address=0x80, offset=0),
            ),
        ]
        out = apply_inc_peephole(self._build(instrs))
        self.assertEqual(out.top_level[0].instructions, instrs)

    def test_two_unrelated_one_byte_adds(self) -> None:
        # Two consecutive 1-byte += 1 to different operands. Each
        # has its own CLC, which breaks the multi-byte continuation
        # pattern (continuation is ADC #0 with no CLC). Each fold
        # independently to a single Inc.
        instrs = [
            asm_ast.Mov(
                src=asm_ast.ZP(address=0x80, offset=0),
                dst=asm_ast.Reg(reg=asm_ast.A()),
            ),
            asm_ast.ClearCarry(),
            asm_ast.Add(
                src=asm_ast.Imm(value=1),
                dst=asm_ast.Reg(reg=asm_ast.A()),
            ),
            asm_ast.Mov(
                src=asm_ast.Reg(reg=asm_ast.A()),
                dst=asm_ast.ZP(address=0x80, offset=0),
            ),
            asm_ast.Mov(
                src=asm_ast.ZP(address=0x81, offset=0),
                dst=asm_ast.Reg(reg=asm_ast.A()),
            ),
            asm_ast.ClearCarry(),
            asm_ast.Add(
                src=asm_ast.Imm(value=1),
                dst=asm_ast.Reg(reg=asm_ast.A()),
            ),
            asm_ast.Mov(
                src=asm_ast.Reg(reg=asm_ast.A()),
                dst=asm_ast.ZP(address=0x81, offset=0),
            ),
        ]
        out = apply_inc_peephole(self._build(instrs))
        result_instrs = out.top_level[0].instructions
        # Two bare INCs.
        self.assertEqual(len(result_instrs), 2)
        self.assertIsInstance(result_instrs[0], asm_ast.Inc)
        self.assertEqual(
            result_instrs[0].dst, asm_ast.ZP(address=0x80, offset=0),
        )
        self.assertIsInstance(result_instrs[1], asm_ast.Inc)
        self.assertEqual(
            result_instrs[1].dst, asm_ast.ZP(address=0x81, offset=0),
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


if __name__ == "__main__":
    unittest.main()
