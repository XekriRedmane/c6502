// SHA-512 (FIPS 180-4), for the string seeds of CPython's `random`
// (src/py_random.h), which hash the string with it.
#pragma once

#include <array>
#include <string_view>

namespace c6502 {

// Returns the 64 bytes of the SHA-512 digest of `data`, as Python's
// `hashlib.sha512(data).digest()` gives them.
std::array<unsigned char, 64> sha512(std::string_view data);

}  // namespace c6502
