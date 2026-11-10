"""Tests for the branch-around-jump inversion peephole."""

import unittest

import asm_ast
from passes.branch_invert import apply_branch_invert


# `.T`, the jump target in these tests, is a label at the end of the
# function. The pass leaves a Jump to any other name alone.
_T = asm_ast.Label(name=".T")


def _instrs(prog: asm_ast.Program) -> list[asm_ast.Type_instruction]:
    out = prog.top_level[0].instructions
    assert out[-1] == _T
    return out[:-1]


def _wrap(instrs):
    return asm_ast.Program(top_level=[asm_ast.Function(
        name="f", is_global=False, params=[], instructions=instrs + [_T],
    )])


class TestBranchInvert(unittest.TestCase):

    def test_branch_around_jump_collapses_to_inverted_branch(self):
        # Branch(CC, L); Jump(T); Label(L) → Branch(CS, T); Label(L).
        # CC's invert is CS.
        prog = _wrap([
            asm_ast.Branch(cond=asm_ast.CC(), target=".L"),
            asm_ast.Jump(target=".T"),
            asm_ast.Label(name=".L"),
            asm_ast.Label(name=".other"),
        ])
        out = _instrs(apply_branch_invert(prog))
        self.assertEqual(out, [
            asm_ast.Branch(cond=asm_ast.CS(), target=".T"),
            asm_ast.Label(name=".L"),
            asm_ast.Label(name=".other"),
        ])

    def test_all_conditions_invert(self):
        pairs = [
            (asm_ast.EQ, asm_ast.NE), (asm_ast.NE, asm_ast.EQ),
            (asm_ast.CC, asm_ast.CS), (asm_ast.CS, asm_ast.CC),
            (asm_ast.MI, asm_ast.PL), (asm_ast.PL, asm_ast.MI),
            (asm_ast.VC, asm_ast.VS), (asm_ast.VS, asm_ast.VC),
        ]
        for orig, inv in pairs:
            prog = _wrap([
                asm_ast.Branch(cond=orig(), target=".L"),
                asm_ast.Jump(target=".T"),
                asm_ast.Label(name=".L"),
            ])
            out = _instrs(apply_branch_invert(prog))
            self.assertEqual(out[0], asm_ast.Branch(cond=inv(), target=".T"),
                f"failed for {orig.__name__}")

    def test_label_mismatch_doesnt_match(self):
        # Branch targets a different label than the one that follows
        # the Jump → not the pattern, leave alone.
        prog = _wrap([
            asm_ast.Branch(cond=asm_ast.EQ(), target=".other"),
            asm_ast.Jump(target=".T"),
            asm_ast.Label(name=".L"),
        ])
        self.assertEqual(_instrs(apply_branch_invert(prog)), _instrs(prog))

    def test_label_preserved_for_other_jump_targets(self):
        # The label is preserved because some other instruction might
        # still target it (e.g. a goto, or a fall-through dispatch
        # chain). Dead-label cleanup is a separate concern.
        prog = _wrap([
            asm_ast.Jump(target=".L"),                 # someone else jumps to L
            asm_ast.Branch(cond=asm_ast.EQ(), target=".L"),
            asm_ast.Jump(target=".T"),
            asm_ast.Label(name=".L"),
        ])
        out = _instrs(apply_branch_invert(prog))
        self.assertIn(asm_ast.Label(name=".L"), out)
        self.assertEqual(
            out[1], asm_ast.Branch(cond=asm_ast.NE(), target=".T"),
        )

    def test_non_adjacent_doesnt_match(self):
        # Anything between Branch and Jump (a Compare, a Mov, an extra
        # Label) defeats the match. Branch-around-jump from the 0/1
        # ordering-result materialization has a `Mov(0, A)` in between,
        # so it's not in this pass's scope.
        prog = _wrap([
            asm_ast.Branch(cond=asm_ast.EQ(), target=".L"),
            asm_ast.Mov(src=asm_ast.Imm(value=0), dst=asm_ast.Reg(reg=asm_ast.A())),
            asm_ast.Jump(target=".T"),
            asm_ast.Label(name=".L"),
        ])
        self.assertEqual(_instrs(apply_branch_invert(prog)), _instrs(prog))

    def test_tail_call_stays_a_jump(self):
        # `if (c == 13) { other(); return; }` ends in a tail call,
        # `JMP other`. A branch to another function's label can't be
        # range-checked, so the JMP stays.
        prog = _wrap([
            asm_ast.Branch(cond=asm_ast.NE(), target=".L"),
            asm_ast.Jump(target="other"),
            asm_ast.Label(name=".L"),
        ])
        self.assertEqual(_instrs(apply_branch_invert(prog)), _instrs(prog))

    def test_idempotent(self):
        prog = _wrap([
            asm_ast.Branch(cond=asm_ast.CC(), target=".L"),
            asm_ast.Jump(target=".T"),
            asm_ast.Label(name=".L"),
        ])
        once = apply_branch_invert(prog)
        twice = apply_branch_invert(once)
        self.assertEqual(once, twice)


if __name__ == "__main__":
    unittest.main()
