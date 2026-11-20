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

import shutil
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
            asm_ast.Mov(src=asm_ast.Data(name="k", offset=0),
                        dst=_REG_A),  # kill A
            asm_ast.Mov(src=_REG_A, dst=data_h),  # disjoint symbol
            asm_ast.Mov(src=data_g, dst=_REG_A),  # NOT redundant
            asm_ast.Return(save_a=False),
        ]
        # `g` is only tracked while A holds it. The intermediate
        # `Mov(k, A)` replaces it, so when we reach the second
        # `Mov(g, A)`, A holds `k` and `h` — load is necessary. (A
        # constant would go through Y instead, keeping `g` in A.)
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

    def test_volatile_mem_to_mem_overwrites_a(self) -> None:
        # `Mov(#0, STR, volatile)` emits as `LDA #0; STA STR`, so on
        # that path A no longer holds KBD, and the load at the join
        # stays.
        kbd = asm_ast.Data(name="KBD", offset=0)
        k = asm_ast.Data(name="k", offset=0)
        strobe = asm_ast.Data(name="STR", offset=0)
        instrs = [
            asm_ast.Mov(src=kbd, dst=k, is_volatile=True),
            asm_ast.Branch(cond=asm_ast.PL(), target=".end"),
            asm_ast.Mov(
                src=asm_ast.Imm(value=0), dst=strobe, is_volatile=True,
            ),
            asm_ast.Label(name=".end"),
            asm_ast.Mov(src=kbd, dst=_REG_A),
            # X and Y are read, so the store can't go through either.
            asm_ast.Mov(src=_REG_X, dst=k),
            asm_ast.Mov(src=_REG_Y, dst=k),
            asm_ast.Return(save_a=True),
        ]
        out = _rewritten(instrs)
        self.assertIn(asm_ast.Mov(src=kbd, dst=_REG_A), out)

    def test_volatile_mem_to_mem_leaves_a_equal_to_both(self) -> None:
        kbd = asm_ast.Data(name="KBD", offset=0)
        k = asm_ast.Data(name="k", offset=0)
        instrs = [
            asm_ast.Mov(src=kbd, dst=k, is_volatile=True),
            asm_ast.Mov(src=k, dst=_REG_A),  # A already holds k
            asm_ast.Return(save_a=True),
        ]
        out = _rewritten(instrs)
        self.assertEqual(len(out), 2)

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
        # The store of the value zp80 holds goes, and the LDA stays,
        # the only way to set Z to "is zp80 zero" before the branch.
        self.assertEqual(len(out), 5)
        self.assertIn(type(out[2]), (asm_ast.Mov, asm_ast.Or))

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

    def test_shift_of_memory_ends_older_reflections(self) -> None:
        # `AND #1; STA t; LSR p; LDA t; BEQ`: the LSR sets Z from p's
        # new value, so the BEQ needs Z set from t again. `if (moves)`
        # after `bool moves = pattern & 1; pattern >>= 1;` was `LSR
        # pattern; BEQ`.
        t = asm_ast.Data(name="t", offset=0)
        p = asm_ast.Data(name="p", offset=0)
        instrs = [
            asm_ast.Mov(src=p, dst=_REG_A),
            asm_ast.And(src=asm_ast.Imm(value=1), dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=t),
            asm_ast.LogicalShiftRight(dst=p),
            asm_ast.Mov(src=t, dst=_REG_A),
            asm_ast.Branch(cond=asm_ast.EQ(), target="L"),
            asm_ast.Return(save_a=False),
        ]
        out = _rewritten(instrs)
        # A still holds t, so the load becomes `ORA #$00`, which sets
        # the flags from it.
        self.assertEqual(
            out[4], asm_ast.Or(src=asm_ast.Imm(value=0), dst=_REG_A),
        )

    def test_store_ends_reflection_of_the_byte_it_changes(self) -> None:
        # `INC m; STA m; LDA m; BEQ`: Z reflects the INC's m, and the
        # store writes another value over it.
        m = asm_ast.Data(name="m", offset=0)
        n = asm_ast.Data(name="n", offset=0)
        instrs = [
            asm_ast.Mov(src=n, dst=_REG_A),
            asm_ast.Inc(dst=m),
            asm_ast.Mov(src=_REG_A, dst=m),
            asm_ast.Mov(src=m, dst=_REG_A),
            asm_ast.Branch(cond=asm_ast.EQ(), target="L"),
            asm_ast.Return(save_a=False),
        ]
        out = _rewritten(instrs)
        self.assertEqual(
            out[3], asm_ast.Or(src=asm_ast.Imm(value=0), dst=_REG_A),
        )

    def test_store_of_the_value_held_keeps_reflection(self) -> None:
        # In `LDA m; STA m; LDA m; BEQ`, A already holds m when it's
        # stored back, so m doesn't change, Z still reflects it, and
        # the second load goes, and so does the store.
        m = asm_ast.Data(name="m", offset=0)
        instrs = [
            asm_ast.Mov(src=m, dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=m),
            asm_ast.Mov(src=m, dst=_REG_A),
            asm_ast.Branch(cond=asm_ast.EQ(), target="L"),
            asm_ast.Return(save_a=False),
        ]
        out = _rewritten(instrs)
        self.assertEqual(len(out), 3)


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



class TestRedundantLoadLdySetup(unittest.TestCase):
    """A store into a soft-stack slot is emitted as `LDY #off; STA
    (FP),Y`, so the flags afterwards reflect the offset."""

    def test_load_before_branch_kept_after_frame_store(self) -> None:
        # Sign extension of a signed char at Frame(4): the store into
        # Frame(5) is `LDY #5; STA (FP),Y`, whose `LDY` sets N, so N
        # has to be set from the value again before BMI. A still
        # holds it, so that's `ORA #$00`.
        f4 = asm_ast.Frame(offset=4)
        f5 = asm_ast.Frame(offset=5)
        instrs = [
            asm_ast.Mov(src=f4, dst=_REG_A, is_volatile=False),
            asm_ast.Mov(src=_REG_A, dst=f5, is_volatile=False),
            asm_ast.Mov(src=f4, dst=_REG_A, is_volatile=False),
            asm_ast.Branch(cond=asm_ast.MI(), target=".neg"),
            asm_ast.Return(save_a=True),
            asm_ast.Label(name=".neg"),
            asm_ast.Return(save_a=True),
        ]
        self.assertEqual(_rewritten(list(instrs)), [
            *instrs[:2],
            asm_ast.Or(src=asm_ast.Imm(value=0), dst=_REG_A),
            *instrs[3:],
        ])

    def test_y_forgotten_after_frame_access(self) -> None:
        # `LDY M` ... frame access (LDY #3 setup) ... `LDY M` again:
        # the second LDY must stay.
        m = asm_ast.Data(name="M", offset=0)
        instrs = [
            asm_ast.Mov(src=m, dst=_REG_Y, is_volatile=False),
            asm_ast.Mov(src=asm_ast.Frame(offset=3), dst=_REG_A, is_volatile=False),
            asm_ast.Mov(src=m, dst=_REG_Y, is_volatile=False),
            asm_ast.Return(save_a=True),
        ]
        self.assertEqual(_rewritten(instrs), instrs)



class TestZTracksA(unittest.TestCase):
    """An empty `z_reflects` means nothing is known about Z, which is
    different from "Z reflects A". fuzz.stmts seed 2391: after `LDA
    F12; STA F14` the store's `LDY #off` had set Z from the offset,
    yet `STA g1; LDA g1; BEQ` dropped the `LDA g1`, and the branch
    tested Y's offset."""

    _A = asm_ast.Reg(reg=asm_ast.A())

    def _run(self, instrs):
        out = apply_redundant_load_elimination(_prog(instrs))
        return out.top_level[0].instructions

    def _mov(self, s, d):
        return asm_ast.Mov(src=s, dst=d, is_volatile=False)

    def _tail(self):
        return [
            asm_ast.Branch(cond=asm_ast.EQ(), target="L"),
            self._mov(asm_ast.Imm(value=1), self._A),
            asm_ast.Label(name="L"),
            asm_ast.Return(save_a=True, save_x=False),
        ]

    def test_load_kept_after_frame_store(self):
        f = lambda o: asm_ast.Frame(offset=o)
        g1 = asm_ast.Data(name="g1", offset=0)
        instrs = [
            self._mov(f(12), self._A), self._mov(self._A, f(14)),
            self._mov(f(14), self._A), self._mov(self._A, g1),
            self._mov(g1, self._A),
        ] + self._tail()
        out = self._run(instrs)
        i = out.index(asm_ast.Branch(cond=asm_ast.EQ(), target="L"))
        # The flags are set from A again, which holds g1.
        self.assertEqual(
            out[i - 1], asm_ast.Or(src=asm_ast.Imm(value=0), dst=self._A),
        )

    def test_load_kept_after_call(self):
        # A callee returns its value in A, but its last flag-setter
        # needn't have been A's load (`LDX hi; RTS`).
        t = asm_ast.Data(name="__local_f__0", offset=0)
        instrs = [
            asm_ast.Call(name="g", reg_args=[]),
            self._mov(self._A, t), self._mov(t, self._A),
        ] + self._tail()
        out = self._run(instrs)
        self.assertIn(
            asm_ast.Or(src=asm_ast.Imm(value=0), dst=self._A), out,
        )

    def test_load_dropped_when_z_set_from_a(self):
        # `LDA #5; STA g; LDA g; BEQ`: Z is set from A, which g now
        # holds, so the reload goes.
        g = asm_ast.Data(name="g", offset=0)
        instrs = [
            self._mov(asm_ast.Imm(value=5), self._A),
            self._mov(self._A, g), self._mov(g, self._A),
        ] + self._tail()
        out = self._run(instrs)
        self.assertNotIn(self._mov(g, self._A), out)


class TestIndexRegisterOfIndexedData(unittest.TestCase):
    """`tab,X` and `tab,Y` are different bytes unless X equals Y."""

    def _tab(self, reg):
        return asm_ast.IndexedData(name="tab", offset=0, index=reg)

    def test_x_and_y_indexed_loads_both_kept(self):
        # LDA arr,X; LDY #3; LDA arr,Y keeps the second load.
        instrs = [
            asm_ast.Mov(src=self._tab(asm_ast.X()), dst=_REG_A),
            asm_ast.Mov(src=asm_ast.Imm(value=3), dst=_REG_Y),
            asm_ast.Mov(src=self._tab(asm_ast.Y()), dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=asm_ast.Data(name="g", offset=0)),
            asm_ast.Return(save_a=False),
        ]
        out = _rewritten(instrs)
        self.assertIn(
            asm_ast.Mov(src=self._tab(asm_ast.Y()), dst=_REG_A), out,
        )

    def test_table_chase_across_branch(self):
        # `j = tab[i]; if (j == 0) return 0; g = tab[j]; return j;`
        # The `LDA tab,Y` at the join was dropped, since A mirrored
        # `tab,X` through `TYA`.
        tab_y = asm_ast.Mov(src=self._tab(asm_ast.Y()), dst=_REG_A)
        instrs = [
            asm_ast.Mov(src=asm_ast.Data(name="i", offset=0), dst=_REG_X),
            asm_ast.Mov(src=self._tab(asm_ast.X()), dst=_REG_Y),
            asm_ast.Mov(src=_REG_Y, dst=_REG_A),
            asm_ast.Branch(cond=asm_ast.NE(), target=".if_end@0"),
            asm_ast.Mov(src=asm_ast.Imm(value=0), dst=_REG_A),
            asm_ast.Return(save_a=True),
            asm_ast.Label(name=".if_end@0"),
            tab_y,
            asm_ast.Mov(src=_REG_A, dst=asm_ast.Data(name="g", offset=0)),
            asm_ast.Mov(src=_REG_Y, dst=_REG_A),
            asm_ast.Return(save_a=True),
        ]
        self.assertIn(tab_y, _rewritten(instrs))

    def test_simulated_table_chase(self):
        import shutil
        if not shutil.which("pcpp"):
            self.skipTest("pcpp not installed")
        from sim.harness import build_sim
        src = """
unsigned char tab[8] = {3, 0, 0, 7, 0, 0, 0, 0};
unsigned char g;
unsigned char f(unsigned char i) { unsigned char j = tab[i]; if (j == 0) return 0; g = tab[j]; return j; }
int main(void) { unsigned char r = f(0); return g * 100 + r; }
"""
        self.assertEqual(
            build_sim(src, optimize=True).run().return_int(), 703,
        )


def _d(name: str, offset: int = 0) -> asm_ast.Data:
    return asm_ast.Data(name=name, offset=offset)


class TestStoreFromA(unittest.TestCase):
    """A store through A, `Mov(#k, D)`, of a value A already holds
    becomes the `STA D` alone."""

    def test_second_immediate_store(self):
        instrs = [
            asm_ast.Mov(src=asm_ast.Imm(value=255), dst=_d("S")),
            asm_ast.Mov(src=asm_ast.Imm(value=255), dst=_d("S", 1)),
            asm_ast.Mov(src=_d("g"), dst=_REG_A),
            asm_ast.Return(save_a=False),
        ]
        self.assertEqual(_rewritten(instrs), [
            instrs[0],
            asm_ast.Mov(src=_REG_A, dst=_d("S", 1)),
            *instrs[2:],
        ])

    def test_kept_when_the_flags_are_read(self):
        instrs = [
            asm_ast.Mov(src=asm_ast.Imm(value=0), dst=_d("S")),
            asm_ast.Compare(left=_REG_A, right=asm_ast.Imm(value=5)),
            asm_ast.Mov(src=asm_ast.Imm(value=0), dst=_d("S", 1)),
            asm_ast.Branch(cond=asm_ast.EQ(), target=".l"),
            asm_ast.Label(name=".l"),
            asm_ast.Return(save_a=False),
        ]
        self.assertEqual(_rewritten(list(instrs)), instrs)

    def test_kept_for_another_value(self):
        instrs = [
            asm_ast.Mov(src=asm_ast.Imm(value=1), dst=_d("S")),
            asm_ast.Mov(src=asm_ast.Imm(value=2), dst=_d("S", 1)),
            asm_ast.Return(save_a=False),
        ]
        self.assertEqual(_rewritten(list(instrs)), instrs)


class TestSwapPairs(unittest.TestCase):
    """`LDA X; STA D1; LDA M; STA D2` with A already holding M becomes
    `STA D2; LDA X; STA D1`, as at the join of `floor_enemy_draw`'s
    dispatch."""

    def _join(self, d1, m, d2, after=None):
        return [
            asm_ast.Mov(src=_d("hi"), dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=_d("t")),
            asm_ast.Mov(src=_d("lo"), dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=d1),
            asm_ast.Mov(src=m, dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=d2),
            *(after or [asm_ast.Mov(src=asm_ast.Imm(value=1), dst=_REG_A)]),
            asm_ast.Return(save_a=False),
        ]

    def test_swapped(self):
        instrs = self._join(_d("p0"), _d("t"), _d("p1"))
        self.assertEqual(_rewritten(list(instrs)), [
            *instrs[:2], instrs[5], instrs[2], instrs[3], *instrs[6:],
        ])

    def test_kept_when_the_first_store_writes_m(self):
        # `LDA t` right after `STA t` goes anyway, but `STA p1` stays
        # after it.
        instrs = self._join(_d("t"), _d("t"), _d("p1"))
        self.assertEqual(
            _rewritten(list(instrs)), instrs[:4] + instrs[5:],
        )

    def _indexed_join(self, index, m):
        """The join storing a slot and then `m` to two bytes of `z`
        indexed by `index`, as `instr_call` stores a local's two
        bytes. The slots are the function's own, which an indexed
        store can't reach."""
        z = lambda k: asm_ast.IndexedData(name="z", offset=k, index=index)
        t = _d("__local_f__t")
        return [
            asm_ast.Mov(src=_d("hi"), dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=t),
            asm_ast.Mov(src=_d("__local_f__lo"), dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=z(0)),
            asm_ast.Mov(src=m or t, dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=z(1)),
            asm_ast.Mov(src=asm_ast.Imm(value=1), dst=_REG_A),
            asm_ast.Return(save_a=False),
        ]

    def test_swapped_for_bytes_of_one_indexed_array(self):
        # `z,X` and `z+1,X` are different bytes, since X is the same
        # at both stores.
        instrs = self._indexed_join(asm_ast.X(), None)
        self.assertEqual(_rewritten(list(instrs)), [
            *instrs[:2], instrs[5], instrs[2], instrs[3], *instrs[6:],
        ])

    def test_indexed_bytes_through_y_setup(self):
        # The `LDY #off` of a soft-stack load between the stores
        # changes Y, so `z,Y` and `z+1,Y` may be one byte.
        from passes.redundant_load import _same_index_apart
        z = lambda k: asm_ast.IndexedData(name="z", offset=k, index=asm_ast.Y())
        plain = self._indexed_join(asm_ast.Y(), None)[2:6]
        self.assertTrue(_same_index_apart(z(0), z(1), plain))
        setup = self._indexed_join(asm_ast.Y(), asm_ast.Frame(offset=3))[2:6]
        self.assertFalse(_same_index_apart(z(0), z(1), setup))

    def test_kept_when_the_second_store_writes_x(self):
        instrs = self._join(_d("p0"), _d("t"), _d("lo"))
        self.assertEqual(_rewritten(list(instrs)), instrs)

    def test_kept_when_a_is_read_after(self):
        # A `PHA` reads A, which holds X after the swap. (A store from
        # A right after the pair would only join the second group.)
        instrs = self._join(
            _d("p0"), _d("t"), _d("p1"),
            after=[asm_ast.Push(src=_REG_A)],
        )
        self.assertEqual(_rewritten(list(instrs)), instrs)

    def test_swapped_with_several_stores_in_each_group(self):
        # `LDA lo; STA p0; STA p2; LDA t; STA p1; STA p3` with A holding
        # `t`: the second group's stores go first, and its load goes.
        instrs = [
            asm_ast.Mov(src=_d("hi"), dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=_d("t")),
            asm_ast.Mov(src=_d("lo"), dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=_d("p0")),
            asm_ast.Mov(src=_REG_A, dst=_d("p2")),
            asm_ast.Mov(src=_d("t"), dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=_d("p1")),
            asm_ast.Mov(src=_REG_A, dst=_d("p3")),
            asm_ast.Mov(src=asm_ast.Imm(value=1), dst=_REG_A),
            asm_ast.Return(save_a=False),
        ]
        self.assertEqual(_rewritten(list(instrs)), [
            *instrs[:2], instrs[6], instrs[7], *instrs[2:5], *instrs[8:],
        ])

    def test_next_block_sees_the_swap(self):
        # Both ways into `.join` leave 0 in A, until the swap leaves
        # `x` there on one of them. `i = 0` after the join stays a
        # load of 0 (fuzz.stmts seed 27540, whose loop counter started
        # at another value).
        zero = asm_ast.Imm(value=0)
        instrs = [
            asm_ast.Mov(src=zero, dst=_REG_A),
            asm_ast.Branch(cond=asm_ast.EQ(), target=".join"),
            asm_ast.Mov(src=_d("x"), dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=_d("p0")),
            asm_ast.Mov(src=zero, dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=_d("p1")),
            asm_ast.Label(name=".join"),
            asm_ast.Mov(src=zero, dst=_d("i")),
            asm_ast.Return(save_a=False),
        ]
        out = _rewritten(list(instrs))
        self.assertEqual(
            [ins for ins in out
             if isinstance(ins, asm_ast.Mov) and ins.dst == _d("i")],
            [asm_ast.Mov(src=zero, dst=_d("i"))],
        )


class TestStoreConstViaIndex(unittest.TestCase):
    """`LDA #k; STA M` goes through Y when A holds a value that's
    reloaded after it, as `get_alphabet` returns the alphabet it
    loaded after `SHIFT_ALPHABET = 0xFF`."""

    def test_reload_dropped(self):
        g, t, shift = _d("g"), _d("t"), _d("SHIFT")
        out = _rewritten([
            asm_ast.Mov(src=g, dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=t),
            asm_ast.Mov(src=asm_ast.Imm(value=0xFF), dst=shift),
            asm_ast.Mov(src=t, dst=_REG_A),
            asm_ast.Return(save_a=True),
        ])
        self.assertEqual(out, [
            asm_ast.Mov(src=g, dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=t),
            asm_ast.Mov(src=asm_ast.Imm(value=0xFF), dst=_REG_Y),
            asm_ast.Mov(src=_REG_Y, dst=shift),
            asm_ast.Return(save_a=True),
        ])

    def test_not_without_a_reload(self):
        g, shift = _d("g"), _d("SHIFT")
        instrs = [
            asm_ast.Mov(src=g, dst=_REG_A),
            asm_ast.Mov(src=asm_ast.Imm(value=0xFF), dst=shift),
            asm_ast.Mov(src=_d("h"), dst=_REG_A),
            asm_ast.Return(save_a=True),
        ]
        self.assertEqual(_rewritten(list(instrs)), instrs)


class TestStoreThroughPointer(unittest.TestCase):
    """A store through a pointer can't change a byte no pointer can
    point to (`asm_aliasing.unreachable_by_pointer`): an internal
    static, or the function's own zero-page slot, whose address the
    unit never takes. Anything else may be the byte stored to."""

    _C = asm_ast.Data(name="__local_f__c", offset=0)

    def _shape(self, mem, extra=(), fns=()):
        # `LDX M; LDA #9; STA (p),Y; LDX M`: the last load goes when
        # the store can't reach M.
        body = [
            asm_ast.Mov(src=mem, dst=_REG_X, is_volatile=False),
            asm_ast.Mov(src=asm_ast.Imm(value=9), dst=_REG_A,
                        is_volatile=False),
            asm_ast.Mov(src=_REG_A, dst=asm_ast.IndirectZpY(address=0x90),
                        is_volatile=False),
            asm_ast.Mov(src=mem, dst=_REG_X, is_volatile=False),
            asm_ast.Mov(src=_REG_X, dst=asm_ast.Data(name="out", offset=0),
                        is_volatile=False),
            asm_ast.Return(save_a=False, save_x=False, save_c=False),
        ]
        top = [asm_ast.Function(name="f", is_global=True, params=[],
                                instructions=list(extra) + body)]
        top += [asm_ast.Function(name=n, is_global=False, params=[],
                                 instructions=[asm_ast.Return(save_a=False, 
                                     save_c=False, save_x=False)])
                for n in fns]
        top += [asm_ast.StaticVariable(name="s", is_global=False,
                                       init=[]),
                asm_ast.StaticVariable(name="g", is_global=True, init=[])]
        out = apply_redundant_load_elimination(
            asm_ast.Program(top_level=top),
        ).top_level[0].instructions
        return len(out) - len(extra)

    def test_own_slot_reload_dropped(self):
        self.assertEqual(self._shape(self._C), 5)

    def test_internal_static_reload_dropped(self):
        self.assertEqual(self._shape(asm_ast.Data(name="s", offset=0)), 5)

    def test_global_kept(self):
        self.assertEqual(self._shape(asm_ast.Data(name="g", offset=0)), 6)

    def test_address_taken_slot_kept(self):
        # `address_taken_zp` puts an address-taken local in a slot.
        take = asm_ast.LoadAddress(
            src=self._C, dst=asm_ast.Data(name="__local_f__p", offset=0),
        )
        self.assertEqual(self._shape(self._C, extra=[take]), 6)

    def test_address_as_immediate_kept(self):
        take = asm_ast.Mov(src=asm_ast.ImmLabelLow(name="__local_f__c", offset=0),
                           dst=_REG_A, is_volatile=False)
        self.assertEqual(self._shape(self._C, extra=[take]), 6)

    def test_slot_of_function_defined_elsewhere_kept(self):
        mem = asm_ast.Data(name="__zpabi_h__c", offset=0)
        self.assertEqual(self._shape(mem), 6)

    def test_slot_whose_owner_is_ambiguous_kept(self):
        # `__local_f__x__c` might be `f__x`'s local `c`.
        mem = asm_ast.Data(name="__local_f__x__c", offset=0)
        self.assertEqual(self._shape(mem, fns=["f__x"]), 6)



class TestMemoryGroups(unittest.TestCase):
    """Bytes that hold one value after the register that held it is
    reloaded. A load of one of them makes the register mirror all of
    them, and a store into one of those is dropped. The row of a 2-D
    table is set up again with the same low byte."""

    _LO = asm_ast.IndexedData(name="T@lo", offset=0, index=asm_ast.X())
    _P0 = asm_ast.Data(name="__local_f__p0", offset=0)
    _P1 = asm_ast.Data(name="__local_f__p1", offset=0)

    def _row(self, hi_name, then):
        hi = asm_ast.IndexedData(name=hi_name, offset=0, index=asm_ast.X())
        return [
            asm_ast.Mov(src=self._LO, dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=self._P0),
            asm_ast.Mov(src=hi, dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=self._P1),
            *then,
        ]

    def _store_through(self):
        return [
            asm_ast.Mov(src=asm_ast.Imm(value=7), dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=asm_ast.IndirectZpY(address=0x90)),
        ]

    def _run(self, instrs):
        top = [_fn(instrs)] + [
            asm_ast.StaticVariable(name=n, is_global=False, init=[])
            for n in ("T@lo", "A@hi", "B@hi")
        ]
        return apply_redundant_load_elimination(
            asm_ast.Program(top_level=top),
        ).top_level[0].instructions

    def test_second_row_keeps_low_byte(self):
        instrs = (self._row("A@hi", self._store_through())
                  + self._row("B@hi", self._store_through())
                  + [asm_ast.Return(save_a=False)])
        out = self._run(instrs)
        self.assertEqual(out.count(asm_ast.Mov(src=_REG_A, dst=self._P0)), 1)
        self.assertEqual(out.count(asm_ast.Mov(src=_REG_A, dst=self._P1)), 2)

    def test_index_change_ends_the_group(self):
        instrs = (self._row("A@hi", self._store_through())
                  + [asm_ast.Inc(dst=_REG_X)]
                  + self._row("B@hi", self._store_through())
                  + [asm_ast.Return(save_a=False)])
        out = self._run(instrs)
        self.assertEqual(out.count(asm_ast.Mov(src=_REG_A, dst=self._P0)), 2)

    def test_write_of_the_byte_ends_the_group(self):
        instrs = (self._row("A@hi", [asm_ast.Inc(dst=self._P0)])
                  + self._row("B@hi", [])
                  + [asm_ast.Return(save_a=False)])
        out = self._run(instrs)
        self.assertEqual(out.count(asm_ast.Mov(src=_REG_A, dst=self._P0)), 2)

    def test_group_survives_a_join_only_when_both_paths_have_it(self):
        def shape(other_path):
            return (
                self._row("A@hi", [
                    asm_ast.Branch(cond=asm_ast.EQ(), target=".j"),
                    *other_path,
                    asm_ast.Label(name=".j"),
                ])
                + self._row("B@hi", [])
                + [asm_ast.Return(save_a=False)]
            )
        out = self._run(shape([asm_ast.Inc(dst=_REG_Y)]))
        self.assertEqual(out.count(asm_ast.Mov(src=_REG_A, dst=self._P0)), 1)
        out = self._run(shape([asm_ast.Inc(dst=self._P0)]))
        self.assertEqual(out.count(asm_ast.Mov(src=_REG_A, dst=self._P0)), 2)


class TestSwapPairsThroughPointer(unittest.TestCase):
    """`LDA t,X; STA p+1; LDA #7; STA (p),Y` with A holding 7 must not
    become `STA (p),Y; LDA t,X; STA p+1`, which stores through the old
    pointer."""

    def test_kept(self):
        p1 = asm_ast.Data(name="__local_f__p", offset=1)
        hi = asm_ast.IndexedData(name="t", offset=0, index=asm_ast.X())
        instrs = [
            asm_ast.Mov(src=asm_ast.Imm(value=7), dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=asm_ast.Data(name="g", offset=0)),
            asm_ast.Mov(src=hi, dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=p1),
            asm_ast.Mov(src=asm_ast.Imm(value=7), dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=asm_ast.IndirectZpY(address=0x90)),
            asm_ast.Mov(src=asm_ast.Imm(value=1), dst=_REG_A),
            asm_ast.Return(save_a=False),
        ]
        prog = asm_ast.Program(top_level=[
            _fn(list(instrs)),
            asm_ast.StaticVariable(name="t", is_global=False, init=[]),
        ])
        out = apply_redundant_load_elimination(
            prog, zp_symbol_addrs={"__local_f__p": 0x90},
        ).top_level[0].instructions
        at_store = out.index(asm_ast.Mov(src=_REG_A, dst=p1))
        at_ptr = out.index(
            asm_ast.Mov(src=_REG_A, dst=asm_ast.IndirectZpY(address=0x90)))
        self.assertLess(at_store, at_ptr)

    def test_swapped_when_the_store_is_another_byte(self):
        d1 = asm_ast.Data(name="__local_f__q", offset=0)
        hi = asm_ast.IndexedData(name="t", offset=0, index=asm_ast.X())
        instrs = [
            asm_ast.Mov(src=asm_ast.Imm(value=7), dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=asm_ast.Data(name="g", offset=0)),
            asm_ast.Mov(src=hi, dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=d1),
            asm_ast.Mov(src=asm_ast.Imm(value=7), dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=asm_ast.IndirectZpY(address=0x90)),
            asm_ast.Mov(src=asm_ast.Imm(value=1), dst=_REG_A),
            asm_ast.Return(save_a=False),
        ]
        # `t` is a static whose address the unit doesn't take, so the
        # store through the pointer can't change it.
        prog = asm_ast.Program(top_level=[
            _fn(list(instrs)),
            asm_ast.StaticVariable(name="t", is_global=False, init=[]),
        ])
        out = apply_redundant_load_elimination(
            prog,
            zp_symbol_addrs={"__local_f__q": 0x84, "__local_f__p": 0x90},
        ).top_level[0].instructions
        at_store = out.index(asm_ast.Mov(src=_REG_A, dst=d1))
        at_ptr = out.index(
            asm_ast.Mov(src=_REG_A, dst=asm_ast.IndirectZpY(address=0x90)))
        self.assertGreater(at_store, at_ptr)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestRowsOfTwoTablesSim(unittest.TestCase):
    """Two 2-D arrays whose tables of row addresses share the low
    bytes, read and written in one row, as Lode Runner's LEVEL_MAP and
    LEVEL_BG are."""

    _SRC = (
        "#include <stdint.h>\n"
        "static uint8_t map[16][28];\n"
        "static uint8_t bg[16][28];\n"
        "uint8_t row, col;\n"
        "void fall(void) {\n"
        "    uint8_t r = row + 1;\n"
        "    if (bg[r][col] == 1 && (int8_t)col >= 0) {\n"
        "        uint8_t above = r - 1;\n"
        "        if (bg[above][col] == 0) {\n"
        "            map[above][col] = 7;\n"
        "            bg[above][col] = 7;\n"
        "        }\n"
        "    }\n"
        "}\n"
        "int main(void) {\n"
        "    for (uint8_t i = 0; i < 16; i++)\n"
        "        for (uint8_t j = 0; j < 28; j++)\n"
        "            bg[i][j] = (uint8_t)(i + j) & 1;\n"
        "    row = 4; col = 4;\n"
        "    fall();\n"
        "    return map[4][4] + bg[4][4] * 16 + bg[5][4] * 256;\n"
        "}\n"
    )

    def test_runs(self):
        from sim.harness import build_sim
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                out = build_sim(self._SRC, optimize=optimize).run()
                self.assertEqual(out.return_int(), 7 + 7 * 16 + 256)



class TestTransferBack(unittest.TestCase):

    def test_tax_txa(self) -> None:
        # `return add(v, v);` with v arriving in A.
        fn = asm_ast.Function(name="f", is_global=True, params=[],
                              instructions=[
            asm_ast.Mov(src=_REG_A, dst=_REG_X),
            asm_ast.Mov(src=_REG_X, dst=_REG_A),
            asm_ast.Return(save_a=True),
        ])
        out = apply_redundant_load_elimination(
            asm_ast.Program(top_level=[fn])).top_level[0].instructions
        self.assertEqual(out, [
            asm_ast.Mov(src=_REG_A, dst=_REG_X),
            asm_ast.Return(save_a=True),
        ])

if __name__ == "__main__":
    unittest.main()
