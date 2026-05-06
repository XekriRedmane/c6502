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
"""
from __future__ import annotations

import shutil
import unittest

import asm_ast
from passes.redundant_load_after_rmw import (
    apply_redundant_load_after_rmw,
)


_REG_A = asm_ast.Reg(reg=asm_ast.A())
_REG_X = asm_ast.Reg(reg=asm_ast.X())


def _zp(addr: int) -> asm_ast.ZP:
    return asm_ast.ZP(address=addr, offset=0)


def _data(name: str) -> asm_ast.Data:
    return asm_ast.Data(name=name, offset=0)


def _imm(v: int) -> asm_ast.Imm:
    return asm_ast.Imm(value=v)


def _prog(instrs: list[asm_ast.Type_instruction]) -> asm_ast.Program:
    return asm_ast.Program(top_level=[
        asm_ast.Function(
            name="f", is_global=True, params=[], instructions=instrs,
        ),
    ])


def _instrs(prog: asm_ast.Program) -> list[asm_ast.Type_instruction]:
    fn = prog.top_level[0]
    assert isinstance(fn, asm_ast.Function)
    return fn.instructions


class TestRedundantLoadAfterRmw(unittest.TestCase):
    def test_drops_lda_after_dec_with_branch_to_killing_target(self) -> None:
        # Pattern matching the rotated countdown loop tail. Both
        # the fall-through (LDA #$00) and the target (LDA #$00 at
        # .top) start by killing A. So the LDA $C6 between DEC and
        # BPL is purely a flag re-set we can drop.
        instrs = [
            asm_ast.Label(name=".top"),
            asm_ast.Mov(src=_imm(0), dst=_REG_A),  # LDA #$00 — kills A at top
            asm_ast.Mov(src=_REG_A, dst=_zp(0xC0)),
            asm_ast.Dec(dst=_zp(0xC6)),
            asm_ast.Mov(src=_zp(0xC6), dst=_REG_A),  # to drop
            asm_ast.Branch(cond=asm_ast.PL(), target=".top"),
            asm_ast.Mov(src=_imm(1), dst=_REG_A),  # post-branch — kills A
            asm_ast.Return(save_a=False),
        ]
        out = _instrs(apply_redundant_load_after_rmw(_prog(instrs)))
        self.assertEqual(len(out), len(instrs) - 1)
        # The redundant LDA is gone; everything else preserved.
        self.assertEqual(out[3], asm_ast.Dec(dst=_zp(0xC6)))
        self.assertEqual(
            out[4], asm_ast.Branch(cond=asm_ast.PL(), target=".top"),
        )

    def test_drops_lda_after_inc(self) -> None:
        instrs = [
            asm_ast.Inc(dst=_zp(0x80)),
            asm_ast.Mov(src=_zp(0x80), dst=_REG_A),
            asm_ast.Branch(cond=asm_ast.NE(), target=".end"),
            asm_ast.Label(name=".end"),
            asm_ast.Mov(src=_imm(0), dst=_REG_A),
            asm_ast.Return(save_a=False),
        ]
        out = _instrs(apply_redundant_load_after_rmw(_prog(instrs)))
        self.assertEqual(len(out), 5)
        self.assertEqual(out[0], instrs[0])
        self.assertEqual(out[1], instrs[2])  # Branch follows Dec directly

    def test_keeps_lda_when_a_read_on_fall_through(self) -> None:
        # The fall-through (after the BPL) reads A before killing
        # it. So A is live at the LDA — keep the LDA.
        instrs = [
            asm_ast.Label(name=".top"),
            asm_ast.Mov(src=_imm(0), dst=_REG_A),
            asm_ast.Dec(dst=_zp(0xC6)),
            asm_ast.Mov(src=_zp(0xC6), dst=_REG_A),  # NOT droppable
            asm_ast.Branch(cond=asm_ast.PL(), target=".top"),
            # Fall-through: stash A into memory before killing it.
            asm_ast.Mov(src=_REG_A, dst=_zp(0x80)),
            asm_ast.Return(save_a=False),
        ]
        out = _instrs(apply_redundant_load_after_rmw(_prog(instrs)))
        self.assertEqual(out, instrs)

    def test_keeps_lda_when_a_read_at_branch_target(self) -> None:
        # The branch target reads A before killing it. Keep LDA.
        instrs = [
            asm_ast.Label(name=".top"),
            # Top reads A first (e.g., depends on an A-resident
            # value passed across the back-edge).
            asm_ast.Mov(src=_REG_A, dst=_zp(0x80)),
            asm_ast.Mov(src=_imm(0), dst=_REG_A),
            asm_ast.Dec(dst=_zp(0xC6)),
            asm_ast.Mov(src=_zp(0xC6), dst=_REG_A),  # NOT droppable
            asm_ast.Branch(cond=asm_ast.PL(), target=".top"),
            asm_ast.Mov(src=_imm(0), dst=_REG_A),
            asm_ast.Return(save_a=False),
        ]
        out = _instrs(apply_redundant_load_after_rmw(_prog(instrs)))
        self.assertEqual(out, instrs)

    def test_does_not_match_when_addresses_differ(self) -> None:
        # DEC $C6; LDA $C7 — different cells, not redundant.
        instrs = [
            asm_ast.Dec(dst=_zp(0xC6)),
            asm_ast.Mov(src=_zp(0xC7), dst=_REG_A),
            asm_ast.Mov(src=_imm(0), dst=_REG_A),
            asm_ast.Return(save_a=False),
        ]
        out = _instrs(apply_redundant_load_after_rmw(_prog(instrs)))
        self.assertEqual(out, instrs)

    def test_does_not_match_when_lda_dst_not_a(self) -> None:
        # DEC m; LDX m — X load doesn't fit our pattern.
        instrs = [
            asm_ast.Dec(dst=_zp(0xC6)),
            asm_ast.Mov(src=_zp(0xC6), dst=_REG_X),
            asm_ast.Return(save_a=False),
        ]
        out = _instrs(apply_redundant_load_after_rmw(_prog(instrs)))
        self.assertEqual(out, instrs)

    def test_does_not_match_when_no_rmw(self) -> None:
        # Bare LDA without preceding rmw — not the pattern.
        instrs = [
            asm_ast.Mov(src=_imm(7), dst=_zp(0xC6)),
            asm_ast.Mov(src=_zp(0xC6), dst=_REG_A),
            asm_ast.Return(save_a=False),
        ]
        out = _instrs(apply_redundant_load_after_rmw(_prog(instrs)))
        self.assertEqual(out, instrs)

    def test_handles_data_operand(self) -> None:
        # Same pattern with `Data` instead of `ZP` (static-storage rmw).
        instrs = [
            asm_ast.Inc(dst=_data("counter")),
            asm_ast.Mov(src=_data("counter"), dst=_REG_A),
            asm_ast.Branch(cond=asm_ast.NE(), target=".end"),
            asm_ast.Label(name=".end"),
            asm_ast.Mov(src=_imm(0), dst=_REG_A),
            asm_ast.Return(save_a=False),
        ]
        out = _instrs(apply_redundant_load_after_rmw(_prog(instrs)))
        self.assertEqual(len(out), 5)
        self.assertEqual(out[0], instrs[0])
        self.assertEqual(out[1], instrs[2])

    def test_drops_txa_after_dex(self) -> None:
        # `DEX; TXA; BPL .top` — register rmw form. TXA's flag set
        # is redundant after DEX. Both branch paths kill A first.
        instrs = [
            asm_ast.Label(name=".top"),
            asm_ast.Mov(src=_imm(0), dst=_REG_A),
            asm_ast.Dec(dst=_REG_X),
            asm_ast.Mov(src=_REG_X, dst=_REG_A),  # TXA — to drop
            asm_ast.Branch(cond=asm_ast.PL(), target=".top"),
            asm_ast.Mov(src=_imm(0), dst=_REG_A),
            asm_ast.Return(save_a=False),
        ]
        out = _instrs(apply_redundant_load_after_rmw(_prog(instrs)))
        # TXA should be gone; everything else preserved.
        self.assertEqual(len(out), len(instrs) - 1)
        self.assertEqual(out[2], asm_ast.Dec(dst=_REG_X))
        self.assertEqual(
            out[3], asm_ast.Branch(cond=asm_ast.PL(), target=".top"),
        )

    def test_drops_tya_after_dey(self) -> None:
        instrs = [
            asm_ast.Label(name=".top"),
            asm_ast.Mov(src=_imm(0), dst=_REG_A),
            asm_ast.Dec(dst=asm_ast.Reg(reg=asm_ast.Y())),
            asm_ast.Mov(
                src=asm_ast.Reg(reg=asm_ast.Y()), dst=_REG_A,
            ),
            asm_ast.Branch(cond=asm_ast.PL(), target=".top"),
            asm_ast.Mov(src=_imm(0), dst=_REG_A),
            asm_ast.Return(save_a=False),
        ]
        out = _instrs(apply_redundant_load_after_rmw(_prog(instrs)))
        self.assertEqual(len(out), len(instrs) - 1)

    def test_drops_txa_after_inx(self) -> None:
        # INX/INY also count — same flag semantics as DEC, just
        # opposite direction.
        instrs = [
            asm_ast.Label(name=".top"),
            asm_ast.Mov(src=_imm(0), dst=_REG_A),
            asm_ast.Inc(dst=_REG_X),
            asm_ast.Mov(src=_REG_X, dst=_REG_A),
            asm_ast.Branch(cond=asm_ast.NE(), target=".top"),
            asm_ast.Mov(src=_imm(0), dst=_REG_A),
            asm_ast.Return(save_a=False),
        ]
        out = _instrs(apply_redundant_load_after_rmw(_prog(instrs)))
        self.assertEqual(len(out), len(instrs) - 1)

    def test_self_loop_no_infinite_recursion(self) -> None:
        # Tight self-loop where the back-edge target is the same
        # block. The cycle visited-set prevents infinite recursion.
        instrs = [
            asm_ast.Label(name=".top"),
            asm_ast.Mov(src=_imm(0), dst=_REG_A),
            asm_ast.Dec(dst=_zp(0xC6)),
            asm_ast.Mov(src=_zp(0xC6), dst=_REG_A),
            asm_ast.Branch(cond=asm_ast.PL(), target=".top"),
            asm_ast.Return(save_a=False),
        ]
        out = _instrs(apply_redundant_load_after_rmw(_prog(instrs)))
        # The LDA inside the loop should drop since both paths
        # (back-edge to .top, fall-through to Return) kill A first.
        self.assertEqual(len(out), 5)
        self.assertNotIn(
            asm_ast.Mov(src=_zp(0xC6), dst=_REG_A), out,
        )


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestRedundantLoadAfterRmwE2E(unittest.TestCase):
    def _compile(self, src: str) -> str:
        from compile import _run_stage
        from preprocessor import preprocess
        return _run_stage("codegen", preprocess(src), optimize=True)

    def test_signed_countdown_collapses_to_dec_bpl(self) -> None:
        # `int8_t` (1 byte signed): the post-decrement is a 1-byte
        # SBC chain that `dec_peephole` collapses to DEC, and the
        # cond becomes a 1-byte test against zero. This pass then
        # drops the redundant LDA between DEC and BPL.
        src = (
            "#include <stdint.h>\n"
            "int sum;\n"
            "int main(void) {\n"
            "    for (int8_t x = 15; x >= 0; x--) sum += x;\n"
            "    return sum;\n"
            "}\n"
        )
        asm = self._compile(src)
        # The loop tail must contain "DEC <op>" followed (possibly
        # via a label) by "BPL", with no intervening "LDA <op>"
        # for the same op.
        lines = asm.split("\n")
        dec_line = None
        for i, l in enumerate(lines):
            if l.strip().startswith("DEC   "):
                dec_line = i
                break
        self.assertIsNotNone(dec_line, msg=f"no DEC in:\n{asm}")
        # The instruction after DEC (skipping any blank lines)
        # should be BPL — no LDA between.
        nxt = dec_line + 1
        while nxt < len(lines) and not lines[nxt].strip():
            nxt += 1
        self.assertTrue(
            lines[nxt].strip().startswith("BPL "),
            msg=f"expected BPL after DEC, got {lines[nxt]!r}\n{asm}",
        )

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
