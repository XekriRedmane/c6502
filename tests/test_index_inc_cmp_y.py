"""Tests for `passes.index_inc_cmp_y.apply_index_inc_cmp_y`."""

import unittest

import asm_ast
from passes.index_inc_cmp_y import apply_index_inc_cmp_y


A = asm_ast.Reg(reg=asm_ast.A())
X = asm_ast.Reg(reg=asm_ast.X())
Y = asm_ast.Reg(reg=asm_ast.Y())


def _run(instrs):
    fn = asm_ast.Function(
        name="f", is_global=True, params=[], instructions=instrs,
    )
    out = apply_index_inc_cmp_y(asm_ast.Program(top_level=[fn]))
    return out.top_level[0].instructions


def _indexed_x(name):
    return asm_ast.IndexedData(name=name, index=asm_ast.X())


class TestInc(unittest.TestCase):
    def test_5_atom_inc_pattern_folds_to_3(self):
        tmp = asm_ast.Data(name="__local_f__0", offset=0)
        other = asm_ast.Data(name="beam_y", offset=0)
        instrs = [
            asm_ast.Mov(src=_indexed_x("floor_ceil"), dst=A, is_volatile=False),
            asm_ast.Mov(src=A, dst=tmp, is_volatile=False),
            asm_ast.Inc(dst=tmp),
            asm_ast.Mov(src=tmp, dst=A, is_volatile=False),
            asm_ast.Compare(left=A, right=other),
            asm_ast.Branch(cond=asm_ast.EQ(), target=".skip"),
            asm_ast.Return(save_a=False),
            asm_ast.Label(name=".skip"),
            asm_ast.Return(save_a=False),
        ]
        out = _run(instrs)
        # Expected first 3: LDY floor_ceil,X ; INY ; CPY beam_y.
        self.assertIsInstance(out[0], asm_ast.Mov)
        self.assertEqual(out[0].dst, Y)
        self.assertEqual(out[0].src, _indexed_x("floor_ceil"))
        self.assertIsInstance(out[1], asm_ast.Inc)
        self.assertEqual(out[1].dst, Y)
        self.assertIsInstance(out[2], asm_ast.Compare)
        self.assertEqual(out[2].left, Y)
        self.assertEqual(out[2].right, other)

    def test_dec_form_folds(self):
        tmp = asm_ast.Data(name="__local_f__0", offset=0)
        other = asm_ast.Data(name="beam_y", offset=0)
        instrs = [
            asm_ast.Mov(src=_indexed_x("tbl"), dst=A, is_volatile=False),
            asm_ast.Mov(src=A, dst=tmp, is_volatile=False),
            asm_ast.Dec(dst=tmp),
            asm_ast.Mov(src=tmp, dst=A, is_volatile=False),
            asm_ast.Compare(left=A, right=other),
            asm_ast.Return(save_a=False),
        ]
        out = _run(instrs)
        self.assertIsInstance(out[1], asm_ast.Dec)
        self.assertEqual(out[1].dst, Y)


class TestSkips(unittest.TestCase):
    def test_skips_when_y_live(self):
        # A pre-pattern use of Y means Y's current value is live →
        # don't clobber Y.
        tmp = asm_ast.Data(name="__local_f__0", offset=0)
        other = asm_ast.Data(name="beam_y", offset=0)
        instrs = [
            # Y is loaded with something the post-pattern uses.
            asm_ast.Mov(src=asm_ast.Imm(value=5), dst=Y, is_volatile=False),
            asm_ast.Mov(src=_indexed_x("tbl"), dst=A, is_volatile=False),
            asm_ast.Mov(src=A, dst=tmp, is_volatile=False),
            asm_ast.Inc(dst=tmp),
            asm_ast.Mov(src=tmp, dst=A, is_volatile=False),
            asm_ast.Compare(left=A, right=other),
            # Use Y after the pattern.
            asm_ast.Mov(
                src=asm_ast.IndexedData(name="tbl2", index=asm_ast.Y()),
                dst=A, is_volatile=False,
            ),
            asm_ast.Return(save_a=False),
        ]
        out = _run(instrs)
        # The 5-atom sequence must remain unchanged.
        self.assertEqual(len(out), 8)
        # The middle atoms are still LDA/STA/INC/LDA/CMP.
        self.assertIsInstance(out[1], asm_ast.Mov)
        self.assertEqual(out[1].dst, A)
        self.assertIsInstance(out[2], asm_ast.Mov)
        self.assertEqual(out[2].dst, tmp)
        self.assertIsInstance(out[3], asm_ast.Inc)
        self.assertEqual(out[3].dst, tmp)

    def test_skips_when_a_live(self):
        # A is read after the CMP → can't drop the LDA-into-A.
        tmp = asm_ast.Data(name="__local_f__0", offset=0)
        other = asm_ast.Data(name="beam_y", offset=0)
        sink = asm_ast.Data(name="sink", offset=0)
        instrs = [
            asm_ast.Mov(src=_indexed_x("tbl"), dst=A, is_volatile=False),
            asm_ast.Mov(src=A, dst=tmp, is_volatile=False),
            asm_ast.Inc(dst=tmp),
            asm_ast.Mov(src=tmp, dst=A, is_volatile=False),
            asm_ast.Compare(left=A, right=other),
            asm_ast.Mov(src=A, dst=sink, is_volatile=False),  # reads A
            asm_ast.Return(save_a=False),
        ]
        out = _run(instrs)
        self.assertEqual(len(out), 7)

    def test_skips_when_tmp_used_after(self):
        # tmp is read again later in the function → can't drop
        # the STA/INC writes.
        tmp = asm_ast.Data(name="__local_f__0", offset=0)
        other = asm_ast.Data(name="beam_y", offset=0)
        sink = asm_ast.Data(name="sink", offset=0)
        instrs = [
            asm_ast.Mov(src=_indexed_x("tbl"), dst=A, is_volatile=False),
            asm_ast.Mov(src=A, dst=tmp, is_volatile=False),
            asm_ast.Inc(dst=tmp),
            asm_ast.Mov(src=tmp, dst=A, is_volatile=False),
            asm_ast.Compare(left=A, right=other),
            asm_ast.Mov(src=tmp, dst=sink, is_volatile=False),  # later use
            asm_ast.Return(save_a=False),
        ]
        out = _run(instrs)
        self.assertEqual(len(out), 7)

    def test_skips_when_index_is_y(self):
        # `LDA tbl,Y` source means we can't use Y for the result.
        tmp = asm_ast.Data(name="__local_f__0", offset=0)
        other = asm_ast.Data(name="beam_y", offset=0)
        instrs = [
            asm_ast.Mov(
                src=asm_ast.IndexedData(name="tbl", index=asm_ast.Y()),
                dst=A, is_volatile=False,
            ),
            asm_ast.Mov(src=A, dst=tmp, is_volatile=False),
            asm_ast.Inc(dst=tmp),
            asm_ast.Mov(src=tmp, dst=A, is_volatile=False),
            asm_ast.Compare(left=A, right=other),
            asm_ast.Return(save_a=False),
        ]
        out = _run(instrs)
        self.assertEqual(len(out), 6)

    def test_skips_when_cmp_against_indirect(self):
        # CPY supports only Imm/zp/abs — Indirect isn't valid for
        # CPY's right operand. (The asm_emit Compare also wouldn't
        # support it; the rewrite gate must reject.)
        tmp = asm_ast.Data(name="__local_f__0", offset=0)
        instrs = [
            asm_ast.Mov(src=_indexed_x("tbl"), dst=A, is_volatile=False),
            asm_ast.Mov(src=A, dst=tmp, is_volatile=False),
            asm_ast.Inc(dst=tmp),
            asm_ast.Mov(src=tmp, dst=A, is_volatile=False),
            asm_ast.Compare(left=A, right=asm_ast.Indirect(offset=0)),
            asm_ast.Return(save_a=False),
        ]
        out = _run(instrs)
        self.assertEqual(len(out), 6)


if __name__ == "__main__":
    unittest.main()
