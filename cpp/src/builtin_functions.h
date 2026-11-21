// The port of builtin_functions.py: the names of the compiler's
// builtins and the number of arguments each takes, the packed BCD
// arithmetic of the 6502's decimal mode, and the operations of
// <stdbit.h>, which constant folding, c99_to_tac, and the type checker
// compute with.
#pragma once

#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "asdl/runtime.h"

namespace c6502::builtin_functions {

inline constexpr std::string_view kBcdAdd = "__c6502_bcd_add";
inline constexpr std::string_view kBcdSub = "__c6502_bcd_sub";
inline constexpr std::string_view kBinToBcd = "__c6502_bin_to_bcd";

// C23's bit and byte utilities (§7.18), in the standard's order
// (Python's `STDBIT_OPERATIONS`).
const std::vector<std::string_view>& stdbit_operations();

// The suffix of each <stdbit.h> function with a parameter of one type,
// and that type as `type_checking` names it, in Python's order
// (`STDBIT_SUFFIXES`).
const std::vector<std::pair<std::string_view, std::string_view>>& stdbit_suffixes();

// One <stdbit.h> builtin: its operation, and its suffix, which is none
// for the type-generic form.
struct StdbitBuiltin {
  std::string name;
  std::string_view operation;
  std::optional<std::string_view> suffix;
};

// Every <stdbit.h> builtin, `__c6502_stdc_<operation>` and then its five
// forms of one type for each operation in turn, in the order of Python's
// `STDBIT_BUILTINS`.
const std::vector<StdbitBuiltin>& stdbit_builtins();

// Returns the <stdbit.h> builtin named `name`, or null.
const StdbitBuiltin* find_stdbit_builtin(std::string_view name);

// Returns the number of arguments the builtin `name` takes, or none
// when `name` isn't a builtin (Python's `BUILTINS`).
std::optional<int> builtin_arity(std::string_view name);

// Returns whether `name` is a builtin.
bool is_builtin(std::string_view name);

// The accumulator and the carry after a decimal-mode ADC or SBC.
struct ByteResult {
  int a = 0;
  int carry = 0;
};

// Returns A and C after `ADC b` with the decimal flag set, A holding `a`
// and the carry `carry` (0 or 1). This is py65's decimal-mode `opADC`,
// for digits above 9 too.
ByteResult adc_byte(int a, int b, int carry);

// Returns A and C after `SBC b` with the decimal flag set, A holding `a`
// and the carry `carry` (1 for no borrow). This is py65's decimal-mode
// `opSBC`.
ByteResult sbc_byte(int a, int b, int carry);

// Returns `a + b` in packed BCD over `nbytes` bytes, as `CLC` and then
// an `ADC` per byte, low byte first, compute it. The carry out of the
// top byte is dropped.
asdl::Int bcd_add(asdl::Int a, asdl::Int b, int nbytes);

// Returns `a - b` in packed BCD over `nbytes` bytes, as `SEC` and then
// an `SBC` per byte, low byte first, compute it. The borrow out of the
// top byte is dropped.
asdl::Int bcd_sub(asdl::Int a, asdl::Int b, int nbytes);

// Returns the low 16 bits of `value` as five packed BCD digits, the ones
// in bits 0-3 and the ten-thousands in bits 16-19, as the runtime helper
// `bin16_to_bcd` computes them. The result is at most 0x65535.
asdl::Int bin_to_bcd(asdl::Int value);

// Returns the result of the <stdbit.h> `operation` (one of
// `stdbit_operations`) on `value` taken as an unsigned integer of
// `width` bits, from 1 to 64, as C23 §7.18 gives it. The rules for the
// most and least significant indexes, `first_...`, and `bit_ceil` are
// in the comment above the definition. Throws `InternalError` for an
// unknown operation, where Python raises `ValueError`.
asdl::Int stdbit(std::string_view operation, asdl::Int value, int width);

}  // namespace c6502::builtin_functions
