// pcpp's lexer: the rules of pcpp/parser.py, with preprocessor.py's
// pp-number in place of pcpp's integer rule, run the way PLY's `lex`
// runs them.
//
// PLY joins the rules into one regular expression of alternatives, the
// rules given as functions first in the order of their source lines and
// then the rules given as strings, longest pattern first. Python's
// alternation takes the first alternative that matches, whatever the
// length of the others' matches, so the order decides the token. This
// lexer tests the rules by hand in the same order:
//
//     CPP_WS          [ \t]+ or one \n
//     CPP_LINECONT    \ then [ \t]* then \n, its value the spaces between
//     CPP_INTEGER     a pp-number (pp_numbers.pp_number_at)
//     CPP_STRING      "..." with \ escaping any character, no newline
//     CPP_CHAR        '...' or L'...', the same way
//     CPP_COMMENT1    /* ... */, the shortest
//     CPP_COMMENT2    // to the end of the line
//     CPP_FLOAT       Unicode digits, which no pp-number starts with
//     CPP_ID          [A-Za-z_] then Unicode word characters
//     punctuators     ## || ++ <<= *= |= += >>= &= | \ -> /= . == >= ^ !=
//                     [ <= && ( << -= -- %= + # ? ] ) >> * ^= & : , " = !
//                     / > { < - % } ; ' ~
//
// Some consequences of the order are visible in the output. `^=` is two
// tokens, `^` and `=`, since `\^` comes first. pcpp writes the pattern
// of CPP_XOREQUAL as `^=` without escaping the `^`, which anchors it at
// the start of the lexer's input, so a lone `=` there is a CPP_XOREQUAL
// whose value is "=". An unclosed `"`, `'`, or `/*` lexes as punctuation,
// and a character no rule matches, such as `@`, `$`, or a carriage
// return, is a `Char` token of its own (PLY's `t_error` in pcpp).
#pragma once

#include <string>

#include "pcpp_token.h"

namespace c6502::pcpp {

// A PLY lexer over one input, which `token` reads a token at a time. A
// copy is PLY's `clone`.
class Lexer {
 public:
  // Starts reading `text`, as PLY's `input` does. The line number stays
  // what it was.
  void input(std::string text) {
    data_ = std::move(text);
    pos_ = 0;
    counted_ = 0;
    code_points_ = 0;
  }

  // Returns the next token, or null at the end of the input.
  TokenPtr token();

  int lineno = 1;

 private:
  std::string data_;
  std::size_t pos_ = 0;
  // The code points of `data_` before byte `counted_`, which give a
  // token's `lexpos` as Python counts it in a `str`.
  std::size_t counted_ = 0;
  std::size_t code_points_ = 0;
};

// Returns every token of `text`, from a new lexer whose line number
// starts at 1.
Tokens lex_all(const std::string& text);

}  // namespace c6502::pcpp
