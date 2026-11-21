/* IEEE 754 bit-pattern adapter, the port of fp_arith.py.

c6502 carries floating-point values as their IEEE 754 bit patterns,
never as host floats, at every layer from the c99 AST through asm
emission. The Python module goes through numpy, which converts at the
target precision for every IEEE 754 case (subnormals, an overflow to an
infinity, NaNs, signed zeros) and keeps Python's `float()` from raising
on an overflow. This port reproduces numpy's results with the host's
`float` and `double`, which on x86-64 are the SSE unit's, as numpy's
are. Two of numpy's conversions are worth knowing about.

- `np.float32(s)` of a string parses it as a double and then rounds the
  double to a single, so a decimal string or an integer (which the
  Python module converts through `str`) is rounded twice. `(1 << 60) +
  (1 << 36) + 1` gives 0x5D800000, where one rounding gives
  0x5D800001. The port converts through a double too.
- `int()` of a numpy float gives the exact integer part, which for a
  double can take up to 1024 bits. The port returns it as a
  `TruncatedInt`, which callers reduce modulo 2^N or take whole when it
  fits.

The shapes of the patterns: a single's is a 32-bit pattern in 0 to
2^32 - 1, and a double's a 64-bit pattern in 0 to 2^64 - 1. */
#include "fp_arith.h"

#include <bit>
#include <cmath>
#include <cstdlib>
#include <string>

#include "errors.h"

namespace c6502::fp_arith {

namespace {

constexpr std::uint64_t kSingleMask = 0xFFFFFFFFull;
constexpr std::uint32_t kSingleSign = 0x80000000u;
constexpr std::uint64_t kDoubleSign = 0x8000000000000000ull;

std::uint32_t single_pattern(asdl::Int bits) {
  return static_cast<std::uint32_t>(static_cast<std::uint64_t>(bits & asdl::Int(kSingleMask)));
}

// The low 64 bits of `bits`, as Python's `bits & _DOUBLE_MASK` keeps.
std::uint64_t double_pattern(asdl::Int bits) {
  return static_cast<std::uint64_t>(bits);
}

float as_single(asdl::Int bits) { return std::bit_cast<float>(single_pattern(bits)); }
double as_double(asdl::Int bits) { return std::bit_cast<double>(double_pattern(bits)); }

asdl::Int single_bits(float f) {
  return asdl::Int(static_cast<std::uint64_t>(std::bit_cast<std::uint32_t>(f)));
}
asdl::Int double_bits(double d) { return asdl::Int(std::bit_cast<std::uint64_t>(d)); }

// Returns the double nearest to `value`, ties to even, as Python's
// `float(str(value))` gives it.
double int_to_double(asdl::Int value) {
  const bool negative = value < 0;
  const asdl::Int magnitude = negative ? -value : value;
  int length = 0;
  for (asdl::Int m = magnitude; m != 0; m >>= 1) {
    ++length;
  }
  double d;
  if (length <= 53) {
    d = static_cast<double>(static_cast<std::uint64_t>(magnitude));
  } else {
    // Keep the top 53 bits and round on the rest, half to even.
    const int shift = length - 53;
    asdl::Int mantissa = magnitude >> shift;
    const asdl::Int rest = magnitude - (mantissa << shift);
    const asdl::Int half = asdl::Int(1) << (shift - 1);
    if (rest > half || (rest == half && (mantissa & 1) != 0)) {
      mantissa += 1;
    }
    d = std::ldexp(static_cast<double>(static_cast<std::uint64_t>(mantissa)), shift);
  }
  return negative ? -d : d;
}

// Returns the double that `s` spells, as Python's float() of the string
// gives it: optional whitespace around a decimal number. strtod rounds
// correctly, and gives an infinity on an overflow and a subnormal or a
// zero on an underflow, as numpy does.
double parse_double(std::string_view s) {
  const auto is_space = [](char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v';
  };
  while (!s.empty() && is_space(s.front())) {
    s.remove_prefix(1);
  }
  while (!s.empty() && is_space(s.back())) {
    s.remove_suffix(1);
  }
  const std::string text(s);
  char* end = nullptr;
  const double d = std::strtod(text.c_str(), &end);
  if (text.empty() || end != text.c_str() + text.size()) {
    throw InternalError("could not convert string to float: '" + text + "'");
  }
  return d;
}

// Returns the integer part of `±(fraction) * 2^(exponent_field -
// bias)`, where `fraction` already holds the implicit bit of a normal
// value.
TruncatedInt truncate(bool negative, std::uint64_t fraction, int exponent) {
  TruncatedInt out{.negative = negative};
  if (exponent >= 0) {
    out.mantissa = fraction;
    out.exponent = exponent;
  } else if (-exponent < 64) {
    out.mantissa = fraction >> -exponent;
  }
  return out;
}

}  // namespace

std::optional<asdl::Int> TruncatedInt::exact() const {
  if (mantissa == 0) {
    return asdl::Int(0);
  }
  const int length = 64 - std::countl_zero(mantissa);
  if (length + exponent > 126) {
    return std::nullopt;
  }
  const asdl::Int v = asdl::Int(mantissa) << exponent;
  return negative ? -v : v;
}

asdl::Int TruncatedInt::wrapped(int bits) const {
  // A value with an exponent of 64 or more is a multiple of 2^64.
  std::uint64_t v = exponent >= 64 ? 0 : mantissa << exponent;
  if (negative) {
    v = 0 - v;
  }
  if (bits < 64) {
    v &= (std::uint64_t{1} << bits) - 1;
  }
  return asdl::Int(v);
}

asdl::Int single_string_to_bits(std::string_view s) {
  return single_bits(static_cast<float>(parse_double(s)));
}

asdl::Int double_string_to_bits(std::string_view s) { return double_bits(parse_double(s)); }

asdl::Int int_to_single_bits(asdl::Int value) {
  return single_bits(static_cast<float>(int_to_double(value)));
}

asdl::Int int_to_double_bits(asdl::Int value) { return double_bits(int_to_double(value)); }

std::optional<TruncatedInt> single_bits_to_int(asdl::Int bits) {
  const std::uint32_t p = single_pattern(bits);
  const int field = static_cast<int>((p >> 23) & 0xFF);
  if (field == 0xFF) {
    return std::nullopt;
  }
  std::uint64_t fraction = p & 0x7FFFFF;
  if (field != 0) {
    fraction |= 0x800000;
  }
  // A subnormal's exponent is that of the smallest normal value.
  const int exponent = (field == 0 ? 1 : field) - 150;
  return truncate((p & kSingleSign) != 0, fraction, exponent);
}

std::optional<TruncatedInt> double_bits_to_int(asdl::Int bits) {
  const std::uint64_t p = double_pattern(bits);
  const int field = static_cast<int>((p >> 52) & 0x7FF);
  if (field == 0x7FF) {
    return std::nullopt;
  }
  std::uint64_t fraction = p & 0xFFFFFFFFFFFFFull;
  if (field != 0) {
    fraction |= 0x10000000000000ull;
  }
  const int exponent = (field == 0 ? 1 : field) - 1075;
  return truncate((p & kDoubleSign) != 0, fraction, exponent);
}

asdl::Int single_bits_to_double_bits(asdl::Int bits) {
  return double_bits(static_cast<double>(as_single(bits)));
}

asdl::Int double_bits_to_single_bits(asdl::Int bits) {
  return single_bits(static_cast<float>(as_double(bits)));
}

asdl::Int single_negate(asdl::Int bits) {
  return asdl::Int(static_cast<std::uint64_t>(single_pattern(bits) ^ kSingleSign));
}

asdl::Int double_negate(asdl::Int bits) { return asdl::Int(double_pattern(bits) ^ kDoubleSign); }

namespace {

// Returns the NaN that numpy's arithmetic gives when an operand is a
// NaN, or none when neither is: the first operand's when it is a NaN,
// and otherwise the second's, made quiet by setting the top bit of its
// fraction. That is the SSE unit's rule for `a op b` with `a` in the
// destination register. The port picks the NaN itself because a
// compiler may swap the operands of `+` and `*`, which commute for
// every other value, and GCC 12 did, which gave the second operand's
// NaN for a sum of two NaNs.
std::optional<asdl::Int> single_nan(asdl::Int a, asdl::Int b) {
  for (const asdl::Int x : {a, b}) {
    if (std::isnan(as_single(x))) {
      return asdl::Int(static_cast<std::uint64_t>(single_pattern(x) | 0x400000u));
    }
  }
  return std::nullopt;
}

std::optional<asdl::Int> double_nan(asdl::Int a, asdl::Int b) {
  for (const asdl::Int x : {a, b}) {
    if (std::isnan(as_double(x))) {
      return asdl::Int(double_pattern(x) | 0x8000000000000ull);
    }
  }
  return std::nullopt;
}

}  // namespace

asdl::Int single_add(asdl::Int a, asdl::Int b) {
  return single_nan(a, b).value_or(single_bits(as_single(a) + as_single(b)));
}
asdl::Int single_sub(asdl::Int a, asdl::Int b) {
  return single_nan(a, b).value_or(single_bits(as_single(a) - as_single(b)));
}
asdl::Int single_mul(asdl::Int a, asdl::Int b) {
  return single_nan(a, b).value_or(single_bits(as_single(a) * as_single(b)));
}
asdl::Int single_div(asdl::Int a, asdl::Int b) {
  return single_nan(a, b).value_or(single_bits(as_single(a) / as_single(b)));
}
asdl::Int double_add(asdl::Int a, asdl::Int b) {
  return double_nan(a, b).value_or(double_bits(as_double(a) + as_double(b)));
}
asdl::Int double_sub(asdl::Int a, asdl::Int b) {
  return double_nan(a, b).value_or(double_bits(as_double(a) - as_double(b)));
}
asdl::Int double_mul(asdl::Int a, asdl::Int b) {
  return double_nan(a, b).value_or(double_bits(as_double(a) * as_double(b)));
}
asdl::Int double_div(asdl::Int a, asdl::Int b) {
  return double_nan(a, b).value_or(double_bits(as_double(a) / as_double(b)));
}

namespace {

template <class F>
Order compare(F a, F b) {
  if (std::isnan(a) || std::isnan(b)) {
    return Order::kUnordered;
  }
  if (a < b) {
    return Order::kLt;
  }
  if (a > b) {
    return Order::kGt;
  }
  return Order::kEq;
}

}  // namespace

Order single_compare(asdl::Int a, asdl::Int b) { return compare(as_single(a), as_single(b)); }
Order double_compare(asdl::Int a, asdl::Int b) { return compare(as_double(a), as_double(b)); }

bool single_is_zero(asdl::Int bits) { return as_single(bits) == 0.0f; }
bool double_is_zero(asdl::Int bits) { return as_double(bits) == 0.0; }

bool single_is_finite(asdl::Int bits) { return std::isfinite(as_single(bits)); }
bool double_is_finite(asdl::Int bits) { return std::isfinite(as_double(bits)); }

bool single_is_truthy(asdl::Int bits) { return !single_is_zero(bits); }
bool double_is_truthy(asdl::Int bits) { return !double_is_zero(bits); }

}  // namespace c6502::fp_arith
