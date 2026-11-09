"""Tests for moving a constant out of an address offset
(`passes/optimization/reassoc_offset.py`) and for hoisting loop-
invariant extends (`passes/optimization/licm.py`)."""
from __future__ import annotations

import shutil
import unittest

import c99_ast
import tac_ast
from passes.optimization.framework import PassContext
from passes.optimization.licm import hoist_loop_invariants
from passes.optimization.reassoc_offset import reassoc_offset_constant
from passes.type_checking import LocalAttr, Symbol, SymbolTable
from sim.harness import build_sim


def _var(name: str) -> tac_ast.Var:
    return tac_ast.Var(name=name)


def _ctx(types: dict[str, object]) -> PassContext:
    symbols = SymbolTable()
    for name, t in types.items():
        symbols[name] = Symbol(type=t, attrs=LocalAttr())
    return PassContext(symbols=symbols, ssa_dsts=set(types))


def _fn(instrs) -> tac_ast.Function:
    return tac_ast.Function(
        name="f", is_global=True, params=[], instructions=list(instrs),
    )


_PTR = c99_ast.Pointer(referenced_type=c99_ast.UChar())


class TestReassocOffset(unittest.TestCase):

    def _offset_chain(self, k_const):
        return [
            tac_ast.Binary(op=tac_ast.Add(), src1=_var("x"),
                           src2=tac_ast.Constant(const=k_const),
                           dst=_var("off")),
            tac_ast.Binary(op=tac_ast.Add(), src1=_var("p"),
                           src2=_var("off"), dst=_var("addr")),
            tac_ast.Load(src_ptr=_var("addr"), dst=_var("b"),
                         is_volatile=False),
        ]

    def test_constant_moves_out(self):
        ctx = _ctx({"x": c99_ast.Int(), "p": _PTR, "off": c99_ast.Int(),
                    "addr": _PTR, "b": c99_ast.UChar()})
        out = reassoc_offset_constant(
            _fn(self._offset_chain(tac_ast.ConstInt(value=3))), ctx,
        ).instructions
        self.assertEqual(out[0], tac_ast.Binary(
            op=tac_ast.Add(), src1=_var("p"), src2=_var("x"),
            dst=_var("addr.ra"),
        ))
        self.assertEqual(out[1].src1, _var("addr.ra"))
        self.assertEqual(out[1].src2.const.value, 3)
        self.assertEqual(out[1].dst, _var("addr"))
        self.assertIn("addr.ra", ctx.ssa_dsts)

    def test_multi_use_offset_left_alone(self):
        ctx = _ctx({"x": c99_ast.Int(), "p": _PTR, "off": c99_ast.Int(),
                    "addr": _PTR, "b": c99_ast.UChar(),
                    "c": c99_ast.Int()})
        instrs = self._offset_chain(tac_ast.ConstInt(value=3)) + [
            tac_ast.Copy(src=_var("off"), dst=_var("c")),
        ]
        out = reassoc_offset_constant(_fn(instrs), ctx).instructions
        self.assertEqual(out, instrs)

    def test_different_widths_left_alone(self):
        # A 4-byte offset added to a pointer isn't the same 2-byte sum.
        ctx = _ctx({"x": c99_ast.Long(), "p": _PTR, "off": c99_ast.Long(),
                    "addr": _PTR, "b": c99_ast.UChar()})
        instrs = self._offset_chain(tac_ast.ConstLong(value=3))
        out = reassoc_offset_constant(_fn(instrs), ctx).instructions
        self.assertEqual(out, instrs)


class TestHoistLoopInvariants(unittest.TestCase):

    def _loop(self):
        """`pre: n = 0; top: Phi; e = zext(c); s = s + e; if s != 9
        goto top`."""
        return [
            tac_ast.Label(name="pre"),
            tac_ast.Copy(src=tac_ast.Constant(
                const=tac_ast.ConstInt(value=0)), dst=_var("s0")),
            tac_ast.Label(name="top"),
            tac_ast.Phi(dst=_var("s1"), args=[
                tac_ast.PhiArg(pred_label="pre", source=_var("s0")),
                tac_ast.PhiArg(pred_label="top", source=_var("s2")),
            ]),
            tac_ast.ZeroExtend(src=_var("c"), dst=_var("e")),
            tac_ast.Binary(op=tac_ast.Add(), src1=_var("s1"),
                           src2=_var("e"), dst=_var("s2")),
            tac_ast.JumpIfCmp(op=tac_ast.NotEqual(), src1=_var("s2"),
                              src2=tac_ast.Constant(
                                  const=tac_ast.ConstInt(value=9)),
                              target="top"),
            tac_ast.Ret(val=_var("s2")),
        ]

    def test_extend_of_invariant_moves_to_preheader(self):
        ctx = _ctx({"c": c99_ast.UChar(), "e": c99_ast.Int(),
                    "s0": c99_ast.Int(), "s1": c99_ast.Int(),
                    "s2": c99_ast.Int()})
        out = hoist_loop_invariants(_fn(self._loop()), ctx).instructions
        ext = tac_ast.ZeroExtend(src=_var("c"), dst=_var("e"))
        self.assertEqual(out.index(ext), 2)
        self.assertLess(out.index(ext), out.index(tac_ast.Label(name="top")))

    def test_extend_of_loop_value_stays(self):
        instrs = self._loop()
        instrs[4] = tac_ast.ZeroExtend(src=_var("s1"), dst=_var("e"))
        ctx = _ctx({"c": c99_ast.UChar(), "e": c99_ast.Long(),
                    "s0": c99_ast.Int(), "s1": c99_ast.Int(),
                    "s2": c99_ast.Int()})
        out = hoist_loop_invariants(_fn(instrs), ctx).instructions
        self.assertEqual(out, instrs)

    def test_add_not_moved(self):
        instrs = self._loop()
        instrs[4] = tac_ast.Binary(
            op=tac_ast.Add(), src1=_var("c"), src2=_var("c"), dst=_var("e"),
        )
        ctx = _ctx({"c": c99_ast.Int(), "e": c99_ast.Int(),
                    "s0": c99_ast.Int(), "s1": c99_ast.Int(),
                    "s2": c99_ast.Int()})
        out = hoist_loop_invariants(_fn(instrs), ctx).instructions
        self.assertEqual(out, instrs)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestReassocLicmSim(unittest.TestCase):

    def _both(self, src: str, want: int) -> None:
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                got = build_sim(src, optimize=optimize).run().return_int()
                self.assertEqual(got, want)

    def test_header_fields_after_variable_skip(self):
        src = (
            "unsigned char t[12] = {2, 9, 9, 7, 1, 4, 0, 0, 0, 0, 0, 0};\n"
            "int f(unsigned char *d) {\n"
            "    unsigned char n = d[0];\n"
            "    return d[n + 1] * 100 + d[n + 2] * 10 + d[n + 3];\n"
            "}\n"
            "int main(void) { return f(t); }\n"
        )
        self._both(src, 714)

    def test_pointer_advanced_by_byte_in_loop(self):
        src = (
            "unsigned char t[40];\n"
            "int f(unsigned char *p, unsigned char step, unsigned char n) {\n"
            "    int sum = 0;\n"
            "    while (n--) { sum += *p; p += step; }\n"
            "    return sum;\n"
            "}\n"
            "int main(void) {\n"
            "    for (unsigned char i = 0; i < 40; i++) t[i] = i;\n"
            "    return f(t + 1, 3, 12);\n"
            "}\n"
        )
        # 1 + 4 + ... + 34, twelve terms.
        self._both(src, sum(1 + 3 * k for k in range(12)))


if __name__ == "__main__":
    unittest.main()
