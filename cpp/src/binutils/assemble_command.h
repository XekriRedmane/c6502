#pragma once

#include <string>

#include "binutils/elf.h"

namespace c6502::binutils {

// Returns the response to the request `text`. A request is text with LF
// line endings:
//
//     c6502-assemble 1
//     args <JSON, one line>
//     text <length>
//     <the assembly text, UTF-8>
//
// where the length counts the bytes after its line, and the arguments
// are an object with `filename`, `keep_source`, and `link_symbols`, a
// list of `[name, address]` pairs with null for an address outside zero
// page, or null for none. The response is
//
//     object <length>
//     <the object's bytes>
//
// or, when the assembler rejects the text,
//
//     error <JSON {"type": ..., "message": ...}>
//
// with the Python class of the exception and its message. Python's
// `elf.write` packs each field with `struct`, which raises `struct.error`
// ("argument out of range", whose class is named "error") for a value
// outside the field, so the command gives that error where Python's
// would come, for a relocation's addend outside 32 signed bits or a size
// outside 32 unsigned bits, where the C++ writer would keep the low 32
// bits.
//
// Throws `std::runtime_error` for a request that can't be read, and
// passes on an `InternalError`.
std::string assemble_response(const std::string& text);

}  // namespace c6502::binutils
