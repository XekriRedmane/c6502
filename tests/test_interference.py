"""Behavioral tests for `passes.optimization.interference`.

Coverage:
  - Disjoint lifetimes → no edge.
  - Diamond merge: two distinct values simultaneously live → edge.
  - Widths drawn from the symbol table (1 / 2 / 4 / 8 bytes).
  - `lives_across_call` set for values live across a FunctionCall.
  - A call's own dst is not flagged as living across itself.
  - Sibling Phi dsts in the same block all interfere.
  - Non-LocalAttr names (statics, functions) are excluded from the
    graph.
  - Void-returning calls with no dst don't introduce a spurious node
    or crash.
"""

from __future__ import annotations

import unittest

import c99_ast
import tac_ast
from passes.optimization.interference import build_interference
from passes.optimization.liveness import compute_liveness
from passes.type_checking import (
    Initial,
    LocalAttr,
    StaticAttr,
    Symbol,
    SymbolTable,
)


def _ci(v: int) -> tac_ast.Constant:
    return tac_ast.Constant(const=tac_ast.ConstInt(value=v))


def _var(name: str) -> tac_ast.Var:
    return tac_ast.Var(name=name)


def _fn(*instrs, name: str = "main", params=()) -> tac_ast.Function:
    return tac_ast.Function(
        name=name, is_global=True,
        params=list(params), instructions=list(instrs),
    )


def _symbols(**kinds: c99_ast.Type_data_type) -> SymbolTable:
    st = SymbolTable()
    for name, t in kinds.items():
        st[name] = Symbol(type=t, attrs=LocalAttr())
    return st


def _build(fn, symbols):
    liv = compute_liveness(fn)
    return build_interference(fn, liv, symbols)


class TestBasicInterference(unittest.TestCase):
    def test_disjoint_lifetimes_no_edge(self) -> None:
        # x = 1; ret x; (separately) y = 2; ret y — separate code
        # paths, but inline as: a = 1; b = a + 1; ret b. `a` and `b`
        # are NOT simultaneously live (a dies as it's used to define
        # b), so no edge.
        fn = _fn(
            tac_ast.Copy(src=_ci(1), dst=_var("a")),
            tac_ast.Binary(
                op=tac_ast.Add(), src1=_var("a"), src2=_ci(1),
                dst=_var("b"),
            ),
            tac_ast.Ret(val=_var("b")),
        )
        st = _symbols(a=c99_ast.Int(), b=c99_ast.Int())
        g = _build(fn, st)
        self.assertIn("a", g.nodes)
        self.assertIn("b", g.nodes)
        self.assertFalse(g.has_edge("a", "b"))

    def test_overlapping_lifetimes_edge(self) -> None:
        # a = 1; b = 2; c = a + b; ret c — a and b are both live at
        # the moment c is being computed.
        fn = _fn(
            tac_ast.Copy(src=_ci(1), dst=_var("a")),
            tac_ast.Copy(src=_ci(2), dst=_var("b")),
            tac_ast.Binary(
                op=tac_ast.Add(), src1=_var("a"), src2=_var("b"),
                dst=_var("c"),
            ),
            tac_ast.Ret(val=_var("c")),
        )
        st = _symbols(a=c99_ast.Int(), b=c99_ast.Int(), c=c99_ast.Int())
        g = _build(fn, st)
        self.assertTrue(g.has_edge("a", "b"))


class TestDiamondInterference(unittest.TestCase):
    def test_two_values_live_at_merge(self) -> None:
        # if c jump .else;
        # .then: x = 1; jump .join;
        # .else: x = 2;
        # .join: y = 3; ret x + y;  — x and y both live at ret.
        fn = _fn(
            tac_ast.JumpIfFalse(condition=_var("c"), target=".else"),
            tac_ast.Copy(src=_ci(1), dst=_var("x")),
            tac_ast.Jump(target=".join"),
            tac_ast.Label(name=".else"),
            tac_ast.Copy(src=_ci(2), dst=_var("x")),
            tac_ast.Label(name=".join"),
            tac_ast.Copy(src=_ci(3), dst=_var("y")),
            tac_ast.Binary(
                op=tac_ast.Add(), src1=_var("x"), src2=_var("y"),
                dst=_var("z"),
            ),
            tac_ast.Ret(val=_var("z")),
        )
        st = _symbols(
            c=c99_ast.Int(), x=c99_ast.Int(),
            y=c99_ast.Int(), z=c99_ast.Int(),
        )
        g = _build(fn, st)
        self.assertTrue(g.has_edge("x", "y"))


class TestWidths(unittest.TestCase):
    def test_widths_from_symbol_table(self) -> None:
        # One Char (1B), one Int (2B), one Long (4B), one LongLong (8B).
        # Each is defined and used so each has a node.
        fn = _fn(
            tac_ast.Copy(src=_ci(1), dst=_var("c")),
            tac_ast.Copy(src=_ci(2), dst=_var("i")),
            tac_ast.Copy(src=_ci(3), dst=_var("l")),
            tac_ast.Copy(src=_ci(4), dst=_var("ll")),
            # Use them all in a single arithmetic expression so they
            # all become live.
            tac_ast.Binary(
                op=tac_ast.Add(), src1=_var("c"), src2=_var("i"),
                dst=_var("t1"),
            ),
            tac_ast.Binary(
                op=tac_ast.Add(), src1=_var("t1"), src2=_var("l"),
                dst=_var("t2"),
            ),
            tac_ast.Binary(
                op=tac_ast.Add(), src1=_var("t2"), src2=_var("ll"),
                dst=_var("t3"),
            ),
            tac_ast.Ret(val=_var("t3")),
        )
        st = _symbols(
            c=c99_ast.Char(), i=c99_ast.Int(),
            l=c99_ast.Long(), ll=c99_ast.LongLong(),
            t1=c99_ast.Int(), t2=c99_ast.Long(), t3=c99_ast.LongLong(),
        )
        g = _build(fn, st)
        self.assertEqual(g.nodes["c"].width, 1)
        self.assertEqual(g.nodes["i"].width, 2)
        self.assertEqual(g.nodes["l"].width, 4)
        self.assertEqual(g.nodes["ll"].width, 8)


class TestLivesAcrossCall(unittest.TestCase):
    def test_value_live_across_call_flagged(self) -> None:
        # x = 1; y = call f(); ret x + y. `x` is live across the call,
        # `y` is defined by the call (not live across).
        fn = _fn(
            tac_ast.Copy(src=_ci(1), dst=_var("x")),
            tac_ast.FunctionCall(name="f", args=[], dst=_var("y")),
            tac_ast.Binary(
                op=tac_ast.Add(), src1=_var("x"), src2=_var("y"),
                dst=_var("r"),
            ),
            tac_ast.Ret(val=_var("r")),
        )
        st = _symbols(
            x=c99_ast.Int(), y=c99_ast.Int(), r=c99_ast.Int(),
        )
        g = _build(fn, st)
        self.assertTrue(g.nodes["x"].lives_across_call)
        self.assertFalse(g.nodes["y"].lives_across_call)

    def test_call_without_live_across_flags_nothing(self) -> None:
        # A simple call with no surrounding live values.
        fn = _fn(
            tac_ast.FunctionCall(name="f", args=[], dst=_var("y")),
            tac_ast.Ret(val=_var("y")),
        )
        st = _symbols(y=c99_ast.Int())
        g = _build(fn, st)
        self.assertFalse(g.nodes["y"].lives_across_call)


class TestPhiInterference(unittest.TestCase):
    def test_sibling_phi_dsts_interfere(self) -> None:
        # Two Phis at the same block:
        #   .join:  x.3 = phi(...); y.3 = phi(...); ret x.3 + y.3
        fn = _fn(
            tac_ast.Label(name=".entry"),
            tac_ast.JumpIfFalse(condition=_var("c"), target=".else"),
            tac_ast.Label(name=".then"),
            tac_ast.Copy(src=_ci(1), dst=_var("x.1")),
            tac_ast.Copy(src=_ci(10), dst=_var("y.1")),
            tac_ast.Jump(target=".join"),
            tac_ast.Label(name=".else"),
            tac_ast.Copy(src=_ci(2), dst=_var("x.2")),
            tac_ast.Copy(src=_ci(20), dst=_var("y.2")),
            tac_ast.Label(name=".join"),
            tac_ast.Phi(
                dst=_var("x.3"),
                args=[
                    tac_ast.PhiArg(pred_label=".then", source=_var("x.1")),
                    tac_ast.PhiArg(pred_label=".else", source=_var("x.2")),
                ],
            ),
            tac_ast.Phi(
                dst=_var("y.3"),
                args=[
                    tac_ast.PhiArg(pred_label=".then", source=_var("y.1")),
                    tac_ast.PhiArg(pred_label=".else", source=_var("y.2")),
                ],
            ),
            tac_ast.Binary(
                op=tac_ast.Add(), src1=_var("x.3"), src2=_var("y.3"),
                dst=_var("r"),
            ),
            tac_ast.Ret(val=_var("r")),
        )
        st = _symbols(
            c=c99_ast.Int(),
            **{
                "x.1": c99_ast.Int(), "x.2": c99_ast.Int(),
                "x.3": c99_ast.Int(),
                "y.1": c99_ast.Int(), "y.2": c99_ast.Int(),
                "y.3": c99_ast.Int(),
            },
            r=c99_ast.Int(),
        )
        g = _build(fn, st)
        self.assertTrue(g.has_edge("x.3", "y.3"))


class TestExclusions(unittest.TestCase):
    def test_static_not_in_graph(self) -> None:
        # A static variable referenced as a TAC Var should be excluded
        # from the graph (regalloc doesn't color it).
        fn = _fn(
            tac_ast.Copy(src=_ci(7), dst=_var("g")),
            tac_ast.Copy(src=_var("g"), dst=_var("local")),
            tac_ast.Ret(val=_var("local")),
        )
        st = SymbolTable()
        st["g"] = Symbol(
            type=c99_ast.Int(),
            attrs=StaticAttr(initial_value=Initial(value=0), is_global=True),
        )
        st["local"] = Symbol(type=c99_ast.Int(), attrs=LocalAttr())
        g = _build(fn, st)
        self.assertNotIn("g", g.nodes)
        self.assertIn("local", g.nodes)

    def test_void_call_no_dst_no_node(self) -> None:
        # call f();  — no dst, just a side-effecting call.
        fn = _fn(
            tac_ast.FunctionCall(name="f", args=[], dst=None),
            tac_ast.Ret(val=_ci(0)),
        )
        st = _symbols()
        g = _build(fn, st)
        # No spurious nodes; no crash.
        self.assertEqual(g.nodes, {})

    def test_unknown_var_defaults_to_width_one(self) -> None:
        # Var not in the symbol table — keep with width 1 (synthetic
        # test fixture backstop).
        fn = _fn(
            tac_ast.Copy(src=_ci(1), dst=_var("mystery")),
            tac_ast.Ret(val=_var("mystery")),
        )
        st = SymbolTable()  # empty
        g = _build(fn, st)
        self.assertIn("mystery", g.nodes)
        self.assertEqual(g.nodes["mystery"].width, 1)
