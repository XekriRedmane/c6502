"""Tests for `passes.transfer_past_branch.apply_transfer_past_branch`."""

import unittest

import asm_ast
from passes.transfer_past_branch import apply_transfer_past_branch


A = asm_ast.Reg(reg=asm_ast.A())
X = asm_ast.Reg(reg=asm_ast.X())
Y = asm_ast.Reg(reg=asm_ast.Y())


def _run(instrs):
    fn = asm_ast.Function(
        name="f", is_global=True, params=[], instructions=instrs,
    )
    out = apply_transfer_past_branch(asm_ast.Program(top_level=[fn]))
    return out.top_level[0].instructions


class TestSwap(unittest.TestCase):
    def test_ldx_txa_bpl_moves_txa_past_branch(self):
        """LDX M; TXA; BPL T; <A-using fall-through>; T: <A dead>
        → LDX M; BPL T; TXA; <fall-through>."""
        m = asm_ast.Data(name="state", offset=0)
        zp = asm_ast.ZP(address=0x82, offset=0)
        instrs = [
            asm_ast.Mov(src=m, dst=X, is_volatile=False),       # LDX state
            asm_ast.Mov(src=X, dst=A, is_volatile=False),       # TXA
            asm_ast.Branch(cond=asm_ast.PL(), target=".tgt"),    # BPL
            # Fall-through: A is used → live.
            asm_ast.And(src=asm_ast.Imm(value=0x0F), dst=A),
            asm_ast.Mov(src=A, dst=zp, is_volatile=False),
            asm_ast.Return(save_a=False),
            asm_ast.Label(name=".tgt"),
            # Target: A is killed before any read (LDA #0 below).
            asm_ast.Mov(src=asm_ast.Imm(value=0), dst=A, is_volatile=False),
            asm_ast.Return(save_a=False),
        ]
        out = _run(instrs)
        # Expected: instrs[0] = LDX, [1] = Branch, [2] = TXA.
        self.assertIsInstance(out[0], asm_ast.Mov)
        self.assertEqual(out[0].dst, X)
        self.assertIsInstance(out[1], asm_ast.Branch)
        self.assertIsInstance(out[2], asm_ast.Mov)
        self.assertEqual(out[2].src, X)
        self.assertEqual(out[2].dst, A)

    def test_ldy_tya_beq_moves(self):
        """Y mirror of the above."""
        m = asm_ast.Data(name="state", offset=0)
        zp = asm_ast.ZP(address=0x82, offset=0)
        instrs = [
            asm_ast.Mov(src=m, dst=Y, is_volatile=False),       # LDY state
            asm_ast.Mov(src=Y, dst=A, is_volatile=False),       # TYA
            asm_ast.Branch(cond=asm_ast.EQ(), target=".tgt"),
            asm_ast.And(src=asm_ast.Imm(value=0x0F), dst=A),
            asm_ast.Mov(src=A, dst=zp, is_volatile=False),
            asm_ast.Return(save_a=False),
            asm_ast.Label(name=".tgt"),
            asm_ast.Mov(src=asm_ast.Imm(value=0), dst=A, is_volatile=False),
            asm_ast.Return(save_a=False),
        ]
        out = _run(instrs)
        self.assertIsInstance(out[0], asm_ast.Mov)
        self.assertEqual(out[0].dst, Y)
        self.assertIsInstance(out[1], asm_ast.Branch)
        self.assertIsInstance(out[2], asm_ast.Mov)
        self.assertEqual(out[2].src, Y)
        self.assertEqual(out[2].dst, A)


class TestSkips(unittest.TestCase):
    def test_skips_when_a_live_on_branch_target(self):
        """When the target path reads A, we'd lose the TXA's
        A := X effect. Don't move."""
        m = asm_ast.Data(name="state", offset=0)
        zp = asm_ast.ZP(address=0x82, offset=0)
        instrs = [
            asm_ast.Mov(src=m, dst=X, is_volatile=False),
            asm_ast.Mov(src=X, dst=A, is_volatile=False),
            asm_ast.Branch(cond=asm_ast.PL(), target=".tgt"),
            asm_ast.And(src=asm_ast.Imm(value=0x0F), dst=A),
            asm_ast.Mov(src=A, dst=zp, is_volatile=False),
            asm_ast.Return(save_a=False),
            asm_ast.Label(name=".tgt"),
            # Target USES A.
            asm_ast.Mov(src=A, dst=zp, is_volatile=False),
            asm_ast.Return(save_a=False),
        ]
        out = _run(instrs)
        # TXA stays before the Branch.
        self.assertIsInstance(out[1], asm_ast.Mov)
        self.assertEqual(out[1].src, X)
        self.assertEqual(out[1].dst, A)
        self.assertIsInstance(out[2], asm_ast.Branch)

    def test_skips_when_a_dead_on_both_arms(self):
        """If A is dead on BOTH arms, let `dead_a_arith` drop the
        TXA — no motion needed."""
        m = asm_ast.Data(name="state", offset=0)
        instrs = [
            asm_ast.Mov(src=m, dst=X, is_volatile=False),
            asm_ast.Mov(src=X, dst=A, is_volatile=False),
            asm_ast.Branch(cond=asm_ast.PL(), target=".tgt"),
            # Fall-through: A immediately killed.
            asm_ast.Mov(src=asm_ast.Imm(value=0), dst=A, is_volatile=False),
            asm_ast.Return(save_a=False),
            asm_ast.Label(name=".tgt"),
            # Target: A immediately killed.
            asm_ast.Mov(src=asm_ast.Imm(value=0), dst=A, is_volatile=False),
            asm_ast.Return(save_a=False),
        ]
        out = _run(instrs)
        # No swap — TXA stays where it was.
        self.assertIsInstance(out[1], asm_ast.Mov)
        self.assertEqual(out[1].src, X)
        self.assertEqual(out[1].dst, A)

    def test_skips_when_prev_doesnt_set_x_flags(self):
        """LDA M; TXA; B?? — the LDA doesn't set flags from X, so
        TXA's flag side effect IS observable post-LDA. Don't swap."""
        m = asm_ast.Data(name="state", offset=0)
        zp = asm_ast.ZP(address=0x82, offset=0)
        instrs = [
            # LDA M — sets flags from M, not X.
            asm_ast.Mov(src=m, dst=A, is_volatile=False),
            asm_ast.Mov(src=X, dst=A, is_volatile=False),  # TXA — would set flags from X
            asm_ast.Branch(cond=asm_ast.PL(), target=".tgt"),
            asm_ast.And(src=asm_ast.Imm(value=0x0F), dst=A),
            asm_ast.Mov(src=A, dst=zp, is_volatile=False),
            asm_ast.Return(save_a=False),
            asm_ast.Label(name=".tgt"),
            asm_ast.Mov(src=asm_ast.Imm(value=0), dst=A, is_volatile=False),
            asm_ast.Return(save_a=False),
        ]
        out = _run(instrs)
        # TXA stays.
        self.assertIsInstance(out[1], asm_ast.Mov)
        self.assertEqual(out[1].src, X)
        self.assertEqual(out[1].dst, A)

    def test_skips_when_target_external(self):
        """Branch target outside the function (tail call / long-
        branch trampoline): can't analyze A there. Skip."""
        m = asm_ast.Data(name="state", offset=0)
        zp = asm_ast.ZP(address=0x82, offset=0)
        instrs = [
            asm_ast.Mov(src=m, dst=X, is_volatile=False),
            asm_ast.Mov(src=X, dst=A, is_volatile=False),
            asm_ast.Branch(cond=asm_ast.PL(), target="external_label"),
            asm_ast.And(src=asm_ast.Imm(value=0x0F), dst=A),
            asm_ast.Mov(src=A, dst=zp, is_volatile=False),
            asm_ast.Return(save_a=False),
        ]
        out = _run(instrs)
        self.assertIsInstance(out[1], asm_ast.Mov)
        self.assertEqual(out[1].src, X)


if __name__ == "__main__":
    unittest.main()
