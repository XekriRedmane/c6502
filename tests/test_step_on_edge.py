"""Tests for `passes.optimization.step_on_edge`, which moves a step
whose result is copied on one edge alone onto that edge, and runs of
the loops it rewrites in both pipelines."""

from __future__ import annotations

import shutil
import unittest

import tac_ast
from passes.optimization.step_on_edge import move_steps_onto_edges


def _var(name):
    return tac_ast.Var(name=name)


def _uc(v):
    return tac_ast.Constant(const=tac_ast.ConstUChar(value=v))


def _fn(*instrs):
    return tac_ast.Function(
        name="f", is_global=True, params=["first"],
        instructions=list(instrs),
    )


_SSA = {"i", "t", "first"}


def _loop(*between, tail=()):
    """`do g(i); while (i-- != first);` after SSA destruction."""
    return _fn(
        tac_ast.Label(name=".loop"),
        tac_ast.FunctionCall(name="g", args=[_var("i")], dst=None,
                             hw_stack_save=False, noreturn=False),
        tac_ast.Binary(op=tac_ast.Subtract(), src1=_var("i"), src2=_uc(1),
                       dst=_var("t")),
        *between,
        tac_ast.JumpIfCmp(op=tac_ast.NotEqual(), src1=_var("i"),
                          src2=_var("first"), target=".split"),
        tac_ast.Ret(val=None),
        tac_ast.Label(name=".split"),
        tac_ast.Copy(src=_var("t"), dst=_var("i")),
        tac_ast.Jump(target=".loop"),
        *tail,
    )


class TestMoveStepsOntoEdges(unittest.TestCase):

    def test_step_moves_into_the_split_block(self):
        out = move_steps_onto_edges(_loop(), _SSA).instructions
        self.assertNotIn(
            tac_ast.Binary(op=tac_ast.Subtract(), src1=_var("i"),
                           src2=_uc(1), dst=_var("t")), out)
        self.assertEqual(out[-2], tac_ast.Binary(
            op=tac_ast.Subtract(), src1=_var("i"), src2=_uc(1),
            dst=_var("i")))

    def test_step_moves_onto_the_fall_through(self):
        fn = _fn(
            tac_ast.Binary(op=tac_ast.Add(), src1=_var("i"), src2=_uc(1),
                           dst=_var("t")),
            tac_ast.JumpIfCmp(op=tac_ast.Equal(), src1=_var("i"),
                              src2=_var("first"), target=".out"),
            tac_ast.Copy(src=_var("t"), dst=_var("i")),
            tac_ast.Label(name=".out"),
            tac_ast.Ret(val=_var("i")),
        )
        out = move_steps_onto_edges(fn, _SSA).instructions
        self.assertEqual(out[1], tac_ast.Binary(
            op=tac_ast.Add(), src1=_var("i"), src2=_uc(1), dst=_var("i")))
        self.assertEqual(len(out), 4)

    def test_kept_when_the_value_changes_before_the_jump(self):
        fn = _loop(tac_ast.Copy(src=_uc(3), dst=_var("i")))
        self.assertEqual(move_steps_onto_edges(fn, _SSA), fn)

    def test_kept_when_the_result_is_read_twice(self):
        fn = _loop(tail=[tac_ast.Ret(val=_var("t"))])
        self.assertEqual(move_steps_onto_edges(fn, _SSA), fn)

    def test_kept_when_the_block_has_another_way_in(self):
        fn = _loop(tail=[tac_ast.Jump(target=".split")])
        self.assertEqual(move_steps_onto_edges(fn, _SSA), fn)

    def test_kept_for_a_static(self):
        fn = _loop()
        self.assertEqual(move_steps_onto_edges(fn, {"t", "first"}), fn)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestStepOnEdgeSim(unittest.TestCase):

    _SRC = """
#include <stdint.h>
uint8_t seen[300];
uint16_t n;
void g(uint8_t i) { seen[n++] = i; }
void down(uint8_t first, uint8_t last) {
    uint8_t i = last;
    do g(i); while (i-- != first);
}
uint8_t up(uint8_t i, uint8_t last) {
    while (i++ != last)
        g(i);
    return i;
}
int main(void) {
    down(%d, %d);
    seen[n++] = up(%d, %d);
    return n;
}
"""

    def _model(self, a, b, c, d):
        out = []
        i = b
        while True:
            out.append(i)
            old = i
            i = (i - 1) & 0xFF
            if old == a:
                break
        i = c
        while True:
            old = i
            i = (i + 1) & 0xFF
            if old == d:
                break
            out.append(i)
        out.append(i)
        return out

    def test_runs(self):
        from sim.harness import build_sim
        for a, b, c, d in [(0, 0, 0, 0), (3, 7, 250, 255), (0, 5, 4, 9),
                           (255, 255, 254, 255), (10, 9, 0, 3)]:
            want = self._model(a, b, c, d)
            if len(want) > 300:
                continue
            for optimize in (False, True):
                with self.subTest(args=(a, b, c, d), optimize=optimize):
                    sim = build_sim(self._SRC % (a, b, c, d),
                                    optimize=optimize)
                    result = sim.run(max_cycles=5_000_000)
                    self.assertEqual(result.return_int(), len(want))
                    base = sim.symbols["seen"]
                    got = list(sim.mpu.memory[base:base + len(want)])
                    self.assertEqual(got, want)


if __name__ == "__main__":
    unittest.main()
