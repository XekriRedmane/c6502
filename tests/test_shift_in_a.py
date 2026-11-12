"""Unit tests for `passes.shift_in_a`: `STA M; <shifts of M>; LDA M`
becomes the shifts on A and one `STA M`."""
from __future__ import annotations

import unittest

import asm_ast
from passes.shift_in_a import apply_shift_in_a

_A = asm_ast.Reg(reg=asm_ast.A())
_X = asm_ast.Reg(reg=asm_ast.X())
_M = asm_ast.ZP(address=0x80, offset=0)
_N = asm_ast.ZP(address=0x81, offset=0)


def _run(instrs):
    prog = asm_ast.Program(top_level=[asm_ast.Function(
        name="f", is_global=True, params=[], instructions=instrs,
    )])
    return apply_shift_in_a(prog).top_level[0].instructions


def _mov(src, dst, volatile=False):
    return asm_ast.Mov(src=src, dst=dst, is_volatile=volatile)


_RTS = asm_ast.Return(save_a=False, save_x=False, save_c=False)
_RTS_A = asm_ast.Return(save_a=True, save_x=False, save_c=False)


class TestShiftInA(unittest.TestCase):

    def test_store_shift_reload(self) -> None:
        out = _run([
            _mov(_A, _M),
            asm_ast.ArithmeticShiftLeft(dst=_M),
            _mov(_M, _A),
            _RTS_A,
        ])
        self.assertEqual(out, [
            asm_ast.ArithmeticShiftLeft(dst=_A), _mov(_A, _M), _RTS_A,
        ])

    def test_several_shifts_and_rotates(self) -> None:
        out = _run([
            _mov(_A, _M),
            asm_ast.LogicalShiftRight(dst=_M),
            asm_ast.RotateRight(dst=_M),
            asm_ast.RotateLeft(dst=_M),
            _mov(_M, _A),
            _RTS_A,
        ])
        self.assertEqual(out, [
            asm_ast.LogicalShiftRight(dst=_A),
            asm_ast.RotateRight(dst=_A),
            asm_ast.RotateLeft(dst=_A),
            _mov(_A, _M),
            _RTS_A,
        ])

    def test_no_reload_with_a_dead(self) -> None:
        out = _run([
            _mov(_A, _M),
            asm_ast.ArithmeticShiftLeft(dst=_M),
            _mov(asm_ast.Imm(value=0), _A),
            _mov(_M, _N),
            _RTS,
        ])
        self.assertEqual(out[:2], [
            asm_ast.ArithmeticShiftLeft(dst=_A), _mov(_A, _M),
        ])

    def test_no_reload_with_a_live(self) -> None:
        # TAX reads the unshifted value.
        instrs = [
            _mov(_A, _M),
            asm_ast.ArithmeticShiftLeft(dst=_M),
            _mov(_A, _X),
            _RTS,
        ]
        self.assertEqual(_run(instrs), instrs)

    def test_volatile_reload_is_kept(self) -> None:
        # The volatile LDA has to read M again, after the new store.
        out = _run([
            _mov(_A, _M),
            asm_ast.ArithmeticShiftLeft(dst=_M),
            _mov(_M, _A, volatile=True),
            _RTS_A,
        ])
        self.assertEqual(out, [
            asm_ast.ArithmeticShiftLeft(dst=_A), _mov(_A, _M),
            _mov(_M, _A, volatile=True), _RTS_A,
        ])

    def test_left_alone(self) -> None:
        for name, instrs in (
            ("volatile store", [
                _mov(_A, _M, volatile=True),
                asm_ast.ArithmeticShiftLeft(dst=_M),
                _mov(_M, _A), _RTS_A,
            ]),
            ("another byte", [
                _mov(_A, _M),
                asm_ast.ArithmeticShiftLeft(dst=_N),
                _mov(_N, _A), _RTS_A,
            ]),
            ("INC", [
                _mov(_A, _M), asm_ast.Inc(dst=_M), _mov(_M, _A), _RTS_A,
            ]),
        ):
            with self.subTest(name):
                self.assertEqual(_run(instrs), instrs)


if __name__ == "__main__":
    unittest.main()
