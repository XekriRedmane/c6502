/* The compiler's builtin functions, and the arithmetic behind them.

A builtin is called like a function, but no function is defined for it.
The front end recognizes the name, the type checker types the call by
the builtin's own rules, and c99_to_tac lowers it to a TAC instruction.
The names are in the implementation's reserved namespace (C99 §7.1.3,
an identifier beginning with two underscores), so they can't collide
with a program's own names.

Three builtins work with packed BCD in the 6502's decimal mode
(docs/builtins.md), and the rest are C23's bit and byte utilities, which
<stdbit.h> names (docs/stdbit.md).

  `__c6502_bcd_add(a, b)` is `a + b` in packed BCD.
  `__c6502_bcd_sub(a, b)` is `a - b` in packed BCD.
  `__c6502_bin_to_bcd(x)` is the 16-bit `x` as five packed BCD digits.

Packed BCD holds two decimal digits in each byte, the higher digit in
the high nibble, and a multi-byte value keeps its low byte first like
any c6502 integer, so 0x0250 is 250. The two arithmetic builtins lower
to TAC `Binary(BcdAdd)` and `Binary(BcdSubtract)`, and
`__c6502_bin_to_bcd` to `Unary(BinToBcd)`, which tac_to_asm lowers to a
call of the runtime helper `bin16_to_bcd`.

`adc_byte` and `sbc_byte` copy py65's `opADC` and `opSBC` in decimal
mode (py65/devices/mpu6502.py), so constant folding, tac_sim, and the
simulator give the same result for every pair of bytes, digits that
aren't 0-9 included. For those, py65's accumulator and carry match the
NMOS 6502's algorithm in 6502.org's "Decimal Mode" tutorial (Appendix
A). That comes from comparing the two algorithms, and it hasn't been
checked on the hardware.

The <stdbit.h> builtins are `__c6502_stdc_<operation>` for the
type-generic form of each of C23's 14 operations (§7.18.3 through
§7.18.16), typed by its argument, and `__c6502_stdc_<operation>_uc`,
`_us`, `_ui`, `_ul`, and `_ull` for the forms with a parameter of
`unsigned char` through `unsigned long long`. */
#include "builtin_functions.h"

#include <bit>
#include <cstdint>

#include "errors.h"

namespace c6502::builtin_functions {

const std::vector<std::string_view>& stdbit_operations() {
  static const std::vector<std::string_view> ops = {
      "leading_zeros",       "leading_ones",       "trailing_zeros", "trailing_ones",
      "first_leading_zero",  "first_leading_one",  "first_trailing_zero",
      "first_trailing_one",  "count_zeros",        "count_ones",     "has_single_bit",
      "bit_width",           "bit_floor",          "bit_ceil",
  };
  return ops;
}

const std::vector<std::pair<std::string_view, std::string_view>>& stdbit_suffixes() {
  static const std::vector<std::pair<std::string_view, std::string_view>> suffixes = {
      {"uc", "unsigned char"},
      {"us", "unsigned short"},
      {"ui", "unsigned int"},
      {"ul", "unsigned long"},
      {"ull", "unsigned long long"},
  };
  return suffixes;
}

const std::vector<StdbitBuiltin>& stdbit_builtins() {
  static const std::vector<StdbitBuiltin> builtins = [] {
    std::vector<StdbitBuiltin> out;
    for (const std::string_view op : stdbit_operations()) {
      out.push_back({"__c6502_stdc_" + std::string(op), op, std::nullopt});
      for (const auto& [suffix, type] : stdbit_suffixes()) {
        out.push_back(
            {"__c6502_stdc_" + std::string(op) + "_" + std::string(suffix), op, suffix});
      }
    }
    return out;
  }();
  return builtins;
}

const StdbitBuiltin* find_stdbit_builtin(std::string_view name) {
  for (const auto& b : stdbit_builtins()) {
    if (b.name == name) {
      return &b;
    }
  }
  return nullptr;
}

std::optional<int> builtin_arity(std::string_view name) {
  if (name == kBcdAdd || name == kBcdSub) {
    return 2;
  }
  if (name == kBinToBcd || find_stdbit_builtin(name) != nullptr) {
    return 1;
  }
  return std::nullopt;
}

bool is_builtin(std::string_view name) { return builtin_arity(name).has_value(); }

ByteResult adc_byte(int a, int b, int carry) {
  int nibble0 = (b & 0xF) + (a & 0xF) + carry;
  int adjust0 = 0;
  int adjust1 = 0;
  int halfcarry = 0;
  int decimalcarry = 0;
  if (nibble0 > 9) {
    adjust0 = 6;
    halfcarry = 1;
  }
  int nibble1 = ((b >> 4) & 0xF) + ((a >> 4) & 0xF) + halfcarry;
  if (nibble1 > 9) {
    adjust1 = 6;
    decimalcarry = 1;
  }
  nibble0 = ((nibble0 & 0xF) + adjust0) & 0xF;
  nibble1 = ((nibble1 & 0xF) + adjust1) & 0xF;
  return {(nibble1 << 4) + nibble0, decimalcarry};
}

ByteResult sbc_byte(int a, int b, int carry) {
  int adjust0 = 0;
  int adjust1 = 0;
  int halfcarry = 1;
  const int nibble0_sum = (a & 0xF) + (~b & 0xF) + carry;
  if (nibble0_sum <= 0xF) {
    halfcarry = 0;
    adjust0 = 10;
  }
  const int nibble1_sum = ((a >> 4) & 0xF) + ((~b >> 4) & 0xF) + halfcarry;
  if (nibble1_sum <= 0xF) {
    adjust1 = 10 << 4;
  }
  int aluresult = a + (~b & 0xFF) + carry;
  const int decimalcarry = aluresult > 0xFF ? 1 : 0;
  aluresult &= 0xFF;
  const int nibble0 = (aluresult + adjust0) & 0xF;
  const int nibble1 = ((aluresult + adjust1) >> 4) & 0xF;
  return {(nibble1 << 4) + nibble0, decimalcarry};
}

namespace {

int byte_of(asdl::Int v, int k) { return static_cast<int>((v >> (8 * k)) & 0xFF); }

}  // namespace

asdl::Int bcd_add(asdl::Int a, asdl::Int b, int nbytes) {
  asdl::Int out = 0;
  int carry = 0;
  for (int k = 0; k < nbytes; ++k) {
    const ByteResult r = adc_byte(byte_of(a, k), byte_of(b, k), carry);
    carry = r.carry;
    out |= asdl::Int(r.a) << (8 * k);
  }
  return out;
}

asdl::Int bcd_sub(asdl::Int a, asdl::Int b, int nbytes) {
  asdl::Int out = 0;
  int carry = 1;
  for (int k = 0; k < nbytes; ++k) {
    const ByteResult r = sbc_byte(byte_of(a, k), byte_of(b, k), carry);
    carry = r.carry;
    out |= asdl::Int(r.a) << (8 * k);
  }
  return out;
}

asdl::Int bin_to_bcd(asdl::Int value) {
  int v = static_cast<int>(value & 0xFFFF);
  asdl::Int out = 0;
  for (int k = 0; k < 5; ++k) {
    out |= asdl::Int(v % 10) << (4 * k);
    v /= 10;
  }
  return out;
}

// The most significant index counts from the most significant bit, 0,
// to the least significant bit, `width - 1` (§7.18.1p3), and the least
// significant index counts the other way (§7.18.1p4). Each `first_...`
// result is the index plus 1, or 0 when there's no such bit. `bit_ceil`
// gives 0 when the power of 2 doesn't fit in `width` bits (§7.18.16p2),
// and `bit_ceil(0)` is 1, the smallest power of 2.
asdl::Int stdbit(std::string_view operation, asdl::Int value, int width) {
  const std::uint64_t mask = width >= 64 ? ~std::uint64_t{0} : (std::uint64_t{1} << width) - 1;
  const std::uint64_t v = static_cast<std::uint64_t>(value) & mask;
  const std::uint64_t inverse = v ^ mask;
  const auto bit_length = [](std::uint64_t x) { return 64 - std::countl_zero(x); };
  const auto trailing_zeros = [&](std::uint64_t x) {
    return x == 0 ? width : std::countr_zero(x);
  };
  const auto result = [](std::uint64_t r) { return asdl::Int(r); };
  if (operation == "leading_zeros") {
    return width - bit_length(v);
  }
  if (operation == "leading_ones") {
    return width - bit_length(inverse);
  }
  if (operation == "trailing_zeros") {
    return trailing_zeros(v);
  }
  if (operation == "trailing_ones") {
    return trailing_zeros(inverse);
  }
  if (operation == "first_leading_zero") {
    return v == mask ? 0 : width - bit_length(inverse) + 1;
  }
  if (operation == "first_leading_one") {
    return v == 0 ? 0 : width - bit_length(v) + 1;
  }
  if (operation == "first_trailing_zero") {
    return v == mask ? 0 : trailing_zeros(inverse) + 1;
  }
  if (operation == "first_trailing_one") {
    return v == 0 ? 0 : trailing_zeros(v) + 1;
  }
  if (operation == "count_zeros") {
    return width - std::popcount(v);
  }
  if (operation == "count_ones") {
    return std::popcount(v);
  }
  if (operation == "has_single_bit") {
    return std::popcount(v) == 1 ? 1 : 0;
  }
  if (operation == "bit_width") {
    return bit_length(v);
  }
  if (operation == "bit_floor") {
    return v == 0 ? result(0) : result(std::uint64_t{1} << (bit_length(v) - 1));
  }
  if (operation == "bit_ceil") {
    if (v <= 1) {
      return 1;
    }
    // `1 << (v - 1).bit_length()` is 2^64 for the largest values, which
    // the mask of 64 bits makes 0.
    const int shift = bit_length(v - 1);
    return shift >= 64 ? result(0) : result((std::uint64_t{1} << shift) & mask);
  }
  throw InternalError("unknown <stdbit.h> operation '" + std::string(operation) + "'");
}

}  // namespace c6502::builtin_functions
