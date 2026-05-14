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


if __name__ == "__main__":
    unittest.main()
