// The calls of the constant-expression evaluator that the
// Python unit tests make (tests/test_constant_expression.py), each
// recorded with Python's result: the expression's dump, and the value
// with the type's repr, or the message of the ConstantExpressionError.
#include <optional>
#include <string>
#include <utility>

#include "c99_ast.h"
#include "doctest.h"
#include "passes/constant_expression.h"

namespace {

namespace ce = ::c6502::passes::constant_expression;

struct Case {
  const char* input;
  bool error;
  const char* expected;
};

const Case kCases[] = {
    {R"tc(Binary(Add Constant(ConstInt(1) None) Constant(ConstInt(2) None) None))tc", true, R"tc(operator in a constant expression has no data_type — the constant evaluator requires the expression to be type-checked first)tc"},
    {R"tc(Cast(Float Constant(ConstInt(1) None) None))tc", true, R"tc(integer constant expression cannot have a non-integer cast target (Float()); §6.6.6 requires integer type)tc"},
    {R"tc(Cast(Int Constant(ConstLong(65535) None) None))tc", false, R"tc(-1 Int())tc"},
    {R"tc(Cast(Int Constant(ConstLong(65536) None) None))tc", false, R"tc(0 Int())tc"},
    {R"tc(Cast(Int Constant(ConstLong(256) None) None))tc", false, R"tc(256 Int())tc"},
    {R"tc(Cast(Pointer(Int) Constant(ConstInt(0) None) None))tc", true, R"tc(integer constant expression cannot have a non-integer cast target (Pointer(referenced_type=Int())); §6.6.6 requires integer type)tc"},
    {R"tc(Cast(UInt Constant(ConstLong(255) None) None))tc", false, R"tc(255 UInt())tc"},
    {R"tc(Constant(ConstInt(42) None))tc", false, R"tc(42 Int())tc"},
    {R"tc(Constant(ConstLong(1000) None))tc", false, R"tc(1000 Long())tc"},
    {R"tc(Constant(ConstUInt(200) None))tc", false, R"tc(200 UInt())tc"},
    {R"tc(Constant(ConstFloat(1065353216) None))tc", true, R"tc(floating constant ConstFloat(bits=1065353216) is not an integer constant)tc"},
    {R"tc(Cast(Long Cast(Int Constant(ConstLong(5) None) None) None))tc", false, R"tc(5 Long())tc"},
    {R"tc(SizeOfExp(Var("x" Long) None))tc", false, R"tc(4 UInt())tc"},
    {R"tc(SizeOfExp(Var("x" None) None))tc", true, R"tc(sizeof's inner expression has no data_type — the constant evaluator requires the operand to be type-checked first)tc"},
    {R"tc(SizeOfType(Array(Int 10) None))tc", false, R"tc(20 UInt())tc"},
    {R"tc(SizeOfType(Array(Array(Long 5) 3) None))tc", false, R"tc(60 UInt())tc"},
    {R"tc(SizeOfType(Double None))tc", false, R"tc(8 UInt())tc"},
    {R"tc(SizeOfType(Int None))tc", false, R"tc(2 UInt())tc"},
    {R"tc(SizeOfType(LongLong None))tc", false, R"tc(8 UInt())tc"},
    {R"tc(SizeOfType(Long None))tc", false, R"tc(4 UInt())tc"},
    {R"tc(SizeOfType(Pointer(Int) None))tc", false, R"tc(2 UInt())tc"},
    {R"tc(SizeOfType(Void None))tc", true, R"tc(sizeof of incomplete or function type: Void())tc"},
    {R"tc(Var("x" None))tc", true, R"tc(expression is not an integer constant expression: Var(name='x', data_type=None))tc"},
    {R"tc(Binary(Add Binary(LeftShift Constant(ConstInt(1) Int) Constant(ConstInt(8) Int) Int) Constant(ConstInt(44) Int) Int))tc", false, R"tc(300 Int())tc"},
    {R"tc(Unary(Complement Constant(ConstUInt(0) UInt) UInt))tc", false, R"tc(65535 UInt())tc"},
    {R"tc(Conditional(Binary(GreaterThan Constant(ConstInt(3) Int) Constant(ConstInt(2) Int) Int) Constant(ConstInt(10) Int) Constant(ConstInt(20) Int) Int))tc", false, R"tc(10 Int())tc"},
    {R"tc(Binary(Divide Constant(ConstInt(1) Int) Constant(ConstInt(0) Int) Int))tc", true, R"tc(division by zero in constant expression)tc"},
    {R"tc(Binary(Divide Unary(Negate Constant(ConstInt(7) Int) Int) Constant(ConstInt(2) Int) Int))tc", false, R"tc(-3 Int())tc"},
    {R"tc(Binary(LogicalAnd Constant(ConstInt(0) Int) Binary(Divide Constant(ConstInt(1) Int) Constant(ConstInt(0) Int) Int) Int))tc", false, R"tc(0 Int())tc"},
    {R"tc(Unary(Negate Constant(ConstInt(1000) Int) Int))tc", false, R"tc(-1000 Int())tc"},
    {R"tc(Binary(Add Constant(ConstInt(32767) Int) Constant(ConstInt(1) Int) Int))tc", true, R"tc(signed overflow in constant expression (32768 does not fit in Int()))tc"},
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
      const auto [value, type] = ce::evaluate_integer_constant_expression(exp);
      got = asdl::int_to_string(value) + " " + asdl::py_repr(type);
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

TEST_CASE("a right shift of a negative value is arithmetic") {
  using namespace c6502;
  struct Shift {
    const char* exp;
    asdl::Int value;
  };
  const Shift shifts[] = {
      {R"(Binary(RightShift Unary(Negate Constant(ConstInt(7) Int) Int) Constant(ConstInt(1) Int) Int))", -4},
      {R"(Binary(RightShift Unary(Negate Constant(ConstInt(1) Int) Int) Constant(ConstInt(15) Int) Int))", -1},
      {R"(Binary(RightShift Unary(Negate Constant(ConstInt(8) Int) Int) Constant(ConstInt(0) Int) Int))", -8},
      {R"(Binary(RightShift Unary(Negate Constant(ConstLong(2147483647) Long) Long) Constant(ConstInt(30) Int) Long))", -2},
      {R"(Binary(RightShift Constant(ConstUInt(65535) UInt) Constant(ConstInt(4) Int) UInt))", 4095},
  };
  for (const Shift& s : shifts) {
    CAPTURE(s.exp);
    CHECK(ce::evaluate_integer_constant_expression(asdl::load<c99_ast::Type_exp>(s.exp)).first ==
          s.value);
  }
}

TEST_CASE("an error names a named constant as the program spells it") {
  using namespace c6502;
  const ce::NamedConstants scope(
      [](const c99_ast::Type_exp&) -> std::optional<ce::NamedConstant> {
        return ce::NamedConstant{.type = c99_ast::Double{}, .value = {}};
      });
  const std::pair<const char*, const char*> cases[] = {
      {R"(Var("@3.d" Double))", "named constant `d` doesn't"},
      {R"(Var("g" Double))", "named constant `g` doesn't"},
      {R"(Dot(Var("@4.s" None) "m" Double))", "named constant `s.m` doesn't"},
  };
  for (const auto& [input, want] : cases) {
    CAPTURE(input);
    std::string got;
    try {
      ce::evaluate_integer_constant_expression(asdl::load<c99_ast::Type_exp>(input));
    } catch (const ce::ConstantExpressionError& e) {
      got = e.what();
    }
    CHECK(got.starts_with(want));
  }
}
