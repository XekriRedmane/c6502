// The parts of bit_precise.py that code generation and `c99_to_tac`
// read: the byte count and signedness of each TAC integer constant
// class, among them the classes of the 3-, 5-, 6-, and 7-byte integers
// that are left of C23's bit-precise types after `c99_to_tac`, the
// width of the runtime helper an operand of such a size goes to, and
// the range, padding, and storage type of a bit-precise type.
#pragma once

#include <optional>
#include <utility>

#include "asdl/runtime.h"
#include "c99_ast.h"
#include "tac_ast.h"

namespace c6502::bit_precise {

// The size in bytes and the signedness of a TAC integer constant class.
struct ConstLayout {
  int bytes = 0;
  bool is_signed = false;
};

// Returns the layout of `c` when it is one of the odd classes,
// `ConstInt24` to `ConstUInt56` (Python's `ODD_CONST_LAYOUT`), or none
// for any other class.
std::optional<ConstLayout> odd_const_layout(const tac_ast::Type_const& c);

// Returns the layout of `c` when it is an integer constant of any width
// (Python's `INTEGER_CONST_LAYOUT`), or none for `ConstFloat` and
// `ConstDouble`.
std::optional<ConstLayout> integer_const_layout(const tac_ast::Type_const& c);

// Returns the bytes a value of `_BitInt(width)` occupies, ceil(width/8).
asdl::Int storage_bytes(asdl::Int width);

// Returns the operand width of the runtime helper that a multiply,
// divide, remainder, shift by a variable, or conversion to or from a
// floating type of an `n`-byte integer uses: the least of 1, 2, 4, and
// 8 that is at least `n`. Throws `InternalError` for a wider `n`, where
// Python raises `ValueError`.
int helper_bytes(int n);

// Returns `value` modulo 2^width, read as a signed or unsigned value of
// that many bits.
asdl::Int wrap(asdl::Int value, int width, bool is_signed);

// Returns the `value` field of the TAC constant `c`, as Python's
// `getattr(c, "value", None)` gives it: the stored integer for an
// integer class and none for `ConstFloat` and `ConstDouble`.
std::optional<asdl::Int> raw_value(const tac_ast::Type_const& c);

// Returns a constant of the class of `c` holding `value`, as Python's
// `type(c)(value=value)` makes one. `c` must be an integer constant.
tac_ast::Type_const with_value(const tac_ast::Type_const& c, asdl::Int value);

// Returns the value of the TAC integer constant `c`, read at its
// class's width and signedness. `c` must be an integer constant.
asdl::Int const_value(const tac_ast::Type_const& c);

// Returns the TAC integer constant of `n` bytes and that signedness
// holding `value` modulo 2^(8n) as its bit pattern, which isn't
// negative, as the type checker gives its constants.
tac_ast::Type_const make_const(int n, bool is_signed, asdl::Int value);

// Returns the least and greatest values of the integer type of `width`
// bits and that signedness.
std::pair<asdl::Int, asdl::Int> value_range(int width, bool is_signed);

// Returns whether the top byte of `t` holds padding bits.
bool has_padding(const c99_ast::BitInt& t);

// Returns the integer type of `t`'s storage, which the back end
// handles: the standard type of the same size and signedness for 1, 2,
// 4, and 8 bytes, and `BitInt(8 * bytes)` for the other sizes.
c99_ast::Type_data_type storage_type(const c99_ast::BitInt& t);

// The widest bit-precise type, the least C23 allows, since the
// runtime's helpers stop at 8 bytes (Python's `BITINT_MAXWIDTH`).
inline constexpr int kBitIntMaxWidth = 64;

// Returns the width N of the type of a `wb` constant (`is_signed`) or a
// `uwb` one whose value has `bit_length` bits: the smallest N greater
// than 1 that holds the value and the sign bit, or the smallest N
// greater than 0 that holds the value (C23 §6.4.4.2p6). The value isn't
// negative. Python's `constant_width` takes the value, and this takes
// its `int.bit_length()`, since a constant may be wider than any C++
// integer.
inline int constant_width(int bit_length, bool is_signed) {
  if (is_signed) {
    return bit_length + 1 > 2 ? bit_length + 1 : 2;
  }
  return bit_length > 1 ? bit_length : 1;
}

}  // namespace c6502::bit_precise
