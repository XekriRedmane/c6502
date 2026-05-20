"""Tests for the branch-to-next-instruction peephole."""

import unittest

import asm_ast
from passes.branch_to_next_drop import apply_branch_to_next_drop


def _instrs(prog: asm_ast.Program) -> list[asm_ast.Type_instruction]:
    return prog.top_level[0].instructions


def _wrap(instrs):
    return asm_ast.Program(top_level=[asm_ast.Function(
        name="f", is_global=False, params=[], instructions=instrs,
    )])


class TestBranchToNextDrop(unittest.TestCase):

    def test_branch_to_next_label_dropped(self):
        # Branch(EQ, L); Label(L) → (both dropped). The branch is a
        # no-op, and L is now orphan (no other Branch/Jump targets
        # it), so the Label drops too.
        prog = _wrap([
            asm_ast.Branch(cond=asm_ast.EQ(), target=".L"),
            asm_ast.Label(name=".L"),
        ])
        out = _instrs(apply_branch_to_next_drop(prog))
        self.assertEqual(out, [])

    def test_all_conditions_drop(self):
        for cls in (asm_ast.EQ, asm_ast.NE, asm_ast.CC, asm_ast.CS,
                    asm_ast.MI, asm_ast.PL, asm_ast.VC, asm_ast.VS):
            prog = _wrap([
                asm_ast.Branch(cond=cls(), target=".L"),
                asm_ast.Label(name=".L"),
            ])
            out = _instrs(apply_branch_to_next_drop(prog))
            self.assertEqual(out, [], f"failed for {cls.__name__}")

    def test_branch_to_non_adjacent_label_kept(self):
        # An intervening instruction between Branch and Label means
        # the Branch is meaningful — keep it.
        prog = _wrap([
            asm_ast.Branch(cond=asm_ast.EQ(), target=".L"),
            asm_ast.Mov(
                src=asm_ast.Imm(value=0),
                dst=asm_ast.Reg(reg=asm_ast.A()),
            ),
            asm_ast.Label(name=".L"),
        ])
        out = _instrs(apply_branch_to_next_drop(prog))
        self.assertEqual(out, prog.top_level[0].instructions)

    def test_branch_to_different_label_kept(self):
        # Branch targets a label that isn't the immediately following
        # one — keep.
        prog = _wrap([
            asm_ast.Branch(cond=asm_ast.EQ(), target=".other"),
            asm_ast.Label(name=".L"),
        ])
        out = _instrs(apply_branch_to_next_drop(prog))
        self.assertEqual(out, prog.top_level[0].instructions)

    def test_label_preserved_for_other_refs(self):
        # The Label is intentionally left intact — another Branch /
        # Jump elsewhere may still target it. dead_label_drop reaps
        # truly orphaned labels in a separate pass.
        prog = _wrap([
            asm_ast.Branch(cond=asm_ast.MI(), target=".L"),
            asm_ast.Label(name=".L"),
            asm_ast.Mov(
                src=asm_ast.Imm(value=0),
                dst=asm_ast.Reg(reg=asm_ast.A()),
            ),
            asm_ast.Jump(target=".L"),
        ])
        out = _instrs(apply_branch_to_next_drop(prog))
        self.assertEqual(out, [
            asm_ast.Label(name=".L"),
            asm_ast.Mov(
                src=asm_ast.Imm(value=0),
                dst=asm_ast.Reg(reg=asm_ast.A()),
            ),
            asm_ast.Jump(target=".L"),
        ])

    def test_residual_sx_diamond_collapses(self):
        # The headline case: the sign-extension diamond residue.
        # `Mov(X, A); Branch(PL, sx_done); Label(sx_done); ...` —
        # the Branch is dropped, and Label(sx_done) is dropped too
        # because nothing else targets it. The orphan TXA is the
        # dead_a_arith / redundant_load's problem; this pass only
        # handles the branch + orphan label.
        prog = _wrap([
            asm_ast.Mov(
                src=asm_ast.Reg(reg=asm_ast.X()),
                dst=asm_ast.Reg(reg=asm_ast.A()),
            ),
            asm_ast.Branch(cond=asm_ast.PL(), target=".sx_done@1"),
            asm_ast.Label(name=".sx_done@1"),
            asm_ast.Mov(
                src=asm_ast.Reg(reg=asm_ast.X()),
                dst=asm_ast.Reg(reg=asm_ast.A()),
            ),
        ])
        out = _instrs(apply_branch_to_next_drop(prog))
        self.assertEqual(out, [
            asm_ast.Mov(
                src=asm_ast.Reg(reg=asm_ast.X()),
                dst=asm_ast.Reg(reg=asm_ast.A()),
            ),
            asm_ast.Mov(
                src=asm_ast.Reg(reg=asm_ast.X()),
                dst=asm_ast.Reg(reg=asm_ast.A()),
            ),
        ])

    def test_stacked_branches_to_same_label(self):
        # Two stacked Branches both targeting the immediately-following
        # Label. First sweep: i=0 (Branch(EQ)), i+1 is Branch(NE) —
        # not Label, keep. i=1 (Branch(NE)), i+1 is Label(.L) — drop
        # the inner Branch. The Label has ref_count=2 (both Branches);
        # we just dropped one ref, so 2 > 1 → keep the Label.
        # Resulting first-pass output: Branch(EQ, .L); Label(.L).
        # Second pass: now Branch(EQ); Label adjacent, ref_count=1
        # so both drop.
        prog = _wrap([
            asm_ast.Branch(cond=asm_ast.EQ(), target=".L"),
            asm_ast.Branch(cond=asm_ast.NE(), target=".L"),
            asm_ast.Label(name=".L"),
        ])
        out = _instrs(apply_branch_to_next_drop(prog))
        self.assertEqual(out, [
            asm_ast.Branch(cond=asm_ast.EQ(), target=".L"),
            asm_ast.Label(name=".L"),
        ])
        out2 = _instrs(apply_branch_to_next_drop(_wrap(out)))
        self.assertEqual(out2, [])

    def test_branch_to_end_label_kept(self):
        # Branch with no instructions after it (and no matching label
        # at i+1) — keep.
        prog = _wrap([
            asm_ast.Branch(cond=asm_ast.EQ(), target=".L"),
        ])
        out = _instrs(apply_branch_to_next_drop(prog))
        self.assertEqual(out, prog.top_level[0].instructions)

    def test_idempotent(self):
        # Both Branches drop, AND both Labels drop (each has just
        # the one reference from the about-to-be-dropped Branch).
        prog = _wrap([
            asm_ast.Branch(cond=asm_ast.CC(), target=".L"),
            asm_ast.Label(name=".L"),
            asm_ast.Branch(cond=asm_ast.NE(), target=".M"),
            asm_ast.Label(name=".M"),
        ])
        once = apply_branch_to_next_drop(prog)
        twice = apply_branch_to_next_drop(once)
        self.assertEqual(once, twice)
        self.assertEqual(_instrs(once), [])

    def test_label_with_jump_target_kept(self):
        # The Label is reachable from a Jump elsewhere — only the
        # Branch drops; the Label stays.
        prog = _wrap([
            asm_ast.Branch(cond=asm_ast.EQ(), target=".L"),
            asm_ast.Label(name=".L"),
            asm_ast.Jump(target=".L"),
        ])
        out = _instrs(apply_branch_to_next_drop(prog))
        self.assertEqual(out, [
            asm_ast.Label(name=".L"),
            asm_ast.Jump(target=".L"),
        ])


if __name__ == "__main__":
    unittest.main()
