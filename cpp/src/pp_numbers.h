// pp_numbers.py: the spelling of integer and floating constants, which
// are preprocessing numbers, digit separators, binary constants, and
// suffixes. The lexer, the parser, and the preprocessor's `#if` all use
// these, so the three accept the same spellings and give the same
// messages.
#pragma once

#include <string>
#include <string_view>
#include <utility>

#include "asdl/runtime.h"
#include "errors.h"

namespace c6502::pp_numbers {

// A pp-number that isn't a valid constant, Python's `NumberError` (a
// `ValueError`). The message names the spelling and what's wrong with
// it. Its callers catch it and raise their own error with its message.
class NumberError : public CompileError {
 public:
  explicit NumberError(const std::string& message) : CompileError("NumberError", message) {}
};

// An integer constant read by `parse_integer`. `value` ignores the
// digit separators. `base` is 2, 8, 10, or 16. `length` is "", "l",
// "ll", or "wb", lowercased, and `is_unsigned` is true when the suffix
// has a `u` or `U`.
//
// Python's int holds any value, and the spelling of a constant may
// have any number of digits. `bits` is the value's exact
// `int.bit_length()`, and `value` holds the value when `bits` is at
// most 127, which every caller checks before it reads `value`, since
// no constant of more than 64 bits has a type. A wider value leaves
// `value` 0.
struct IntegerConstant {
  asdl::Int value = 0;
  int base = 10;
  bool is_unsigned = false;
  std::string length;
  int bits = 0;

  // Returns whether the suffix is `wb` or `uwb`, which give a
  // `_BitInt` constant.
  bool bit_precise() const { return length == "wb"; }

  // Returns whether `value` holds the whole value.
  bool fits() const { return bits <= 127; }

  bool operator==(const IntegerConstant&) const = default;
};

// Returns the pp-number (C23 §6.4.8) that starts at `text[pos]`, or ""
// when none does: the longest run that begins with a digit, or a period
// and a digit, and continues with identifier characters, periods, the
// pairs `e+`, `e-`, `E+`, `E-`, `p+`, `p-`, `P+`, and `P-`, and a single
// quote followed by a digit or a nondigit.
std::string pp_number_at(std::string_view text, std::size_t pos);

// Returns whether the pp-number `spelling` has the form of a floating
// constant: a period, or an exponent (`e` in a decimal number, `p` in a
// hexadecimal one). A binary constant has neither, and its `b` isn't an
// exponent.
bool is_floating(std::string_view spelling);

// Reads the integer constant `spelling`, a pp-number with no period and
// no exponent. Throws NumberError when it isn't a valid integer
// constant.
IntegerConstant parse_integer(std::string_view spelling);

// Checks the floating constant `spelling`, a pp-number with a period or
// an exponent, and returns the constant without its digit separators
// and suffix, and the suffix. Throws NumberError when it isn't a valid
// floating constant (C23 §6.4.4.3p1).
std::pair<std::string, std::string> check_floating(std::string_view spelling);

// Returns the width N of the type of `constant`, whose suffix is `wb`
// or `uwb`: the `_BitInt(N)` or `unsigned _BitInt(N)` of the smallest N
// that holds its value, with a sign bit for `wb` (C23 §6.4.4.2p6).
// Throws NumberError when N is greater than BITINT_MAXWIDTH, since the
// constant then has no type (§6.4.4.2p7).
int bit_precise_width(const IntegerConstant& constant, std::string_view spelling);

// Throws NumberError when the pp-number `spelling` isn't a valid
// integer or floating constant.
void check(std::string_view spelling);

}  // namespace c6502::pp_numbers
