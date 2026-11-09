"""Tests for common-subexpression elimination
(`passes/optimization/cse.py`).

The unit tests build SSA-form functions directly and check which
instructions become Copies. The sim tests compile C whose repeated
subscripts and tests go through the pass, in both pipelines.
"""
from __future__ import annotations

import shutil
import unittest

import c99_ast
import tac_ast
from passes.optimization.cse import eliminate_common_subexpressions
from passes.optimization.framework import PassContext
from passes.type_checking import LocalAttr, Symbol, SymbolTable
from sim.harness import build_sim


def _var(name: str) -> tac_ast.Var:
    return tac_ast.Var(name=name)


def _uint(v: int) -> tac_ast.Constant:
    return tac_ast.Constant(const=tac_ast.ConstUInt(value=v))


def _run(instrs, types: dict[str, object]):
    """Runs the pass on `instrs`, with every name in `types` an SSA
    local of the given c99 type. Returns the new instruction list."""
    symbols = SymbolTable()
    for name, t in types.items():
        symbols[name] = Symbol(type=t, attrs=LocalAttr())
    fn = tac_ast.Function(
        name="f", is_global=True, params=[], instructions=list(instrs),
    )
    ctx = PassContext(symbols=symbols, ssa_dsts=set(types))
    return eliminate_common_subexpressions(fn, ctx).instructions


def _add(a, b, dst) -> tac_ast.Binary:
    return tac_ast.Binary(op=tac_ast.Add(), src1=a, src2=b, dst=_var(dst))


class TestCseUnit(unittest.TestCase):

    def test_repeated_add_becomes_copy(self) -> None:
        out = _run(
            [_add(_var("a"), _uint(1), "x"), _add(_var("a"), _uint(1), "y"),
             tac_ast.Ret(val=_var("y"))],
            {"a": c99_ast.UInt(), "x": c99_ast.UInt(), "y": c99_ast.UInt()},
        )
        self.assertEqual(out[1], tac_ast.Copy(src=_var("x"), dst=_var("y")))

    def test_commutative_operands_match(self) -> None:
        out = _run(
            [_add(_var("a"), _var("b"), "x"), _add(_var("b"), _var("a"), "y"),
             tac_ast.Ret(val=_var("y"))],
            {n: c99_ast.UInt() for n in ("a", "b", "x", "y")},
        )
        self.assertIsInstance(out[1], tac_ast.Copy)

    def test_subtract_operand_order_kept(self) -> None:
        sub = lambda a, b, d: tac_ast.Binary(
            op=tac_ast.Subtract(), src1=a, src2=b, dst=_var(d),
        )
        instrs = [sub(_var("a"), _var("b"), "x"),
                  sub(_var("b"), _var("a"), "y"), tac_ast.Ret(val=_var("y"))]
        out = _run(instrs, {n: c99_ast.UInt() for n in ("a", "b", "x", "y")})
        self.assertEqual(out, instrs)

    def test_double_constants_distinguished(self) -> None:
        # ConstDouble keeps its value in `bits`. d - 1.0 and d - 2.0
        # once matched, which broke a chapter_13 program.
        def dsub(bits, dst):
            return tac_ast.Binary(
                op=tac_ast.Subtract(), src1=_var("d"),
                src2=tac_ast.Constant(const=tac_ast.ConstDouble(bits=bits)),
                dst=_var(dst),
            )
        instrs = [dsub(0x3FF0000000000000, "x"),
                  dsub(0x4000000000000000, "y"), tac_ast.Ret(val=_var("y"))]
        out = _run(instrs, {n: c99_ast.Double() for n in ("d", "x", "y")})
        self.assertEqual(out, instrs)

    def test_extend_to_different_widths_kept(self) -> None:
        instrs = [
            tac_ast.ZeroExtend(src=_var("c"), dst=_var("x")),
            tac_ast.ZeroExtend(src=_var("c"), dst=_var("y")),
            tac_ast.Ret(val=None),
        ]
        out = _run(instrs, {"c": c99_ast.UChar(), "x": c99_ast.UInt(),
                            "y": c99_ast.ULong()})
        self.assertEqual(out, instrs)

    def test_load_reused_without_store_between(self) -> None:
        load = lambda d: tac_ast.Load(
            src_ptr=_var("p"), dst=_var(d), is_volatile=False,
        )
        out = _run(
            [load("x"), load("y"), tac_ast.Ret(val=_var("y"))],
            {"p": c99_ast.Pointer(referenced_type=c99_ast.UChar()),
             "x": c99_ast.UChar(), "y": c99_ast.UChar()},
        )
        self.assertEqual(out[1], tac_ast.Copy(src=_var("x"), dst=_var("y")))

    def test_load_not_reused_across_store(self) -> None:
        load = lambda d: tac_ast.Load(
            src_ptr=_var("p"), dst=_var(d), is_volatile=False,
        )
        instrs = [
            load("x"),
            tac_ast.Store(src=_var("x"), dst_ptr=_var("q"), is_volatile=False),
            load("y"), tac_ast.Ret(val=_var("y")),
        ]
        ptr = c99_ast.Pointer(referenced_type=c99_ast.UChar())
        out = _run(instrs, {"p": ptr, "q": ptr, "x": c99_ast.UChar(),
                            "y": c99_ast.UChar()})
        self.assertEqual(out, instrs)

    def test_load_not_reused_across_call(self) -> None:
        instrs = [
            tac_ast.IndexedLoad(name="g", index=_var("i"), dst=_var("x"),
                                is_volatile=False),
            tac_ast.FunctionCall(name="h", args=[], dst=None),
            tac_ast.IndexedLoad(name="g", index=_var("i"), dst=_var("y"),
                                is_volatile=False),
            tac_ast.Ret(val=_var("y")),
        ]
        out = _run(instrs, {"i": c99_ast.UChar(), "x": c99_ast.UChar(),
                            "y": c99_ast.UChar()})
        self.assertEqual(out, instrs)

    def test_volatile_load_not_reused(self) -> None:
        load = lambda d: tac_ast.Load(
            src_ptr=_var("p"), dst=_var(d), is_volatile=True,
        )
        instrs = [load("x"), load("y"), tac_ast.Ret(val=_var("y"))]
        out = _run(instrs, {
            "p": c99_ast.Pointer(referenced_type=c99_ast.UChar()),
            "x": c99_ast.UChar(), "y": c99_ast.UChar(),
        })
        self.assertEqual(out, instrs)

    def test_load_not_carried_into_join(self) -> None:
        # The load in L's block has two predecessors, one of which
        # stores, so the earlier load isn't available there.
        ptr = c99_ast.Pointer(referenced_type=c99_ast.UChar())
        load = lambda d: tac_ast.Load(
            src_ptr=_var("p"), dst=_var(d), is_volatile=False,
        )
        instrs = [
            load("x"),
            tac_ast.JumpIfTrue(condition=_var("c"), target="L"),
            tac_ast.Store(src=_var("x"), dst_ptr=_var("p"), is_volatile=False),
            tac_ast.Label(name="L"),
            load("y"),
            tac_ast.Ret(val=_var("y")),
        ]
        out = _run(instrs, {"p": ptr, "c": c99_ast.UChar(),
                            "x": c99_ast.UChar(), "y": c99_ast.UChar()})
        self.assertEqual(out, instrs)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestCseSim(unittest.TestCase):

    def _both(self, src: str, want: int) -> None:
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                got = build_sim(src, optimize=optimize).run().return_int()
                self.assertEqual(got, want)

    def test_repeated_subscripts_and_tests(self) -> None:
        src = (
            "unsigned char t[8] = {9, 3, 3, 7, 1, 2, 5, 4};\n"
            "unsigned char *p = t;\n"
            "int cmp(unsigned char k, unsigned char n) {\n"
            "    if (k < p[n + 1]) return -1;\n"
            "    if (k == p[n + 1]) return p[n + 2] + p[n + 3];\n"
            "    return 1;\n"
            "}\n"
            "int main(void) {\n"
            "    return cmp(1, 0) * 100 + cmp(3, 0) + cmp(9, 4) * 1000;\n"
            "}\n"
        )
        # cmp(1, 0): 1 < 3, -1. cmp(3, 0): 3 + 7 = 10. cmp(9, 4):
        # 9 > 2, 1.
        self._both(src, -100 + 10 + 1000)

    def test_store_between_loads(self) -> None:
        src = (
            "unsigned char t[4] = {1, 2, 3, 4};\n"
            "int f(unsigned char *p) {\n"
            "    int a = p[1];\n"
            "    p[1] = 9;\n"
            "    return a * 10 + p[1];\n"
            "}\n"
            "int main(void) { return f(t); }\n"
        )
        self._both(src, 29)

    def test_doubles(self) -> None:
        src = (
            "int f(double d) {\n"
            "    if (d - 1 != 5) return 1;\n"
            "    if (d - 2 != 4) return 2;\n"
            "    return 0;\n"
            "}\n"
            "int main(void) { return f(6.0); }\n"
        )
        self._both(src, 0)


if __name__ == "__main__":
    unittest.main()
