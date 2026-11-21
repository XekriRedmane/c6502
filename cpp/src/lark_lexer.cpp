// The port of Lark's basic lexer (lark_lexer.h). The terminals and
// their order come from lark_table.h, and each pattern of type "re" is
// matched by py_regex with Python's semantics.
#include "lark_lexer.h"

#include <memory>
#include <mutex>
#include <vector>

#include "py_regex.h"
#include "pyrepr.h"

namespace c6502::lark {

namespace {

// The compiled form of the table's terminals, built once per table.
struct Compiled {
  // One per terminal, in the table's order: the regular expression of a
  // terminal of type "re", or null for a string.
  std::vector<std::unique_ptr<py_regex::Regex>> regexes;
  std::vector<std::u32string> strings;
  int identifier = -1;
};

const Compiled& compiled(const lark_table::Table& table) {
  static std::once_flag once;
  static const lark_table::Table* built_for = nullptr;
  static Compiled c;
  std::call_once(once, [&] {
    built_for = &table;
    for (const auto& t : table.lex_terminals) {
      if (t.is_regex) {
        c.regexes.push_back(std::make_unique<py_regex::Regex>(t.pattern));
        c.strings.emplace_back();
      } else {
        c.regexes.push_back(nullptr);
        c.strings.push_back(decode_utf8(t.pattern));
      }
    }
    c.identifier = symbol_of(table, "IDENTIFIER");
  });
  if (built_for != &table) {
    throw InternalError("lark_lexer: one table per process");
  }
  return c;
}

void append_utf8(std::string& out, char32_t c) {
  if (c < 0x80) {
    out += static_cast<char>(c);
  } else if (c < 0x800) {
    out += static_cast<char>(0xC0 | (c >> 6));
    out += static_cast<char>(0x80 | (c & 0x3F));
  } else if (c < 0x10000) {
    out += static_cast<char>(0xE0 | (c >> 12));
    out += static_cast<char>(0x80 | ((c >> 6) & 0x3F));
    out += static_cast<char>(0x80 | (c & 0x3F));
  } else {
    out += static_cast<char>(0xF0 | (c >> 18));
    out += static_cast<char>(0x80 | ((c >> 12) & 0x3F));
    out += static_cast<char>(0x80 | ((c >> 6) & 0x3F));
    out += static_cast<char>(0x80 | (c & 0x3F));
  }
}

// Returns the length of `s` after Python's `s.expandtabs()`, with tab
// stops every 8 columns.
std::size_t expanded_length(std::u32string_view s) {
  std::size_t total = 0;
  std::size_t column = 0;
  for (char32_t c : s) {
    if (c == U'\t') {
      const std::size_t n = 8 - column % 8;
      total += n;
      column += n;
    } else {
      ++total;
      column = (c == U'\n' || c == U'\r') ? 0 : column + 1;
    }
  }
  return total;
}

}  // namespace

int symbol_of(const lark_table::Table& table, std::string_view name) {
  for (int i = 0; i < table.terminal_count; ++i) {
    if (table.name(i) == name) {
      return i;
    }
  }
  return -1;
}

std::u32string decode_utf8(std::string_view text) {
  std::u32string out;
  out.reserve(text.size());
  for (std::size_t i = 0; i < text.size();) {
    const auto b = static_cast<unsigned char>(text[i]);
    int n = 0;
    char32_t c = 0;
    if (b < 0x80) {
      c = b, n = 0;
    } else if ((b & 0xE0) == 0xC0) {
      c = b & 0x1F, n = 1;
    } else if ((b & 0xF0) == 0xE0) {
      c = b & 0x0F, n = 2;
    } else if ((b & 0xF8) == 0xF0) {
      c = b & 0x07, n = 3;
    } else {
      throw InternalError("decode_utf8: the text isn't UTF-8");
    }
    if (i + static_cast<std::size_t>(n) >= text.size() && n > 0) {
      throw InternalError("decode_utf8: the text isn't UTF-8");
    }
    for (int k = 1; k <= n; ++k) {
      const auto cb = static_cast<unsigned char>(text[i + static_cast<std::size_t>(k)]);
      if ((cb & 0xC0) != 0x80) {
        throw InternalError("decode_utf8: the text isn't UTF-8");
      }
      c = (c << 6) | (cb & 0x3F);
    }
    out += c;
    i += static_cast<std::size_t>(n) + 1;
  }
  return out;
}

std::string encode_utf8(std::u32string_view text) {
  std::string out;
  out.reserve(text.size());
  for (char32_t c : text) {
    append_utf8(out, c);
  }
  return out;
}

std::string token_repr(const Token& token, const lark_table::Table& table) {
  return "Token(" + pyrepr::string(table.name(token.type)) + ", " + pyrepr::string(token.value) +
         ")";
}

Token end_token(const Token* last) {
  Token t;
  t.type = 0;  // `$END`, the first terminal by name
  if (last != nullptr) {
    t.start_pos = last->start_pos;
    t.line = last->line;
    t.column = last->column;
    t.end_line = last->end_line;
    t.end_column = last->end_column;
    t.end_pos = last->end_pos;
  } else {
    t.start_pos = 0;
    t.line = 1;
    t.column = 1;
  }
  return t;
}

std::string context(std::u32string_view text, std::size_t pos, std::size_t span) {
  const std::size_t start = pos > span ? pos - span : 0;
  const std::size_t end = std::min(text.size(), pos + span);
  std::u32string_view before = text.substr(start, pos - start);
  const std::size_t nl = before.rfind(U'\n');
  if (nl != std::u32string_view::npos) {
    before = before.substr(nl + 1);
  }
  std::u32string_view after = pos < end ? text.substr(pos, end - pos) : std::u32string_view{};
  const std::size_t anl = after.find(U'\n');
  if (anl != std::u32string_view::npos) {
    after = after.substr(0, anl);
  }
  return encode_utf8(before) + encode_utf8(after) + "\n" +
         std::string(expanded_length(before), ' ') + "^\n";
}

namespace {

std::string characters_message(std::u32string_view text, std::size_t pos, int line, int column,
                               const std::optional<Token>& last,
                               const lark_table::Table& table) {
  std::string m = "No terminal matches '";
  append_utf8(m, text[pos]);
  m += "' in the current parser context, at line " + std::to_string(line) + " col " +
       std::to_string(column);
  m += "\n\n" + context(text, pos);
  m += "Expected one of: \n\t* ";
  bool first = true;
  for (const char* name : table.allowed) {
    if (!first) {
      m += "\n\t* ";
    }
    first = false;
    m += name;
  }
  m += "\n";
  if (last) {
    m += "\nPrevious tokens: " + token_repr(*last, table) + "\n";
  }
  return m;
}

}  // namespace

UnexpectedCharacters::UnexpectedCharacters(std::u32string_view text, std::size_t pos, int line,
                                           int column, const std::optional<Token>& last,
                                           const lark_table::Table& table)
    : CompileError("UnexpectedCharacters",
                   characters_message(text, pos, line, column, last, table)),
      line_(line),
      column_(column),
      pos_(pos) {
  append_utf8(char_, text[pos]);
}

BasicLexer::BasicLexer(std::u32string_view text, const lark_table::Table& table)
    : text_(text), table_(table) {
  compiled(table);
}

std::optional<Token> BasicLexer::next_token() {
  const Compiled& c = compiled(table_);
  while (pos_ < text_.size()) {
    std::size_t index = 0;
    std::size_t end = 0;
    bool found = false;
    for (; index < table_.lex_terminals.size(); ++index) {
      if (const auto& re = c.regexes[index]) {
        if (auto m = re->match(text_, pos_)) {
          end = *m;
          found = true;
          break;
        }
      } else {
        const std::u32string& s = c.strings[index];
        if (text_.substr(pos_, s.size()) == s) {
          end = pos_ + s.size();
          found = true;
          break;
        }
      }
    }
    if (!found) {
      throw UnexpectedCharacters(text_, pos_, line_, column_, last_token_, table_);
    }
    const lark_table::LexTerminal& term = table_.lex_terminals[index];
    const std::u32string_view value = text_.substr(pos_, end - pos_);
    Token t;
    const bool ignored = term.symbol < 0;
    if (!ignored) {
      t.type = term.symbol;
      t.value = encode_utf8(value);
      t.start_pos = pos_;
      t.line = line_;
      t.column = column_;
    }
    // LineCounter.feed
    if (term.counts_newlines) {
      const std::size_t last_nl = value.rfind(U'\n');
      if (last_nl != std::u32string_view::npos) {
        for (char32_t ch : value) {
          line_ += ch == U'\n';
        }
        line_start_ = pos_ + last_nl + 1;
      }
    }
    pos_ = end;
    column_ = static_cast<int>(pos_ - line_start_ + 1);
    if (ignored) {
      continue;
    }
    t.end_line = line_;
    t.end_column = column_;
    t.end_pos = pos_;
    if (t.type == c.identifier) {
      // The callback of IDENTIFIER, which retypes a keyword.
      for (const auto& k : table_.keywords) {
        if (t.value == k.spelling) {
          t.type = k.symbol;
          break;
        }
      }
    }
    last_token_ = t;
    return t;
  }
  return std::nullopt;
}

}  // namespace c6502::lark
