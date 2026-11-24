// SHA-256 (FIPS 180-4), for the digests of the objects and executables
// that the golden files hold (src/golden.h).
#pragma once

#include <string>
#include <string_view>

namespace c6502 {

// Returns the SHA-256 digest of `data` as 64 lowercase hexadecimal
// digits, as Python's `hashlib.sha256(data).hexdigest()` gives it.
std::string sha256_hex(std::string_view data);

}  // namespace c6502
