// Python's integers as the assembler (binutils/assembler.py) and the
// parser of the link metadata (passes/zp_link_metadata.py) compute with
// them: `int(text, base)` with its rules and its messages, and the
// operators of an expression, with floor division and an arithmetic
// `>>` of a negative value.
//
// Python's integers have no width, and the port's are the 128 bits of
// `asdl::Int`. The values c6502 writes take at most 32 bits (a `DC.L`),
// so a value that takes more than 128 bits comes only from a hand-written
// text. Where a result doesn't fit, the functions throw
// `CompileError("OverflowError", ...)` with a message that names the
// limit, so that the port never gives a result Python wouldn't.
#pragma once

#include <cstdint>
#include <string>
#include <string_view>

#include "asdl/runtime.h"

namespace c6502::py_int {

using asdl::Int;

// The largest and smallest values of `Int`.
Int max_value();
Int min_value();

// Throws the error for a value that takes more than 128 bits, with
// `where` in front of the message, as "where: ...".
[[noreturn]] void too_wide(std::string_view where);

// Returns the decimal value of the code point `cp`, which `\d` matches
// (`unicodedata.decimal`), or -1 for one it doesn't.
int decimal_value(std::uint32_t cp);

// Returns `int(text, base)` for the UTF-8 string `text` and a base of 2,
// 10, or 16. Python first turns each code point with a decimal value
// into its ASCII digit and each space into an ASCII space, so a number
// may be written in any script's digits, and then takes spaces around
// the number, a sign, a `0x` (or `0b`) prefix for base 16 (or 2), and
// single underscores between the digits. Throws
// `CompileError("ValueError", ...)` with Python's message, "invalid
// literal for int() with base 10: 'x'" (the repr cut to 200 code points)
// or, for more than 4,300 decimal digits, Python's limit on the digits
// of a conversion, and the `too_wide` error, with `where`, for a value
// Python reads but `Int` can't hold.
Int parse(std::string_view text, int base, std::string_view where);

// Return `a + b`, `a - b`, `a * b`, `-a`, `a // b` (for `b` other than
// 0), `a << b`, and `a >> b` as Python computes them, or throw the
// `too_wide` error with `where`. The shifts throw
// `CompileError("ValueError", "negative shift count")` for a negative
// `b`, as Python does, and `a >> b` is floor division by 2**b.
Int add(Int a, Int b, std::string_view where);
Int sub(Int a, Int b, std::string_view where);
Int mul(Int a, Int b, std::string_view where);
Int neg(Int a, std::string_view where);
Int floor_div(Int a, Int b, std::string_view where);
Int lshift(Int a, Int b, std::string_view where);
Int rshift(Int a, Int b);

// Returns `v` as a `std::int64_t`, the width of the fields of the C++
// ELF classes (src/binutils/elf.h), or throws an OverflowError, with
// `where`, that names those 64 bits.
std::int64_t to_int64(Int v, std::string_view where);

}  // namespace c6502::py_int
