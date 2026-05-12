"""Tests for the shared asm-level aliasing predicate."""
from __future__ import annotations

import unittest

import asm_ast
from passes.asm_aliasing import (
    DEFAULT_POOL_LO,
    DEFAULT_POOL_HI,
    may_alias,
)


def _zp(addr: int, off: int = 0) -> asm_ast.ZP:
    return asm_ast.ZP(address=addr, offset=off)


def _data(name: str, off: int = 0) -> asm_ast.Data:
    return asm_ast.Data(name=name, offset=off)


def _idata(name: str, off: int = 0, idx=None) -> asm_ast.IndexedData:
    return asm_ast.IndexedData(
        name=name, offset=off, index=idx or asm_ast.X(),
    )


def _frame(off: int) -> asm_ast.Frame:
    return asm_ast.Frame(offset=off)


def _stack(off: int) -> asm_ast.Stack:
    return asm_ast.Stack(offset=off)


def _indirect(off: int) -> asm_ast.Indirect:
    return asm_ast.Indirect(offset=off)


def _indy() -> asm_ast.IndirectY:
    return asm_ast.IndirectY()


class TestImmediates(unittest.TestCase):
    def test_imm_never_aliases_anything(self) -> None:
        imm = asm_ast.Imm(value=5)
        self.assertFalse(may_alias(imm, imm))
        self.assertFalse(may_alias(imm, _zp(0x80)))
        self.assertFalse(may_alias(_data("DPTR"), imm))


class TestZpVsZp(unittest.TestCase):
    def test_same_address_aliases(self) -> None:
        self.assertTrue(may_alias(_zp(0x80), _zp(0x80)))

    def test_different_addresses_do_not_alias(self) -> None:
        self.assertFalse(may_alias(_zp(0x80), _zp(0x81)))

    def test_offset_resolves_to_same_byte(self) -> None:
        # ZP(0x80, off=1) and ZP(0x81, off=0) both name byte $81.
        self.assertTrue(may_alias(_zp(0x80, 1), _zp(0x81, 0)))


class TestZpVsData(unittest.TestCase):
    def test_zp_and_data_are_disjoint_namespaces(self) -> None:
        # ZP-as-literal-address and Data-as-symbol-name don't overlap
        # in c6502's emission convention.
        self.assertFalse(may_alias(_zp(0x80), _data("g")))
        self.assertFalse(may_alias(_zp(0x80), _data("DPTR")))


class TestDataVsData(unittest.TestCase):
    def test_same_name_offset_aliases(self) -> None:
        self.assertTrue(may_alias(_data("g", 2), _data("g", 2)))

    def test_different_offsets_do_not_alias(self) -> None:
        self.assertFalse(may_alias(_data("g", 0), _data("g", 1)))

    def test_different_names_do_not_alias(self) -> None:
        self.assertFalse(may_alias(_data("g"), _data("h")))


class TestFrameStackVsZp(unittest.TestCase):
    """Frame and Stack reads go through FP/SSP, which point into
    the soft stack in main RAM (≥ $0800). They never reach ZP."""

    def test_frame_does_not_alias_zp(self) -> None:
        self.assertFalse(may_alias(_frame(0), _zp(0x80)))
        self.assertFalse(may_alias(_zp(0x84), _frame(5)))
        self.assertFalse(may_alias(_zp(0x00), _frame(0)))

    def test_stack_does_not_alias_zp(self) -> None:
        self.assertFalse(may_alias(_stack(3), _zp(0x80)))
        self.assertFalse(may_alias(_zp(0x90), _stack(0)))


class TestIndirectVsZpPool(unittest.TestCase):
    """Indirect / IndirectY go through DPTR, holding a user pointer.
    User pointers don't point into the asm-level regalloc pool
    ($80-$FF by default) because address-taken locals spill to
    Frame, not ZP."""

    def test_indirect_does_not_alias_pool_zp(self) -> None:
        self.assertFalse(may_alias(_indirect(0), _zp(0x80)))
        self.assertFalse(may_alias(_zp(0x84), _indirect(7)))
        self.assertFalse(may_alias(_indirect(0), _zp(0xFF)))

    def test_indirect_y_does_not_alias_pool_zp(self) -> None:
        self.assertFalse(may_alias(_indy(), _zp(0x80)))
        self.assertFalse(may_alias(_zp(0xC0), _indy()))

    def test_indirect_does_alias_non_pool_zp(self) -> None:
        # Below the pool: HARGS / DPTR / FP / SSP territory.
        # User pointers could conceivably target HARGS, so be
        # conservative.
        self.assertTrue(may_alias(_indirect(0), _zp(0x04)))
        self.assertTrue(may_alias(_zp(0x24), _indy()))

    def test_pool_boundary(self) -> None:
        # ZP at exactly DEFAULT_POOL_LO is in the pool.
        self.assertFalse(may_alias(_indirect(0), _zp(DEFAULT_POOL_LO)))
        # ZP just below the pool is not.
        self.assertTrue(
            may_alias(_indirect(0), _zp(DEFAULT_POOL_LO - 1)),
        )

    def test_custom_pool_range(self) -> None:
        # If the pool starts at $90, $80..$8F isn't in it.
        self.assertTrue(
            may_alias(_indirect(0), _zp(0x80), pool_lo=0x90),
        )
        self.assertFalse(
            may_alias(_indirect(0), _zp(0x90), pool_lo=0x90),
        )


class TestSameKindIndirects(unittest.TestCase):
    """Same-kind aliasing for indirect-Y operand families."""

    def test_frame_same_offset_aliases(self) -> None:
        self.assertTrue(may_alias(_frame(3), _frame(3)))

    def test_frame_different_offset_does_not_alias(self) -> None:
        self.assertFalse(may_alias(_frame(3), _frame(4)))

    def test_stack_same_offset_aliases(self) -> None:
        self.assertTrue(may_alias(_stack(0), _stack(0)))

    def test_stack_different_offset_does_not_alias(self) -> None:
        self.assertFalse(may_alias(_stack(0), _stack(1)))

    def test_indirect_same_offset_aliases(self) -> None:
        self.assertTrue(may_alias(_indirect(2), _indirect(2)))

    def test_indirect_different_offset_does_not_alias(self) -> None:
        self.assertFalse(may_alias(_indirect(0), _indirect(1)))

    def test_frame_vs_stack_do_not_alias(self) -> None:
        # Separate ZP pointer pairs (FP vs SSP).
        self.assertFalse(may_alias(_frame(5), _stack(5)))
        self.assertFalse(may_alias(_stack(0), _frame(0)))


class TestIndexedData(unittest.TestCase):
    """IndexedData (absolute,X / absolute,Y) doesn't alias ZP
    (different memory regions) but may alias Data / IndexedData
    (conservative — we don't resolve the index range here)."""

    def test_indexed_data_vs_zp(self) -> None:
        self.assertFalse(may_alias(_idata("", 0x4000), _zp(0x80)))
        self.assertFalse(may_alias(_zp(0xFF), _idata("g", 0)))
