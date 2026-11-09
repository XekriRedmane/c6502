"""Tests for `passes.dec_peephole` (single-byte `x - 1` to DEC)."""

from __future__ import annotations

import unittest

import asm_ast
from passes.dec_peephole import apply_dec_peephole


_A = asm_ast.Reg(reg=asm_ast.A())


def _run(instrs):
    prog = asm_ast.Program(top_level=[asm_ast.Function(
        name="f", is_global=True, params=[], instructions=instrs,
    )])
    return apply_dec_peephole(prog).top_level[0].instructions


def _sub1(m):
    return [
        asm_ast.Mov(src=m, dst=_A, is_volatile=False),
        asm_ast.SetCarry(),
        asm_ast.Sub(src=asm_ast.Imm(value=1), dst=_A),
        asm_ast.Mov(src=_A, dst=m, is_volatile=False),
    ]


class TestDecPeephole(unittest.TestCase):

    def test_single_byte_folds(self):
        m = asm_ast.ZP(address=0x80, offset=0)
        out = _run(_sub1(m) + [asm_ast.Return(save_a=False)])
        self.assertEqual(out, [asm_ast.Dec(dst=m), asm_ast.Return(save_a=False)])

    def test_high_byte_sbc_into_register_blocks_fold(self):
        # 16-bit `x - 1` whose high byte isn't stored back in place:
        # `LDA hi; SBC #0; TAX` reads the low byte's borrow, which DEC
        # doesn't produce.
        lo = asm_ast.ZP(address=0x80, offset=0)
        hi = asm_ast.ZP(address=0x81, offset=0)
        instrs = _sub1(lo) + [
            asm_ast.Mov(src=hi, dst=_A, is_volatile=False),
            asm_ast.Sub(src=asm_ast.Imm(value=0), dst=_A),
            asm_ast.Mov(src=_A, dst=asm_ast.Reg(reg=asm_ast.X()), is_volatile=False),
            asm_ast.Return(save_a=False),
        ]
        self.assertEqual(_run(instrs), instrs)

    def test_carry_redefined_before_reader_allows_fold(self):
        lo = asm_ast.ZP(address=0x80, offset=0)
        other = asm_ast.ZP(address=0x82, offset=0)
        tail = [
            asm_ast.Mov(src=other, dst=_A, is_volatile=False),
            asm_ast.ClearCarry(),
            asm_ast.Add(src=asm_ast.Imm(value=1), dst=_A),
            asm_ast.Return(save_a=True),
        ]
        out = _run(_sub1(lo) + tail)
        self.assertEqual(out, [asm_ast.Dec(dst=lo)] + tail)


if __name__ == "__main__":
    unittest.main()
