"""Tests for `passes.optimization.divmod_hoist`, which computes a divide
and a remainder of the same operands next to each other, and runs of
the functions it rewrites in both pipelines."""

from __future__ import annotations

import shutil
import unittest

import c99_ast
import tac_ast
from passes.optimization.divmod_hoist import hoist_divmod_pairs
from passes.optimization.framework import PassContext
from passes.type_checking import LocalAttr, StaticAttr, Symbol, SymbolTable


def _var(name):
    return tac_ast.Var(name=name)


def _uc(v):
    return tac_ast.Constant(const=tac_ast.ConstUChar(value=v))


def _ctx():
    symbols = SymbolTable()
    for name in ("s", "q", "r", "t", "p"):
        symbols[name] = Symbol(type=c99_ast.UChar(), attrs=LocalAttr())
    for name in ("V", "Q", "R"):
        symbols[name] = Symbol(type=c99_ast.UChar(), attrs=StaticAttr(
            initial_value=None, is_global=True))
    return PassContext(symbols=symbols, ssa_dsts={"s", "q", "r", "t", "p"})


def _fn(*instrs):
    return tac_ast.Function(name="f", is_global=True, params=["s"],
                            instructions=list(instrs))


def _div(src, dst, op=tac_ast.Divide, c=13):
    return tac_ast.Binary(op=op(), src1=_var(src), src2=_uc(c),
                          dst=_var(dst))


def _store(v):
    return tac_ast.IndirectIndexedStore(ptr=_var("p"), index=_uc(5),
                                        src=_var(v), is_volatile=False)


class TestHoistDivmodPairs(unittest.TestCase):

    def test_store_between_on_a_local(self):
        fn = _fn(_div("s", "r", tac_ast.Modulo), _store("r"),
                 _div("s", "q"), tac_ast.Ret(val=_var("q")))
        out = hoist_divmod_pairs(fn, _ctx()).instructions
        self.assertEqual(out[:2], [_div("s", "r", tac_ast.Modulo),
                                   _div("s", "q")])
        self.assertEqual(out[2], _store("r"))

    def test_branch_between_on_a_global(self):
        fn = _fn(
            tac_ast.JumpIfCmp(op=tac_ast.LessThan(), src1=_var("V"),
                              src2=_uc(24), target=".end"),
            _div("V", "Q", c=24),
            tac_ast.Label(name=".end"),
            _div("V", "R", tac_ast.Modulo, c=24),
            tac_ast.Ret(val=None),
        )
        ctx = _ctx()
        out = hoist_divmod_pairs(fn, ctx).instructions
        self.assertEqual(out[:2], [_div("V", "Q.dm0", c=24),
                                   _div("V", "R.dm0", tac_ast.Modulo, c=24)])
        self.assertIn(tac_ast.Copy(src=_var("Q.dm0"), dst=_var("Q")), out)
        self.assertIn(tac_ast.Copy(src=_var("R.dm0"), dst=_var("R")), out)
        self.assertIn("Q.dm0", ctx.ssa_dsts)

    def test_global_kept_across_a_pointer_store(self):
        fn = _fn(_div("V", "r", tac_ast.Modulo), _store("r"),
                 _div("V", "q"), tac_ast.Ret(val=_var("q")))
        self.assertEqual(hoist_divmod_pairs(fn, _ctx()), fn)

    def test_global_kept_across_a_call(self):
        fn = _fn(
            tac_ast.JumpIfCmp(op=tac_ast.LessThan(), src1=_var("V"),
                              src2=_uc(24), target=".end"),
            _div("V", "Q", c=24),
            tac_ast.FunctionCall(name="g", args=[], dst=None,
                                 hw_stack_save=False, noreturn=False),
            tac_ast.Label(name=".end"),
            _div("V", "R", tac_ast.Modulo, c=24),
            tac_ast.Ret(val=None),
        )
        self.assertEqual(hoist_divmod_pairs(fn, _ctx()), fn)

    def test_kept_when_a_path_computes_neither(self):
        # if (s < 24) { q = s / 24; if (t) r = s % 24; } -- the pair
        # would go at the block of the divide, which dominates the
        # remainder, so it's taken; with the divide on one arm and the
        # remainder on a path that may skip both, nothing moves.
        fn = _fn(
            tac_ast.JumpIfCmp(op=tac_ast.LessThan(), src1=_var("s"),
                              src2=_uc(24), target=".a"),
            tac_ast.JumpIfTrue(condition=_var("t"), target=".b"),
            tac_ast.Ret(val=None),
            tac_ast.Label(name=".b"),
            _div("s", "r", tac_ast.Modulo, c=24),
            tac_ast.Ret(val=_var("r")),
            tac_ast.Label(name=".a"),
            _div("s", "q", c=24),
            tac_ast.Ret(val=_var("q")),
        )
        self.assertEqual(hoist_divmod_pairs(fn, _ctx()), fn)

    def test_divide_by_zero_never_hoisted(self):
        fn = _fn(_div("s", "r", tac_ast.Modulo, c=0), _store("r"),
                 _div("s", "q", c=0), tac_ast.Ret(val=_var("q")))
        self.assertEqual(hoist_divmod_pairs(fn, _ctx()), fn)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestDivmodHoistSim(unittest.TestCase):

    _SRC = """
#include <stdint.h>
struct iob { uint8_t pad[4]; uint8_t track, sector; };
struct iob IOB;
struct iob *P = &IOB;
uint8_t V, Q, R, S;
uint8_t OUT[256 * 5];
void g(uint8_t s) { P->sector = s %% 13; P->track = 0x11 + s / 13; }
void f(void) { if (V >= 24) Q = V / 24; R = V %% 24; }
uint8_t h(uint8_t sel) {
    if (V >= 24)
        S = sel + V / 24;
    return V %% 24 * 4 + 0x20;
}
int main(void) {
    for (uint16_t v = 0; v < 256; v++) {
        g((uint8_t)v);
        V = (uint8_t)v;
        Q = 0xEE;
        f();
        S = 0xEE;
        uint8_t x = h(%d);
        OUT[v * 5] = IOB.sector;
        OUT[v * 5 + 1] = IOB.track;
        OUT[v * 5 + 2] = Q;
        OUT[v * 5 + 3] = R;
        OUT[v * 5 + 4] = x ^ S;
    }
    return 0;
}
"""

    def test_runs(self):
        from sim.harness import build_sim
        sel = 7
        want = []
        for v in range(256):
            q = v // 24 if v >= 24 else 0xEE
            s = (sel + v // 24) & 0xFF if v >= 24 else 0xEE
            x = (v % 24 * 4 + 0x20) & 0xFF
            want += [v % 13, (0x11 + v // 13) & 0xFF, q, v % 24, x ^ s]
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                sim = build_sim(self._SRC % sel, optimize=optimize)
                result = sim.run(max_cycles=20_000_000)
                self.assertFalse(result.timed_out)
                base = sim.symbols["OUT"]
                got = list(sim.mpu.memory[base:base + len(want)])
                self.assertEqual(got, want)


if __name__ == "__main__":
    unittest.main()
