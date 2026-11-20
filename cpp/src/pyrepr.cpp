#include "pyrepr.h"

#include <algorithm>

namespace c6502::pyrepr {

std::string string(std::string_view s) {
  const bool has_single = s.find('\'') != std::string_view::npos;
  const bool has_double = s.find('"') != std::string_view::npos;
  const char quote = (has_single && !has_double) ? '"' : '\'';
  static constexpr char kDigits[] = "0123456789abcdef";
  std::string out(1, quote);
  for (const char ch : s) {
    const auto c = static_cast<unsigned char>(ch);
    if (ch == quote || ch == '\\') {
      out += '\\';
      out += ch;
    } else if (ch == '\t') {
      out += "\\t";
    } else if (ch == '\n') {
      out += "\\n";
    } else if (ch == '\r') {
      out += "\\r";
    } else if (c < 0x20 || c == 0x7F) {
      out += "\\x";
      out += kDigits[c >> 4];
      out += kDigits[c & 0xF];
    } else {
      out += ch;
    }
  }
  out += quote;
  return out;
}

std::string integer(asdl::Int v) { return asdl::int_to_string(v); }

std::string hex(asdl::Int v, int width) {
  static constexpr char kDigits[] = "0123456789ABCDEF";
  const bool negative = v < 0;
  if (negative) {
    v = -v;
    --width;
  }
  std::string out;
  if (v == 0) {
    out += '0';
  }
  while (v > 0) {
    out += kDigits[static_cast<int>(v % 16)];
    v /= 16;
  }
  while (static_cast<int>(out.size()) < width) {
    out += '0';
  }
  if (negative) {
    out += '-';
  }
  std::reverse(out.begin(), out.end());
  return out;
}

std::string signed_integer(asdl::Int v) {
  // Appending to the sign, where `"+" + s` would do, keeps GCC 12 from a
  // false -Wrestrict warning.
  std::string out = v < 0 ? "" : "+";
  out += asdl::int_to_string(v);
  return out;
}

std::vector<std::uint32_t> code_points(std::string_view s) {
  std::vector<std::uint32_t> out;
  out.reserve(s.size());
  for (std::size_t i = 0; i < s.size();) {
    const auto c = static_cast<unsigned char>(s[i]);
    int extra = 0;
    std::uint32_t cp = c;
    if (c >= 0xF0) {
      extra = 3;
      cp = c & 0x07;
    } else if (c >= 0xE0) {
      extra = 2;
      cp = c & 0x0F;
    } else if (c >= 0xC0) {
      extra = 1;
      cp = c & 0x1F;
    }
    ++i;
    for (int k = 0; k < extra && i < s.size(); ++k, ++i) {
      cp = (cp << 6) | (static_cast<unsigned char>(s[i]) & 0x3F);
    }
    out.push_back(cp);
  }
  return out;
}

}  // namespace c6502::pyrepr
