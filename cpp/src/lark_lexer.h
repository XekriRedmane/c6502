// The port of Lark 1.3.1's basic lexer (`lark.lexer.BasicLexer`), as
// parser.py's `_TypedefLexer` and lexer.py's `tokenize` run it over
// c99.lark's terminals, with Lark's tokens and the message of its
// `UnexpectedCharacters`.
//
// Lark tries the terminals in one order (lark_table.h's `lex_terminals`)
// and takes the first that matches at the position, whatever the
// length of its match, so `08` is `0` and `8`. A keyword is an
// IDENTIFIER whose whole match is the keyword's string, which Lark's
// callback for IDENTIFIER retypes. `WS` is matched and dropped. The
// line and column of a token count code points, and only the terminals
// that can match a newline (`WS` and `ATTR_OPEN`) move the line.
#pragma once

#include <cstddef>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "errors.h"
#include "lark_table.h"

namespace c6502::lark {

// A token, Lark's `Token`: its terminal (a symbol of lark_table.h's
// table), its text, and where it starts and ends. Positions count code
// points from the start of the text, lines and columns from 1.
struct Token {
  int type = 0;
  std::string value;
  std::size_t start_pos = 0;
  int line = 1;
  int column = 1;
  int end_line = 1;
  int end_column = 1;
  std::size_t end_pos = 0;
};

// Returns Lark's `repr` of `token`, `Token('IDENTIFIER', 'x')`.
std::string token_repr(const Token& token, const lark_table::Table& table);

// Returns the token `$END` that Lark's LALR parser feeds after the last
// token, at the last token's position (`Token.new_borrow_pos`), or at
// line 1, column 1 when the text held no token.
Token end_token(const Token* last);

// Returns `text`, which is UTF-8, as code points, which is what Python
// indexes a `str` by. Throws InternalError on bytes that aren't UTF-8,
// which compile.py's reading of a source rejects before this.
std::u32string decode_utf8(std::string_view text);

// Returns the code points `text` as UTF-8.
std::string encode_utf8(std::u32string_view text);

// Lark's `UnexpectedCharacters`: no terminal matches the text at a
// position. The message is Lark's, with the terminals the lexer takes
// sorted, as parser.py's `_sort_expected` sorts them before the
// message is shown:
//
//     No terminal matches '@' in the current parser context, at line 1 col 9
//
//     int x = @;
//             ^
//     Expected one of:
//     	* AMP
//     	...
//
//     Previous tokens: Token('ASSIGN', '=')
class UnexpectedCharacters : public CompileError {
 public:
  UnexpectedCharacters(std::u32string_view text, std::size_t pos, int line, int column,
                       const std::optional<Token>& last, const lark_table::Table& table);

  // The character no terminal matched, as UTF-8.
  const std::string& character() const { return char_; }
  int line() const { return line_; }
  int column() const { return column_; }
  std::size_t pos() const { return pos_; }

 private:
  std::string char_;
  int line_;
  int column_;
  std::size_t pos_;
};

// Returns Lark's `get_context(text)` at `pos`: the line around the
// position, up to 40 code points either side, and a caret under it.
std::string context(std::u32string_view text, std::size_t pos, std::size_t span = 40);

// The basic lexer over one text, Lark's `BasicLexer` with its
// `LexerState`.
class BasicLexer {
 public:
  BasicLexer(std::u32string_view text, const lark_table::Table& table);

  // Returns the next token, or nothing at the end of the text. Throws
  // UnexpectedCharacters where no terminal matches.
  std::optional<Token> next_token();

  // The last token returned (Lark's `LexerState.last_token`), which an
  // UnexpectedCharacters shows as the previous token.
  const std::optional<Token>& last_token() const { return last_token_; }
  // The type the next UnexpectedCharacters shows for the last token.
  // parser.py's lexer retypes a token in place after Lark returned it
  // only when it didn't retype it when it was lexed (see
  // parser_context.h), so the caller sets the type here then.
  void set_last_token_type(int type) {
    if (last_token_) {
      last_token_->type = type;
    }
  }

  std::u32string_view text() const { return text_; }
  const lark_table::Table& table() const { return table_; }

 private:
  std::u32string_view text_;
  const lark_table::Table& table_;
  std::size_t pos_ = 0;
  int line_ = 1;
  int column_ = 1;
  std::size_t line_start_ = 0;
  std::optional<Token> last_token_;
};

// Returns the symbol of the terminal named `name` in `table`, or -1.
int symbol_of(const lark_table::Table& table, std::string_view name);

}  // namespace c6502::lark
