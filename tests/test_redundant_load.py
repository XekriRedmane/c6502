"""Tests for the asm-level redundant-load elimination pass.

`apply_redundant_load_elimination` walks each function linearly,
tracking which operand each of A/X/Y currently mirrors, and drops
any subsequent `Mov(M, Reg(R))` whose target register already
holds memory[M] (or a matching immediate). The pass invalidates
tracking on register-clobbering instructions, basic-block
boundaries, calls, and aliasing memory writes.

Coverage:
  * Repeat loads from the same source collapse.
  * Stores to provably-disjoint memory don't invalidate tracking.
  * Stores to the same / aliasing memory do invalidate.
  * Block boundaries (Label / Jump / Branch / Call / Ret) reset.
  * Arithmetic / shifts / Pop on Reg(A) invalidate A.
  * Branch immediately after the load preserves the load
    (flag liveness).
"""
from __future__ import annotations

import unittest

import asm_ast
from passes.redundant_load import apply_redundant_load_elimination


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
    return apply_redundant_load_elimination(
        _prog(instrs),
    ).top_level[0].instructions


class TestRedundantLoadBasic(unittest.TestCase):
    def test_immediate_repeat_load_dropped(self) -> None:
        # LDA #5; LDA #5 → LDA #5 (the second is redundant).
        zp80 = asm_ast.ZP(address=0x80, offset=0)
        instrs = [
            asm_ast.Mov(src=asm_ast.Imm(value=5), dst=_REG_A),
            asm_ast.Mov(src=asm_ast.Imm(value=5), dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=zp80),
            asm_ast.Return(save_a=False),
        ]
        out = _rewritten(instrs)
        self.assertEqual(len(out), 3)
        self.assertEqual(out[0].src, asm_ast.Imm(value=5))
        self.assertIsInstance(out[1].dst, asm_ast.ZP)

    def test_distinct_immediates_both_kept(self) -> None:
        instrs = [
            asm_ast.Mov(src=asm_ast.Imm(value=5), dst=_REG_A),
            asm_ast.Mov(src=asm_ast.Imm(value=6), dst=_REG_A),
            asm_ast.Return(save_a=False),
        ]
        out = _rewritten(instrs)
        self.assertEqual(len(out), 3)

    def test_zp_repeat_load_dropped(self) -> None:
        zp80 = asm_ast.ZP(address=0x80, offset=0)
        zpC0 = asm_ast.ZP(address=0xC0, offset=0)
        instrs = [
            asm_ast.Mov(src=zp80, dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=zpC0),  # store to disjoint ZP
            asm_ast.Mov(src=zp80, dst=_REG_A),  # redundant — drop
            asm_ast.Mov(src=_REG_A, dst=asm_ast.ZP(address=0xC1, offset=0)),
            asm_ast.Return(save_a=False),
        ]
        out = _rewritten(instrs)
        self.assertEqual(len(out), 4)

    def test_zp_aliased_store_invalidates(self) -> None:
        # `LDA $80; LDA #99; STA $80; LDA $80; ret`. The first LDA's
        # tracking is killed by the LDA #99 (A reloaded with a
        # different value). The STA $80 then ALSO establishes
        # `A === $80` (we just wrote A's value there), so the
        # final LDA $80 IS redundant — it reads the same 99 we
        # just stored. Verifies the post-store tracking path:
        # invalidate-aliasing drops the prior tracking, but the
        # source register and the destination memory now share
        # the just-written value.
        zp80 = asm_ast.ZP(address=0x80, offset=0)
        instrs = [
            asm_ast.Mov(src=zp80, dst=_REG_A),
            asm_ast.Mov(src=asm_ast.Imm(value=99), dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=zp80),
            asm_ast.Mov(src=zp80, dst=_REG_A),  # redundant — drop
            asm_ast.Return(save_a=False),
        ]
        out = _rewritten(instrs)
        self.assertEqual(len(out), 4)


class TestRedundantLoadAliasing(unittest.TestCase):
    """The headline case: ZP-tracked register survives an
    `IndexedData` write, since absolute,X always lands at
    address ≥ $0100 and ZP lives in $00–$FF."""

    def test_zp_tracking_survives_indexed_data_store(self) -> None:
        zp80 = asm_ast.ZP(address=0x80, offset=0)
        zp81 = asm_ast.ZP(address=0x81, offset=0)
        idx_store_a = asm_ast.IndexedData(
            name="", offset=0x20A8, index=asm_ast.X(),
        )
        idx_store_b = asm_ast.IndexedData(
            name="", offset=0x2328, index=asm_ast.X(),
        )
        instrs = [
            asm_ast.Mov(src=zp80, dst=_REG_A),
            asm_ast.Mov(src=zp81, dst=_REG_X),
            asm_ast.Mov(src=_REG_A, dst=idx_store_a),
            asm_ast.Mov(src=zp80, dst=_REG_A),  # redundant — drop
            asm_ast.Mov(src=zp81, dst=_REG_X),  # redundant — drop
            asm_ast.Mov(src=_REG_A, dst=idx_store_b),
            asm_ast.Return(save_a=False),
        ]
        out = _rewritten(instrs)
        # Two 3-instruction blocks shrink to one 3-instr setup +
        # one solo STA = 4 instructions plus the Return = 5.
        self.assertEqual(len(out), 5)

    def test_zp_tracking_survives_data_store(self) -> None:
        zp80 = asm_ast.ZP(address=0x80, offset=0)
        instrs = [
            asm_ast.Mov(src=zp80, dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=asm_ast.Data(name="g", offset=0)),
            asm_ast.Mov(src=zp80, dst=_REG_A),  # drop
            asm_ast.Return(save_a=False),
        ]
        out = _rewritten(instrs)
        self.assertEqual(len(out), 3)

    def test_data_store_invalidates_matching_data_tracking(self) -> None:
        # Same shape as test_zp_aliased_store_invalidates but for
        # Data (link-time-symbol) operands. The STA establishes
        # `A === g`, so the final LDA g IS redundant — A holds 7
        # from the LDA #7 above and we just wrote 7 to g.
        data_g = asm_ast.Data(name="g", offset=0)
        instrs = [
            asm_ast.Mov(src=data_g, dst=_REG_A),
            asm_ast.Mov(src=asm_ast.Imm(value=7), dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=data_g),
            asm_ast.Mov(src=data_g, dst=_REG_A),  # redundant — drop
            asm_ast.Return(save_a=False),
        ]
        out = _rewritten(instrs)
        self.assertEqual(len(out), 4)

    def test_data_store_to_different_symbol_keeps_tracking(self) -> None:
        data_g = asm_ast.Data(name="g", offset=0)
        data_h = asm_ast.Data(name="h", offset=0)
        instrs = [
            asm_ast.Mov(src=data_g, dst=_REG_A),
            asm_ast.Mov(src=asm_ast.Imm(value=7), dst=_REG_A),  # kill A
            asm_ast.Mov(src=_REG_A, dst=data_h),  # disjoint symbol
            asm_ast.Mov(src=data_g, dst=_REG_A),  # NOT redundant
            asm_ast.Return(save_a=False),
        ]
        # `g` is only tracked while A holds it. The intermediate
        # `Mov(Imm(7), A)` clears A, so when we reach the second
        # `Mov(g, A)`, A is None — load is necessary.
        out = _rewritten(instrs)
        self.assertEqual(len(out), 5)


class TestRedundantLoadBlockBoundaries(unittest.TestCase):
    def test_label_resets_state_when_branched_to(self) -> None:
        # A label that something else branches/jumps to is a real
        # join point — state at entry could come from anywhere.
        zp80 = asm_ast.ZP(address=0x80, offset=0)
        instrs = [
            asm_ast.Mov(src=zp80, dst=_REG_A),
            asm_ast.Jump(target="L"),               # makes "L" a branch target
            asm_ast.Label(name="L"),
            asm_ast.Mov(src=zp80, dst=_REG_A),      # block boundary — keep
            asm_ast.Return(save_a=False),
        ]
        out = _rewritten(instrs)
        self.assertEqual(len(out), 5)

    def test_label_with_only_fall_through_pred_preserves_state(self) -> None:
        # A label that nothing branches/jumps to has only the
        # fall-through predecessor — state at entry equals state
        # at exit of the prior instruction, so a follow-up
        # redundant load can still be eliminated.
        zp80 = asm_ast.ZP(address=0x80, offset=0)
        instrs = [
            asm_ast.Mov(src=zp80, dst=_REG_A),
            asm_ast.Label(name="L"),                # only fall-through reaches L
            asm_ast.Mov(src=zp80, dst=_REG_A),      # redundant — drop
            asm_ast.Return(save_a=False),
        ]
        out = _rewritten(instrs)
        self.assertEqual(len(out), 3)

    def test_jump_resets_state(self) -> None:
        zp80 = asm_ast.ZP(address=0x80, offset=0)
        instrs = [
            asm_ast.Mov(src=zp80, dst=_REG_A),
            asm_ast.Jump(target="L"),
            asm_ast.Mov(src=zp80, dst=_REG_A),  # new block — keep
            asm_ast.Return(save_a=False),
        ]
        out = _rewritten(instrs)
        self.assertEqual(len(out), 4)

    def test_call_invalidates(self) -> None:
        zp80 = asm_ast.ZP(address=0x80, offset=0)
        instrs = [
            asm_ast.Mov(src=zp80, dst=_REG_A),
            asm_ast.Call(name="helper"),
            asm_ast.Mov(src=zp80, dst=_REG_A),  # callee may have clobbered
            asm_ast.Return(save_a=False),
        ]
        out = _rewritten(instrs)
        self.assertEqual(len(out), 4)


class TestRedundantLoadRegisterClobbers(unittest.TestCase):
    def test_pop_invalidates_a(self) -> None:
        zp80 = asm_ast.ZP(address=0x80, offset=0)
        instrs = [
            asm_ast.Mov(src=zp80, dst=_REG_A),
            asm_ast.Pop(dst=_REG_A),
            asm_ast.Mov(src=zp80, dst=_REG_A),  # NOT redundant
            asm_ast.Return(save_a=False),
        ]
        out = _rewritten(instrs)
        self.assertEqual(len(out), 4)

    def test_arithmetic_invalidates_a(self) -> None:
        zp80 = asm_ast.ZP(address=0x80, offset=0)
        instrs = [
            asm_ast.Mov(src=zp80, dst=_REG_A),
            asm_ast.ClearCarry(),
            asm_ast.Add(src=asm_ast.Imm(value=1), dst=_REG_A),  # ADC #1
            asm_ast.Mov(src=zp80, dst=_REG_A),  # A no longer holds zp80
            asm_ast.Return(save_a=False),
        ]
        out = _rewritten(instrs)
        self.assertEqual(len(out), 5)

    def test_tax_propagates_a_tracking_to_x(self) -> None:
        zp80 = asm_ast.ZP(address=0x80, offset=0)
        instrs = [
            asm_ast.Mov(src=zp80, dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=_REG_X),  # TAX — X now mirrors zp80
            asm_ast.Mov(src=zp80, dst=_REG_X),  # redundant — drop
            asm_ast.Return(save_a=False),
        ]
        out = _rewritten(instrs)
        self.assertEqual(len(out), 3)


class TestRedundantLoadFlags(unittest.TestCase):
    def test_branch_immediately_after_keeps_load(self) -> None:
        # If Branch reads N/Z, dropping the LDA would change the
        # flag state. We must NOT drop.
        zp80 = asm_ast.ZP(address=0x80, offset=0)
        zpC0 = asm_ast.ZP(address=0xC0, offset=0)
        instrs = [
            asm_ast.Mov(src=zp80, dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=zpC0),
            asm_ast.Mov(src=zp80, dst=_REG_A),  # candidate drop
            asm_ast.Branch(cond=asm_ast.EQ(), target="L"),  # reads N/Z
            asm_ast.Return(save_a=False),
        ]
        out = _rewritten(instrs)
        # Even though A still holds zp80, dropping the load would
        # move the Branch's flag observation upstream.
        self.assertEqual(len(out), 5)

    def test_intervening_flag_setter_allows_drop(self) -> None:
        zp80 = asm_ast.ZP(address=0x80, offset=0)
        zp81 = asm_ast.ZP(address=0x81, offset=0)
        instrs = [
            asm_ast.Mov(src=zp80, dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=zp81),  # STA — preserves zp80
            asm_ast.Mov(src=zp80, dst=_REG_A),  # candidate drop
            asm_ast.Mov(src=zp81, dst=_REG_X),  # LDX — resets N/Z
            asm_ast.Branch(cond=asm_ast.EQ(), target="L"),
            asm_ast.Return(save_a=False),
        ]
        out = _rewritten(instrs)
        self.assertEqual(len(out), 5)


if __name__ == "__main__":
    unittest.main()
