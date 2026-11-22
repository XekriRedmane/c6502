#include "py_int.h"

#include <algorithm>
#include <iterator>

#include "errors.h"
#include "pcpp_text.h"
#include "pcpp_unicode.h"

namespace c6502::py_int {

Int max_value() {
  // 2**127 - 1, built without an intermediate that overflows.
  return ((Int(1) << 126) - 1) * 2 + 1;
}

Int min_value() { return -max_value() - 1; }

void too_wide(std::string_view where) {
  throw CompileError("OverflowError",
                     std::string(where) +
                         ": the value takes more than 128 bits, the width of the C++ port's "
                         "integers, where Python's integers have none");
}

int decimal_value(std::uint32_t cp) {
  // Unicode gives every decimal digit a run of ten code points, 0 to 9
  // in order, and the table's ranges join adjacent runs, so a digit's
  // value is its distance from the start of its range, modulo 10.
  const auto* begin = std::begin(pcpp::unicode::kDigit);
  const auto* end = std::end(pcpp::unicode::kDigit);
  const auto* it = std::upper_bound(
      begin, end, cp, [](std::uint32_t c, const pcpp::unicode::Range& r) { return c < r.first; });
  if (it == begin) return -1;
  --it;
  if (cp > it->last) return -1;
  return static_cast<int>((cp - it->first) % 10);
}

namespace {

bool ascii_space(char c) {
  return c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r';
}

// Returns the value of the ASCII digit or letter `c` in bases up to 36,
// as `_PyLong_DigitValue` gives it, or 37 for any other character.
int digit_value(char c) {
  if (c >= '0' && c <= '9') return c - '0';
  if (c >= 'a' && c <= 'z') return c - 'a' + 10;
  if (c >= 'A' && c <= 'Z') return c - 'A' + 10;
  return 37;
}

// The longest repr Python's message shows (`%.200R`).
constexpr std::size_t kReprLimit = 200;

// Python's limit on the decimal digits of a conversion between `int` and
// `str` (`sys.get_int_max_str_digits()`).
constexpr std::size_t kMaxStrDigits = 4300;

[[noreturn]] void invalid(std::string_view text, int base) {
  const std::string repr = pcpp::py_repr(text);
  std::string shown;
  std::size_t count = 0;
  for (std::size_t i = 0; i < repr.size() && count < kReprLimit; ++count) {
    std::size_t len = 0;
    pcpp::decode_at(repr, i, len);
    shown.append(repr, i, len);
    i += len;
  }
  throw CompileError("ValueError",
                     "invalid literal for int() with base " + std::to_string(base) + ": " + shown);
}

}  // namespace

Int parse(std::string_view text, int base, std::string_view where) {
  // `_PyUnicode_TransformDecimalAndSpaceToASCII`: a code point below 127
  // stays, a space becomes ' ', a decimal digit its ASCII digit, and
  // anything else ends the text with a '?', which no number can hold.
  std::string a;
  for (std::size_t i = 0; i < text.size();) {
    std::size_t len = 0;
    const std::uint32_t cp = pcpp::decode_at(text, i, len);
    i += len;
    if (cp < 127) {
      a += static_cast<char>(cp);
    } else if (pcpp::is_space(cp)) {
      a += ' ';
    } else if (const int d = decimal_value(cp); d >= 0) {
      a += static_cast<char>('0' + d);
    } else {
      a += '?';
      break;
    }
  }
  // `PyLong_FromString`.
  std::size_t i = 0;
  const std::size_t n = a.size();
  while (i < n && ascii_space(a[i])) ++i;
  bool negative = false;
  if (i < n && (a[i] == '+' || a[i] == '-')) {
    negative = a[i] == '-';
    ++i;
  }
  const char prefix = base == 16 ? 'x' : base == 2 ? 'b' : '\0';
  if (prefix != '\0' && i + 1 < n && a[i] == '0' &&
      (a[i + 1] == prefix || a[i + 1] == prefix - 'a' + 'A')) {
    i += 2;
    // One underscore may follow the prefix.
    if (i < n && a[i] == '_') ++i;
  }
  if (i < n && a[i] == '_') invalid(text, base);
  const std::size_t start = i;
  std::size_t digits = 0;
  char prev = '\0';
  while (i < n && (digit_value(a[i]) < base || a[i] == '_')) {
    if (a[i] == '_') {
      if (prev == '_') invalid(text, base);
    } else {
      ++digits;
    }
    prev = a[i];
    ++i;
  }
  if (prev == '_') invalid(text, base);
  if (base == 10 && digits > kMaxStrDigits) {
    throw CompileError("ValueError", "Exceeds the limit (4300 digits) for integer string "
                                     "conversion: value has " +
                                         std::to_string(digits) +
                                         " digits; use sys.set_int_max_str_digits() to "
                                         "increase the limit");
  }
  const std::size_t stop = i;
  if (stop == start) invalid(text, base);
  while (i < n && ascii_space(a[i])) ++i;
  if (i != n) invalid(text, base);
  // The value, accumulated as a negative number so that the most
  // negative `Int` can be read.
  Int v = 0;
  const Int low = min_value();
  for (std::size_t k = start; k < stop; ++k) {
    if (a[k] == '_') continue;
    const int d = digit_value(a[k]);
    if (v < (low + d) / base) too_wide(where);
    v = v * base - d;
  }
  if (!negative) {
    if (v == low) too_wide(where);
    v = -v;
  }
  return v;
}

Int add(Int a, Int b, std::string_view where) {
  if ((b > 0 && a > max_value() - b) || (b < 0 && a < min_value() - b)) too_wide(where);
  return a + b;
}

Int sub(Int a, Int b, std::string_view where) {
  if ((b < 0 && a > max_value() + b) || (b > 0 && a < min_value() + b)) too_wide(where);
  return a - b;
}

Int mul(Int a, Int b, std::string_view where) {
  if (a == 0 || b == 0) return 0;
  const Int hi = max_value();
  const Int lo = min_value();
  bool over = false;
  if (a > 0) {
    over = b > 0 ? a > hi / b : b < lo / a;
  } else {
    over = b > 0 ? a < lo / b : b < hi / a;
  }
  if (over) too_wide(where);
  return a * b;
}

Int neg(Int a, std::string_view where) {
  if (a == min_value()) too_wide(where);
  return -a;
}

Int floor_div(Int a, Int b, std::string_view where) {
  if (a == min_value() && b == -1) too_wide(where);
  return asdl::floor_div(a, b);
}

Int lshift(Int a, Int b, std::string_view where) {
  if (b < 0) throw CompileError("ValueError", "negative shift count");
  if (a == 0) return 0;
  // A nonzero value shifted by 128 or more takes more than 128 bits.
  if (b >= 128) too_wide(where);
  for (int k = 0; k < static_cast<int>(b); ++k) a = mul(a, 2, where);
  return a;
}

Int rshift(Int a, Int b) {
  if (b < 0) throw CompileError("ValueError", "negative shift count");
  if (b >= 127) return a < 0 ? Int(-1) : Int(0);
  return asdl::floor_div(a, Int(1) << static_cast<int>(b));
}

std::int64_t to_int64(Int v, std::string_view where) {
  if (v > Int(INT64_MAX) || v < Int(INT64_MIN)) {
    throw CompileError("OverflowError",
                       std::string(where) +
                           ": the value takes more than the 64 bits of the C++ port's ELF "
                           "fields, where Python's integers have no width");
  }
  return static_cast<std::int64_t>(v);
}

}  // namespace c6502::py_int
