"""Tests for the dead-A-arith elimination pass.

`apply_dead_a_arith_elimination` drops instructions whose only
observable effects are on `Reg(A)` and the N/Z/C/V flags, when
both `Reg(A)` and the flags are dead afterward.

Coverage:
  * Canonical LDA imm and ADC imm drops when A + flags dead.
  * TXA / TYA drops when A + flags dead.
  * Iteration drops the `LDA #$00 / ADC #$00` pair: ADC goes
    first (A dead after via JMP→DEX→…), then a re-run drops
    the LDA (whose only consumer was the dropped ADC).
  * Operand-shape gates: Frame / Stack / Indirect / IndirectY
    operands aren't dropped because their emission clobbers Y.
  * Liveness gates: a subsequent read of A or of the flags
    blocks the drop.
"""
from __future__ import annotations

import unittest

import asm_ast
from passes.dead_a_arith import apply_dead_a_arith_elimination


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
    return apply_dead_a_arith_elimination(
        _prog(instrs),
    ).top_level[0].instructions


class TestDeadAArithBasic(unittest.TestCase):
    def test_dead_lda_imm_drops(self) -> None:
        # LDA #$00 followed by JMP — A is dead at JMP target.
        instrs = [
            asm_ast.Mov(src=asm_ast.Imm(value=0), dst=_REG_A),
            asm_ast.Jump(target="L"),
            asm_ast.Label(name="L"),
            asm_ast.Return(save_a=False),
        ]
        out = _rewritten(instrs)
        self.assertEqual(len(out), 3)
        self.assertNotIsInstance(out[0], asm_ast.Mov)

    def test_dead_adc_imm_drops(self) -> None:
        instrs = [
            asm_ast.Add(src=asm_ast.Imm(value=0), dst=_REG_A),
            asm_ast.Jump(target="L"),
            asm_ast.Label(name="L"),
            asm_ast.Return(save_a=False),
        ]
        out = _rewritten(instrs)
        self.assertEqual(len(out), 3)
        self.assertNotIsInstance(out[0], asm_ast.Add)

    def test_dead_txa_drops(self) -> None:
        instrs = [
            asm_ast.Mov(src=_REG_X, dst=_REG_A),
            asm_ast.Jump(target="L"),
            asm_ast.Label(name="L"),
            asm_ast.Return(save_a=False),
        ]
        out = _rewritten(instrs)
        self.assertEqual(len(out), 3)


class TestDeadAArithIteration(unittest.TestCase):
    """The headline case: LDA #$00; ADC #$00; JMP. The pass now
    iterates to a local fixed point in a single call — first round
    drops the ADC (A dead after JMP), second round drops the LDA
    (its only consumer was the ADC, now gone). The outer peephole
    fixedpoint depended on dead_a_arith leaving NO droppable atom
    behind in one call: a downstream pass (e.g. volatile_void_read_
    cmp) that extends A's liveness by rewriting LDA-from-indirect
    into CMP-from-indirect would otherwise lock in a still-
    droppable LDA #imm by extending A's liveness across the
    Compare it introduced."""

    def test_pair_drops_in_one_call(self) -> None:
        instrs = [
            asm_ast.Mov(src=asm_ast.Imm(value=0), dst=_REG_A),
            asm_ast.Add(src=asm_ast.Imm(value=0), dst=_REG_A),
            asm_ast.Jump(target="L"),
            asm_ast.Label(name="L"),
            asm_ast.Return(save_a=False),
        ]
        # Single call now drops BOTH: ADC dropped in round 1, then
        # LDA dropped in round 2 (now that the ADC's A-read is
        # gone). Local fixed point converges in 2 rounds.
        out = _rewritten(instrs)
        self.assertEqual(len(out), 3)
        self.assertIsInstance(out[0], asm_ast.Jump)


class TestDeadAArithOperandShape(unittest.TestCase):
    def test_frame_source_does_not_drop(self) -> None:
        # LDA (FP),Y emits LDY #imm; LDA (FP),Y — clobbers Y.
        # Dropping the LDA would lose that Y clobber.
        instrs = [
            asm_ast.Mov(src=asm_ast.Frame(offset=3), dst=_REG_A),
            asm_ast.Jump(target="L"),
            asm_ast.Label(name="L"),
            asm_ast.Return(save_a=False),
        ]
        out = _rewritten(instrs)
        self.assertEqual(out, instrs)

    def test_indirect_source_does_not_drop(self) -> None:
        instrs = [
            asm_ast.Mov(src=asm_ast.Indirect(offset=0), dst=_REG_A),
            asm_ast.Jump(target="L"),
            asm_ast.Label(name="L"),
            asm_ast.Return(save_a=False),
        ]
        out = _rewritten(instrs)
        self.assertEqual(out, instrs)

    def test_zp_source_drops(self) -> None:
        # LDA $80 is a pure load — no LDY setup. Droppable.
        instrs = [
            asm_ast.Mov(
                src=asm_ast.ZP(address=0x80, offset=0), dst=_REG_A,
            ),
            asm_ast.Jump(target="L"),
            asm_ast.Label(name="L"),
            asm_ast.Return(save_a=False),
        ]
        out = _rewritten(instrs)
        self.assertEqual(len(out), 3)

    def test_data_source_drops(self) -> None:
        instrs = [
            asm_ast.Mov(
                src=asm_ast.Data(name="g", offset=0), dst=_REG_A,
            ),
            asm_ast.Jump(target="L"),
            asm_ast.Label(name="L"),
            asm_ast.Return(save_a=False),
        ]
        out = _rewritten(instrs)
        self.assertEqual(len(out), 3)


class TestDeadAArithLiveness(unittest.TestCase):
    def test_subsequent_a_read_blocks_drop(self) -> None:
        # STA $84 after LDA reads A — A is live.
        instrs = [
            asm_ast.Mov(src=asm_ast.Imm(value=0), dst=_REG_A),
            asm_ast.Mov(
                src=_REG_A,
                dst=asm_ast.ZP(address=0x84, offset=0),
            ),
            asm_ast.Return(save_a=False),
        ]
        out = _rewritten(instrs)
        self.assertEqual(out, instrs)

    def test_subsequent_branch_blocks_drop(self) -> None:
        # LDA sets N/Z; BNE reads them. Drop blocked by
        # flags-live.
        instrs = [
            asm_ast.Mov(src=asm_ast.Imm(value=0), dst=_REG_A),
            asm_ast.Branch(cond=asm_ast.NE(), target="L"),
            asm_ast.Label(name="L"),
            asm_ast.Return(save_a=False),
        ]
        out = _rewritten(instrs)
        self.assertEqual(out, instrs)

    def test_save_a_return_blocks_drop(self) -> None:
        # save_a=True epilogue does PHA — reads A.
        instrs = [
            asm_ast.Mov(src=asm_ast.Imm(value=0), dst=_REG_A),
            asm_ast.Return(save_a=True),
        ]
        out = _rewritten(instrs)
        self.assertEqual(out, instrs)

    def test_subsequent_kill_allows_drop(self) -> None:
        # Second LDA kills A without reading it; both flag and A
        # are then dead at the first LDA's exit.
        instrs = [
            asm_ast.Mov(src=asm_ast.Imm(value=0), dst=_REG_A),
            asm_ast.Mov(src=asm_ast.Imm(value=42), dst=_REG_A),
            asm_ast.Mov(
                src=_REG_A,
                dst=asm_ast.ZP(address=0x90, offset=0),
            ),
            asm_ast.Return(save_a=False),
        ]
        out = _rewritten(instrs)
        # The first LDA drops; the second's value is observed by
        # the STA so it stays.
        self.assertEqual(len(out), 3)
        self.assertEqual(out[0].src, asm_ast.Imm(value=42))
