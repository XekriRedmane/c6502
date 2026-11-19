"""Tests for `passes.optimization.read_stored_global`, which reads a
global just stored from a temporary in place of the temporary, and runs
of `*++P` in both pipelines."""

from __future__ import annotations

import shutil
import unittest

import c99_ast
import tac_ast
from passes.optimization.framework import PassContext
from passes.optimization.read_stored_global import read_stored_globals
from passes.type_checking import LocalAttr, StaticAttr, Symbol, SymbolTable


def _var(name):
    return tac_ast.Var(name=name)


_PTR = c99_ast.Pointer(referenced_type=c99_ast.UChar())


def _ctx(volatile=False):
    symbols = SymbolTable()
    for name in ("t", "v"):
        symbols[name] = Symbol(type=_PTR if name == "t" else c99_ast.UChar(),
                               attrs=LocalAttr())
    ptype = c99_ast.Pointer(referenced_type=c99_ast.Const(
        referenced_type=c99_ast.UChar()))
    if volatile:
        ptype = c99_ast.Volatile(referenced_type=ptype)
    symbols["P"] = Symbol(type=ptype, attrs=StaticAttr(
        initial_value=None, is_global=True))
    return PassContext(symbols=symbols, ssa_dsts={"t", "v"})


def _fn(*between, use=None):
    return tac_ast.Function(name="f", is_global=True, params=[],
                            instructions=[
        tac_ast.Binary(op=tac_ast.Add(), src1=_var("P"),
                       src2=tac_ast.Constant(const=tac_ast.ConstInt(value=1)),
                       dst=_var("t")),
        tac_ast.Copy(src=_var("t"), dst=_var("P")),
        *between,
        use or tac_ast.Load(src_ptr=_var("t"), dst=_var("v"),
                            is_volatile=False),
        tac_ast.Ret(val=_var("v")),
    ])


class TestReadStoredGlobals(unittest.TestCase):

    def test_load_through_the_stored_pointer(self):
        out = read_stored_globals(_fn(), _ctx()).instructions
        self.assertEqual(out[2], tac_ast.Load(
            src_ptr=_var("P"), dst=_var("v"), is_volatile=False))

    def test_kept_across_a_call(self):
        fn = _fn(tac_ast.FunctionCall(name="g", args=[], dst=None,
                                      hw_stack_save=False, noreturn=False))
        self.assertEqual(read_stored_globals(fn, _ctx()), fn)

    def test_kept_across_a_store(self):
        fn = _fn(tac_ast.Store(src=_var("v"), dst_ptr=_var("t"),
                               is_volatile=False))
        self.assertEqual(read_stored_globals(fn, _ctx()), fn)

    def test_kept_for_a_volatile_global(self):
        fn = _fn()
        self.assertEqual(read_stored_globals(fn, _ctx(volatile=True)), fn)

    def test_kept_when_stored_through_itself(self):
        # `*t = t` reads t as a value too.
        fn = _fn(use=tac_ast.Store(src=_var("t"), dst_ptr=_var("t"),
                                   is_volatile=False))
        self.assertEqual(read_stored_globals(fn, _ctx()), fn)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestReadStoredGlobalSim(unittest.TestCase):

    _SRC = """
#include <stdint.h>
const uint8_t T[] = {3, 1, 4, 1, 5, 9, 2, 6};
extern const uint8_t *P [[c6502::zeropage(0xEC)]];
const uint8_t *Q;
uint8_t next(void) { return *++P; }
uint8_t nextq(void) { return *++Q + *Q; }
int main(void) {
    P = T; Q = T;
    uint16_t sum = 0;
    for (uint8_t k = 0; k < 7; k++)
        sum = sum * 3 + next() + nextq();
    return sum + (P - T) * 1000;
}
"""

    def test_runs(self):
        from sim.harness import build_sim
        want, p = 0, 0
        t = [3, 1, 4, 1, 5, 9, 2, 6]
        for _ in range(7):
            p += 1
            want = (want * 3 + t[p] + 2 * t[p]) & 0xFFFF
        want = (want + p * 1000) & 0xFFFF
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                out = build_sim(self._SRC, optimize=optimize).run()
                self.assertEqual(out.return_int() & 0xFFFF, want)


if __name__ == "__main__":
    unittest.main()
