"""Behavioral tests for `passes.zp_local_allocation`.

Coverage:
  - Single eligible function → gets a private pool sized to
    `local_bytes`.
  - Caller-callee chain → disjoint local pools.
  - Sibling functions → may share addresses.
  - Diamond pattern → leaf disjoint from both intermediates.
  - Local pool avoids coexisting zp_abi param slots (ancestor
    AND descendant).
  - Ineligible: function with IndirectCall.
  - Ineligible: function in a cycle.
  - Ineligible: function calling a non-zp_abi extern.
  - Ineligible-callee propagation: caller of ineligible →
    ineligible.
  - zp_abi extern callee allowed (treated as bounded leaf).
  - Zero local_bytes → empty pool returned (function still
    listed as eligible).
"""
from __future__ import annotations

import unittest

import tac_ast
from passes.abi_selection import SoftStackLayout, ZpLayout
from passes.zp_local_allocation import (
    ZpLocalAllocationError, allocate_function_locals,
)


def _fn(name: str, *instrs) -> tac_ast.Function:
    return tac_ast.Function(
        name=name, is_global=True, params=[],
        instructions=list(instrs),
    )


def _call(name: str) -> tac_ast.FunctionCall:
    return tac_ast.FunctionCall(name=name, args=[], dst=None)


def _icall(ptr: tac_ast.Type_val) -> tac_ast.IndirectCall:
    return tac_ast.IndirectCall(ptr=ptr, args=[], dst=None)


def _zp_layout(*addrs: int) -> ZpLayout:
    return ZpLayout(
        slot_symbols=[f"__zpabi_p{k}" for k in range(len(addrs))],
        addrs=list(addrs),
    )


def _prog(*tls) -> tac_ast.Program:
    return tac_ast.Program(top_level=list(tls))


class TestEligibilityAndAllocation(unittest.TestCase):
    def test_single_eligible_function(self) -> None:
        prog = _prog(_fn("f"))
        out = allocate_function_locals(prog, {}, {"f": 3})
        self.assertIn("f", out)
        self.assertEqual(len(out["f"]), 3)
        # Lowest addresses in the caller-saved window.
        self.assertEqual(out["f"], [0x80, 0x81, 0x82])

    def test_caller_callee_disjoint(self) -> None:
        # caller (2 bytes) calls callee (1 byte). Pools disjoint.
        prog = _prog(
            _fn("caller", _call("callee")),
            _fn("callee"),
        )
        out = allocate_function_locals(
            prog, {}, {"caller": 2, "callee": 1},
        )
        self.assertEqual(out["caller"], [0x80, 0x81])
        self.assertEqual(out["callee"], [0x82])

    def test_siblings_share(self) -> None:
        # main → left, main → right. left and right are siblings
        # (neither calls the other). They can share the same byte
        # range.
        prog = _prog(
            _fn("main", _call("left"), _call("right")),
            _fn("left"),
            _fn("right"),
        )
        out = allocate_function_locals(
            prog, {}, {"main": 1, "left": 2, "right": 2},
        )
        self.assertEqual(out["main"], [0x80])
        self.assertEqual(out["left"], [0x81, 0x82])
        self.assertEqual(out["right"], [0x81, 0x82])

    def test_diamond_leaf_disjoint_from_intermediates(self) -> None:
        # root → mid_a, root → mid_b. Both mid_a and mid_b call
        # leaf. leaf must be disjoint from root, mid_a, AND mid_b.
        prog = _prog(
            _fn("root", _call("mid_a"), _call("mid_b")),
            _fn("mid_a", _call("leaf")),
            _fn("mid_b", _call("leaf")),
            _fn("leaf"),
        )
        out = allocate_function_locals(
            prog, {}, {"root": 1, "mid_a": 1, "mid_b": 1, "leaf": 1},
        )
        leaf = out["leaf"][0]
        self.assertNotIn(leaf, out["root"])
        self.assertNotIn(leaf, out["mid_a"])
        self.assertNotIn(leaf, out["mid_b"])

    def test_local_pool_avoids_coexisting_zp_abi_params(self) -> None:
        # main → zp_callee. zp_callee is a zp_abi function with
        # params at $80..$81 (allocated by zp_slot_allocation
        # beforehand). main's local pool must avoid those.
        prog = _prog(
            _fn("main", _call("zp_callee")),
            _fn("zp_callee"),
        )
        abi = {"zp_callee": _zp_layout(0x80, 0x81)}
        out = allocate_function_locals(
            prog, abi, {"main": 2, "zp_callee": 0},
        )
        # main's locals start ABOVE $80..$81.
        self.assertEqual(out["main"], [0x82, 0x83])

    def test_zp_abi_own_params_avoided_by_own_locals(self) -> None:
        # An eligible zp_abi function must keep its own body
        # locals disjoint from its own param slots.
        prog = _prog(_fn("f"))
        abi = {"f": _zp_layout(0x80, 0x81)}
        out = allocate_function_locals(prog, abi, {"f": 2})
        # Locals can't overlap with params $80..$81.
        self.assertEqual(out["f"], [0x82, 0x83])


class TestIneligibility(unittest.TestCase):
    def test_indirect_call_makes_ineligible(self) -> None:
        ptr = tac_ast.Var(name="fp")
        prog = _prog(_fn("f", _icall(ptr)))
        out = allocate_function_locals(prog, {}, {"f": 1})
        # No private pool for f.
        self.assertNotIn("f", out)

    def test_self_recursion_makes_ineligible(self) -> None:
        prog = _prog(_fn("f", _call("f")))
        out = allocate_function_locals(prog, {}, {"f": 1})
        self.assertNotIn("f", out)

    def test_mutual_recursion_makes_both_ineligible(self) -> None:
        prog = _prog(
            _fn("a", _call("b")),
            _fn("b", _call("a")),
        )
        out = allocate_function_locals(prog, {}, {"a": 1, "b": 1})
        self.assertNotIn("a", out)
        self.assertNotIn("b", out)

    def test_caller_of_recursive_is_ineligible(self) -> None:
        # main → rec, rec → rec. main is eligible's prerequisite
        # fails on the rec callee being ineligible.
        prog = _prog(
            _fn("main", _call("rec")),
            _fn("rec", _call("rec")),
        )
        out = allocate_function_locals(prog, {}, {"main": 1, "rec": 1})
        self.assertNotIn("main", out)
        self.assertNotIn("rec", out)

    def test_non_zp_abi_extern_callee_disqualifies(self) -> None:
        # main calls "puts" which isn't defined in this prog and
        # isn't in the abi. main becomes ineligible.
        prog = _prog(_fn("main", _call("puts")))
        out = allocate_function_locals(prog, {}, {"main": 1})
        self.assertNotIn("main", out)

    def test_zp_abi_extern_callee_allowed(self) -> None:
        # main calls "helper" which is declared zp_abi but not
        # defined here. Allowed — treated as a leaf with bounded
        # write set (its declared param slots).
        prog = _prog(_fn("main", _call("helper")))
        abi = {"helper": _zp_layout(0x82, 0x83)}
        out = allocate_function_locals(
            prog, abi, {"main": 2},
        )
        # main IS eligible; its pool avoids helper's params.
        self.assertIn("main", out)
        self.assertEqual(out["main"], [0x80, 0x81])

    def test_softstack_extern_callee_disqualifies(self) -> None:
        # If the extern has SoftStackLayout in the abi (or no
        # entry), it's treated as unbounded.
        prog = _prog(_fn("main", _call("legacy")))
        abi = {"legacy": SoftStackLayout()}
        out = allocate_function_locals(prog, abi, {"main": 1})
        self.assertNotIn("main", out)


class TestZeroAndEmpty(unittest.TestCase):
    def test_zero_local_bytes_returns_empty_list(self) -> None:
        prog = _prog(_fn("f"))
        out = allocate_function_locals(prog, {}, {"f": 0})
        self.assertEqual(out["f"], [])

    def test_missing_from_local_bytes_treated_as_zero(self) -> None:
        prog = _prog(_fn("f"))
        out = allocate_function_locals(prog, {}, {})
        # Eligible but empty.
        self.assertEqual(out.get("f"), [])


class TestSpill(unittest.TestCase):
    def test_chain_exhausts_zp_then_spills(self) -> None:
        # 33-deep chain of 2-byte functions exhausts the 64-byte
        # caller-saved window. The 33rd function spills above $FF.
        depth = 33
        top = []
        for i in range(depth):
            if i + 1 < depth:
                top.append(_fn(f"f{i}", _call(f"f{i + 1}")))
            else:
                top.append(_fn(f"f{i}"))
        prog = tac_ast.Program(top_level=top)
        local_bytes = {f"f{i}": 2 for i in range(depth)}
        out = allocate_function_locals(prog, {}, local_bytes)
        self.assertEqual(out["f0"], [0x80, 0x81])
        self.assertEqual(out["f31"], [0xBE, 0xBF])
        # f32 spills into the fallback region.
        self.assertGreaterEqual(out["f32"][0], 0x0200)


if __name__ == "__main__":
    unittest.main()
