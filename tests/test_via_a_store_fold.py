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


class TestCopyThroughIndex(unittest.TestCase):
    """`STA t; LDA M; STA D; LDA t` as `LDX M; STX D`, as at the entry
    of Zork's `do_var`."""

    T = asm_ast.Data(name="__local_f__t", offset=0)
    M = asm_ast.Data(name="OPERANDS", offset=0)
    D = asm_ast.Data(name="__local_f__d", offset=0)

    def _copy(self, after):
        return [
            asm_ast.Mov(src=A, dst=self.T),
            asm_ast.Mov(src=self.M, dst=A),
            asm_ast.Mov(src=A, dst=self.D),
            asm_ast.Mov(src=self.T, dst=A),
            *after,
            asm_ast.Return(save_a=False),
        ]

    def _run(self, instrs):
        from passes.via_a_store_fold import apply_copy_through_index
        fn = asm_ast.Function(
            name="f", is_global=True, params=[], instructions=instrs,
        )
        out = apply_copy_through_index(asm_ast.Program(top_level=[fn]))
        return out.top_level[0].instructions

    def test_through_x(self):
        cmp = asm_ast.Compare(left=A, right=asm_ast.Imm(value=10))
        out = self._run(self._copy([cmp]))
        self.assertEqual(out[:3], [
            asm_ast.Mov(src=self.M, dst=X),
            asm_ast.Mov(src=X, dst=self.D),
            cmp,
        ])

    def test_through_y_when_x_is_read(self):
        out = self._run(self._copy([
            asm_ast.Compare(left=A, right=asm_ast.Imm(value=10)),
            asm_ast.Mov(src=X, dst=self.D),
        ]))
        self.assertEqual(out[0], asm_ast.Mov(src=self.M, dst=Y))

    def test_kept_when_the_flags_are_read(self):
        instrs = self._copy([asm_ast.Branch(cond=asm_ast.EQ(), target=".l"),
                             asm_ast.Label(name=".l")])
        self.assertEqual(self._run(list(instrs)), instrs)

    def test_kept_when_the_slot_is_read_later(self):
        instrs = self._copy([
            asm_ast.Compare(left=A, right=asm_ast.Imm(value=10)),
            asm_ast.Mov(src=self.T, dst=A),
            asm_ast.Mov(src=A, dst=asm_ast.Data(name="g", offset=0)),
        ])
        self.assertEqual(self._run(list(instrs)), instrs)


if __name__ == "__main__":
    unittest.main()
