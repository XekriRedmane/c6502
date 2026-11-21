// The port of fp_arith.py: the conversions between numbers and the
// IEEE 754 bit patterns that c6502 carries floating values as, and the
// arithmetic and comparisons of constant folding on those patterns.
// c99_to_tac, the type checker, and constant folding call them.
//
// A pattern is an `asdl::Int`, as the IR holds it: a single's 32 bits
// in 0 to 2^32 - 1 and a double's 64 bits in 0 to 2^64 - 1. Each
// function masks the pattern it is given to its width first, as the
// Python functions do.
#pragma once

#include <cstdint>
#include <optional>
#include <string_view>

#include "asdl/runtime.h"

namespace c6502::fp_arith {

// Returns the 32-bit pattern of the decimal string `s` read as a
// single. Python's numpy parses the string as a double first and then
// rounds that to a single, so a string or an integer converts with two
// roundings (`1.0000001788139343261718749` gives 0x3F800002, where one
// rounding gives 0x3F800001), and so does this. An overflow gives an
// infinity and an underflow a zero or a subnormal. Throws
// `InternalError` for a string that isn't a decimal number with
// optional surrounding whitespace, where Python raises `ValueError`.
// Python also accepts underscores between digits and the words `inf`
// and `nan`, which no caller passes.
asdl::Int single_string_to_bits(std::string_view s);

// Returns the 64-bit pattern of the decimal string `s` read as a
// double, correctly rounded.
asdl::Int double_string_to_bits(std::string_view s);

// Returns the pattern of the integer `value` as a single, rounded
// first to a double and then to a single, as Python's conversion
// through `str` and numpy does.
asdl::Int int_to_single_bits(asdl::Int value);

// Returns the pattern of the integer `value` as a double, correctly
// rounded.
asdl::Int int_to_double_bits(asdl::Int value);

// The integer part of a finite floating value, `±mantissa * 2^exponent`,
// which Python's `int` of a numpy float gives exactly as an int of up
// to 1024 bits. Callers reduce it modulo 2^N, which `wrapped` does, or
// take it whole when it fits in an `asdl::Int`.
struct TruncatedInt {
  bool negative = false;
  std::uint64_t mantissa = 0;
  int exponent = 0;

  // Returns whether the value is 0.
  bool is_zero() const { return mantissa == 0; }
  // Returns the value, or none when it doesn't fit in an `asdl::Int`
  // (at least 2^126 in magnitude, which a double above that holds).
  std::optional<asdl::Int> exact() const;
  // Returns the value modulo 2^bits, from 0 to 2^bits - 1, for `bits`
  // from 1 to 64.
  asdl::Int wrapped(int bits) const;
};

// Returns the single with pattern `bits` truncated toward zero, or none
// for a NaN or an infinity, where Python's `int` raises.
std::optional<TruncatedInt> single_bits_to_int(asdl::Int bits);

// Returns the double with pattern `bits` truncated toward zero, or none
// for a NaN or an infinity.
std::optional<TruncatedInt> double_bits_to_int(asdl::Int bits);

// Returns the double pattern of the single with pattern `bits`, which
// is exact.
asdl::Int single_bits_to_double_bits(asdl::Int bits);

// Returns the single pattern of the double with pattern `bits`, rounded
// to nearest even, with an overflow to an infinity and an underflow to
// a subnormal or a zero.
asdl::Int double_bits_to_single_bits(asdl::Int bits);

// Return the pattern with its sign bit flipped, which keeps a NaN's
// payload.
asdl::Int single_negate(asdl::Int bits);
asdl::Int double_negate(asdl::Int bits);

// Return the sum, difference, product, and quotient of two singles or
// two doubles at their own precision, rounded to nearest even, with
// IEEE 754's infinities and NaNs. A division by zero gives an infinity,
// or a NaN for 0/0. An operation with a NaN operand gives the first
// NaN operand, made quiet, as numpy does on x86-64, whichever order the
// compiler puts the operands of `+` and `*` in. The other operations
// are the SSE unit's, as numpy's are,
// so the build must not contract them into fused multiply-adds or use
// `-ffast-math`.
asdl::Int single_add(asdl::Int a, asdl::Int b);
asdl::Int single_sub(asdl::Int a, asdl::Int b);
asdl::Int single_mul(asdl::Int a, asdl::Int b);
asdl::Int single_div(asdl::Int a, asdl::Int b);
asdl::Int double_add(asdl::Int a, asdl::Int b);
asdl::Int double_sub(asdl::Int a, asdl::Int b);
asdl::Int double_mul(asdl::Int a, asdl::Int b);
asdl::Int double_div(asdl::Int a, asdl::Int b);

// The outcome of a comparison of two floating values. Python's
// functions return the strings "lt", "eq", "gt", and "unordered", and a
// comparison with a NaN is unordered.
enum class Order { kLt, kEq, kGt, kUnordered };

// Return how the first value compares with the second. +0 and -0 are
// equal.
Order single_compare(asdl::Int a, asdl::Int b);
Order double_compare(asdl::Int a, asdl::Int b);

// Return whether the value is +0 or -0. A NaN isn't zero.
bool single_is_zero(asdl::Int bits);
bool double_is_zero(asdl::Int bits);

// Return whether the value is neither a NaN nor an infinity.
bool single_is_finite(asdl::Int bits);
bool double_is_finite(asdl::Int bits);

// Return whether the value compares unequal to 0, the truth of a
// controlling expression (C99 §6.3.1.2). A NaN is true, and both zeros
// are false.
bool single_is_truthy(asdl::Int bits);
bool double_is_truthy(asdl::Int bits);

}  // namespace c6502::fp_arith
