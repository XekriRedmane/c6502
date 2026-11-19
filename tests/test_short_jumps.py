"""Tests for `passes.short_jumps.apply_short_jumps`: jumps to tail-call
stubs go to the function, the stubs go, and a `JMP` after a constant
load becomes a branch when its label is in reach."""

import unittest

import asm_ast
from passes.short_jumps import apply_short_jumps

_A = asm_ast.Reg(reg=asm_ast.A())


def _run(instrs):
    prog = asm_ast.Program(top_level=[asm_ast.Function(
        name="f", is_global=False, params=[], instructions=instrs,
    )])
    return apply_short_jumps(prog).top_level[0].instructions


def _ret():
    return asm_ast.Return(save_a=False)


class TestTailCallStubs(unittest.TestCase):

    def test_jump_and_table_go_to_the_function(self):
        out = _run([
            asm_ast.Jump(target=".default"),
            asm_ast.JumpTable(table=".jtab@0", index="Y", base=0,
                              targets=[".default", ".case"]),
            asm_ast.Label(name=".case"),
            _ret(),
            asm_ast.Label(name=".default"),
            asm_ast.Jump(target="internal_error"),
        ])
        self.assertEqual(out, [
            asm_ast.Jump(target="internal_error"),
            asm_ast.JumpTable(table=".jtab@0", index="Y", base=0,
                              targets=["internal_error", ".case"]),
            asm_ast.Label(name=".case"),
            _ret(),
        ])

    def test_stub_reached_by_a_branch_stays(self):
        instrs = [
            asm_ast.Branch(cond=asm_ast.EQ(), target=".default"),
            _ret(),
            asm_ast.Label(name=".default"),
            asm_ast.Jump(target="internal_error"),
        ]
        self.assertEqual(_run(list(instrs)), instrs)

    def test_tail_call_with_register_arguments_left_alone(self):
        instrs = [
            asm_ast.Jump(target=".stub"),
            asm_ast.Label(name=".stub"),
            asm_ast.Jump(target="g", reg_args=["A"]),
        ]
        self.assertEqual(_run(list(instrs)), instrs)


class TestConstantJumps(unittest.TestCase):

    def _bool(self, gap=0, k=0):
        return [
            asm_ast.Branch(cond=asm_ast.EQ(), target=".t"),
            asm_ast.Mov(src=asm_ast.Imm(value=k), dst=_A),
            asm_ast.Jump(target=".end"),
            asm_ast.Label(name=".t"),
            asm_ast.Mov(src=asm_ast.Imm(value=1), dst=_A),
            *[asm_ast.ClearCarry()] * gap,
            asm_ast.Label(name=".end"),
            _ret(),
        ]

    def test_load_of_zero_then_jump_is_beq(self):
        out = _run(self._bool())
        self.assertEqual(out[2], asm_ast.Branch(cond=asm_ast.EQ(), target=".end"))

    def test_load_of_nonzero_then_jump_is_bne(self):
        out = _run(self._bool(k=5))
        self.assertEqual(out[2], asm_ast.Branch(cond=asm_ast.NE(), target=".end"))

    def test_out_of_reach_stays_a_jump(self):
        # `.end` is `3 + gap` bytes past the end of the branch: the
        # `JMP`'s third byte, `LDA #1`, and the CLCs. The margin allows
        # 126.
        out = _run(self._bool(gap=124))
        self.assertEqual(out[2], asm_ast.Jump(target=".end"))
        out = _run(self._bool(gap=123))
        self.assertEqual(out[2], asm_ast.Branch(cond=asm_ast.EQ(), target=".end"))


class TestJumpAfterBranch(unittest.TestCase):

    def _code(self, gap=0, label_between=False):
        return [
            asm_ast.Branch(cond=asm_ast.CS(), target=".a"),
            *([asm_ast.Label(name=".m")] if label_between else []),
            asm_ast.Jump(target=".b"),
            asm_ast.Label(name=".a"),
            *[asm_ast.ClearCarry()] * gap,
            _ret(),
            asm_ast.Label(name=".b"),
            _ret(),
        ]

    def test_jump_becomes_the_opposite_branch(self):
        out = _run(self._code())
        self.assertEqual(out[1], asm_ast.Branch(cond=asm_ast.CC(), target=".b"))

    def test_label_between_stays_a_jump(self):
        # `.m` may be reached with C set.
        out = _run(self._code(label_between=True))
        self.assertEqual(out[2], asm_ast.Jump(target=".b"))

    def test_out_of_reach_stays_a_jump(self):
        # `.b` is `2 + gap` bytes past the end of the branch: the
        # `JMP`'s third byte, the CLCs, and the `RTS`.
        out = _run(self._code(gap=125))
        self.assertEqual(out[1], asm_ast.Jump(target=".b"))
        out = _run(self._code(gap=124))
        self.assertEqual(out[1], asm_ast.Branch(cond=asm_ast.CC(), target=".b"))


if __name__ == "__main__":
    unittest.main()
