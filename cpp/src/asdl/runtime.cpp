#include "asdl/runtime.h"

#include <algorithm>
#include <string>

namespace c6502::asdl {

std::string int_to_string(Int v) {
  if (v == 0) {
    return "0";
  }
  const bool negative = v < 0;
  // Work on the negative value, whose range includes the most negative
  // 128-bit integer.
  Int n = negative ? v : -v;
  std::string digits;
  while (n != 0) {
    const int digit = static_cast<int>(-(n % 10));
    digits += static_cast<char>('0' + digit);
    n /= 10;
  }
  if (negative) {
    digits += '-';
  }
  std::reverse(digits.begin(), digits.end());
  return digits;
}

Int floor_div(Int a, Int b) {
  Int q = a / b;
  if ((a % b != 0) && ((a < 0) != (b < 0))) {
    q -= 1;
  }
  return q;
}

Int floor_mod(Int a, Int b) {
  Int m = a % b;
  if (m != 0 && ((m < 0) != (b < 0))) {
    m += b;
  }
  return m;
}

// --- Writer ---

void Writer::write_int(Int v) { out_ += int_to_string(v); }

namespace {

const char kHex[] = "0123456789abcdef";

// Decodes the UTF-8 code point at `s[i]`, advancing `i`. A surrogate
// code point, which Python's `str` can hold, is taken in the three-byte
// form any other code point of its size has.
std::uint32_t decode_utf8(std::string_view s, std::size_t& i) {
  const auto byte = [&](std::size_t k) {
    return static_cast<std::uint32_t>(static_cast<unsigned char>(s[k]));
  };
  const std::uint32_t b0 = byte(i);
  int extra;
  std::uint32_t cp;
  if (b0 < 0x80) {
    i += 1;
    return b0;
  } else if ((b0 & 0xE0) == 0xC0) {
    extra = 1;
    cp = b0 & 0x1F;
  } else if ((b0 & 0xF0) == 0xE0) {
    extra = 2;
    cp = b0 & 0x0F;
  } else if ((b0 & 0xF8) == 0xF0) {
    extra = 3;
    cp = b0 & 0x07;
  } else {
    throw std::runtime_error("invalid UTF-8 in an IR string");
  }
  if (i + extra >= s.size()) {
    throw std::runtime_error("truncated UTF-8 in an IR string");
  }
  for (int k = 1; k <= extra; ++k) {
    const std::uint32_t b = byte(i + k);
    if ((b & 0xC0) != 0x80) {
      throw std::runtime_error("invalid UTF-8 in an IR string");
    }
    cp = (cp << 6) | (b & 0x3F);
  }
  i += extra + 1;
  return cp;
}

void encode_utf8(std::uint32_t cp, std::string& out) {
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

}  // namespace

void Writer::write_string(std::string_view s) {
  out_ += '"';
  std::size_t i = 0;
  while (i < s.size()) {
    const std::uint32_t cp = decode_utf8(s, i);
    if (cp == '"' || cp == '\\') {
      out_ += '\\';
      out_ += static_cast<char>(cp);
    } else if (cp >= 0x20 && cp < 0x7F) {
      out_ += static_cast<char>(cp);
    } else if (cp < 0x100) {
      out_ += "\\x";
      out_ += kHex[cp >> 4];
      out_ += kHex[cp & 0xF];
    } else {
      out_ += "\\u{";
      std::string hex;
      for (std::uint32_t n = cp; n != 0; n >>= 4) {
        hex += kHex[n & 0xF];
      }
      std::reverse(hex.begin(), hex.end());
      out_ += hex;
      out_ += '}';
    }
  }
  out_ += '"';
}

void Writer::newline() {
  out_ += '\n';
  out_.append(static_cast<std::size_t>(2 * depth_), ' ');
}

void Writer::begin_seq(bool lines) {
  out_ += '[';
  if (lines) {
    ++depth_;
  } else {
    first_ = true;
  }
}

void Writer::seq_item(bool lines) {
  if (lines) {
    newline();
  } else {
    if (!first_) {
      out_ += ' ';
    }
    first_ = false;
  }
}

void Writer::end_seq(bool lines, bool empty) {
  if (lines) {
    --depth_;
    if (!empty) {
      newline();
    }
  }
  out_ += ']';
  // An element of an enclosing one-line sequence may follow, and it
  // needs its separating space.
  first_ = false;
}

// --- Reader ---

void Reader::skip_ws() {
  while (i_ < s_.size()) {
    const char c = s_[i_];
    if (c == ' ' || c == '\n' || c == '\r' || c == '\t') {
      ++i_;
    } else {
      return;
    }
  }
}

void Reader::error(const std::string& message) const {
  std::size_t line = 1;
  std::size_t col = 1;
  for (std::size_t k = 0; k < i_ && k < s_.size(); ++k) {
    if (s_[k] == '\n') {
      ++line;
      col = 1;
    } else {
      ++col;
    }
  }
  throw ParseError(std::to_string(line) + ":" + std::to_string(col) + ": " +
                   message);
}

void Reader::expect(char c) {
  skip_ws();
  if (i_ >= s_.size() || s_[i_] != c) {
    error(std::string("expected '") + c + "'");
  }
  ++i_;
}

namespace {

bool is_name_start(char c) {
  return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_';
}

bool is_name_char(char c) { return is_name_start(c) || (c >= '0' && c <= '9'); }

}  // namespace

std::string_view Reader::read_name() {
  skip_ws();
  const std::size_t start = i_;
  if (i_ >= s_.size() || !is_name_start(s_[i_])) {
    error("expected a name");
  }
  while (i_ < s_.size() && is_name_char(s_[i_])) {
    ++i_;
  }
  return s_.substr(start, i_ - start);
}

void Reader::expect_name(std::string_view name) {
  const std::size_t start = i_;
  const std::string_view got = read_name();
  if (got != name) {
    i_ = start;
    skip_ws();
    error("expected " + std::string(name) + ", got " + std::string(got));
  }
}

Int Reader::read_int() {
  skip_ws();
  bool negative = false;
  if (i_ < s_.size() && s_[i_] == '-') {
    negative = true;
    ++i_;
  }
  if (i_ >= s_.size() || s_[i_] < '0' || s_[i_] > '9') {
    error("expected an integer");
  }
  // Accumulate the negative value, whose range includes the most
  // negative 128-bit integer.
  Int n = 0;
  int digits = 0;
  while (i_ < s_.size() && s_[i_] >= '0' && s_[i_] <= '9') {
    n = n * 10 - (s_[i_] - '0');
    ++i_;
    if (++digits > 38) {
      error("integer too large for 128 bits");
    }
  }
  if (!negative) {
    if (n == -n && n != 0) {
      error("integer too large for 128 bits");
    }
    n = -n;
  }
  return n;
}

bool Reader::read_bool() {
  const std::string_view name = read_name();
  if (name == "true") {
    return true;
  }
  if (name == "false") {
    return false;
  }
  error("expected true or false, got " + std::string(name));
}

namespace {

int hex_value(char c) {
  if (c >= '0' && c <= '9') return c - '0';
  if (c >= 'a' && c <= 'f') return c - 'a' + 10;
  if (c >= 'A' && c <= 'F') return c - 'A' + 10;
  return -1;
}

}  // namespace

std::string Reader::read_string() {
  expect('"');
  std::string out;
  while (true) {
    if (i_ >= s_.size()) {
      error("unterminated string");
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
      error("unterminated string");
    }
    const char e = s_[i_++];
    if (e == '"' || e == '\\') {
      out += e;
    } else if (e == 'x') {
      if (i_ + 2 > s_.size() || hex_value(s_[i_]) < 0 ||
          hex_value(s_[i_ + 1]) < 0) {
        error("bad \\x escape");
      }
      const auto cp = static_cast<std::uint32_t>(hex_value(s_[i_]) * 16 +
                                                 hex_value(s_[i_ + 1]));
      i_ += 2;
      encode_utf8(cp, out);
    } else if (e == 'u') {
      if (i_ >= s_.size() || s_[i_] != '{') {
        error("bad \\u escape");
      }
      ++i_;
      std::uint32_t cp = 0;
      int digits = 0;
      while (i_ < s_.size() && s_[i_] != '}') {
        const int h = hex_value(s_[i_]);
        if (h < 0 || ++digits > 6) {
          error("bad \\u escape");
        }
        cp = cp * 16 + static_cast<std::uint32_t>(h);
        ++i_;
      }
      if (i_ >= s_.size() || digits == 0 || cp > 0x10FFFF) {
        error("bad \\u escape");
      }
      ++i_;
      encode_utf8(cp, out);
    } else {
      error("unknown escape in a string");
    }
  }
}

bool Reader::read_none() {
  skip_ws();
  if (s_.compare(i_, 4, "None") == 0 &&
      (i_ + 4 >= s_.size() || !is_name_char(s_[i_ + 4]))) {
    i_ += 4;
    return true;
  }
  return false;
}

bool Reader::end_seq() {
  skip_ws();
  if (i_ < s_.size() && s_[i_] == ']') {
    ++i_;
    return true;
  }
  return false;
}

void Reader::expect_end() {
  skip_ws();
  if (i_ != s_.size()) {
    error("unexpected text after the value");
  }
}

void Reader::unknown_constructor(std::string_view name,
                                 std::string_view type) {
  error("unknown constructor " + std::string(name) + " of " +
        std::string(type));
}

}  // namespace c6502::asdl
