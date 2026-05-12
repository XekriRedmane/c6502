"""Tests for the indirect-base copy-propagation pass.

`apply_indirect_base_prop` detects the 4-instruction DPTR-stage
shape

    LDA <ZP zp_lo> ; STA DPTR
    LDA <ZP zp_hi> ; STA DPTR+1

where `zp_lo` and `zp_hi` resolve to adjacent ZP byte addresses
`(N, N+1)`, records the equivalence `DPTR === zp_pair(N)`, and
rewrites subsequent `Indirect(off)` / `IndirectY()` operands to
`IndirectZp(N, off)` / `IndirectZpY(N)` within the equivalence
window. The window closes on writes to `{DPTR, DPTR+1, N, N+1}`
or any block boundary.

Coverage:
  * Canonical stage + Indirect access collapses.
  * Multiple subsequent indirect accesses all get rewritten.
  * A write to one of the source ZP bytes invalidates.
  * A write through DPTR (STA (DPTR),Y) invalidates (conservative —
    could alias DPTR's bytes via aliasing).
  * Disjoint stores (STA $84, STA $20A8,X) preserve the equivalence.
  * Block boundaries reset state.
  * Non-adjacent source pair doesn't match.
"""
from __future__ import annotations

import unittest

import asm_ast
from passes.indirect_base_prop import apply_indirect_base_prop


_REG_A = asm_ast.Reg(reg=asm_ast.A())
_REG_X = asm_ast.Reg(reg=asm_ast.X())


def _fn(instrs: list[asm_ast.Type_instruction]) -> asm_ast.Function:
    return asm_ast.Function(
        name="f", is_global=True, params=[], instructions=instrs,
    )


def _prog(instrs: list[asm_ast.Type_instruction]) -> asm_ast.Program:
    return asm_ast.Program(top_level=[_fn(instrs)])


def _rewritten(instrs):
    return apply_indirect_base_prop(_prog(instrs)).top_level[0].instructions


def _zp(addr: int, off: int = 0) -> asm_ast.ZP:
    return asm_ast.ZP(address=addr, offset=off)


def _dptr(off: int) -> asm_ast.Data:
    return asm_ast.Data(name="DPTR", offset=off)


def _stage_dptr_from(zp_addr: int):
    """Build the 4-instruction DPTR-stage shape that copies
    bytes at `(zp_addr, zp_addr+1)` into `(DPTR, DPTR+1)`."""
    return [
        asm_ast.Mov(src=_zp(zp_addr), dst=_REG_A),
        asm_ast.Mov(src=_REG_A, dst=_dptr(0)),
        asm_ast.Mov(src=_zp(zp_addr + 1), dst=_REG_A),
        asm_ast.Mov(src=_REG_A, dst=_dptr(1)),
    ]


class TestIndirectBaseBasic(unittest.TestCase):
    def test_indirect_y_load_after_stage_rewrites(self) -> None:
        # Stage DPTR from ($80, $81); then `LDA (DPTR),Y` becomes
        # `LDA ($80),Y` (an IndirectZpY operand).
        instrs = (
            _stage_dptr_from(0x80)
            + [
                asm_ast.Mov(src=asm_ast.IndirectY(), dst=_REG_A),
                asm_ast.Return(save_a=False),
            ]
        )
        out = _rewritten(instrs)
        # Stage stays (downstream DSE drops it once nothing reads
        # DPTR); the IndirectY operand is rewritten.
        self.assertIsInstance(out[4].src, asm_ast.IndirectZpY)
        self.assertEqual(out[4].src.address, 0x80)

    def test_indirect_offset_load_rewrites(self) -> None:
        # `LDA (DPTR),Y` with a compile-time Y (Indirect(off)) →
        # IndirectZp(addr, off).
        instrs = (
            _stage_dptr_from(0x80)
            + [
                asm_ast.Mov(
                    src=asm_ast.Indirect(offset=3), dst=_REG_A,
                ),
                asm_ast.Return(save_a=False),
            ]
        )
        out = _rewritten(instrs)
        self.assertIsInstance(out[4].src, asm_ast.IndirectZp)
        self.assertEqual(out[4].src.address, 0x80)
        self.assertEqual(out[4].src.offset, 3)

    def test_indirect_store_rewrites(self) -> None:
        # `STA (DPTR),Y` rewrites the dst.
        instrs = (
            _stage_dptr_from(0x80)
            + [
                asm_ast.Mov(src=_REG_A, dst=asm_ast.IndirectY()),
                asm_ast.Return(save_a=False),
            ]
        )
        out = _rewritten(instrs)
        self.assertIsInstance(out[4].dst, asm_ast.IndirectZpY)
        self.assertEqual(out[4].dst.address, 0x80)

    def test_multiple_indirect_uses_all_rewrite(self) -> None:
        instrs = (
            _stage_dptr_from(0x80)
            + [
                asm_ast.Mov(src=asm_ast.IndirectY(), dst=_REG_A),
                asm_ast.Mov(
                    src=_REG_A,
                    dst=asm_ast.ZP(address=0x20, offset=0),
                ),
                asm_ast.Mov(src=asm_ast.IndirectY(), dst=_REG_A),
                asm_ast.Return(save_a=False),
            ]
        )
        out = _rewritten(instrs)
        # Both IndirectY operands rewritten.
        rewritten_ops = [
            ins.src for ins in out
            if isinstance(ins, asm_ast.Mov)
            and isinstance(ins.src, (asm_ast.IndirectY, asm_ast.IndirectZpY))
        ]
        self.assertEqual(len(rewritten_ops), 2)
        for op in rewritten_ops:
            self.assertIsInstance(op, asm_ast.IndirectZpY)


class TestIndirectBaseInvalidation(unittest.TestCase):
    def test_write_to_source_invalidates(self) -> None:
        # A write to the source pair's low byte breaks the
        # equivalence; subsequent indirect uses don't get rewritten.
        instrs = (
            _stage_dptr_from(0x80)
            + [
                asm_ast.Mov(
                    src=asm_ast.Imm(value=0xFF),
                    dst=_REG_A,
                ),
                asm_ast.Mov(src=_REG_A, dst=_zp(0x80)),
                asm_ast.Mov(src=asm_ast.IndirectY(), dst=_REG_A),
                asm_ast.Return(save_a=False),
            ]
        )
        out = _rewritten(instrs)
        # The IndirectY operand is NOT rewritten.
        last_mov_with_ind = out[-2]
        self.assertIsInstance(last_mov_with_ind.src, asm_ast.IndirectY)

    def test_write_to_dptr_invalidates(self) -> None:
        instrs = (
            _stage_dptr_from(0x80)
            + [
                asm_ast.Mov(src=asm_ast.Imm(value=0), dst=_REG_A),
                asm_ast.Mov(src=_REG_A, dst=_dptr(0)),
                asm_ast.Mov(src=asm_ast.IndirectY(), dst=_REG_A),
                asm_ast.Return(save_a=False),
            ]
        )
        out = _rewritten(instrs)
        self.assertIsInstance(out[-2].src, asm_ast.IndirectY)

    def test_disjoint_writes_preserve_equivalence(self) -> None:
        # STA $84 and STA $20A8,X don't touch DPTR or $80/$81.
        # The equivalence survives; the IndirectY rewrites.
        instrs = (
            _stage_dptr_from(0x80)
            + [
                asm_ast.Mov(
                    src=asm_ast.Imm(value=0x42), dst=_REG_A,
                ),
                asm_ast.Mov(src=_REG_A, dst=_zp(0x84)),
                asm_ast.Mov(
                    src=_REG_A,
                    dst=asm_ast.IndexedData(
                        name="", offset=0x20A8, index=asm_ast.X(),
                    ),
                ),
                asm_ast.Mov(src=asm_ast.IndirectY(), dst=_REG_A),
                asm_ast.Return(save_a=False),
            ]
        )
        out = _rewritten(instrs)
        # Last Mov's src is rewritten.
        self.assertIsInstance(out[-2].src, asm_ast.IndirectZpY)


class TestIndirectBaseBoundaries(unittest.TestCase):
    def test_label_resets_state(self) -> None:
        instrs = (
            _stage_dptr_from(0x80)
            + [
                asm_ast.Label(name=".mid"),
                asm_ast.Mov(src=asm_ast.IndirectY(), dst=_REG_A),
                asm_ast.Return(save_a=False),
            ]
        )
        out = _rewritten(instrs)
        # After Label, equivalence is cleared.
        self.assertIsInstance(out[-2].src, asm_ast.IndirectY)

    def test_branch_resets_state(self) -> None:
        instrs = (
            _stage_dptr_from(0x80)
            + [
                asm_ast.Branch(cond=asm_ast.NE(), target="L"),
                asm_ast.Mov(src=asm_ast.IndirectY(), dst=_REG_A),
                asm_ast.Return(save_a=False),
                asm_ast.Label(name="L"),
                asm_ast.Return(save_a=False),
            ]
        )
        out = _rewritten(instrs)
        # The Mov after Branch is in a fresh block; not rewritten.
        # (Even though dominance would allow it, our per-block
        # walker is intentionally simple.)
        self.assertIsInstance(out[5].src, asm_ast.IndirectY)


class TestIndirectBaseStageShape(unittest.TestCase):
    def test_non_adjacent_pair_does_not_match(self) -> None:
        # Source pair is ($80, $A0) — not adjacent. No match.
        instrs = [
            asm_ast.Mov(src=_zp(0x80), dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=_dptr(0)),
            asm_ast.Mov(src=_zp(0xA0), dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=_dptr(1)),
            asm_ast.Mov(src=asm_ast.IndirectY(), dst=_REG_A),
            asm_ast.Return(save_a=False),
        ]
        out = _rewritten(instrs)
        self.assertIsInstance(out[4].src, asm_ast.IndirectY)

    def test_wrong_dst_does_not_match(self) -> None:
        # The "stage" writes to some Data other than DPTR. No match.
        instrs = [
            asm_ast.Mov(src=_zp(0x80), dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=asm_ast.Data(name="g", offset=0)),
            asm_ast.Mov(src=_zp(0x81), dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=asm_ast.Data(name="g", offset=1)),
            asm_ast.Mov(src=asm_ast.IndirectY(), dst=_REG_A),
            asm_ast.Return(save_a=False),
        ]
        out = _rewritten(instrs)
        self.assertIsInstance(out[4].src, asm_ast.IndirectY)
