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

    def test_sbc_after_sec(self):
        # do_rwts_on_sector's `sector - SECTORS_PER_TRACK`.
        src = asm_ast.Data(name="SECTORS_PER_TRACK", offset=0)
        tmp = asm_ast.Data(name="T", offset=0)
        other = asm_ast.Data(name="sector", offset=0)
        prog = _wrap([
            _lda(src), _sta(tmp), _lda(other), asm_ast.SetCarry(),
            asm_ast.Sub(src=tmp, dst=_A), asm_ast.Return(save_a=True),
        ])
        out = _instrs(apply_cmp_through_tmp_fold(prog))
        self.assertEqual(out, [
            _lda(other), asm_ast.SetCarry(), asm_ast.Sub(src=src, dst=_A),
            asm_ast.Return(save_a=True),
        ])

    def test_logic_and_add(self):
        src = asm_ast.IndexedData(name="tab", offset=0, index=asm_ast.X())
        tmp = asm_ast.Data(name="T", offset=0)
        other = asm_ast.Data(name="O", offset=0)
        for op in (
            asm_ast.And(src=tmp, dst=_A),
            asm_ast.Or(src=tmp, dst=_A),
            asm_ast.Xor(src1=_A, src2=tmp, dst=_A),
        ):
            with self.subTest(op=type(op).__name__):
                prog = _wrap([_lda(src), _sta(tmp), _lda(other), op])
                out = _instrs(apply_cmp_through_tmp_fold(prog))
                self.assertEqual(out[0], _lda(other))
                self.assertEqual(len(out), 2)
                self.assertNotIn(tmp, vars(out[1]).values())
        prog = _wrap([
            _lda(src), _sta(tmp), _lda(other), asm_ast.ClearCarry(),
            asm_ast.Add(src=tmp, dst=_A),
        ])
        self.assertEqual(_instrs(apply_cmp_through_tmp_fold(prog)), [
            _lda(other), asm_ast.ClearCarry(), asm_ast.Add(src=src, dst=_A),
        ])

    def test_alu_needs_an_addressable_source(self):
        # SBC has no `(zp)` form without Y, which IndirectZp loads.
        src = asm_ast.IndirectZp(address=0x90, offset=0)
        tmp = asm_ast.Data(name="T", offset=0)
        other = asm_ast.Data(name="O", offset=0)
        prog = _wrap([
            _lda(src), _sta(tmp), _lda(other), asm_ast.SetCarry(),
            asm_ast.Sub(src=tmp, dst=_A),
        ])
        out = _instrs(apply_cmp_through_tmp_fold(prog))
        self.assertEqual(len(out), 5)

    def test_tmp_read_later_blocks_alu(self):
        src = asm_ast.Data(name="S", offset=0)
        tmp = asm_ast.Data(name="T", offset=0)
        other = asm_ast.Data(name="O", offset=0)
        prog = _wrap([
            _lda(src), _sta(tmp), _lda(other), asm_ast.SetCarry(),
            asm_ast.Sub(src=tmp, dst=_A), _lda(tmp),
            asm_ast.Return(save_a=True),
        ])
        out = _instrs(apply_cmp_through_tmp_fold(prog))
        self.assertEqual(len(out), 7)

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



class TestCmpThroughTmpFoldXor(unittest.TestCase):

    def test_xor_after_candidate_does_not_crash(self):
        # The tmp-read scan used to read `instr.src` on an Xor, which
        # has `src1` / `src2`, and crashed on about one random
        # program in twenty under --optimize. The Xor reads tmp, so
        # the fold must not fire.
        src = asm_ast.Data(name="S", offset=0)
        tmp = asm_ast.Data(name="T", offset=0)
        other = asm_ast.Data(name="O", offset=0)
        xor = asm_ast.Xor(src1=_A, src2=tmp, dst=_A)
        prog = _wrap([_lda(src), _sta(tmp), _lda(other), _cmp(tmp), xor])
        out = _instrs(apply_cmp_through_tmp_fold(prog))
        self.assertEqual(out[-1], xor)



class TestCmpThroughTmpFoldIndirect(unittest.TestCase):
    """The `LDA (p),Y; STA tmp; LDA key; CMP tmp` shape of comparing a
    key against a byte read through a pointer."""

    def test_indirect_zp_y_src_folds(self):
        src = asm_ast.IndirectZpY(address=0x90)
        tmp = asm_ast.ZP(address=0x84, offset=0)
        key = asm_ast.Data(name="key", offset=1)
        instrs = [
            _lda(src), _sta(tmp), _lda(key), _cmp(tmp),
            asm_ast.Return(save_a=False, save_x=False),
        ]
        out = _instrs(apply_cmp_through_tmp_fold(_wrap(instrs)))
        self.assertEqual(out[:2], [_lda(key), _cmp(src)])

    def test_tmp_reused_later_still_folds_when_dead(self):
        # The slot is written again (for an unrelated value) and read
        # after that, but the value this CMP read is dead after it.
        src = asm_ast.IndirectZpY(address=0x90)
        tmp = asm_ast.ZP(address=0x84, offset=0)
        key = asm_ast.Data(name="key", offset=1)
        instrs = [
            _lda(src), _sta(tmp), _lda(key), _cmp(tmp),
            _lda(asm_ast.Imm(value=3)), _sta(tmp), _lda(tmp),
            asm_ast.Return(save_a=True, save_x=False),
        ]
        out = _instrs(apply_cmp_through_tmp_fold(_wrap(instrs)))
        self.assertEqual(out[:2], [_lda(key), _cmp(src)])

    def test_tmp_read_after_kept(self):
        src = asm_ast.IndirectZpY(address=0x90)
        tmp = asm_ast.ZP(address=0x84, offset=0)
        key = asm_ast.Data(name="key", offset=1)
        instrs = [
            _lda(src), _sta(tmp), _lda(key), _cmp(tmp),
            _lda(tmp), asm_ast.Return(save_a=True, save_x=False),
        ]
        out = _instrs(apply_cmp_through_tmp_fold(_wrap(list(instrs))))
        self.assertEqual(out, instrs)

    def test_y_setup_other_blocks_fold(self):
        # `other` loaded through an operand with an LDY setup would
        # change the Y that `(p),Y` reads.
        src = asm_ast.IndirectZpY(address=0x90)
        tmp = asm_ast.ZP(address=0x84, offset=0)
        instrs = [
            _lda(src), _sta(tmp), _lda(asm_ast.Frame(offset=3)), _cmp(tmp),
            asm_ast.Return(save_a=False, save_x=False),
        ]
        out = _instrs(apply_cmp_through_tmp_fold(_wrap(list(instrs))))
        self.assertEqual(out, instrs)

    def test_bit_test_reads_tmp(self):
        # `BIT tmp` after the compare reads the staged byte, so the
        # store stays. `BitTest`'s operand field is `src`, and reading
        # it under another name raised AttributeError.
        src = asm_ast.Data(name="S", offset=0)
        tmp = asm_ast.Data(name="T", offset=0)
        other = asm_ast.Data(name="O", offset=0)
        instrs = [
            _lda(src), _sta(tmp), _lda(other), _cmp(tmp),
            asm_ast.BitTest(src=tmp),
            asm_ast.Return(save_a=False, save_x=False),
        ]
        out = _instrs(apply_cmp_through_tmp_fold(_wrap(list(instrs))))
        self.assertEqual(out, instrs)


if __name__ == "__main__":
    unittest.main()
