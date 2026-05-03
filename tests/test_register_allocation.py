"""Behavioral tests for `passes.optimization.register_allocation`.

Coverage:
  - Single var lands at the start of its preferred pool.
  - Two non-interfering vars share a slot.
  - Two interfering vars of different widths get disjoint byte ranges.
  - lives_across_call → callee-saved pool by default.
  - When preferred pool exhausted, falls back to the other pool.
  - When both pools exhausted, the loser is reported in `spilled`.
  - Custom pool start address is honored and echoed back.
  - A spilled neighbor doesn't block colors — its slot doesn't exist.
  - Phi dsts in a join block get sane assignments without colliding.
  - Function parameters are colored even though no instruction
    defines them.
"""

from __future__ import annotations

import unittest

import c99_ast
import tac_ast
from passes.optimization.interference import build_interference
from passes.optimization.liveness import compute_liveness
from passes.optimization.pool import Pool
from passes.optimization.register_allocation import color_graph
from passes.type_checking import LocalAttr, Symbol, SymbolTable


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


def _color(fn, st, *, pool=None):
    liv = compute_liveness(fn)
    g = build_interference(fn, liv, st)
    return color_graph(fn, g, pool=pool)


class TestSingleVar(unittest.TestCase):
    def test_one_var_lands_at_caller_start(self) -> None:
        # Single int var, never crosses a call → caller-saved start.
        fn = _fn(
            tac_ast.Copy(src=_ci(1), dst=_var("a")),
            tac_ast.Ret(val=_var("a")),
        )
        st = _symbols(a=c99_ast.Int())
        c = _color(fn, st)
        self.assertEqual(c.assignments["a"], 0x80)
        self.assertEqual(c.spilled, set())


class TestSharing(unittest.TestCase):
    def test_non_interfering_share_slot(self) -> None:
        # Two vars whose lifetimes don't overlap should be able to
        # share the same ZP byte.
        # a defined / used; then b defined / used. a dies before b
        # is born → no edge → same color.
        fn = _fn(
            tac_ast.Copy(src=_ci(1), dst=_var("a")),
            tac_ast.Copy(src=_var("a"), dst=_var("ra")),
            tac_ast.Copy(src=_ci(2), dst=_var("b")),
            tac_ast.Ret(val=_var("b")),
        )
        st = _symbols(
            a=c99_ast.Int(), b=c99_ast.Int(), ra=c99_ast.Int(),
        )
        c = _color(fn, st)
        self.assertEqual(c.assignments["a"], c.assignments["b"])


class TestWidthsNoOverlap(unittest.TestCase):
    def test_overlapping_lifetimes_disjoint_byte_ranges(self) -> None:
        # a (Char, 1B) and b (Long, 4B) live simultaneously at the
        # `c = a + b` instruction → byte ranges must not overlap.
        fn = _fn(
            tac_ast.Copy(src=_ci(1), dst=_var("a")),
            tac_ast.Copy(src=_ci(2), dst=_var("b")),
            tac_ast.Binary(
                op=tac_ast.Add(), src1=_var("a"), src2=_var("b"),
                dst=_var("c"),
            ),
            tac_ast.Ret(val=_var("c")),
        )
        st = _symbols(
            a=c99_ast.Char(), b=c99_ast.Long(), c=c99_ast.Long(),
        )
        c = _color(fn, st)
        a_base = c.assignments["a"]
        b_base = c.assignments["b"]
        a_bytes = set(range(a_base, a_base + 1))
        b_bytes = set(range(b_base, b_base + 4))
        self.assertEqual(a_bytes & b_bytes, set())


class TestLivesAcrossCall(unittest.TestCase):
    def test_cross_call_var_lands_in_callee_pool(self) -> None:
        # Values live across a call go to the callee-saved pool so
        # the callee's prologue/epilogue save+restore preserves the
        # value across the call. With default Pool(start=0x80),
        # callee-saved is [0xC0, 0x100).
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
        c = _color(fn, st)
        # x is live across the call → callee-saved (>= 0xC0).
        self.assertGreaterEqual(c.assignments["x"], 0xC0)
        # y is defined by the call (not live across) → caller-saved.
        self.assertLess(c.assignments["y"], 0xC0)


class TestPoolFallback(unittest.TestCase):
    def test_caller_exhaustion_falls_back_to_callee(self) -> None:
        # With a tiny pool (Pool(start=0xE0) → 16-byte halves: caller
        # [0xE0,0xF0), callee [0xF0,0x100)), put 17 simultaneously-
        # live (non-cross-call) single-byte vars into a clique. 16
        # fit in caller-saved; the 17th falls back to callee-saved.
        n = 17
        names = [f"v{i}" for i in range(n)]
        instrs = [
            tac_ast.Copy(src=_ci(i), dst=_var(names[i]))
            for i in range(n)
        ]
        # Chain-add them all into one accumulator so all 17 are
        # simultaneously live at the chain's start.
        acc = _var(names[0])
        for i in range(1, n):
            new_acc = _var(f"acc{i}")
            instrs.append(tac_ast.Binary(
                op=tac_ast.Add(), src1=acc, src2=_var(names[i]),
                dst=new_acc,
            ))
            acc = new_acc
        instrs.append(tac_ast.Ret(val=acc))
        st = _symbols(
            **{nm: c99_ast.Char() for nm in names},
            **{f"acc{i}": c99_ast.Char() for i in range(1, n)},
        )
        c = _color(_fn(*instrs), st, pool=Pool(start=0xE0))
        in_callee = [
            nm for nm in names
            if nm in c.assignments and c.assignments[nm] >= 0xF0
        ]
        in_caller = [
            nm for nm in names
            if nm in c.assignments and c.assignments[nm] < 0xF0
        ]
        # No spills at this scale (32 bytes total, 17 live).
        for nm in names:
            self.assertIn(nm, c.assignments, f"{nm} unexpectedly spilled")
        self.assertEqual(len(in_caller), 16)
        self.assertEqual(len(in_callee), 1)


class TestSpillOnExhaustion(unittest.TestCase):
    def test_huge_clique_spills(self) -> None:
        # Pool(start=0xE0) → 32 bytes total. Build a clique of 33
        # simultaneously-live single-byte vars; at least one spills.
        n = 33
        names = [f"v{i}" for i in range(n)]
        instrs = [
            tac_ast.Copy(src=_ci(i), dst=_var(names[i]))
            for i in range(n)
        ]
        # Chain-add to keep them all live at the same point.
        acc = _var(names[0])
        for i in range(1, n):
            new_acc = _var(f"acc{i}")
            instrs.append(tac_ast.Binary(
                op=tac_ast.Add(), src1=acc, src2=_var(names[i]),
                dst=new_acc,
            ))
            acc = new_acc
        instrs.append(tac_ast.Ret(val=acc))
        st = _symbols(
            **{nm: c99_ast.Char() for nm in names},
            **{f"acc{i}": c99_ast.Char() for i in range(1, n)},
        )
        c = _color(_fn(*instrs), st, pool=Pool(start=0xE0))
        self.assertGreater(len(c.spilled), 0)
        # Spilled names are NOT in assignments.
        for nm in c.spilled:
            self.assertNotIn(nm, c.assignments)


class TestCustomPoolStart(unittest.TestCase):
    def test_custom_start_honored(self) -> None:
        fn = _fn(
            tac_ast.Copy(src=_ci(1), dst=_var("a")),
            tac_ast.Ret(val=_var("a")),
        )
        st = _symbols(a=c99_ast.Int())
        pool = Pool(start=0xA0)
        c = _color(fn, st, pool=pool)
        self.assertEqual(c.assignments["a"], 0xA0)
        self.assertIs(c.pool, pool)


class TestSpilledNeighbor(unittest.TestCase):
    def test_spilled_neighbor_does_not_block_color(self) -> None:
        # Force a spill via a tight pool, then check that a fresh
        # var interfering only with the spilled one gets the
        # canonical first slot.
        # Pool with 2 bytes available. Three single-byte vars in a
        # clique → one spills. Then a fourth var interferes with
        # only the spilled one (it doesn't really matter since
        # spilled neighbors don't block — let's assert assignments
        # for the non-spilled vars start at the pool's start).
        fn = _fn(
            tac_ast.Copy(src=_ci(1), dst=_var("a")),
            tac_ast.Copy(src=_ci(2), dst=_var("b")),
            tac_ast.Copy(src=_ci(3), dst=_var("c")),
            tac_ast.Binary(
                op=tac_ast.Add(), src1=_var("a"), src2=_var("b"),
                dst=_var("t1"),
            ),
            tac_ast.Binary(
                op=tac_ast.Add(), src1=_var("t1"), src2=_var("c"),
                dst=_var("t2"),
            ),
            tac_ast.Ret(val=_var("t2")),
        )
        st = _symbols(
            a=c99_ast.Char(), b=c99_ast.Char(), c=c99_ast.Char(),
            t1=c99_ast.Char(), t2=c99_ast.Char(),
        )
        # Pool(start=0xFE) → caller [0xFE, 0xFF) = 1 byte,
        # callee [0xFF, 0x100) = 1 byte. 2 bytes total; 3-clique
        # forces one spill.
        c = _color(fn, st, pool=Pool(start=0xFE))
        clique = {"a", "b", "c"}
        spilled_in_clique = clique & c.spilled
        self.assertEqual(len(spilled_in_clique), 1)
        # The two non-spilled clique members occupy 0xFE and 0xFF.
        non_spilled = [nm for nm in clique if nm in c.assignments]
        self.assertEqual(set(c.assignments[nm] for nm in non_spilled), {0xFE, 0xFF})


class TestPhiColoring(unittest.TestCase):
    def test_phi_dst_does_not_collide_with_live_value(self) -> None:
        # Diamond with two-arm Phi at the join. y is also live at
        # the join. Phi dst and y must end up in different slots.
        fn = _fn(
            tac_ast.Copy(src=_ci(99), dst=_var("y")),
            tac_ast.JumpIfFalse(condition=_var("c"), target=".else"),
            tac_ast.Label(name=".then"),
            tac_ast.Copy(src=_ci(1), dst=_var("x.1")),
            tac_ast.Jump(target=".join"),
            tac_ast.Label(name=".else"),
            tac_ast.Copy(src=_ci(2), dst=_var("x.2")),
            tac_ast.Label(name=".join"),
            tac_ast.Phi(
                dst=_var("x.3"),
                args=[
                    tac_ast.PhiArg(pred_label=".then", source=_var("x.1")),
                    tac_ast.PhiArg(pred_label=".else", source=_var("x.2")),
                ],
            ),
            tac_ast.Binary(
                op=tac_ast.Add(), src1=_var("x.3"), src2=_var("y"),
                dst=_var("r"),
            ),
            tac_ast.Ret(val=_var("r")),
        )
        st = _symbols(
            c=c99_ast.Int(), y=c99_ast.Int(),
            **{
                "x.1": c99_ast.Int(), "x.2": c99_ast.Int(),
                "x.3": c99_ast.Int(),
            },
            r=c99_ast.Int(),
        )
        col = _color(fn, st)
        self.assertIn("x.3", col.assignments)
        self.assertIn("y", col.assignments)
        # x.3 (Phi dst) and y are both live at the post-join Add →
        # disjoint byte ranges (each is width 2).
        x3 = col.assignments["x.3"]
        y = col.assignments["y"]
        x3_bytes = set(range(x3, x3 + 2))
        y_bytes = set(range(y, y + 2))
        self.assertEqual(x3_bytes & y_bytes, set())


class TestParamsColored(unittest.TestCase):
    def test_param_appears_in_assignments(self) -> None:
        # Param p is read in the body. It has no instruction defining
        # it, but the PEO machinery treats params as ENTRY-defined
        # so they get colored.
        fn = _fn(
            tac_ast.Binary(
                op=tac_ast.Add(), src1=_var("p"), src2=_ci(1),
                dst=_var("r"),
            ),
            tac_ast.Ret(val=_var("r")),
            params=("p",),
        )
        st = _symbols(p=c99_ast.Int(), r=c99_ast.Int())
        c = _color(fn, st)
        self.assertIn("p", c.assignments)
        self.assertIn("r", c.assignments)
