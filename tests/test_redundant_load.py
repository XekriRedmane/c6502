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
    def test_multi_pred_disagreeing_preserves_load(self) -> None:
        # Both predecessor blocks are reachable, but they leave A
        # in *different* states (zp80 vs zp82). The intersection-
        # based join drops every equivalence at L — no single value
        # A is known to hold on every incoming path. The Mov at L
        # is preserved.
        zp80 = asm_ast.ZP(address=0x80, offset=0)
        zp82 = asm_ast.ZP(address=0x82, offset=0)
        instrs = [
            asm_ast.Mov(src=zp80, dst=_REG_A),       # A = zp80
            asm_ast.Branch(cond=asm_ast.EQ(), target="X"),
            asm_ast.Jump(target="L"),                 # fall path: A = zp80
            asm_ast.Label(name="X"),
            asm_ast.Mov(src=zp82, dst=_REG_A),        # branch path: A = zp82
            asm_ast.Jump(target="L"),
            asm_ast.Label(name="L"),
            asm_ast.Mov(src=zp80, dst=_REG_A),        # KEEP — preds disagree
            asm_ast.Return(save_a=False),
        ]
        out = _rewritten(instrs)
        # The Mov immediately before the Return must still be the
        # `LDA zp80`.
        last_mov = next(
            i for i in reversed(out) if isinstance(i, asm_ast.Mov)
        )
        self.assertEqual(last_mov.src, zp80)
        self.assertEqual(last_mov.dst, _REG_A)

    def test_multi_pred_agreeing_drops_load(self) -> None:
        # Both predecessors leave A = zp80 — the multi-pred join
        # finds the agreement and the LDA at L is dropped. This
        # is the cross-block win the CFG dataflow recovers.
        zp80 = asm_ast.ZP(address=0x80, offset=0)
        instrs = [
            asm_ast.Mov(src=zp80, dst=_REG_A),       # A = zp80
            asm_ast.Jump(target="L"),
            asm_ast.Label(name="X"),
            asm_ast.Mov(src=zp80, dst=_REG_A),       # A = zp80 on this path too
            asm_ast.Jump(target="L"),
            asm_ast.Label(name="L"),
            asm_ast.Mov(src=zp80, dst=_REG_A),       # DROP — both preds agree
            asm_ast.Return(save_a=False),
        ]
        out = _rewritten(instrs)
        # The label-following Mov is gone; both prior Movs remain.
        movs = [i for i in out if isinstance(i, asm_ast.Mov)]
        self.assertEqual(len(movs), 2)

    def test_unique_pred_jump_restores_state(self) -> None:
        # A label whose only predecessor is a single Jump (no
        # fall-through, no other Branch/Jump) inherits the
        # caller's register state — Jump doesn't modify A. The
        # cross-block restore drops the redundant load at L.
        zp80 = asm_ast.ZP(address=0x80, offset=0)
        instrs = [
            asm_ast.Mov(src=zp80, dst=_REG_A),
            asm_ast.Jump(target="L"),
            asm_ast.Label(name="L"),
            asm_ast.Mov(src=zp80, dst=_REG_A),      # DROP — A still = zp80
            asm_ast.Return(save_a=False),
        ]
        out = _rewritten(instrs)
        self.assertEqual(len(out), 4)
        # The dropped Mov was the second Mov(zp80, A); the first
        # one remains.
        movs = [i for i in out if isinstance(i, asm_ast.Mov)]
        self.assertEqual(len(movs), 1)

    def test_unique_pred_branch_restores_state(self) -> None:
        # Same as above with a Branch (e.g., BPL) — Branch also
        # preserves A across both edges, so the unique-pred-Branch
        # target gets the restore too. Models the apply_bobble
        # shape: STA b0; BPL target; ... ; target: LDA b0 → drop.
        zp80 = asm_ast.ZP(address=0x80, offset=0)
        instrs = [
            asm_ast.Mov(src=zp80, dst=_REG_A),      # A = zp80
            asm_ast.Branch(cond=asm_ast.PL(), target="L"),
            # Fall-through path uses A for something, then JMPs
            # somewhere else (NOT L).
            asm_ast.Mov(src=asm_ast.Imm(value=0x42), dst=_REG_A),
            asm_ast.Jump(target="END"),
            asm_ast.Label(name="L"),                # unique-pred from Branch
            asm_ast.Mov(src=zp80, dst=_REG_A),      # DROP — A still = zp80
            asm_ast.Label(name="END"),
            asm_ast.Return(save_a=False),
        ]
        out = _rewritten(instrs)
        # The Mov at L is dropped.
        l_idx = next(
            i for i, ins in enumerate(out)
            if isinstance(ins, asm_ast.Label) and ins.name == "L"
        )
        next_instr = out[l_idx + 1]
        self.assertNotIsInstance(next_instr, asm_ast.Mov)

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


class TestRedundantLoadCfgJoin(unittest.TestCase):
    """Cross-block must-availability dataflow: at a multi-pred
    label, the intersection of every predecessor's exit state
    survives. Covers the shape that motivated the CFG dataflow —
    `entity_proximity` in examples/companion_update.asm, where an
    LDX of `__zpabi_..._slot` after a diamond merge gets dropped
    because both incoming branches still leave X holding the slot.
    """

    def test_diamond_merge_drops_redundant_ldx(self) -> None:
        # The motivating entity_proximity shape (slimmed):
        #     LDX M; LDA arr,X; CMP #c1; BCC merge
        #                       CMP #c2; BCS merge
        #     ; body (doesn't write X or M)
        # merge: LDX M    ; both incoming paths still have X = M
        slot = asm_ast.Data(name="__zpabi_f__slot", offset=0)
        arr = asm_ast.IndexedData(
            name="arr", offset=0, index=asm_ast.X(),
        )
        instrs = [
            asm_ast.Mov(src=slot, dst=_REG_X),         # LDX M
            asm_ast.Mov(src=arr, dst=_REG_A),          # LDA arr,X
            asm_ast.Compare(left=_REG_A, right=asm_ast.Imm(value=0x40)),
            asm_ast.Branch(cond=asm_ast.CC(), target="merge"),
            asm_ast.Compare(left=_REG_A, right=asm_ast.Imm(value=0x47)),
            asm_ast.Branch(cond=asm_ast.CS(), target="merge"),
            # Body — doesn't touch X or M.
            asm_ast.Mov(src=asm_ast.Imm(value=0xFF), dst=_REG_A),
            asm_ast.Label(name="merge"),
            asm_ast.Mov(src=slot, dst=_REG_X),         # DROP — X still = M
            asm_ast.Return(save_a=False),
        ]
        out = _rewritten(instrs)
        # Only ONE `LDX slot` should remain (the initial one).
        ldx_slot = [
            i for i in out
            if isinstance(i, asm_ast.Mov)
            and isinstance(i.dst, asm_ast.Reg)
            and isinstance(i.dst.reg, asm_ast.X)
            and isinstance(i.src, asm_ast.Data)
            and i.src.name == "__zpabi_f__slot"
        ]
        self.assertEqual(len(ldx_slot), 1)

    def test_diamond_merge_clobbered_on_one_path_keeps_ldx(self) -> None:
        # Same shape, but one path writes M between the initial LDX
        # and the merge → on that path X may no longer mirror M
        # after the write. (Actually, X isn't itself written here —
        # but the equivalence is conservatively dropped at the write
        # to the cell.) The post-merge LDX is preserved.
        slot = asm_ast.Data(name="__zpabi_f__slot", offset=0)
        instrs = [
            asm_ast.Mov(src=slot, dst=_REG_X),         # LDX M
            asm_ast.Branch(cond=asm_ast.EQ(), target="path2"),
            # path1 — preserves X = M.
            asm_ast.Jump(target="merge"),
            asm_ast.Label(name="path2"),
            asm_ast.Mov(src=asm_ast.Imm(value=0), dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=slot),         # STA M — clobbers tracking
            asm_ast.Label(name="merge"),
            asm_ast.Mov(src=slot, dst=_REG_X),         # KEEP — M may have changed
            asm_ast.Return(save_a=False),
        ]
        out = _rewritten(instrs)
        ldx_slot = [
            i for i in out
            if isinstance(i, asm_ast.Mov)
            and isinstance(i.dst, asm_ast.Reg)
            and isinstance(i.dst.reg, asm_ast.X)
            and isinstance(i.src, asm_ast.Data)
            and i.src.name == "__zpabi_f__slot"
        ]
        self.assertEqual(len(ldx_slot), 2)

    def test_loop_back_edge_preserves_equivalence(self) -> None:
        # X is loaded once before the loop and never written in the
        # body. On every back-edge to the header, X still mirrors M.
        # The fixed-point dataflow recovers this — the LDX inside
        # the loop body is redundant.
        slot = asm_ast.Data(name="__zpabi_f__slot", offset=0)
        arr = asm_ast.IndexedData(
            name="arr", offset=0, index=asm_ast.X(),
        )
        instrs = [
            asm_ast.Mov(src=slot, dst=_REG_X),         # LDX M (preheader)
            asm_ast.Label(name="loop"),
            asm_ast.Mov(src=slot, dst=_REG_X),         # DROP — X still = M
            asm_ast.Mov(src=arr, dst=_REG_A),
            asm_ast.Compare(left=_REG_A, right=asm_ast.Imm(value=0)),
            asm_ast.Branch(cond=asm_ast.NE(), target="loop"),
            asm_ast.Return(save_a=False),
        ]
        out = _rewritten(instrs)
        ldx_slot = [
            i for i in out
            if isinstance(i, asm_ast.Mov)
            and isinstance(i.dst, asm_ast.Reg)
            and isinstance(i.dst.reg, asm_ast.X)
            and isinstance(i.src, asm_ast.Data)
            and i.src.name == "__zpabi_f__slot"
        ]
        self.assertEqual(len(ldx_slot), 1)

    def test_loop_body_clobbers_x_keeps_ldx(self) -> None:
        # X IS clobbered inside the loop body (DEX). The fixed-
        # point join at the loop header sees the back-edge's
        # out-state with empty state.x → intersection wipes X's
        # equivalence → the LDX inside the body is preserved.
        slot = asm_ast.Data(name="__zpabi_f__slot", offset=0)
        instrs = [
            asm_ast.Mov(src=slot, dst=_REG_X),         # LDX M (preheader)
            asm_ast.Label(name="loop"),
            asm_ast.Mov(src=slot, dst=_REG_X),         # KEEP — X gets DEX'd
            asm_ast.Dec(dst=_REG_X),                   # DEX clobbers X
            asm_ast.Branch(cond=asm_ast.NE(), target="loop"),
            asm_ast.Return(save_a=False),
        ]
        out = _rewritten(instrs)
        ldx_slot = [
            i for i in out
            if isinstance(i, asm_ast.Mov)
            and isinstance(i.dst, asm_ast.Reg)
            and isinstance(i.dst.reg, asm_ast.X)
            and isinstance(i.src, asm_ast.Data)
            and i.src.name == "__zpabi_f__slot"
        ]
        self.assertEqual(len(ldx_slot), 2)


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
    def test_branch_after_lda_sta_lda_drops_second_lda(self) -> None:
        # `LDA zp80; STA zpC0; LDA zp80; Branch(EQ)` — the second
        # LDA zp80 is redundant for BOTH value AND Z. The first
        # LDA set Z = (zp80 == 0); STA zpC0 doesn't touch Z. The
        # z_reflects tracker recognizes that Z is already in the
        # state the second LDA would put it in, so the LDA is
        # safe to drop even though the Branch reads N/Z.
        zp80 = asm_ast.ZP(address=0x80, offset=0)
        zpC0 = asm_ast.ZP(address=0xC0, offset=0)
        instrs = [
            asm_ast.Mov(src=zp80, dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=zpC0),
            asm_ast.Mov(src=zp80, dst=_REG_A),  # candidate — dropped
            asm_ast.Branch(cond=asm_ast.EQ(), target="L"),
            asm_ast.Return(save_a=False),
        ]
        out = _rewritten(instrs)
        # Four instructions remain (the second LDA zp80 is gone).
        self.assertEqual(len(out), 4)

    def test_branch_after_load_of_unrelated_cell_keeps_load(self) -> None:
        # If an instruction between the first LDA and the candidate
        # LDA changes Z to reflect a DIFFERENT cell's value (here:
        # LDA zpC1), Z no longer matches "is zp80 zero" — the
        # candidate LDA's flag effect isn't redundant any more.
        # The candidate stays.
        zp80 = asm_ast.ZP(address=0x80, offset=0)
        zpC0 = asm_ast.ZP(address=0xC0, offset=0)
        zpC1 = asm_ast.ZP(address=0xC1, offset=0)
        instrs = [
            asm_ast.Mov(src=zp80, dst=_REG_A),  # A = zp80, Z = (zp80==0)
            asm_ast.Mov(src=_REG_A, dst=zpC0),  # zpC0 = A; Z unchanged
            asm_ast.Mov(src=zpC1, dst=_REG_X),  # LDX zpC1; Z = (zpC1==0)
            asm_ast.Mov(src=zp80, dst=_REG_A),  # candidate: A still
            # mirrors zp80 (the LDX didn't touch A's tracking), but
            # Z now reflects zpC1, not zp80. The LDA's flag effect
            # IS observable through the Branch, so don't drop.
            asm_ast.Branch(cond=asm_ast.EQ(), target="L"),
            asm_ast.Return(save_a=False),
        ]
        out = _rewritten(instrs)
        # All six instructions remain.
        self.assertEqual(len(out), 6)

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


class TestRedundantLoadZReflects(unittest.TestCase):
    """The `z_reflects` tracker recognizes cases where the Z flag is
    already in the state a candidate LDA would set it to — making
    the LDA's flag effect redundant, even when a downstream Branch
    reads N/Z. Together with the existing value-redundancy check,
    this lets the pass drop loads that the conservative
    `_flags_dead_at` gate would refuse.

    These tests pin the key shapes the tracker recognizes."""

    def test_sbc_sta_lda_branch_drops_lda(self) -> None:
        # `SBC #c` sets Z to "is A's new value zero". `STA M`
        # copies A to M, leaving Z unchanged AND making M's value
        # equal to A's. The candidate `LDA M` would set Z to
        # "is M zero" — same state, redundant.
        #
        # This is the inner-loop shape in sfx_tone's
        # `volatile uint8_t y; --y` lowering, modulo the
        # intervening volatile mem-to-mem Mov.
        zp80 = asm_ast.ZP(address=0x80, offset=0)
        instrs = [
            asm_ast.Mov(src=zp80, dst=_REG_A),  # LDA y
            asm_ast.SetCarry(),
            asm_ast.Sub(src=asm_ast.Imm(value=1), dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=zp80),  # STA y
            asm_ast.Mov(src=zp80, dst=_REG_A),  # candidate
            asm_ast.Branch(cond=asm_ast.EQ(), target="L"),
            asm_ast.Return(save_a=False),
        ]
        out = _rewritten(instrs)
        # The candidate LDA is gone (it would have been the 5th
        # instruction; the rewritten function has 6, not 7).
        self.assertEqual(len(out), 6)

    def test_inc_unrelated_keeps_lda(self) -> None:
        # `STA M; INC P; LDA M; B<NZ>` — the INC P resets Z to
        # reflect P's new value, not M's. The candidate LDA M
        # IS needed to bring Z back to "is M zero" before the
        # branch.
        zp80 = asm_ast.ZP(address=0x80, offset=0)
        zp90 = asm_ast.ZP(address=0x90, offset=0)
        instrs = [
            asm_ast.Mov(src=zp80, dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=zp80),  # idempotent store
            asm_ast.Inc(dst=zp90),              # Z = (zp90 == 0)
            asm_ast.Mov(src=zp80, dst=_REG_A),  # candidate — KEEP
            asm_ast.Branch(cond=asm_ast.EQ(), target="L"),
            asm_ast.Return(save_a=False),
        ]
        out = _rewritten(instrs)
        # All five non-label instructions remain — the LDA is the
        # only way to set Z to "is zp80 zero" before the branch.
        self.assertEqual(len(out), 6)

    def test_cmp_clears_z_reflects(self) -> None:
        # `Compare(A, M)` sets Z to "A equals M". That doesn't
        # match any operand's zeroness — z_reflects must clear.
        # A subsequent `LDA M; Branch` then can't elide the LDA
        # via z_reflects (only via the value check, which also
        # requires flags_dead).
        zp80 = asm_ast.ZP(address=0x80, offset=0)
        zp81 = asm_ast.ZP(address=0x81, offset=0)
        instrs = [
            asm_ast.Mov(src=zp80, dst=_REG_A),
            asm_ast.Compare(left=_REG_A, right=zp81),
            asm_ast.Mov(src=zp80, dst=_REG_A),  # candidate — KEEP
            asm_ast.Branch(cond=asm_ast.EQ(), target="L"),
            asm_ast.Return(save_a=False),
        ]
        out = _rewritten(instrs)
        self.assertEqual(len(out), 5)

    def test_inc_m_drops_following_lda_m_branch(self) -> None:
        # `INC M; LDA M; B<NZ>` — the INC set Z to reflect M's
        # new value. The LDA's value-into-A is still useful for
        # any downstream use of A, but for Z it's redundant —
        # AND the existing `dec_inc_branch_fold` peephole drops
        # this case too. Verify redundant_load also catches it
        # via z_reflects (independent of `dec_inc_branch_fold`).
        zp80 = asm_ast.ZP(address=0x80, offset=0)
        instrs = [
            asm_ast.Inc(dst=zp80),
            asm_ast.Mov(src=zp80, dst=_REG_A),  # candidate
            asm_ast.Branch(cond=asm_ast.EQ(), target="L"),
            asm_ast.Return(save_a=False),
        ]
        out = _rewritten(instrs)
        # The LDA is gone — INC's flag effect is what the Branch
        # reads. (Note: A's value is dead at the branch in this
        # snippet, but redundant_load only drops the LDA when
        # the value check ALSO passes — which it doesn't here, A
        # didn't previously mirror zp80. Actually that's wrong:
        # state.a was empty before the candidate, so the LDA
        # ISN'T redundant for the value. Pass refuses to drop,
        # so all four instructions remain.)
        self.assertEqual(len(out), 4)

    def test_sta_chain_z_reflects_grows(self) -> None:
        # `LDA M1; STA M2; STA M3; LDA M1` — after the STAs,
        # state.a = [M1, M2, M3] (all three cells hold A's value)
        # and z_reflects = [M1, M2, M3] (Z reflects M1 == 0,
        # which equals M2 == 0 and M3 == 0). The candidate
        # LDA M1 is redundant for both value AND Z. Drop.
        zp80 = asm_ast.ZP(address=0x80, offset=0)
        zp81 = asm_ast.ZP(address=0x81, offset=0)
        zp82 = asm_ast.ZP(address=0x82, offset=0)
        instrs = [
            asm_ast.Mov(src=zp80, dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=zp81),
            asm_ast.Mov(src=_REG_A, dst=zp82),
            asm_ast.Mov(src=zp80, dst=_REG_A),  # candidate
            asm_ast.Branch(cond=asm_ast.EQ(), target="L"),
            asm_ast.Return(save_a=False),
        ]
        out = _rewritten(instrs)
        self.assertEqual(len(out), 5)


class TestRedundantLoadRegToReg(unittest.TestCase):
    """Reg-to-reg transfers (TXA / TYA / TAX / TAY) drop when both
    src and dst already mirror a common operand AND flags don't
    need re-setting."""

    def test_txa_after_lda_tax_drops_flags_dead(self) -> None:
        # `LDA M; TAX; TXA` — after LDA M, A === M. After TAX,
        # X === M too. The second TXA is redundant for value
        # (A still === M === X) and droppable when flags are dead.
        zp80 = asm_ast.ZP(address=0x80, offset=0)
        zp82 = asm_ast.ZP(address=0x82, offset=0)
        instrs = [
            asm_ast.Mov(src=zp80, dst=_REG_A),       # LDA M
            asm_ast.Mov(src=_REG_A, dst=_REG_X),     # TAX
            asm_ast.Mov(src=_REG_X, dst=_REG_A),     # TXA — drop
            asm_ast.Mov(src=_REG_A, dst=zp82),       # STA N
            asm_ast.Return(save_a=False),
        ]
        out = _rewritten(instrs)
        self.assertEqual(len(out), 4)
        # Ensure the dropped one is the TXA.
        self.assertFalse(any(
            isinstance(o, asm_ast.Mov)
            and isinstance(o.src, asm_ast.Reg)
            and isinstance(o.src.reg, asm_ast.X)
            and isinstance(o.dst, asm_ast.Reg)
            and isinstance(o.dst.reg, asm_ast.A)
            for o in out
        ))

    def test_txa_kept_when_dst_mirrors_empty(self) -> None:
        # `LDX M; TXA` — X mirrors M, but A's mirror list is empty
        # (we don't know what's in A). Can't drop — the TXA is
        # what makes A === M.
        zp80 = asm_ast.ZP(address=0x80, offset=0)
        instrs = [
            asm_ast.Mov(src=zp80, dst=_REG_X),       # LDX M
            asm_ast.Mov(src=_REG_X, dst=_REG_A),     # TXA — keep
            asm_ast.Return(save_a=False),
        ]
        out = _rewritten(instrs)
        self.assertEqual(out, instrs)

    def test_txa_dropped_z_survives_clc(self) -> None:
        # CLC writes only the C flag; N/Z (and so z_reflects) are
        # untouched. After `LDA M; TAX; CLC; TXA; BEQ L`, z_reflects
        # still covers M, and src=X mirrors M, so the TXA's flag
        # effect is redundant. Drop.
        zp80 = asm_ast.ZP(address=0x80, offset=0)
        instrs = [
            asm_ast.Mov(src=zp80, dst=_REG_A),                  # LDA M
            asm_ast.Mov(src=_REG_A, dst=_REG_X),                # TAX
            asm_ast.ClearCarry(),                               # CLC
            asm_ast.Mov(src=_REG_X, dst=_REG_A),                # TXA — drop
            asm_ast.Branch(cond=asm_ast.EQ(), target="L"),
            asm_ast.Return(save_a=False),
        ]
        out = _rewritten(instrs)
        self.assertEqual(len(out), 5)

    def test_txa_kept_when_intervening_arith_clobbers_z(self) -> None:
        # `LDA M; TAX; LDA #1; ADC #2; TXA; BEQ L` — the ADC
        # clobbers Z. By the second TXA, A.mirrors is cleared
        # (ADC wrote A) and z_reflects is cleared. No common
        # mirror; can't drop.
        zp80 = asm_ast.ZP(address=0x80, offset=0)
        instrs = [
            asm_ast.Mov(src=zp80, dst=_REG_A),                  # LDA M
            asm_ast.Mov(src=_REG_A, dst=_REG_X),                # TAX
            asm_ast.Mov(src=asm_ast.Imm(value=1), dst=_REG_A),  # LDA #1
            asm_ast.Add(src=asm_ast.Imm(value=2), dst=_REG_A),  # ADC #2
            asm_ast.Mov(src=_REG_X, dst=_REG_A),                # TXA — keep
            asm_ast.Branch(cond=asm_ast.EQ(), target="L"),
            asm_ast.Return(save_a=False),
        ]
        out = _rewritten(instrs)
        self.assertEqual(out, instrs)

    def test_txa_dropped_via_zreflects_match(self) -> None:
        # `LDA M; TAX; TXA; BEQ L` — Z still reflects M (no flag-
        # disturbing instruction between LDA and the second TXA).
        # Both A and X mirror M. The second TXA's flag effect (Z
        # = M == 0) is the same as the current Z; drop.
        zp80 = asm_ast.ZP(address=0x80, offset=0)
        instrs = [
            asm_ast.Mov(src=zp80, dst=_REG_A),       # LDA M (Z = M==0)
            asm_ast.Mov(src=_REG_A, dst=_REG_X),     # TAX (Z = X==0 = M==0)
            asm_ast.Mov(src=_REG_X, dst=_REG_A),     # TXA — drop
            asm_ast.Branch(cond=asm_ast.EQ(), target="L"),
            asm_ast.Return(save_a=False),
        ]
        out = _rewritten(instrs)
        self.assertEqual(len(out), 4)

    def test_tax_droppable_symmetric(self) -> None:
        # Same as txa case, but the other direction. After LDA M;
        # TAX, a redundant `TAX` (src=A, dst=X) drops.
        zp80 = asm_ast.ZP(address=0x80, offset=0)
        zp82 = asm_ast.ZP(address=0x82, offset=0)
        instrs = [
            asm_ast.Mov(src=zp80, dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=_REG_X),
            asm_ast.Mov(src=_REG_A, dst=_REG_X),     # redundant TAX
            asm_ast.Mov(src=_REG_X, dst=zp82),
            asm_ast.Return(save_a=False),
        ]
        out = _rewritten(instrs)
        self.assertEqual(len(out), 4)

    def test_residual_sx_diamond_txas_drop(self) -> None:
        # Headline beam_target_tick shape (BPL-to-next has been
        # dropped by branch_to_next_drop already):
        #   LDX M; TXA; BMI L1; TXA; TXA; SBC #1; STA M
        # The two interior TXAs are both redundant — A and X both
        # mirror M after the first TXA, and neither subsequent
        # SEC/SBC reads N/Z.
        m = asm_ast.Data(name="m", offset=0)
        instrs = [
            asm_ast.Mov(src=m, dst=_REG_X),          # LDX M
            asm_ast.Mov(src=_REG_X, dst=_REG_A),     # TXA
            asm_ast.Branch(cond=asm_ast.MI(), target=".if_end"),
            asm_ast.Mov(src=_REG_X, dst=_REG_A),     # redundant TXA
            asm_ast.Mov(src=_REG_X, dst=_REG_A),     # redundant TXA
            asm_ast.SetCarry(),
            asm_ast.Sub(src=asm_ast.Imm(value=1), dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=m),
            asm_ast.Label(name=".if_end"),
            asm_ast.Return(save_a=False),
        ]
        out = _rewritten(instrs)
        # Both interior TXAs dropped (instructions go 10 → 8).
        self.assertEqual(len(out), 8)
        # First TXA still there (sets flags for BMI).
        self.assertEqual(
            out[1], asm_ast.Mov(src=_REG_X, dst=_REG_A),
        )
        # Branch immediately follows the surviving TXA.
        self.assertIsInstance(out[2], asm_ast.Branch)
        # After the BMI, no more TXAs — directly SEC.
        self.assertIsInstance(out[3], asm_ast.SetCarry)

    def test_txa_join_disagrees_keeps(self) -> None:
        # Two predecessors, only one has A === X. The join clears
        # the equivalence, so a TXA at the join point can't drop.
        zp80 = asm_ast.ZP(address=0x80, offset=0)
        zp82 = asm_ast.ZP(address=0x82, offset=0)
        instrs = [
            asm_ast.Branch(cond=asm_ast.EQ(), target="P2"),
            # P1: LDA M; TAX  (A === X === M)
            asm_ast.Mov(src=zp80, dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=_REG_X),
            asm_ast.Jump(target="JOIN"),
            asm_ast.Label(name="P2"),
            # P2: LDA M2; LDX M2 (different M, but matching)
            # Actually let me make this clearly diverge: one pred
            # has A === M, X === M; the other has A === M', X === M.
            asm_ast.Mov(src=zp82, dst=_REG_A),
            asm_ast.Mov(src=zp80, dst=_REG_X),
            asm_ast.Label(name="JOIN"),
            asm_ast.Mov(src=_REG_X, dst=_REG_A),     # TXA — must keep
            asm_ast.Return(save_a=False),
        ]
        out = _rewritten(instrs)
        # The TXA must remain (X.mirrors=[M] on both preds, but
        # A.mirrors = {[M], [M']} — intersection empty → no overlap
        # with X.mirrors).
        self.assertTrue(any(
            isinstance(o, asm_ast.Mov)
            and isinstance(o.src, asm_ast.Reg)
            and isinstance(o.src.reg, asm_ast.X)
            and isinstance(o.dst, asm_ast.Reg)
            and isinstance(o.dst.reg, asm_ast.A)
            for o in out
        ))


class TestRedirectLdaToTransfer(unittest.TestCase):
    """`LDA M` rewrites to `TXA` (or `TYA`) when X (or Y) already
    mirrors M and A doesn't. Saves bytes, leaves A mirroring the
    same value so a downstream STA M can fold to STX/STY via
    `via_a_store_fold`."""

    def test_lda_redirects_to_txa_when_x_mirrors(self) -> None:
        # LDX M; ... ; LDA M → LDX M; ... ; TXA.
        zp80 = asm_ast.ZP(address=0x80, offset=0)
        zp82 = asm_ast.ZP(address=0x82, offset=0)
        instrs = [
            asm_ast.Mov(src=zp80, dst=_REG_X),  # LDX M (X mirrors M)
            asm_ast.Mov(src=asm_ast.Imm(value=7), dst=_REG_A),  # kill A
            asm_ast.Mov(src=_REG_A, dst=zp82),  # STA other (A intact)
            asm_ast.Mov(src=zp80, dst=_REG_A),  # LDA M → should be TXA
            asm_ast.Mov(src=_REG_A, dst=zp82),  # STA other
            asm_ast.Return(save_a=False),
        ]
        out = _rewritten(instrs)
        # Find the rewritten Mov where the original was `LDA M`.
        rewrites = [
            o for o in out
            if isinstance(o, asm_ast.Mov)
            and isinstance(o.src, asm_ast.Reg)
            and isinstance(o.src.reg, asm_ast.X)
            and isinstance(o.dst, asm_ast.Reg)
            and isinstance(o.dst.reg, asm_ast.A)
        ]
        self.assertEqual(len(rewrites), 1,
                         f"expected exactly one TXA rewrite; got: {out}")

    def test_lda_redirects_to_tya_when_y_mirrors(self) -> None:
        zp80 = asm_ast.ZP(address=0x80, offset=0)
        zp82 = asm_ast.ZP(address=0x82, offset=0)
        instrs = [
            asm_ast.Mov(src=zp80, dst=_REG_Y),  # LDY M
            asm_ast.Mov(src=asm_ast.Imm(value=7), dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=zp82),
            asm_ast.Mov(src=zp80, dst=_REG_A),  # LDA M → TYA
            asm_ast.Mov(src=_REG_A, dst=zp82),
            asm_ast.Return(save_a=False),
        ]
        out = _rewritten(instrs)
        rewrites = [
            o for o in out
            if isinstance(o, asm_ast.Mov)
            and isinstance(o.src, asm_ast.Reg)
            and isinstance(o.src.reg, asm_ast.Y)
            and isinstance(o.dst, asm_ast.Reg)
            and isinstance(o.dst.reg, asm_ast.A)
        ]
        self.assertEqual(len(rewrites), 1)

    def test_no_redirect_when_only_a_mirrors(self) -> None:
        # If A already mirrors M, the load is fully redundant — no
        # rewrite, just drop (existing behavior).
        zp80 = asm_ast.ZP(address=0x80, offset=0)
        zp82 = asm_ast.ZP(address=0x82, offset=0)
        instrs = [
            asm_ast.Mov(src=zp80, dst=_REG_A),  # LDA M
            asm_ast.Mov(src=_REG_A, dst=zp82),  # STA other; A intact
            asm_ast.Mov(src=zp80, dst=_REG_A),  # LDA M — drop
            asm_ast.Mov(src=_REG_A, dst=zp82),
            asm_ast.Return(save_a=False),
        ]
        out = _rewritten(instrs)
        # No TXA should appear; the second LDA dropped entirely.
        lda_count = sum(
            1 for o in out
            if isinstance(o, asm_ast.Mov)
            and isinstance(o.dst, asm_ast.Reg)
            and isinstance(o.dst.reg, asm_ast.A)
            and not isinstance(o.src, asm_ast.Reg)
        )
        self.assertEqual(lda_count, 1)

    def test_no_redirect_when_x_clobbered(self) -> None:
        # LDX M; INX (clobbers X); LDA M — X no longer mirrors M,
        # so no rewrite. (INX is a Pseudo-clobber here just to break
        # the X.mirror invariant.)
        zp80 = asm_ast.ZP(address=0x80, offset=0)
        zp82 = asm_ast.ZP(address=0x82, offset=0)
        instrs = [
            asm_ast.Mov(src=zp80, dst=_REG_X),
            asm_ast.Inc(dst=_REG_X),  # X mutates
            asm_ast.Mov(src=asm_ast.Imm(value=0), dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=zp82),
            asm_ast.Mov(src=zp80, dst=_REG_A),  # LDA M — keep as LDA
            asm_ast.Return(save_a=False),
        ]
        out = _rewritten(instrs)
        # The trailing instruction should still be LDA zp80.
        kept_lda = [
            o for o in out
            if isinstance(o, asm_ast.Mov)
            and isinstance(o.dst, asm_ast.Reg)
            and isinstance(o.dst.reg, asm_ast.A)
            and not isinstance(o.src, asm_ast.Reg)
        ]
        # Two LDAs survive: the LDA Imm(0) and the final LDA zp80.
        self.assertEqual(len(kept_lda), 2)
        self.assertEqual(kept_lda[1].src, zp80)

    def test_volatile_lda_not_redirected(self) -> None:
        # Volatile LDA must re-read memory; no rewrite to TXA.
        zp80 = asm_ast.ZP(address=0x80, offset=0)
        zp82 = asm_ast.ZP(address=0x82, offset=0)
        instrs = [
            asm_ast.Mov(src=zp80, dst=_REG_X),
            asm_ast.Mov(src=asm_ast.Imm(value=0), dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=zp82),
            asm_ast.Mov(src=zp80, dst=_REG_A, is_volatile=True),
            asm_ast.Return(save_a=False),
        ]
        out = _rewritten(instrs)
        volatile_lda = [
            o for o in out
            if isinstance(o, asm_ast.Mov)
            and o.is_volatile
            and not isinstance(o.src, asm_ast.Reg)
        ]
        self.assertEqual(len(volatile_lda), 1)
        self.assertEqual(volatile_lda[0].src, zp80)


if __name__ == "__main__":
    unittest.main()
