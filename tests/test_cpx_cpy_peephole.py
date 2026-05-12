"""Tests for the CPX / CPY peephole.

`apply_cpx_cpy_peephole` rewrites

    Mov(Reg(X|Y), Reg(A))       ; TXA / TYA
    Compare(Reg(A), R)          ; CMP R

into

    Compare(Reg(X|Y), R)        ; CPX / CPY R

when R ∈ Imm/Data/ZP and Reg(A) is dead after the Compare (the
CFG-wide walk in `asm_liveness.a_dead_at`). Saves 1 byte / 2
cycles per occurrence.

Coverage:
  * Canonical X-side and Y-side rewrites with each addressing
    mode (Imm / Data / ZP).
  * Right operand outside the CPX/CPY-addressable set (Stack /
    Frame / Indirect) blocks the fold.
  * A-liveness blocks the fold when a subsequent instruction
    reads A before killing it.
  * The CFG walk lets the fold fire across a Branch when both
    paths terminate cleanly (e.g. loop tail compare-and-branch).
"""
from __future__ import annotations

import unittest

import asm_ast
from passes.cpx_cpy_peephole import apply_cpx_cpy_peephole


_REG_A = asm_ast.Reg(reg=asm_ast.A())
_REG_X = asm_ast.Reg(reg=asm_ast.X())
_REG_Y = asm_ast.Reg(reg=asm_ast.Y())


def _fn(instrs: list[asm_ast.Type_instruction]) -> asm_ast.Function:
    return asm_ast.Function(
        name="f", is_global=True, params=[], instructions=instrs,
    )


def _prog(instrs: list[asm_ast.Type_instruction]) -> asm_ast.Program:
    return asm_ast.Program(top_level=[_fn(instrs)])


def _rewritten(instrs):
    return apply_cpx_cpy_peephole(_prog(instrs)).top_level[0].instructions


class TestCpxCpyBasic(unittest.TestCase):
    def test_txa_cmp_imm_folds_to_cpx(self) -> None:
        # TXA; CMP #$28; Return → CPX #$28; Return.
        instrs = [
            asm_ast.Mov(src=_REG_X, dst=_REG_A),
            asm_ast.Compare(left=_REG_A, right=asm_ast.Imm(value=0x28)),
            asm_ast.Return(save_a=False),
        ]
        out = _rewritten(instrs)
        self.assertEqual(len(out), 2)
        self.assertEqual(
            out[0],
            asm_ast.Compare(left=_REG_X, right=asm_ast.Imm(value=0x28)),
        )

    def test_tya_cmp_imm_folds_to_cpy(self) -> None:
        instrs = [
            asm_ast.Mov(src=_REG_Y, dst=_REG_A),
            asm_ast.Compare(left=_REG_A, right=asm_ast.Imm(value=0x10)),
            asm_ast.Return(save_a=False),
        ]
        out = _rewritten(instrs)
        self.assertEqual(out[0].left, _REG_Y)

    def test_txa_cmp_zp_folds(self) -> None:
        instrs = [
            asm_ast.Mov(src=_REG_X, dst=_REG_A),
            asm_ast.Compare(
                left=_REG_A,
                right=asm_ast.ZP(address=0x83, offset=0),
            ),
            asm_ast.Return(save_a=False),
        ]
        out = _rewritten(instrs)
        self.assertEqual(len(out), 2)
        self.assertEqual(out[0].left, _REG_X)
        self.assertEqual(out[0].right, asm_ast.ZP(address=0x83, offset=0))

    def test_txa_cmp_data_folds(self) -> None:
        instrs = [
            asm_ast.Mov(src=_REG_X, dst=_REG_A),
            asm_ast.Compare(
                left=_REG_A,
                right=asm_ast.Data(name="g", offset=0),
            ),
            asm_ast.Return(save_a=False),
        ]
        out = _rewritten(instrs)
        self.assertEqual(out[0].left, _REG_X)
        self.assertEqual(out[0].right, asm_ast.Data(name="g", offset=0))


class TestCpxCpyRightOperand(unittest.TestCase):
    def test_frame_right_does_not_fold(self) -> None:
        # CPX/CPY have no indirect-Y mode, so a Frame right operand
        # must stay routed through CMP.
        instrs = [
            asm_ast.Mov(src=_REG_X, dst=_REG_A),
            asm_ast.Compare(left=_REG_A, right=asm_ast.Frame(offset=3)),
            asm_ast.Return(save_a=False),
        ]
        out = _rewritten(instrs)
        # No change.
        self.assertEqual(out, instrs)

    def test_stack_right_does_not_fold(self) -> None:
        instrs = [
            asm_ast.Mov(src=_REG_X, dst=_REG_A),
            asm_ast.Compare(left=_REG_A, right=asm_ast.Stack(offset=0)),
            asm_ast.Return(save_a=False),
        ]
        out = _rewritten(instrs)
        self.assertEqual(out, instrs)

    def test_indirect_right_does_not_fold(self) -> None:
        instrs = [
            asm_ast.Mov(src=_REG_X, dst=_REG_A),
            asm_ast.Compare(left=_REG_A, right=asm_ast.Indirect(offset=0)),
            asm_ast.Return(save_a=False),
        ]
        out = _rewritten(instrs)
        self.assertEqual(out, instrs)


class TestCpxCpyLiveness(unittest.TestCase):
    def test_subsequent_a_read_blocks_fold(self) -> None:
        # A is read after the Compare via STA $84.
        instrs = [
            asm_ast.Mov(src=_REG_X, dst=_REG_A),
            asm_ast.Compare(left=_REG_A, right=asm_ast.Imm(value=0)),
            asm_ast.Mov(src=_REG_A, dst=asm_ast.ZP(address=0x84, offset=0)),
            asm_ast.Return(save_a=False),
        ]
        out = _rewritten(instrs)
        self.assertEqual(out, instrs)

    def test_subsequent_a_kill_allows_fold(self) -> None:
        # LDA #imm kills A without reading it.
        instrs = [
            asm_ast.Mov(src=_REG_X, dst=_REG_A),
            asm_ast.Compare(left=_REG_A, right=asm_ast.Imm(value=0)),
            asm_ast.Mov(src=asm_ast.Imm(value=42), dst=_REG_A),
            asm_ast.Return(save_a=False),
        ]
        out = _rewritten(instrs)
        self.assertEqual(out[0].left, _REG_X)

    def test_save_a_return_blocks_fold(self) -> None:
        # Return(save_a=True) does PHA — reads A.
        instrs = [
            asm_ast.Mov(src=_REG_X, dst=_REG_A),
            asm_ast.Compare(left=_REG_A, right=asm_ast.Imm(value=0)),
            asm_ast.Return(save_a=True),
        ]
        out = _rewritten(instrs)
        self.assertEqual(out, instrs)


class TestCpxCpyCfg(unittest.TestCase):
    def test_branch_with_clean_targets_folds(self) -> None:
        # The loop-tail shape: TXA; CMP $83; BNE target; ...
        # Both fall-through and target paths kill A before reading.
        instrs = [
            asm_ast.Mov(src=_REG_X, dst=_REG_A),
            asm_ast.Compare(
                left=_REG_A,
                right=asm_ast.ZP(address=0x83, offset=0),
            ),
            asm_ast.Branch(cond=asm_ast.NE(), target="L"),
            # Fall-through path: a Return is sufficient.
            asm_ast.Return(save_a=False),
            asm_ast.Label(name="L"),
            # Target path: LDA kills A.
            asm_ast.Mov(src=asm_ast.Imm(value=0), dst=_REG_A),
            asm_ast.Return(save_a=False),
        ]
        out = _rewritten(instrs)
        # Compare is rewritten; TXA dropped.
        self.assertEqual(len(out), 6)
        self.assertEqual(
            out[0],
            asm_ast.Compare(
                left=_REG_X,
                right=asm_ast.ZP(address=0x83, offset=0),
            ),
        )

    def test_branch_target_reads_a_blocks_fold(self) -> None:
        # Target path: STA $84 reads A — A is live there. Fold blocks.
        instrs = [
            asm_ast.Mov(src=_REG_X, dst=_REG_A),
            asm_ast.Compare(left=_REG_A, right=asm_ast.Imm(value=0)),
            asm_ast.Branch(cond=asm_ast.NE(), target="L"),
            asm_ast.Return(save_a=False),
            asm_ast.Label(name="L"),
            asm_ast.Mov(src=_REG_A, dst=asm_ast.ZP(address=0x84, offset=0)),
            asm_ast.Return(save_a=False),
        ]
        out = _rewritten(instrs)
        self.assertEqual(out, instrs)
