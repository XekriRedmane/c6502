// Part R3 of the parser's descent (parser_descent.h): statements,
// expressions, constants, initializers, designators, and compound
// literals (parser.py:1679-1990, 3352-3505, 3607-3716, and 4170-4673).
#include <string>

#include "parser_descent.h"

namespace c6502::parser {

namespace {

[[noreturn]] void not_ported(const char* what) {
  throw InternalError(std::string("parser: ") + what + " isn't ported yet (part R3)");
}

}  // namespace

Exp Descent::parse_exp() { not_ported("exp"); }

Exp Descent::parse_assignment_exp() { not_ported("assignment_exp"); }

Exp Descent::parse_constant_exp() { not_ported("constant_exp"); }

Exp Descent::parse_initializer() { not_ported("initializer"); }

c99_ast::Block Descent::parse_block() { not_ported("block"); }

c99_ast::Type_statement Descent::parse_statement() { not_ported("statement"); }

IntegerToken Descent::parse_integer_token(std::string_view) { not_ported("_parse_integer_token"); }

}  // namespace c6502::parser
