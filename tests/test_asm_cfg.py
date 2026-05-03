"""Behavioral tests for `passes.optimization_asm.cfg`.

Mirrors the TAC-level test_cfg.py but with asm_ast atoms. Coverage:
  - Empty function: ENTRY → EXIT directly.
  - Single straight-line block ending in `Return` → ENTRY → B → EXIT.
  - Fall-through partitioning at `Label`.
  - Mid-function `Return` closes the current block.
  - Forward `Jump` to a later label.
  - `Branch(_, L)` produces two successors (taken + fall-through).
  - Backward jump (loop).
  - Trailing block without an explicit terminator falls through to
    EXIT.
  - Legacy `Ret(...)` is accepted as a terminator (so the CFG can
    operate on either bare-exit or full-frame asm).
  - `Call` / `AllocateStack` are NOT terminators — they ride
    inside the current block.
  - `cfg_to_function` round-trips an unmodified CFG.
  - Dominance + DF on hand-constructed graphs.
"""

from __future__ import annotations

import unittest

import asm_ast
from passes.optimization_asm.cfg import (
    ENTRY_ID,
    EXIT_ID,
    build_cfg,
    cfg_to_function,
    dominance_frontiers,
    dominator_tree_children,
    immediate_dominators,
    reverse_postorder,
)


def _imm(v: int) -> asm_ast.Imm:
    return asm_ast.Imm(value=v)


def _A() -> asm_ast.Reg:
    return asm_ast.Reg(reg=asm_ast.A())


def _ps(name: str, off: int = 0) -> asm_ast.Pseudo:
    return asm_ast.Pseudo(name=name, offset=off)


def _mov(src, dst) -> asm_ast.Mov:
    return asm_ast.Mov(src=src, dst=dst)


def _ret_bare(save_a: bool = True) -> asm_ast.Return:
    return asm_ast.Return(save_a=save_a)


def _ret_full(save_a: bool = True) -> asm_ast.Ret:
    return asm_ast.Ret(arg_bytes=0, local_bytes=0, save_a=save_a,
                       callee_saved_addrs=[])


def _label(name: str) -> asm_ast.Label:
    return asm_ast.Label(name=name)


def _jump(target: str) -> asm_ast.Jump:
    return asm_ast.Jump(target=target)


def _branch(target: str) -> asm_ast.Branch:
    # Condition is irrelevant for CFG shape; pick EQ.
    return asm_ast.Branch(cond=asm_ast.EQ(), target=target)


def _fn(*instrs, name: str = "main", params=()) -> asm_ast.Function:
    return asm_ast.Function(
        name=name,
        is_global=True,
        params=list(params),
        instructions=list(instrs),
    )


class TestBuildAsmCFG(unittest.TestCase):
    def test_empty_function_entry_to_exit(self) -> None:
        cfg = build_cfg(_fn())
        self.assertEqual(cfg.block_order, [])
        self.assertEqual(cfg.blocks[ENTRY_ID].successors, [EXIT_ID])
        self.assertEqual(cfg.blocks[EXIT_ID].predecessors, [ENTRY_ID])

    def test_single_block_bare_return(self) -> None:
        fn = _fn(_mov(_imm(1), _ps("x")), _ret_bare())
        cfg = build_cfg(fn)
        self.assertEqual(len(cfg.block_order), 1)
        bid = cfg.block_order[0]
        self.assertEqual(cfg.blocks[ENTRY_ID].successors, [bid])
        self.assertEqual(cfg.blocks[bid].successors, [EXIT_ID])
        self.assertEqual(cfg.blocks[EXIT_ID].predecessors, [bid])

    def test_single_block_legacy_ret(self) -> None:
        # Legacy compound Ret(...) counts as a terminator too.
        fn = _fn(_mov(_imm(1), _ps("x")), _ret_full())
        cfg = build_cfg(fn)
        self.assertEqual(len(cfg.block_order), 1)
        bid = cfg.block_order[0]
        self.assertEqual(cfg.blocks[bid].successors, [EXIT_ID])

    def test_label_splits_block(self) -> None:
        # Two block: B0 = [Mov], B1 = [Label, Ret]. B0 falls through
        # to B1.
        fn = _fn(
            _mov(_imm(1), _ps("x")),
            _label("L"),
            _ret_bare(),
        )
        cfg = build_cfg(fn)
        self.assertEqual(len(cfg.block_order), 2)
        b0, b1 = cfg.block_order
        self.assertEqual(cfg.blocks[b0].successors, [b1])
        self.assertEqual(cfg.blocks[b1].predecessors, [b0])
        self.assertEqual(cfg.blocks[b1].successors, [EXIT_ID])
        # First instr of B1 is the label.
        self.assertIsInstance(cfg.blocks[b1].instructions[0], asm_ast.Label)

    def test_jump_to_forward_label(self) -> None:
        # B0 = [Mov, Jump L], B1 = [Mov(2), Ret] (unreachable),
        # B2 = [Label L, Ret].
        fn = _fn(
            _mov(_imm(1), _ps("x")),
            _jump("L"),
            _mov(_imm(2), _ps("x")),
            _ret_bare(),
            _label("L"),
            _ret_bare(),
        )
        cfg = build_cfg(fn)
        b0, b1, b2 = cfg.block_order
        # B0 → B2 only (Jump is unconditional).
        self.assertEqual(cfg.blocks[b0].successors, [b2])
        # B1 is unreachable from ENTRY but its Return wires to EXIT.
        self.assertEqual(cfg.blocks[b1].successors, [EXIT_ID])
        # B2 has only B0 as predecessor — B1's Return goes to EXIT,
        # not to B2.
        self.assertEqual(cfg.blocks[b2].predecessors, [b0])

    def test_branch_two_successors(self) -> None:
        # B0 = [Mov, Branch L], B1 = [Mov(2), Ret], B2 = [Label L, Ret].
        # Branch produces taken (L) AND fall-through (B1).
        fn = _fn(
            _mov(_imm(1), _ps("x")),
            _branch("L"),
            _mov(_imm(2), _ps("x")),
            _ret_bare(),
            _label("L"),
            _ret_bare(),
        )
        cfg = build_cfg(fn)
        b0, b1, b2 = cfg.block_order
        # B0 → both B2 (taken) and B1 (fall-through). Order: taken
        # first, fall-through second, by the build_cfg edge order.
        self.assertEqual(cfg.blocks[b0].successors, [b2, b1])
        self.assertEqual(cfg.blocks[b1].predecessors, [b0])
        self.assertEqual(cfg.blocks[b2].predecessors, [b0])

    def test_call_does_not_split_block(self) -> None:
        # Call returns to the next instruction, so it doesn't end
        # the block. AllocateStack also doesn't.
        fn = _fn(
            asm_ast.AllocateStack(bytes=2),
            asm_ast.Call(name="foo"),
            _mov(_A(), _ps("y")),
            _ret_bare(),
        )
        cfg = build_cfg(fn)
        self.assertEqual(len(cfg.block_order), 1)
        bid = cfg.block_order[0]
        self.assertEqual(len(cfg.blocks[bid].instructions), 4)
        self.assertEqual(cfg.blocks[bid].successors, [EXIT_ID])

    def test_loop_back_edge(self) -> None:
        # B0 = [Label L, Mov, Branch L]. Backward branch wires B0 to
        # itself (taken) and to fall-through (next block, EXIT-bound).
        fn = _fn(
            _label("L"),
            _mov(_imm(1), _ps("x")),
            _branch("L"),
            _ret_bare(),
        )
        cfg = build_cfg(fn)
        # B0 = [Label, Mov, Branch], B1 = [Ret].
        b0, b1 = cfg.block_order
        self.assertEqual(cfg.blocks[b0].successors, [b0, b1])
        # B0 is its own predecessor via the back-edge AND the ENTRY
        # → first-block edge gives ENTRY too.
        self.assertEqual(set(cfg.blocks[b0].predecessors), {ENTRY_ID, b0})

    def test_no_terminator_falls_through_to_exit(self) -> None:
        fn = _fn(_mov(_imm(1), _ps("x")))
        cfg = build_cfg(fn)
        bid = cfg.block_order[0]
        self.assertEqual(cfg.blocks[bid].successors, [EXIT_ID])

    def test_cfg_to_function_round_trips(self) -> None:
        instrs = [
            _mov(_imm(1), _ps("x")),
            _branch("L"),
            _mov(_imm(2), _ps("x")),
            _ret_bare(),
            _label("L"),
            _ret_bare(),
        ]
        fn = _fn(*instrs)
        cfg = build_cfg(fn)
        out = cfg_to_function(fn, cfg)
        self.assertEqual(out.name, "main")
        self.assertEqual(out.instructions, instrs)


class TestAsmDominance(unittest.TestCase):
    def test_diamond_dominance_frontiers(self) -> None:
        # Diamond: B0 → {B1, B2} → B3.
        # Hand-built via Branch L1 / Jump L3 / Label L1 / Jump L3 /
        # Label L3.
        fn = _fn(
            _mov(_imm(1), _ps("x")),
            _branch("L1"),
            _mov(_imm(2), _ps("x")),
            _jump("L3"),
            _label("L1"),
            _mov(_imm(3), _ps("x")),
            _jump("L3"),
            _label("L3"),
            _ret_bare(),
        )
        cfg = build_cfg(fn)
        idom = immediate_dominators(cfg)
        # Every block's idom is the head of the diamond.
        b0 = cfg.block_order[0]
        b3_id = next(
            bid for bid in cfg.block_order
            if isinstance(cfg.blocks[bid].instructions[0], asm_ast.Label)
            and cfg.blocks[bid].instructions[0].name == "L3"
        )
        self.assertEqual(idom[b3_id], b0)
        df = dominance_frontiers(cfg)
        # The arms (B1 = fall-through after Branch, B2 = labeled L1)
        # both have B3 as their dominance frontier.
        # We can find them by their successor going to B3.
        arm_ids = [
            bid for bid in cfg.block_order
            if b3_id in cfg.blocks[bid].successors and bid != b0
        ]
        self.assertEqual(len(arm_ids), 2)
        for arm in arm_ids:
            self.assertEqual(df[arm], {b3_id})


if __name__ == "__main__":
    unittest.main()
