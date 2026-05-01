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

import c99_ast
import fp_arith
import tac_ast
from passes.optimization.constant_folding import constant_fold
from passes.type_checking import LocalAttr, Symbol, SymbolTable


def _ci(v: int) -> tac_ast.Constant:
    return tac_ast.Constant(const=tac_ast.ConstInt(value=v))


def _cl(v: int) -> tac_ast.Constant:
    return tac_ast.Constant(const=tac_ast.ConstLong(value=v))


def _cll(v: int) -> tac_ast.Constant:
    return tac_ast.Constant(const=tac_ast.ConstLongLong(value=v))


def _cui(v: int) -> tac_ast.Constant:
    return tac_ast.Constant(const=tac_ast.ConstUInt(value=v))


def _cul(v: int) -> tac_ast.Constant:
    return tac_ast.Constant(const=tac_ast.ConstULong(value=v))


def _cull(v: int) -> tac_ast.Constant:
    return tac_ast.Constant(const=tac_ast.ConstULongLong(value=v))


def _cf(s: str) -> tac_ast.Constant:
    return tac_ast.Constant(const=tac_ast.ConstFloat(
        bits=fp_arith.single_string_to_bits(s),
    ))


def _var(name: str) -> tac_ast.Var:
    return tac_ast.Var(name=name)


def _ret() -> tac_ast.Ret:
    return tac_ast.Ret(val=tac_ast.Constant(const=tac_ast.ConstInt(value=0)))


def _fn(*instrs) -> tac_ast.Function:
    return tac_ast.Function(
        name="main",
        is_global=True,
        params=[],
        instructions=list(instrs),
    )


def _fold_one(instr, *, symbols=None) -> list:
    """Run constant_fold on a single-instruction function and
    return the resulting instruction list (useful for assertions
    that include the trailing Ret too)."""
    return constant_fold(_fn(instr, _ret()), symbols=symbols).instructions


def _symtab(**kwargs) -> SymbolTable:
    """Build a minimal SymbolTable from keyword args of the form
    `var_name=c99_type_class()`. Every entry is treated as a
    LocalAttr automatic-storage object — fold paths only read
    `Symbol.type`, so this is enough for tests."""
    st = SymbolTable()
    for name, t in kwargs.items():
        st[name] = Symbol(type=t, attrs=LocalAttr())
    return st


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

    def test_unary_complement_on_float_unchanged(self) -> None:
        # Bitwise complement is integer-only in C; the type checker
        # rejects ~3.14f, but constant_folding stays defensive and
        # leaves the instruction alone.
        instr = tac_ast.Unary(
            op=tac_ast.Complement(), src=_cf("1.5"), dst=_var("x"),
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

    def test_modulo_on_float_unchanged(self) -> None:
        # `%` is integer-only in C; the type checker rejects FP
        # operands, but constant_folding stays defensive.
        instr = tac_ast.Binary(
            op=tac_ast.Modulo(), src1=_cf("1.0"), src2=_cf("2.0"),
            dst=_var("x"),
        )
        out = _fold_one(instr)
        self.assertEqual(out[0], instr)

    def test_bitwise_on_float_unchanged(self) -> None:
        instr = tac_ast.Binary(
            op=tac_ast.BitwiseAnd(), src1=_cf("1.0"), src2=_cf("2.0"),
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

    # FP truthiness coverage lives below in TestFoldJumpIfFP.


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


def _cd(s: str) -> tac_ast.Constant:
    """Build a TAC ConstDouble from a decimal string. Goes through
    fp_arith so the bit pattern matches what the parser would
    produce from `s` written verbatim in source code."""
    return tac_ast.Constant(const=tac_ast.ConstDouble(
        bits=fp_arith.double_string_to_bits(s),
    ))


class TestFoldFPUnary(unittest.TestCase):

    def test_negate_single(self) -> None:
        out = _fold_one(tac_ast.Unary(
            op=tac_ast.Negate(), src=_cf("1.0"), dst=_var("x"),
        ))
        self.assertEqual(out[0],
                         tac_ast.Copy(src=_cf("-1.0"), dst=_var("x")))

    def test_negate_double(self) -> None:
        out = _fold_one(tac_ast.Unary(
            op=tac_ast.Negate(), src=_cd("3.14"), dst=_var("x"),
        ))
        self.assertEqual(out[0],
                         tac_ast.Copy(src=_cd("-3.14"), dst=_var("x")))

    def test_negate_positive_zero_yields_negative_zero(self) -> None:
        # Sign-bit flip — exact, no rounding. +0 → -0 (different
        # bit pattern, even though they compare equal).
        out = _fold_one(tac_ast.Unary(
            op=tac_ast.Negate(), src=_cf("0.0"), dst=_var("x"),
        ))
        expected_bits = fp_arith.single_string_to_bits("-0.0")
        self.assertEqual(out[0], tac_ast.Copy(
            src=tac_ast.Constant(
                const=tac_ast.ConstFloat(bits=expected_bits),
            ),
            dst=_var("x"),
        ))

    def test_logical_not_zero_single(self) -> None:
        out = _fold_one(tac_ast.Unary(
            op=tac_ast.LogicalNot(), src=_cf("0.0"), dst=_var("x"),
        ))
        self.assertEqual(out[0],
                         tac_ast.Copy(src=_ci(1), dst=_var("x")))

    def test_logical_not_negative_zero_single(self) -> None:
        # ±0 both compare equal to 0, so both are falsy and
        # `!` returns 1.
        out = _fold_one(tac_ast.Unary(
            op=tac_ast.LogicalNot(), src=_cf("-0.0"), dst=_var("x"),
        ))
        self.assertEqual(out[0],
                         tac_ast.Copy(src=_ci(1), dst=_var("x")))

    def test_logical_not_nonzero_single(self) -> None:
        out = _fold_one(tac_ast.Unary(
            op=tac_ast.LogicalNot(), src=_cf("1.5"), dst=_var("x"),
        ))
        self.assertEqual(out[0],
                         tac_ast.Copy(src=_ci(0), dst=_var("x")))

    def test_logical_not_nan_is_truthy(self) -> None:
        # NaN compares unequal to 0, so it's truthy → `!nan` = 0.
        nan = tac_ast.Constant(const=tac_ast.ConstFloat(
            bits=fp_arith.single_string_to_bits("nan"),
        ))
        out = _fold_one(tac_ast.Unary(
            op=tac_ast.LogicalNot(), src=nan, dst=_var("x"),
        ))
        self.assertEqual(out[0],
                         tac_ast.Copy(src=_ci(0), dst=_var("x")))

    def test_logical_not_double_returns_int(self) -> None:
        out = _fold_one(tac_ast.Unary(
            op=tac_ast.LogicalNot(), src=_cd("0.0"), dst=_var("x"),
        ))
        # Result is ConstInt regardless of operand precision.
        self.assertEqual(out[0],
                         tac_ast.Copy(src=_ci(1), dst=_var("x")))


class TestFoldFPArithmetic(unittest.TestCase):

    def test_single_add(self) -> None:
        out = _fold_one(tac_ast.Binary(
            op=tac_ast.Add(),
            src1=_cf("1.0"), src2=_cf("2.0"), dst=_var("x"),
        ))
        self.assertEqual(out[0],
                         tac_ast.Copy(src=_cf("3.0"), dst=_var("x")))

    def test_single_add_rounds_at_single_precision(self) -> None:
        # 0.1f + 0.2f rounds to a single-precision result that
        # differs from doing the same addition at double precision.
        # Pin the exact single-precision bit pattern.
        a_bits = fp_arith.single_string_to_bits("0.1")
        b_bits = fp_arith.single_string_to_bits("0.2")
        expected = fp_arith.single_add(a_bits, b_bits)
        out = _fold_one(tac_ast.Binary(
            op=tac_ast.Add(),
            src1=_cf("0.1"), src2=_cf("0.2"), dst=_var("x"),
        ))
        self.assertEqual(out[0], tac_ast.Copy(
            src=tac_ast.Constant(
                const=tac_ast.ConstFloat(bits=expected),
            ),
            dst=_var("x"),
        ))

    def test_double_sub(self) -> None:
        out = _fold_one(tac_ast.Binary(
            op=tac_ast.Subtract(),
            src1=_cd("5.0"), src2=_cd("2.0"), dst=_var("x"),
        ))
        self.assertEqual(out[0],
                         tac_ast.Copy(src=_cd("3.0"), dst=_var("x")))

    def test_single_mul(self) -> None:
        out = _fold_one(tac_ast.Binary(
            op=tac_ast.Multiply(),
            src1=_cf("1.5"), src2=_cf("2.0"), dst=_var("x"),
        ))
        self.assertEqual(out[0],
                         tac_ast.Copy(src=_cf("3.0"), dst=_var("x")))

    def test_double_div(self) -> None:
        out = _fold_one(tac_ast.Binary(
            op=tac_ast.Divide(),
            src1=_cd("1.0"), src2=_cd("2.0"), dst=_var("x"),
        ))
        self.assertEqual(out[0],
                         tac_ast.Copy(src=_cd("0.5"), dst=_var("x")))

    def test_single_overflow_to_inf(self) -> None:
        # A finite-times-finite that overflows single precision
        # rounds to +inf rather than raising.
        out = _fold_one(tac_ast.Binary(
            op=tac_ast.Multiply(),
            src1=_cf("1e20"), src2=_cf("1e20"), dst=_var("x"),
        ))
        self.assertEqual(out[0],
                         tac_ast.Copy(src=_cf("inf"), dst=_var("x")))

    def test_single_div_by_zero_yields_inf(self) -> None:
        # 1.0 / 0.0 = +inf in IEEE 754 — well-defined, so we fold.
        out = _fold_one(tac_ast.Binary(
            op=tac_ast.Divide(),
            src1=_cf("1.0"), src2=_cf("0.0"), dst=_var("x"),
        ))
        self.assertEqual(out[0],
                         tac_ast.Copy(src=_cf("inf"), dst=_var("x")))

    def test_single_zero_div_zero_yields_nan(self) -> None:
        # 0.0 / 0.0 is NaN — also well-defined, also folded.
        out = _fold_one(tac_ast.Binary(
            op=tac_ast.Divide(),
            src1=_cf("0.0"), src2=_cf("0.0"), dst=_var("x"),
        ))
        # Compare bit patterns: any NaN is fine, but it must have
        # the NaN exponent (all-ones) and a nonzero mantissa.
        result_bits = out[0].src.const.bits
        self.assertEqual((result_bits >> 23) & 0xFF, 0xFF,
                         msg=f"expected NaN exponent, got 0x{result_bits:08X}")
        self.assertNotEqual(result_bits & 0x7FFFFF, 0,
                            msg="expected nonzero mantissa for NaN")

    def test_single_inf_minus_inf_is_nan(self) -> None:
        out = _fold_one(tac_ast.Binary(
            op=tac_ast.Subtract(),
            src1=_cf("inf"), src2=_cf("inf"), dst=_var("x"),
        ))
        result_bits = out[0].src.const.bits
        self.assertEqual((result_bits >> 23) & 0xFF, 0xFF)
        self.assertNotEqual(result_bits & 0x7FFFFF, 0)

    def test_mismatched_fp_precision_unchanged(self) -> None:
        # ConstFloat + ConstDouble shouldn't happen post-type-check
        # (the type checker promotes to a common precision), but we
        # bail rather than guess.
        instr = tac_ast.Binary(
            op=tac_ast.Add(), src1=_cf("1.0"), src2=_cd("2.0"),
            dst=_var("x"),
        )
        out = _fold_one(instr)
        self.assertEqual(out[0], instr)


class TestFoldFPComparison(unittest.TestCase):

    def test_equal_true(self) -> None:
        out = _fold_one(tac_ast.Binary(
            op=tac_ast.Equal(),
            src1=_cf("1.5"), src2=_cf("1.5"), dst=_var("x"),
        ))
        self.assertEqual(out[0],
                         tac_ast.Copy(src=_ci(1), dst=_var("x")))

    def test_equal_signed_zero(self) -> None:
        # IEEE 754: +0 == -0.
        out = _fold_one(tac_ast.Binary(
            op=tac_ast.Equal(),
            src1=_cf("0.0"), src2=_cf("-0.0"), dst=_var("x"),
        ))
        self.assertEqual(out[0],
                         tac_ast.Copy(src=_ci(1), dst=_var("x")))

    def test_equal_nan_yields_zero(self) -> None:
        # NaN ≠ everything, including itself.
        nan = tac_ast.Constant(const=tac_ast.ConstFloat(
            bits=fp_arith.single_string_to_bits("nan"),
        ))
        out = _fold_one(tac_ast.Binary(
            op=tac_ast.Equal(), src1=nan, src2=nan, dst=_var("x"),
        ))
        self.assertEqual(out[0],
                         tac_ast.Copy(src=_ci(0), dst=_var("x")))

    def test_not_equal_nan_yields_one(self) -> None:
        nan = tac_ast.Constant(const=tac_ast.ConstFloat(
            bits=fp_arith.single_string_to_bits("nan"),
        ))
        out = _fold_one(tac_ast.Binary(
            op=tac_ast.NotEqual(), src1=nan, src2=nan, dst=_var("x"),
        ))
        self.assertEqual(out[0],
                         tac_ast.Copy(src=_ci(1), dst=_var("x")))

    def test_less_than_double(self) -> None:
        out = _fold_one(tac_ast.Binary(
            op=tac_ast.LessThan(),
            src1=_cd("1.0"), src2=_cd("2.0"), dst=_var("x"),
        ))
        self.assertEqual(out[0],
                         tac_ast.Copy(src=_ci(1), dst=_var("x")))

    def test_relational_against_nan_returns_zero(self) -> None:
        # Per IEEE 754: any of <, >, <=, >= against NaN is unordered
        # → false (0).
        nan = tac_ast.Constant(const=tac_ast.ConstFloat(
            bits=fp_arith.single_string_to_bits("nan"),
        ))
        for op_cls in (tac_ast.LessThan, tac_ast.GreaterThan,
                       tac_ast.LessOrEqual, tac_ast.GreaterOrEqual):
            with self.subTest(op=op_cls.__name__):
                out = _fold_one(tac_ast.Binary(
                    op=op_cls(), src1=nan, src2=_cf("1.0"),
                    dst=_var("x"),
                ))
                self.assertEqual(out[0],
                                 tac_ast.Copy(src=_ci(0), dst=_var("x")))

    def test_less_or_equal_boundary(self) -> None:
        out = _fold_one(tac_ast.Binary(
            op=tac_ast.LessOrEqual(),
            src1=_cf("3.0"), src2=_cf("3.0"), dst=_var("x"),
        ))
        self.assertEqual(out[0],
                         tac_ast.Copy(src=_ci(1), dst=_var("x")))

    def test_mismatched_precision_unchanged(self) -> None:
        instr = tac_ast.Binary(
            op=tac_ast.Equal(), src1=_cf("1.0"), src2=_cd("1.0"),
            dst=_var("x"),
        )
        out = _fold_one(instr)
        self.assertEqual(out[0], instr)


class TestFoldJumpIfFP(unittest.TestCase):

    def test_jump_if_true_zero_dropped(self) -> None:
        fn = _fn(
            tac_ast.JumpIfTrue(condition=_cf("0.0"), target="L"),
            _ret(),
        )
        self.assertEqual(constant_fold(fn).instructions, [_ret()])

    def test_jump_if_true_negative_zero_dropped(self) -> None:
        # -0.0 compares equal to 0 → falsy → not taken.
        fn = _fn(
            tac_ast.JumpIfTrue(condition=_cf("-0.0"), target="L"),
            _ret(),
        )
        self.assertEqual(constant_fold(fn).instructions, [_ret()])

    def test_jump_if_true_nonzero_replaced(self) -> None:
        fn = _fn(
            tac_ast.JumpIfTrue(condition=_cf("1.5"), target="L"),
            _ret(),
        )
        self.assertEqual(constant_fold(fn).instructions,
                         [tac_ast.Jump(target="L"), _ret()])

    def test_jump_if_true_nan_replaced(self) -> None:
        # NaN ≠ 0 → truthy → jump taken.
        nan = tac_ast.Constant(const=tac_ast.ConstFloat(
            bits=fp_arith.single_string_to_bits("nan"),
        ))
        fn = _fn(
            tac_ast.JumpIfTrue(condition=nan, target="L"),
            _ret(),
        )
        self.assertEqual(constant_fold(fn).instructions,
                         [tac_ast.Jump(target="L"), _ret()])

    def test_jump_if_false_zero_replaced(self) -> None:
        fn = _fn(
            tac_ast.JumpIfFalse(condition=_cf("0.0"), target="L"),
            _ret(),
        )
        self.assertEqual(constant_fold(fn).instructions,
                         [tac_ast.Jump(target="L"), _ret()])

    def test_jump_if_false_nan_dropped(self) -> None:
        # NaN is truthy, so JumpIfFalse drops.
        nan = tac_ast.Constant(const=tac_ast.ConstDouble(
            bits=fp_arith.double_string_to_bits("nan"),
        ))
        fn = _fn(
            tac_ast.JumpIfFalse(condition=nan, target="L"),
            _ret(),
        )
        self.assertEqual(constant_fold(fn).instructions, [_ret()])


class TestFoldSignExtend(unittest.TestCase):
    """SignExtend(Constant, Var) folds when the symbol table tells us
    the dst's TAC width. Source signedness is encoded by the choice
    of node (vs. ZeroExtend), so we sign-interpret the source value
    at its TAC variant's width."""

    def test_int_to_long(self) -> None:
        # Int(-1) sign-extended to Long stays -1 (16-bit signed).
        symbols = _symtab(x=c99_ast.Long())
        out = _fold_one(
            tac_ast.SignExtend(src=_ci(-1), dst=_var("x")),
            symbols=symbols,
        )
        self.assertEqual(out[0],
                         tac_ast.Copy(src=_cl(-1), dst=_var("x")))

    def test_int_to_long_long(self) -> None:
        symbols = _symtab(x=c99_ast.LongLong())
        out = _fold_one(
            tac_ast.SignExtend(src=_ci(-1), dst=_var("x")),
            symbols=symbols,
        )
        self.assertEqual(out[0],
                         tac_ast.Copy(src=_cll(-1), dst=_var("x")))

    def test_long_to_long_long(self) -> None:
        symbols = _symtab(x=c99_ast.LongLong())
        out = _fold_one(
            tac_ast.SignExtend(src=_cl(-1234), dst=_var("x")),
            symbols=symbols,
        )
        self.assertEqual(out[0],
                         tac_ast.Copy(src=_cll(-1234), dst=_var("x")))

    def test_int_to_unsigned_long_widens_via_signed(self) -> None:
        # SignExtend's contract is "the source is signed"; the dst's
        # signedness picks the result variant. ConstInt(-1) sign-
        # extended to ULong is the bit pattern 0xFFFF, stored as
        # ConstULong(65535).
        symbols = _symtab(x=c99_ast.ULong())
        out = _fold_one(
            tac_ast.SignExtend(src=_ci(-1), dst=_var("x")),
            symbols=symbols,
        )
        self.assertEqual(out[0],
                         tac_ast.Copy(src=_cul(65535), dst=_var("x")))

    def test_skip_without_symbols(self) -> None:
        instr = tac_ast.SignExtend(src=_ci(-1), dst=_var("x"))
        out = _fold_one(instr)
        self.assertEqual(out[0], instr)

    def test_skip_unknown_var(self) -> None:
        # Symbol table doesn't have an entry for `x` — bail rather
        # than guess at the dst width.
        instr = tac_ast.SignExtend(src=_ci(-1), dst=_var("x"))
        out = _fold_one(instr, symbols=_symtab())
        self.assertEqual(out[0], instr)

    def test_skip_with_var_src(self) -> None:
        # Non-Constant src — nothing to fold.
        instr = tac_ast.SignExtend(src=_var("y"), dst=_var("x"))
        out = _fold_one(instr, symbols=_symtab(
            x=c99_ast.Long(), y=c99_ast.Int(),
        ))
        self.assertEqual(out[0], instr)


class TestFoldZeroExtend(unittest.TestCase):
    """ZeroExtend(Constant, Var): mask src to its unsigned bit
    pattern, then store at dst width."""

    def test_uint_to_ulong_negative_value_masks(self) -> None:
        # ConstInt(-1) is the 8-bit pattern 0xFF; zero-extended to 16
        # bits → 0x00FF = 255. Result variant is ConstULong (the
        # dst's c99 type is ULong).
        symbols = _symtab(x=c99_ast.ULong())
        out = _fold_one(
            tac_ast.ZeroExtend(src=_ci(-1), dst=_var("x")),
            symbols=symbols,
        )
        self.assertEqual(out[0],
                         tac_ast.Copy(src=_cul(255), dst=_var("x")))

    def test_uint_to_long_long(self) -> None:
        symbols = _symtab(x=c99_ast.ULongLong())
        out = _fold_one(
            tac_ast.ZeroExtend(src=_ci(-1), dst=_var("x")),
            symbols=symbols,
        )
        self.assertEqual(out[0],
                         tac_ast.Copy(src=_cull(255), dst=_var("x")))

    def test_ulong_to_ulong_long(self) -> None:
        # ConstULong(65535) is the 16-bit pattern 0xFFFF; zero-extend
        # to 32 bits → 0x0000FFFF = 65535.
        symbols = _symtab(x=c99_ast.ULongLong())
        out = _fold_one(
            tac_ast.ZeroExtend(src=_cul(65535), dst=_var("x")),
            symbols=symbols,
        )
        self.assertEqual(out[0],
                         tac_ast.Copy(src=_cull(65535), dst=_var("x")))

    def test_skip_without_symbols(self) -> None:
        instr = tac_ast.ZeroExtend(src=_ci(-1), dst=_var("x"))
        out = _fold_one(instr)
        self.assertEqual(out[0], instr)


class TestFoldTruncate(unittest.TestCase):
    """Truncate(Constant, Var): keep the low dst-width bytes,
    signed-canonicalize at that width."""

    def test_long_to_int_keeps_low_byte(self) -> None:
        # 0x1234 → 0x34 = 52 in signed 8-bit.
        symbols = _symtab(x=c99_ast.Int())
        out = _fold_one(
            tac_ast.Truncate(src=_cl(0x1234), dst=_var("x")),
            symbols=symbols,
        )
        self.assertEqual(out[0],
                         tac_ast.Copy(src=_ci(0x34), dst=_var("x")))

    def test_long_to_int_canonicalizes_sign(self) -> None:
        # 0x12FF → 0xFF, signed 8-bit = -1.
        symbols = _symtab(x=c99_ast.Int())
        out = _fold_one(
            tac_ast.Truncate(src=_cl(0x12FF), dst=_var("x")),
            symbols=symbols,
        )
        self.assertEqual(out[0],
                         tac_ast.Copy(src=_ci(-1), dst=_var("x")))

    def test_long_long_to_long(self) -> None:
        symbols = _symtab(x=c99_ast.Long())
        out = _fold_one(
            tac_ast.Truncate(src=_cll(0x12345678), dst=_var("x")),
            symbols=symbols,
        )
        # Low 16 bits of 0x12345678 = 0x5678; fits 16-bit signed.
        self.assertEqual(out[0],
                         tac_ast.Copy(src=_cl(0x5678), dst=_var("x")))

    def test_long_long_to_int(self) -> None:
        symbols = _symtab(x=c99_ast.Int())
        out = _fold_one(
            tac_ast.Truncate(src=_cll(0x12345678), dst=_var("x")),
            symbols=symbols,
        )
        self.assertEqual(out[0],
                         tac_ast.Copy(src=_ci(0x78), dst=_var("x")))

    def test_skip_without_symbols(self) -> None:
        instr = tac_ast.Truncate(src=_cl(0x1234), dst=_var("x"))
        out = _fold_one(instr)
        self.assertEqual(out[0], instr)


class TestFoldFPCrossPrecision(unittest.TestCase):
    """FloatToDouble / DoubleToFloat fold without needing a symbol
    table — the src/dst precision is determined by the node class."""

    def test_float_to_double(self) -> None:
        out = _fold_one(tac_ast.FloatToDouble(
            src=_cf("1.5"), dst=_var("d"),
        ))
        # Widening single → double is exact for finite singles.
        expected_bits = fp_arith.single_bits_to_double_bits(
            fp_arith.single_string_to_bits("1.5"),
        )
        self.assertEqual(out[0], tac_ast.Copy(
            src=tac_ast.Constant(
                const=tac_ast.ConstDouble(bits=expected_bits),
            ),
            dst=_var("d"),
        ))

    def test_double_to_float_round_trip(self) -> None:
        out = _fold_one(tac_ast.DoubleToFloat(
            src=_cd("1.5"), dst=_var("f"),
        ))
        # 1.5 is exactly representable in single, so the round trip
        # is bit-identical to single("1.5").
        expected_bits = fp_arith.single_string_to_bits("1.5")
        self.assertEqual(out[0], tac_ast.Copy(
            src=tac_ast.Constant(
                const=tac_ast.ConstFloat(bits=expected_bits),
            ),
            dst=_var("f"),
        ))

    def test_double_to_float_overflow_to_inf(self) -> None:
        # 1e300 doesn't fit single — narrows to +inf.
        out = _fold_one(tac_ast.DoubleToFloat(
            src=_cd("1e300"), dst=_var("f"),
        ))
        self.assertEqual(out[0],
                         tac_ast.Copy(src=_cf("inf"), dst=_var("f")))

    def test_skip_with_var_src(self) -> None:
        instr = tac_ast.FloatToDouble(src=_var("a"), dst=_var("d"))
        out = _fold_one(instr)
        self.assertEqual(out[0], instr)

    def test_skip_with_wrong_const_variant(self) -> None:
        # FloatToDouble with a ConstDouble source is a malformed AST —
        # bail rather than fold a same-precision-to-double widening.
        instr = tac_ast.FloatToDouble(
            src=_cd("1.5"), dst=_var("d"),
        )
        out = _fold_one(instr)
        self.assertEqual(out[0], instr)


class TestFoldIntToFP(unittest.TestCase):
    """IntToFloat / IntToDouble: signedness rides on the operand's
    TAC variant — Const{Int,Long,LongLong} treat the value as
    signed; Const{UInt,ULong,ULongLong} treat it as the non-negative
    bit pattern. Both fold without ambiguity."""

    def test_int_to_float_positive(self) -> None:
        out = _fold_one(tac_ast.IntToFloat(
            src=_ci(5), dst=_var("f"),
        ))
        expected = fp_arith.int_to_single_bits(5)
        self.assertEqual(out[0], tac_ast.Copy(
            src=tac_ast.Constant(
                const=tac_ast.ConstFloat(bits=expected),
            ),
            dst=_var("f"),
        ))

    def test_int_to_double_positive(self) -> None:
        out = _fold_one(tac_ast.IntToDouble(
            src=_cl(1234), dst=_var("d"),
        ))
        expected = fp_arith.int_to_double_bits(1234)
        self.assertEqual(out[0], tac_ast.Copy(
            src=tac_ast.Constant(
                const=tac_ast.ConstDouble(bits=expected),
            ),
            dst=_var("d"),
        ))

    def test_long_long_to_double_positive(self) -> None:
        out = _fold_one(tac_ast.IntToDouble(
            src=_cll(2_000_000), dst=_var("d"),
        ))
        expected = fp_arith.int_to_double_bits(2_000_000)
        self.assertEqual(out[0], tac_ast.Copy(
            src=tac_ast.Constant(
                const=tac_ast.ConstDouble(bits=expected),
            ),
            dst=_var("d"),
        ))

    def test_int_to_float_zero(self) -> None:
        out = _fold_one(tac_ast.IntToFloat(
            src=_ci(0), dst=_var("f"),
        ))
        expected = fp_arith.int_to_single_bits(0)
        self.assertEqual(out[0], tac_ast.Copy(
            src=tac_ast.Constant(
                const=tac_ast.ConstFloat(bits=expected),
            ),
            dst=_var("f"),
        ))

    def test_negative_signed_int_folds_to_negative_fp(self) -> None:
        # ConstInt(-1) is signed → -1.0f.
        out = _fold_one(tac_ast.IntToFloat(
            src=_ci(-1), dst=_var("f"),
        ))
        expected = fp_arith.int_to_single_bits(-1)
        self.assertEqual(out[0], tac_ast.Copy(
            src=tac_ast.Constant(
                const=tac_ast.ConstFloat(bits=expected),
            ),
            dst=_var("f"),
        ))

    def test_max_unsigned_int_folds_to_positive_fp(self) -> None:
        # ConstUInt(255) is unsigned → 255.0f. Same bit pattern as
        # ConstInt(-1), but the variant tells us to read it
        # unsigned.
        out = _fold_one(tac_ast.IntToFloat(
            src=_cui(255), dst=_var("f"),
        ))
        expected = fp_arith.int_to_single_bits(255)
        self.assertEqual(out[0], tac_ast.Copy(
            src=tac_ast.Constant(
                const=tac_ast.ConstFloat(bits=expected),
            ),
            dst=_var("f"),
        ))

    def test_negative_signed_long_folds_to_negative_double(self) -> None:
        out = _fold_one(tac_ast.IntToDouble(
            src=_cl(-1234), dst=_var("d"),
        ))
        expected = fp_arith.int_to_double_bits(-1234)
        self.assertEqual(out[0], tac_ast.Copy(
            src=tac_ast.Constant(
                const=tac_ast.ConstDouble(bits=expected),
            ),
            dst=_var("d"),
        ))

    def test_skip_with_var_src(self) -> None:
        instr = tac_ast.IntToFloat(src=_var("a"), dst=_var("f"))
        out = _fold_one(instr)
        self.assertEqual(out[0], instr)


class TestFoldFPToInt(unittest.TestCase):
    """FloatToInt / DoubleToInt: truncate toward zero (C99 §6.3.1.4),
    mask to dst width. Bails on NaN / ±inf."""

    def test_float_to_int(self) -> None:
        symbols = _symtab(x=c99_ast.Int())
        out = _fold_one(
            tac_ast.FloatToInt(src=_cf("3.7"), dst=_var("x")),
            symbols=symbols,
        )
        # Truncation toward zero: 3.7 → 3; fits 8-bit signed.
        self.assertEqual(out[0],
                         tac_ast.Copy(src=_ci(3), dst=_var("x")))

    def test_float_to_int_negative(self) -> None:
        symbols = _symtab(x=c99_ast.Int())
        out = _fold_one(
            tac_ast.FloatToInt(src=_cf("-3.7"), dst=_var("x")),
            symbols=symbols,
        )
        # -3.7 → -3.
        self.assertEqual(out[0],
                         tac_ast.Copy(src=_ci(-3), dst=_var("x")))

    def test_double_to_long(self) -> None:
        symbols = _symtab(x=c99_ast.Long())
        out = _fold_one(
            tac_ast.DoubleToInt(src=_cd("1234.5"), dst=_var("x")),
            symbols=symbols,
        )
        self.assertEqual(out[0],
                         tac_ast.Copy(src=_cl(1234), dst=_var("x")))

    def test_double_to_long_long(self) -> None:
        symbols = _symtab(x=c99_ast.LongLong())
        out = _fold_one(
            tac_ast.DoubleToInt(src=_cd("123456.789"), dst=_var("x")),
            symbols=symbols,
        )
        self.assertEqual(out[0],
                         tac_ast.Copy(src=_cll(123456), dst=_var("x")))

    def test_float_to_uint(self) -> None:
        # Dst signedness picks the result variant: ConstUInt(3)
        # rather than ConstInt(3). Bit pattern is identical, but the
        # variant identity differs so downstream consumers read the
        # right signedness.
        symbols = _symtab(x=c99_ast.UInt())
        out = _fold_one(
            tac_ast.FloatToInt(src=_cf("3.7"), dst=_var("x")),
            symbols=symbols,
        )
        self.assertEqual(out[0],
                         tac_ast.Copy(src=_cui(3), dst=_var("x")))

    def test_nan_unchanged(self) -> None:
        nan = tac_ast.Constant(const=tac_ast.ConstFloat(
            bits=fp_arith.single_string_to_bits("nan"),
        ))
        instr = tac_ast.FloatToInt(src=nan, dst=_var("x"))
        out = _fold_one(instr, symbols=_symtab(x=c99_ast.Int()))
        self.assertEqual(out[0], instr)

    def test_inf_unchanged(self) -> None:
        instr = tac_ast.FloatToInt(src=_cf("inf"), dst=_var("x"))
        out = _fold_one(instr, symbols=_symtab(x=c99_ast.Int()))
        self.assertEqual(out[0], instr)

    def test_double_inf_unchanged(self) -> None:
        instr = tac_ast.DoubleToInt(src=_cd("-inf"), dst=_var("x"))
        out = _fold_one(instr, symbols=_symtab(x=c99_ast.Long()))
        self.assertEqual(out[0], instr)

    def test_skip_without_symbols(self) -> None:
        instr = tac_ast.FloatToInt(src=_cf("3.7"), dst=_var("x"))
        out = _fold_one(instr)
        self.assertEqual(out[0], instr)


class TestFoldSignednessAware(unittest.TestCase):
    """Folds where signedness affects the result, dispatched off the
    operand variant. These pin the contract that `tac_to_asm` uses
    BCC/BCS for unsigned ordering and `lsr*` for unsigned right shift
    — the constant folder has to agree, otherwise --optimize would
    diverge from the unoptimized lowering."""

    def test_unsigned_less_than_above_signed_threshold(self) -> None:
        # 200 < 100 is false in unsigned; 200 (signed -56) < 100 is
        # TRUE in signed. We want unsigned → false.
        out = _fold_one(tac_ast.Binary(
            op=tac_ast.LessThan(),
            src1=_cui(200), src2=_cui(100), dst=_var("x"),
        ))
        self.assertEqual(out[0],
                         tac_ast.Copy(src=_ci(0), dst=_var("x")))

    def test_signed_less_than_above_signed_threshold(self) -> None:
        # ConstInt(-56) is bit pattern 0xC8 (= unsigned 200). Signed
        # interpretation: -56 < 100 → true.
        out = _fold_one(tac_ast.Binary(
            op=tac_ast.LessThan(),
            src1=_ci(-56), src2=_ci(100), dst=_var("x"),
        ))
        self.assertEqual(out[0],
                         tac_ast.Copy(src=_ci(1), dst=_var("x")))

    def test_unsigned_right_shift_zero_fills(self) -> None:
        # ConstUInt(200) is bit pattern 0xC8; logical right shift by
        # 1 → 0x64 = 100. Arithmetic shift would also give 100 here
        # because 200 fits the unsigned-positive interpretation, but
        # the next test exercises the divergence.
        out = _fold_one(tac_ast.Binary(
            op=tac_ast.RightShift(),
            src1=_cui(200), src2=_cui(1), dst=_var("x"),
        ))
        self.assertEqual(out[0],
                         tac_ast.Copy(src=_cui(100), dst=_var("x")))

    def test_signed_right_shift_negative_value_preserves_sign(self) -> None:
        # -8 >> 1 = -4 (arithmetic shift, sign-preserving).
        out = _fold_one(tac_ast.Binary(
            op=tac_ast.RightShift(),
            src1=_ci(-8), src2=_ci(1), dst=_var("x"),
        ))
        self.assertEqual(out[0],
                         tac_ast.Copy(src=_ci(-4), dst=_var("x")))

    def test_unsigned_division_uses_unsigned_arithmetic(self) -> None:
        # 200 / 3 (unsigned) = 66; signed (-56 / 3) = -18 (truncation
        # toward zero). The variants must produce different results
        # to match codegen.
        out_unsigned = _fold_one(tac_ast.Binary(
            op=tac_ast.Divide(),
            src1=_cui(200), src2=_cui(3), dst=_var("x"),
        ))
        self.assertEqual(out_unsigned[0],
                         tac_ast.Copy(src=_cui(66), dst=_var("x")))

    def test_unsigned_arithmetic_canonicalizes_to_non_negative(self) -> None:
        # Add wraps at 8 bits: 200 + 100 = 300 mod 256 = 44.
        # Unsigned canonicalization keeps it in 0..255.
        out = _fold_one(tac_ast.Binary(
            op=tac_ast.Add(),
            src1=_cui(200), src2=_cui(100), dst=_var("x"),
        ))
        self.assertEqual(out[0],
                         tac_ast.Copy(src=_cui(44), dst=_var("x")))

    def test_signed_arithmetic_canonicalizes_signed(self) -> None:
        # 127 + 1 = 128 → signed 8-bit wraps to -128. Same bit
        # pattern as ConstUInt(128), but the value field is negative.
        out = _fold_one(tac_ast.Binary(
            op=tac_ast.Add(),
            src1=_ci(127), src2=_ci(1), dst=_var("x"),
        ))
        self.assertEqual(out[0],
                         tac_ast.Copy(src=_ci(-128), dst=_var("x")))

    def test_mismatched_signedness_unchanged(self) -> None:
        # ConstInt + ConstUInt shouldn't happen post-type-check (both
        # operands are promoted to the same type), but bail rather
        # than guess at the result variant.
        instr = tac_ast.Binary(
            op=tac_ast.Add(),
            src1=_ci(1), src2=_cui(2), dst=_var("x"),
        )
        out = _fold_one(instr)
        self.assertEqual(out[0], instr)

    def test_unsigned_negate_wraps(self) -> None:
        # -ConstUInt(1) at 8 bits → bit pattern 0xFF = 255.
        out = _fold_one(tac_ast.Unary(
            op=tac_ast.Negate(), src=_cui(1), dst=_var("x"),
        ))
        self.assertEqual(out[0],
                         tac_ast.Copy(src=_cui(255), dst=_var("x")))

    def test_unsigned_complement(self) -> None:
        out = _fold_one(tac_ast.Unary(
            op=tac_ast.Complement(), src=_cui(0), dst=_var("x"),
        ))
        # ~0 in 8-bit unsigned = 255.
        self.assertEqual(out[0],
                         tac_ast.Copy(src=_cui(255), dst=_var("x")))


class TestFoldCopy(unittest.TestCase):
    """Copy(Constant, Var) where the source variant disagrees with
    the dst's c99 type. Same-width signed↔unsigned casts get elided
    in `c99_to_tac` (the bit pattern is identical), so a same-width
    cast feeding into an Assignment can leave a Copy whose constant
    variant doesn't match the dst — this fold canonicalizes it."""

    def test_signed_to_unsigned_same_width(self) -> None:
        # `unsigned int x = (unsigned int)1;` lowers to
        # Copy(ConstInt(1), Var(x)) with x typed UInt; the cast was
        # elided. Fold rewraps as ConstUInt(1).
        symbols = _symtab(x=c99_ast.UInt())
        out = _fold_one(
            tac_ast.Copy(src=_ci(1), dst=_var("x")),
            symbols=symbols,
        )
        self.assertEqual(out[0],
                         tac_ast.Copy(src=_cui(1), dst=_var("x")))

    def test_signed_to_unsigned_negative_canonicalizes(self) -> None:
        # ConstInt(-1) bit pattern 0xFF reinterpreted as ConstUInt:
        # value 255.
        symbols = _symtab(x=c99_ast.UInt())
        out = _fold_one(
            tac_ast.Copy(src=_ci(-1), dst=_var("x")),
            symbols=symbols,
        )
        self.assertEqual(out[0],
                         tac_ast.Copy(src=_cui(255), dst=_var("x")))

    def test_unsigned_to_signed_high_bit_set(self) -> None:
        # ConstUInt(200) bit pattern 0xC8 reinterpreted as ConstInt:
        # signed 8-bit value -56.
        symbols = _symtab(x=c99_ast.Int())
        out = _fold_one(
            tac_ast.Copy(src=_cui(200), dst=_var("x")),
            symbols=symbols,
        )
        self.assertEqual(out[0],
                         tac_ast.Copy(src=_ci(-56), dst=_var("x")))

    def test_signed_long_to_unsigned_long(self) -> None:
        symbols = _symtab(x=c99_ast.ULong())
        out = _fold_one(
            tac_ast.Copy(src=_cl(-1), dst=_var("x")),
            symbols=symbols,
        )
        self.assertEqual(out[0],
                         tac_ast.Copy(src=_cul(0xFFFF), dst=_var("x")))

    def test_signed_long_long_to_unsigned_long_long(self) -> None:
        symbols = _symtab(x=c99_ast.ULongLong())
        out = _fold_one(
            tac_ast.Copy(src=_cll(-1), dst=_var("x")),
            symbols=symbols,
        )
        self.assertEqual(out[0],
                         tac_ast.Copy(src=_cull(0xFFFFFFFF), dst=_var("x")))

    def test_char_dst_variant_is_int(self) -> None:
        # `char` maps to ConstInt (same width and signedness as Int —
        # plain char is signed in c6502). A ConstUInt source is
        # rewrapped as ConstInt.
        symbols = _symtab(x=c99_ast.Char())
        out = _fold_one(
            tac_ast.Copy(src=_cui(200), dst=_var("x")),
            symbols=symbols,
        )
        self.assertEqual(out[0],
                         tac_ast.Copy(src=_ci(-56), dst=_var("x")))

    def test_uchar_dst_variant_is_uint(self) -> None:
        symbols = _symtab(x=c99_ast.UChar())
        out = _fold_one(
            tac_ast.Copy(src=_ci(-1), dst=_var("x")),
            symbols=symbols,
        )
        self.assertEqual(out[0],
                         tac_ast.Copy(src=_cui(255), dst=_var("x")))

    def test_matching_variant_unchanged(self) -> None:
        # Variant already matches the dst's c99 type — nothing to do.
        instr = tac_ast.Copy(src=_ci(7), dst=_var("x"))
        out = _fold_one(instr, symbols=_symtab(x=c99_ast.Int()))
        self.assertEqual(out[0], instr)

    def test_skip_without_symbols(self) -> None:
        # No symbol table → no way to know the dst's type. Bail.
        instr = tac_ast.Copy(src=_ci(1), dst=_var("x"))
        out = _fold_one(instr)
        self.assertEqual(out[0], instr)

    def test_skip_unknown_var(self) -> None:
        # Symbol table doesn't know `x`. Bail rather than guess.
        instr = tac_ast.Copy(src=_ci(1), dst=_var("x"))
        out = _fold_one(instr, symbols=_symtab())
        self.assertEqual(out[0], instr)

    def test_skip_with_var_src(self) -> None:
        # Non-Constant src — Copy isn't carrying a foldable constant.
        instr = tac_ast.Copy(src=_var("y"), dst=_var("x"))
        out = _fold_one(instr, symbols=_symtab(
            x=c99_ast.UInt(), y=c99_ast.Int(),
        ))
        self.assertEqual(out[0], instr)

    def test_skip_fp_dst(self) -> None:
        # Float-typed dsts don't have a signedness — Copy of an FP
        # constant carries no signedness ambiguity, and Copy of an
        # integer constant into a Float dst shouldn't reach this pass
        # anyway (c99_to_tac would emit IntToFloat). Either way: no
        # fold.
        instr = tac_ast.Copy(src=_cf("1.0"), dst=_var("x"))
        out = _fold_one(instr, symbols=_symtab(x=c99_ast.Float()))
        self.assertEqual(out[0], instr)

    def test_skip_width_mismatch(self) -> None:
        # A width-changing Copy shouldn't reach this pass (c99_to_tac
        # emits SignExtend / ZeroExtend / Truncate for those). Bail
        # rather than silently widening or narrowing.
        instr = tac_ast.Copy(src=_ci(1), dst=_var("x"))
        out = _fold_one(instr, symbols=_symtab(x=c99_ast.Long()))
        self.assertEqual(out[0], instr)


if __name__ == "__main__":
    unittest.main()
