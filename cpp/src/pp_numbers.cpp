/*
The spelling of integer and floating constants: preprocessing numbers,
digit separators, binary constants, and suffixes (pp_numbers.py).

C23 §6.4.8 defines a *preprocessing number* (pp-number) as the longest
run that begins with a digit, or a period and a digit, and continues
with identifier characters, periods, the pairs `e+`, `e-`, `E+`, `E-`,
`p+`, `p-`, `P+`, and `P-`, and a single quote followed by a digit or a
nondigit (a letter or `_`). Every integer and floating constant is a
pp-number, and a pp-number that isn't one of them is an error once it
reaches translation phase 7.

C23 §6.4.4.2 adds two things to the spelling of a constant.

  * A *binary constant* is `0b` or `0B` followed by the digits 0 and 1.
    It's typed by the same list as octal and hexadecimal constants (the
    table in §6.4.4.2p6).
  * A *digit separator* is a single quote between two digits of an
    integer or floating constant, as in `1'000'000` or `0xFF'FF`.
    Separators are ignored for the value. The grammar puts one only
    between two digits of the same digit sequence, so a separator can't
    come first or last, can't be doubled, and can't touch the prefix
    (`0x'FF`), the period (`1'.5`, `1.'5`), the exponent's letter or
    sign (`1'e5`, `1e'5`), or the suffix (`1'u`). An octal constant's
    leading 0 is one of its digits, so `0'17` is octal 017.

`pp_number_at` finds a pp-number in a source text, and `parse_integer`
and `check_floating` read one, throwing `NumberError` with a message
that names what's wrong.

The suffix grammar includes C23's `wb` and `uwb` for `_BitInt`
constants. `parse_integer` reports such a suffix in
`IntegerConstant::bit_precise`, and `bit_precise_width` gives the width
of the constant's type, or an error when that's wider than
BITINT_MAXWIDTH.

The Python module tests membership with `in` on strings, where a
character such as `before` may be the empty string at either end of the
spelling, and `"" in "+-"` is true. `py_in` keeps that meaning, so the
messages come out as Python's do for every position.
*/
#include "pp_numbers.h"

#include <cstdint>
#include <vector>

#include "bit_precise.h"
#include "pyrepr.h"

namespace c6502::pp_numbers {

namespace {

bool is_digit(char c) { return c >= '0' && c <= '9'; }

bool is_ident_char(char c) {
  return is_digit(c) || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_';
}

// Returns whether `needle` is a substring of `hay`, as Python's
// `needle in hay` does for two strings, so the empty string is in every
// string.
bool py_in(std::string_view needle, std::string_view hay) {
  return hay.find(needle) != std::string_view::npos;
}

// Returns the one-character string at `s[i]`, or "" when `i` is out of
// range, as Python's `s[i] if ... else ""` gives the neighbours of a
// separator.
std::string_view char_at(std::string_view s, std::size_t i) {
  return i < s.size() ? s.substr(i, 1) : std::string_view{};
}

// The digits of each base. An octal constant's digits include its
// leading 0.
std::string_view digits_of(int base) {
  switch (base) {
    case 2:
      return "01";
    case 8:
      return "01234567";
    case 10:
      return "0123456789";
    default:
      return "0123456789abcdefABCDEF";
  }
}

std::string_view base_name(int base) {
  switch (base) {
    case 2:
      return "binary";
    case 8:
      return "octal";
    case 10:
      return "decimal";
    default:
      return "hexadecimal";
  }
}

bool is_base_digit(char c, int base) {
  return digits_of(base).find(c) != std::string_view::npos;
}

bool prefix_is(std::string_view s, std::string_view a, std::string_view b) {
  const std::string_view head = s.substr(0, 2);
  return head == a || head == b;
}

// Returns whether `spelling[i]` is a digit of a hexadecimal number's
// digit sequence, where `e` is a digit, rather than an exponent letter.
bool is_hex_digit_here(std::string_view spelling, std::size_t i) {
  if (!prefix_is(spelling, "0x", "0X")) {
    return false;
  }
  if (i < spelling.size() && (spelling[i] == 'p' || spelling[i] == 'P')) {
    return false;
  }
  const std::string_view rest = i > 2 ? spelling.substr(2, i - 2) : std::string_view{};
  return rest.find_first_of("pP") == std::string_view::npos;
}

// Returns the message for the digit at `spelling[i]`, which the
// number's base doesn't have.
std::string bad_digit(std::string_view spelling, std::size_t i) {
  const int base = prefix_is(spelling, "0b", "0B") ? 2 : 8;
  return "invalid digit " + pyrepr::string(char_at(spelling, i)) + " in " +
         std::string(base_name(base)) + " constant " + pyrepr::string(spelling);
}

// Returns the message for the digit separator at `spelling[i]`, which
// isn't between two digits of one digit sequence.
std::string misplaced(std::string_view spelling, std::size_t i) {
  const std::string_view before = i > 0 ? char_at(spelling, i - 1) : std::string_view{};
  const std::string_view after = char_at(spelling, i + 1);
  std::string why;
  if (after.empty()) {
    why = "ends the number";
  } else if (after == "'") {
    why = "is doubled";
  } else if (py_in(before, "xXbB") && i == 2 && spelling[0] == '0') {
    why = "comes right after the prefix 0" + std::string(before);
  } else if (before == "." || after == ".") {
    why = "touches the period";
  } else if (py_in(before, "+-") ||
             (py_in(before, "eEpP") && !is_hex_digit_here(spelling, i - 1))) {
    why = "comes right after the exponent's letter or sign";
  } else if (py_in(after, "eEpP") && !is_hex_digit_here(spelling, i + 1)) {
    why = "comes right before the exponent";
  } else if (is_digit(after[0])) {
    // `0'8` or `0b1'2`: a digit that the base doesn't have.
    return bad_digit(spelling, i + 1);
  } else {
    why = "comes right before the suffix";
  }
  return "a digit separator has to be between two digits, and the one in " +
         pyrepr::string(spelling) + " " + why;
}

// Reads the digit sequence of `base` that starts at `spelling[i]`, with
// digit separators between its digits, and returns the index after it.
// Throws NumberError for a separator that isn't between two digits of
// the sequence. The sequence may be empty.
std::size_t scan_digits(std::string_view spelling, std::size_t i, int base) {
  const std::size_t n = spelling.size();
  while (i < n) {
    const char c = spelling[i];
    if (is_base_digit(c, base)) {
      ++i;
      continue;
    }
    if (c != '\'') {
      return i;
    }
    if (i == 0 || !is_base_digit(spelling[i - 1], base)) {
      throw NumberError(misplaced(spelling, i));
    }
    if (i + 1 < n && is_base_digit(spelling[i + 1], base)) {
      ++i;
      continue;
    }
    throw NumberError(misplaced(spelling, i));
  }
  return i;
}

// The value of a digit string in `base`, which may have any number of
// digits, as Python's `int(digits, base)` reads it: the exact bit
// length, and the value itself when it fits in 127 bits.
void read_value(std::string_view digits, int base, IntegerConstant& out) {
  // Little-endian limbs of 32 bits.
  std::vector<std::uint32_t> limbs;
  for (const char c : digits) {
    std::uint32_t d;
    if (is_digit(c)) {
      d = static_cast<std::uint32_t>(c - '0');
    } else if (c >= 'a' && c <= 'f') {
      d = static_cast<std::uint32_t>(c - 'a' + 10);
    } else {
      d = static_cast<std::uint32_t>(c - 'A' + 10);
    }
    std::uint64_t carry = d;
    for (auto& limb : limbs) {
      const std::uint64_t t = static_cast<std::uint64_t>(limb) * static_cast<std::uint64_t>(base) + carry;
      limb = static_cast<std::uint32_t>(t);
      carry = t >> 32;
    }
    if (carry != 0) {
      limbs.push_back(static_cast<std::uint32_t>(carry));
    }
  }
  while (!limbs.empty() && limbs.back() == 0) {
    limbs.pop_back();
  }
  int bits = 0;
  if (!limbs.empty()) {
    bits = static_cast<int>(32 * (limbs.size() - 1));
    for (std::uint32_t top = limbs.back(); top != 0; top >>= 1) {
      ++bits;
    }
  }
  out.bits = bits;
  out.value = 0;
  if (bits <= 127) {
    asdl::Int v = 0;
    for (std::size_t k = limbs.size(); k-- > 0;) {
      v = v * asdl::Int(4294967296LL) + asdl::Int(static_cast<std::int64_t>(limbs[k]));
    }
    out.value = v;
  }
}

// Matches the integer suffix (C23 §6.4.4.2p1) as Python's fullmatch of
// `_INTEGER_SUFFIX_RE` does: `u` or `U` with or without a length, and a
// length with or without `u` or `U` after it. The lengths are `l`, `L`,
// `ll`, `LL`, `wb`, and `WB`, so `lL` and `wB` aren't suffixes. Sets
// the lowercased length and whether the suffix is unsigned.
bool match_integer_suffix(std::string_view suffix, std::string& length, bool& is_unsigned) {
  static constexpr std::string_view kLengths[] = {"ll", "LL", "l", "L", "wb", "WB"};
  auto lower = [](std::string_view s) {
    std::string out(s);
    for (char& c : out) {
      if (c >= 'A' && c <= 'Z') {
        c = static_cast<char>(c - 'A' + 'a');
      }
    }
    return out;
  };
  length.clear();
  is_unsigned = false;
  if (suffix.empty()) {
    return true;
  }
  if (suffix[0] == 'u' || suffix[0] == 'U') {
    const std::string_view rest = suffix.substr(1);
    if (rest.empty()) {
      is_unsigned = true;
      return true;
    }
    for (const auto len : kLengths) {
      if (rest == len) {
        is_unsigned = true;
        length = lower(len);
        return true;
      }
    }
    return false;
  }
  for (const auto len : kLengths) {
    if (suffix.substr(0, len.size()) != len) {
      continue;
    }
    const std::string_view rest = suffix.substr(len.size());
    if (rest.empty() || rest == "u" || rest == "U") {
      length = lower(len);
      is_unsigned = !rest.empty();
      return true;
    }
  }
  return false;
}

}  // namespace

std::string pp_number_at(std::string_view text, std::size_t pos) {
  const std::size_t n = text.size();
  std::size_t i = pos;
  // `\.?[0-9]`: the period is taken only when a digit follows it.
  if (i < n && text[i] == '.' && i + 1 < n && is_digit(text[i + 1])) {
    i += 2;
  } else if (i < n && is_digit(text[i])) {
    i += 1;
  } else {
    return "";
  }
  // `(?:[eEpP][+-]|'[0-9A-Za-z_]|[0-9A-Za-z_.])*`, in the order of the
  // alternatives, so the `+` or `-` after an `e` is taken with it.
  while (i < n) {
    const char c = text[i];
    if ((c == 'e' || c == 'E' || c == 'p' || c == 'P') && i + 1 < n &&
        (text[i + 1] == '+' || text[i + 1] == '-')) {
      i += 2;
    } else if (c == '\'' && i + 1 < n && is_ident_char(text[i + 1])) {
      i += 2;
    } else if (is_ident_char(c) || c == '.') {
      i += 1;
    } else {
      break;
    }
  }
  return std::string(text.substr(pos, i - pos));
}

bool is_floating(std::string_view spelling) {
  if (prefix_is(spelling, "0b", "0B")) {
    return false;
  }
  if (spelling.find('.') != std::string_view::npos) {
    return true;
  }
  if (prefix_is(spelling, "0x", "0X")) {
    return spelling.find_first_of("pP") != std::string_view::npos;
  }
  return spelling.find_first_of("eE") != std::string_view::npos;
}

IntegerConstant parse_integer(std::string_view spelling) {
  const std::size_t n = spelling.size();
  int base;
  std::size_t i;
  if (prefix_is(spelling, "0x", "0X")) {
    base = 16;
    i = 2;
  } else if (prefix_is(spelling, "0b", "0B")) {
    base = 2;
    i = 2;
  } else if (spelling.substr(0, 1) == "0") {
    base = 8;
    i = 0;
  } else {
    base = 10;
    i = 0;
  }
  if ((base == 2 || base == 16) && (i == n || !is_base_digit(spelling[i], base))) {
    if (i < n && spelling[i] == '\'') {
      throw NumberError(misplaced(spelling, i));
    }
    if (i < n && is_digit(spelling[i])) {
      throw NumberError(bad_digit(spelling, i));
    }
    throw NumberError("no digits after the prefix " + std::string(spelling.substr(0, 2)) +
                      " in " + pyrepr::string(spelling));
  }
  const std::size_t end = scan_digits(spelling, i, base);
  if (end < n && is_digit(spelling[end])) {
    throw NumberError(bad_digit(spelling, end));
  }
  std::string digits;
  for (const char c : spelling.substr(i, end - i)) {
    if (c != '\'') {
      digits += c;
    }
  }
  const std::string_view suffix = spelling.substr(end);
  if (base == 2 && suffix.find('.') != std::string_view::npos) {
    throw NumberError("a binary constant is an integer, so " + pyrepr::string(spelling) +
                      " can't have a period");
  }
  IntegerConstant out;
  if (!match_integer_suffix(suffix, out.length, out.is_unsigned)) {
    throw NumberError("invalid suffix " + pyrepr::string(suffix) + " on integer constant " +
                      pyrepr::string(spelling));
  }
  if (digits.empty()) {
    // Python's `int("", base)` raises ValueError here, which no
    // pp-number without a period or an exponent reaches.
    throw InternalError("parse_integer: no digits in " + pyrepr::string(spelling));
  }
  out.base = base;
  read_value(digits, base, out);
  return out;
}

std::pair<std::string, std::string> check_floating(std::string_view spelling) {
  const std::size_t n = spelling.size();
  const bool hexadecimal = prefix_is(spelling, "0x", "0X");
  const int base = hexadecimal ? 16 : 10;
  std::size_t i = hexadecimal ? 2 : 0;
  if (hexadecimal && i < n && spelling[i] == '\'') {
    throw NumberError(misplaced(spelling, i));
  }
  const std::size_t whole_end = scan_digits(spelling, i, base);
  bool has_digits = whole_end > i;
  i = whole_end;
  if (i < n && spelling[i] == '.') {
    ++i;
    if (i < n && spelling[i] == '\'') {
      throw NumberError(misplaced(spelling, i));
    }
    const std::size_t frac_end = scan_digits(spelling, i, base);
    has_digits = has_digits || frac_end > i;
    i = frac_end;
  }
  if (!has_digits) {
    throw NumberError("floating constant " + pyrepr::string(spelling) +
                      " has no digits before or after the period");
  }
  const std::string_view exponent_letters = hexadecimal ? "pP" : "eE";
  if (i < n && exponent_letters.find(spelling[i]) != std::string_view::npos) {
    ++i;
    if (i < n && (spelling[i] == '+' || spelling[i] == '-')) {
      ++i;
    }
    if (i < n && spelling[i] == '\'') {
      throw NumberError(misplaced(spelling, i));
    }
    const std::size_t exp_end = scan_digits(spelling, i, 10);
    if (exp_end == i) {
      throw NumberError("the exponent of " + pyrepr::string(spelling) + " has no digits");
    }
    i = exp_end;
  } else if (hexadecimal) {
    throw NumberError("hexadecimal floating constant " + pyrepr::string(spelling) +
                      " has no binary exponent (a p and a power of 2)");
  }
  const std::string_view body = spelling.substr(0, i);
  const std::string_view suffix = spelling.substr(i);
  if (suffix == "df" || suffix == "dd" || suffix == "dl" || suffix == "DF" || suffix == "DD" ||
      suffix == "DL") {
    throw NumberError("the suffix " + pyrepr::string(suffix) + " of " + pyrepr::string(spelling) +
                      " gives a decimal floating type, which c6502 doesn't have");
  }
  if (!(suffix.empty() || suffix == "f" || suffix == "F" || suffix == "l" || suffix == "L")) {
    throw NumberError("invalid suffix " + pyrepr::string(suffix) + " on floating constant " +
                      pyrepr::string(spelling));
  }
  std::string stripped;
  for (const char c : body) {
    if (c != '\'') {
      stripped += c;
    }
  }
  return {stripped, std::string(suffix)};
}

int bit_precise_width(const IntegerConstant& constant, std::string_view spelling) {
  const int width = bit_precise::constant_width(constant.bits, !constant.is_unsigned);
  if (width > bit_precise::kBitIntMaxWidth) {
    const std::string kind = constant.is_unsigned ? "unsigned _BitInt" : "_BitInt";
    throw NumberError("integer constant " + pyrepr::string(spelling) + " needs a " + kind + "(" +
                      std::to_string(width) + "), and BITINT_MAXWIDTH is " +
                      std::to_string(bit_precise::kBitIntMaxWidth) + " (C23 §6.4.4.2p6)");
  }
  return width;
}

void check(std::string_view spelling) {
  if (is_floating(spelling)) {
    check_floating(spelling);
  } else {
    parse_integer(spelling);
  }
}

}  // namespace c6502::pp_numbers
