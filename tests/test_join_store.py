"""Tests for `passes.join_store.apply_join_store`: a join's `LDA t; STA
M` goes into each arm's store to `t`."""

import unittest

import asm_ast
from passes.join_store import apply_join_store

_A = asm_ast.Reg(reg=asm_ast.A())
_X = asm_ast.Reg(reg=asm_ast.X())
_T = asm_ast.Data(name="__local_f__t", offset=0)
_M = asm_ast.Data(name="m", offset=1)
_LO = asm_ast.Data(name="m", offset=0)


def _run(instrs):
    prog = asm_ast.Program(top_level=[asm_ast.Function(
        name="f", is_global=False, params=[], instructions=instrs,
    )])
    return apply_join_store(prog).top_level[0].instructions


def _diamond(join):
    """`f`'s two arms, one storing X into `t` and one a constant, and
    the join, which runs `join` and returns."""
    return [
        asm_ast.Mov(src=asm_ast.Data(name="c", offset=0), dst=_A),
        asm_ast.Branch(cond=asm_ast.EQ(), target=".else"),
        asm_ast.Call(name="g"),
        asm_ast.Mov(src=_X, dst=_T),
        asm_ast.Jump(target=".end"),
        asm_ast.Label(name=".else"),
        asm_ast.Call(name="h"),
        asm_ast.Mov(src=asm_ast.Imm(value=0), dst=_T),
        asm_ast.Label(name=".end"),
        *join,
        asm_ast.Return(),
    ]


_JOIN = [
    asm_ast.Mov(src=_A, dst=_LO),
    asm_ast.Mov(src=_T, dst=_A),
    asm_ast.Mov(src=_A, dst=_M),
]


class TestJoinStore(unittest.TestCase):

    def test_arms_store_to_m(self):
        out = _run(_diamond(_JOIN))
        self.assertIn(asm_ast.Mov(src=_X, dst=_M), out)
        self.assertIn(asm_ast.Mov(src=asm_ast.Imm(value=0), dst=_M), out)
        self.assertNotIn(asm_ast.Mov(src=_T, dst=_A), out)
        k = out.index(asm_ast.Label(name=".end"))
        self.assertEqual(out[k + 1:], [
            asm_ast.Mov(src=_A, dst=_LO), asm_ast.Return(),
        ])

    def test_join_reads_m_first(self):
        # The join reads M before it copies t there, so M must keep
        # its old value until then.
        join = [asm_ast.Compare(left=_A, right=_M)] + _JOIN
        instrs = _diamond(join)
        self.assertEqual(_run(instrs), instrs)

    def test_t_read_later(self):
        instrs = _diamond(_JOIN + [asm_ast.Mov(src=_T, dst=_LO)])
        self.assertEqual(_run(instrs), instrs)

    def test_a_read_later(self):
        # A holds t after the copy, and the next store reads it.
        instrs = _diamond(_JOIN + [asm_ast.Mov(
            src=_A, dst=asm_ast.Data(name="n", offset=0))])
        self.assertEqual(_run(instrs), instrs)

    def test_branch_into_join(self):
        # A branch's store to t would reach its other path too.
        instrs = _diamond(_JOIN)
        k = instrs.index(asm_ast.Jump(target=".end"))
        instrs[k] = asm_ast.Branch(cond=asm_ast.NE(), target=".end")
        self.assertEqual(_run(instrs), instrs)

    def test_call_after_arm_store(self):
        # The call might read M, which would now be written before it.
        instrs = _diamond(_JOIN)
        k = instrs.index(asm_ast.Jump(target=".end"))
        instrs.insert(k, asm_ast.Call(name="k"))
        self.assertEqual(_run(instrs), instrs)


if __name__ == "__main__":
    unittest.main()
