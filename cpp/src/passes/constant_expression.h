// The validator and integer folder of C99 §6.6's constant expressions.
// The evaluator reads the types the type checker stamped, so a caller
// type-checks an expression first.
#pragma once

#include <functional>
#include <optional>
#include <string>
#include <utility>

#include "asdl/runtime.h"
#include "c99_ast.h"
#include "errors.h"
#include "passes/type_checking.h"

namespace c6502::passes::constant_expression {

// Raised when an expression that has to be a constant expression (C99
// §6.6) doesn't meet that section's constraints. Its `python_type()` is
// "ConstantExpressionError", the class errors.py raises it as.
class ConstantExpressionError : public CompileError {
 public:
  explicit ConstantExpressionError(const std::string& message)
      : CompileError("ConstantExpressionError", message) {}
};

// The type and value of a named constant (C23 §6.6p7) or a compound
// literal constant (§6.6p6). The value has the form of a static
// initializer's (`Initial.value`): an integer for an integer type, the
// IEEE 754 bits for `float` and `double`, 0 for a null pointer, and a
// tuple of the elements or members for an aggregate.
struct NamedConstant {
  c99_ast::Type_data_type type;
  ValueTree value;
};

// A resolver takes a `Var`, a `Dot`, or a `CompoundLiteral` and returns
// its type and value, or none when it isn't a named constant.
using NamedConstantResolver =
    std::function<std::optional<NamedConstant>(const c99_ast::Type_exp&)>;

// Makes `resolver` give the values of named constants for as long as the
// object lives. The stack of resolvers is kept per thread, and the
// innermost one is used.
class NamedConstants {
 public:
  explicit NamedConstants(NamedConstantResolver resolver);
  ~NamedConstants();
  NamedConstants(const NamedConstants&) = delete;
  NamedConstants& operator=(const NamedConstants&) = delete;
};

// Returns the type and value of `exp` when it is a named constant, a
// compound literal constant, or a `.` member of one, from the innermost
// resolver, or none when no resolver is installed or `exp` is none of
// those.
std::optional<NamedConstant> named_constant(const c99_ast::Type_exp& exp);

// Returns the value and the integer type of the integer constant
// expression `exp` (C99 §6.6p6), or throws ConstantExpressionError. The
// value is in the range of its type, negative for a negative value of a
// signed type. Division by zero, a shift count out of range, a left
// shift of a negative value, and signed overflow are undefined, so they
// are reported as not being constant. The type has no top-level
// qualifiers.
std::pair<asdl::Int, c99_ast::Type_data_type> evaluate_integer_constant_expression(
    const c99_ast::Type_exp& exp);

// Throws ConstantExpressionError when `exp` contains an operator that
// C99 §6.6p3 forbids in a constant expression (assignment, increment,
// decrement, or a call other than of a builtin) outside an operand of
// `sizeof`.
void validate_constant_expression(const c99_ast::Type_exp& exp);

}  // namespace c6502::passes::constant_expression
