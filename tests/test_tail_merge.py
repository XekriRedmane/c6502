"""Tests for the return-tail merge (`passes/tail_merge.py`)."""

import shutil
import unittest

import asm_ast
from passes.tail_merge import apply_tail_merge
from sim.harness import build_sim

_A = asm_ast.Reg(reg=asm_ast.A())
_X = asm_ast.Reg(reg=asm_ast.X())


def _run(instrs):
    prog = asm_ast.Program(top_level=[asm_ast.Function(
        name="f", is_global=False, params=[], instructions=instrs,
    )])
    return apply_tail_merge(prog).top_level[0].instructions


def _ret0():
    """`LDX #0; TXA; RTS`, the tail of `return 0` in A and X."""
    return [
        asm_ast.Mov(src=asm_ast.Imm(value=0), dst=_X, is_volatile=False),
        asm_ast.Mov(src=_X, dst=_A, is_volatile=False),
        asm_ast.Return(save_a=True, save_x=True),
    ]


def _cmp():
    return asm_ast.Compare(left=_A, right=asm_ast.Imm(value=3))


class TestTailMerge(unittest.TestCase):

    def test_branch_over_tail_becomes_branch_to_tail(self):
        instrs = (
            [_cmp(), asm_ast.Branch(cond=asm_ast.CS(), target="L")]
            + _ret0()
            + [asm_ast.Label(name="L"),
               asm_ast.Mov(src=asm_ast.Imm(value=7), dst=_A,
                           is_volatile=False)]
            + _ret0()
        )
        out = _run(instrs)
        self.assertEqual(out[1], asm_ast.Branch(
            cond=asm_ast.CC(), target=".tail@0",
        ))
        self.assertEqual(out[2], asm_ast.Label(name="L"))
        self.assertEqual(out[4], asm_ast.Label(name=".tail@0"))
        self.assertEqual(out[5:], _ret0())

    def test_existing_label_reused(self):
        instrs = (
            [asm_ast.Branch(cond=asm_ast.EQ(), target="L")]
            + _ret0()
            + [asm_ast.Label(name="L"),
               asm_ast.Jump(target="T", reg_args=[]),
               asm_ast.Label(name="T")]
            + _ret0()
        )
        out = _run(instrs)
        self.assertEqual(out[0], asm_ast.Branch(
            cond=asm_ast.NE(), target="T",
        ))

    def test_lone_return_not_merged(self):
        ret = asm_ast.Return(save_a=False, save_x=False)
        instrs = [
            asm_ast.Branch(cond=asm_ast.EQ(), target="L"), ret,
            asm_ast.Label(name="L"),
            asm_ast.Mov(src=asm_ast.Imm(value=1), dst=_A, is_volatile=False),
            ret,
        ]
        self.assertEqual(_run(instrs), instrs)

    def test_out_of_reach_not_merged(self):
        filler = [
            asm_ast.Mov(src=asm_ast.Imm(value=1),
                        dst=asm_ast.Data(name="g", offset=0),
                        is_volatile=False)
            for _ in range(40)
        ]
        instrs = (
            [asm_ast.Branch(cond=asm_ast.EQ(), target="L")]
            + _ret0()
            + [asm_ast.Label(name="L")] + filler + _ret0()
        )
        self.assertEqual(_run(instrs), instrs)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestTailMergeSim(unittest.TestCase):

    def test_early_returns(self):
        src = (
            "unsigned int f(unsigned char a, unsigned char b) {\n"
            "    if (a < 3) return 0;\n"
            "    if (b < 3) return 0;\n"
            "    if (a == b) return 0;\n"
            "    return a + b;\n"
            "}\n"
            "int main(void) {\n"
            "    return f(1, 9) + f(9, 1) + f(5, 5) + f(4, 6) * 10;\n"
            "}\n"
        )
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                got = build_sim(src, optimize=optimize).run().return_int()
                self.assertEqual(got, 100)


if __name__ == "__main__":
    unittest.main()
