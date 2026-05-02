"""Behavioral tests for `passes.optimization.dead_store_elimination`.

Coverage:
  - Pure defs (Copy / Binary / Unary / cast / Phi) whose dst is
    unused are dropped.
  - FunctionCall / IndirectCall with unused dst keep the call but
    drop the dst (the call's side effects survive).
  - Side-effecting instructions are always kept: Store, Ret,
    control flow.
  - A def whose dst isn't in `ssa_dsts` is kept regardless of in-
    function usage (writes to statics / address-taken locals are
    observable elsewhere).
  - Iterates to fixed point: dropping a def removes its inputs'
    last reads, exposing more defs as dead.
  - Without `ssa_dsts` (legacy / non-SSA caller) the pass is a
    no-op.
"""

from __future__ import annotations

import unittest

import tac_ast
from passes.optimization.dead_store_elimination import (
    eliminate_dead_stores,
)


def _ci(v: int) -> tac_ast.Constant:
    return tac_ast.Constant(const=tac_ast.ConstInt(value=v))


def _var(n: str) -> tac_ast.Var:
    return tac_ast.Var(name=n)


def _fn(*instrs, params=()) -> tac_ast.Function:
    return tac_ast.Function(
        name="f", is_global=True,
        params=list(params), instructions=list(instrs),
    )


class TestDeadStoreElimination(unittest.TestCase):
    def test_dead_pure_def_dropped(self) -> None:
        # Copy whose dst is unused (the Ret returns a Constant).
        fn = _fn(
            tac_ast.Copy(src=_ci(7), dst=_var("x.1")),
            tac_ast.Ret(val=_ci(0)),
        )
        out = eliminate_dead_stores(fn, ssa_dsts={"x.1"})
        self.assertEqual(out.instructions, [tac_ast.Ret(val=_ci(0))])

    def test_chain_of_dead_defs_iterates_to_fixed_point(self) -> None:
        # x.1 = 7; y.1 = x.1 + 1; z.1 = y.1; (none used)
        # First pass: z.1 is dead (no reads), drop the Copy.
        # Second pass: y.1's only use was z.1's Copy (now gone), so
        # y.1 is dead — drop the Binary.
        # Third pass: x.1's only use was the Binary (now gone), so
        # drop the Copy. Fixed point: just the Ret remains.
        fn = _fn(
            tac_ast.Copy(src=_ci(7), dst=_var("x.1")),
            tac_ast.Binary(
                op=tac_ast.Add(), src1=_var("x.1"), src2=_ci(1),
                dst=_var("y.1"),
            ),
            tac_ast.Copy(src=_var("y.1"), dst=_var("z.1")),
            tac_ast.Ret(val=_ci(0)),
        )
        out = eliminate_dead_stores(
            fn, ssa_dsts={"x.1", "y.1", "z.1"},
        )
        self.assertEqual(out.instructions, [tac_ast.Ret(val=_ci(0))])

    def test_function_call_unused_dst_loses_dst_keeps_call(self) -> None:
        # FunctionCall with unused dst — drop dst, keep call.
        fn = _fn(
            tac_ast.FunctionCall(
                name="side_effect", args=[_ci(3)], dst=_var("ret.1"),
            ),
            tac_ast.Ret(val=_ci(0)),
        )
        out = eliminate_dead_stores(fn, ssa_dsts={"ret.1"})
        self.assertEqual(out.instructions, [
            tac_ast.FunctionCall(
                name="side_effect", args=[_ci(3)], dst=None,
            ),
            tac_ast.Ret(val=_ci(0)),
        ])

    def test_store_always_kept(self) -> None:
        # Store has observable effect (writes through pointer).
        fn = _fn(
            tac_ast.Store(src=_ci(5), dst_ptr=_var("p")),
            tac_ast.Ret(val=_ci(0)),
        )
        out = eliminate_dead_stores(fn, ssa_dsts={"p"})
        self.assertEqual(out, fn)

    def test_def_to_non_ssa_dst_is_kept(self) -> None:
        # Copy(7, globl) where globl isn't SSA-renamed. Even if no
        # in-function read of globl, other functions might read
        # the static after this returns.
        fn = _fn(
            tac_ast.Copy(src=_ci(7), dst=_var("globl")),
            tac_ast.Ret(val=_ci(0)),
        )
        out = eliminate_dead_stores(fn, ssa_dsts=set())
        self.assertEqual(out, fn)

    def test_ret_value_keeps_def_alive(self) -> None:
        # Ret's val is a use of x.1; the Copy stays.
        fn = _fn(
            tac_ast.Copy(src=_ci(42), dst=_var("x.1")),
            tac_ast.Ret(val=_var("x.1")),
        )
        out = eliminate_dead_stores(fn, ssa_dsts={"x.1"})
        self.assertEqual(out, fn)

    def test_dead_phi_dropped(self) -> None:
        # A Phi whose dst is unread is dead just like any other
        # pure def.
        fn = _fn(
            tac_ast.Phi(
                dst=_var("dead.1"),
                args=[
                    tac_ast.PhiArg(pred_label="L1", source=_ci(1)),
                    tac_ast.PhiArg(pred_label="L2", source=_ci(2)),
                ],
            ),
            tac_ast.Ret(val=_ci(0)),
        )
        out = eliminate_dead_stores(fn, ssa_dsts={"dead.1"})
        self.assertEqual(out.instructions, [tac_ast.Ret(val=_ci(0))])

    def test_legacy_mode_is_noop(self) -> None:
        # Without ssa_dsts, eliminate_dead_stores doesn't drop
        # anything (no safe way to identify droppable Vars).
        fn = _fn(
            tac_ast.Copy(src=_ci(7), dst=_var("x")),
            tac_ast.Ret(val=_ci(0)),
        )
        self.assertEqual(eliminate_dead_stores(fn), fn)


if __name__ == "__main__":
    unittest.main()
