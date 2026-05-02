"""Behavioral tests for `passes.optimization.cfg`.

Coverage:
  - Empty function: ENTRY → EXIT directly.
  - Single straight-line block ending in `Ret` → ENTRY → B → EXIT.
  - Fall-through partitioning at `Label` introduces a second block
    with an inter-block fall-through edge.
  - Mid-function `Ret` closes the current block, the instruction
    after it starts a new (unreachable) block.
  - Forward `Jump` to a later label wires the target's block as the
    sole successor; the source-order successor is unreached.
  - `JumpIfTrue` / `JumpIfFalse` produce two successors: taken
    (the labeled block) and fall-through.
  - Backward jump (loop) wires a back-edge to an earlier block.
  - Predecessors mirror successors (every edge appears in both
    directions).
  - Trailing block without an explicit terminator falls through to
    EXIT.
  - `cfg_to_function` round-trips an unmodified CFG and drops blocks
    excluded from `block_order`.
"""

from __future__ import annotations

import unittest

import tac_ast
from passes.optimization.cfg import (
    ENTRY_ID,
    EXIT_ID,
    build_cfg,
    cfg_to_function,
)


def _ci(v: int) -> tac_ast.Constant:
    return tac_ast.Constant(const=tac_ast.ConstInt(value=v))


def _ret(v: int = 0) -> tac_ast.Ret:
    return tac_ast.Ret(val=_ci(v))


def _copy(v: int, dst: str) -> tac_ast.Copy:
    return tac_ast.Copy(src=_ci(v), dst=tac_ast.Var(name=dst))


def _fn(*instrs, name: str = "main", params=()) -> tac_ast.Function:
    return tac_ast.Function(
        name=name,
        is_global=True,
        params=list(params),
        instructions=list(instrs),
    )


class TestBuildCFG(unittest.TestCase):
    def test_empty_function_entry_to_exit(self) -> None:
        cfg = build_cfg(_fn())
        self.assertEqual(cfg.block_order, [])
        self.assertEqual(cfg.blocks[ENTRY_ID].successors, [EXIT_ID])
        self.assertEqual(cfg.blocks[EXIT_ID].predecessors, [ENTRY_ID])

    def test_single_block_returns(self) -> None:
        fn = _fn(_copy(1, "x"), _ret(0))
        cfg = build_cfg(fn)
        self.assertEqual(len(cfg.block_order), 1)
        bid = cfg.block_order[0]
        self.assertEqual(cfg.blocks[ENTRY_ID].successors, [bid])
        self.assertEqual(cfg.blocks[bid].predecessors, [ENTRY_ID])
        self.assertEqual(cfg.blocks[bid].successors, [EXIT_ID])
        self.assertEqual(cfg.blocks[EXIT_ID].predecessors, [bid])
        self.assertEqual(
            cfg.blocks[bid].instructions, [_copy(1, "x"), _ret(0)],
        )

    def test_label_starts_new_block(self) -> None:
        fn = _fn(
            _copy(1, "x"),
            tac_ast.Label(name="L1"),
            _copy(2, "y"),
            _ret(0),
        )
        cfg = build_cfg(fn)
        self.assertEqual(len(cfg.block_order), 2)
        b0, b1 = cfg.block_order
        self.assertEqual(cfg.blocks[b0].instructions, [_copy(1, "x")])
        self.assertEqual(
            cfg.blocks[b1].instructions,
            [tac_ast.Label(name="L1"), _copy(2, "y"), _ret(0)],
        )
        # Fall-through wires b0 → b1, b1 → EXIT.
        self.assertEqual(cfg.blocks[b0].successors, [b1])
        self.assertEqual(cfg.blocks[b1].predecessors, [b0])
        self.assertEqual(cfg.blocks[b1].successors, [EXIT_ID])

    def test_mid_function_ret_starts_new_block(self) -> None:
        # An explicit Ret closes the current block; whatever follows
        # starts a fresh block with no incoming edges.
        fn = _fn(_ret(0), _copy(1, "x"), _ret(1))
        cfg = build_cfg(fn)
        self.assertEqual(len(cfg.block_order), 2)
        b0, b1 = cfg.block_order
        self.assertEqual(cfg.blocks[b0].instructions, [_ret(0)])
        self.assertEqual(cfg.blocks[b1].instructions, [_copy(1, "x"), _ret(1)])
        self.assertEqual(cfg.blocks[b0].successors, [EXIT_ID])
        self.assertEqual(cfg.blocks[b1].successors, [EXIT_ID])
        self.assertEqual(cfg.blocks[b0].predecessors, [ENTRY_ID])
        self.assertEqual(cfg.blocks[b1].predecessors, [])

    def test_forward_jump_skips_block(self) -> None:
        fn = _fn(
            _copy(1, "x"),
            tac_ast.Jump(target="L1"),
            _copy(2, "y"),
            tac_ast.Label(name="L1"),
            _ret(0),
        )
        cfg = build_cfg(fn)
        self.assertEqual(len(cfg.block_order), 3)
        b0, b1, b2 = cfg.block_order
        self.assertEqual(
            cfg.blocks[b0].instructions,
            [_copy(1, "x"), tac_ast.Jump(target="L1")],
        )
        self.assertEqual(cfg.blocks[b1].instructions, [_copy(2, "y")])
        self.assertEqual(
            cfg.blocks[b2].instructions,
            [tac_ast.Label(name="L1"), _ret(0)],
        )
        self.assertEqual(cfg.blocks[b0].successors, [b2])
        # b1 is the dead block; it has no incoming edges, but its
        # outgoing fall-through edge to b2 is still wired (consumers
        # like UCE drop dead blocks by traversal-from-entry, not by
        # missing edges).
        self.assertEqual(cfg.blocks[b1].predecessors, [])
        self.assertEqual(cfg.blocks[b1].successors, [b2])
        self.assertEqual(sorted(cfg.blocks[b2].predecessors), sorted([b0, b1]))

    def test_jump_if_true_two_successors(self) -> None:
        fn = _fn(
            tac_ast.JumpIfTrue(condition=tac_ast.Var(name="c"), target="L1"),
            _copy(0, "x"),
            tac_ast.Jump(target="L2"),
            tac_ast.Label(name="L1"),
            _copy(1, "x"),
            tac_ast.Label(name="L2"),
            _ret(0),
        )
        cfg = build_cfg(fn)
        self.assertEqual(len(cfg.block_order), 4)
        b_cond, b_else, b_then, b_end = cfg.block_order
        # Conditional source has both successors: taken (L1) and
        # fall-through (the next block in source order).
        self.assertEqual(
            sorted(cfg.blocks[b_cond].successors), sorted([b_then, b_else]),
        )
        self.assertIn(b_cond, cfg.blocks[b_then].predecessors)
        self.assertIn(b_cond, cfg.blocks[b_else].predecessors)
        # Else-arm jumps to L2.
        self.assertEqual(cfg.blocks[b_else].successors, [b_end])
        # Then-arm falls through to L2.
        self.assertEqual(cfg.blocks[b_then].successors, [b_end])
        self.assertEqual(
            sorted(cfg.blocks[b_end].predecessors), sorted([b_else, b_then]),
        )

    def test_jump_if_false_two_successors(self) -> None:
        fn = _fn(
            tac_ast.JumpIfFalse(
                condition=tac_ast.Var(name="c"), target="end",
            ),
            _copy(1, "x"),
            tac_ast.Label(name="end"),
            _ret(0),
        )
        cfg = build_cfg(fn)
        self.assertEqual(len(cfg.block_order), 3)
        b_cond, b_body, b_end = cfg.block_order
        self.assertEqual(
            sorted(cfg.blocks[b_cond].successors), sorted([b_body, b_end]),
        )
        self.assertEqual(cfg.blocks[b_body].successors, [b_end])

    def test_backward_jump_creates_loop(self) -> None:
        # `do { x = 1; } while (c);` shape: the body's tail jumps back
        # to its own head label.
        fn = _fn(
            tac_ast.Label(name="top"),
            _copy(1, "x"),
            tac_ast.JumpIfTrue(
                condition=tac_ast.Var(name="c"), target="top",
            ),
            _ret(0),
        )
        cfg = build_cfg(fn)
        self.assertEqual(len(cfg.block_order), 2)
        b_loop, b_end = cfg.block_order
        # Loop block's two successors: itself (taken) and the next
        # block (fall-through).
        self.assertEqual(
            sorted(cfg.blocks[b_loop].successors), sorted([b_loop, b_end]),
        )
        # Loop block is its own predecessor (back-edge), and ENTRY's
        # successor.
        self.assertEqual(
            sorted(cfg.blocks[b_loop].predecessors),
            sorted([ENTRY_ID, b_loop]),
        )

    def test_trailing_block_without_terminator_falls_to_exit(self) -> None:
        # Defensive: if some pass strips the implicit Ret, the last
        # block still gets an EXIT successor via fall-through.
        fn = _fn(_copy(1, "x"))
        cfg = build_cfg(fn)
        self.assertEqual(len(cfg.block_order), 1)
        bid = cfg.block_order[0]
        self.assertEqual(cfg.blocks[bid].successors, [EXIT_ID])
        self.assertEqual(cfg.blocks[EXIT_ID].predecessors, [bid])

    def test_consecutive_labels_each_start_new_block(self) -> None:
        fn = _fn(
            tac_ast.Label(name="A"),
            tac_ast.Label(name="B"),
            _ret(0),
        )
        cfg = build_cfg(fn)
        self.assertEqual(len(cfg.block_order), 2)
        b_a, b_b = cfg.block_order
        self.assertEqual(
            cfg.blocks[b_a].instructions, [tac_ast.Label(name="A")],
        )
        self.assertEqual(
            cfg.blocks[b_b].instructions,
            [tac_ast.Label(name="B"), _ret(0)],
        )
        self.assertEqual(cfg.blocks[b_a].successors, [b_b])
        self.assertEqual(cfg.blocks[b_b].predecessors, [b_a])

    def test_predecessors_mirror_successors(self) -> None:
        # General invariant: edge (u, v) appears in u.successors iff
        # in v.predecessors. Use a function that exercises every edge
        # kind (entry, fall-through, conditional taken / not-taken,
        # unconditional jump, ret).
        fn = _fn(
            _copy(1, "x"),
            tac_ast.JumpIfFalse(
                condition=tac_ast.Var(name="c"), target="L2",
            ),
            _copy(2, "y"),
            tac_ast.Jump(target="L3"),
            tac_ast.Label(name="L2"),
            _copy(3, "y"),
            tac_ast.Label(name="L3"),
            _ret(0),
        )
        cfg = build_cfg(fn)
        # Walk every block, collect (src, dst) pairs from successors,
        # then verify each appears in the dst's predecessors. Counts
        # have to match too — duplicate edges (none here, but defensive)
        # would have to mirror.
        forward: list[tuple[int, int]] = []
        backward: list[tuple[int, int]] = []
        for bid, blk in cfg.blocks.items():
            for succ in blk.successors:
                forward.append((bid, succ))
            for pred in blk.predecessors:
                backward.append((pred, bid))
        self.assertEqual(sorted(forward), sorted(backward))


class TestCFGToFunction(unittest.TestCase):
    def test_round_trip(self) -> None:
        # Building a CFG and immediately flattening it yields the same
        # instruction list (and the same name / params / linkage).
        fn = _fn(
            _copy(1, "x"),
            tac_ast.JumpIfTrue(
                condition=tac_ast.Var(name="c"), target="end",
            ),
            _copy(2, "y"),
            tac_ast.Label(name="end"),
            _ret(0),
            name="foo",
            params=("p",),
        )
        cfg = build_cfg(fn)
        round_tripped = cfg_to_function(fn, cfg)
        self.assertEqual(round_tripped, fn)

    def test_dropping_block_removes_its_instructions(self) -> None:
        fn = _fn(
            _copy(1, "x"),
            tac_ast.Jump(target="L1"),
            _copy(2, "y"),  # unreachable
            tac_ast.Label(name="L1"),
            _ret(0),
        )
        cfg = build_cfg(fn)
        # Block 1 is the dead one (the lone Copy between the Jump and
        # the labeled block).
        dead = cfg.block_order[1]
        cfg.block_order.remove(dead)
        del cfg.blocks[dead]
        out = cfg_to_function(fn, cfg)
        self.assertEqual(out.instructions, [
            _copy(1, "x"),
            tac_ast.Jump(target="L1"),
            tac_ast.Label(name="L1"),
            _ret(0),
        ])


if __name__ == "__main__":
    unittest.main()
