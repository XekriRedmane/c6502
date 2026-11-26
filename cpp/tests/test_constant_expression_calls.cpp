// The calls that the tests of tests/test_constant_expression.py made of the functions
// that `check-captures` runs, each with what the Python function
// returned, changed, or raised, which the C++ port must return,
// change, or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed (step 8,
// groups G6 to G8). A call that another C++ case already checks
// isn't repeated here.
#include "doctest.h"
#include "record_case.h"
#include "constant_expression_case.h"

TEST_CASE("const_expr: test_constant_expression.py::TestEvaluateIntegerConstantExpression::test_binary_rejected_today (call 1)") {
  c6502::testing::check_constant_expression(
      "evaluate_integer_constant_expression",
      "Binary(Add Constant(ConstInt(1) None) Constant(ConstInt(2) None) None)\n",
      "{\"message\": \"operator in a constant expression has no data_type \\u2014 the constant evaluator requires the expression to be type-checked first\", \"type\": \"ConstantExpressionError\"}",
      true);
}

TEST_CASE("const_expr: test_constant_expression.py::TestEvaluateIntegerConstantExpression::test_cast_to_float_rejected (call 1)") {
  c6502::testing::check_constant_expression(
      "evaluate_integer_constant_expression",
      "Cast(Float Constant(ConstInt(1) None) None)\n",
      "{\"message\": \"integer constant expression cannot have a non-integer cast target (Float()); \\u00a76.6.6 requires integer type\", \"type\": \"ConstantExpressionError\"}",
      true);
}

TEST_CASE("const_expr: test_constant_expression.py::TestEvaluateIntegerConstantExpression::test_cast_to_int_sign_extends (call 1)") {
  c6502::testing::check_constant_expression(
      "evaluate_integer_constant_expression",
      "Cast(Int Constant(ConstLong(65535) None) None)\n",
      "-1 Int()",
      false);
}

TEST_CASE("const_expr: test_constant_expression.py::TestEvaluateIntegerConstantExpression::test_cast_to_int_truncates (call 1)") {
  c6502::testing::check_constant_expression(
      "evaluate_integer_constant_expression",
      "Cast(Int Constant(ConstLong(65536) None) None)\n",
      "0 Int()",
      false);
}

TEST_CASE("const_expr: test_constant_expression.py::TestEvaluateIntegerConstantExpression::test_cast_to_int_truncates (call 2)") {
  c6502::testing::check_constant_expression(
      "evaluate_integer_constant_expression",
      "Cast(Int Constant(ConstLong(256) None) None)\n",
      "256 Int()",
      false);
}

TEST_CASE("const_expr: test_constant_expression.py::TestEvaluateIntegerConstantExpression::test_cast_to_pointer_rejected (call 1)") {
  c6502::testing::check_constant_expression(
      "evaluate_integer_constant_expression",
      "Cast(Pointer(Int) Constant(ConstInt(0) None) None)\n",
      "{\"message\": \"integer constant expression cannot have a non-integer cast target (Pointer(referenced_type=Int())); \\u00a76.6.6 requires integer type\", \"type\": \"ConstantExpressionError\"}",
      true);
}

TEST_CASE("const_expr: test_constant_expression.py::TestEvaluateIntegerConstantExpression::test_cast_to_uint_zero_extends (call 1)") {
  c6502::testing::check_constant_expression(
      "evaluate_integer_constant_expression",
      "Cast(UInt Constant(ConstLong(255) None) None)\n",
      "255 UInt()",
      false);
}

TEST_CASE("const_expr: test_constant_expression.py::TestEvaluateIntegerConstantExpression::test_const_int_returns_value_and_type (call 1)") {
  c6502::testing::check_constant_expression(
      "evaluate_integer_constant_expression",
      "Constant(ConstInt(42) None)\n",
      "42 Int()",
      false);
}

TEST_CASE("const_expr: test_constant_expression.py::TestEvaluateIntegerConstantExpression::test_const_long_returns_long_type (call 1)") {
  c6502::testing::check_constant_expression(
      "evaluate_integer_constant_expression",
      "Constant(ConstLong(1000) None)\n",
      "1000 Long()",
      false);
}

TEST_CASE("const_expr: test_constant_expression.py::TestEvaluateIntegerConstantExpression::test_const_uint_returns_uint_type (call 1)") {
  c6502::testing::check_constant_expression(
      "evaluate_integer_constant_expression",
      "Constant(ConstUInt(200) None)\n",
      "200 UInt()",
      false);
}

TEST_CASE("const_expr: test_constant_expression.py::TestEvaluateIntegerConstantExpression::test_floating_constant_rejected (call 1)") {
  c6502::testing::check_constant_expression(
      "evaluate_integer_constant_expression",
      "Constant(ConstFloat(1065353216) None)\n",
      "{\"message\": \"floating constant ConstFloat(bits=1065353216) is not an integer constant\", \"type\": \"ConstantExpressionError\"}",
      true);
}

TEST_CASE("const_expr: test_constant_expression.py::TestEvaluateIntegerConstantExpression::test_nested_casts_compose (call 1)") {
  c6502::testing::check_constant_expression(
      "evaluate_integer_constant_expression",
      "Cast(Long Cast(Int Constant(ConstLong(5) None) None) None)\n",
      "5 Long()",
      false);
}

TEST_CASE("const_expr: test_constant_expression.py::TestEvaluateIntegerConstantExpression::test_sizeof_exp_reads_inner_data_type (call 1)") {
  c6502::testing::check_constant_expression(
      "evaluate_integer_constant_expression",
      "SizeOfExp(Var(\"x\" Long) None)\n",
      "4 UInt()",
      false);
}

TEST_CASE("const_expr: test_constant_expression.py::TestEvaluateIntegerConstantExpression::test_sizeof_exp_without_data_type_rejected (call 1)") {
  c6502::testing::check_constant_expression(
      "evaluate_integer_constant_expression",
      "SizeOfExp(Var(\"x\" None) None)\n",
      "{\"message\": \"sizeof's inner expression has no data_type \\u2014 the constant evaluator requires the operand to be type-checked first\", \"type\": \"ConstantExpressionError\"}",
      true);
}

TEST_CASE("const_expr: test_constant_expression.py::TestEvaluateIntegerConstantExpression::test_sizeof_type_array_multiplies (call 1)") {
  c6502::testing::check_constant_expression(
      "evaluate_integer_constant_expression",
      "SizeOfType(Array(Int 10) None)\n",
      "20 UInt()",
      false);
}

TEST_CASE("const_expr: test_constant_expression.py::TestEvaluateIntegerConstantExpression::test_sizeof_type_array_multiplies (call 2)") {
  c6502::testing::check_constant_expression(
      "evaluate_integer_constant_expression",
      "SizeOfType(Array(Array(Long 5) 3) None)\n",
      "60 UInt()",
      false);
}

TEST_CASE("const_expr: test_constant_expression.py::TestEvaluateIntegerConstantExpression::test_sizeof_type_double_returns_eight (call 1)") {
  c6502::testing::check_constant_expression(
      "evaluate_integer_constant_expression",
      "SizeOfType(Double None)\n",
      "8 UInt()",
      false);
}

TEST_CASE("const_expr: test_constant_expression.py::TestEvaluateIntegerConstantExpression::test_sizeof_type_int_returns_two (call 1)") {
  c6502::testing::check_constant_expression(
      "evaluate_integer_constant_expression",
      "SizeOfType(Int None)\n",
      "2 UInt()",
      false);
}

TEST_CASE("const_expr: test_constant_expression.py::TestEvaluateIntegerConstantExpression::test_sizeof_type_long_long_returns_eight (call 1)") {
  c6502::testing::check_constant_expression(
      "evaluate_integer_constant_expression",
      "SizeOfType(LongLong None)\n",
      "8 UInt()",
      false);
}

TEST_CASE("const_expr: test_constant_expression.py::TestEvaluateIntegerConstantExpression::test_sizeof_type_long_returns_four (call 1)") {
  c6502::testing::check_constant_expression(
      "evaluate_integer_constant_expression",
      "SizeOfType(Long None)\n",
      "4 UInt()",
      false);
}

TEST_CASE("const_expr: test_constant_expression.py::TestEvaluateIntegerConstantExpression::test_sizeof_type_pointer_returns_two (call 1)") {
  c6502::testing::check_constant_expression(
      "evaluate_integer_constant_expression",
      "SizeOfType(Pointer(Int) None)\n",
      "2 UInt()",
      false);
}

TEST_CASE("const_expr: test_constant_expression.py::TestEvaluateIntegerConstantExpression::test_sizeof_type_void_rejected (call 1)") {
  c6502::testing::check_constant_expression(
      "evaluate_integer_constant_expression",
      "SizeOfType(Void None)\n",
      "{\"message\": \"sizeof of incomplete or function type: Void()\", \"type\": \"ConstantExpressionError\"}",
      true);
}

TEST_CASE("const_expr: test_constant_expression.py::TestEvaluateIntegerConstantExpression::test_var_rejected (call 1)") {
  c6502::testing::check_constant_expression(
      "evaluate_integer_constant_expression",
      "Var(\"x\" None)\n",
      "{\"message\": \"expression is not an integer constant expression: Var(name='x', data_type=None)\", \"type\": \"ConstantExpressionError\"}",
      true);
}

TEST_CASE("const_expr: test_constant_expression.py::TestEvaluateOperators::test_arithmetic_and_shift (call 1)") {
  c6502::testing::check_constant_expression(
      "evaluate_integer_constant_expression",
      "Binary(Add Binary(LeftShift Constant(ConstInt(1) Int) Constant(ConstInt(8) Int) Int) Constant(ConstInt(44) Int) Int)\n",
      "300 Int()",
      false);
}

TEST_CASE("const_expr: test_constant_expression.py::TestEvaluateOperators::test_complement_unsigned (call 1)") {
  c6502::testing::check_constant_expression(
      "evaluate_integer_constant_expression",
      "Unary(Complement Constant(ConstUInt(0) UInt) UInt)\n",
      "65535 UInt()",
      false);
}

TEST_CASE("const_expr: test_constant_expression.py::TestEvaluateOperators::test_conditional_and_comparison (call 1)") {
  c6502::testing::check_constant_expression(
      "evaluate_integer_constant_expression",
      "Conditional(Binary(GreaterThan Constant(ConstInt(3) Int) Constant(ConstInt(2) Int) Int) Constant(ConstInt(10) Int) Constant(ConstInt(20) Int) Int)\n",
      "10 Int()",
      false);
}

TEST_CASE("const_expr: test_constant_expression.py::TestEvaluateOperators::test_division_by_zero_rejected (call 1)") {
  c6502::testing::check_constant_expression(
      "evaluate_integer_constant_expression",
      "Binary(Divide Constant(ConstInt(1) Int) Constant(ConstInt(0) Int) Int)\n",
      "{\"message\": \"division by zero in constant expression\", \"type\": \"ConstantExpressionError\"}",
      true);
}

TEST_CASE("const_expr: test_constant_expression.py::TestEvaluateOperators::test_division_truncates_toward_zero (call 1)") {
  c6502::testing::check_constant_expression(
      "evaluate_integer_constant_expression",
      "Binary(Divide Unary(Negate Constant(ConstInt(7) Int) Int) Constant(ConstInt(2) Int) Int)\n",
      "-3 Int()",
      false);
}

TEST_CASE("const_expr: test_constant_expression.py::TestEvaluateOperators::test_logical_and_skips_right_operand (call 1)") {
  c6502::testing::check_constant_expression(
      "evaluate_integer_constant_expression",
      "Binary(LogicalAnd Constant(ConstInt(0) Int) Binary(Divide Constant(ConstInt(1) Int) Constant(ConstInt(0) Int) Int) Int)\n",
      "0 Int()",
      false);
}

TEST_CASE("const_expr: test_constant_expression.py::TestEvaluateOperators::test_negate (call 1)") {
  c6502::testing::check_constant_expression(
      "evaluate_integer_constant_expression",
      "Unary(Negate Constant(ConstInt(1000) Int) Int)\n",
      "-1000 Int()",
      false);
}

TEST_CASE("const_expr: test_constant_expression.py::TestEvaluateOperators::test_signed_overflow_rejected (call 1)") {
  c6502::testing::check_constant_expression(
      "evaluate_integer_constant_expression",
      "Binary(Add Constant(ConstInt(32767) Int) Constant(ConstInt(1) Int) Int)\n",
      "{\"message\": \"signed overflow in constant expression (32768 does not fit in Int())\", \"type\": \"ConstantExpressionError\"}",
      true);
}
