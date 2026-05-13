"""Behavioral tests for the `private_pool_addrs` exclusion in
`passes.replace_pseudoregisters._compute_callee_saved_addrs`.

When `zp_local_allocation` hands a function a private pool whose
addresses happen to fall in the default `Pool.callee_saved()`
range ($C0..$FF), those addresses must NOT be saved/restored by
the prologue/epilogue — the private-pool allocator already
guarantees no coexisting function touches them, so the save is
pure waste (and the addresses aren't being preserved for any
caller anyway).

Coverage:
  - No private pool → existing behavior: callee-saved addresses
    in coloring.assignments go into the callee_saved_addrs list.
  - Private pool covers some addresses → those are excluded from
    callee_saved_addrs.
  - Private pool overlaps the callee-saved range entirely → the
    function emits with zero callee-save bytes.
"""
from __future__ import annotations

import unittest

import asm_ast
from passes.optimization.pool import Pool
from passes.optimization.register_allocation import Coloring
from passes.replace_pseudoregisters import replace_function_bare_exit


def _ps(name: str, off: int = 0) -> asm_ast.Pseudo:
    return asm_ast.Pseudo(name=name, offset=off)


def _A() -> asm_ast.Reg:
    return asm_ast.Reg(reg=asm_ast.A())


def _mov(src, dst) -> asm_ast.Mov:
    return asm_ast.Mov(src=src, dst=dst)


def _ret_bare() -> asm_ast.Return:
    return asm_ast.Return(save_a=True)


def _fn(name: str, *instrs, params=()) -> asm_ast.Function:
    return asm_ast.Function(
        name=name, is_global=True,
        params=list(params), instructions=list(instrs),
    )


class TestPrivatePoolCalleeSaveExclusion(unittest.TestCase):
    def test_no_private_pool_keeps_default_behavior(self) -> None:
        # A 1-byte local colored to $C0 (callee-saved range). With
        # no private pool, the address should appear in
        # callee_saved_addrs.
        fn = _fn("f", _mov(_ps("x"), _A()), _ret_bare())
        coloring = Coloring(
            assignments={"x": 0xC0}, pool=Pool(),
        )
        _, dims = replace_function_bare_exit(
            fn, coloring=coloring,
        )
        self.assertEqual(dims.callee_saved_addrs, [0xC0])

    def test_private_pool_excludes_callee_save(self) -> None:
        # Same coloring, but `x`'s address is in this function's
        # private pool. callee_saved_addrs should be empty —
        # the pool guarantees no coexisting function touches
        # $C0, so save/restore is unneeded.
        fn = _fn("f", _mov(_ps("x"), _A()), _ret_bare())
        coloring = Coloring(
            assignments={"x": 0xC0}, pool=Pool(),
        )
        _, dims = replace_function_bare_exit(
            fn, coloring=coloring,
            private_pool_addrs=frozenset({0xC0}),
        )
        self.assertEqual(dims.callee_saved_addrs, [])

    def test_partial_overlap_excludes_only_private(self) -> None:
        # Two locals: `x` at $C0 (in private pool), `y` at $C1
        # (NOT in private pool — somehow leaked through, e.g. a
        # spilled value the regalloc placed in callee-saved range
        # outside the function's private range). Only `y` should
        # be flagged for save/restore.
        fn = _fn(
            "f",
            _mov(_ps("x"), _A()),
            _mov(_ps("y"), _A()),
            _ret_bare(),
        )
        coloring = Coloring(
            assignments={"x": 0xC0, "y": 0xC1}, pool=Pool(),
        )
        _, dims = replace_function_bare_exit(
            fn, coloring=coloring,
            private_pool_addrs=frozenset({0xC0}),
        )
        self.assertEqual(dims.callee_saved_addrs, [0xC1])

    def test_private_pool_outside_callee_range_no_op(self) -> None:
        # Private pool addresses in the caller-saved range
        # ($80..$BF) wouldn't have been added to callee_saved_addrs
        # anyway, so the exclusion is a no-op. Test for safety.
        fn = _fn("f", _mov(_ps("x"), _A()), _ret_bare())
        coloring = Coloring(
            assignments={"x": 0x84}, pool=Pool(),
        )
        _, dims = replace_function_bare_exit(
            fn, coloring=coloring,
            private_pool_addrs=frozenset({0x84}),
        )
        self.assertEqual(dims.callee_saved_addrs, [])


if __name__ == "__main__":
    unittest.main()
