"""Tests for the DEC/INC + LDA + Branch peephole."""

import unittest

import asm_ast
from passes.dec_inc_branch_fold import apply_dec_inc_branch_fold


_A = asm_ast.Reg(reg=asm_ast.A())


# The length of the tail the last `_wrap` added.
_TAIL = [0]


def _instrs(prog):
    """The function's instructions without the tail `_wrap` adds."""
    out = prog.top_level[0].instructions
    return out[:len(out) - _TAIL[0]]


def _wrap(instrs):
    """`instrs` as a function, followed by a label and a return for
    each branch target that isn't already a label in it. The passes
    check A at a branch's target, and a target that isn't in the
    function counts as live."""
    labels = {i.name for i in instrs if isinstance(i, asm_ast.Label)}
    tail = []
    for i in instrs:
        if isinstance(i, asm_ast.Branch) and i.target not in labels:
            labels.add(i.target)
            tail += [asm_ast.Label(name=i.target), asm_ast.Return(save_a=False)]
    fn = asm_ast.Function(
        name="f", is_global=False, params=[], instructions=instrs + tail,
    )
    _TAIL[0] = len(tail)
    return asm_ast.Program(top_level=[fn])


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


class TestBranchTarget(unittest.TestCase):

    def test_a_read_at_target_blocks(self):
        # `t = --timer; if (t == 0) {...} return t;` reads t in A at
        # the branch target, so the LDA stays.
        m = asm_ast.Data(name="timer", offset=0)
        instrs = [
            asm_ast.Dec(dst=m),
            asm_ast.Mov(src=m, dst=_A),
            asm_ast.Branch(cond=asm_ast.NE(), target=".t"),
            asm_ast.Mov(src=asm_ast.Imm(value=7), dst=_A),
            asm_ast.Return(save_a=True),
            asm_ast.Label(name=".t"),
            asm_ast.Return(save_a=True),
        ]
        self.assertEqual(_instrs(apply_dec_inc_branch_fold(_wrap(instrs))),
                         instrs)


if __name__ == "__main__":
    unittest.main()
