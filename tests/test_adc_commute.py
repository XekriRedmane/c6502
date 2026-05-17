"""Tests for the ADC / AND / ORA commutativity peephole."""

import unittest

import asm_ast
from passes.adc_commute import apply_adc_commute


_A = asm_ast.Reg(reg=asm_ast.A())
_X = asm_ast.Reg(reg=asm_ast.X())
_Y = asm_ast.Reg(reg=asm_ast.Y())


def _wrap(instrs):
    return asm_ast.Program(top_level=[asm_ast.Function(
        name="f", is_global=False, params=[], instructions=instrs,
    )])


def _instrs(prog):
    return prog.top_level[0].instructions


def _zp(addr, off=0):
    return asm_ast.ZP(address=addr, offset=off)


def _data(name, off=0):
    return asm_ast.Data(name=name, offset=off)


def _ix(name, off=0, index=asm_ast.X()):
    return asm_ast.IndexedData(name=name, offset=off, index=index)


def _lda(src):
    return asm_ast.Mov(src=src, dst=_A, is_volatile=False)


def _sta(dst):
    return asm_ast.Mov(src=_A, dst=dst, is_volatile=False)


def _ldx(src):
    return asm_ast.Mov(src=src, dst=_X, is_volatile=False)


def _ldy(src):
    return asm_ast.Mov(src=src, dst=_Y, is_volatile=False)


def _adc(src):
    return asm_ast.Add(src=src, dst=_A)


def _and(src):
    return asm_ast.And(src=src, dst=_A)


def _or(src):
    return asm_ast.Or(src=src, dst=_A)


def _clc():
    return asm_ast.ClearCarry()


def _sec():
    return asm_ast.SetCarry()


class TestAdcCommuteBasic(unittest.TestCase):
    """The five-instruction canonical pattern, strictly adjacent."""

    def test_adc_zp_strict_adjacency(self):
        # STA temp; LDA mem; CLC; ADC temp; STA mem
        prog = _wrap([
            _sta(_zp(0x80)),         # STA temp (was: V landed in A)
            _lda(_data("M")),        # LDA M
            _clc(),
            _adc(_zp(0x80)),         # ADC temp
            _sta(_data("M")),        # STA M
        ])
        out = _instrs(apply_adc_commute(prog))
        # Drop the LDA, rewrite the ADC's src from temp → M.
        self.assertEqual(out, [
            _sta(_zp(0x80)),
            _clc(),
            _adc(_data("M")),
            _sta(_data("M")),
        ])

    def test_and_pattern(self):
        prog = _wrap([
            _sta(_zp(0x80)),
            _lda(_data("M")),
            _and(_zp(0x80)),         # no CLC for AND
            _sta(_data("M")),
        ])
        out = _instrs(apply_adc_commute(prog))
        self.assertEqual(out, [
            _sta(_zp(0x80)),
            _and(_data("M")),
            _sta(_data("M")),
        ])

    def test_or_pattern(self):
        prog = _wrap([
            _sta(_zp(0x80)),
            _lda(_data("M")),
            _or(_zp(0x80)),
            _sta(_data("M")),
        ])
        out = _instrs(apply_adc_commute(prog))
        self.assertEqual(out, [
            _sta(_zp(0x80)),
            _or(_data("M")),
            _sta(_data("M")),
        ])

    def test_sec_in_place_of_clc(self):
        # The carry source can be either CLC or SEC (the peephole is
        # operand-flag-agnostic — we don't touch C).
        prog = _wrap([
            _sta(_zp(0x80)),
            _lda(_data("M")),
            _sec(),
            _adc(_zp(0x80)),
            _sta(_data("M")),
        ])
        out = _instrs(apply_adc_commute(prog))
        self.assertEqual(out, [
            _sta(_zp(0x80)),
            _sec(),
            _adc(_data("M")),
            _sta(_data("M")),
        ])

    def test_no_carry_setup_still_fires(self):
        # CLC/SEC is optional. The peephole still fires when ADC
        # follows LDA directly (the carry comes from upstream).
        prog = _wrap([
            _sta(_zp(0x80)),
            _lda(_data("M")),
            _adc(_zp(0x80)),
            _sta(_data("M")),
        ])
        out = _instrs(apply_adc_commute(prog))
        self.assertEqual(out, [
            _sta(_zp(0x80)),
            _adc(_data("M")),
            _sta(_data("M")),
        ])


class TestAdcCommuteIndexedData(unittest.TestCase):
    """The mem side can be IndexedData (abs,X / abs,Y) — ADC
    supports those modes."""

    def test_indexed_x_mem(self):
        # STA temp; LDX foo; LDA arr,X; CLC; ADC temp; STA arr,X
        # The LDX in the intervening is the index-reg setup that
        # motivates the relaxed intervening band.
        prog = _wrap([
            _sta(_zp(0x82)),
            _ldx(_data("slot")),
            _lda(_ix("arr", 0, asm_ast.X())),
            _clc(),
            _adc(_zp(0x82)),
            _sta(_ix("arr", 0, asm_ast.X())),
        ])
        out = _instrs(apply_adc_commute(prog))
        self.assertEqual(out, [
            _sta(_zp(0x82)),
            _ldx(_data("slot")),
            _clc(),
            _adc(_ix("arr", 0, asm_ast.X())),
            _sta(_ix("arr", 0, asm_ast.X())),
        ])

    def test_indexed_y_mem(self):
        prog = _wrap([
            _sta(_zp(0x82)),
            _ldy(_data("slot")),
            _lda(_ix("arr", 0, asm_ast.Y())),
            _clc(),
            _adc(_zp(0x82)),
            _sta(_ix("arr", 0, asm_ast.Y())),
        ])
        out = _instrs(apply_adc_commute(prog))
        self.assertEqual(out, [
            _sta(_zp(0x82)),
            _ldy(_data("slot")),
            _clc(),
            _adc(_ix("arr", 0, asm_ast.Y())),
            _sta(_ix("arr", 0, asm_ast.Y())),
        ])


class TestAdcCommuteRejections(unittest.TestCase):
    """Patterns the peephole must NOT rewrite."""

    def test_a_clobber_in_intervening_aborts(self):
        # Mov(_, Reg(A)) in the intervening clobbers A, so we can't
        # assume A still holds V at the ADC point.
        prog = _wrap([
            _sta(_zp(0x80)),
            _lda(_data("X")),        # A clobbered!
            # Now the next LDA would also be a fresh A write —
            # this looks like the pattern but isn't valid.
            _lda(_data("M")),
            _adc(_zp(0x80)),
            _sta(_data("M")),
        ])
        out = _instrs(apply_adc_commute(prog))
        # First LDA "X" looks like the LDA at [j], not part of
        # intervening. Match attempts: STA at 0; LDA at 1; then we'd
        # need [optional CLC/SEC] then ADC then STA. But [2] is _lda
        # again, not CLC/SEC or ADC. So the match must abort.
        self.assertEqual(out, prog.top_level[0].instructions)

    def test_intervening_writes_temp_aborts(self):
        prog = _wrap([
            _sta(_zp(0x80)),
            _sta(_zp(0x80)),         # second STA temp — write to temp
            _lda(_data("M")),
            _adc(_zp(0x80)),
            _sta(_data("M")),
        ])
        # The second STA temp aliases temp; match aborts on it.
        # Then we'd try matching starting at index 1, but the
        # second pattern is the same shape — and rewriting THAT
        # would be sound (the intervening is empty). So we expect
        # ONE rewrite, anchored at the inner STA.
        out = _instrs(apply_adc_commute(prog))
        self.assertEqual(out, [
            _sta(_zp(0x80)),
            _sta(_zp(0x80)),
            _adc(_data("M")),
            _sta(_data("M")),
        ])

    def test_volatile_lda_blocks(self):
        prog = _wrap([
            _sta(_zp(0x80)),
            asm_ast.Mov(
                src=_data("M"), dst=_A, is_volatile=True,
            ),
            _adc(_zp(0x80)),
            _sta(_data("M")),
        ])
        out = _instrs(apply_adc_commute(prog))
        self.assertEqual(out, prog.top_level[0].instructions)

    def test_volatile_sta_blocks(self):
        prog = _wrap([
            asm_ast.Mov(
                src=_A, dst=_zp(0x80), is_volatile=True,
            ),
            _lda(_data("M")),
            _adc(_zp(0x80)),
            _sta(_data("M")),
        ])
        out = _instrs(apply_adc_commute(prog))
        self.assertEqual(out, prog.top_level[0].instructions)

    def test_sbc_not_commutative_rejected(self):
        # Sub (SBC) is not commutative; the peephole must skip.
        prog = _wrap([
            _sta(_zp(0x80)),
            _lda(_data("M")),
            _sec(),
            asm_ast.Sub(src=_zp(0x80), dst=_A),
            _sta(_data("M")),
        ])
        out = _instrs(apply_adc_commute(prog))
        self.assertEqual(out, prog.top_level[0].instructions)

    def test_lda_and_sta_different_mem_rejected(self):
        # LDA reads one mem; STA writes a different mem. The
        # rewrite would change the read address — unsound.
        prog = _wrap([
            _sta(_zp(0x80)),
            _lda(_data("M")),
            _adc(_zp(0x80)),
            _sta(_data("N")),
        ])
        out = _instrs(apply_adc_commute(prog))
        self.assertEqual(out, prog.top_level[0].instructions)

    def test_block_boundary_in_intervening_aborts(self):
        # A Label in the intervening means we'd need cross-block
        # analysis to know whether A is preserved. Conservative
        # abort.
        prog = _wrap([
            _sta(_zp(0x80)),
            asm_ast.Label(name=".L1"),
            _lda(_data("M")),
            _adc(_zp(0x80)),
            _sta(_data("M")),
        ])
        out = _instrs(apply_adc_commute(prog))
        self.assertEqual(out, prog.top_level[0].instructions)

    def test_call_in_intervening_aborts(self):
        prog = _wrap([
            _sta(_zp(0x80)),
            asm_ast.Call(name="helper"),
            _lda(_data("M")),
            _adc(_zp(0x80)),
            _sta(_data("M")),
        ])
        out = _instrs(apply_adc_commute(prog))
        self.assertEqual(out, prog.top_level[0].instructions)


class TestAdcCommuteMultipleMatches(unittest.TestCase):
    """Independent matches in the same function should all fire."""

    def test_two_independent_patterns(self):
        prog = _wrap([
            # First pattern.
            _sta(_zp(0x80)),
            _lda(_data("M")),
            _clc(),
            _adc(_zp(0x80)),
            _sta(_data("M")),
            # Block boundary separating them.
            asm_ast.Label(name=".mid"),
            # Second pattern.
            _sta(_zp(0x82)),
            _lda(_data("N")),
            _clc(),
            _adc(_zp(0x82)),
            _sta(_data("N")),
        ])
        out = _instrs(apply_adc_commute(prog))
        self.assertEqual(out, [
            _sta(_zp(0x80)),
            _clc(),
            _adc(_data("M")),
            _sta(_data("M")),
            asm_ast.Label(name=".mid"),
            _sta(_zp(0x82)),
            _clc(),
            _adc(_data("N")),
            _sta(_data("N")),
        ])


if __name__ == "__main__":
    unittest.main()
