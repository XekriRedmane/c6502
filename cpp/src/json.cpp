#include "json.h"

#include <cstdint>
#include <cstdio>

namespace c6502::json {

const Value* Value::get(std::string_view key) const {
  for (std::size_t i = 0; i < keys.size(); ++i) {
    if (keys[i] == key) {
      return &items[i];
    }
  }
  return nullptr;
}

namespace {

class Parser {
 public:
  explicit Parser(std::string_view s) : s_(s) {}

  Value value() {
    skip_ws();
    if (i_ >= s_.size()) {
      fail("unexpected end of JSON");
    }
    const char c = s_[i_];
    Value v;
    if (c == '{') {
      ++i_;
      v.kind = Value::Kind::kObject;
      skip_ws();
      if (peek('}')) {
        ++i_;
        return v;
      }
      while (true) {
        skip_ws();
        v.keys.push_back(string());
        skip_ws();
        expect(':');
        v.items.push_back(value());
        skip_ws();
        if (peek(',')) {
          ++i_;
          continue;
        }
        expect('}');
        return v;
      }
    }
    if (c == '[') {
      ++i_;
      v.kind = Value::Kind::kArray;
      skip_ws();
      if (peek(']')) {
        ++i_;
        return v;
      }
      while (true) {
        v.items.push_back(value());
        skip_ws();
        if (peek(',')) {
          ++i_;
          continue;
        }
        expect(']');
        return v;
      }
    }
    if (c == '"') {
      v.kind = Value::Kind::kString;
      v.string = string();
      return v;
    }
    if (word("null")) {
      return v;
    }
    if (word("true")) {
      v.kind = Value::Kind::kBool;
      v.boolean = true;
      return v;
    }
    if (word("false")) {
      v.kind = Value::Kind::kBool;
      return v;
    }
    v.kind = Value::Kind::kInt;
    v.integer = integer();
    return v;
  }

  void finish() {
    skip_ws();
    if (i_ != s_.size()) {
      fail("unexpected text after the JSON value");
    }
  }

 private:
  void skip_ws() {
    while (i_ < s_.size() &&
           (s_[i_] == ' ' || s_[i_] == '\n' || s_[i_] == '\r' || s_[i_] == '\t')) {
      ++i_;
    }
  }

  bool peek(char c) const { return i_ < s_.size() && s_[i_] == c; }

  void expect(char c) {
    if (!peek(c)) {
      fail(std::string("expected '") + c + "' in JSON");
    }
    ++i_;
  }

  bool word(std::string_view w) {
    if (s_.compare(i_, w.size(), w) == 0) {
      i_ += w.size();
      return true;
    }
    return false;
  }

  asdl::Int integer() {
    bool negative = false;
    if (peek('-')) {
      negative = true;
      ++i_;
    }
    if (i_ >= s_.size() || s_[i_] < '0' || s_[i_] > '9') {
      fail("expected a JSON value");
    }
    asdl::Int n = 0;
    int digits = 0;
    while (i_ < s_.size() && s_[i_] >= '0' && s_[i_] <= '9') {
      n = n * 10 - (s_[i_] - '0');
      ++i_;
      if (++digits > 38) {
        fail("integer too large for 128 bits");
      }
    }
    if (peek('.') || peek('e') || peek('E')) {
      fail("the capture's JSON holds only integers");
    }
    return negative ? n : -n;
  }

  static int hex(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return -1;
  }

  std::uint32_t hex4() {
    if (i_ + 4 > s_.size()) {
      fail("bad \\u escape in JSON");
    }
    std::uint32_t v = 0;
    for (int k = 0; k < 4; ++k) {
      const int h = hex(s_[i_ + k]);
      if (h < 0) {
        fail("bad \\u escape in JSON");
      }
      v = v * 16 + static_cast<std::uint32_t>(h);
    }
    i_ += 4;
    return v;
  }

  static void put_utf8(std::uint32_t cp, std::string& out) {
    if (cp < 0x80) {
      out += static_cast<char>(cp);
    } else if (cp < 0x800) {
      out += static_cast<char>(0xC0 | (cp >> 6));
      out += static_cast<char>(0x80 | (cp & 0x3F));
    } else if (cp < 0x10000) {
      out += static_cast<char>(0xE0 | (cp >> 12));
      out += static_cast<char>(0x80 | ((cp >> 6) & 0x3F));
      out += static_cast<char>(0x80 | (cp & 0x3F));
    } else {
      out += static_cast<char>(0xF0 | (cp >> 18));
      out += static_cast<char>(0x80 | ((cp >> 12) & 0x3F));
      out += static_cast<char>(0x80 | ((cp >> 6) & 0x3F));
      out += static_cast<char>(0x80 | (cp & 0x3F));
    }
  }

  std::string string() {
    expect('"');
    std::string out;
    while (true) {
      if (i_ >= s_.size()) {
        fail("unterminated JSON string");
      }
      const char c = s_[i_++];
      if (c == '"') {
        return out;
      }
      if (c != '\\') {
        out += c;
        continue;
      }
      if (i_ >= s_.size()) {
        fail("unterminated JSON string");
      }
      const char e = s_[i_++];
      switch (e) {
        case '"': out += '"'; break;
        case '\\': out += '\\'; break;
        case '/': out += '/'; break;
        case 'b': out += '\b'; break;
        case 'f': out += '\f'; break;
        case 'n': out += '\n'; break;
        case 'r': out += '\r'; break;
        case 't': out += '\t'; break;
        case 'u': {
          std::uint32_t cp = hex4();
          // A surrogate pair is one code point. A lone surrogate, which
          // Python's json writes for a lone surrogate in a str, is kept
          // in the three-byte form.
          if (cp >= 0xD800 && cp < 0xDC00 && s_.compare(i_, 2, "\\u") == 0) {
            const std::size_t save = i_;
            i_ += 2;
            const std::uint32_t low = hex4();
            if (low >= 0xDC00 && low < 0xE000) {
              cp = 0x10000 + ((cp - 0xD800) << 10) + (low - 0xDC00);
            } else {
              i_ = save;
            }
          }
          put_utf8(cp, out);
          break;
        }
        default:
          fail("unknown escape in a JSON string");
      }
    }
  }

  [[noreturn]] void fail(const std::string& message) const {
    throw asdl::ParseError("JSON offset " + std::to_string(i_) + ": " + message);
  }

  std::string_view s_;
  std::size_t i_ = 0;
};

}  // namespace

Value parse(std::string_view text) {
  Parser p(text);
  Value v = p.value();
  p.finish();
  return v;
}

std::string quote(std::string_view s) {
  std::string out = "\"";
  const auto escape = [&](unsigned cp) {
    char buf[8];
    std::snprintf(buf, sizeof buf, "\\u%04x", cp);
    out += buf;
  };
  for (std::size_t i = 0; i < s.size();) {
    const auto c = static_cast<unsigned char>(s[i]);
    unsigned cp = c;
    std::size_t len = 1;
    if (c >= 0xF0 && i + 3 < s.size()) {
      cp = ((c & 0x07u) << 18) | ((static_cast<unsigned char>(s[i + 1]) & 0x3Fu) << 12) |
           ((static_cast<unsigned char>(s[i + 2]) & 0x3Fu) << 6) |
           (static_cast<unsigned char>(s[i + 3]) & 0x3Fu);
      len = 4;
    } else if (c >= 0xE0 && i + 2 < s.size()) {
      cp = ((c & 0x0Fu) << 12) | ((static_cast<unsigned char>(s[i + 1]) & 0x3Fu) << 6) |
           (static_cast<unsigned char>(s[i + 2]) & 0x3Fu);
      len = 3;
    } else if (c >= 0xC0 && i + 1 < s.size()) {
      cp = ((c & 0x1Fu) << 6) | (static_cast<unsigned char>(s[i + 1]) & 0x3Fu);
      len = 2;
    }
    i += len;
    switch (cp) {
      case '"':
        out += "\\\"";
        continue;
      case '\\':
        out += "\\\\";
        continue;
      case '\n':
        out += "\\n";
        continue;
      case '\r':
        out += "\\r";
        continue;
      case '\t':
        out += "\\t";
        continue;
      case '\b':
        out += "\\b";
        continue;
      case '\f':
        out += "\\f";
        continue;
      default:
        break;
    }
    if (cp >= 0x20 && cp < 0x7F) {
      out += static_cast<char>(cp);
    } else if (cp > 0xFFFF) {
      // Python writes a character outside the BMP as a surrogate pair.
      const unsigned v = cp - 0x10000;
      escape(0xD800 | (v >> 10));
      escape(0xDC00 | (v & 0x3FF));
    } else {
      escape(cp);
    }
  }
  out += '"';
  return out;
}

namespace {

void dump_to(const Value& v, std::string& out) {
  switch (v.kind) {
    case Value::Kind::kNull:
      out += "null";
      return;
    case Value::Kind::kBool:
      out += v.boolean ? "true" : "false";
      return;
    case Value::Kind::kInt:
      out += asdl::int_to_string(v.integer);
      return;
    case Value::Kind::kString:
      out += quote(v.string);
      return;
    case Value::Kind::kArray:
      out += '[';
      for (std::size_t i = 0; i < v.items.size(); ++i) {
        if (i != 0) out += ", ";
        dump_to(v.items[i], out);
      }
      out += ']';
      return;
    case Value::Kind::kObject:
      out += '{';
      for (std::size_t i = 0; i < v.items.size(); ++i) {
        if (i != 0) out += ", ";
        out += quote(v.keys[i]);
        out += ": ";
        dump_to(v.items[i], out);
      }
      out += '}';
      return;
  }
}

}  // namespace

std::string dump(const Value& v) {
  std::string out;
  dump_to(v, out);
  return out;
}

}  // namespace c6502::json
