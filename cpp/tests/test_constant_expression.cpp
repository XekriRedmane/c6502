// The calls of the constant-expression evaluator and validator that the
// Python unit tests make (tests/test_constant_expression.py), each
// recorded with Python's result: the expression's dump, and the value
// with the type's repr, or the message of the ConstantExpressionError.
#include <string>

#include "c99_ast.h"
#include "doctest.h"
#include "passes/constant_expression.h"

namespace {

namespace ce = ::c6502::passes::constant_expression;

struct Case {
  bool evaluate;  // evaluate_integer_constant_expression, or validate
  const char* input;
  bool error;
  const char* expected;
};

const Case kCases[] = {
    {true, R"tc(Binary(Add Constant(ConstInt(1) None) Constant(ConstInt(2) None) None))tc", true, R"tc(operator in a constant expression has no data_type — the constant evaluator requires the expression to be type-checked first)tc"},
    {true, R"tc(Cast(Float Constant(ConstInt(1) None) None))tc", true, R"tc(integer constant expression cannot have a non-integer cast target (Float()); §6.6.6 requires integer type)tc"},
    {true, R"tc(Cast(Int Constant(ConstLong(65535) None) None))tc", false, R"tc(-1 Int())tc"},
    {true, R"tc(Cast(Int Constant(ConstLong(65536) None) None))tc", false, R"tc(0 Int())tc"},
    {true, R"tc(Cast(Int Constant(ConstLong(256) None) None))tc", false, R"tc(256 Int())tc"},
    {true, R"tc(Cast(Pointer(Int) Constant(ConstInt(0) None) None))tc", true, R"tc(integer constant expression cannot have a non-integer cast target (Pointer(referenced_type=Int())); §6.6.6 requires integer type)tc"},
    {true, R"tc(Cast(UInt Constant(ConstLong(255) None) None))tc", false, R"tc(255 UInt())tc"},
    {true, R"tc(Constant(ConstInt(42) None))tc", false, R"tc(42 Int())tc"},
    {true, R"tc(Constant(ConstLong(1000) None))tc", false, R"tc(1000 Long())tc"},
    {true, R"tc(Constant(ConstUInt(200) None))tc", false, R"tc(200 UInt())tc"},
    {true, R"tc(Constant(ConstFloat(1065353216) None))tc", true, R"tc(floating constant ConstFloat(bits=1065353216) is not an integer constant)tc"},
    {true, R"tc(Cast(Long Cast(Int Constant(ConstLong(5) None) None) None))tc", false, R"tc(5 Long())tc"},
    {true, R"tc(SizeOfExp(Var("x" Long) None))tc", false, R"tc(4 UInt())tc"},
    {true, R"tc(SizeOfExp(Var("x" None) None))tc", true, R"tc(sizeof's inner expression has no data_type — the constant evaluator requires the operand to be type-checked first)tc"},
    {true, R"tc(SizeOfType(Array(Int 10) None))tc", false, R"tc(20 UInt())tc"},
    {true, R"tc(SizeOfType(Array(Array(Long 5) 3) None))tc", false, R"tc(60 UInt())tc"},
    {true, R"tc(SizeOfType(Double None))tc", false, R"tc(8 UInt())tc"},
    {true, R"tc(SizeOfType(Int None))tc", false, R"tc(2 UInt())tc"},
    {true, R"tc(SizeOfType(LongLong None))tc", false, R"tc(8 UInt())tc"},
    {true, R"tc(SizeOfType(Long None))tc", false, R"tc(4 UInt())tc"},
    {true, R"tc(SizeOfType(Pointer(Int) None))tc", false, R"tc(2 UInt())tc"},
    {true, R"tc(SizeOfType(Void None))tc", true, R"tc(sizeof of incomplete or function type: Void())tc"},
    {true, R"tc(Var("x" None))tc", true, R"tc(expression is not an integer constant expression: Var(name='x', data_type=None))tc"},
    {true, R"tc(Binary(Add Binary(LeftShift Constant(ConstInt(1) Int) Constant(ConstInt(8) Int) Int) Constant(ConstInt(44) Int) Int))tc", false, R"tc(300 Int())tc"},
    {true, R"tc(Unary(Complement Constant(ConstUInt(0) UInt) UInt))tc", false, R"tc(65535 UInt())tc"},
    {true, R"tc(Conditional(Binary(GreaterThan Constant(ConstInt(3) Int) Constant(ConstInt(2) Int) Int) Constant(ConstInt(10) Int) Constant(ConstInt(20) Int) Int))tc", false, R"tc(10 Int())tc"},
    {true, R"tc(Binary(Divide Constant(ConstInt(1) Int) Constant(ConstInt(0) Int) Int))tc", true, R"tc(division by zero in constant expression)tc"},
    {true, R"tc(Binary(Divide Unary(Negate Constant(ConstInt(7) Int) Int) Constant(ConstInt(2) Int) Int))tc", false, R"tc(-3 Int())tc"},
    {true, R"tc(Binary(LogicalAnd Constant(ConstInt(0) Int) Binary(Divide Constant(ConstInt(1) Int) Constant(ConstInt(0) Int) Int) Int))tc", false, R"tc(0 Int())tc"},
    {true, R"tc(Unary(Negate Constant(ConstInt(1000) Int) Int))tc", false, R"tc(-1000 Int())tc"},
    {true, R"tc(Binary(Add Constant(ConstInt(32767) Int) Constant(ConstInt(1) Int) Int))tc", true, R"tc(signed overflow in constant expression (32768 does not fit in Int()))tc"},
    {false, R"tc(Assignment(Var("x" None) Constant(ConstInt(1) None) None))tc", true, R"tc(assignment is not allowed in a constant expression)tc"},
    {false, R"tc(Binary(Add Constant(ConstInt(1) None) Constant(ConstInt(2) None) None))tc", false, R"tc(None)tc"},
    {false, R"tc(Constant(ConstInt(5) None))tc", false, R"tc(None)tc"},
    {false, R"tc(FunctionCall("f" [] None false))tc", true, R"tc(function call is not allowed in a constant expression)tc"},
    {false, R"tc(Postfix(Increment Var("x" None) None))tc", true, R"tc(postfix increment/decrement is not allowed in a constant expression)tc"},
    {false, R"tc(SizeOfType(Int None))tc", false, R"tc(None)tc"},
    {false, R"tc(SizeOfExp(FunctionCall("foo" [] None false) None))tc", false, R"tc(None)tc"},
    {false, R"tc(SizeOfExp(Postfix(Increment Var("i" None) None) None))tc", false, R"tc(None)tc"},
    {false, R"tc(Unary(Negate Constant(ConstInt(5) None) None))tc", false, R"tc(None)tc"},
};

}  // namespace

TEST_CASE("constant_expression gives Python's results on the unit tests' calls") {
  using namespace c6502;
  for (const Case& c : kCases) {
    CAPTURE(c.input);
    const auto exp = asdl::load<c99_ast::Type_exp>(c.input);
    std::string got;
    bool raised = false;
    try {
      if (c.evaluate) {
        const auto [value, type] = ce::evaluate_integer_constant_expression(exp);
        got = asdl::int_to_string(value) + " " + asdl::py_repr(type);
      } else {
        ce::validate_constant_expression(exp);
        got = "None";
      }
    } catch (const ce::ConstantExpressionError& e) {
      raised = true;
      got = e.what();
    }
    CHECK(raised == c.error);
    CHECK(got == c.expected);
  }
}

TEST_CASE("sizeof of a string literal counts its code points and the null") {
  // Python's type checker gives a string literal it checks a
  // `data_type` of `char[N+1]`, an attribute the C++ `String` can't
  // hold, so the evaluator computes the size. The string's text holds
  // the code point U+00E9 in two bytes of UTF-8, which count as one.
  using namespace c6502;
  const auto exp = asdl::load<c99_ast::Type_exp>(R"(SizeOfExp(String("ab\xe9") UInt))");
  const auto [value, type] = ce::evaluate_integer_constant_expression(exp);
  CHECK(value == 4);
  CHECK(asdl::py_repr(type) == "UInt()");
}
