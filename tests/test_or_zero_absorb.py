"""Tests for the asm-SSA `absorb_zero_load` pass.

Folds `Mov(Imm(0), A); Or(X, A)` to `Mov(X, A)`.
"""
from __future__ import annotations

import unittest

import asm_ast
from passes.optimization_asm.or_zero_absorb import absorb_zero_load


_REG_A = asm_ast.Reg(reg=asm_ast.A())
_REG_X = asm_ast.Reg(reg=asm_ast.X())


def _fn(instrs):
    return asm_ast.Function(
        name="f", is_global=True, params=[], instructions=instrs,
    )


class TestAbsorbZeroLoad(unittest.TestCase):
    def test_absorbs_pseudo_source(self) -> None:
        p = asm_ast.Pseudo(name="%t", offset=0)
        instrs = [
            asm_ast.Mov(src=asm_ast.Imm(value=0), dst=_REG_A),
            asm_ast.Or(src=p, dst=_REG_A),
            asm_ast.Return(save_a=False),
        ]
        out = absorb_zero_load(_fn(instrs)).instructions
        self.assertEqual(
            out,
            [
                asm_ast.Mov(src=p, dst=_REG_A),
                asm_ast.Return(save_a=False),
            ],
        )

    def test_absorbs_data_source(self) -> None:
        d = asm_ast.Data(name="x", offset=0)
        instrs = [
            asm_ast.Mov(src=asm_ast.Imm(value=0), dst=_REG_A),
            asm_ast.Or(src=d, dst=_REG_A),
        ]
        out = absorb_zero_load(_fn(instrs)).instructions
        self.assertEqual(out, [asm_ast.Mov(src=d, dst=_REG_A)])

    def test_non_zero_immediate_not_absorbed(self) -> None:
        instrs = [
            asm_ast.Mov(src=asm_ast.Imm(value=1), dst=_REG_A),
            asm_ast.Or(src=asm_ast.Data(name="x", offset=0), dst=_REG_A),
        ]
        out = absorb_zero_load(_fn(instrs)).instructions
        self.assertEqual(out, instrs)

    def test_or_to_non_a_register_not_absorbed(self) -> None:
        # ORA targets the accumulator in 6502; we never see Or with
        # X/Y as dst at the asm level, but the pass should still bail
        # safely on any non-A dst.
        instrs = [
            asm_ast.Mov(src=asm_ast.Imm(value=0), dst=_REG_A),
            asm_ast.Or(
                src=asm_ast.Data(name="x", offset=0), dst=_REG_X,
            ),
        ]
        out = absorb_zero_load(_fn(instrs)).instructions
        self.assertEqual(out, instrs)

    def test_volatile_mov_not_absorbed(self) -> None:
        # A volatile `LDA #0` is a programmer-meaningful side effect;
        # folding past it would change semantics.
        instrs = [
            asm_ast.Mov(
                src=asm_ast.Imm(value=0), dst=_REG_A, is_volatile=True,
            ),
            asm_ast.Or(src=asm_ast.Data(name="x", offset=0), dst=_REG_A),
        ]
        out = absorb_zero_load(_fn(instrs)).instructions
        self.assertEqual(out, instrs)

    def test_non_adjacent_not_absorbed(self) -> None:
        # An intervening instruction (even a flag-preserving one) means
        # we can't safely combine — the absorb pass is window-of-2 only.
        instrs = [
            asm_ast.Mov(src=asm_ast.Imm(value=0), dst=_REG_A),
            asm_ast.ClearCarry(),
            asm_ast.Or(src=asm_ast.Data(name="x", offset=0), dst=_REG_A),
        ]
        out = absorb_zero_load(_fn(instrs)).instructions
        self.assertEqual(out, instrs)


if __name__ == "__main__":
    unittest.main()
