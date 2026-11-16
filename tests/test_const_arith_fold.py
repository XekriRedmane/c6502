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


if __name__ == "__main__":
    unittest.main()
