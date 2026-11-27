// The backend mode `cpu-run`, which runs the CPU (sim/cpu.h) for the
// lockstep mode of sim/harness.py, which compares it with py65.
#pragma once

#include <cstdint>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "json.h"
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

// Returns the response to a `cpu-run` request (src/backend.h), whose
// arguments `kwargs` are an object of
//
//     stops            the addresses where a segment stops, before the fetch
//     stop_at_brk      whether a segment stops at an opcode of $00
//     max_cycles       the cycle count at which a segment stops
//     max_instructions the instructions after which a segment stops, or null
//     segments         a list of {pc, a, x, y, sp, p, cycles, patches}
//
// and whose memory starts as `image`, 65,536 bytes. Each segment sets
// the registers and the cycle count, stores each `[address, value]` of
// `patches` in memory, and runs the CPU from there with memory as the
// segment before it left it. The checks before each instruction come in
// the order of the harness's `run`: the cycles, the instructions, the
// stops, and BRK. The response has, for each segment k,
//
//     segment <k> <stop|brk|cycles|count|error> <instructions executed>
//     state <pc> <a> <x> <y> <sp> <p> <cycles>
//     last-writes <address:value ...>
//     writes <length>
//     <3 bytes for each address written: the address, low byte first, and its last value>
//     digests <length>
//     <the digest of each instruction, 4 bytes, low byte first>
//
// in decimal and hex as the harness reads them. `last-writes` gives the
// writes of the segment's last instruction in hex, and `writes` lists
// the addresses in ascending order. A segment whose step throws
// `CpuError` has the state that the throw left, and it ends the
// response with
//
//     error <JSON message>
//
// Throws `std::runtime_error` for arguments or an image it can't read.
std::string respond(const json::Value& kwargs, std::string_view image);

}  // namespace c6502::sim::cpu_run
