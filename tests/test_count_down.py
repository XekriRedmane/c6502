"""Tests for `passes.optimization.count_down`, which counts a loop down
to 0 when its counter is read only by its own test and step, and runs
of such loops in both pipelines."""

from __future__ import annotations

import shutil
import unittest

import c99_ast
import tac_ast
from passes.optimization.count_down import count_loops_down
from passes.optimization.framework import PassContext
from passes.type_checking import LocalAttr, Symbol, SymbolTable


def _var(name):
    return tac_ast.Var(name=name)


def _uc(v):
    return tac_ast.Constant(const=tac_ast.ConstUChar(value=v))


def _ctx():
    symbols = SymbolTable()
    for name in ("i", "i2", "k"):
        symbols[name] = Symbol(type=c99_ast.UChar(), attrs=LocalAttr())
    return PassContext(symbols=symbols, ssa_dsts={"i", "i2", "k"})


def _call():
    return tac_ast.FunctionCall(name="tick", args=[], dst=None,
                                hw_stack_save=False, noreturn=False)


def _loop(test, start=_uc(0), extra=()):
    """`for (i = start; <test>; i++) tick();` in SSA form."""
    return tac_ast.Function(name="f", is_global=True, params=["k"],
                            instructions=[
        tac_ast.Label(name=".entry"),
        tac_ast.Jump(target=".top"),
        tac_ast.Label(name=".top"),
        tac_ast.Phi(dst=_var("i"), args=[
            tac_ast.PhiArg(pred_label=".entry", source=start),
            tac_ast.PhiArg(pred_label=".cont", source=_var("i2")),
        ]),
        test,
        _call(),
        *extra,
        tac_ast.Label(name=".cont"),
        tac_ast.Binary(op=tac_ast.Add(), src1=_var("i"), src2=_uc(1),
                       dst=_var("i2")),
        tac_ast.Jump(target=".top"),
        tac_ast.Label(name=".break"),
        tac_ast.Ret(val=None),
    ])


def _ge(bound):
    return tac_ast.JumpIfCmp(op=tac_ast.GreaterOrEqual(), src1=_var("i"),
                             src2=bound, target=".break")


class TestCountLoopsDown(unittest.TestCase):

    def test_constant_bound(self):
        out = count_loops_down(_loop(_ge(_uc(20))), _ctx()).instructions
        self.assertEqual(out[3].args[0].source, _uc(20))
        self.assertEqual(out[4], tac_ast.JumpIfCmp(
            op=tac_ast.Equal(), src1=_var("i"), src2=_uc(0),
            target=".break"))
        self.assertEqual(out[7], tac_ast.Binary(
            op=tac_ast.Subtract(), src1=_var("i"), src2=_uc(1),
            dst=_var("i2")))

    def test_start_above_zero(self):
        out = count_loops_down(_loop(_ge(_uc(20)), start=_uc(5)),
                               _ctx()).instructions
        self.assertEqual(out[3].args[0].source, _uc(15))

    def test_variable_bound(self):
        out = count_loops_down(_loop(_ge(_var("k"))), _ctx()).instructions
        self.assertEqual(out[3].args[0].source, _var("k"))

    def test_variable_bound_needs_a_start_of_zero(self):
        fn = _loop(_ge(_var("k")), start=_uc(1))
        self.assertEqual(count_loops_down(fn, _ctx()), fn)

    def test_start_above_the_bound_is_left(self):
        fn = _loop(_ge(_uc(3)), start=_uc(5))
        self.assertEqual(count_loops_down(fn, _ctx()), fn)

    def test_counter_read_in_the_body_is_left(self):
        fn = _loop(_ge(_uc(20)), extra=[tac_ast.FunctionCall(
            name="out", args=[_var("i")], dst=None, hw_stack_save=False,
            noreturn=False)])
        self.assertEqual(count_loops_down(fn, _ctx()), fn)

    def test_ordering_test_the_wrong_way_is_left(self):
        fn = _loop(tac_ast.JumpIfCmp(op=tac_ast.LessThan(), src1=_var("i"),
                                     src2=_uc(20), target=".break"))
        self.assertEqual(count_loops_down(fn, _ctx()), fn)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestCountDownSim(unittest.TestCase):

    _SRC = """
#include <stdint.h>
uint16_t ticks;
void tick(void) { ticks++; }
uint8_t K;
void a(void) { for (uint8_t n = 0; n < 20; n++) tick(); }
void b(uint8_t k) { for (uint8_t n = 0; n < k; n++) tick(); }
void c(void) { for (uint8_t n = 3; n != 3; n++) tick(); }
void d(void) { for (uint8_t n = 7; n <= 254; n++) tick(); }
int main(void) {
    a(); uint16_t ra = ticks; ticks = 0;
    b(K); uint16_t rb = ticks; ticks = 0;
    c(); uint16_t rc = ticks; ticks = 0;
    d();
    return ra + rb * 2 + rc * 1000 + ticks * 4;
}
"""

    def test_runs(self):
        from sim.harness import build_sim
        for k in (0, 1, 200, 255):
            for optimize in (False, True):
                with self.subTest(k=k, optimize=optimize):
                    sim = build_sim(self._SRC.replace(
                        "uint8_t K;", f"uint8_t K = {k};"),
                        optimize=optimize)
                    out = sim.run(max_cycles=5_000_000)
                    self.assertEqual(out.return_int() & 0xFFFF,
                                     (20 + 2 * k + 4 * 248) & 0xFFFF)


if __name__ == "__main__":
    unittest.main()
