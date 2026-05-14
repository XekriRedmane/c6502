"""Tests for the DEC/INC + LDA + Branch peephole."""

import unittest

import asm_ast
from passes.dec_inc_branch_fold import apply_dec_inc_branch_fold


_A = asm_ast.Reg(reg=asm_ast.A())


def _instrs(prog):
    return prog.top_level[0].instructions


def _wrap(instrs):
    return asm_ast.Program(top_level=[asm_ast.Function(
        name="f", is_global=False, params=[], instructions=instrs,
    )])


class TestDecIncBranchFold(unittest.TestCase):

    def test_dec_lda_bpl_drops_lda(self):
        M = asm_ast.Data(name="M", offset=0)
        prog = _wrap([
            asm_ast.Dec(dst=M),
            asm_ast.Mov(src=M, dst=_A),
            asm_ast.Branch(cond=asm_ast.PL(), target=".loop"),
            asm_ast.Return(save_a=False),
        ])
        out = _instrs(apply_dec_inc_branch_fold(prog))
        self.assertEqual(out, [
            asm_ast.Dec(dst=M),
            asm_ast.Branch(cond=asm_ast.PL(), target=".loop"),
            asm_ast.Return(save_a=False),
        ])

    def test_inc_lda_beq_drops_lda(self):
        M = asm_ast.Data(name="M", offset=0)
        prog = _wrap([
            asm_ast.Inc(dst=M),
            asm_ast.Mov(src=M, dst=_A),
            asm_ast.Branch(cond=asm_ast.EQ(), target=".end"),
            asm_ast.Return(save_a=False),
        ])
        out = _instrs(apply_dec_inc_branch_fold(prog))
        self.assertEqual(len(out), 3)

    def test_passive_label_between_dec_and_lda_allowed(self):
        # A label that nothing branches to is just a continuation
        # marker — fold through it.
        M = asm_ast.Data(name="M", offset=0)
        prog = _wrap([
            asm_ast.Dec(dst=M),
            asm_ast.Label(name=".passive"),
            asm_ast.Mov(src=M, dst=_A),
            asm_ast.Branch(cond=asm_ast.PL(), target=".loop"),
            asm_ast.Return(save_a=False),
        ])
        out = _instrs(apply_dec_inc_branch_fold(prog))
        # Label kept, LDA dropped.
        self.assertEqual(len(out), 4)
        self.assertIsInstance(out[1], asm_ast.Label)

    def test_active_label_blocks_fold(self):
        M = asm_ast.Data(name="M", offset=0)
        prog = _wrap([
            asm_ast.Jump(target=".tgt"),       # makes .tgt a branch target
            asm_ast.Dec(dst=M),
            asm_ast.Label(name=".tgt"),
            asm_ast.Mov(src=M, dst=_A),
            asm_ast.Branch(cond=asm_ast.PL(), target=".loop"),
            asm_ast.Return(save_a=False),
        ])
        out = _instrs(apply_dec_inc_branch_fold(prog))
        # LDA preserved.
        self.assertEqual(len(out), 6)

    def test_bcc_branch_blocks_fold(self):
        # BCC reads C, which DEC doesn't set. The LDA's flag effects
        # don't include C either, so the BCC's behavior is undefined
        # in this 3-instr window. Conservatively skip BCC/BCS.
        M = asm_ast.Data(name="M", offset=0)
        prog = _wrap([
            asm_ast.Dec(dst=M),
            asm_ast.Mov(src=M, dst=_A),
            asm_ast.Branch(cond=asm_ast.CC(), target=".loop"),
            asm_ast.Return(save_a=False),
        ])
        out = _instrs(apply_dec_inc_branch_fold(prog))
        self.assertEqual(len(out), 4)

    def test_a_live_after_branch_blocks_fold(self):
        M = asm_ast.Data(name="M", offset=0)
        prog = _wrap([
            asm_ast.Dec(dst=M),
            asm_ast.Mov(src=M, dst=_A),
            asm_ast.Branch(cond=asm_ast.PL(), target=".loop"),
            # A is read at the fall-through:
            asm_ast.Mov(src=_A, dst=asm_ast.Data(name="X", offset=0)),
            asm_ast.Return(save_a=False),
        ])
        out = _instrs(apply_dec_inc_branch_fold(prog))
        self.assertEqual(len(out), 5)


if __name__ == "__main__":
    unittest.main()
