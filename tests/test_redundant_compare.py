"""Tests for the redundant-compare peephole (`passes/redundant_compare.py`)."""

import unittest

import asm_ast
from passes.redundant_compare import apply_redundant_compare

_A = asm_ast.Reg(reg=asm_ast.A())


def _run(instrs):
    prog = asm_ast.Program(top_level=[asm_ast.Function(
        name="f", is_global=False, params=[], instructions=instrs,
    )])
    return apply_redundant_compare(prog).top_level[0].instructions


def _cmp(addr=0x80):
    return asm_ast.Compare(left=_A, right=asm_ast.ZP(address=addr, offset=0))


def _ret():
    return asm_ast.Return(save_a=False, save_x=False)


class TestRedundantCompare(unittest.TestCase):

    def test_second_compare_dropped(self):
        # CMP M; BCS L; RTS; L: CMP M; BNE ... → the second CMP goes.
        instrs = [
            _cmp(), asm_ast.Branch(cond=asm_ast.CS(), target="L"), _ret(),
            asm_ast.Label(name="L"), _cmp(),
            asm_ast.Branch(cond=asm_ast.NE(), target="M"), _ret(),
            asm_ast.Label(name="M"), _ret(),
        ]
        out = _run(instrs)
        self.assertEqual(out[:5], instrs[:4] + [instrs[5]])

    def test_compare_after_branches_dropped(self):
        # CMP M; BCC L1; BNE L2; CMP M: falling through the branches
        # changes nothing, so the second CMP goes.
        instrs = [
            _cmp(), asm_ast.Branch(cond=asm_ast.CC(), target="L1"),
            asm_ast.Branch(cond=asm_ast.NE(), target="L2"), _cmp(),
            asm_ast.Branch(cond=asm_ast.EQ(), target="L1"), _ret(),
            asm_ast.Label(name="L1"), _ret(),
            asm_ast.Label(name="L2"), _ret(),
        ]
        out = _run(instrs)
        self.assertEqual(out, instrs[:3] + instrs[4:])

    def test_compare_after_branch_of_another_compare_kept(self):
        instrs = [
            _cmp(0x80), asm_ast.Branch(cond=asm_ast.CC(), target="L1"),
            _cmp(0x81), _ret(), asm_ast.Label(name="L1"), _ret(),
        ]
        self.assertEqual(_run(instrs), instrs)

    def test_label_reached_by_second_of_two_branches(self):
        # CMP M; BCC L1; BCS L; ... L: CMP M: the second CMP goes,
        # since the run of branches after the first compare reaches L.
        instrs = [
            _cmp(), asm_ast.Branch(cond=asm_ast.CC(), target="L1"),
            asm_ast.Branch(cond=asm_ast.CS(), target="L"), _ret(),
            asm_ast.Label(name="L"), _cmp(),
            asm_ast.Branch(cond=asm_ast.NE(), target="L1"), _ret(),
            asm_ast.Label(name="L1"), _ret(),
        ]
        out = _run(instrs)
        self.assertEqual(out, instrs[:5] + instrs[6:])

    def test_fall_through_into_label_kept(self):
        # The label is also reached by falling through, where the
        # flags may differ.
        instrs = [
            _cmp(), asm_ast.Branch(cond=asm_ast.CS(), target="L"),
            asm_ast.Mov(src=asm_ast.Imm(value=1), dst=_A, is_volatile=False),
            asm_ast.Label(name="L"), _cmp(), _ret(),
        ]
        self.assertEqual(_run(instrs), instrs)

    def test_second_branch_to_label_kept(self):
        instrs = [
            _cmp(), asm_ast.Branch(cond=asm_ast.CS(), target="L"),
            _cmp(0x81), asm_ast.Branch(cond=asm_ast.EQ(), target="L"),
            _ret(),
            asm_ast.Label(name="L"), _cmp(), _ret(),
        ]
        self.assertEqual(_run(instrs), instrs)

    def test_different_operand_kept(self):
        instrs = [
            _cmp(), asm_ast.Branch(cond=asm_ast.CS(), target="L"), _ret(),
            asm_ast.Label(name="L"), _cmp(0x81), _ret(),
        ]
        self.assertEqual(_run(instrs), instrs)


def _lda(addr=0x80):
    return asm_ast.Mov(src=asm_ast.ZP(address=addr, offset=0), dst=_A,
                       is_volatile=False)


class TestCompareWithZero(unittest.TestCase):
    """`CMP #$00` after the instruction that set N and Z from the same
    register, when C is dead."""

    _ZERO = asm_ast.Compare(left=_A, right=asm_ast.Imm(value=0))

    def test_after_a_load(self):
        bne = asm_ast.Branch(cond=asm_ast.NE(), target="L")
        out = _run([_lda(), self._ZERO, bne, _ret(),
                    asm_ast.Label(name="L"), _ret()])
        self.assertEqual(out[:2], [_lda(), bne])

    def test_after_a_store_between(self):
        sta = asm_ast.Mov(src=_A, dst=asm_ast.ZP(address=0x81, offset=0),
                          is_volatile=False)
        bne = asm_ast.Branch(cond=asm_ast.NE(), target="L")
        out = _run([_lda(), sta, self._ZERO, bne, _ret(),
                    asm_ast.Label(name="L"), _ret()])
        self.assertEqual(out[:3], [_lda(), sta, bne])

    def test_kept_when_c_is_read(self):
        instrs = [_lda(), self._ZERO,
                  asm_ast.Branch(cond=asm_ast.CS(), target="L"), _ret(),
                  asm_ast.Label(name="L"), _ret()]
        self.assertEqual(_run(list(instrs)), instrs)

    def test_kept_after_a_store_with_a_y_setup(self):
        # `STA (FP),Y` loads Y first, which sets N and Z from Y.
        sta = asm_ast.Mov(src=_A, dst=asm_ast.Frame(offset=3),
                          is_volatile=False)
        instrs = [_lda(), sta, self._ZERO,
                  asm_ast.Branch(cond=asm_ast.NE(), target="L"), _ret(),
                  asm_ast.Label(name="L"), _ret()]
        self.assertEqual(_run(list(instrs)), instrs)

    def test_kept_after_a_label(self):
        instrs = [_lda(), asm_ast.Label(name="M"), self._ZERO,
                  asm_ast.Branch(cond=asm_ast.NE(), target="L"), _ret(),
                  asm_ast.Label(name="L"),
                  asm_ast.Jump(target="M", reg_args=[])]
        self.assertEqual(_run(list(instrs)), instrs)


class TestTestOfAJoin(unittest.TestCase):
    """An `ORA #$00` (or `CMP #$00`) after a label, where every way in
    set N and Z from A, as `equal = a == b; if (!equal) ...` leaves
    it: `LDA #$00; BEQ end; LDA #$01; end: STA equal; ORA #$00`."""

    _ORA0 = asm_ast.Or(src=asm_ast.Imm(value=0), dst=_A)

    def _join(self, first):
        sta = asm_ast.Mov(src=_A, dst=asm_ast.ZP(address=0x81, offset=0),
                          is_volatile=False)
        return [
            first,
            asm_ast.Branch(cond=asm_ast.EQ(), target="end"),
            asm_ast.Mov(src=asm_ast.Imm(value=1), dst=_A, is_volatile=False),
            asm_ast.Label(name="end"),
            sta, self._ORA0,
            asm_ast.Branch(cond=asm_ast.NE(), target="L"), _ret(),
            asm_ast.Label(name="L"), _ret(),
        ]

    def test_every_way_in_set_the_flags(self):
        instrs = self._join(asm_ast.Mov(src=asm_ast.Imm(value=0), dst=_A,
                                        is_volatile=False))
        out = _run(list(instrs))
        self.assertNotIn(self._ORA0, out)
        self.assertEqual(len(out), len(instrs) - 1)

    def test_kept_when_a_way_in_set_them_otherwise(self):
        # The branch to `end` follows a call, whose flags aren't A's.
        instrs = self._join(asm_ast.Call(name="g"))
        self.assertEqual(_run(list(instrs)), instrs)

    def test_kept_at_a_jump_table_target(self):
        instrs = [
            _lda(),
            asm_ast.JumpTable(table="t", index="Y", base=0, targets=["end"]),
            asm_ast.Label(name="end"), self._ORA0,
            asm_ast.Branch(cond=asm_ast.NE(), target="L"), _ret(),
            asm_ast.Label(name="L"), _ret(),
        ]
        self.assertEqual(_run(list(instrs)), instrs)


class TestCarryOfAdd(unittest.TestCase):
    """`CMP m` after `LDA m; CLC; ADC x` sets C opposite to the add's
    carry, so a branch on C takes the opposite condition without it.
    That's `if (new_lo < lo)` after `new_lo = lo + offset` in Zork's
    `branch_to_offset`."""

    M = asm_ast.ZP(address=0x8A, offset=0)

    def _instrs(self, store=asm_ast.ZP(address=0x90, offset=0), tail=None):
        return [
            asm_ast.Mov(src=self.M, dst=_A),
            asm_ast.ClearCarry(),
            asm_ast.Add(src=asm_ast.ZP(address=0x91, offset=0), dst=_A),
            asm_ast.Mov(src=_A, dst=store),
            asm_ast.Compare(left=_A, right=self.M),
            asm_ast.Branch(cond=asm_ast.CS(), target=".l"),
            asm_ast.Inc(dst=asm_ast.ZP(address=0x92, offset=0)),
            asm_ast.Label(name=".l"),
            *(tail or []),
            _ret(),
        ]

    def test_compare_goes_and_branch_flips(self):
        instrs = self._instrs()
        self.assertEqual(_run(list(instrs)), instrs[:4] + [
            asm_ast.Branch(cond=asm_ast.CC(), target=".l"),
        ] + instrs[6:])

    def test_kept_when_the_store_writes_m(self):
        instrs = self._instrs(store=self.M)
        self.assertEqual(_run(list(instrs)), instrs)

    def test_kept_when_z_is_read_after(self):
        instrs = self._instrs(tail=[
            asm_ast.Branch(cond=asm_ast.EQ(), target=".m"),
            asm_ast.Label(name=".m"),
        ])
        self.assertEqual(_run(list(instrs)), instrs)


if __name__ == "__main__":
    unittest.main()
