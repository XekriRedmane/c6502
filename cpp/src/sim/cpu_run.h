// The CRC-32 and the digest of the CPU's state that the games' event
// lists and the tests' checks of what a run gave are written with.
#pragma once

#include <cstdint>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "sim/cpu.h"

namespace c6502::sim::cpu_run {

// Returns zlib's CRC-32 of `bytes`, as Python's `zlib.crc32` gives it.
std::uint32_t crc32(std::string_view bytes);

// Returns the digest of the state after an instruction: zlib's CRC-32 of
// PC (2 bytes), A, X, Y, SP, P, and the cycle count (8 bytes), low byte
// first, followed by each of `writes`, the instruction's writes in
// order, as its address (2 bytes) and its value. Python packs the same
// bytes with `struct.pack("<HBBBBBQ", ...)`.
std::uint32_t digest(const Cpu& cpu, const std::vector<std::pair<std::uint16_t, std::uint8_t>>& writes);

}  // namespace c6502::sim::cpu_run
