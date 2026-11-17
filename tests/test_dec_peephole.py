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


    def _two_byte(self, after):
        lo = asm_ast.Data(name="sp", offset=0)
        hi = asm_ast.Data(name="sp", offset=1)
        return lo, hi, _sub1(lo) + [
            asm_ast.Branch(cond=asm_ast.CS(), target=".carry_inc@0"),
            asm_ast.Dec(dst=hi),
            asm_ast.Label(name=".carry_inc@0"),
            *after,
        ]

    def test_two_byte_decrement_tests_the_low_byte_first(self):
        # `sp--` on a 2-byte pointer, as carry_inc leaves it.
        load = asm_ast.Mov(src=asm_ast.Data(name="v", offset=0), dst=_A,
                           is_volatile=False)
        lo, hi, instrs = self._two_byte([load, asm_ast.Return()])
        self.assertEqual(_run(instrs), [
            asm_ast.Mov(src=lo, dst=_A, is_volatile=False),
            asm_ast.Branch(cond=asm_ast.NE(), target=".carry_inc@0"),
            asm_ast.Dec(dst=hi),
            asm_ast.Label(name=".carry_inc@0"),
            asm_ast.Dec(dst=lo),
            load,
            asm_ast.Return(),
        ])

    def test_two_byte_decrement_kept_when_a_is_read(self):
        # A holds the new low byte after the SBC form, and the old one
        # after the DEC form.
        _, _, instrs = self._two_byte([asm_ast.Return(save_a=True)])
        self.assertEqual(_run(instrs), instrs)

    def test_two_byte_decrement_kept_after_a_store_of_the_low_byte(self):
        # `v = f() - 1`: A holds the low byte from the call, whose
        # flags aren't A's, so the `BNE` form would need `ORA #$00`.
        lo = asm_ast.Data(name="sp", offset=0)
        store = asm_ast.Mov(src=_A, dst=lo, is_volatile=False)
        _, _, instrs = self._two_byte([asm_ast.Return()])
        instrs = [asm_ast.Call(name="f"), store] + instrs
        self.assertEqual(_run(instrs), instrs)

    def test_two_byte_decrement_after_a_store_of_a_flag_setting_value(self):
        # `v = (x & 0xC0) - 1`: the AND set N and Z from A.
        lo = asm_ast.Data(name="sp", offset=0)
        mask = asm_ast.And(src=asm_ast.Imm(value=0xC0), dst=_A)
        store = asm_ast.Mov(src=_A, dst=lo, is_volatile=False)
        _, _, instrs = self._two_byte([asm_ast.Return()])
        out = _run([mask, store] + instrs)
        self.assertIn(asm_ast.Branch(cond=asm_ast.NE(), target=".carry_inc@0"),
                      out)

if __name__ == "__main__":
    unittest.main()
