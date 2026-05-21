"""Tests for the `LDA src; STA tmp; LDA other; CMP tmp` folder."""

import unittest

import asm_ast
from passes.cmp_through_tmp_fold import apply_cmp_through_tmp_fold


_A = asm_ast.Reg(reg=asm_ast.A())


def _instrs(prog):
    return prog.top_level[0].instructions


def _wrap(instrs):
    return asm_ast.Program(top_level=[asm_ast.Function(
        name="f", is_global=False, params=[], instructions=instrs,
    )])


def _lda(src):
    return asm_ast.Mov(src=src, dst=_A, is_volatile=False)


def _sta(dst):
    return asm_ast.Mov(src=_A, dst=dst, is_volatile=False)


def _cmp(right):
    return asm_ast.Compare(left=_A, right=right)


class TestCmpThroughTmpFold(unittest.TestCase):

    def test_indexed_data_x_src(self):
        # The motivating beam_target_tick shape:
        #   LDA floor_y_table,X; STA tmp; LDA beam_y; CMP tmp
        src = asm_ast.IndexedData(
            name="floor_y_table", offset=0, index=asm_ast.X(),
        )
        tmp = asm_ast.Data(name="tmp", offset=0)
        other = asm_ast.Data(name="beam_y", offset=0)
        prog = _wrap([_lda(src), _sta(tmp), _lda(other), _cmp(tmp)])
        out = _instrs(apply_cmp_through_tmp_fold(prog))
        self.assertEqual(out, [_lda(other), _cmp(src)])

    def test_data_src(self):
        src = asm_ast.Data(name="S", offset=0)
        tmp = asm_ast.Data(name="T", offset=0)
        other = asm_ast.Data(name="O", offset=0)
        prog = _wrap([_lda(src), _sta(tmp), _lda(other), _cmp(tmp)])
        out = _instrs(apply_cmp_through_tmp_fold(prog))
        self.assertEqual(out, [_lda(other), _cmp(src)])

    def test_zp_src(self):
        src = asm_ast.ZP(address=0x90, offset=0)
        tmp = asm_ast.ZP(address=0x84, offset=0)
        other = asm_ast.Data(name="O", offset=0)
        prog = _wrap([_lda(src), _sta(tmp), _lda(other), _cmp(tmp)])
        out = _instrs(apply_cmp_through_tmp_fold(prog))
        self.assertEqual(out, [_lda(other), _cmp(src)])

    def test_imm_src(self):
        src = asm_ast.Imm(value=0x42)
        tmp = asm_ast.ZP(address=0x84, offset=0)
        other = asm_ast.Data(name="O", offset=0)
        prog = _wrap([_lda(src), _sta(tmp), _lda(other), _cmp(tmp)])
        out = _instrs(apply_cmp_through_tmp_fold(prog))
        self.assertEqual(out, [_lda(other), _cmp(src)])

    def test_indexed_y_src_with_data_other(self):
        # IndexedData(_, _, Y) src and Data other — safe because Data
        # LDA doesn't write Y.
        src = asm_ast.IndexedData(
            name="tbl", offset=0, index=asm_ast.Y(),
        )
        tmp = asm_ast.Data(name="T", offset=0)
        other = asm_ast.Data(name="O", offset=0)
        prog = _wrap([_lda(src), _sta(tmp), _lda(other), _cmp(tmp)])
        out = _instrs(apply_cmp_through_tmp_fold(prog))
        self.assertEqual(out, [_lda(other), _cmp(src)])

    def test_indexed_y_src_with_frame_other_rejected(self):
        # Frame other emits an LDY #off that clobbers Y — unsafe to
        # rewrite when src indexes through Y.
        src = asm_ast.IndexedData(
            name="tbl", offset=0, index=asm_ast.Y(),
        )
        tmp = asm_ast.Data(name="T", offset=0)
        other = asm_ast.Frame(offset=4)
        prog = _wrap([_lda(src), _sta(tmp), _lda(other), _cmp(tmp)])
        out = _instrs(apply_cmp_through_tmp_fold(prog))
        self.assertEqual(len(out), 4)

    def test_tmp_mismatch_rejected(self):
        # STA dst differs from CMP right — not the same tmp.
        src = asm_ast.Data(name="S", offset=0)
        tmp1 = asm_ast.Data(name="T1", offset=0)
        tmp2 = asm_ast.Data(name="T2", offset=0)
        other = asm_ast.Data(name="O", offset=0)
        prog = _wrap([_lda(src), _sta(tmp1), _lda(other), _cmp(tmp2)])
        out = _instrs(apply_cmp_through_tmp_fold(prog))
        self.assertEqual(len(out), 4)

    def test_other_aliases_tmp_rejected(self):
        # other == tmp would change semantics: original LDA other
        # reads the post-STA value (= src), rewritten reads the
        # pre-STA value.
        src = asm_ast.Data(name="S", offset=0)
        tmp = asm_ast.Data(name="T", offset=0)
        other = asm_ast.Data(name="T", offset=0)
        prog = _wrap([_lda(src), _sta(tmp), _lda(other), _cmp(tmp)])
        out = _instrs(apply_cmp_through_tmp_fold(prog))
        self.assertEqual(len(out), 4)

    def test_indirect_other_rejected(self):
        # Indirect other — runtime aliasing could touch tmp.
        src = asm_ast.Data(name="S", offset=0)
        tmp = asm_ast.Data(name="T", offset=0)
        other = asm_ast.Indirect(offset=0)
        prog = _wrap([_lda(src), _sta(tmp), _lda(other), _cmp(tmp)])
        out = _instrs(apply_cmp_through_tmp_fold(prog))
        self.assertEqual(len(out), 4)

    def test_volatile_rejected(self):
        # Any volatile mov in the pattern bails.
        src = asm_ast.Data(name="S", offset=0)
        tmp = asm_ast.Data(name="T", offset=0)
        other = asm_ast.Data(name="O", offset=0)
        prog = _wrap([
            asm_ast.Mov(src=src, dst=_A, is_volatile=True),
            _sta(tmp), _lda(other), _cmp(tmp),
        ])
        out = _instrs(apply_cmp_through_tmp_fold(prog))
        self.assertEqual(len(out), 4)

    def test_downstream_cmp_against_tmp_rejected(self):
        # `LDA src; STA tmp; LDA o1; CMP tmp; ...; LDA o2; CMP tmp`
        # The second CMP also reads tmp — folding the first occurrence
        # would drop the only STA tmp and leave the second CMP reading
        # a stale slot. Headline shape: the back-to-back column-band
        # test in beam_target_draw (`screen_col > 0x25` followed by
        # `screen_col <= 0x23`).
        src = asm_ast.IndexedData(
            name="proj_screen_col", offset=0, index=asm_ast.X(),
        )
        tmp = asm_ast.Data(name="T", offset=0)
        o1 = asm_ast.Imm(value=0x25)
        o2 = asm_ast.Imm(value=0x23)
        prog = _wrap([
            _lda(src), _sta(tmp),
            _lda(o1), _cmp(tmp),
            _lda(o2), _cmp(tmp),
        ])
        out = _instrs(apply_cmp_through_tmp_fold(prog))
        # Must NOT fold: the second CMP downstream still reads tmp.
        self.assertEqual(len(out), 6)
        self.assertEqual(out[1], _sta(tmp))  # STA tmp preserved

    def test_downstream_lda_of_tmp_rejected(self):
        # Any downstream read of tmp (not just CMP) blocks the fold.
        src = asm_ast.Data(name="S", offset=0)
        tmp = asm_ast.Data(name="T", offset=0)
        other = asm_ast.Data(name="O", offset=0)
        downstream_dst = asm_ast.Data(name="dst", offset=0)
        prog = _wrap([
            _lda(src), _sta(tmp), _lda(other), _cmp(tmp),
            _lda(tmp), _sta(downstream_dst),
        ])
        out = _instrs(apply_cmp_through_tmp_fold(prog))
        self.assertEqual(len(out), 6)
        self.assertEqual(out[1], _sta(tmp))


if __name__ == "__main__":
    unittest.main()
