"""Tests for the AND #$80 + BEQ/BNE → BPL/BMI peephole."""

import unittest

import asm_ast
from passes.and_sign_bit_branch import apply_and_sign_bit_branch


_A = asm_ast.Reg(reg=asm_ast.A())


def _instrs(prog):
    return prog.top_level[0].instructions


def _wrap(instrs):
    return asm_ast.Program(top_level=[asm_ast.Function(
        name="f", is_global=False, params=[], instructions=instrs,
    )])


def _ret():
    """Function-exit atom — used to terminate the function so the
    A-liveness walk has somewhere to bottom out. `save_a=False`
    means A is dead at exit."""
    return asm_ast.Return(save_a=False)


class TestAndSignBitBranch(unittest.TestCase):

    def test_lda_and_80_beq_folds_to_lda_bpl(self):
        prog = _wrap([
            asm_ast.Mov(src=asm_ast.Data(name="x", offset=0), dst=_A),
            asm_ast.And(src=asm_ast.Imm(value=0x80), dst=_A),
            asm_ast.Branch(cond=asm_ast.EQ(), target=".target"),
            _ret(),
        ])
        out = _instrs(apply_and_sign_bit_branch(prog))
        self.assertEqual(out, [
            asm_ast.Mov(src=asm_ast.Data(name="x", offset=0), dst=_A),
            asm_ast.Branch(cond=asm_ast.PL(), target=".target"),
            _ret(),
        ])

    def test_lda_and_80_bne_folds_to_lda_bmi(self):
        prog = _wrap([
            asm_ast.Mov(src=asm_ast.Data(name="x", offset=0), dst=_A),
            asm_ast.And(src=asm_ast.Imm(value=0x80), dst=_A),
            asm_ast.Branch(cond=asm_ast.NE(), target=".target"),
            _ret(),
        ])
        out = _instrs(apply_and_sign_bit_branch(prog))
        self.assertEqual(out[1], asm_ast.Branch(cond=asm_ast.MI(), target=".target"))

    def test_does_not_fire_when_a_live_after_branch(self):
        # If A is read after the Branch (e.g., used by an STA), the
        # AND's value matters — don't drop.
        prog = _wrap([
            asm_ast.Mov(src=asm_ast.Data(name="x", offset=0), dst=_A),
            asm_ast.And(src=asm_ast.Imm(value=0x80), dst=_A),
            asm_ast.Branch(cond=asm_ast.EQ(), target=".target"),
            # A is live here:
            asm_ast.Mov(src=_A, dst=asm_ast.Data(name="y", offset=0)),
            _ret(),
        ])
        out = _instrs(apply_and_sign_bit_branch(prog))
        # Unchanged.
        self.assertEqual(len(out), 5)
        self.assertEqual(out[1], asm_ast.And(src=asm_ast.Imm(value=0x80), dst=_A))

    def test_other_and_mask_doesnt_fire(self):
        # The peephole is specific to mask = 0x80 (sign bit).
        prog = _wrap([
            asm_ast.Mov(src=asm_ast.Data(name="x", offset=0), dst=_A),
            asm_ast.And(src=asm_ast.Imm(value=0x40), dst=_A),
            asm_ast.Branch(cond=asm_ast.EQ(), target=".target"),
            _ret(),
        ])
        out = _instrs(apply_and_sign_bit_branch(prog))
        self.assertEqual(len(out), 4)

    def test_non_branch_after_and_doesnt_fire(self):
        prog = _wrap([
            asm_ast.Mov(src=asm_ast.Data(name="x", offset=0), dst=_A),
            asm_ast.And(src=asm_ast.Imm(value=0x80), dst=_A),
            asm_ast.Mov(src=_A, dst=asm_ast.Data(name="y", offset=0)),
            _ret(),
        ])
        out = _instrs(apply_and_sign_bit_branch(prog))
        self.assertEqual(len(out), 4)


if __name__ == "__main__":
    unittest.main()
