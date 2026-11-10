"""Tests for the flag-liveness walks in `passes.asm_liveness`."""

from __future__ import annotations

import unittest

import asm_ast
from passes.asm_liveness import all_flags_dead_at, flags_dead_at, sets_flags


_A = asm_ast.Reg(reg=asm_ast.A())


def _signed_compare_tail():
    """The end of tac_to_asm's signed ordering compare:
    `SBC hi; BVC .novf; EOR #$80; .novf: BMI .true; ...`."""
    return [
        asm_ast.Sub(src=asm_ast.Imm(value=0), dst=_A),
        asm_ast.Branch(cond=asm_ast.VC(), target=".novf"),
        asm_ast.Xor(src1=_A, src2=asm_ast.Imm(value=0x80), dst=_A),
        asm_ast.Label(name=".novf"),
        asm_ast.Branch(cond=asm_ast.MI(), target=".true"),
        asm_ast.Mov(src=asm_ast.Imm(value=0), dst=_A, is_volatile=False),
        asm_ast.Return(save_a=True),
        asm_ast.Label(name=".true"),
        asm_ast.Mov(src=asm_ast.Imm(value=1), dst=_A, is_volatile=False),
        asm_ast.Return(save_a=True),
    ]


class TestFlagsDeadAt(unittest.TestCase):

    def test_flags_live_through_label(self):
        # The EOR's N flag reaches the BMI after `.novf:`.
        instrs = _signed_compare_tail()
        self.assertFalse(flags_dead_at(instrs, 3))
        self.assertFalse(all_flags_dead_at(instrs, 3))

    def test_flags_live_through_jump(self):
        instrs = [
            asm_ast.Jump(target=".l"),
            asm_ast.Label(name=".l"),
            asm_ast.Branch(cond=asm_ast.EQ(), target=".l"),
        ]
        self.assertFalse(flags_dead_at(instrs, 0))

    def test_flag_setter_after_label_kills(self):
        instrs = [
            asm_ast.Label(name=".l"),
            asm_ast.Mov(src=asm_ast.Imm(value=0), dst=_A, is_volatile=False),
            asm_ast.Branch(cond=asm_ast.EQ(), target=".l"),
        ]
        self.assertTrue(flags_dead_at(instrs, 0))

    def test_return_ends_liveness(self):
        instrs = [asm_ast.Label(name=".l"), asm_ast.Return(save_a=True)]
        self.assertTrue(flags_dead_at(instrs, 0))
        self.assertTrue(all_flags_dead_at(instrs, 0))


class TestSetsFlags(unittest.TestCase):

    _A = asm_ast.Reg(reg=asm_ast.A())

    def test_store_to_data_leaves_flags(self):
        mov = asm_ast.Mov(src=self._A, dst=asm_ast.Data(name="g", offset=0))
        self.assertFalse(sets_flags(mov))

    def test_store_with_y_setup_sets_flags(self):
        # Each of these emits `LDY #off` ahead of the `STA`, and LDY
        # sets N/Z from the offset.
        for dst in (asm_ast.Frame(offset=3), asm_ast.Stack(offset=1),
                    asm_ast.Indirect(offset=0),
                    asm_ast.IndirectZp(address=0x80, offset=2)):
            with self.subTest(dst=type(dst).__name__):
                self.assertTrue(sets_flags(
                    asm_ast.Mov(src=self._A, dst=dst)))


if __name__ == "__main__":
    unittest.main()
