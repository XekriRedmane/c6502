// The token of pcpp 1.30, the C preprocessor that preprocessor.py
// wraps, as its PLY lexer makes it (`ply.lex.LexToken`) and as the rest
// of pcpp changes it.
//
// pcpp's tokens are Python objects with reference semantics. It copies
// one with `copy.copy`, which makes a new token that shares the old
// one's `expanded_from` list, and it changes tokens in place where
// several lists hold the same one (an argument used twice in a macro's
// body appears twice in the expansion). The C++ keeps those semantics,
// so that each quirk of pcpp's macro expansion comes out the same: a
// token is held by `TokenPtr`, `copy_token` is `copy.copy`, and
// `expanded_from` is a shared list that is null when the token has no
// such attribute yet.
#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace c6502::pcpp {

// The token types of pcpp/parser.py's `tokens`, and two more. `Char` is
// the type of the token that PLY's `t_error` makes of a character no
// rule matches, whose Python type is the character itself
// (`Token::chr`). `None` is the type pcpp gives a token that `##` has
// pasted, until it lexes the token's text again.
enum class Type : std::uint8_t {
  CPP_ID,
  CPP_INTEGER,
  CPP_FLOAT,
  CPP_STRING,
  CPP_CHAR,
  CPP_WS,
  CPP_LINECONT,
  CPP_COMMENT1,
  CPP_COMMENT2,
  CPP_POUND,
  CPP_DPOUND,
  CPP_PLUS,
  CPP_MINUS,
  CPP_STAR,
  CPP_FSLASH,
  CPP_PERCENT,
  CPP_BAR,
  CPP_AMPERSAND,
  CPP_TILDE,
  CPP_HAT,
  CPP_LESS,
  CPP_GREATER,
  CPP_EQUAL,
  CPP_EXCLAMATION,
  CPP_QUESTION,
  CPP_LPAREN,
  CPP_RPAREN,
  CPP_LBRACKET,
  CPP_RBRACKET,
  CPP_LCURLY,
  CPP_RCURLY,
  CPP_DOT,
  CPP_COMMA,
  CPP_SEMICOLON,
  CPP_COLON,
  CPP_BSLASH,
  CPP_SQUOTE,
  CPP_DQUOTE,
  CPP_DEREFERENCE,
  CPP_MINUSEQUAL,
  CPP_MINUSMINUS,
  CPP_LSHIFT,
  CPP_LESSEQUAL,
  CPP_RSHIFT,
  CPP_GREATEREQUAL,
  CPP_LOGICALOR,
  CPP_OREQUAL,
  CPP_LOGICALAND,
  CPP_ANDEQUAL,
  CPP_EQUALITY,
  CPP_INEQUALITY,
  CPP_XOREQUAL,
  CPP_MULTIPLYEQUAL,
  CPP_DIVIDEEQUAL,
  CPP_PLUSEQUAL,
  CPP_PLUSPLUS,
  CPP_PERCENTEQUAL,
  CPP_LSHIFTEQUAL,
  CPP_RSHIFTEQUAL,
  Char,
  None,
};

// The list of the macros a token was expanded from, which pcpp's
// `expand_macros` reads to stop a macro from expanding inside itself.
using ExpandedFrom = std::shared_ptr<std::vector<std::string>>;

struct Token {
  Type type = Type::None;
  // The character that is the type of a `Char` token, in UTF-8.
  std::string chr;
  // The token's text, in UTF-8. Python holds it as a `str`, whose
  // characters are the code points of this text.
  std::string value;
  int lineno = 0;
  // The offset of the token in the text its lexer was given, in code
  // points, as PLY counts it in a `str`. The `--debug` log shows it in
  // the `repr` of a macro's tokens.
  std::size_t lexpos = 0;
  // The file the token came from, with pcpp's rewriting of its path,
  // "" for a token of `Preprocessor.tokenize`, and None for a token of a
  // source that has no file name.
  std::optional<std::string> source;
  ExpandedFrom expanded_from;
};

using TokenPtr = std::shared_ptr<Token>;
using Tokens = std::vector<TokenPtr>;

// Returns a new token with the fields of `tok`, as Python's `copy.copy`
// does. The two share `expanded_from`.
inline TokenPtr copy_token(const TokenPtr& tok) { return std::make_shared<Token>(*tok); }

// Returns the name of a type, as pcpp's tokens hold it: "CPP_ID" and so
// on, "None" for `Type::None`. A `Char` token's type is its character,
// which `type_name(const Token&)` gives, and this returns "" for it.
inline const char* type_name(Type t) {
  static const char* const kNames[] = {
      "CPP_ID",
      "CPP_INTEGER",
      "CPP_FLOAT",
      "CPP_STRING",
      "CPP_CHAR",
      "CPP_WS",
      "CPP_LINECONT",
      "CPP_COMMENT1",
      "CPP_COMMENT2",
      "CPP_POUND",
      "CPP_DPOUND",
      "CPP_PLUS",
      "CPP_MINUS",
      "CPP_STAR",
      "CPP_FSLASH",
      "CPP_PERCENT",
      "CPP_BAR",
      "CPP_AMPERSAND",
      "CPP_TILDE",
      "CPP_HAT",
      "CPP_LESS",
      "CPP_GREATER",
      "CPP_EQUAL",
      "CPP_EXCLAMATION",
      "CPP_QUESTION",
      "CPP_LPAREN",
      "CPP_RPAREN",
      "CPP_LBRACKET",
      "CPP_RBRACKET",
      "CPP_LCURLY",
      "CPP_RCURLY",
      "CPP_DOT",
      "CPP_COMMA",
      "CPP_SEMICOLON",
      "CPP_COLON",
      "CPP_BSLASH",
      "CPP_SQUOTE",
      "CPP_DQUOTE",
      "CPP_DEREFERENCE",
      "CPP_MINUSEQUAL",
      "CPP_MINUSMINUS",
      "CPP_LSHIFT",
      "CPP_LESSEQUAL",
      "CPP_RSHIFT",
      "CPP_GREATEREQUAL",
      "CPP_LOGICALOR",
      "CPP_OREQUAL",
      "CPP_LOGICALAND",
      "CPP_ANDEQUAL",
      "CPP_EQUALITY",
      "CPP_INEQUALITY",
      "CPP_XOREQUAL",
      "CPP_MULTIPLYEQUAL",
      "CPP_DIVIDEEQUAL",
      "CPP_PLUSEQUAL",
      "CPP_PLUSPLUS",
      "CPP_PERCENTEQUAL",
      "CPP_LSHIFTEQUAL",
      "CPP_RSHIFTEQUAL",
      "",
      "None",
  };
  return kNames[static_cast<std::size_t>(t)];
}

// Returns the Python value of `tok.type`, as a string.
inline std::string type_name(const Token& tok) {
  return tok.type == Type::Char ? tok.chr : std::string(type_name(tok.type));
}

}  // namespace c6502::pcpp
