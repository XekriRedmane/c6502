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


if __name__ == "__main__":
    unittest.main()
