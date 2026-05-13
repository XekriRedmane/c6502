"""Behavioral tests for `passes.zp_slot_allocation`.

Coverage:
  - Caller-callee chain → disjoint slot ranges.
  - Sibling functions (no caller-callee relation) → may reuse
    the same range.
  - Diamond pattern (two intermediate callers of one leaf) →
    leaf disjoint from both intermediates AND from the root.
  - Non-zp_abi functions in the program are ignored.
  - Each ZpLayout entry's `slot_symbols` is preserved on the
    way through; `addrs` is rewritten by the allocator.
  - `sym_to_addr` dict has one entry per slot symbol with the
    matching numeric address.
  - Spill to non-ZP region when the ZP window saturates.
"""
from __future__ import annotations

import unittest

import tac_ast
from passes.abi_selection import SoftStackLayout, ZpLayout
from passes.optimization.pool import Pool
from passes.zp_slot_allocation import (
    ZpSlotAllocationError, allocate_zp_slots,
)


def _fn(name: str, *instrs) -> tac_ast.Function:
    return tac_ast.Function(
        name=name, is_global=True, params=[],
        instructions=list(instrs),
    )


def _call(name: str) -> tac_ast.FunctionCall:
    return tac_ast.FunctionCall(name=name, args=[], dst=None)


def _zp(name: str, n_bytes: int) -> ZpLayout:
    """Construct a ZpLayout with placeholder addrs — the
    allocator will overwrite them. Slot symbols follow the
    `__zpabi_<name>_p<k>` convention used by `select_abi`."""
    return ZpLayout(
        slot_symbols=[f"__zpabi_{name}_p{k}" for k in range(n_bytes)],
        addrs=[0x80 + k for k in range(n_bytes)],
    )


class TestZpSlotAllocation(unittest.TestCase):
    def test_caller_callee_chain_disjoint(self) -> None:
        # caller (4 bytes) → callee (2 bytes). Slots must not
        # overlap: caller at $80..$83, callee pushed to $84..$85.
        prog = tac_ast.Program(top_level=[
            _fn("caller", _call("callee")),
            _fn("callee"),
        ])
        abi = {"caller": _zp("caller", 4), "callee": _zp("callee", 2)}
        new_abi, syms = allocate_zp_slots(prog, abi)
        self.assertEqual(new_abi["caller"].addrs, [0x80, 0x81, 0x82, 0x83])
        self.assertEqual(new_abi["callee"].addrs, [0x84, 0x85])
        self.assertEqual(syms["__zpabi_caller_p0"], 0x80)
        self.assertEqual(syms["__zpabi_callee_p1"], 0x85)

    def test_siblings_can_share(self) -> None:
        # main calls left and right; left and right don't call
        # each other. left at $80..$81 and right at $80..$81 is
        # safe (their activations are never simultaneous).
        prog = tac_ast.Program(top_level=[
            _fn("main", _call("left"), _call("right")),
            _fn("left"),
            _fn("right"),
        ])
        abi = {
            "main": _zp("main", 2),
            "left": _zp("left", 2),
            "right": _zp("right", 2),
        }
        new_abi, _ = allocate_zp_slots(prog, abi)
        # main at $80..$81, left and right both at $82..$83 (the
        # next available range above main's).
        self.assertEqual(new_abi["main"].addrs, [0x80, 0x81])
        self.assertEqual(new_abi["left"].addrs, [0x82, 0x83])
        self.assertEqual(new_abi["right"].addrs, [0x82, 0x83])

    def test_diamond(self) -> None:
        # root → left → leaf, root → right → leaf. Leaf is a
        # descendant of both left and right; leaf's slots must be
        # disjoint from root, left, AND right.
        prog = tac_ast.Program(top_level=[
            _fn("root", _call("left"), _call("right")),
            _fn("left", _call("leaf")),
            _fn("right", _call("leaf")),
            _fn("leaf"),
        ])
        abi = {
            "root": _zp("root", 1),
            "left": _zp("left", 1),
            "right": _zp("right", 1),
            "leaf": _zp("leaf", 1),
        }
        new_abi, _ = allocate_zp_slots(prog, abi)
        leaf_addr = new_abi["leaf"].addrs[0]
        self.assertNotIn(leaf_addr, new_abi["root"].addrs)
        self.assertNotIn(leaf_addr, new_abi["left"].addrs)
        self.assertNotIn(leaf_addr, new_abi["right"].addrs)

    def test_softstack_function_ignored(self) -> None:
        # A SoftStackLayout entry isn't touched.
        prog = tac_ast.Program(top_level=[
            _fn("zp", _call("ss")),
            _fn("ss"),
        ])
        abi = {"zp": _zp("zp", 2), "ss": SoftStackLayout()}
        new_abi, syms = allocate_zp_slots(prog, abi)
        self.assertIsInstance(new_abi["ss"], SoftStackLayout)
        # No `__zpabi_ss_*` symbols in the map.
        self.assertNotIn("__zpabi_ss_p0", syms)

    def test_extern_zp_abi_gets_slots(self) -> None:
        # An extern declared zp_abi has no body in `prog.top_level`,
        # but its slot symbols still need addresses bound (call sites
        # in this TU will write to them).
        prog = tac_ast.Program(top_level=[
            _fn("local", _call("ext")),
        ])
        abi = {"local": _zp("local", 1), "ext": _zp("ext", 1)}
        new_abi, syms = allocate_zp_slots(prog, abi)
        self.assertIn("__zpabi_ext_p0", syms)
        # ext is treated as a leaf (no outgoing edges from this TU).
        # Allocator pushes it past `local`'s slot.
        self.assertNotEqual(
            new_abi["local"].addrs, new_abi["ext"].addrs,
        )

    def test_spill_to_non_zp(self) -> None:
        # Build a 32-function-deep call chain where each function
        # takes 2 byte parameters. The default ZP caller-saved
        # window is 64 bytes ($80..$BF), so chain length 32 exactly
        # fills it. A 33rd function in the chain must spill above.
        depth = 33
        top: list[tac_ast.Type_top_level] = []
        for i in range(depth):
            if i + 1 < depth:
                top.append(_fn(f"f{i}", _call(f"f{i + 1}")))
            else:
                top.append(_fn(f"f{i}"))
        prog = tac_ast.Program(top_level=top)
        abi = {f"f{i}": _zp(f"f{i}", 2) for i in range(depth)}
        new_abi, _ = allocate_zp_slots(prog, abi)
        # f0..f31 fill $80..$BF exactly.
        self.assertEqual(new_abi["f0"].addrs, [0x80, 0x81])
        self.assertEqual(new_abi["f31"].addrs, [0xBE, 0xBF])
        # f32 spills into the non-ZP fallback (default starts at
        # $0200). Same call-site emit code, just costs one extra
        # cycle / byte per access (absolute addressing).
        self.assertGreaterEqual(new_abi["f32"].addrs[0], 0x0200)


class TestZpSlotAllocationErrors(unittest.TestCase):
    def test_cycle_raises(self) -> None:
        # Defensive: select_abi should have rejected this, but
        # if a cycle reaches the allocator it must error out
        # rather than infinite-loop or silently misalloc.
        prog = tac_ast.Program(top_level=[
            _fn("a", _call("b")),
            _fn("b", _call("a")),
        ])
        abi = {"a": _zp("a", 1), "b": _zp("b", 1)}
        with self.assertRaises(ZpSlotAllocationError) as cm:
            allocate_zp_slots(prog, abi)
        self.assertIn("cycle", str(cm.exception))


if __name__ == "__main__":
    unittest.main()
