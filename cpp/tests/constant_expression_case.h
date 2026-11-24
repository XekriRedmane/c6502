// The check of a recorded call of constant_expression.py's
// `evaluate_integer_constant_expression` or
// `validate_constant_expression`, which tools/record_test_calls.py
// records as a `const_expr` record while a Python unit test runs
// (docs/cpp_port.md, step 8, group G8).
#pragma once

#include <string>
#include <string_view>

#include "asdl/runtime.h"
#include "c99_ast.h"
#include "doctest.h"
#include "json.h"
#include "passes/constant_expression.h"

namespace c6502::testing {

// Runs the C++ function `name` on the expression whose dump is `input`
// and checks that it gives `expected`: the value and the `repr` of its
// type, as `<value> <repr>`, or `None` for the validator, or, when
// `is_error` is set, the ConstantExpressionError whose
// `{"type": ..., "message": ...}` JSON `expected` holds.
inline void check_constant_expression(std::string_view name, const std::string& input,
                                      const std::string& expected, bool is_error) {
  namespace ce = passes::constant_expression;
  INFO(name, " of ", input);
  const auto exp = asdl::load<c99_ast::Type_exp>(input);
  std::string got;
  std::string type;
  bool raised = false;
  try {
    if (name == "evaluate_integer_constant_expression") {
      const auto [value, data_type] = ce::evaluate_integer_constant_expression(exp);
      got = asdl::int_to_string(value) + " " + asdl::py_repr(data_type);
    } else {
      REQUIRE(name == "validate_constant_expression");
      ce::validate_constant_expression(exp);
      got = "None";
    }
  } catch (const ce::ConstantExpressionError& e) {
    raised = true;
    type = "ConstantExpressionError";
    got = e.what();
  }
  CHECK(raised == is_error);
  if (!is_error) {
    CHECK(got == expected);
    return;
  }
  const json::Value want = json::parse(expected);
  CHECK(type == want.get("type")->string);
  CHECK(got == want.get("message")->string);
}

}  // namespace c6502::testing
