// The decompression of .xz streams, for the frozen captures that
// `c6502cpp check-frozen` reads, with the decoder of XZ Embedded in
// cpp/third_party/xz-embedded.
#pragma once

#include <string>
#include <string_view>

namespace c6502 {

// Returns the bytes that the .xz stream `data` holds. The stream's
// check must be CRC32 or none, the checks XZ Embedded reads, and
// tools/freeze_captures.py writes CRC32. Raises `std::runtime_error`
// for a stream that is damaged or that the decoder doesn't take.
std::string xz_decompress(std::string_view data);

}  // namespace c6502
