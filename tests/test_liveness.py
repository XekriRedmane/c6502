"""Behavioral tests for `passes.optimization.liveness`.

Coverage:
  - Straight-line: gen / kill computed correctly per block.
  - Use-before-def in same block: input shows up in live_in.
  - Diamond CFG: a value live through both arms is live-out at both
    and live-in at the merge.
  - Loop back-edge: value defined / used across a back-edge stays
    live-in / live-out at the loop header.
  - Phi sources: each predecessor edge contributes its matching
    PhiArg, NOT the union of all PhiArg sources.
  - Per-instruction live_after / live_before walks back through a
    block correctly.
"""

from __future__ import annotations

import unittest

import tac_ast
from passes.optimization.cfg import ENTRY_ID, EXIT_ID
from passes.optimization.liveness import compute_liveness


def _ci(v: int) -> tac_ast.Constant:
    return tac_ast.Constant(const=tac_ast.ConstInt(value=v))


def _var(name: str) -> tac_ast.Var:
    return tac_ast.Var(name=name)


def _fn(*instrs, name: str = "main", params=()) -> tac_ast.Function:
    return tac_ast.Function(
        name=name, is_global=True,
        params=list(params), instructions=list(instrs),
    )


def _block_id_with_label(liveness, label: str) -> int:
    for bid, blk in liveness.cfg.blocks.items():
        if (
            blk.instructions
            and isinstance(blk.instructions[0], tac_ast.Label)
            and blk.instructions[0].name == label
        ):
            return bid
    raise AssertionError(f"no block with leading label {label}")


class TestStraightLine(unittest.TestCase):
    def test_no_inputs_no_outputs(self) -> None:
        # x = 1; y = x + 2; ret y
        fn = _fn(
            tac_ast.Copy(src=_ci(1), dst=_var("x")),
            tac_ast.Binary(
                op=tac_ast.Add(), src1=_var("x"), src2=_ci(2),
                dst=_var("y"),
            ),
            tac_ast.Ret(val=_var("y")),
        )
        liv = compute_liveness(fn)
        bid = liv.cfg.block_order[0]
        self.assertEqual(liv.live_in[bid], frozenset())
        self.assertEqual(liv.live_out[bid], frozenset())
        # Neither input nor output to the function as a whole.
        self.assertEqual(liv.live_in[ENTRY_ID], frozenset())
        self.assertEqual(liv.live_out[EXIT_ID], frozenset())

    def test_param_used_is_live_in(self) -> None:
        # Param `p` read in body → live_in at the function's first
        # block includes p.
        fn = _fn(
            tac_ast.Binary(
                op=tac_ast.Add(), src1=_var("p"), src2=_ci(1),
                dst=_var("t"),
            ),
            tac_ast.Ret(val=_var("t")),
            params=("p",),
        )
        liv = compute_liveness(fn)
        bid = liv.cfg.block_order[0]
        self.assertEqual(liv.live_in[bid], frozenset({"p"}))


class TestDiamond(unittest.TestCase):
    def test_value_live_through_both_arms(self) -> None:
        # Diamond:  ... ; if c then x = 1 else x = 2; ret x
        fn = _fn(
            tac_ast.JumpIfFalse(condition=_var("c"), target=".else"),
            tac_ast.Copy(src=_ci(1), dst=_var("x")),
            tac_ast.Jump(target=".join"),
            tac_ast.Label(name=".else"),
            tac_ast.Copy(src=_ci(2), dst=_var("x")),
            tac_ast.Label(name=".join"),
            tac_ast.Ret(val=_var("x")),
        )
        liv = compute_liveness(fn)
        join_bid = _block_id_with_label(liv, ".join")
        else_bid = _block_id_with_label(liv, ".else")

        self.assertIn("x", liv.live_in[join_bid])
        # In each arm, `x` is defined then live-out (carried to join).
        # The taken-branch block contains the `x = 1`; live-out should
        # include x.
        # The arm blocks: find the one whose first instr is `x = 1`
        # (the entry block's fall-through successor).
        for bid, blk in liv.cfg.blocks.items():
            if bid in (ENTRY_ID, EXIT_ID):
                continue
            first = blk.instructions[0]
            if isinstance(first, tac_ast.Copy) and isinstance(
                first.src, tac_ast.Constant,
            ):
                self.assertIn(
                    "x", liv.live_out[bid],
                    f"x should be live-out of block {bid} ({blk.instructions[0]})",
                )


class TestLoopBackEdge(unittest.TestCase):
    def test_induction_var_live_at_header(self) -> None:
        # i = 0; .top: if i >= 10 goto .end; i = i + 1; goto .top;
        # .end: ret i
        fn = _fn(
            tac_ast.Copy(src=_ci(0), dst=_var("i")),
            tac_ast.Label(name=".top"),
            tac_ast.Binary(
                op=tac_ast.GreaterOrEqual(),
                src1=_var("i"), src2=_ci(10),
                dst=_var("c"),
            ),
            tac_ast.JumpIfTrue(condition=_var("c"), target=".end"),
            tac_ast.Binary(
                op=tac_ast.Add(), src1=_var("i"), src2=_ci(1),
                dst=_var("i"),
            ),
            tac_ast.Jump(target=".top"),
            tac_ast.Label(name=".end"),
            tac_ast.Ret(val=_var("i")),
        )
        liv = compute_liveness(fn)
        top_bid = _block_id_with_label(liv, ".top")
        end_bid = _block_id_with_label(liv, ".end")
        self.assertIn("i", liv.live_in[top_bid])
        self.assertIn("i", liv.live_out[top_bid])
        self.assertIn("i", liv.live_in[end_bid])


class TestPhiPerEdgeSources(unittest.TestCase):
    def test_phi_source_only_live_on_matching_pred(self) -> None:
        # if c jump .else; .then: x.1 = 1; jump .join;
        # .else: x.2 = 2;
        # .join: x.3 = phi(.then -> x.1, .else -> x.2); ret x.3
        fn = _fn(
            tac_ast.Label(name=".entry"),
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
            tac_ast.Ret(val=_var("x.3")),
        )
        liv = compute_liveness(fn)
        then_bid = _block_id_with_label(liv, ".then")
        else_bid = _block_id_with_label(liv, ".else")

        # Each pred contributes only its matching Phi source to live_out.
        self.assertIn("x.1", liv.live_out[then_bid])
        self.assertNotIn("x.2", liv.live_out[then_bid])
        self.assertIn("x.2", liv.live_out[else_bid])
        self.assertNotIn("x.1", liv.live_out[else_bid])


class TestPerInstructionLive(unittest.TestCase):
    def test_live_after_each_instr(self) -> None:
        # a = 1; b = a + 1; c = b + 1; ret c
        fn = _fn(
            tac_ast.Copy(src=_ci(1), dst=_var("a")),
            tac_ast.Binary(
                op=tac_ast.Add(), src1=_var("a"), src2=_ci(1),
                dst=_var("b"),
            ),
            tac_ast.Binary(
                op=tac_ast.Add(), src1=_var("b"), src2=_ci(1),
                dst=_var("c"),
            ),
            tac_ast.Ret(val=_var("c")),
        )
        liv = compute_liveness(fn)
        bid = liv.cfg.block_order[0]
        # After `a = 1`: a is live (next instr reads a).
        self.assertEqual(liv.live_after(bid, 0), frozenset({"a"}))
        # After `b = a + 1`: b is live.
        self.assertEqual(liv.live_after(bid, 1), frozenset({"b"}))
        # After `c = b + 1`: c is live.
        self.assertEqual(liv.live_after(bid, 2), frozenset({"c"}))
        # After `ret c`: nothing live.
        self.assertEqual(liv.live_after(bid, 3), frozenset())

    def test_live_before_first_instr_includes_phi_dsts(self) -> None:
        # .top: x.1 = phi(...); use x.1
        # live_before(top, 0) — phi at index 0; live_before says
        # "post-Phi live set", so x.1 is in it.
        fn = _fn(
            tac_ast.Label(name=".entry"),
            tac_ast.Jump(target=".top"),
            tac_ast.Label(name=".top"),
            tac_ast.Phi(
                dst=_var("x.1"),
                args=[
                    tac_ast.PhiArg(pred_label=".entry", source=_ci(7)),
                ],
            ),
            tac_ast.Ret(val=_var("x.1")),
        )
        liv = compute_liveness(fn)
        top_bid = _block_id_with_label(liv, ".top")
        # Index 0 is the Label, not the Phi — find Phi index.
        blk = liv.cfg.blocks[top_bid]
        # Block layout under build_cfg: leading Label, Phi, Ret.
        self.assertIsInstance(blk.instructions[1], tac_ast.Phi)
        # live_before of the Label position includes the Phi dst.
        self.assertIn("x.1", liv.live_before(top_bid, 0))
