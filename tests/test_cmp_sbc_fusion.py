"""Tests for the CMP/SBC fusion peephole."""

import unittest

import asm_ast
from passes.cmp_sbc_fusion import apply_cmp_sbc_fusion


_A = asm_ast.Reg(reg=asm_ast.A())


def _instrs(prog):
    return prog.top_level[0].instructions


def _wrap(instrs):
    return asm_ast.Program(top_level=[asm_ast.Function(
        name="f", is_global=False, params=[], instructions=instrs,
    )])


class TestCmpSbcFusion(unittest.TestCase):

    def test_lda_cmp_bcc_label_sec_sbc_fuses(self):
        N = asm_ast.Data(name="N", offset=0)
        prog = _wrap([
            asm_ast.Mov(src=asm_ast.Data(name="M", offset=0), dst=_A),
            asm_ast.Compare(left=_A, right=N),
            asm_ast.Branch(cond=asm_ast.CC(), target=".skip"),
            asm_ast.Label(name=".cont"),
            asm_ast.SetCarry(),
            asm_ast.Sub(src=N, dst=_A),
            asm_ast.Return(save_a=False),
            asm_ast.Label(name=".skip"),
            asm_ast.Return(save_a=False),
        ])
        out = _instrs(apply_cmp_sbc_fusion(prog))
        self.assertEqual(out, [
            asm_ast.Mov(src=asm_ast.Data(name="M", offset=0), dst=_A),
            asm_ast.SetCarry(),
            asm_ast.Sub(src=N, dst=_A),
            asm_ast.Branch(cond=asm_ast.CC(), target=".skip"),
            asm_ast.Label(name=".cont"),
            asm_ast.Return(save_a=False),
            asm_ast.Label(name=".skip"),
            asm_ast.Return(save_a=False),
        ])

    def test_fuses_without_a_label(self):
        N = asm_ast.Data(name="N", offset=0)
        T = asm_ast.Data(name="T", offset=0)
        prog = _wrap([
            asm_ast.Mov(src=asm_ast.Data(name="M", offset=0), dst=_A),
            asm_ast.Compare(left=_A, right=N),
            asm_ast.Branch(cond=asm_ast.CC(), target=".skip"),
            asm_ast.SetCarry(),
            asm_ast.Sub(src=N, dst=_A),
            asm_ast.Mov(src=_A, dst=T),
            asm_ast.Label(name=".skip"),
            asm_ast.Return(save_a=False),
        ])
        out = _instrs(apply_cmp_sbc_fusion(prog))
        self.assertEqual(out, [
            asm_ast.Mov(src=asm_ast.Data(name="M", offset=0), dst=_A),
            asm_ast.SetCarry(),
            asm_ast.Sub(src=N, dst=_A),
            asm_ast.Branch(cond=asm_ast.CC(), target=".skip"),
            asm_ast.Mov(src=_A, dst=T),
            asm_ast.Label(name=".skip"),
            asm_ast.Return(save_a=False),
        ])

    def test_bcc_without_sec(self):
        # carry_known drops the SEC after a BCC, whose fall-through
        # has C set; the moved SBC gets it back.
        N = asm_ast.Data(name="N", offset=0)
        T = asm_ast.Data(name="T", offset=0)
        prog = _wrap([
            asm_ast.Mov(src=asm_ast.Data(name="M", offset=0), dst=_A),
            asm_ast.Compare(left=_A, right=N),
            asm_ast.Branch(cond=asm_ast.CC(), target=".skip"),
            asm_ast.Sub(src=N, dst=_A),
            asm_ast.Mov(src=_A, dst=T),
            asm_ast.Label(name=".skip"),
            asm_ast.Return(save_a=False),
        ])
        out = _instrs(apply_cmp_sbc_fusion(prog))
        self.assertEqual(out[:4], [
            asm_ast.Mov(src=asm_ast.Data(name="M", offset=0), dst=_A),
            asm_ast.SetCarry(),
            asm_ast.Sub(src=N, dst=_A),
            asm_ast.Branch(cond=asm_ast.CC(), target=".skip"),
        ])

    def test_bcs_without_sec_left_alone(self):
        # After BCS the fall-through has C clear, so the SBC there
        # subtracts one more; it isn't the compare's subtraction.
        N = asm_ast.Data(name="N", offset=0)
        prog = _wrap([
            asm_ast.Mov(src=asm_ast.Data(name="M", offset=0), dst=_A),
            asm_ast.Compare(left=_A, right=N),
            asm_ast.Branch(cond=asm_ast.CS(), target=".skip"),
            asm_ast.Sub(src=N, dst=_A),
            asm_ast.Label(name=".skip"),
            asm_ast.Return(save_a=False),
        ])
        out = _instrs(apply_cmp_sbc_fusion(prog))
        self.assertIsInstance(out[1], asm_ast.Compare)

    def test_a_live_at_the_target_blocks_fusion(self):
        # `.skip` returns A, which the CMP left as M.
        N = asm_ast.Imm(value=3)
        prog = _wrap([
            asm_ast.Mov(src=asm_ast.Data(name="M", offset=0), dst=_A),
            asm_ast.Compare(left=_A, right=N),
            asm_ast.Branch(cond=asm_ast.CC(), target=".skip"),
            asm_ast.SetCarry(),
            asm_ast.Sub(src=N, dst=_A),
            asm_ast.Label(name=".skip"),
            asm_ast.Return(save_a=True),
        ])
        out = _instrs(apply_cmp_sbc_fusion(prog))
        self.assertIsInstance(out[1], asm_ast.Compare)

    def test_label_in_branch_targets_blocks_fusion(self):
        N = asm_ast.Data(name="N", offset=0)
        # Make .cont a branch target — fusion should NOT fire because
        # some other path could reach .cont with a different A.
        prog = _wrap([
            asm_ast.Jump(target=".cont"),
            asm_ast.Mov(src=asm_ast.Data(name="M", offset=0), dst=_A),
            asm_ast.Compare(left=_A, right=N),
            asm_ast.Branch(cond=asm_ast.CC(), target=".skip"),
            asm_ast.Label(name=".cont"),
            asm_ast.SetCarry(),
            asm_ast.Sub(src=N, dst=_A),
        ])
        out = _instrs(apply_cmp_sbc_fusion(prog))
        # Unchanged.
        self.assertEqual(len(out), 7)
        self.assertIsInstance(out[2], asm_ast.Compare)

    def test_different_operands_dont_fuse(self):
        # CMP and SBC against different operands — can't fuse.
        prog = _wrap([
            asm_ast.Mov(src=asm_ast.Data(name="M", offset=0), dst=_A),
            asm_ast.Compare(left=_A, right=asm_ast.Data(name="N1", offset=0)),
            asm_ast.Branch(cond=asm_ast.CC(), target=".skip"),
            asm_ast.Label(name=".cont"),
            asm_ast.SetCarry(),
            asm_ast.Sub(src=asm_ast.Data(name="N2", offset=0), dst=_A),
        ])
        out = _instrs(apply_cmp_sbc_fusion(prog))
        self.assertEqual(len(out), 6)
        self.assertIsInstance(out[1], asm_ast.Compare)

    def test_bvc_branch_skips_fusion(self):
        # BVC/BVS read V; SBC sets V but CMP doesn't, so fusion
        # would change the V flag observed by the branch.
        N = asm_ast.Data(name="N", offset=0)
        prog = _wrap([
            asm_ast.Mov(src=asm_ast.Data(name="M", offset=0), dst=_A),
            asm_ast.Compare(left=_A, right=N),
            asm_ast.Branch(cond=asm_ast.VC(), target=".skip"),
            asm_ast.Label(name=".cont"),
            asm_ast.SetCarry(),
            asm_ast.Sub(src=N, dst=_A),
        ])
        out = _instrs(apply_cmp_sbc_fusion(prog))
        self.assertEqual(len(out), 6)
        self.assertIsInstance(out[1], asm_ast.Compare)

    def _shape(self, target_tail):
        N = asm_ast.Data(name="N", offset=0)
        return [
            asm_ast.Mov(src=asm_ast.Data(name="M", offset=0), dst=_A),
            asm_ast.Compare(left=_A, right=N),
            asm_ast.Branch(cond=asm_ast.CC(), target=".skip"),
            asm_ast.Label(name=".cont"),
            asm_ast.SetCarry(),
            asm_ast.Sub(src=N, dst=_A),
            asm_ast.Return(save_a=True),
            asm_ast.Label(name=".skip"),
            *target_tail,
        ]

    def test_a_live_at_target_blocks_fusion(self):
        # The taken path returns M in A. After fusion it would return
        # M - N.
        instrs = self._shape([asm_ast.Return(save_a=True)])
        out = _instrs(apply_cmp_sbc_fusion(_wrap(list(instrs))))
        self.assertEqual(out, instrs)

    def test_v_live_at_target_blocks_fusion(self):
        # A `BVS` at the target reads the V that CMP leaves alone and
        # SBC sets.
        instrs = self._shape([
            asm_ast.Branch(cond=asm_ast.VS(), target=".v"),
            asm_ast.Return(save_a=False),
            asm_ast.Label(name=".v"),
            asm_ast.Return(save_a=False),
        ])
        out = _instrs(apply_cmp_sbc_fusion(_wrap(list(instrs))))
        self.assertEqual(out, instrs)

    def test_a_reloaded_at_target_fuses(self):
        instrs = self._shape([
            asm_ast.Mov(src=asm_ast.Imm(value=0), dst=_A),
            asm_ast.Return(save_a=True),
        ])
        out = _instrs(apply_cmp_sbc_fusion(_wrap(list(instrs))))
        self.assertIsInstance(out[1], asm_ast.SetCarry)


if __name__ == "__main__":
    unittest.main()
