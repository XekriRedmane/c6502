import unittest

import c99_ast
from passes.constant_expression import (
    ConstantExpressionError,
    evaluate_integer_constant_expression,
    validate_constant_expression,
)


def _const_int(v: int) -> c99_ast.Type_exp:
    return c99_ast.Constant(const=c99_ast.ConstInt(int=v))


def _const_long(v: int) -> c99_ast.Type_exp:
    return c99_ast.Constant(const=c99_ast.ConstLong(int=v))


def _const_uint(v: int) -> c99_ast.Type_exp:
    return c99_ast.Constant(const=c99_ast.ConstUInt(int=v))


def _const_float(v: float) -> c99_ast.Type_exp:
    return c99_ast.Constant(const=c99_ast.ConstFloat(float=v))


class TestEvaluateIntegerConstantExpression(unittest.TestCase):
    def test_const_int_returns_value_and_type(self):
        v, t = evaluate_integer_constant_expression(_const_int(42))
        self.assertEqual(v, 42)
        self.assertEqual(t, c99_ast.Int())

    def test_const_long_returns_long_type(self):
        v, t = evaluate_integer_constant_expression(_const_long(1000))
        self.assertEqual(v, 1000)
        self.assertEqual(t, c99_ast.Long())

    def test_const_uint_returns_uint_type(self):
        v, t = evaluate_integer_constant_expression(_const_uint(200))
        self.assertEqual(v, 200)
        self.assertEqual(t, c99_ast.UInt())

    def test_cast_to_int_truncates(self):
        # Casting 256 to Int (1 byte) wraps to 0.
        exp = c99_ast.Cast(target_type=c99_ast.Int(), exp=_const_long(256))
        v, t = evaluate_integer_constant_expression(exp)
        self.assertEqual(v, 0)
        self.assertEqual(t, c99_ast.Int())

    def test_cast_to_int_sign_extends(self):
        # Casting Long 0xFF (positive) to Int wraps to -1 (signed
        # 1-byte interpretation of 0xFF).
        exp = c99_ast.Cast(target_type=c99_ast.Int(), exp=_const_long(0xFF))
        v, t = evaluate_integer_constant_expression(exp)
        self.assertEqual(v, -1)
        self.assertEqual(t, c99_ast.Int())

    def test_cast_to_uint_zero_extends(self):
        # 0xFF as ULong cast to UInt — value preserved as 0xFF.
        exp = c99_ast.Cast(target_type=c99_ast.UInt(), exp=_const_long(0xFF))
        v, t = evaluate_integer_constant_expression(exp)
        self.assertEqual(v, 0xFF)
        self.assertEqual(t, c99_ast.UInt())

    def test_nested_casts_compose(self):
        # (long)(int)5L — same as (long)(int)5 — value 5.
        inner = c99_ast.Cast(target_type=c99_ast.Int(), exp=_const_long(5))
        outer = c99_ast.Cast(target_type=c99_ast.Long(), exp=inner)
        v, t = evaluate_integer_constant_expression(outer)
        self.assertEqual(v, 5)
        self.assertEqual(t, c99_ast.Long())

    def test_floating_constant_rejected(self):
        with self.assertRaises(ConstantExpressionError):
            evaluate_integer_constant_expression(_const_float(1.0))

    def test_cast_to_float_rejected(self):
        exp = c99_ast.Cast(target_type=c99_ast.Float(), exp=_const_int(1))
        with self.assertRaises(ConstantExpressionError):
            evaluate_integer_constant_expression(exp)

    def test_cast_to_pointer_rejected(self):
        exp = c99_ast.Cast(
            target_type=c99_ast.Pointer(referenced_type=c99_ast.Int()),
            exp=_const_int(0),
        )
        with self.assertRaises(ConstantExpressionError):
            evaluate_integer_constant_expression(exp)

    def test_var_rejected(self):
        with self.assertRaises(ConstantExpressionError):
            evaluate_integer_constant_expression(c99_ast.Var(name="x"))

    def test_binary_rejected_today(self):
        # Arithmetic on integer constants isn't folded yet — recursion
        # into Binary is a planned future expansion.
        exp = c99_ast.Binary(
            op=c99_ast.Add(),
            left=_const_int(1),
            right=_const_int(2),
        )
        with self.assertRaises(ConstantExpressionError):
            evaluate_integer_constant_expression(exp)


class TestValidateConstantExpression(unittest.TestCase):
    def test_constant_passes(self):
        validate_constant_expression(_const_int(5))

    def test_unary_passes(self):
        exp = c99_ast.Unary(op=c99_ast.Negate(), exp=_const_int(5))
        validate_constant_expression(exp)

    def test_binary_passes(self):
        exp = c99_ast.Binary(
            op=c99_ast.Add(), left=_const_int(1), right=_const_int(2),
        )
        validate_constant_expression(exp)

    def test_assignment_rejected(self):
        exp = c99_ast.Assignment(
            lval=c99_ast.Var(name="x"), rval=_const_int(1),
        )
        with self.assertRaisesRegex(ConstantExpressionError, "assignment"):
            validate_constant_expression(exp)

    def test_postfix_rejected(self):
        exp = c99_ast.Postfix(
            op=c99_ast.Increment(), operand=c99_ast.Var(name="x"),
        )
        with self.assertRaisesRegex(ConstantExpressionError, "postfix"):
            validate_constant_expression(exp)

    def test_function_call_rejected(self):
        exp = c99_ast.FunctionCall(name="f", args=[])
        with self.assertRaisesRegex(ConstantExpressionError, "function call"):
            validate_constant_expression(exp)


if __name__ == "__main__":
    unittest.main()
