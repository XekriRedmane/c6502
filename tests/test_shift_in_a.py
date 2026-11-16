"""Unit tests for `passes.shift_in_a`: `STA M; <shifts of M>; LDA M`
becomes the shifts on A and one `STA M`, and `STA M; INC M; LDA M`
becomes `CLC; ADC #1; STA M`."""
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

    def test_widened_byte_shift(self) -> None:
        # `(unsigned int)c << 3`: the zero high byte's store moves up
        # in front of the load of c, and the high byte's ROLs stay
        # between the shifts, now of A.
        c = asm_ast.ZP(address=0x90, offset=0)
        out = _run([
            _mov(c, _A),
            asm_ast.ArithmeticShiftLeft(dst=_A),
            _mov(_A, _M),
            _mov(asm_ast.Imm(value=0), _N),
            asm_ast.RotateLeft(dst=_N),
            asm_ast.ArithmeticShiftLeft(dst=_M),
            asm_ast.RotateLeft(dst=_N),
            _RTS,
        ])
        self.assertEqual(out, [
            _mov(asm_ast.Imm(value=0), _N),
            _mov(c, _A),
            asm_ast.ArithmeticShiftLeft(dst=_A),
            asm_ast.RotateLeft(dst=_N),
            asm_ast.ArithmeticShiftLeft(dst=_A),
            _mov(_A, _M),
            asm_ast.RotateLeft(dst=_N),
            _RTS,
        ])

    def test_constant_store_stays_when_a_chain_reads_it(self) -> None:
        # The code that computed A reads N, so the store to N can't
        # move above it.
        instrs = [
            _mov(_N, _A),
            asm_ast.ArithmeticShiftLeft(dst=_A),
            _mov(_A, _M),
            _mov(asm_ast.Imm(value=0), _N),
            asm_ast.RotateLeft(dst=_N),
            asm_ast.ArithmeticShiftLeft(dst=_M),
            _RTS,
        ]
        self.assertEqual(_run(list(instrs)), instrs)

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
            ("INC with C read after", [
                _mov(_A, _M), asm_ast.Inc(dst=_M), _mov(_M, _A),
                asm_ast.Add(src=_N, dst=_A), _RTS_A,
            ]),
            ("INC with no reload", [
                _mov(_A, _M), asm_ast.Inc(dst=_M), _RTS,
            ]),
        ):
            with self.subTest(name):
                self.assertEqual(_run(instrs), instrs)

    def test_increment_in_a(self) -> None:
        # `STA M; INC M; LDA M` with C and V dead after is `CLC; ADC
        # #1; STA M`, and the DEC form `SEC; SBC #1`.
        for step, carry, arith in (
            (asm_ast.Inc, asm_ast.ClearCarry, asm_ast.Add),
            (asm_ast.Dec, asm_ast.SetCarry, asm_ast.Sub),
        ):
            with self.subTest(step.__name__):
                out = _run([
                    _mov(_A, _M), step(dst=_M), _mov(_M, _A), _RTS_A,
                ])
                self.assertEqual(out, [
                    carry(), arith(src=asm_ast.Imm(value=1), dst=_A),
                    _mov(_A, _M), _RTS_A,
                ])


class TestShiftInPlace(unittest.TestCase):
    """`LDA M; ASL; STA M` as `ASL M` when A is dead after."""

    def test_one_shift(self):
        out = _run([_mov(_M, _A), asm_ast.ArithmeticShiftLeft(dst=_A),
                    _mov(_A, _M), _RTS])
        self.assertEqual(out, [asm_ast.ArithmeticShiftLeft(dst=_M), _RTS])

    def test_two_shifts(self):
        out = _run([_mov(_M, _A), asm_ast.LogicalShiftRight(dst=_A),
                    asm_ast.RotateRight(dst=_A), _mov(_A, _M), _RTS])
        self.assertEqual(out, [asm_ast.LogicalShiftRight(dst=_M),
                               asm_ast.RotateRight(dst=_M), _RTS])

    def test_three_shifts_left_alone(self):
        instrs = ([_mov(_M, _A)] + [asm_ast.ArithmeticShiftLeft(dst=_A)] * 3
                  + [_mov(_A, _M), _RTS])
        self.assertEqual(_run(list(instrs)), instrs)

    def test_a_read_after(self):
        instrs = [_mov(_M, _A), asm_ast.ArithmeticShiftLeft(dst=_A),
                  _mov(_A, _M), _RTS_A]
        self.assertEqual(_run(list(instrs)), instrs)

    def test_another_byte_left_alone(self):
        instrs = [_mov(_M, _A), asm_ast.ArithmeticShiftLeft(dst=_A),
                  _mov(_A, _N), _RTS]
        self.assertEqual(_run(list(instrs)), instrs)

    def test_volatile_left_alone(self):
        # An NMOS 6502 read-modify-write writes the old value back
        # first.
        instrs = [_mov(_M, _A, volatile=True),
                  asm_ast.ArithmeticShiftLeft(dst=_A),
                  _mov(_A, _M, volatile=True), _RTS]
        self.assertEqual(_run(list(instrs)), instrs)


if __name__ == "__main__":
    unittest.main()
