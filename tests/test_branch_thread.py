"""Tests for `passes.branch_thread.apply_branch_thread`: a branch to a
branch on the same flag goes where that one would."""

import unittest

import asm_ast
from passes.branch_thread import apply_branch_thread


def _run(instrs):
    prog = asm_ast.Program(top_level=[asm_ast.Function(
        name="f", is_global=False, params=[], instructions=instrs,
    )])
    return apply_branch_thread(prog).top_level[0].instructions


def _br(cond, target):
    return asm_ast.Branch(cond=cond, target=target)


_X = asm_ast.Reg(reg=asm_ast.X())


class TestBranchThread(unittest.TestCase):

    def test_opposite_goes_past(self):
        # do_rwts_on_sector: from the BCS, C is set, so the BCC isn't
        # taken.
        out = _run([
            _br(asm_ast.CS(), ".skip"),
            asm_ast.Dec(dst=_X),
            asm_ast.Label(name=".skip"),
            _br(asm_ast.CC(), ".break"),
            asm_ast.Inc(dst=_X),
            asm_ast.Return(save_a=False),
            asm_ast.Label(name=".break"),
            asm_ast.Return(save_a=False),
        ])
        self.assertEqual(out[0], _br(asm_ast.CS(), ".thread@0"))
        self.assertEqual(out[3:5], [
            _br(asm_ast.CC(), ".break"), asm_ast.Label(name=".thread@0"),
        ])

    def test_opposite_uses_an_existing_label(self):
        out = _run([
            _br(asm_ast.EQ(), ".a"),
            asm_ast.Dec(dst=_X),
            asm_ast.Label(name=".a"),
            _br(asm_ast.NE(), ".b"),
            asm_ast.Label(name=".next"),
            asm_ast.Return(save_a=False),
            asm_ast.Label(name=".b"),
            asm_ast.Return(save_a=False),
        ])
        self.assertEqual(out[0], _br(asm_ast.EQ(), ".next"))
        self.assertEqual(len(out), 8)

    def test_same_goes_to_its_target(self):
        out = _run([
            _br(asm_ast.MI(), ".a"),
            asm_ast.Dec(dst=_X),
            asm_ast.Label(name=".a"),
            _br(asm_ast.MI(), ".b"),
            asm_ast.Return(save_a=False),
            asm_ast.Label(name=".b"),
            asm_ast.Return(save_a=False),
        ])
        self.assertEqual(out[0], _br(asm_ast.MI(), ".b"))

    def test_other_flag_left_alone(self):
        instrs = [
            _br(asm_ast.CS(), ".a"),
            asm_ast.Dec(dst=_X),
            asm_ast.Label(name=".a"),
            _br(asm_ast.EQ(), ".b"),
            asm_ast.Return(save_a=False),
            asm_ast.Label(name=".b"),
            asm_ast.Return(save_a=False),
        ]
        self.assertEqual(_run(list(instrs)), instrs)

    def test_instruction_between_left_alone(self):
        # The DEX after the label could change Z.
        instrs = [
            _br(asm_ast.NE(), ".a"),
            asm_ast.Inc(dst=_X),
            asm_ast.Label(name=".a"),
            asm_ast.Dec(dst=_X),
            _br(asm_ast.EQ(), ".b"),
            asm_ast.Return(save_a=False),
            asm_ast.Label(name=".b"),
            asm_ast.Return(save_a=False),
        ]
        self.assertEqual(_run(list(instrs)), instrs)


if __name__ == "__main__":
    unittest.main()
