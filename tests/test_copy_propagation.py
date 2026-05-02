"""Behavioral tests for `passes.optimization.copy_propagation`.

Coverage:
  - Constant Copy substitutes through every use of dst.
  - SSA-renamed Var → Var Copy substitutes (chain-resolved through
    multi-step Copies).
  - Copy whose dst isn't in `ssa_dsts` doesn't contribute (writes
    to statics / address-taken locals are observable elsewhere).
  - Copy whose src is a non-SSA Var doesn't contribute (reading a
    non-SSA name returns a memory value at a specific point;
    propagating across that read would observe stale memory).
  - Phi sources are rewritten.
  - Without `ssa_dsts` (legacy / non-SSA caller) the pass is a no-op.
  - GetAddress.operand is left alone (it names a storage cell, not a
    value).
"""

from __future__ import annotations

import unittest

import tac_ast
from passes.optimization.copy_propagation import copy_propagate


def _ci(v: int) -> tac_ast.Constant:
    return tac_ast.Constant(const=tac_ast.ConstInt(value=v))


def _var(n: str) -> tac_ast.Var:
    return tac_ast.Var(name=n)


def _fn(*instrs, params=()) -> tac_ast.Function:
    return tac_ast.Function(
        name="f", is_global=True,
        params=list(params), instructions=list(instrs),
    )


class TestCopyPropagation(unittest.TestCase):
    def test_constant_propagates_to_uses(self) -> None:
        fn = _fn(
            tac_ast.Copy(src=_ci(7), dst=_var("x.1")),
            tac_ast.Binary(
                op=tac_ast.Add(), src1=_var("x.1"), src2=_ci(1),
                dst=_var("y.1"),
            ),
            tac_ast.Ret(val=_var("y.1")),
        )
        out = copy_propagate(fn, ssa_dsts={"x.1", "y.1"})
        # The Binary's src1 is rewritten from x.1 → 7. The Copy
        # stays (DSE will drop it later) but no longer feeds
        # anyone.
        self.assertEqual(out.instructions, [
            tac_ast.Copy(src=_ci(7), dst=_var("x.1")),
            tac_ast.Binary(
                op=tac_ast.Add(), src1=_ci(7), src2=_ci(1),
                dst=_var("y.1"),
            ),
            tac_ast.Ret(val=_var("y.1")),
        ])

    def test_var_to_var_chain_resolves(self) -> None:
        # x.1 = 5;  y.1 = x.1;  z.1 = y.1;  return z.1
        # Expected: every use of y.1 / z.1 resolves to 5.
        fn = _fn(
            tac_ast.Copy(src=_ci(5), dst=_var("x.1")),
            tac_ast.Copy(src=_var("x.1"), dst=_var("y.1")),
            tac_ast.Copy(src=_var("y.1"), dst=_var("z.1")),
            tac_ast.Ret(val=_var("z.1")),
        )
        out = copy_propagate(fn, ssa_dsts={"x.1", "y.1", "z.1"})
        # The Ret's value walks z.1 → y.1 → x.1 → 5.
        self.assertEqual(out.instructions[-1], tac_ast.Ret(val=_ci(5)))

    def test_dst_not_in_ssa_dsts_is_skipped(self) -> None:
        # Copy(src=5, dst=globl) where globl isn't SSA-renamed —
        # the Copy doesn't contribute. Subsequent reads of globl
        # stay as Var(globl).
        fn = _fn(
            tac_ast.Copy(src=_ci(5), dst=_var("globl")),
            tac_ast.Ret(val=_var("globl")),
        )
        out = copy_propagate(fn, ssa_dsts=set())
        self.assertEqual(out, fn)

    def test_src_not_in_ssa_dsts_is_skipped(self) -> None:
        # Copy(src=globl, dst=x.1) — globl isn't SSA, so x.1's
        # value can't be propagated to other reads of x.1 (the
        # value of globl might change before the next read).
        fn = _fn(
            tac_ast.Copy(src=_var("globl"), dst=_var("x.1")),
            tac_ast.Ret(val=_var("x.1")),
        )
        out = copy_propagate(fn, ssa_dsts={"x.1"})
        # The Ret should NOT have been rewritten to read globl.
        self.assertEqual(out, fn)

    def test_phi_sources_are_rewritten(self) -> None:
        # A Phi's src args are uses of SSA names; copy propagation
        # should rewrite them.
        fn = _fn(
            tac_ast.Copy(src=_ci(7), dst=_var("a.1")),
            tac_ast.Phi(
                dst=_var("merged.1"),
                args=[
                    tac_ast.PhiArg(pred_label="L1", source=_var("a.1")),
                    tac_ast.PhiArg(pred_label="L2", source=_ci(8)),
                ],
            ),
            tac_ast.Ret(val=_var("merged.1")),
        )
        out = copy_propagate(fn, ssa_dsts={"a.1", "merged.1"})
        # The PhiArg whose source was a.1 now reads 7 directly.
        phi = next(
            i for i in out.instructions if isinstance(i, tac_ast.Phi)
        )
        self.assertEqual(phi.args[0].source, _ci(7))
        self.assertEqual(phi.args[1].source, _ci(8))

    def test_legacy_mode_is_noop(self) -> None:
        # Without ssa_dsts, copy_propagate doesn't transform
        # anything (the SSA invariant isn't asserted, so any
        # propagation could be unsound).
        fn = _fn(
            tac_ast.Copy(src=_ci(7), dst=_var("x")),
            tac_ast.Ret(val=_var("x")),
        )
        self.assertEqual(copy_propagate(fn), fn)

    def test_get_address_operand_not_substituted(self) -> None:
        # GetAddress's operand names a storage cell — even if the
        # operand "matches" a Copy's dst by name, we don't
        # substitute (the address of x.1 wouldn't equal the value
        # x.1 holds). In practice promotable Vars don't appear as
        # GetAddress operands (that's how `_identify_promotable`
        # excludes address-taken Vars), but the rule is asserted
        # defensively here.
        fn = _fn(
            tac_ast.Copy(src=_ci(5), dst=_var("x.1")),
            tac_ast.GetAddress(operand=_var("x.1"), dst=_var("p.1")),
            tac_ast.Ret(val=_var("p.1")),
        )
        out = copy_propagate(fn, ssa_dsts={"x.1", "p.1"})
        get_addr = next(
            i for i in out.instructions
            if isinstance(i, tac_ast.GetAddress)
        )
        self.assertEqual(get_addr.operand, _var("x.1"))


if __name__ == "__main__":
    unittest.main()
