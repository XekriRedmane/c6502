"""Behavioral tests for `passes.optimization.unreachable_code_elimination`.

Coverage:
  - Step 1 (unreachable blocks):
      * Code after `Ret` (with no jumped-to label between) is dropped.
      * Code after `Jump` (with no jumped-to label between) is dropped.
      * A block reached only by a Jump from a now-dead block is itself
        dropped.
  - Step 2 (useless jumps):
      * `Jump(L)` where L is the next block in source order — dropped.
      * `JumpIfTrue(c, L)` / `JumpIfFalse(c, L)` where L is the next
        block — dropped.
      * `Ret` is never treated as a useless jump (its successor is
        EXIT, not a real block).
      * A `Jump` whose target is two blocks down (not adjacent) is
        kept.
  - Step 3 (useless labels):
      * A label only reached by fall-through (no Jump targets it) is
        dropped.
      * A label that's the target of a remaining Jump is kept.
      * A label whose only Jump references were inside dead code
        (dropped in step 1) is now useless and gets dropped.
  - Combined:
      * Idempotence: running the pass twice equals running it once.
      * The function's name / linkage / params are preserved.
      * Straight-line code with no unreachable / useless content
        passes through unchanged.
"""

from __future__ import annotations

import unittest

import tac_ast
from passes.optimization.unreachable_code_elimination import (
    eliminate_unreachable_code,
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


class TestUnreachableBlocks(unittest.TestCase):
    def test_drops_code_after_ret(self) -> None:
        fn = _fn(_copy(1, "x"), _ret(0), _copy(2, "y"), _ret(1))
        out = eliminate_unreachable_code(fn)
        self.assertEqual(out.instructions, [_copy(1, "x"), _ret(0)])

    def test_drops_code_after_jump_with_no_label(self) -> None:
        fn = _fn(
            _copy(1, "x"),
            tac_ast.Jump(target="L1"),
            _copy(2, "y"),  # unreachable
            tac_ast.Label(name="L1"),
            _ret(0),
        )
        out = eliminate_unreachable_code(fn)
        # The dead `Copy(2, "y")` is gone. The `Jump(L1)` itself
        # also drops in step 2 (useless jump — L1 IS the next block
        # after the dead block is removed). The Label(L1) drops in
        # step 3 (no remaining Jump references it).
        self.assertEqual(out.instructions, [_copy(1, "x"), _ret(0)])

    def test_keeps_block_reached_via_jump(self) -> None:
        # The middle block is unreachable by fall-through but reached
        # by the explicit Jump. Both are kept.
        fn = _fn(
            tac_ast.Jump(target="L1"),
            _copy(99, "y"),  # unreachable by fall-through
            tac_ast.Label(name="L1"),
            _copy(1, "x"),
            _ret(0),
        )
        out = eliminate_unreachable_code(fn)
        # The dead `Copy(99, "y")` is gone. After that drop, L1 is
        # the only successor of the Jump's block AND its source-order
        # successor — so step 2 drops the Jump, then step 3 drops
        # the now-unreferenced Label.
        self.assertEqual(out.instructions, [_copy(1, "x"), _ret(0)])

    def test_drops_chain_reachable_only_from_dead_code(self) -> None:
        # B1 is reachable only from B0; B0 is unreachable (after Ret).
        # B2 is the live tail. After dropping B0, B1 becomes
        # unreachable too and must be dropped on the same pass.
        fn = _fn(
            _ret(0),  # function ends here
            tac_ast.Label(name="dead_entry"),
            _copy(1, "x"),
            tac_ast.Jump(target="dead_tail"),
            tac_ast.Label(name="dead_tail"),
            _copy(2, "y"),
            _ret(1),
        )
        out = eliminate_unreachable_code(fn)
        self.assertEqual(out.instructions, [_ret(0)])


class TestUselessJumps(unittest.TestCase):
    def test_drops_unconditional_jump_to_next_block(self) -> None:
        fn = _fn(
            _copy(1, "x"),
            tac_ast.Jump(target="L1"),
            tac_ast.Label(name="L1"),
            _ret(0),
        )
        out = eliminate_unreachable_code(fn)
        # Jump dropped (L1 is the next block); Label dropped (no
        # remaining Jump references L1).
        self.assertEqual(out.instructions, [_copy(1, "x"), _ret(0)])

    def test_drops_jump_if_true_to_next_block(self) -> None:
        fn = _fn(
            tac_ast.JumpIfTrue(
                condition=tac_ast.Var(name="c"), target="L1",
            ),
            tac_ast.Label(name="L1"),
            _ret(0),
        )
        out = eliminate_unreachable_code(fn)
        self.assertEqual(out.instructions, [_ret(0)])

    def test_drops_jump_if_false_to_next_block(self) -> None:
        fn = _fn(
            tac_ast.JumpIfFalse(
                condition=tac_ast.Var(name="c"), target="L1",
            ),
            tac_ast.Label(name="L1"),
            _ret(0),
        )
        out = eliminate_unreachable_code(fn)
        self.assertEqual(out.instructions, [_ret(0)])

    def test_keeps_ret_terminator(self) -> None:
        # Ret is the last instruction and its successor is EXIT (not
        # a real block); it must never be elided.
        fn = _fn(_copy(1, "x"), _ret(7))
        out = eliminate_unreachable_code(fn)
        self.assertEqual(out.instructions, [_copy(1, "x"), _ret(7)])

    def test_keeps_jump_to_non_adjacent_target(self) -> None:
        # The Jump's target is two blocks down with a reachable block
        # in between, so the Jump is not adjacent to its target and
        # must be kept. (The naive "drop the Jump" rewrite would skip
        # the intervening block's instructions on the taken path.)
        fn = _fn(
            tac_ast.JumpIfFalse(
                condition=tac_ast.Var(name="c"), target="L_skip",
            ),
            _copy(1, "x"),
            tac_ast.Jump(target="L_after"),  # non-adjacent target
            tac_ast.Label(name="L_skip"),
            _copy(2, "z"),
            tac_ast.Label(name="L_after"),
            _ret(0),
        )
        out = eliminate_unreachable_code(fn)
        # Every block is reachable; no Jump is adjacent to its
        # target; both labels have live references — output is
        # structurally identical to the input.
        self.assertEqual(out.instructions, list(fn.instructions))


class TestUselessLabels(unittest.TestCase):
    def test_drops_label_with_no_jump_references(self) -> None:
        # Reach the labeled block by fall-through only.
        fn = _fn(
            _copy(1, "x"),
            tac_ast.Label(name="unreferenced"),
            _ret(0),
        )
        out = eliminate_unreachable_code(fn)
        self.assertEqual(out.instructions, [_copy(1, "x"), _ret(0)])

    def test_keeps_label_targeted_by_a_jump(self) -> None:
        # `head` is the back-edge target of a do-while-style loop. It
        # has Jump references and must stay.
        fn = _fn(
            tac_ast.Label(name="head"),
            _copy(1, "x"),
            tac_ast.JumpIfTrue(
                condition=tac_ast.Var(name="c"), target="head",
            ),
            _ret(0),
        )
        out = eliminate_unreachable_code(fn)
        self.assertEqual(out.instructions, [
            tac_ast.Label(name="head"),
            _copy(1, "x"),
            tac_ast.JumpIfTrue(
                condition=tac_ast.Var(name="c"), target="head",
            ),
            _ret(0),
        ])

    def test_drops_label_whose_only_reference_was_in_dead_code(self) -> None:
        # `cleanup` is referenced only by a Jump inside an
        # unreachable block. Step 1 drops the dead block; step 3
        # then drops the now-unreferenced Label.
        fn = _fn(
            _ret(0),
            _copy(1, "x"),
            tac_ast.Jump(target="cleanup"),  # in dead block
            tac_ast.Label(name="cleanup"),
            _ret(1),
        )
        out = eliminate_unreachable_code(fn)
        self.assertEqual(out.instructions, [_ret(0)])


class TestPreservedAttributesAndShape(unittest.TestCase):
    def test_preserves_name_linkage_params(self) -> None:
        fn = _fn(_ret(0), name="foo", params=("a", "b"))
        out = eliminate_unreachable_code(fn)
        self.assertEqual(out.name, "foo")
        self.assertTrue(out.is_global)
        self.assertEqual(out.params, ["a", "b"])

    def test_straight_line_code_unchanged(self) -> None:
        fn = _fn(
            _copy(1, "x"),
            tac_ast.Binary(
                op=tac_ast.Add(),
                src1=tac_ast.Var(name="x"),
                src2=_ci(2),
                dst=tac_ast.Var(name="y"),
            ),
            _ret(0),
        )
        self.assertEqual(eliminate_unreachable_code(fn), fn)

    def test_idempotence(self) -> None:
        # A non-trivial function with reachable / dead / useless mix.
        # Running the pass twice equals running it once — no further
        # opportunities are exposed within a single pass invocation.
        fn = _fn(
            _copy(1, "x"),
            tac_ast.Jump(target="L1"),
            _copy(99, "y"),  # unreachable
            tac_ast.Label(name="L1"),
            tac_ast.JumpIfTrue(
                condition=tac_ast.Var(name="c"), target="L2",
            ),
            tac_ast.Label(name="L2"),
            _ret(0),
        )
        once = eliminate_unreachable_code(fn)
        twice = eliminate_unreachable_code(once)
        self.assertEqual(once, twice)


if __name__ == "__main__":
    unittest.main()
