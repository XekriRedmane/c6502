"""Unit tests for `passes.optimization.dead_loop_elimination`."""

import unittest

import ir_dump
import tac_ast
from passes.optimization.dead_loop_elimination import EliminateDeadLoops
from passes.optimization.framework import PassContext


def _ci(v: int) -> tac_ast.Constant:
    return tac_ast.Constant(const=tac_ast.ConstInt(value=v))


def _loop_with_exit_phi() -> tac_ast.Function:
    """A pure loop whose one exit leaves from `b`, a block after its
    header `h`, to a join whose Phi names `b`."""
    var = tac_ast.Var
    return tac_ast.Function(name="f", is_global=True, params=[], instructions=[
        tac_ast.Label(name="h"),
        tac_ast.Copy(src=_ci(1), dst=var(name="t.1")),
        tac_ast.Label(name="b"),
        tac_ast.JumpIfTrue(condition=var(name="c"), target="x"),
        tac_ast.Jump(target="h"),
        tac_ast.Label(name="x"),
        tac_ast.Phi(dst=var(name="y.1"), args=[
            tac_ast.PhiArg(pred_label="b", source=_ci(5)),
        ]),
        tac_ast.Ret(val=var(name="y.1")),
    ])


class TestExitPhis(unittest.TestCase):
    def test_exit_phi_names_the_header(self):
        # The header now jumps to the exit, so the exit's Phi takes its
        # argument from the header.
        out = EliminateDeadLoops().run(
            _loop_with_exit_phi(), PassContext(ssa_dsts={"t.1", "y.1"}),
        )
        [phi] = [i for i in out.instructions if isinstance(i, tac_ast.Phi)]
        self.assertEqual([a.pred_label for a in phi.args], ["h"])

    def test_input_unchanged(self):
        # The Phi is a new object. Retagging the argument in place
        # changed the function the pass was given, which the driver's
        # fixed point compares with the function after the sweep.
        fn = _loop_with_exit_phi()
        before = ir_dump.dump(fn)
        EliminateDeadLoops().run(fn, PassContext(ssa_dsts={"t.1", "y.1"}))
        self.assertEqual(ir_dump.dump(fn), before)


if __name__ == "__main__":
    unittest.main()
