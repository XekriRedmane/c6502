"""Tests for `passes.label_offset_fold.apply_label_offset_fold`: a
label's address plus a constant as one immediate."""

import unittest

import asm_ast
from passes.label_offset_fold import apply_label_offset_fold

_A = asm_ast.Reg(reg=asm_ast.A())
_T = asm_ast.Data(name="t", offset=0)
_T1 = asm_ast.Data(name="t", offset=1)


def _run(instrs):
    prog = asm_ast.Program(top_level=[asm_ast.Function(
        name="f", is_global=False, params=[], instructions=instrs,
    )])
    return apply_label_offset_fold(prog).top_level[0].instructions


def _lo(k=0):
    return asm_ast.ImmLabelLow(name="LOCS", offset=k)


def _hi(k=0):
    return asm_ast.ImmLabelHigh(name="LOCS", offset=k)


class TestLabelOffsetFold(unittest.TestCase):

    def test_both_bytes(self):
        out = _run([
            asm_ast.Mov(src=asm_ast.Imm(value=0x2F), dst=_A),
            asm_ast.ClearCarry(),
            asm_ast.Add(src=_lo(), dst=_A),
            asm_ast.Mov(src=_A, dst=_T),
            asm_ast.Mov(src=asm_ast.Imm(value=1), dst=_A),
            asm_ast.Add(src=_hi(), dst=_A),
            asm_ast.Mov(src=_A, dst=_T1),
            asm_ast.Return(save_a=False),
        ])
        self.assertEqual(out, [
            asm_ast.Mov(src=_lo(0x12F), dst=_A),
            asm_ast.Mov(src=_A, dst=_T),
            asm_ast.Mov(src=_hi(0x12F), dst=_A),
            asm_ast.Mov(src=_A, dst=_T1),
            asm_ast.Return(save_a=False),
        ])

    def test_low_byte_alone(self):
        # `(uint8_t)(uint16_t)&LOCS[1]` on 9-byte records.
        out = _run([
            asm_ast.Mov(src=_lo(), dst=_A),
            asm_ast.ClearCarry(),
            asm_ast.Add(src=asm_ast.Imm(value=9), dst=_A),
            asm_ast.Mov(src=_A, dst=_T),
            asm_ast.Return(save_a=False),
        ])
        self.assertEqual(out, [
            asm_ast.Mov(src=_lo(9), dst=_A),
            asm_ast.Mov(src=_A, dst=_T),
            asm_ast.Return(save_a=False),
        ])

    def test_high_byte_into_x(self):
        # `return T + 7;` returns the high byte in X.
        t0 = asm_ast.Data(name="t", offset=0)
        out = _run([
            asm_ast.Mov(src=_lo(), dst=_A),
            asm_ast.ClearCarry(),
            asm_ast.Add(src=asm_ast.Imm(value=7), dst=_A),
            asm_ast.Mov(src=_A, dst=t0),
            asm_ast.Mov(src=_hi(), dst=_A),
            asm_ast.Add(src=asm_ast.Imm(value=0), dst=_A),
            asm_ast.Mov(src=_A, dst=asm_ast.Reg(reg=asm_ast.X())),
            asm_ast.Mov(src=t0, dst=_A),
            asm_ast.Return(save_a=True),
        ])
        self.assertEqual(out[:4], [
            asm_ast.Mov(src=_lo(7), dst=_A),
            asm_ast.Mov(src=_A, dst=t0),
            asm_ast.Mov(src=_hi(7), dst=_A),
            asm_ast.Mov(src=_A, dst=asm_ast.Reg(reg=asm_ast.X())),
        ])

    def test_low_byte_stays_when_its_carry_is_read(self):
        # The carry out goes into a high byte of another value.
        instrs = [
            asm_ast.Mov(src=_lo(), dst=_A),
            asm_ast.ClearCarry(),
            asm_ast.Add(src=asm_ast.Imm(value=9), dst=_A),
            asm_ast.Mov(src=_A, dst=_T),
            asm_ast.Mov(src=asm_ast.Data(name="u", offset=0), dst=_A),
            asm_ast.Add(src=asm_ast.Imm(value=0), dst=_A),
            asm_ast.Mov(src=_A, dst=_T1),
            asm_ast.Return(save_a=False),
        ]
        self.assertEqual(_run(instrs), instrs)


if __name__ == "__main__":
    unittest.main()
