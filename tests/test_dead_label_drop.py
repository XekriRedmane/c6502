"""Unit tests for `passes.dead_label_drop.apply_dead_label_drop`.

Drops `Label` instances that no `Jump` / `Branch` / `Phi`
references — pure noise from upstream passes (SSA construction's
per-block markers, `apply_branch_invert`'s orphans, etc.) that
also blocks `apply_branch_invert`'s consecutive-pattern match.
"""

import unittest

import asm_ast
from passes.dead_label_drop import apply_dead_label_drop


def _wrap(instrs):
    fn = asm_ast.Function(
        name="f", is_global=True, params=[], instructions=instrs,
    )
    return apply_dead_label_drop(asm_ast.Program(top_level=[fn]))


def _instrs(prog):
    return prog.top_level[0].instructions


class TestDeadLabelDrop(unittest.TestCase):

    def test_drops_unreferenced_label(self):
        instrs = [
            asm_ast.Label(name=".never_targeted"),
            asm_ast.Return(save_a=False),
        ]
        out = _instrs(_wrap(instrs))
        self.assertEqual(len(out), 1)
        self.assertIsInstance(out[0], asm_ast.Return)

    def test_keeps_referenced_label(self):
        instrs = [
            asm_ast.Jump(target=".here"),
            asm_ast.Label(name=".here"),
            asm_ast.Return(save_a=False),
        ]
        out = _instrs(_wrap(instrs))
        self.assertEqual(len(out), 3)
        self.assertEqual(out[1].name, ".here")

    def test_keeps_branch_target(self):
        instrs = [
            asm_ast.Branch(cond=asm_ast.EQ(), target=".tgt"),
            asm_ast.Label(name=".tgt"),
            asm_ast.Return(save_a=False),
        ]
        out = _instrs(_wrap(instrs))
        self.assertEqual(len(out), 3)

    def test_keeps_phi_pred_label(self):
        # Phi.args[k].pred_label names a predecessor label; if the
        # Phi is still present, the label must survive even when
        # no Jump/Branch targets it.
        instrs = [
            asm_ast.Label(name=".pred"),
            asm_ast.Phi(
                dst=asm_ast.Pseudo(name="x", offset=0),
                args=[asm_ast.AsmPhiArg(
                    pred_label=".pred",
                    source=asm_ast.Imm(value=0),
                )],
            ),
            asm_ast.Return(save_a=False),
        ]
        out = _instrs(_wrap(instrs))
        self.assertEqual(len(out), 3)
        self.assertEqual(out[0].name, ".pred")

    def test_drops_multiple_orphans(self):
        # The motivating case: a chain of unreferenced ssa_block
        # labels between meaningful instructions.
        instrs = [
            asm_ast.Mov(
                src=asm_ast.Imm(value=1),
                dst=asm_ast.Reg(reg=asm_ast.A()),
                is_volatile=False,
            ),
            asm_ast.Label(name=".ssa_block@0"),
            asm_ast.Label(name=".ssa_block@1"),
            asm_ast.Label(name=".ssa_block@2"),
            asm_ast.Return(save_a=False),
        ]
        out = _instrs(_wrap(instrs))
        self.assertEqual(len(out), 2)
        self.assertNotIsInstance(out[1], asm_ast.Label)

    def test_no_change_when_all_labels_referenced(self):
        instrs = [
            asm_ast.Jump(target=".a"),
            asm_ast.Label(name=".a"),
            asm_ast.Return(save_a=False),
        ]
        prog_in = asm_ast.Program(top_level=[
            asm_ast.Function(
                name="f", is_global=True, params=[],
                instructions=instrs,
            ),
        ])
        prog_out = apply_dead_label_drop(prog_in)
        # When nothing changes the pass returns the function
        # unchanged — the top-level Program may be a different
        # instance but instructions are identical.
        self.assertEqual(
            prog_out.top_level[0].instructions, instrs,
        )


if __name__ == "__main__":
    unittest.main()
