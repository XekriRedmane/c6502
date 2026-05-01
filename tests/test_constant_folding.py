"""Behavioral tests for `passes.optimization.constant_folding`.

Coverage:
  - Unary: Negate / Complement / LogicalNot, including 8-bit
    overflow wrap (-INT_MIN) and the LogicalNot result-width rule.
  - Binary arithmetic / bitwise: Add / Sub / Mul / Div / Mod /
    And / Or / Xor on each of ConstInt / ConstLong / ConstLongLong,
    including width-truncating overflow.
  - Binary shifts: LeftShift / RightShift (arithmetic), with bounds
    checks (negative count / count ≥ width are skipped).
  - Comparisons: each of the six, signed interpretation, ConstInt
    result.
  - JumpIfTrue / JumpIfFalse: replaced with Jump or dropped based
    on truth value.
  - Skip cases: floating constants, mismatched-width binary,
    non-Constant operands, divisor=0.

Each test builds a small `tac_ast.Function` with a single foldable
instruction and asserts the rewritten function matches the expected
shape.
"""

from __future__ import annotations

import unittest

import fp_arith
import tac_ast
from passes.optimization.constant_folding import constant_fold


def _ci(v: int) -> tac_ast.Constant:
    return tac_ast.Constant(const=tac_ast.ConstInt(int=v))


def _cl(v: int) -> tac_ast.Constant:
    return tac_ast.Constant(const=tac_ast.ConstLong(int=v))


def _cll(v: int) -> tac_ast.Constant:
    return tac_ast.Constant(const=tac_ast.ConstLongLong(int=v))


def _cf(s: str) -> tac_ast.Constant:
    return tac_ast.Constant(const=tac_ast.ConstFloat(
        bits=fp_arith.single_string_to_bits(s),
    ))


def _var(name: str) -> tac_ast.Var:
    return tac_ast.Var(name=name)


def _ret() -> tac_ast.Ret:
    return tac_ast.Ret(val=tac_ast.Constant(const=tac_ast.ConstInt(int=0)))


def _fn(*instrs) -> tac_ast.Function:
    return tac_ast.Function(
        name="main",
        is_global=True,
        params=[],
        instructions=list(instrs),
    )


def _fold_one(instr) -> list:
    """Run constant_fold on a single-instruction function and
    return the resulting instruction list (useful for assertions
    that include the trailing Ret too)."""
    return constant_fold(_fn(instr, _ret())).instructions


class TestFoldUnary(unittest.TestCase):

    def test_negate_basic(self) -> None:
        out = _fold_one(tac_ast.Unary(
            op=tac_ast.Negate(), src=_ci(5), dst=_var("x"),
        ))
        self.assertEqual(out[0],
                         tac_ast.Copy(src=_ci(-5), dst=_var("x")))

    def test_negate_int_min_wraps(self) -> None:
        # -(-128) overflows signed 8-bit: 128 mod 256 → 128, signed
        # = -128 (the canonical two's-complement wrap).
        out = _fold_one(tac_ast.Unary(
            op=tac_ast.Negate(), src=_ci(-128), dst=_var("x"),
        ))
        self.assertEqual(out[0],
                         tac_ast.Copy(src=_ci(-128), dst=_var("x")))

    def test_complement_int(self) -> None:
        out = _fold_one(tac_ast.Unary(
            op=tac_ast.Complement(), src=_ci(0), dst=_var("x"),
        ))
        # ~0 = -1, fits in signed 8-bit as -1.
        self.assertEqual(out[0],
                         tac_ast.Copy(src=_ci(-1), dst=_var("x")))

    def test_complement_long(self) -> None:
        out = _fold_one(tac_ast.Unary(
            op=tac_ast.Complement(), src=_cl(0), dst=_var("x"),
        ))
        self.assertEqual(out[0],
                         tac_ast.Copy(src=_cl(-1), dst=_var("x")))

    def test_logical_not_zero_yields_one(self) -> None:
        out = _fold_one(tac_ast.Unary(
            op=tac_ast.LogicalNot(), src=_ci(0), dst=_var("x"),
        ))
        self.assertEqual(out[0],
                         tac_ast.Copy(src=_ci(1), dst=_var("x")))

    def test_logical_not_nonzero_yields_zero(self) -> None:
        out = _fold_one(tac_ast.Unary(
            op=tac_ast.LogicalNot(), src=_ci(42), dst=_var("x"),
        ))
        self.assertEqual(out[0],
                         tac_ast.Copy(src=_ci(0), dst=_var("x")))

    def test_logical_not_long_returns_int(self) -> None:
        # `!` on a Long should still yield a 1-byte ConstInt result.
        out = _fold_one(tac_ast.Unary(
            op=tac_ast.LogicalNot(), src=_cl(1234), dst=_var("x"),
        ))
        self.assertEqual(out[0],
                         tac_ast.Copy(src=_ci(0), dst=_var("x")))

    def test_unary_with_var_operand_unchanged(self) -> None:
        instr = tac_ast.Unary(
            op=tac_ast.Negate(), src=_var("y"), dst=_var("x"),
        )
        out = _fold_one(instr)
        self.assertEqual(out[0], instr)

    def test_unary_on_float_unchanged(self) -> None:
        instr = tac_ast.Unary(
            op=tac_ast.Negate(), src=_cf("1.5"), dst=_var("x"),
        )
        out = _fold_one(instr)
        self.assertEqual(out[0], instr)


class TestFoldBinaryArith(unittest.TestCase):

    def test_add_basic(self) -> None:
        out = _fold_one(tac_ast.Binary(
            op=tac_ast.Add(), src1=_ci(2), src2=_ci(3), dst=_var("x"),
        ))
        self.assertEqual(out[0],
                         tac_ast.Copy(src=_ci(5), dst=_var("x")))

    def test_add_overflow_wraps(self) -> None:
        # 127 + 1 = 128, signed 8-bit wrap → -128.
        out = _fold_one(tac_ast.Binary(
            op=tac_ast.Add(), src1=_ci(127), src2=_ci(1), dst=_var("x"),
        ))
        self.assertEqual(out[0],
                         tac_ast.Copy(src=_ci(-128), dst=_var("x")))

    def test_sub_long(self) -> None:
        out = _fold_one(tac_ast.Binary(
            op=tac_ast.Subtract(),
            src1=_cl(1000), src2=_cl(7), dst=_var("x"),
        ))
        self.assertEqual(out[0],
                         tac_ast.Copy(src=_cl(993), dst=_var("x")))

    def test_mul_overflow_wraps(self) -> None:
        # 16 * 16 = 256 mod 256 = 0.
        out = _fold_one(tac_ast.Binary(
            op=tac_ast.Multiply(),
            src1=_ci(16), src2=_ci(16), dst=_var("x"),
        ))
        self.assertEqual(out[0],
                         tac_ast.Copy(src=_ci(0), dst=_var("x")))

    def test_divide_truncates_toward_zero(self) -> None:
        # -7 / 2 = -3 (toward zero), not -4 (toward -inf).
        out = _fold_one(tac_ast.Binary(
            op=tac_ast.Divide(),
            src1=_ci(-7), src2=_ci(2), dst=_var("x"),
        ))
        self.assertEqual(out[0],
                         tac_ast.Copy(src=_ci(-3), dst=_var("x")))

    def test_modulo_dividend_signed(self) -> None:
        # -7 % 2: with truncation toward zero, q=-3, r = -7 - (-3*2)
        # = -1. Sign of result matches dividend.
        out = _fold_one(tac_ast.Binary(
            op=tac_ast.Modulo(),
            src1=_ci(-7), src2=_ci(2), dst=_var("x"),
        ))
        self.assertEqual(out[0],
                         tac_ast.Copy(src=_ci(-1), dst=_var("x")))

    def test_divide_by_zero_unchanged(self) -> None:
        instr = tac_ast.Binary(
            op=tac_ast.Divide(),
            src1=_ci(7), src2=_ci(0), dst=_var("x"),
        )
        out = _fold_one(instr)
        self.assertEqual(out[0], instr)

    def test_modulo_by_zero_unchanged(self) -> None:
        instr = tac_ast.Binary(
            op=tac_ast.Modulo(),
            src1=_ci(7), src2=_ci(0), dst=_var("x"),
        )
        out = _fold_one(instr)
        self.assertEqual(out[0], instr)

    def test_bitwise_and_or_xor(self) -> None:
        out_and = _fold_one(tac_ast.Binary(
            op=tac_ast.BitwiseAnd(),
            src1=_ci(0b1100), src2=_ci(0b1010), dst=_var("x"),
        ))
        out_or = _fold_one(tac_ast.Binary(
            op=tac_ast.BitwiseOr(),
            src1=_ci(0b1100), src2=_ci(0b1010), dst=_var("x"),
        ))
        out_xor = _fold_one(tac_ast.Binary(
            op=tac_ast.BitwiseXor(),
            src1=_ci(0b1100), src2=_ci(0b1010), dst=_var("x"),
        ))
        self.assertEqual(out_and[0],
                         tac_ast.Copy(src=_ci(0b1000), dst=_var("x")))
        self.assertEqual(out_or[0],
                         tac_ast.Copy(src=_ci(0b1110), dst=_var("x")))
        self.assertEqual(out_xor[0],
                         tac_ast.Copy(src=_ci(0b0110), dst=_var("x")))

    def test_long_long_arithmetic(self) -> None:
        out = _fold_one(tac_ast.Binary(
            op=tac_ast.Add(),
            src1=_cll(100_000), src2=_cll(200_000), dst=_var("x"),
        ))
        self.assertEqual(out[0],
                         tac_ast.Copy(src=_cll(300_000), dst=_var("x")))

    def test_binary_with_one_var_unchanged(self) -> None:
        instr = tac_ast.Binary(
            op=tac_ast.Add(), src1=_ci(1), src2=_var("y"),
            dst=_var("x"),
        )
        out = _fold_one(instr)
        self.assertEqual(out[0], instr)

    def test_binary_mismatched_widths_unchanged(self) -> None:
        # Shouldn't happen post-type-checking, but the folder bails
        # rather than guess at the result variant.
        instr = tac_ast.Binary(
            op=tac_ast.Add(), src1=_ci(1), src2=_cl(1),
            dst=_var("x"),
        )
        out = _fold_one(instr)
        self.assertEqual(out[0], instr)

    def test_binary_on_float_unchanged(self) -> None:
        instr = tac_ast.Binary(
            op=tac_ast.Add(), src1=_cf("1.0"), src2=_cf("2.0"),
            dst=_var("x"),
        )
        out = _fold_one(instr)
        self.assertEqual(out[0], instr)


class TestFoldShift(unittest.TestCase):

    def test_left_shift_basic(self) -> None:
        out = _fold_one(tac_ast.Binary(
            op=tac_ast.LeftShift(),
            src1=_ci(1), src2=_ci(3), dst=_var("x"),
        ))
        self.assertEqual(out[0],
                         tac_ast.Copy(src=_ci(8), dst=_var("x")))

    def test_left_shift_overflows_into_sign(self) -> None:
        # 1 << 7 = 128; signed 8-bit canonicalizes to -128.
        out = _fold_one(tac_ast.Binary(
            op=tac_ast.LeftShift(),
            src1=_ci(1), src2=_ci(7), dst=_var("x"),
        ))
        self.assertEqual(out[0],
                         tac_ast.Copy(src=_ci(-128), dst=_var("x")))

    def test_arithmetic_right_shift_negative_value(self) -> None:
        # -8 >> 1 = -4 (sign-preserving).
        out = _fold_one(tac_ast.Binary(
            op=tac_ast.RightShift(),
            src1=_ci(-8), src2=_ci(1), dst=_var("x"),
        ))
        self.assertEqual(out[0],
                         tac_ast.Copy(src=_ci(-4), dst=_var("x")))

    def test_shift_count_at_width_unchanged(self) -> None:
        instr = tac_ast.Binary(
            op=tac_ast.LeftShift(),
            src1=_ci(1), src2=_ci(8), dst=_var("x"),
        )
        out = _fold_one(instr)
        self.assertEqual(out[0], instr)

    def test_shift_count_negative_unchanged(self) -> None:
        instr = tac_ast.Binary(
            op=tac_ast.LeftShift(),
            src1=_ci(1), src2=_ci(-1), dst=_var("x"),
        )
        out = _fold_one(instr)
        self.assertEqual(out[0], instr)

    def test_shift_long_with_int_count(self) -> None:
        # A common shape post-type-check: the value is widened to
        # Long, the count stays at Int.
        out = _fold_one(tac_ast.Binary(
            op=tac_ast.LeftShift(),
            src1=_cl(0x0001), src2=_ci(8), dst=_var("x"),
        ))
        self.assertEqual(out[0],
                         tac_ast.Copy(src=_cl(0x0100), dst=_var("x")))


class TestFoldComparison(unittest.TestCase):

    def test_equal_true(self) -> None:
        out = _fold_one(tac_ast.Binary(
            op=tac_ast.Equal(), src1=_cl(7), src2=_cl(7),
            dst=_var("x"),
        ))
        self.assertEqual(out[0],
                         tac_ast.Copy(src=_ci(1), dst=_var("x")))

    def test_equal_false(self) -> None:
        out = _fold_one(tac_ast.Binary(
            op=tac_ast.Equal(), src1=_cl(7), src2=_cl(8),
            dst=_var("x"),
        ))
        self.assertEqual(out[0],
                         tac_ast.Copy(src=_ci(0), dst=_var("x")))

    def test_not_equal(self) -> None:
        out = _fold_one(tac_ast.Binary(
            op=tac_ast.NotEqual(), src1=_ci(1), src2=_ci(2),
            dst=_var("x"),
        ))
        self.assertEqual(out[0],
                         tac_ast.Copy(src=_ci(1), dst=_var("x")))

    def test_less_than_signed(self) -> None:
        # -1 < 1 in signed; in unsigned 8-bit, 0xFF < 0x01 would be
        # false. We honor signed because c6502's V-corrected SBC
        # sequence is used for every integer ordering today.
        out = _fold_one(tac_ast.Binary(
            op=tac_ast.LessThan(), src1=_ci(-1), src2=_ci(1),
            dst=_var("x"),
        ))
        self.assertEqual(out[0],
                         tac_ast.Copy(src=_ci(1), dst=_var("x")))

    def test_greater_than(self) -> None:
        out = _fold_one(tac_ast.Binary(
            op=tac_ast.GreaterThan(), src1=_ci(5), src2=_ci(3),
            dst=_var("x"),
        ))
        self.assertEqual(out[0],
                         tac_ast.Copy(src=_ci(1), dst=_var("x")))

    def test_less_or_equal_boundary(self) -> None:
        out = _fold_one(tac_ast.Binary(
            op=tac_ast.LessOrEqual(), src1=_ci(3), src2=_ci(3),
            dst=_var("x"),
        ))
        self.assertEqual(out[0],
                         tac_ast.Copy(src=_ci(1), dst=_var("x")))

    def test_greater_or_equal_boundary(self) -> None:
        out = _fold_one(tac_ast.Binary(
            op=tac_ast.GreaterOrEqual(), src1=_ci(3), src2=_ci(3),
            dst=_var("x"),
        ))
        self.assertEqual(out[0],
                         tac_ast.Copy(src=_ci(1), dst=_var("x")))


class TestFoldJumpIf(unittest.TestCase):

    def test_jump_if_true_zero_dropped(self) -> None:
        fn = _fn(
            tac_ast.JumpIfTrue(condition=_ci(0), target="L"),
            _ret(),
        )
        self.assertEqual(constant_fold(fn).instructions, [_ret()])

    def test_jump_if_true_nonzero_replaced(self) -> None:
        fn = _fn(
            tac_ast.JumpIfTrue(condition=_ci(1), target="L"),
            _ret(),
        )
        self.assertEqual(constant_fold(fn).instructions,
                         [tac_ast.Jump(target="L"), _ret()])

    def test_jump_if_false_zero_replaced(self) -> None:
        fn = _fn(
            tac_ast.JumpIfFalse(condition=_ci(0), target="L"),
            _ret(),
        )
        self.assertEqual(constant_fold(fn).instructions,
                         [tac_ast.Jump(target="L"), _ret()])

    def test_jump_if_false_nonzero_dropped(self) -> None:
        fn = _fn(
            tac_ast.JumpIfFalse(condition=_ci(42), target="L"),
            _ret(),
        )
        self.assertEqual(constant_fold(fn).instructions, [_ret()])

    def test_jump_if_with_var_unchanged(self) -> None:
        instr = tac_ast.JumpIfTrue(condition=_var("c"), target="L")
        fn = _fn(instr, _ret())
        self.assertEqual(constant_fold(fn).instructions, [instr, _ret()])

    def test_jump_if_with_long_constant(self) -> None:
        # 16-bit non-zero value still folds.
        fn = _fn(
            tac_ast.JumpIfTrue(condition=_cl(0x0100), target="L"),
            _ret(),
        )
        self.assertEqual(constant_fold(fn).instructions,
                         [tac_ast.Jump(target="L"), _ret()])

    def test_jump_if_on_float_unchanged(self) -> None:
        instr = tac_ast.JumpIfTrue(condition=_cf("1.0"), target="L")
        fn = _fn(instr, _ret())
        self.assertEqual(constant_fold(fn).instructions, [instr, _ret()])


class TestProgramShape(unittest.TestCase):
    """The pass returns a fresh Function with the rewritten
    instruction list — the rest of the function (name, is_global,
    params) is preserved."""

    def test_function_metadata_preserved(self) -> None:
        fn = tac_ast.Function(
            name="foo",
            is_global=False,
            params=["a", "b"],
            instructions=[
                tac_ast.Binary(op=tac_ast.Add(),
                               src1=_ci(1), src2=_ci(2), dst=_var("x")),
                _ret(),
            ],
        )
        out = constant_fold(fn)
        self.assertEqual(out.name, "foo")
        self.assertFalse(out.is_global)
        self.assertEqual(out.params, ["a", "b"])
        self.assertEqual(out.instructions, [
            tac_ast.Copy(src=_ci(3), dst=_var("x")),
            _ret(),
        ])

    def test_non_foldable_instructions_pass_through(self) -> None:
        # A Copy and a non-foldable Binary stay as-is.
        instrs = [
            tac_ast.Copy(src=_ci(7), dst=_var("a")),
            tac_ast.Binary(op=tac_ast.Add(),
                           src1=_var("a"), src2=_ci(1), dst=_var("b")),
            _ret(),
        ]
        fn = _fn(*instrs)
        self.assertEqual(constant_fold(fn).instructions, instrs)


if __name__ == "__main__":
    unittest.main()
