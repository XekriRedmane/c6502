// The port of passes/constant_expression.py, the constant-expression
// validator and integer folder of C99 §6.6.
//
// C99 §6.6 defines two flavors of constant expression. A constant
// expression (§6.6p3) "shall not contain assignment, increment,
// decrement, function-call, or comma operators, except when they are
// contained within a subexpression that is not evaluated", which the
// integer, floating, and address constant expressions all share. An
// integer constant expression (§6.6p6) is the strictest form, which
// "shall have integer type and shall only have operands that are
// integer constants, enumeration constants, character constants, sizeof
// expressions whose results are integer constants, _Alignof
// expressions, and floating constants that are the immediate operands
// of casts". Case labels, enumeration constants, array sizes, and the
// `#if` directives take one.
//
// `evaluate_integer_constant_expression` serves the §6.6p6 sites and
// returns the value with its integer type. `validate_constant_expression`
// checks §6.6p3 without folding, and nothing calls it yet.
//
// The integer folder accepts integer and character constants, integer
// casts, `sizeof`, the unary operators `-`, `~`, `!`, and `+`, every
// binary operator other than assignment, `?:`, and the builtins whose
// operands are constant. It relies on the type checker's `data_type`
// stamps and implicit casts, so each operator computes in its own result
// type, and its operands already have that width and signedness.
// Floating operands are rejected, except as the operand of a cast to an
// integer type (C23 §6.6p8). A static initializer handles its floating
// constants in the type checker and calls this folder for its integer
// operator expressions.
//
// A cast to an integer type reduces the value modulo the target's width
// and reads it with the target's signedness, the rule of the casts that
// c99_to_tac lowers. `sizeof` folds to the size of its operand's type as
// an `unsigned int`, c6502's `size_t`. For `sizeof e` the type is the
// one the type checker stamped on `e`, which isn't evaluated (C99
// §6.5.3.4p2), so the check of §6.6p3 doesn't look inside it either.
//
// Python's ints have no bound. Every value here is in the range of a
// type of at most 64 bits, so a sum or a difference fits in `asdl::Int`,
// and so does a product or a left shift of signed operands. A product
// or a left shift of unsigned operands can reach 2^128, and only its low
// bits survive the reduction to the type, so it is computed modulo 2^64.
#include "passes/constant_expression.h"

#include <cstdint>
#include <vector>

#include "bit_precise.h"
#include "builtin_functions.h"
#include "fp_arith.h"
#include "passes/type_checker.h"
#include "pyrepr.h"

namespace c6502::passes::constant_expression {

namespace {

namespace ir = ::c6502::c99_ast;
using Type = ir::Type_data_type;
using Exp = ir::Type_exp;
using Int = asdl::Int;
using K = ir::Kind_data_type;

std::vector<NamedConstantResolver>& resolvers() {
  thread_local std::vector<NamedConstantResolver> stack;
  return stack;
}

const Type& strip_quals(const Type& t) {
  const Type* p = &t;
  while (true) {
    if (const auto* c = std::get_if<ir::Const>(p)) {
      p = &*c->referenced_type;
    } else if (const auto* v = std::get_if<ir::Volatile>(p)) {
      p = &*v->referenced_type;
    } else {
      return *p;
    }
  }
}

// Returns the width in bytes of the integer types other than the
// bit-precise ones, or 0 for any other type. Python keeps its own table
// here so that the module imports nothing from the type checker.
int int_width(const Type& t) {
  switch (ir::kind(t)) {
    case K::Char:
    case K::SChar:
    case K::UChar:
    case K::Bool:
      return 1;
    case K::Int:
    case K::UInt:
    case K::Short:
    case K::UShort:
      return 2;
    case K::Long:
    case K::ULong:
      return 4;
    case K::LongLong:
    case K::ULongLong:
      return 8;
    default:
      return 0;
  }
}

bool is_integer_type(const Type& t) {
  const Type& s = strip_quals(t);
  return int_width(s) != 0 || asdl::isa<ir::BitInt>(s);
}

// Returns the width of the integer type `t` in bits: N for `_BitInt(N)`,
// and 8 bits a byte for the others.
int width_bits(const Type& t) {
  const Type& s = strip_quals(t);
  if (const auto* b = std::get_if<ir::BitInt>(&s)) {
    return static_cast<int>(b->width);
  }
  const int w = int_width(s);
  if (w == 0) {
    throw InternalError("KeyError: " + asdl::py_repr(s));
  }
  return 8 * w;
}

// Plain `char` is unsigned in c6502 (C99 §6.2.5p15 leaves the choice to
// the implementation), so only `signed char` is a signed character type.
bool is_signed_type(const Type& t) {
  const Type& s = strip_quals(t);
  if (const auto* b = std::get_if<ir::BitInt>(&s)) {
    return b->is_signed;
  }
  return asdl::isa<ir::Int, ir::Long, ir::LongLong, ir::SChar, ir::Short>(s);
}

// Returns the bytes a value of type `t` occupies. Python keeps a copy of
// the type checker's `_sizeof` here, for the same reason as the width
// table, and it throws ConstantExpressionError for an incomplete or
// function type, which the type checker flags before this runs.
Int sizeof_type(const Type& t) {
  const Type& s = strip_quals(t);
  switch (ir::kind(s)) {
    case K::Char:
    case K::SChar:
    case K::UChar:
    case K::Bool:
      return 1;
    // `nullptr_t` is as wide as a pointer to a character type (C23
    // §7.21.2p3).
    case K::Int:
    case K::UInt:
    case K::Short:
    case K::UShort:
    case K::Pointer:
    case K::NullptrT:
      return 2;
    case K::Long:
    case K::ULong:
    case K::Float:
      return 4;
    case K::LongLong:
    case K::ULongLong:
    case K::Double:
      return 8;
    case K::BitInt:
      return bit_precise::storage_bytes(std::get<ir::BitInt>(s).width);
    case K::Array: {
      const auto& a = std::get<ir::Array>(s);
      return sizeof_type(*a.element_type) * a.size;
    }
    default:
      throw ConstantExpressionError("sizeof of incomplete or function type: " +
                                    asdl::py_repr(s));
  }
}

// Reduces `value` modulo 2^width and reads it with the target's
// signedness, which is what the byte-level casts tac_to_asm lowers
// compute for a known value. A `bool` target gets 0 or 1 instead (C99
// §6.3.1.2).
Int coerce_to_integer_type(Int value, const Type& t) {
  const Type& s = strip_quals(t);
  if (asdl::isa<ir::Bool>(s)) {
    return value != 0 ? 1 : 0;
  }
  const int bits = width_bits(s);
  const Int modulus = Int(1) << bits;
  Int raw = value & (modulus - 1);
  if (is_signed_type(s) && (raw & (Int(1) << (bits - 1))) != 0) {
    raw -= modulus;
  }
  return raw;
}

// Returns the integer type of the constant `c`.
Type const_type(const ir::Type_const& c) {
  switch (ir::kind(c)) {
    case ir::Kind_const::ConstInt:
      return ir::Int{};
    case ir::Kind_const::ConstLong:
      return ir::Long{};
    case ir::Kind_const::ConstLongLong:
      return ir::LongLong{};
    case ir::Kind_const::ConstUInt:
      return ir::UInt{};
    case ir::Kind_const::ConstULong:
      return ir::ULong{};
    case ir::Kind_const::ConstULongLong:
      return ir::ULongLong{};
    case ir::Kind_const::ConstChar:
      return ir::SChar{};
    case ir::Kind_const::ConstUChar:
      return ir::UChar{};
    case ir::Kind_const::ConstBool:
      return ir::Bool{};
    case ir::Kind_const::ConstBitInt: {
      const auto& b = std::get<ir::ConstBitInt>(c);
      return ir::BitInt{.width = b.width, .is_signed = b.is_signed};
    }
    default:
      throw ConstantExpressionError("non-integer constant " + asdl::py_repr(c) +
                                    " has no integer type");
  }
}

// Returns the value of an integer constant. Each integer variant holds
// the bit pattern as a nonnegative int, and its variant gives the
// signedness.
Int const_int_value(const ir::Type_const& c) {
  switch (ir::kind(c)) {
    case ir::Kind_const::ConstNullptr:
      // C23 §6.6p8 admits only operands of integer type, or of
      // arithmetic type under a cast, and `nullptr` has neither.
      throw ConstantExpressionError(
          "`nullptr` has type nullptr_t, which isn't an integer type, so it can't be an "
          "operand of an integer constant expression (C23 §6.6p8)");
    case ir::Kind_const::ConstFloat:
    case ir::Kind_const::ConstDouble:
      throw ConstantExpressionError("floating constant " + asdl::py_repr(c) +
                                    " is not an integer constant");
    default:
      break;
  }
  Int value = 0;
  switch (ir::kind(c)) {
    case ir::Kind_const::ConstInt:
      value = std::get<ir::ConstInt>(c).value;
      break;
    case ir::Kind_const::ConstLong:
      value = std::get<ir::ConstLong>(c).value;
      break;
    case ir::Kind_const::ConstLongLong:
      value = std::get<ir::ConstLongLong>(c).value;
      break;
    case ir::Kind_const::ConstUInt:
      value = std::get<ir::ConstUInt>(c).value;
      break;
    case ir::Kind_const::ConstULong:
      value = std::get<ir::ConstULong>(c).value;
      break;
    case ir::Kind_const::ConstULongLong:
      value = std::get<ir::ConstULongLong>(c).value;
      break;
    case ir::Kind_const::ConstChar:
      value = std::get<ir::ConstChar>(c).value;
      break;
    case ir::Kind_const::ConstUChar:
      value = std::get<ir::ConstUChar>(c).value;
      break;
    case ir::Kind_const::ConstBool:
      value = std::get<ir::ConstBool>(c).value;
      break;
    case ir::Kind_const::ConstBitInt:
      value = std::get<ir::ConstBitInt>(c).value;
      break;
    default:
      break;
  }
  return coerce_to_integer_type(value, const_type(c));
}

// Returns `exp`, a `Var`, a compound literal, or a chain of `Dot`s on
// one, as source text.
std::string describe(const Exp& exp) {
  if (const auto* d = std::get_if<ir::Dot>(&exp)) {
    return describe(*d->operand) + "." + d->member;
  }
  if (asdl::isa<ir::CompoundLiteral>(exp)) {
    return "(a compound literal)";
  }
  std::string name;
  if (const auto* v = std::get_if<ir::Var>(&exp)) {
    name = v->name;
  }
  // identifier_resolution names a block-scope object `@N.name`.
  if (!name.empty() && name[0] == '@') {
    const std::size_t dot = name.find('.');
    if (dot != std::string::npos) {
      name = name.substr(dot + 1);
    }
  }
  return "`" + name + "`";
}

struct FloatingOperand {
  Type type;
  Int bits = 0;
};

// Returns the type and bits of `exp` when it is a floating constant or
// a named constant of floating type, the operands C23 §6.6p8 lets a
// cast in an integer constant expression convert.
std::optional<FloatingOperand> floating_operand(const Exp& exp) {
  if (const auto* c = std::get_if<ir::Constant>(&exp)) {
    if (const auto* f = std::get_if<ir::ConstFloat>(&c->const_)) {
      return FloatingOperand{ir::Float{}, f->bits};
    }
    if (const auto* d = std::get_if<ir::ConstDouble>(&c->const_)) {
      return FloatingOperand{ir::Double{}, d->bits};
    }
  }
  const auto named = named_constant(exp);
  if (named && asdl::isa<ir::Float, ir::Double>(strip_quals(named->type))) {
    return FloatingOperand{strip_quals(named->type), named->value.integer};
  }
  return std::nullopt;
}

// Returns the floating value with IEEE 754 `bits` of type `t` converted
// to the integer type `target`, truncated toward zero. A value whose
// integer part `target` can't hold makes the conversion undefined (C99
// §6.3.1.4p1), so it isn't a constant. A `bool` target is 0 for a zero
// of either sign and 1 for anything else (C99 §6.3.1.2).
Int floating_to_integer(const Type& t, Int bits, const Type& target) {
  const bool single = asdl::isa<ir::Float>(t);
  if (asdl::isa<ir::Bool>(strip_quals(target))) {
    const Int magnitude = single ? Int(0x7FFFFFFF) : (Int(1) << 63) - 1;
    return (bits & magnitude) != 0 ? 1 : 0;
  }
  const auto truncated = single ? fp_arith::single_bits_to_int(bits)
                                : fp_arith::double_bits_to_int(bits);
  std::optional<Int> value;
  if (truncated) {
    value = truncated->exact();
  }
  if (!value || coerce_to_integer_type(*value, target) != *value) {
    throw ConstantExpressionError("the floating value converted to " +
                                  asdl::py_repr(strip_quals(target)) +
                                  " is out of its range, which is undefined (C99 §6.3.1.4)");
  }
  return *value;
}

// Returns the integer type the type checker stamped on an operator, `t`,
// or throws when it is missing or isn't an integer type.
Type result_type(const std::optional<Type>& t) {
  if (!t) {
    throw ConstantExpressionError(
        "operator in a constant expression has no data_type — the constant evaluator "
        "requires the expression to be type-checked first");
  }
  if (!is_integer_type(*t)) {
    throw ConstantExpressionError("integer constant expression has non-integer type " +
                                  asdl::py_repr(*t));
  }
  return strip_quals(*t);
}

// Returns `value` as a value of type `t`. Unsigned arithmetic wraps
// (C99 §6.2.5p9). A signed result outside `t`'s range is overflow, which
// is undefined, so it is rejected.
Int checked(Int value, const Type& t) {
  const Int wrapped = coerce_to_integer_type(value, t);
  if (is_signed_type(t) && wrapped != value) {
    throw ConstantExpressionError("signed overflow in constant expression (" +
                                  asdl::int_to_string(value) + " does not fit in " +
                                  asdl::py_repr(t) + ")");
  }
  return wrapped;
}

// Returns the low 64 bits of an unsigned operand's product or left
// shift, which is all of it that its reduction to a type of at most 64
// bits keeps.
Int low64(std::uint64_t v) { return Int(v); }

using Result = std::pair<Int, Type>;

Result evaluate_binary(const ir::Binary& exp) {
  const Type t = result_type(exp.data_type);
  const Int a = evaluate_integer_constant_expression(*exp.left).first;
  using O = ir::Kind_binary_operator;
  const O op = ir::kind(exp.op);
  // `&&` and `||` don't evaluate the right operand when the left one
  // decides the result (C99 §6.5.13p4, §6.5.14p4).
  if (op == O::LogicalAnd) {
    if (a == 0) {
      return {0, t};
    }
    const Int b = evaluate_integer_constant_expression(*exp.right).first;
    return {b != 0 ? 1 : 0, t};
  }
  if (op == O::LogicalOr) {
    if (a != 0) {
      return {1, t};
    }
    const Int b = evaluate_integer_constant_expression(*exp.right).first;
    return {b != 0 ? 1 : 0, t};
  }
  const Int b = evaluate_integer_constant_expression(*exp.right).first;
  const bool is_unsigned = !is_signed_type(t);
  switch (op) {
    case O::Equal:
      return {a == b ? 1 : 0, t};
    case O::NotEqual:
      return {a != b ? 1 : 0, t};
    case O::LessThan:
      return {a < b ? 1 : 0, t};
    case O::GreaterThan:
      return {a > b ? 1 : 0, t};
    case O::LessOrEqual:
      return {a <= b ? 1 : 0, t};
    case O::GreaterOrEqual:
      return {a >= b ? 1 : 0, t};
    case O::LeftShift:
    case O::RightShift: {
      if (b < 0 || b >= width_bits(t)) {
        throw ConstantExpressionError("shift count " + asdl::int_to_string(b) +
                                      " out of range in constant expression");
      }
      const int n = static_cast<int>(b);
      if (op == O::RightShift) {
        // A negative left operand shifts arithmetically, which is
        // c6502's implementation-defined choice (asr*). Python's `>>`
        // rounds toward negative infinity, as floor division does.
        return {coerce_to_integer_type(asdl::floor_div(a, Int(1) << n), t), t};
      }
      if (!is_unsigned && a < 0) {
        throw ConstantExpressionError("left shift of a negative value in constant expression");
      }
      if (is_unsigned) {
        return {checked(low64(static_cast<std::uint64_t>(a) << n), t), t};
      }
      return {checked(a << n, t), t};
    }
    case O::Divide:
    case O::Modulo: {
      if (b == 0) {
        throw ConstantExpressionError("division by zero in constant expression");
      }
      // C99 §6.5.5p6: the quotient truncates toward zero.
      Int q = (a < 0 ? -a : a) / (b < 0 ? -b : b);
      if ((a < 0) != (b < 0)) {
        q = -q;
      }
      const Int r = op == O::Divide ? q : a - q * b;
      return {checked(r, t), t};
    }
    case O::Add:
      return {checked(a + b, t), t};
    case O::Subtract:
      return {checked(a - b, t), t};
    case O::Multiply:
      if (is_unsigned) {
        return {checked(low64(static_cast<std::uint64_t>(a) * static_cast<std::uint64_t>(b)), t),
                t};
      }
      return {checked(a * b, t), t};
    case O::BitwiseAnd:
      return {coerce_to_integer_type(a & b, t), t};
    case O::BitwiseOr:
      return {coerce_to_integer_type(a | b, t), t};
    case O::BitwiseXor:
      return {coerce_to_integer_type(a ^ b, t), t};
    default:
      throw ConstantExpressionError("operator " + asdl::py_repr(exp.op) +
                                    " is not allowed in an integer constant expression");
  }
}

[[noreturn]] void not_constant(const Exp& exp) {
  throw ConstantExpressionError("expression is not an integer constant expression: " +
                                asdl::py_repr(exp));
}

}  // namespace

NamedConstants::NamedConstants(NamedConstantResolver resolver) {
  resolvers().push_back(std::move(resolver));
}

NamedConstants::~NamedConstants() { resolvers().pop_back(); }

std::optional<NamedConstant> named_constant(const Exp& exp) {
  if (resolvers().empty() || !asdl::isa<ir::Var, ir::Dot, ir::CompoundLiteral>(exp)) {
    return std::nullopt;
  }
  // The resolver is copied first, so that one which installs another
  // while it runs can't invalidate it.
  const NamedConstantResolver resolver = resolvers().back();
  return resolver(exp);
}

std::pair<Int, Type> evaluate_integer_constant_expression(const Exp& exp) {
  using E = ir::Kind_exp;
  switch (ir::kind(exp)) {
    case E::Constant: {
      const auto& c = std::get<ir::Constant>(exp).const_;
      return {const_int_value(c), const_type(c)};
    }
    case E::Var:
    case E::Dot:
    case E::CompoundLiteral: {
      const auto named = named_constant(exp);
      if (!named) {
        break;
      }
      if (!is_integer_type(named->type)) {
        throw ConstantExpressionError(
            "named constant " + describe(exp) +
            " doesn't have an integer type, so it can be in an integer constant expression "
            "only as the operand of a cast to one (C23 §6.6p8)");
      }
      return {coerce_to_integer_type(named->value.integer, named->type),
              strip_quals(named->type)};
    }
    case E::Cast: {
      const auto& c = std::get<ir::Cast>(exp);
      if (!is_integer_type(c.target_type)) {
        throw ConstantExpressionError(
            "integer constant expression cannot have a non-integer cast target (" +
            asdl::py_repr(c.target_type) + "); §6.6.6 requires integer type");
      }
      if (const auto floating = floating_operand(*c.exp)) {
        return {floating_to_integer(floating->type, floating->bits, c.target_type),
                strip_quals(c.target_type)};
      }
      const Int inner = evaluate_integer_constant_expression(*c.exp).first;
      return {coerce_to_integer_type(inner, c.target_type), strip_quals(c.target_type)};
    }
    case E::SizeOfType:
      return {sizeof_type(std::get<ir::SizeOfType>(exp).target_type), ir::UInt{}};
    case E::SizeOfExp: {
      const auto& inner = *std::get<ir::SizeOfExp>(exp).exp;
      const std::optional<Type>& t = type_checking::data_type_of(inner);
      if (!t) {
        throw ConstantExpressionError(
            "sizeof's inner expression has no data_type — the constant evaluator requires "
            "the operand to be type-checked first");
      }
      return {sizeof_type(*t), ir::UInt{}};
    }
    case E::Unary: {
      const auto& u = std::get<ir::Unary>(exp);
      const Type t = result_type(type_checking::data_type_of(exp));
      const Int v = evaluate_integer_constant_expression(*u.exp).first;
      switch (ir::kind(u.op)) {
        case ir::Kind_unary_operator::LogicalNot:
          return {v == 0 ? 1 : 0, t};
        case ir::Kind_unary_operator::Negate:
          return {checked(-v, t), t};
        case ir::Kind_unary_operator::Plus:
          return {coerce_to_integer_type(v, t), t};
        default:
          return {coerce_to_integer_type(~v, t), t};
      }
    }
    case E::Binary:
      return evaluate_binary(std::get<ir::Binary>(exp));
    case E::Conditional: {
      const auto& c = std::get<ir::Conditional>(exp);
      const Type t = result_type(type_checking::data_type_of(exp));
      const Int cond = evaluate_integer_constant_expression(*c.condition).first;
      const Int v =
          evaluate_integer_constant_expression(cond != 0 ? *c.true_clause : *c.false_clause).first;
      return {coerce_to_integer_type(v, t), t};
    }
    case E::FunctionCall: {
      const auto& call = std::get<ir::FunctionCall>(exp);
      namespace bf = ::c6502::builtin_functions;
      if (call.name == bf::kBcdAdd || call.name == bf::kBcdSub) {
        // A packed BCD builtin of constant operands. It isn't a
        // function call, so it is a constant (docs/builtins.md).
        const Type t = result_type(type_checking::data_type_of(exp));
        if (call.args.size() < 2) {
          throw InternalError("IndexError: list index out of range");
        }
        const Int a = evaluate_integer_constant_expression(call.args[0]).first;
        const Int b = evaluate_integer_constant_expression(call.args[1]).first;
        const int n = static_cast<int>(sizeof_type(t));
        return {call.name == bf::kBcdAdd ? bf::bcd_add(a, b, n) : bf::bcd_sub(a, b, n), t};
      }
      if (call.args.size() != 1) {
        break;
      }
      if (call.name == bf::kBinToBcd) {
        // `__c6502_bin_to_bcd` of a constant, which the type checker
        // converted to `unsigned int`.
        const Type t = result_type(type_checking::data_type_of(exp));
        const Int v = evaluate_integer_constant_expression(call.args[0]).first;
        return {bf::bin_to_bcd(v), t};
      }
      if (const auto* builtin = bf::find_stdbit_builtin(call.name)) {
        // A <stdbit.h> builtin of a constant argument, which the type
        // checker converted to the value type (docs/stdbit.md).
        const Type t = result_type(type_checking::data_type_of(exp));
        const auto [v, value_t] = evaluate_integer_constant_expression(call.args[0]);
        return {bf::stdbit(builtin->operation, v, 8 * static_cast<int>(sizeof_type(value_t))), t};
      }
      break;
    }
    default:
      break;
  }
  not_constant(exp);
}

void validate_constant_expression(const Exp& exp) {
  using E = ir::Kind_exp;
  switch (ir::kind(exp)) {
    case E::Assignment:
      throw ConstantExpressionError("assignment is not allowed in a constant expression");
    case E::CompoundAssignment:
      throw ConstantExpressionError("compound assignment is not allowed in a constant expression");
    case E::Postfix:
      throw ConstantExpressionError(
          "postfix increment/decrement is not allowed in a constant expression");
    case E::FunctionCall: {
      const auto& call = std::get<ir::FunctionCall>(exp);
      if (builtin_functions::is_builtin(call.name)) {
        for (const Exp& arg : call.args) {
          validate_constant_expression(arg);
        }
        return;
      }
      throw ConstantExpressionError("function call is not allowed in a constant expression");
    }
    case E::IndirectCall:
      throw ConstantExpressionError("function call is not allowed in a constant expression");
    case E::Constant:
    case E::Var:
      return;
    // The operand of `sizeof` is "a subexpression that is not evaluated"
    // (C99 §6.5.3.4p2), so §6.6p3's rule doesn't apply inside it, and
    // `sizeof(i++)` is a constant expression.
    case E::SizeOfExp:
    case E::SizeOfType:
      return;
    case E::Cast:
      validate_constant_expression(*std::get<ir::Cast>(exp).exp);
      return;
    case E::Unary:
      validate_constant_expression(*std::get<ir::Unary>(exp).exp);
      return;
    case E::Dereference:
      validate_constant_expression(*std::get<ir::Dereference>(exp).exp);
      return;
    case E::AddressOf:
      validate_constant_expression(*std::get<ir::AddressOf>(exp).exp);
      return;
    case E::Binary: {
      const auto& b = std::get<ir::Binary>(exp);
      validate_constant_expression(*b.left);
      validate_constant_expression(*b.right);
      return;
    }
    case E::Conditional: {
      const auto& c = std::get<ir::Conditional>(exp);
      validate_constant_expression(*c.condition);
      validate_constant_expression(*c.true_clause);
      validate_constant_expression(*c.false_clause);
      return;
    }
    case E::Subscript: {
      const auto& s = std::get<ir::Subscript>(exp);
      validate_constant_expression(*s.array);
      validate_constant_expression(*s.index);
      return;
    }
    case E::InitList:
      for (const Exp& item : std::get<ir::InitList>(exp).items) {
        validate_constant_expression(item);
      }
      return;
    case E::Designation:
      validate_constant_expression(*std::get<ir::Designation>(exp).init);
      return;
    default:
      throw ConstantExpressionError(
          "unexpected expression in constant-expression validation: " + asdl::py_repr(exp));
  }
}

}  // namespace c6502::passes::constant_expression
