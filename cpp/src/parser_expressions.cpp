// Part R3 of the parser's descent (parser_descent.h): the expressions
// and the constants (parser.py:1679-1990, the member accesses of
// 3352-3363, and the expression actions of 4396-4673).
//
// Expressions are parsed by recursive descent on the grammar's levels,
// from `exp` (the comma operator) down to `atom`. Each binary level is
// left-associative, so its loop builds the node for `a op b` before it
// takes the next operator, which is when Lark reduces the rule, since
// Lark reduces `add_exp PLUS mul_exp` on seeing the token after the
// `mul_exp`.
#include <algorithm>
#include <string>
#include <utility>
#include <vector>

#include "fp_arith.h"
#include "parser_descent.h"
#include "pyrepr.h"

namespace c6502::parser {

namespace ir = c99_ast;

namespace {

// --- Constants ---

// The largest value of each integer type of C99 §6.4.4.1p5, whose
// literals are never negative, since a minus sign is an operator.
constexpr asdl::Int kIntMax = 32767;
constexpr asdl::Int kLongMax = 2147483647;
constexpr asdl::Int kLongLongMax = 9223372036854775807LL;
constexpr asdl::Int kUIntMax = 65535;
constexpr asdl::Int kULongMax = 4294967295LL;
const asdl::Int kULongLongMax = asdl::Int(18446744073709551615ULL);

enum class IntType { kInt, kLong, kLongLong, kUInt, kULong, kULongLong };

struct Candidate {
  asdl::Int max;
  IntType type;
};

// parser.py's `_CANDIDATES`: for each kind of integer token and base,
// the types C99 §6.4.4.1p5 tries in order, of which a constant takes
// the first whose range holds its value. "hex_oct" is hexadecimal,
// binary, and octal. The lexer doesn't split `LL` out from `L`, so a
// LONG_INTEGER or ULONG_INTEGER with `ll` in its suffix takes the list
// of the long long kind (`long_long` here).
std::vector<Candidate> candidates(std::string_view kind, bool hex_oct, bool long_long) {
  using T = IntType;
  if (kind == "INTEGER_CONSTANT") {
    if (!hex_oct) {
      return {{kIntMax, T::kInt}, {kLongMax, T::kLong}, {kLongLongMax, T::kLongLong}};
    }
    return {{kIntMax, T::kInt},         {kUIntMax, T::kUInt},
            {kLongMax, T::kLong},       {kULongMax, T::kULong},
            {kLongLongMax, T::kLongLong}, {kULongLongMax, T::kULongLong}};
  }
  if (kind == "LONG_INTEGER" && !long_long) {
    if (!hex_oct) {
      return {{kLongMax, T::kLong}, {kLongLongMax, T::kLongLong}};
    }
    return {{kLongMax, T::kLong},
            {kULongMax, T::kULong},
            {kLongLongMax, T::kLongLong},
            {kULongLongMax, T::kULongLong}};
  }
  if (kind == "LONG_INTEGER") {
    if (!hex_oct) {
      return {{kLongLongMax, T::kLongLong}};
    }
    return {{kLongLongMax, T::kLongLong}, {kULongLongMax, T::kULongLong}};
  }
  if (kind == "UINT_INTEGER") {
    return {{kUIntMax, T::kUInt}, {kULongMax, T::kULong}, {kULongLongMax, T::kULongLong}};
  }
  if (kind == "ULONG_INTEGER" && !long_long) {
    return {{kULongMax, T::kULong}, {kULongLongMax, T::kULongLong}};
  }
  if (kind == "ULONG_INTEGER") {
    return {{kULongLongMax, T::kULongLong}};
  }
  throw InternalError("parser: no candidate types for " + std::string(kind));
}

ir::Type_const int_const(IntType type, asdl::Int value) {
  switch (type) {
    case IntType::kInt:
      return ir::ConstInt{.value = value};
    case IntType::kLong:
      return ir::ConstLong{.value = value};
    case IntType::kLongLong:
      return ir::ConstLongLong{.value = value};
    case IntType::kUInt:
      return ir::ConstUInt{.value = value};
    case IntType::kULong:
      return ir::ConstULong{.value = value};
    case IntType::kULongLong:
      return ir::ConstULongLong{.value = value};
  }
  throw InternalError("parser: an integer type out of range");
}

// The values of the single-character escapes of C99 §6.4.4.4p4, or -1.
int simple_escape(char32_t c) {
  switch (c) {
    case U'\'':
      return 0x27;
    case U'"':
      return 0x22;
    case U'?':
      return 0x3F;
    case U'\\':
      return 0x5C;
    case U'a':
      return 0x07;
    case U'b':
      return 0x08;
    case U'f':
      return 0x0C;
    case U'n':
      return 0x0A;
    case U'r':
      return 0x0D;
    case U't':
      return 0x09;
    case U'v':
      return 0x0B;
    default:
      return -1;
  }
}

int hex_digit(char32_t c) {
  if (c >= U'0' && c <= U'9') {
    return static_cast<int>(c - U'0');
  }
  if (c >= U'a' && c <= U'f') {
    return static_cast<int>(c - U'a') + 10;
  }
  if (c >= U'A' && c <= U'F') {
    return static_cast<int>(c - U'A') + 10;
  }
  return -1;
}

// parser.py's `_decode_escapes`: the bytes that the body of a character
// constant or a string literal stands for, the text between its quotes.
// A plain character gives the low byte of its code point (the lexer
// takes only printable ASCII there), and the escapes are C99's: the
// simple ones, `\x` with one or more hex digits, and `\` with one to
// three octal digits, each reduced to its low 8 bits, since c6502 has
// no wide characters. `\u` and `\U` lex, and are rejected here.
std::vector<unsigned char> decode_escapes(std::string_view body_utf8) {
  const std::u32string body = lark::decode_utf8(body_utf8);
  std::vector<unsigned char> out;
  const std::size_t n = body.size();
  std::size_t i = 0;
  while (i < n) {
    const char32_t c = body[i];
    if (c != U'\\') {
      out.push_back(static_cast<unsigned char>(c & 0xFF));
      ++i;
      continue;
    }
    ++i;
    if (i >= n) {
      throw ParserError("trailing backslash in literal body " + pyrepr::string(body_utf8));
    }
    const char32_t e = body[i];
    if (const int v = simple_escape(e); v >= 0) {
      out.push_back(static_cast<unsigned char>(v));
      ++i;
      continue;
    }
    if (e == U'x') {
      ++i;
      std::size_t j = i;
      unsigned value = 0;
      while (j < n && hex_digit(body[j]) >= 0) {
        value = (value * 16 + static_cast<unsigned>(hex_digit(body[j]))) & 0xFF;
        ++j;
      }
      if (j == i) {
        throw ParserError("empty \\x escape in literal body " + pyrepr::string(body_utf8));
      }
      out.push_back(static_cast<unsigned char>(value));
      i = j;
      continue;
    }
    if (e >= U'0' && e <= U'7') {
      std::size_t j = i;
      unsigned value = 0;
      while (j < std::min(i + 3, n) && body[j] >= U'0' && body[j] <= U'7') {
        value = value * 8 + static_cast<unsigned>(body[j] - U'0');
        ++j;
      }
      out.push_back(static_cast<unsigned char>(value & 0xFF));
      i = j;
      continue;
    }
    if (e == U'u' || e == U'U') {
      throw ParserError(
          "Unicode escapes (\\u / \\U) are not supported (c6502 has no wide-char model)");
    }
    throw ParserError("unrecognized escape \\" + lark::encode_utf8(std::u32string(1, e)) +
                      " in literal");
  }
  return out;
}

// Returns the text of a literal between its quotes, after an optional
// `L` prefix, which c6502 accepts and treats as narrow.
std::string_view literal_body(std::string_view text) {
  if (!text.empty() && text.front() == 'L') {
    text.remove_prefix(1);
  }
  return text.substr(1, text.size() - 2);
}

// parser.py's `_decode_char_constant`: the value of a character
// constant, which is an `int` (C99 §6.4.4.4p10) and rides as a
// `ConstInt`. A constant of more than one character has an
// implementation-defined value, and c6502 rejects it rather than pick
// an encoding.
asdl::Int decode_char_constant(const lark::Token& token) {
  const std::vector<unsigned char> decoded = decode_escapes(literal_body(token.value));
  if (decoded.size() != 1) {
    throw ParserError("multi-character character constant " + token.value +
                      " is not supported");
  }
  return decoded[0];
}

}  // namespace

IntegerToken Descent::parse_integer_token(std::string_view text) {
  // The lexer has already checked the spelling (pp_numbers.py). Digit
  // separators (C23 §6.4.4.2p2) are ignored, and binary constants take
  // the type list of octal and hexadecimal ones.
  IntegerToken out;
  try {
    out.constant = pp_numbers::parse_integer(text);
  } catch (const pp_numbers::NumberError& e) {
    throw ParserError(e.what());
  }
  out.hex_oct = out.constant.base != 10;
  out.has_ll = out.constant.length == "ll";
  return out;
}

Exp Descent::constant_exp_of(const lark::Token& token) {
  // parser.py's `_const_for_token`, the action of `const`. An integer
  // literal takes the first type of its list whose range holds its
  // value (C99 §6.4.4.1p5), and a floating literal the type its suffix
  // names (§6.4.4.2). `true` and `false` are constants of type `bool`,
  // and `nullptr` a null pointer constant of type `nullptr_t` (C23
  // §6.4.4.6).
  ir::Type_const c = action("const", [&]() -> ir::Type_const {
    const std::string& text = token.value;
    const int k = token.type;
    if (k == t.TRUE || k == t.FALSE) {
      return ir::ConstBool{.value = k == t.TRUE ? 1 : 0};
    }
    if (k == t.NULLPTR) {
      return ir::ConstNullptr{};
    }
    if (k == t.CHAR_CONSTANT) {
      return ir::ConstInt{.value = decode_char_constant(token)};
    }
    if (k == t.DOUBLE_CONSTANT || k == t.FLOAT_CONSTANT) {
      // fp_arith knows neither the `f` suffix nor the digit separators.
      std::string body = k == t.FLOAT_CONSTANT ? text.substr(0, text.size() - 1) : text;
      std::erase(body, '\'');
      if (body.starts_with("0x") || body.starts_with("0X")) {
        // Hex floats lex, but the conversion of the C syntax isn't
        // written.
        throw ParserError("hex floating literal " + pyrepr::string(text) + " is not supported");
      }
      // The source string goes straight to an IEEE 754 pattern, so a
      // `float` is rounded once from the text, to nearest even.
      if (k == t.DOUBLE_CONSTANT) {
        return ir::ConstDouble{.bits = fp_arith::double_string_to_bits(body)};
      }
      return ir::ConstFloat{.bits = fp_arith::single_string_to_bits(body)};
    }
    if (k == t.LONG_DOUBLE_CONSTANT) {
      throw ParserError("`long double` is not supported (literal " + pyrepr::string(text) + ")");
    }
    if (k == t.BITINT_INTEGER || k == t.UBITINT_INTEGER) {
      // `wb` gives the `_BitInt(N)` and `uwb` the `unsigned _BitInt(N)`
      // of the least N that holds the value (C23 §6.4.4.2p6), whatever
      // the base.
      try {
        const pp_numbers::IntegerConstant constant = pp_numbers::parse_integer(text);
        const int width = pp_numbers::bit_precise_width(constant, text);
        return ir::ConstBitInt{
            .value = constant.value, .width = width, .is_signed = !constant.is_unsigned};
      } catch (const pp_numbers::NumberError& e) {
        throw ParserError(e.what());
      }
    }
    const IntegerToken it = parse_integer_token(text);
    for (const Candidate& c : candidates(table().name(k), it.hex_oct, it.has_ll)) {
      if (it.constant.fits() && it.constant.value <= c.max) {
        return int_const(c.type, it.constant.value);
      }
    }
    throw ParserError("integer constant " + pyrepr::string(text) +
                      " doesn't fit any supported type (value > 2^32 - 1)");
  });
  return ir::Constant{.const_ = std::move(c)};
}

Exp Descent::identifier_exp(const lark::Token& name) {
  return action("identifier", [&]() -> Exp {
    const Binding* binding = ctx().lookup(name.value);
    if (binding != nullptr && binding->is_enumerator()) {
      return enum_constant(*binding->enum_value, binding->enum_type);
    }
    return ir::Var{.name = name.value};
  });
}

// --- The levels ---

Exp Descent::parse_exp() {
  Exp left = parse_assignment_exp();
  while (at(t.COMMA)) {
    take();
    Exp right = parse_assignment_exp();
    left = ir::Comma{.left = std::move(left), .right = std::move(right)};
  }
  return left;
}

Exp Descent::parse_assignment_exp() {
  // `conditional_exp ASSIGN assignment_exp`, and the compound forms. A
  // compound assignment is a node of its own, so c99_to_tac evaluates
  // the lvalue's address once, as `arr[i++] += 1` needs.
  Exp lval = parse_conditional_exp();
  const int k = kind();
  if (k == t.ASSIGN) {
    take();
    Exp rval = parse_assignment_exp();
    return ir::Assignment{.lval = std::move(lval), .rval = std::move(rval)};
  }
  ir::Type_binary_operator op;
  if (k == t.PLUS_ASSIGN) {
    op = ir::Add{};
  } else if (k == t.MINUS_ASSIGN) {
    op = ir::Subtract{};
  } else if (k == t.STAR_ASSIGN) {
    op = ir::Multiply{};
  } else if (k == t.SLASH_ASSIGN) {
    op = ir::Divide{};
  } else if (k == t.PERCENT_ASSIGN) {
    op = ir::Modulo{};
  } else if (k == t.AMP_ASSIGN) {
    op = ir::BitwiseAnd{};
  } else if (k == t.PIPE_ASSIGN) {
    op = ir::BitwiseOr{};
  } else if (k == t.CARET_ASSIGN) {
    op = ir::BitwiseXor{};
  } else if (k == t.LSHIFT_ASSIGN) {
    op = ir::LeftShift{};
  } else if (k == t.RSHIFT_ASSIGN) {
    op = ir::RightShift{};
  } else {
    return lval;
  }
  take();
  Exp rval = parse_assignment_exp();
  return ir::CompoundAssignment{.op = std::move(op), .lval = std::move(lval),
                                .rval = std::move(rval)};
}

Exp Descent::parse_constant_exp() {
  // `constant_exp: conditional_exp`, a wrapper whose action passes the
  // expression on. The passes after the parser check that it's
  // constant.
  return parse_conditional_exp();
}

Exp Descent::parse_conditional_exp() {
  Exp condition = parse_binary_exp(0);
  if (!at(t.QUESTION)) {
    return condition;
  }
  take();
  Exp true_clause = parse_exp();
  expect(t.COLON);
  Exp false_clause = parse_conditional_exp();
  return ir::Conditional{.condition = std::move(condition),
                         .true_clause = std::move(true_clause),
                         .false_clause = std::move(false_clause)};
}

Exp Descent::parse_binary_exp(int level) {
  if (level > 9) {
    return parse_cast_exp();
  }
  Exp left = parse_binary_exp(level + 1);
  while (true) {
    const int k = kind();
    ir::Type_binary_operator op;
    bool found = true;
    switch (level) {
      case 0:
        found = k == t.OROR;
        op = ir::LogicalOr{};
        break;
      case 1:
        found = k == t.ANDAND;
        op = ir::LogicalAnd{};
        break;
      case 2:
        found = k == t.PIPE;
        op = ir::BitwiseOr{};
        break;
      case 3:
        found = k == t.CARET;
        op = ir::BitwiseXor{};
        break;
      case 4:
        found = k == t.AMP;
        op = ir::BitwiseAnd{};
        break;
      case 5:
        if (k == t.EQ) {
          op = ir::Equal{};
        } else if (k == t.NE) {
          op = ir::NotEqual{};
        } else {
          found = false;
        }
        break;
      case 6:
        if (k == t.LT) {
          op = ir::LessThan{};
        } else if (k == t.GT) {
          op = ir::GreaterThan{};
        } else if (k == t.LE) {
          op = ir::LessOrEqual{};
        } else if (k == t.GE) {
          op = ir::GreaterOrEqual{};
        } else {
          found = false;
        }
        break;
      case 7:
        if (k == t.LSHIFT) {
          op = ir::LeftShift{};
        } else if (k == t.RSHIFT) {
          op = ir::RightShift{};
        } else {
          found = false;
        }
        break;
      case 8:
        if (k == t.PLUS) {
          op = ir::Add{};
        } else if (k == t.MINUS) {
          op = ir::Subtract{};
        } else {
          found = false;
        }
        break;
      default:
        if (k == t.STAR) {
          op = ir::Multiply{};
        } else if (k == t.SLASH) {
          op = ir::Divide{};
        } else if (k == t.PERCENT) {
          op = ir::Modulo{};
        } else {
          found = false;
        }
        break;
    }
    if (!found) {
      return left;
    }
    take();
    Exp right = parse_binary_exp(level + 1);
    left = ir::Binary{.op = std::move(op), .left = std::move(left), .right = std::move(right)};
  }
}

Exp Descent::parse_cast_exp() {
  // `cast_exp: LPAREN type_name RPAREN cast_exp -> cast | unary_exp`.
  if (at(t.LPAREN) && !pending_primary) {
    return parse_parenthesized(true);
  }
  return parse_unary_exp();
}

Exp Descent::parse_unary_exp() {
  if (pending_primary) {
    return parse_postfix_ops(parse_atom());
  }
  const int k = kind();
  if (k == t.MINUS || k == t.PLUS || k == t.TILDE || k == t.BANG) {
    // `unop cast_exp -> unary`.
    take();
    ir::Type_unary_operator op;
    if (k == t.MINUS) {
      op = ir::Negate{};
    } else if (k == t.PLUS) {
      op = ir::Plus{};
    } else if (k == t.TILDE) {
      op = ir::Complement{};
    } else {
      op = ir::LogicalNot{};
    }
    Exp inner = parse_cast_exp();
    return ir::Unary{.op = std::move(op), .exp = std::move(inner)};
  }
  if (k == t.STAR) {
    // `*e` and `&e` are nodes of their own. The type checker fills in
    // their types: the pointee's for `*e`, a pointer to the operand's
    // for `&e`.
    take();
    return ir::Dereference{.exp = parse_cast_exp()};
  }
  if (k == t.AMP) {
    take();
    return ir::AddressOf{.exp = parse_cast_exp()};
  }
  if (k == t.PLUSPLUS || k == t.MINUSMINUS) {
    // Prefix `++a` and `--a` are nodes of their own, so c99_to_tac
    // evaluates the operand's address once, as `++arr[--i]` needs. The
    // result is the new value.
    take();
    Exp operand = parse_unary_exp();
    ir::Type_incdec_op op = ir::Increment{};
    if (k == t.MINUSMINUS) {
      op = ir::Decrement{};
    }
    return ir::Prefix{.op = std::move(op), .operand = std::move(operand)};
  }
  if (k == t.SIZEOF) {
    // `sizeof e` and `sizeof (T)` (C99 §6.5.3.4). The operand isn't
    // evaluated, so c99_to_tac folds the node to a constant. After
    // `sizeof (T)`, a `{` makes `(T){...}` a compound literal, the
    // operand of `sizeof e`.
    take();
    if (at(t.LPAREN)) {
      take();
      if (at_type_name()) {
        Type target = parse_type_name();
        expect(t.RPAREN);
        if (!at(t.LBRACE)) {
          return ir::SizeOfType{.target_type = std::move(target)};
        }
        Exp literal = parse_compound_literal_rest(std::move(target), {});
        return ir::SizeOfExp{.exp = parse_postfix_ops(std::move(literal))};
      }
      if (at_compound_literal_storage()) {
        std::vector<lark::Token> storage;
        while (at_compound_literal_storage()) {
          storage.push_back(take());
        }
        Type target = parse_type_name();
        expect(t.RPAREN);
        Exp literal = parse_compound_literal_rest(std::move(target), storage);
        return ir::SizeOfExp{.exp = parse_postfix_ops(std::move(literal))};
      }
      Exp inner = parse_exp();
      expect(t.RPAREN);
      return ir::SizeOfExp{.exp = parse_postfix_ops(std::move(inner))};
    }
    return ir::SizeOfExp{.exp = parse_unary_exp()};
  }
  if (k == t.LPAREN) {
    return parse_parenthesized(false);
  }
  return parse_postfix_ops(parse_atom());
}

Exp Descent::parse_parenthesized(bool cast_allowed) {
  expect(t.LPAREN);
  if (at_compound_literal_storage()) {
    // `(static T){...}`: C23's storage-class specifiers come between
    // the `(` and the type (§6.5.3.6).
    std::vector<lark::Token> storage;
    while (at_compound_literal_storage()) {
      storage.push_back(take());
    }
    Type target = parse_type_name();
    expect(t.RPAREN);
    return parse_postfix_ops(parse_compound_literal_rest(std::move(target), storage));
  }
  if (at_type_name()) {
    Type target = parse_type_name();
    expect(t.RPAREN);
    if (at(t.LBRACE) || !cast_allowed) {
      return parse_postfix_ops(parse_compound_literal_rest(std::move(target), {}));
    }
    Exp operand = parse_cast_exp();
    // The type has its role as a cast's target here, where C99
    // §6.5.4p2 forbids an array or a function type. `sizeof (T)` takes
    // both, so the check is the cast's.
    return action("cast", [&]() -> Exp {
      if (std::holds_alternative<ir::Array>(target)) {
        throw ParserError(
            "cannot cast to an array type (C99 §6.5.4.2 — cast target must be scalar or void)");
      }
      if (std::holds_alternative<ir::FunType>(target)) {
        throw ParserError(
            "cannot cast to a function type (C99 §6.5.4.2 — cast target must be scalar or "
            "void)");
      }
      return ir::Cast{.target_type = std::move(target), .exp = std::move(operand)};
    });
  }
  // `LPAREN exp RPAREN -> paren`, whose action passes the expression on.
  Exp inner = parse_exp();
  expect(t.RPAREN);
  return parse_postfix_ops(std::move(inner));
}

Exp Descent::parse_postfix_ops(Exp operand) {
  while (true) {
    const int k = kind();
    if (k == t.PLUSPLUS || k == t.MINUSMINUS) {
      // Postfix `a++` and `a--` return the old value and change the
      // operand, so they have a node of their own.
      take();
      ir::Type_incdec_op op = ir::Increment{};
      if (k == t.MINUSMINUS) {
        op = ir::Decrement{};
      }
      operand = ir::Postfix{.op = std::move(op), .operand = std::move(operand)};
    } else if (k == t.LBRACKET) {
      take();
      Exp index = parse_exp();
      expect(t.RBRACKET);
      operand = ir::Subscript{.array = std::move(operand), .index = std::move(index)};
    } else if (k == t.DOT || k == t.ARROW) {
      // A member name has a name space of its own (C99 §6.2.3), so it's
      // never a `Var`.
      take();
      const lark::Token member = expect(t.IDENTIFIER);
      if (k == t.DOT) {
        operand = ir::Dot{.operand = std::move(operand), .member = member.value};
      } else {
        operand = ir::Arrow{.operand = std::move(operand), .member = member.value};
      }
    } else if (k == t.LPAREN) {
      take();
      std::vector<Exp> args;
      if (!at(t.RPAREN)) {
        args.push_back(parse_assignment_exp());
        while (at(t.COMMA)) {
          take();
          args.push_back(parse_assignment_exp());
        }
      }
      expect(t.RPAREN);
      // A name (`f(x)`, `(fp)(x)`) makes a FunctionCall, and any other
      // callee (`(*fp)(x)`, `t[i](x)`, `pick()(x)`) an IndirectCall,
      // which the type checker turns back into a FunctionCall when
      // stripping `*` and `&` leaves a name. An enumeration constant is
      // replaced by its value before the call, so `A()` is a call of a
      // constant.
      operand = action("function_call", [&]() -> Exp {
        if (auto* v = std::get_if<ir::Var>(&operand)) {
          return ir::FunctionCall{.name = v->name, .args = std::move(args)};
        }
        if (std::holds_alternative<ir::Constant>(operand) ||
            std::holds_alternative<ir::String>(operand)) {
          throw ParserError("called object is not a function");
        }
        return ir::IndirectCall{.callee = std::move(operand), .args = std::move(args)};
      });
    } else {
      return operand;
    }
  }
}

Exp Descent::parse_atom() {
  if (pending_primary) {
    Exp e = std::move(*pending_primary);
    pending_primary.reset();
    return e;
  }
  const int k = kind();
  if (k == t.IDENTIFIER) {
    const lark::Token name = take();
    return identifier_exp(name);
  }
  if (k == t.STRING_LITERAL) {
    // Adjacent string literals are joined (C99 §6.4.5p5) into one
    // String, each byte a code point, as Python decodes the bytes as
    // Latin-1. The null terminator is added in the layout of static
    // storage, which keeps it out of the join.
    std::vector<lark::Token> tokens;
    while (at(t.STRING_LITERAL)) {
      tokens.push_back(take());
    }
    return action("str_literal", [&]() -> Exp {
      std::u32string joined;
      for (const lark::Token& token : tokens) {
        for (unsigned char b : decode_escapes(literal_body(token.value))) {
          joined.push_back(static_cast<char32_t>(b));
        }
      }
      return ir::String{.str = lark::encode_utf8(joined)};
    });
  }
  if (k == t.INTEGER_CONSTANT || k == t.LONG_INTEGER || k == t.UINT_INTEGER ||
      k == t.ULONG_INTEGER || k == t.BITINT_INTEGER || k == t.UBITINT_INTEGER ||
      k == t.DOUBLE_CONSTANT || k == t.FLOAT_CONSTANT || k == t.LONG_DOUBLE_CONSTANT ||
      k == t.CHAR_CONSTANT || k == t.TRUE || k == t.FALSE || k == t.NULLPTR) {
    const lark::Token token = take();
    return constant_exp_of(token);
  }
  unexpected("an expression");
}

}  // namespace c6502::parser
