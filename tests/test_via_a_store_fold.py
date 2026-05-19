"""Tests for `passes.via_a_store_fold.apply_via_a_store_fold`."""

import unittest

import asm_ast
from passes.via_a_store_fold import apply_via_a_store_fold


A = asm_ast.Reg(reg=asm_ast.A())
X = asm_ast.Reg(reg=asm_ast.X())
Y = asm_ast.Reg(reg=asm_ast.Y())


def _run(instrs):
    fn = asm_ast.Function(
        name="f", is_global=True, params=[], instructions=instrs,
    )
    out = apply_via_a_store_fold(asm_ast.Program(top_level=[fn]))
    return out.top_level[0].instructions


class TestFold(unittest.TestCase):
    def test_txa_sta_data_folds(self):
        m = asm_ast.Data(name="m", offset=0)
        # Flag-dead-at + a-dead-at follows because Return is the next
        # instruction.
        out = _run([
            asm_ast.Mov(src=X, dst=A, is_volatile=False),
            asm_ast.Mov(src=A, dst=m, is_volatile=False),
            asm_ast.Return(save_a=False),
        ])
        self.assertEqual(len(out), 2)
        self.assertEqual(out[0].src, X)
        self.assertEqual(out[0].dst, m)
        self.assertIsInstance(out[1], asm_ast.Return)

    def test_tya_sta_zp_folds(self):
        m = asm_ast.ZP(address=0x85, offset=0)
        out = _run([
            asm_ast.Mov(src=Y, dst=A, is_volatile=False),
            asm_ast.Mov(src=A, dst=m, is_volatile=False),
            asm_ast.Return(save_a=False),
        ])
        self.assertEqual(len(out), 2)
        self.assertEqual(out[0].src, Y)
        self.assertEqual(out[0].dst, m)


class TestSkips(unittest.TestCase):
    def test_skips_when_a_live(self):
        # A is used after the STA → can't drop the TXA.
        m = asm_ast.Data(name="m", offset=0)
        n = asm_ast.Data(name="n", offset=0)
        instrs = [
            asm_ast.Mov(src=X, dst=A, is_volatile=False),
            asm_ast.Mov(src=A, dst=m, is_volatile=False),
            asm_ast.Mov(src=A, dst=n, is_volatile=False),  # reads A
        ]
        out = _run(instrs)
        self.assertEqual(len(out), 3)

    def test_skips_when_flags_live(self):
        # A subsequent Branch reads the flags TXA set — can't fold.
        m = asm_ast.Data(name="m", offset=0)
        instrs = [
            asm_ast.Mov(src=X, dst=A, is_volatile=False),
            asm_ast.Mov(src=A, dst=m, is_volatile=False),
            asm_ast.Branch(cond=asm_ast.NE(), target=".skip"),
            asm_ast.Return(save_a=False),
        ]
        out = _run(instrs)
        # Branch reads N/Z → flags live → no fold.
        self.assertEqual(len(out), 4)
        self.assertEqual(out[0].src, X)
        self.assertEqual(out[0].dst, A)

    def test_skips_indirect_dst(self):
        # STX has no indirect addressing mode; can't fold.
        m = asm_ast.Indirect(offset=0)
        instrs = [
            asm_ast.Mov(src=X, dst=A, is_volatile=False),
            asm_ast.Mov(src=A, dst=m, is_volatile=False),
            asm_ast.Return(save_a=False),
        ]
        out = _run(instrs)
        self.assertEqual(len(out), 3)

    def test_skips_volatile_transfer(self):
        m = asm_ast.Data(name="m", offset=0)
        instrs = [
            asm_ast.Mov(src=X, dst=A, is_volatile=True),
            asm_ast.Mov(src=A, dst=m, is_volatile=False),
            asm_ast.Return(save_a=False),
        ]
        out = _run(instrs)
        self.assertEqual(len(out), 3)


if __name__ == "__main__":
    unittest.main()
