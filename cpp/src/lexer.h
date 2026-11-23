// lexer.py: the C lexer, a thin wrapper over Lark's basic lexer
// (lark_lexer.h) with c99.lark's terminals. `tokenize` gives the tokens
// that `compile.py --lex` prints, and `check_number` is the check of a
// number's C23 spelling that `tokenize` and the parser's lexer
// (parser_context.h) both make.
#pragma once

#include <string>
#include <string_view>
#include <vector>

#include "errors.h"
#include "lark_lexer.h"

namespace c6502::lexer {

// The keywords (C99 §6.4.1, with C23's additions), each of which has a
// terminal of its own in c99.lark.
bool is_keyword(std::string_view word);

enum class TokenKind { kKeyword, kIdentifier, kSymbol, kConstant, kStringLiteral };

// Returns the kind's value in Python's `TokenKind`, which `--lex` prints:
// "keyword", "identifier", "symbol", "constant", or "string-literal".
std::string_view kind_value(TokenKind kind);

struct Token {
  TokenKind kind;
  std::string value;
  int line;
  int col;
  bool operator==(const Token&) const = default;
};

// Python's `lexer.LexError`, whose message is `<line>:<col>: <msg>`.
class LexError : public CompileError {
 public:
  LexError(const std::string& message, int line, int col);
  int line() const { return line_; }
  int col() const { return col_; }

 private:
  int line_;
  int col_;
};

// Returns whether `type` is one of the terminals of numeric constants,
// which `check_number` looks at (`NUMBER_TERMINALS`), INVALID_NUMBER
// among them.
bool is_number_terminal(int type, const lark_table::Table& table);

// Throws LexError for the numeric token `lt` of `source` when the
// pp-number it begins (C23 §6.4.8) uses a C23 spelling wrongly: a digit
// separator that isn't between two digits, a binary constant with a
// digit other than 0 and 1 or no digits, or a `wb` or `uwb` constant too
// wide for any `_BitInt`. The message comes from pp_numbers.h. Any other
// number is left to the terminals and to the check in `tokenize`.
//
// A `'` right after a number is a separator that isn't followed by a
// digit. The pp-number ends before it, as in `1'` or `1''000`, and the
// `'` would start a character constant, which can't follow a number in a
// valid program.
void check_number(std::u32string_view source, const lark::Token& lt,
                  const lark_table::Table& table);

// Returns the tokens of `source`, which is UTF-8, as lexer.py's
// `tokenize` gives them, or throws its first LexError: an unexpected
// character, a malformed number, or a number that abuts an identifier
// (C99's pp-numbers are greedy, so `1foo` would be one invalid
// pp-number).
std::vector<Token> tokenize(std::string_view source);

// Returns what `compile.py --lex` prints for the preprocessed `source`,
// compile.py's `_format_tokens`: one `<line>:<col>`, tab, kind, tab,
// value line for each token, each line ending in LF. Throws the first
// LexError of `tokenize`, and then nothing is printed.
std::string format_tokens(std::string_view source);

}  // namespace c6502::lexer
