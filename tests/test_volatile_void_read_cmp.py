"""Tests for `passes.volatile_void_read_cmp`, which turns a volatile
indirect `LDA` whose value is never read into a `CMP` of the same
byte, so A keeps its earlier value.
"""
from __future__ import annotations

import unittest

import asm_ast
from passes.volatile_void_read_cmp import apply_volatile_void_read_cmp

_A = asm_ast.Reg(reg=asm_ast.A())
_LOAD = asm_ast.Mov(src=asm_ast.Indirect(offset=0), dst=_A, is_volatile=True)


def _run(instrs):
    fn = asm_ast.Function(
        name="f", is_global=True, params=[], instructions=instrs,
    )
    out = apply_volatile_void_read_cmp(asm_ast.Program(top_level=[fn]))
    return out.top_level[0].instructions


class TestVolatileVoidReadCmp(unittest.TestCase):

    def test_discarded_read_becomes_compare(self) -> None:
        out = _run([
            _LOAD,
            asm_ast.Mov(src=asm_ast.Imm(value=1), dst=_A, is_volatile=False),
            asm_ast.Return(save_a=True),
        ])
        self.assertEqual(
            out[0], asm_ast.Compare(left=_A, right=asm_ast.Indirect(offset=0)),
        )

    def test_read_feeding_and_is_kept(self) -> None:
        # `while (!(*p & 0x80)) ;` The AND reads the loaded byte.
        instrs = [
            asm_ast.Label(name=".top"),
            _LOAD,
            asm_ast.And(src=asm_ast.Imm(value=0x80), dst=_A),
            asm_ast.Branch(cond=asm_ast.EQ(), target=".top"),
            asm_ast.Return(save_a=False),
        ]
        self.assertEqual(_run(instrs), instrs)

    def test_read_feeding_compare_or_add_is_kept(self) -> None:
        for reader in (
            asm_ast.Compare(left=_A, right=asm_ast.Imm(value=5)),
            asm_ast.Add(src=asm_ast.Imm(value=1), dst=_A),
            asm_ast.Or(src=asm_ast.Imm(value=1), dst=_A),
        ):
            with self.subTest(reader=type(reader).__name__):
                instrs = [
                    asm_ast.ClearCarry(),
                    _LOAD,
                    reader,
                    asm_ast.Return(save_a=True),
                ]
                self.assertEqual(_run(instrs), instrs)

    def test_flags_read_is_kept(self) -> None:
        # `BPL` reads the N flag the load set.
        instrs = [
            asm_ast.Label(name=".top"),
            _LOAD,
            asm_ast.Branch(cond=asm_ast.PL(), target=".top"),
            asm_ast.Mov(src=asm_ast.Imm(value=1), dst=_A, is_volatile=False),
            asm_ast.Return(save_a=True),
        ]
        self.assertEqual(_run(instrs), instrs)


if __name__ == "__main__":
    unittest.main()
