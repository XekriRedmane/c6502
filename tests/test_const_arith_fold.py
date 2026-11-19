"""Tests for the const_arith_fold peephole."""

import unittest

import asm_ast
from passes.const_arith_fold import apply_const_arith_fold


_A = asm_ast.Reg(reg=asm_ast.A())


def _instrs(prog):
    return prog.top_level[0].instructions


def _wrap(instrs):
    return asm_ast.Program(top_level=[asm_ast.Function(
        name="f", is_global=False, params=[], instructions=instrs,
    )])


class TestConstArithFold(unittest.TestCase):

    def test_lda_imm_and_imm_folds_to_lda_constant(self):
        prog = _wrap([
            asm_ast.Mov(src=asm_ast.Imm(value=0x80), dst=_A),
            asm_ast.And(src=asm_ast.Imm(value=0x0F), dst=_A),
        ])
        out = _instrs(apply_const_arith_fold(prog))
        self.assertEqual(out, [asm_ast.Mov(src=asm_ast.Imm(value=0), dst=_A)])

    def test_lda_imm_or_imm_folds(self):
        prog = _wrap([
            asm_ast.Mov(src=asm_ast.Imm(value=0x80), dst=_A),
            asm_ast.Or(src=asm_ast.Imm(value=0x0F), dst=_A),
        ])
        out = _instrs(apply_const_arith_fold(prog))
        self.assertEqual(out, [asm_ast.Mov(src=asm_ast.Imm(value=0x8F), dst=_A)])

    def test_drops_identity_or_zero_after_lda(self):
        # LDA M; ORA #$0 → LDA M (identity).
        prog = _wrap([
            asm_ast.Mov(src=asm_ast.Data(name="M", offset=0), dst=_A),
            asm_ast.Or(src=asm_ast.Imm(value=0), dst=_A),
        ])
        out = _instrs(apply_const_arith_fold(prog))
        self.assertEqual(out, [asm_ast.Mov(src=asm_ast.Data(name="M", offset=0), dst=_A)])

    def test_drops_identity_and_ff_after_lda(self):
        prog = _wrap([
            asm_ast.Mov(src=asm_ast.Data(name="M", offset=0), dst=_A),
            asm_ast.And(src=asm_ast.Imm(value=0xFF), dst=_A),
        ])
        out = _instrs(apply_const_arith_fold(prog))
        self.assertEqual(out, [asm_ast.Mov(src=asm_ast.Data(name="M", offset=0), dst=_A)])

    def test_does_not_drop_non_identity_after_lda(self):
        # LDA M; ORA #$01 isn't identity — keep both.
        prog = _wrap([
            asm_ast.Mov(src=asm_ast.Data(name="M", offset=0), dst=_A),
            asm_ast.Or(src=asm_ast.Imm(value=1), dst=_A),
        ])
        out = _instrs(apply_const_arith_fold(prog))
        self.assertEqual(len(out), 2)

    def test_lda_zero_then_ora_M_absorbs(self):
        # LDA #$00; ORA M → LDA M (0 | M = M with same flag effects).
        prog = _wrap([
            asm_ast.Mov(src=asm_ast.Imm(value=0), dst=_A),
            asm_ast.Or(src=asm_ast.Data(name="M", offset=0), dst=_A),
        ])
        out = _instrs(apply_const_arith_fold(prog))
        self.assertEqual(out, [
            asm_ast.Mov(src=asm_ast.Data(name="M", offset=0), dst=_A),
        ])

    def test_lda_ff_then_and_M_absorbs(self):
        prog = _wrap([
            asm_ast.Mov(src=asm_ast.Imm(value=0xFF), dst=_A),
            asm_ast.And(src=asm_ast.Data(name="M", offset=0), dst=_A),
        ])
        out = _instrs(apply_const_arith_fold(prog))
        self.assertEqual(len(out), 1)

    def test_does_not_fire_when_prev_doesnt_write_a(self):
        # A Compare doesn't write A — the identity drop shouldn't fire.
        prog = _wrap([
            asm_ast.Compare(
                left=_A, right=asm_ast.Imm(value=5),
            ),
            asm_ast.Or(src=asm_ast.Imm(value=0), dst=_A),
        ])
        out = _instrs(apply_const_arith_fold(prog))
        self.assertEqual(len(out), 2)

    def test_and_zero_is_a_load_of_zero(self):
        # `(w & 0xFF00) | b` keeps nothing of `w`'s low byte.
        m = asm_ast.Data(name="m", offset=0)
        prog = _wrap([
            asm_ast.Mov(src=m, dst=_A),
            asm_ast.And(src=asm_ast.Imm(value=0), dst=_A),
            asm_ast.Mov(src=_A, dst=m),
            asm_ast.Return(),
        ])
        out = _instrs(apply_const_arith_fold(prog))
        self.assertEqual(out[1], asm_ast.Mov(src=asm_ast.Imm(value=0), dst=_A))

    def _add(self, after):
        return _wrap([
            asm_ast.Mov(src=asm_ast.Imm(value=0), dst=_A),
            asm_ast.ClearCarry(),
            asm_ast.Add(src=asm_ast.Imm(value=1), dst=_A),
            asm_ast.Mov(src=_A, dst=asm_ast.Data(name="m", offset=0)),
            *after,
            asm_ast.Return(),
        ])

    def test_constant_add_folds_when_carry_is_dead(self):
        out = _instrs(apply_const_arith_fold(self._add([
            asm_ast.ClearCarry(),
        ])))
        self.assertEqual(out[0], asm_ast.Mov(src=asm_ast.Imm(value=1), dst=_A))
        self.assertNotIn(asm_ast.Add(src=asm_ast.Imm(value=1), dst=_A), out)

    def test_constant_add_kept_when_carry_is_read(self):
        prog = self._add([
            asm_ast.Mov(src=asm_ast.Imm(value=0), dst=_A),
            asm_ast.Add(src=asm_ast.Imm(value=0), dst=_A),
        ])
        self.assertEqual(apply_const_arith_fold(prog), prog)

    def _zero_add(self, flag, op, after):
        """`LDA m; <flag>; <op> #0; STA d; <after>; RTS`."""
        return _wrap([
            asm_ast.Mov(src=asm_ast.Data(name="m", offset=0), dst=_A),
            flag,
            op(src=asm_ast.Imm(value=0), dst=_A),
            asm_ast.Mov(src=_A, dst=asm_ast.Data(name="d", offset=0)),
            *after,
            asm_ast.Return(),
        ])

    def test_add_of_zero_after_clc_goes(self):
        # `p + (n << 8)`'s low byte: the add of the shift's 0 leaves A
        # and C as they were, and the high byte's ADC reads the C.
        high = [
            asm_ast.Mov(src=asm_ast.Data(name="m", offset=1), dst=_A),
            asm_ast.Add(src=asm_ast.Data(name="n", offset=0), dst=_A),
        ]
        prog = self._zero_add(asm_ast.ClearCarry(), asm_ast.Add, high)
        out = _instrs(apply_const_arith_fold(prog))
        self.assertEqual(out[:3], [
            asm_ast.Mov(src=asm_ast.Data(name="m", offset=0), dst=_A),
            asm_ast.ClearCarry(),
            asm_ast.Mov(src=_A, dst=asm_ast.Data(name="d", offset=0)),
        ])

    def test_subtract_of_zero_after_sec_goes(self):
        prog = self._zero_add(asm_ast.SetCarry(), asm_ast.Sub, [])
        out = _instrs(apply_const_arith_fold(prog))
        self.assertNotIn(asm_ast.Sub(src=asm_ast.Imm(value=0), dst=_A), out)

    def test_add_of_zero_kept_when_v_is_read(self):
        # The ADC clears V, which the CLC doesn't.
        prog = self._zero_add(asm_ast.ClearCarry(), asm_ast.Add, [
            asm_ast.Branch(cond=asm_ast.VC(), target=".t"),
            asm_ast.Label(name=".t"),
        ])
        self.assertEqual(apply_const_arith_fold(prog), prog)

    def test_add_of_zero_kept_with_the_other_carry(self):
        # `SEC; ADC #0` adds 1.
        prog = self._zero_add(asm_ast.SetCarry(), asm_ast.Add, [])
        self.assertEqual(apply_const_arith_fold(prog), prog)


    def _high_add(self, flag, op, value, label_offset=0):
        return _wrap([
            asm_ast.Mov(src=asm_ast.ImmLabelHigh(name="t", offset=label_offset),
                        dst=_A),
            flag,
            op(src=asm_ast.Imm(value=value), dst=_A),
            asm_ast.Mov(src=_A, dst=asm_ast.Data(name="d", offset=0)),
            asm_ast.Return(),
        ])

    def test_label_high_plus_constant(self):
        # `t + 0x100` on a pointer whose low byte is `t`'s.
        out = _instrs(apply_const_arith_fold(self._high_add(
            asm_ast.ClearCarry(), asm_ast.Add, 1, label_offset=128)))
        self.assertEqual(out[0], asm_ast.Mov(
            src=asm_ast.ImmLabelHigh(name="t", offset=384), dst=_A))
        self.assertEqual(len(out), 3)

    def test_label_high_minus_constant_below_label_left_alone(self):
        # `t - 0x100` would be written `t+-256`.
        prog = self._high_add(asm_ast.SetCarry(), asm_ast.Sub, 1)
        self.assertEqual(apply_const_arith_fold(prog), prog)

    def _shift_add_one(self, after):
        return _wrap([
            asm_ast.Mov(src=asm_ast.Data(name="m", offset=0), dst=_A),
            asm_ast.ArithmeticShiftLeft(dst=_A),
            asm_ast.ClearCarry(),
            asm_ast.Add(src=asm_ast.Imm(value=1), dst=_A),
            *after,
            asm_ast.Return(save_a=True),
        ])

    def test_shift_then_add_one_rotates_a_one_in(self):
        out = _instrs(apply_const_arith_fold(self._shift_add_one([])))
        self.assertEqual(out[1:3], [
            asm_ast.SetCarry(), asm_ast.RotateLeft(dst=_A),
        ])

    def test_shift_then_add_one_kept_when_carry_is_read(self):
        # The add leaves C clear, the rotate leaves the old bit 7.
        prog = self._shift_add_one([
            asm_ast.Branch(cond=asm_ast.CS(), target=".t"),
            asm_ast.Label(name=".t"),
        ])
        self.assertEqual(apply_const_arith_fold(prog), prog)

    def _two_adds(self, first, flag, second, after):
        return _wrap([
            asm_ast.Mov(src=asm_ast.Data(name="m", offset=0), dst=_A),
            first(src=asm_ast.Imm(value=0x15), dst=_A),
            flag,
            second(src=asm_ast.Imm(value=0x01), dst=_A),
            *after,
            asm_ast.Return(save_a=True),
        ])

    def test_add_then_add_is_one_add(self):
        # `a % 21 + 1` after the division loop: ADC #$15; CLC; ADC #$01.
        prog = self._two_adds(asm_ast.Add, asm_ast.ClearCarry(),
                              asm_ast.Add, [])
        out = _instrs(apply_const_arith_fold(prog))
        self.assertEqual(out[1:2], [
            asm_ast.Add(src=asm_ast.Imm(value=0x16), dst=_A),
        ])
        self.assertEqual(len(out), 3)

    def test_subtract_then_add_is_one_subtract(self):
        prog = self._two_adds(asm_ast.Sub, asm_ast.ClearCarry(),
                              asm_ast.Add, [])
        out = _instrs(apply_const_arith_fold(prog))
        self.assertEqual(out[1:2], [
            asm_ast.Sub(src=asm_ast.Imm(value=0x14), dst=_A),
        ])

    def test_add_then_subtract_is_one_add(self):
        prog = self._two_adds(asm_ast.Add, asm_ast.SetCarry(),
                              asm_ast.Sub, [])
        out = _instrs(apply_const_arith_fold(prog))
        self.assertEqual(out[1:2], [
            asm_ast.Add(src=asm_ast.Imm(value=0x14), dst=_A),
        ])

    def test_two_adds_kept_when_carry_is_read(self):
        prog = self._two_adds(asm_ast.Add, asm_ast.ClearCarry(),
                              asm_ast.Add, [
                                  asm_ast.Branch(cond=asm_ast.CS(),
                                                 target=".t"),
                                  asm_ast.Label(name=".t"),
                              ])
        self.assertEqual(apply_const_arith_fold(prog), prog)

    def test_two_adds_kept_with_the_wrong_carry(self):
        # `SEC; ADC #1` adds 2, which the fold doesn't model.
        prog = self._two_adds(asm_ast.Add, asm_ast.SetCarry(),
                              asm_ast.Add, [])
        self.assertEqual(apply_const_arith_fold(prog), prog)

if __name__ == "__main__":
    unittest.main()
