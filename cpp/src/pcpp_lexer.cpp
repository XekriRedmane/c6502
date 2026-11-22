#include "pcpp_lexer.h"

#include <cstring>

#include "pcpp_text.h"
#include "pp_numbers.h"

namespace c6502::pcpp {

namespace {

// Returns the end of the quoted literal that starts at `pos` with
// `quote`, as the lazy pattern `Q([^\\\n]|(\\(.|\n)))*?Q` matches it, or
// npos when no such literal starts there: a backslash takes the next
// character with it, whatever it is, and a newline or the end of the
// input before the closing quote is no match.
std::size_t quoted_end(const std::string& s, std::size_t pos, char quote) {
  std::size_t j = pos + 1;
  while (j < s.size()) {
    const char c = s[j];
    if (c == quote) return j + 1;
    if (c == '\\') {
      if (j + 1 >= s.size()) return std::string::npos;
      // `.` takes one code point, and `\n` the newline, so the escaped
      // character is a whole code point.
      std::size_t len = 0;
      decode_at(s, j + 1, len);
      j += 1 + len;
    } else if (c == '\n') {
      return std::string::npos;
    } else {
      ++j;
    }
  }
  return std::string::npos;
}

std::size_t count_newlines(const std::string& s) {
  std::size_t n = 0;
  for (const char c : s) n += c == '\n';
  return n;
}

// Returns the length of the run of Unicode digits (`\d+`) at `pos`.
std::size_t digits_at(const std::string& s, std::size_t pos) {
  std::size_t j = pos;
  while (j < s.size()) {
    std::size_t len = 0;
    if (!is_digit(decode_at(s, j, len))) break;
    j += len;
  }
  return j - pos;
}

// Returns the length of pcpp's CPP_FLOAT at `pos`,
// `((\d+)(\.\d+)(e(\+|-)?(\d+))?|(\d+)e(\+|-)?(\d+))([lL]|[fF])?`, or 0.
std::size_t float_at(const std::string& s, std::size_t pos) {
  const std::size_t d1 = digits_at(s, pos);
  if (d1 == 0) return 0;
  std::size_t j = pos + d1;
  std::size_t end = 0;
  // The exponent `e(\+|-)?(\d+)` at `k`, or 0.
  auto exponent = [&](std::size_t k) -> std::size_t {
    if (k >= s.size() || s[k] != 'e') return 0;
    std::size_t m = k + 1;
    if (m < s.size() && (s[m] == '+' || s[m] == '-')) {
      const std::size_t d = digits_at(s, m + 1);
      if (d) return 2 + d;
    }
    const std::size_t d = digits_at(s, m);
    return d ? 1 + d : 0;
  };
  if (j < s.size() && s[j] == '.' && digits_at(s, j + 1) > 0) {
    j += 1 + digits_at(s, j + 1);
    j += exponent(j);
    end = j;
  } else {
    const std::size_t e = exponent(j);
    if (e == 0) return 0;
    end = j + e;
  }
  if (end < s.size() && std::strchr("lLfF", s[end]) != nullptr) ++end;
  return end - pos;
}

// Returns the length of the identifier `[A-Za-z_][\w_]*` at `pos`, or 0.
std::size_t identifier_at(const std::string& s, std::size_t pos) {
  const char c = s[pos];
  if (!((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_')) return 0;
  std::size_t j = pos + 1;
  while (j < s.size()) {
    std::size_t len = 0;
    if (!is_word(decode_at(s, j, len))) break;
    j += len;
  }
  return j - pos;
}

struct Punct {
  const char* text;
  Type type;
};

// The rules given as strings, in PLY's order (the longest pattern
// first, the order of `sorted` with `reverse=True` among patterns of the
// same length, which PLY's table gives).
const Punct kPunctuators[] = {
    {"##", Type::CPP_DPOUND},        {"||", Type::CPP_LOGICALOR},
    {"++", Type::CPP_PLUSPLUS},      {"<<=", Type::CPP_LSHIFTEQUAL},
    {"*=", Type::CPP_MULTIPLYEQUAL}, {"|=", Type::CPP_OREQUAL},
    {"+=", Type::CPP_PLUSEQUAL},     {">>=", Type::CPP_RSHIFTEQUAL},
    {"&=", Type::CPP_ANDEQUAL},      {"|", Type::CPP_BAR},
    {"\\", Type::CPP_BSLASH},        {"->", Type::CPP_DEREFERENCE},
    {"/=", Type::CPP_DIVIDEEQUAL},   {".", Type::CPP_DOT},
    {"==", Type::CPP_EQUALITY},      {">=", Type::CPP_GREATEREQUAL},
    {"^", Type::CPP_HAT},            {"!=", Type::CPP_INEQUALITY},
    {"[", Type::CPP_LBRACKET},       {"<=", Type::CPP_LESSEQUAL},
    {"&&", Type::CPP_LOGICALAND},    {"(", Type::CPP_LPAREN},
    {"<<", Type::CPP_LSHIFT},        {"-=", Type::CPP_MINUSEQUAL},
    {"--", Type::CPP_MINUSMINUS},    {"%=", Type::CPP_PERCENTEQUAL},
    {"+", Type::CPP_PLUS},           {"#", Type::CPP_POUND},
    {"?", Type::CPP_QUESTION},       {"]", Type::CPP_RBRACKET},
    {")", Type::CPP_RPAREN},         {">>", Type::CPP_RSHIFT},
    {"*", Type::CPP_STAR},
    // CPP_XOREQUAL, `^=`, comes here, and matches only a `=` at the
    // start of the input (`Lexer::token` tests it).
    {"&", Type::CPP_AMPERSAND},      {":", Type::CPP_COLON},
    {",", Type::CPP_COMMA},          {"\"", Type::CPP_DQUOTE},
    {"=", Type::CPP_EQUAL},          {"!", Type::CPP_EXCLAMATION},
    {"/", Type::CPP_FSLASH},         {">", Type::CPP_GREATER},
    {"{", Type::CPP_LCURLY},         {"<", Type::CPP_LESS},
    {"-", Type::CPP_MINUS},          {"%", Type::CPP_PERCENT},
    {"}", Type::CPP_RCURLY},         {";", Type::CPP_SEMICOLON},
    {"'", Type::CPP_SQUOTE},         {"~", Type::CPP_TILDE},
};

}  // namespace

TokenPtr Lexer::token() {
  const std::string& s = data_;
  if (pos_ >= s.size()) return nullptr;
  auto tok = std::make_shared<Token>();
  tok->lineno = lineno;
  for (; counted_ < pos_; ++counted_) {
    if ((static_cast<unsigned char>(s[counted_]) & 0xC0) != 0x80) ++code_points_;
  }
  tok->lexpos = code_points_;
  const std::size_t pos = pos_;
  auto take = [&](Type type, std::size_t len) {
    tok->type = type;
    tok->value = s.substr(pos, len);
    pos_ = pos + len;
    return tok;
  };
  const char c = s[pos];

  // CPP_WS
  if (c == ' ' || c == '\t') {
    std::size_t j = pos;
    while (j < s.size() && (s[j] == ' ' || s[j] == '\t')) ++j;
    return take(Type::CPP_WS, j - pos);
  }
  if (c == '\n') {
    take(Type::CPP_WS, 1);
    ++lineno;
    return tok;
  }
  // CPP_LINECONT
  if (c == '\\') {
    std::size_t j = pos + 1;
    while (j < s.size() && (s[j] == ' ' || s[j] == '\t')) ++j;
    if (j < s.size() && s[j] == '\n') {
      take(Type::CPP_LINECONT, j + 1 - pos);
      tok->value = tok->value.substr(1, tok->value.size() - 2);
      ++lineno;
      return tok;
    }
  }
  // CPP_INTEGER, preprocessor.py's pp-number.
  {
    const std::string number = pp_numbers::pp_number_at(s, pos);
    if (!number.empty()) return take(Type::CPP_INTEGER, number.size());
  }
  // CPP_STRING, whose value loses its line continuations.
  if (c == '"') {
    const std::size_t end = quoted_end(s, pos, '"');
    if (end != std::string::npos) {
      take(Type::CPP_STRING, end - pos);
      std::string out;
      std::size_t subs = 0;
      const std::string& v = tok->value;
      for (std::size_t i = 0; i < v.size();) {
        if (v[i] == '\\') {
          std::size_t j = i + 1;
          while (j < v.size() && (v[j] == ' ' || v[j] == '\t')) ++j;
          if (j < v.size() && v[j] == '\n') {
            ++subs;
            i = j + 1;
            continue;
          }
        }
        out += v[i++];
      }
      tok->value = out;
      lineno += static_cast<int>(subs + count_newlines(out));
      return tok;
    }
  }
  // CPP_CHAR
  {
    const std::size_t q = c == 'L' ? pos + 1 : pos;
    if (q < s.size() && s[q] == '\'') {
      const std::size_t end = quoted_end(s, q, '\'');
      if (end != std::string::npos) {
        take(Type::CPP_CHAR, end - pos);
        lineno += static_cast<int>(count_newlines(tok->value));
        return tok;
      }
    }
  }
  // CPP_COMMENT1 and CPP_COMMENT2
  if (c == '/' && pos + 1 < s.size() && s[pos + 1] == '*') {
    const std::size_t close = s.find("*/", pos + 2);
    if (close != std::string::npos) {
      take(Type::CPP_COMMENT1, close + 2 - pos);
      lineno += static_cast<int>(count_newlines(tok->value));
      return tok;
    }
  }
  if (c == '/' && pos + 1 < s.size() && s[pos + 1] == '/') {
    const std::size_t nl = s.find('\n', pos);
    return take(Type::CPP_COMMENT2, (nl == std::string::npos ? s.size() : nl) - pos);
  }
  // CPP_FLOAT, which only Unicode digits outside ASCII reach.
  if (const std::size_t len = float_at(s, pos)) return take(Type::CPP_FLOAT, len);
  // CPP_ID
  if (const std::size_t len = identifier_at(s, pos)) return take(Type::CPP_ID, len);
  // The punctuators.
  for (const Punct& p : kPunctuators) {
    const std::size_t len = std::strlen(p.text);
    if (p.type == Type::CPP_AMPERSAND && pos == 0 && c == '=') {
      return take(Type::CPP_XOREQUAL, 1);
    }
    if (s.compare(pos, len, p.text) == 0) return take(p.type, len);
  }
  // PLY's `t_error` in pcpp: the character is a token whose type is the
  // character.
  std::size_t len = 0;
  decode_at(s, pos, len);
  take(Type::Char, len);
  tok->chr = tok->value;
  return tok;
}

Tokens lex_all(const std::string& text) {
  Lexer lex;
  lex.input(text);
  Tokens out;
  while (auto tok = lex.token()) out.push_back(tok);
  return out;
}

}  // namespace c6502::pcpp
