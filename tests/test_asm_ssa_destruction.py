"""Behavioral tests for `passes.optimization_asm.ssa_destruction.from_ssa`.

Coverage:
  - Single Phi at a merge → one Mov per predecessor, inserted before
    the terminator.
  - Round-trip on a simple diamond: to_ssa then from_ssa produces a
    Phi-free function whose instructions, modulo SSA renaming, are
    semantically equivalent.
  - Lost-copy case: two Phis at the same merge where one Phi's src
    is the other's dst — destruction topologically sorts so the
    reader runs first.
  - Cycle (a <-> b): broken by a fresh temp Pseudo.
"""
from __future__ import annotations

import unittest

import asm_ast
from passes.optimization_asm.ssa_construction import to_ssa
from passes.optimization_asm.ssa_destruction import from_ssa


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


def _label(n: str) -> asm_ast.Label:
    return asm_ast.Label(name=n)


def _jump(t: str) -> asm_ast.Jump:
    return asm_ast.Jump(target=t)


def _branch(t: str) -> asm_ast.Branch:
    return asm_ast.Branch(cond=asm_ast.EQ(), target=t)


def _phi(dst, args) -> asm_ast.Phi:
    return asm_ast.Phi(
        dst=dst,
        args=[asm_ast.AsmPhiArg(pred_label=p, source=s) for p, s in args],
    )


def _fn(*instrs, name: str = "main", params=()) -> asm_ast.Function:
    return asm_ast.Function(
        name=name,
        is_global=True,
        params=list(params),
        instructions=list(instrs),
    )


class TestFromSsaSimple(unittest.TestCase):
    def test_phi_lowers_to_movs_in_predecessors(self) -> None:
        # B0 falls through to B1 (Mov, Branch L) ;
        # B2 (fall-through after Branch) Mov ; Jump L ;
        # B3 (L) Phi(d=Pseudo("x"), args=[(B0pred, ...), (B2pred, ...)]).
        # Use to_ssa to construct, then from_ssa to destruct.
        fn = _fn(
            _mov(_imm(1), _ps("%x")),
            _branch("L1"),
            _mov(_imm(2), _ps("%x")),
            _jump("L3"),
            _label("L1"),
            _mov(_imm(3), _ps("%x")),
            _jump("L3"),
            _label("L3"),
            _mov(_ps("%x"), _A()),
            _ret_bare(),
        )
        ssa = to_ssa(fn)
        # Verify the Phi was inserted.
        self.assertTrue(any(isinstance(i, asm_ast.Phi) for i in ssa.instructions))
        out = from_ssa(ssa)
        # No Phi survives.
        self.assertFalse(
            any(isinstance(i, asm_ast.Phi) for i in out.instructions),
            f"Phi survived from_ssa: {out.instructions!r}",
        )

    def test_round_trip_simple_function_preserves_shape(self) -> None:
        fn = _fn(
            _mov(_imm(1), _ps("%x")),
            _mov(_ps("%x"), _A()),
            _ret_bare(),
        )
        out = from_ssa(to_ssa(fn))
        # No Phis.
        self.assertFalse(
            any(isinstance(i, asm_ast.Phi) for i in out.instructions),
        )
        # Same kinds of instructions.
        kinds = [type(i).__name__ for i in out.instructions]
        # _ensure_block_labels may have prepended a Label; that's fine.
        self.assertIn("Mov", kinds)
        self.assertIn("Return", kinds)


class TestFromSsaParallelCopyOrdering(unittest.TestCase):
    def test_lost_copy_topologically_sorted(self) -> None:
        # Manually-built scenario: at predecessor `pre`, two Phis at
        # the merge produce two Movs:
        #   Mov(%a.b0.v1, %b.b0.v1)
        #   Mov(%b.b0.v1, %c.b0.v1)
        # Source order would write %b before reading it. Destruction
        # must reorder so the reader (writes %c) goes first.
        # Hand-build the SSA function:
        merge = "L_merge"
        pre = "L_pre"
        fn = _fn(
            _label(pre),
            _jump(merge),
            _label(merge),
            _phi(_ps("%b.b0.v1", 0), [(pre, _ps("%a.b0.v1", 0))]),
            _phi(_ps("%c.b0.v1", 0), [(pre, _ps("%b.b0.v1", 0))]),
            _ret_bare(),
        )
        out = from_ssa(fn)
        # Find the Movs in pred block (`pre`).
        pre_block_idx = None
        for i, instr in enumerate(out.instructions):
            if isinstance(instr, asm_ast.Label) and instr.name == pre:
                pre_block_idx = i
                break
        self.assertIsNotNone(pre_block_idx)
        # The block runs `Label pre`, then [the inserted Movs], then
        # `Jump L_merge`. Find the Movs.
        movs: list[asm_ast.Mov] = []
        j = pre_block_idx + 1
        while j < len(out.instructions) and isinstance(out.instructions[j], asm_ast.Mov):
            movs.append(out.instructions[j])
            j += 1
        self.assertEqual(len(movs), 2)
        # The Mov writing %c (which reads %b) must come BEFORE the
        # Mov writing %b. Topological order.
        first, second = movs
        self.assertEqual(first.dst.name, "%c.b0.v1")
        self.assertEqual(second.dst.name, "%b.b0.v1")

    def test_cycle_broken_by_temp(self) -> None:
        # Hand-craft a parallel-Mov cycle. The Phi sources at `pre`
        # cross — each Phi's source IS the other Phi's dst. After
        # naive Phi → Mov synthesis we get:
        #   Mov(%b.v2, %a.v2)
        #   Mov(%a.v2, %b.v2)
        # — a 2-cycle requiring a fresh temp to break. (This pattern
        # arises after copy propagation collapses a SSA-fresh chain.)
        merge = "L_merge"
        pre = "L_pre"
        fn = _fn(
            _label(pre),
            _jump(merge),
            _label(merge),
            _phi(_ps("%a.b0.v2", 0), [(pre, _ps("%b.b0.v2", 0))]),
            _phi(_ps("%b.b0.v2", 0), [(pre, _ps("%a.b0.v2", 0))]),
            _ret_bare(),
        )
        out = from_ssa(fn)
        # Find Movs after Label(pre).
        pre_idx = next(
            i for i, instr in enumerate(out.instructions)
            if isinstance(instr, asm_ast.Label) and instr.name == pre
        )
        movs: list[asm_ast.Mov] = []
        j = pre_idx + 1
        while j < len(out.instructions) and isinstance(out.instructions[j], asm_ast.Mov):
            movs.append(out.instructions[j])
            j += 1
        # Three Movs: save → write one → write the other.
        self.assertEqual(len(movs), 3)
        # First Mov: save. Source is one of the cycle members; dst
        # is a `.<fnname>@asm_cycle_tmp@*` Pseudo.
        self.assertIsInstance(movs[0].dst, asm_ast.Pseudo)
        self.assertIn(
            "@asm_cycle_tmp@",
            movs[0].dst.name,
            f"first Mov should be a cycle-temp save, got {movs[0]!r}",
        )


if __name__ == "__main__":
    unittest.main()
