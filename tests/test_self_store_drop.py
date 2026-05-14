"""Tests for the adjacent LDA M; STA M self-store dropper."""

import unittest

import asm_ast
from passes.self_store_drop import apply_self_store_drop


_A = asm_ast.Reg(reg=asm_ast.A())


def _instrs(prog):
    return prog.top_level[0].instructions


def _wrap(instrs):
    return asm_ast.Program(top_level=[asm_ast.Function(
        name="f", is_global=False, params=[], instructions=instrs,
    )])


class TestSelfStoreDrop(unittest.TestCase):

    def test_lda_sta_same_data_drops_sta(self):
        prog = _wrap([
            asm_ast.Mov(src=asm_ast.Data(name="M", offset=0), dst=_A),
            asm_ast.Mov(src=_A, dst=asm_ast.Data(name="M", offset=0)),
        ])
        out = _instrs(apply_self_store_drop(prog))
        self.assertEqual(out, [
            asm_ast.Mov(src=asm_ast.Data(name="M", offset=0), dst=_A),
        ])

    def test_lda_sta_same_zp_drops_sta(self):
        prog = _wrap([
            asm_ast.Mov(src=asm_ast.ZP(address=0x90, offset=0), dst=_A),
            asm_ast.Mov(src=_A, dst=asm_ast.ZP(address=0x90, offset=0)),
        ])
        out = _instrs(apply_self_store_drop(prog))
        self.assertEqual(len(out), 1)

    def test_different_targets_dont_drop(self):
        prog = _wrap([
            asm_ast.Mov(src=asm_ast.Data(name="M", offset=0), dst=_A),
            asm_ast.Mov(src=_A, dst=asm_ast.Data(name="N", offset=0)),
        ])
        out = _instrs(apply_self_store_drop(prog))
        self.assertEqual(len(out), 2)

    def test_different_offsets_dont_drop(self):
        # M[0] and M[1] are different bytes — not a self-store.
        prog = _wrap([
            asm_ast.Mov(src=asm_ast.Data(name="M", offset=0), dst=_A),
            asm_ast.Mov(src=_A, dst=asm_ast.Data(name="M", offset=1)),
        ])
        out = _instrs(apply_self_store_drop(prog))
        self.assertEqual(len(out), 2)


if __name__ == "__main__":
    unittest.main()
