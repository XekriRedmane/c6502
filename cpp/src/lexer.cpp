// lexer.py's C lexer (lexer.h).
#include "lexer.h"

#include <algorithm>
#include <array>
#include <optional>
#include <string_view>

#include "pp_numbers.h"
#include "pyrepr.h"

namespace c6502::lexer {

namespace {

constexpr std::array<std::string_view, 47> kKeywords = {
    "auto", "break", "case", "char", "const", "continue", "default", "do",
    "double", "else", "enum", "extern", "float", "for", "goto", "if",
    "inline", "int", "long", "register", "restrict", "return", "short",
    "signed", "sizeof", "static", "struct", "switch", "typedef", "union",
    "unsigned", "void", "volatile", "while",
    "_Bool", "_Complex", "_Imaginary",
    // C23 §6.4.1 adds these three. `_Bool` stays, as an alternative
    // spelling of `bool` that C23 makes obsolescent.
    "bool", "true", "false",
    // C23 §6.4.1 also adds `nullptr` (§6.4.4.6). Its type, `nullptr_t`,
    // isn't a keyword. <stddef.h> declares it as C23's does, as
    // `typeof_unqual(nullptr)`.
    "nullptr",
    // C23's bit-precise integer types (§6.7.3.1).
    "_BitInt",
    // C23's storage-class specifier for named constants (§6.7.2).
    "constexpr",
    // C23's `thread_local` (§6.7.2) and C11's spelling of it, which the
    // parser rejects, since c6502 has no threads.
    "thread_local", "_Thread_local",
    // C23's typeof operators (§6.7.3.6).
    "typeof", "typeof_unqual",
};

// The punctuators' terminals in c99.lark.
constexpr std::array<std::string_view, 49> kSymbolTerminals = {
    "ELLIPSIS", "COLONCOLON", "LSHIFT_ASSIGN", "RSHIFT_ASSIGN", "ARROW", "PLUSPLUS",
    "MINUSMINUS", "LSHIFT", "RSHIFT", "LE", "GE", "EQ", "NE", "ANDAND", "OROR",
    "STAR_ASSIGN", "SLASH_ASSIGN", "PERCENT_ASSIGN", "PLUS_ASSIGN", "MINUS_ASSIGN",
    "AMP_ASSIGN", "CARET_ASSIGN", "PIPE_ASSIGN", "HASHHASH", "LBRACKET", "RBRACKET",
    "LPAREN", "RPAREN", "LBRACE", "RBRACE", "DOT", "AMP", "STAR", "PLUS", "MINUS",
    "TILDE", "BANG", "SLASH", "PERCENT", "LT", "GT", "CARET", "PIPE", "QUESTION",
    "COLON", "SEMICOLON", "ASSIGN", "COMMA", "HASH",
};

constexpr std::array<std::string_view, 10> kNumberTerminals = {
    "INTEGER_CONSTANT", "LONG_INTEGER", "UINT_INTEGER", "ULONG_INTEGER",
    "BITINT_INTEGER", "UBITINT_INTEGER", "DOUBLE_CONSTANT", "FLOAT_CONSTANT",
    "LONG_DOUBLE_CONSTANT", "INVALID_NUMBER",
};

template <std::size_t N>
bool contains(const std::array<std::string_view, N>& a, std::string_view s) {
  return std::find(a.begin(), a.end(), s) != a.end();
}

// Each keyword has its own terminal in c99.lark, named for the keyword
// uppercased with leading and trailing underscores stripped (`_Complex`
// is `COMPLEX`). `_Bool` would then collide with `bool`, so its terminal
// is `ALT_BOOL`, and `_Thread_local`'s is `ALT_THREAD_LOCAL`.
std::string keyword_terminal(std::string_view kw) {
  if (kw == "_Bool") {
    return "ALT_BOOL";
  }
  if (kw == "_Thread_local") {
    return "ALT_THREAD_LOCAL";
  }
  while (!kw.empty() && kw.front() == '_') {
    kw.remove_prefix(1);
  }
  while (!kw.empty() && kw.back() == '_') {
    kw.remove_suffix(1);
  }
  std::string out(kw);
  for (char& c : out) {
    c = static_cast<char>(c >= 'a' && c <= 'z' ? c - 'a' + 'A' : c);
  }
  return out;
}

// Python's `_TERMINAL_TO_KIND`, or nothing for a terminal it lacks.
std::optional<TokenKind> terminal_kind(std::string_view name) {
  for (std::string_view kw : kKeywords) {
    if (keyword_terminal(kw) == name) {
      return TokenKind::kKeyword;
    }
  }
  if (contains(kSymbolTerminals, name) || name == "ATTR_OPEN") {
    return TokenKind::kSymbol;
  }
  if (name == "IDENTIFIER") {
    return TokenKind::kIdentifier;
  }
  if ((contains(kNumberTerminals, name) && name != "INVALID_NUMBER") || name == "CHAR_CONSTANT") {
    return TokenKind::kConstant;
  }
  if (name == "STRING_LITERAL") {
    return TokenKind::kStringLiteral;
  }
  return std::nullopt;
}

bool is_ascii_name_char(char32_t c) {
  return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') ||
         c == '_' || c == '.' || c == '\'';
}

}  // namespace

bool is_keyword(std::string_view word) { return contains(kKeywords, word); }

std::string_view kind_value(TokenKind kind) {
  switch (kind) {
    case TokenKind::kKeyword:
      return "keyword";
    case TokenKind::kIdentifier:
      return "identifier";
    case TokenKind::kSymbol:
      return "symbol";
    case TokenKind::kConstant:
      return "constant";
    case TokenKind::kStringLiteral:
      return "string-literal";
  }
  return "";
}

LexError::LexError(const std::string& message, int line, int col)
    : CompileError("LexError", std::to_string(line) + ":" + std::to_string(col) + ": " + message),
      line_(line),
      col_(col) {}

bool is_number_terminal(int type, const lark_table::Table& table) {
  return contains(kNumberTerminals, table.name(type));
}

void check_number(std::u32string_view source, const lark::Token& lt,
                  const lark_table::Table& table) {
  const std::size_t start = lt.start_pos;
  // A pp-number holds only ASCII, so the run of ASCII from `start` is
  // all of the text that `pp_number_at` and the pattern of `shown` read.
  std::size_t ascii_end = start;
  while (ascii_end < source.size() && source[ascii_end] < 0x80) {
    ++ascii_end;
  }
  const std::string window = lark::encode_utf8(source.substr(start, ascii_end - start));
  const std::string spelling = pp_numbers::pp_number_at(window, 0);
  const std::size_t end = start + spelling.size();
  if (end < source.size() && source[end] == U'\'') {
    std::size_t n = start;
    while (n < source.size() && is_ascii_name_char(source[n])) {
      ++n;
    }
    const std::string shown = lark::encode_utf8(source.substr(start, n - start));
    throw LexError("a digit separator has to be between two digits, and the one after " +
                       pyrepr::string(spelling) + " in " + pyrepr::string(shown) +
                       " isn't followed by a digit",
                   lt.line, lt.column);
  }
  const std::string_view name = table.name(lt.type);
  const std::string_view prefix = std::string_view(spelling).substr(0, 2);
  if (spelling.find('\'') == std::string::npos && prefix != "0b" && prefix != "0B" &&
      name != "BITINT_INTEGER" && name != "UBITINT_INTEGER") {
    return;
  }
  try {
    pp_numbers::check(spelling);
    if (!pp_numbers::is_floating(spelling)) {
      const auto constant = pp_numbers::parse_integer(spelling);
      if (constant.bit_precise()) {
        pp_numbers::bit_precise_width(constant, spelling);
      }
    }
  } catch (const pp_numbers::NumberError& e) {
    throw LexError(e.what(), lt.line, lt.column);
  }
  if (spelling.size() != lt.value.size()) {
    throw LexError("invalid pp-number " + pyrepr::string(spelling), lt.line, lt.column);
  }
}

std::vector<Token> tokenize(std::string_view source) {
  const lark_table::Table& table = lark_table::c99_table();
  const std::u32string text = lark::decode_utf8(source);
  lark::BasicLexer lex(text, table);
  std::vector<Token> out;
  try {
    while (auto lt = lex.next_token()) {
      const std::string_view name = table.name(lt->type);
      if (is_number_terminal(lt->type, table)) {
        check_number(text, *lt, table);
      }
      if (name == "INVALID_NUMBER") {
        throw LexError("malformed numeric token " + pyrepr::string(lt->value), lt->line,
                       lt->column);
      }
      const auto kind = terminal_kind(name);
      if (!kind) {
        throw LexError("unrecognized token type " + std::string(name), lt->line, lt->column);
      }
      Token tok{*kind, lt->value, lt->line, lt->column};
      if (!out.empty()) {
        const Token& prev = out.back();
        if (prev.kind == TokenKind::kConstant && tok.kind == TokenKind::kIdentifier &&
            prev.line == tok.line &&
            prev.col + static_cast<int>(prev.value.size()) == tok.col) {
          throw LexError("invalid pp-number: " + prev.value + pyrepr::string(tok.value) +
                             " (numeric constant may not abut an identifier)",
                         prev.line, prev.col);
        }
      }
      out.push_back(std::move(tok));
    }
  } catch (const lark::UnexpectedCharacters& e) {
    throw LexError("unexpected character " + pyrepr::string(e.character()), e.line(),
                   e.column());
  }
  return out;
}

std::string format_tokens(std::string_view source) {
  std::string out;
  for (const Token& tok : tokenize(source)) {
    out += std::to_string(tok.line) + ':' + std::to_string(tok.col) + '\t';
    out += kind_value(tok.kind);
    out += '\t' + tok.value + '\n';
  }
  return out;
}

}  // namespace c6502::lexer
