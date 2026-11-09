"""Tests for the redundant-compare peephole (`passes/redundant_compare.py`)."""

import unittest

import asm_ast
from passes.redundant_compare import apply_redundant_compare

_A = asm_ast.Reg(reg=asm_ast.A())


def _run(instrs):
    prog = asm_ast.Program(top_level=[asm_ast.Function(
        name="f", is_global=False, params=[], instructions=instrs,
    )])
    return apply_redundant_compare(prog).top_level[0].instructions


def _cmp(addr=0x80):
    return asm_ast.Compare(left=_A, right=asm_ast.ZP(address=addr, offset=0))


def _ret():
    return asm_ast.Return(save_a=False, save_x=False)


class TestRedundantCompare(unittest.TestCase):

    def test_second_compare_dropped(self):
        # CMP M; BCS L; RTS; L: CMP M; BNE ... → the second CMP goes.
        instrs = [
            _cmp(), asm_ast.Branch(cond=asm_ast.CS(), target="L"), _ret(),
            asm_ast.Label(name="L"), _cmp(),
            asm_ast.Branch(cond=asm_ast.NE(), target="M"), _ret(),
            asm_ast.Label(name="M"), _ret(),
        ]
        out = _run(instrs)
        self.assertEqual(out[:5], instrs[:4] + [instrs[5]])

    def test_fall_through_into_label_kept(self):
        # The label is also reached by falling through, where the
        # flags may differ.
        instrs = [
            _cmp(), asm_ast.Branch(cond=asm_ast.CS(), target="L"),
            asm_ast.Mov(src=asm_ast.Imm(value=1), dst=_A, is_volatile=False),
            asm_ast.Label(name="L"), _cmp(), _ret(),
        ]
        self.assertEqual(_run(instrs), instrs)

    def test_second_branch_to_label_kept(self):
        instrs = [
            _cmp(), asm_ast.Branch(cond=asm_ast.CS(), target="L"),
            _cmp(0x81), asm_ast.Branch(cond=asm_ast.EQ(), target="L"),
            _ret(),
            asm_ast.Label(name="L"), _cmp(), _ret(),
        ]
        self.assertEqual(_run(instrs), instrs)

    def test_different_operand_kept(self):
        instrs = [
            _cmp(), asm_ast.Branch(cond=asm_ast.CS(), target="L"), _ret(),
            asm_ast.Label(name="L"), _cmp(0x81), _ret(),
        ]
        self.assertEqual(_run(instrs), instrs)


if __name__ == "__main__":
    unittest.main()
