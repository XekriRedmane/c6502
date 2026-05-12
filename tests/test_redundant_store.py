"""Tests for the asm-level redundant memory-to-memory store
elimination pass.

`apply_redundant_store_elimination` walks each function linearly,
tracking which source cell each destination cell currently holds.
Drops `Mov(src=mem, A); Mov(A, dst=mem)` pairs whose
`(src, dst)` equivalence is still in the tracking map.

Coverage:
  * Canonical repeat of an LDA/STA pair within a basic block.
  * Intervening writes that don't alias preserve tracking.
  * Intervening writes that DO alias invalidate.
  * Block boundaries reset.
  * Branch immediately after the pair preserves it (flag liveness).
  * IndexedData writes use range analysis when the base is known.
  * Push (PHA) doesn't invalidate ZP / static tracking.
  * The DPTR-stage pattern (the headline case): two
    4-instruction DPTR stagings collapse to one when nothing
    in between writes to DPTR / DPTR+1 or the source bytes.
"""
from __future__ import annotations

import unittest

import asm_ast
from passes.redundant_store import (
    apply_redundant_store_elimination,
)


_REG_A = asm_ast.Reg(reg=asm_ast.A())
_REG_X = asm_ast.Reg(reg=asm_ast.X())


def _fn(instrs: list[asm_ast.Type_instruction]) -> asm_ast.Function:
    return asm_ast.Function(
        name="f", is_global=True, params=[], instructions=instrs,
    )


def _prog(instrs: list[asm_ast.Type_instruction]) -> asm_ast.Program:
    return asm_ast.Program(top_level=[_fn(instrs)])


def _rewritten(instrs):
    return apply_redundant_store_elimination(
        _prog(instrs),
    ).top_level[0].instructions


def _zp(addr: int, off: int = 0) -> asm_ast.ZP:
    return asm_ast.ZP(address=addr, offset=off)


def _data(name: str, off: int = 0) -> asm_ast.Data:
    return asm_ast.Data(name=name, offset=off)


def _lda_sta(src, dst):
    return [
        asm_ast.Mov(src=src, dst=_REG_A),
        asm_ast.Mov(src=_REG_A, dst=dst),
    ]


class TestRedundantStoreBasic(unittest.TestCase):
    def test_immediate_repeat_pair_dropped(self) -> None:
        # LDA $80; STA DPTR; LDA $80; STA DPTR  →
        # LDA $80; STA DPTR  (second pair redundant).
        instrs = (
            _lda_sta(_zp(0x80), _data("DPTR"))
            + _lda_sta(_zp(0x80), _data("DPTR"))
            + [asm_ast.Return(save_a=False)]
        )
        out = _rewritten(instrs)
        # 2 + 1 = 3 instructions remaining.
        self.assertEqual(len(out), 3)
        self.assertEqual(out[0].src, _zp(0x80))
        self.assertEqual(out[1].dst, _data("DPTR"))
        self.assertIsInstance(out[2], asm_ast.Return)

    def test_different_src_keeps_both(self) -> None:
        instrs = (
            _lda_sta(_zp(0x80), _data("DPTR"))
            + _lda_sta(_zp(0x81), _data("DPTR"))
            + [asm_ast.Return(save_a=False)]
        )
        out = _rewritten(instrs)
        self.assertEqual(len(out), 5)

    def test_different_dst_keeps_both(self) -> None:
        instrs = (
            _lda_sta(_zp(0x80), _data("DPTR"))
            + _lda_sta(_zp(0x80), _data("DPTR", 1))
            + [asm_ast.Return(save_a=False)]
        )
        out = _rewritten(instrs)
        self.assertEqual(len(out), 5)


class TestRedundantStoreInvalidation(unittest.TestCase):
    def test_disjoint_zp_store_preserves_tracking(self) -> None:
        # Intervening STA $84 doesn't touch $80 or DPTR ($24).
        instrs = (
            _lda_sta(_zp(0x80), _data("DPTR"))
            + [
                asm_ast.Mov(src=_REG_A, dst=_zp(0x84)),
            ]
            + _lda_sta(_zp(0x80), _data("DPTR"))
            + [asm_ast.Return(save_a=False)]
        )
        out = _rewritten(instrs)
        # The intervening STA $84 stays; the second LDA/STA pair
        # is dropped.
        self.assertEqual(len(out), 4)
        self.assertIsInstance(out[2], asm_ast.Mov)
        self.assertEqual(out[2].dst, _zp(0x84))

    def test_write_to_src_invalidates(self) -> None:
        # Writing to $80 between the two pairs changes the source's
        # value, so the second pair is NOT redundant.
        instrs = (
            _lda_sta(_zp(0x80), _data("DPTR"))
            + [
                asm_ast.Mov(src=asm_ast.Imm(value=0xAA), dst=_REG_A),
                asm_ast.Mov(src=_REG_A, dst=_zp(0x80)),
            ]
            + _lda_sta(_zp(0x80), _data("DPTR"))
            + [asm_ast.Return(save_a=False)]
        )
        out = _rewritten(instrs)
        self.assertEqual(len(out), 7)

    def test_write_to_dst_invalidates(self) -> None:
        # Writing to DPTR between the two pairs changes the dst's
        # value, so the second pair is NOT redundant.
        instrs = (
            _lda_sta(_zp(0x80), _data("DPTR"))
            + [
                asm_ast.Mov(src=asm_ast.Imm(value=0xBB), dst=_REG_A),
                asm_ast.Mov(src=_REG_A, dst=_data("DPTR")),
            ]
            + _lda_sta(_zp(0x80), _data("DPTR"))
            + [asm_ast.Return(save_a=False)]
        )
        out = _rewritten(instrs)
        self.assertEqual(len(out), 7)

    def test_indexed_data_disjoint_preserves(self) -> None:
        # STA $20A8,X writes range [$20A8, $21A7]. Doesn't include
        # $24 (DPTR) or $80 (source). Tracking preserved.
        instrs = (
            _lda_sta(_zp(0x80), _data("DPTR"))
            + [
                asm_ast.Mov(
                    src=_REG_A,
                    dst=asm_ast.IndexedData(
                        name="", offset=0x20A8, index=asm_ast.X(),
                    ),
                ),
            ]
            + _lda_sta(_zp(0x80), _data("DPTR"))
            + [asm_ast.Return(save_a=False)]
        )
        out = _rewritten(instrs)
        # 2 (1st pair) + 1 (STA $20A8,X) + 0 (2nd pair dropped) + 1
        # (Return) = 4.
        self.assertEqual(len(out), 4)

    def test_indexed_data_overlap_invalidates(self) -> None:
        # STA $20,X writes range [$20, $11F]. Includes $24 (DPTR)
        # and $80 (source) and $81. Tracking is invalidated.
        instrs = (
            _lda_sta(_zp(0x80), _data("DPTR"))
            + [
                asm_ast.Mov(
                    src=_REG_A,
                    dst=asm_ast.IndexedData(
                        name="", offset=0x20, index=asm_ast.X(),
                    ),
                ),
            ]
            + _lda_sta(_zp(0x80), _data("DPTR"))
            + [asm_ast.Return(save_a=False)]
        )
        out = _rewritten(instrs)
        # Tracking invalidated; both pairs kept.
        self.assertEqual(len(out), 6)


class TestRedundantStoreBoundaries(unittest.TestCase):
    def test_label_resets_state(self) -> None:
        instrs = (
            _lda_sta(_zp(0x80), _data("DPTR"))
            + [asm_ast.Label(name=".mid")]
            + _lda_sta(_zp(0x80), _data("DPTR"))
            + [asm_ast.Return(save_a=False)]
        )
        out = _rewritten(instrs)
        # Label clears tracking; both pairs kept.
        self.assertEqual(len(out), 6)

    def test_branch_in_middle_resets_state(self) -> None:
        instrs = (
            _lda_sta(_zp(0x80), _data("DPTR"))
            + [asm_ast.Branch(
                cond=asm_ast.NE(), target=".somewhere",
            )]
            + _lda_sta(_zp(0x80), _data("DPTR"))
            + [asm_ast.Return(save_a=False)]
        )
        out = _rewritten(instrs)
        self.assertEqual(len(out), 6)

    def test_call_resets_state(self) -> None:
        instrs = (
            _lda_sta(_zp(0x80), _data("DPTR"))
            + [asm_ast.Call(name="foo")]
            + _lda_sta(_zp(0x80), _data("DPTR"))
            + [asm_ast.Return(save_a=False)]
        )
        out = _rewritten(instrs)
        self.assertEqual(len(out), 6)


class TestRedundantStoreFlagLiveness(unittest.TestCase):
    def test_branch_immediately_after_keeps_pair(self) -> None:
        # The LDA in the second pair sets N/Z, which the Branch
        # would observe. Don't drop.
        instrs = (
            _lda_sta(_zp(0x80), _data("DPTR"))
            + _lda_sta(_zp(0x80), _data("DPTR"))
            + [asm_ast.Branch(cond=asm_ast.NE(), target=".x")]
        )
        out = _rewritten(instrs)
        self.assertEqual(len(out), 5)

    def test_arithmetic_resets_flags_so_pair_drops(self) -> None:
        # The LDA's flag effect is overwritten by the Add before
        # any Branch — safe to drop the second pair.
        instrs = (
            _lda_sta(_zp(0x80), _data("DPTR"))
            + _lda_sta(_zp(0x80), _data("DPTR"))
            + [
                asm_ast.ClearCarry(),
                asm_ast.Add(src=asm_ast.Imm(value=1), dst=_REG_A),
                asm_ast.Branch(cond=asm_ast.NE(), target=".x"),
            ]
        )
        out = _rewritten(instrs)
        # 2 (1st pair) + 0 (2nd pair dropped) + 3 trailer = 5.
        self.assertEqual(len(out), 5)


class TestRedundantStoreHardwareStack(unittest.TestCase):
    def test_push_pop_does_not_invalidate(self) -> None:
        # PHA writes to page 1 ($100-$1FF), which doesn't alias
        # ZP ($00-$FF) or static data segment ($800+). Tracking
        # should survive.
        instrs = (
            _lda_sta(_zp(0x80), _data("DPTR"))
            + [
                asm_ast.Push(src=_REG_A),
                asm_ast.Pop(dst=_REG_A),
            ]
            + _lda_sta(_zp(0x80), _data("DPTR"))
            + [asm_ast.Return(save_a=False)]
        )
        out = _rewritten(instrs)
        # 2 (1st pair) + 2 (Push, Pop) + 0 (2nd pair dropped) +
        # 1 (Return) = 5.
        self.assertEqual(len(out), 5)


class TestDptrStagePattern(unittest.TestCase):
    """The headline case: two consecutive 4-instruction DPTR
    stagings collapse to one."""

    def test_dptr_double_stage_collapses(self) -> None:
        zp80 = _zp(0x80)
        zp81 = _zp(0x81)
        dptr = _data("DPTR", 0)
        dptr1 = _data("DPTR", 1)
        # First stage.
        stage1 = (
            _lda_sta(zp80, dptr) + _lda_sta(zp81, dptr1)
        )
        # Body that doesn't touch any of the four tracked cells.
        body = [
            asm_ast.Mov(src=asm_ast.IndirectY(), dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=_zp(0x84)),
            asm_ast.Mov(
                src=_REG_A,
                dst=asm_ast.IndexedData(
                    name="", offset=0x2000, index=asm_ast.X(),
                ),
            ),
            asm_ast.Inc(dst=asm_ast.Reg(reg=asm_ast.Y())),
        ]
        # Second stage (should be entirely dropped).
        stage2 = (
            _lda_sta(zp80, dptr) + _lda_sta(zp81, dptr1)
        )
        body2 = [
            asm_ast.Mov(src=asm_ast.IndirectY(), dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=_zp(0x84)),
        ]
        instrs = (
            stage1 + body + stage2 + body2
            + [asm_ast.Return(save_a=False)]
        )
        out = _rewritten(instrs)
        # 4 (stage1) + 4 (body) + 0 (stage2 dropped) + 2 (body2)
        # + 1 (Return) = 11.
        self.assertEqual(len(out), 11)
        # First 4 instructions should be the stage; verify the
        # 8 stage-instructions present at all.
        sta_dptr_count = sum(
            1 for ins in out
            if (
                isinstance(ins, asm_ast.Mov)
                and isinstance(ins.dst, asm_ast.Data)
                and ins.dst.name == "DPTR"
            )
        )
        self.assertEqual(sta_dptr_count, 2)


if __name__ == "__main__":
    unittest.main()
