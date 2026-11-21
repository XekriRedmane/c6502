// parser.py's `parse`: the c99 program of a C source, or the error
// Python's parser raises for it, with the same message.
//
// The parser is a recursive descent written by hand (docs/cpp_port.md,
// "The plan for the parser"), which parser_descent.h describes. It reads
// the tokens through R1's `TokenStream`, which feeds each one to a
// recognizer of Lark's table before the descent takes it, so a syntax
// error is Lark's error at Lark's token.
#pragma once

#include <string_view>

#include "c99_ast.h"

namespace c6502::parser {

// Returns the c99 program of `source`, which is UTF-8 text after the
// preprocessor, as parser.py's `parse` builds it. Throws what Python's
// `parse` raises: `ParserError` for a construct the grammar accepts and
// c6502 rejects, and for the syntax errors it rewords,
// `lark::UnexpectedToken` and `lark::UnexpectedCharacters` for the
// others, `lexer::LexError` for a malformed number, and a `CompileError`
// of type "NotImplementedError" for a variable-length array, a K&R
// parameter list, and `[static N]`.
c99_ast::Program parse(std::string_view source);

}  // namespace c6502::parser
