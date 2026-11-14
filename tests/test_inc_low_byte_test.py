"""Tests for `passes.inc_low_byte_test.apply_inc_low_byte_test`."""

import unittest

import asm_ast
from passes.inc_low_byte_test import apply_inc_low_byte_test


A = asm_ast.Reg(reg=asm_ast.A())
M0 = asm_ast.Data(name="P", offset=0)
M1 = asm_ast.Data(name="P", offset=1)
M2 = asm_ast.Data(name="P", offset=2)


def _run(instrs):
    fn = asm_ast.Function(
        name="f", is_global=True, params=[], instructions=instrs,
    )
    out = apply_inc_low_byte_test(asm_ast.Program(top_level=[fn]))
    return out.top_level[0].instructions


def _bne(target):
    return asm_ast.Branch(cond=asm_ast.NE(), target=target)


def _label(name):
    return asm_ast.Label(name=name)


_LDA_M0 = asm_ast.Mov(src=M0, dst=A)
_RET = asm_ast.Return(save_a=False)
# A is read at `.t`: the moved branch would skip the load.
_RET_A = asm_ast.Return(save_a=True)
_BODY = asm_ast.Mov(src=asm_ast.Imm(value=0), dst=asm_ast.Data(name="V", offset=0))


class TestBne(unittest.TestCase):

    def test_two_bytes(self):
        out = _run([
            asm_ast.Inc(dst=M0), _bne(".d"), asm_ast.Inc(dst=M1), _label(".d"),
            _LDA_M0, _bne(".t"), _BODY, _label(".t"), _RET,
        ])
        self.assertEqual(out, [
            asm_ast.Inc(dst=M0), _bne(".t"), asm_ast.Inc(dst=M1),
            _LDA_M0, _BODY, _label(".t"), _RET,
        ])

    def test_three_bytes_keeps_the_label_for_the_later_branch(self):
        out = _run([
            asm_ast.Inc(dst=M0), _bne(".d"), asm_ast.Inc(dst=M1), _bne(".d"),
            asm_ast.Inc(dst=M2), _label(".d"),
            _LDA_M0, _bne(".t"), _BODY, _label(".t"), _RET,
        ])
        self.assertEqual(out, [
            asm_ast.Inc(dst=M0), _bne(".t"), asm_ast.Inc(dst=M1), _bne(".d"),
            asm_ast.Inc(dst=M2), _label(".d"),
            _LDA_M0, _BODY, _label(".t"), _RET,
        ])


class TestBeq(unittest.TestCase):

    def test_two_bytes(self):
        out = _run([
            asm_ast.Inc(dst=M0), _bne(".d"), asm_ast.Inc(dst=M1), _label(".d"),
            _LDA_M0, asm_ast.Branch(cond=asm_ast.EQ(), target=".t"),
            _BODY, _label(".t"), _RET,
        ])
        self.assertEqual(out, [
            asm_ast.Inc(dst=M0), _bne(".inc_test@0"), asm_ast.Inc(dst=M1),
            _LDA_M0, asm_ast.Jump(target=".t"), _label(".inc_test@0"),
            _BODY, _label(".t"), _RET,
        ])


class TestSkips(unittest.TestCase):

    def _unchanged(self, instrs):
        self.assertEqual(_run(instrs), instrs)

    def test_a_read_where_the_branch_would_go(self):
        self._unchanged([
            asm_ast.Inc(dst=M0), _bne(".d"), asm_ast.Inc(dst=M1), _label(".d"),
            _LDA_M0, _bne(".t"), _BODY, _label(".t"), _RET_A,
        ])

    def test_label_reached_from_elsewhere(self):
        self._unchanged([
            _bne(".d"),
            asm_ast.Inc(dst=M0), _bne(".d"), asm_ast.Inc(dst=M1), _label(".d"),
            _LDA_M0, _bne(".t"), _BODY, _label(".t"), _RET,
        ])

    def test_test_of_another_byte(self):
        self._unchanged([
            asm_ast.Inc(dst=M0), _bne(".d"), asm_ast.Inc(dst=M1), _label(".d"),
            asm_ast.Mov(src=M1, dst=A), _bne(".t"), _BODY, _label(".t"), _RET,
        ])

    def test_branch_on_the_sign(self):
        self._unchanged([
            asm_ast.Inc(dst=M0), _bne(".d"), asm_ast.Inc(dst=M1), _label(".d"),
            _LDA_M0, asm_ast.Branch(cond=asm_ast.MI(), target=".t"),
            _BODY, _label(".t"), _RET,
        ])

    def test_volatile_load(self):
        self._unchanged([
            asm_ast.Inc(dst=M0), _bne(".d"), asm_ast.Inc(dst=M1), _label(".d"),
            asm_ast.Mov(src=M0, dst=A, is_volatile=True), _bne(".t"),
            _BODY, _label(".t"), _RET,
        ])


if __name__ == "__main__":
    unittest.main()
