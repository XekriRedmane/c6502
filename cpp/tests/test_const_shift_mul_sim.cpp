// `tac_to_asm` inlines a constant shift `8q + r` as byte moves and `r`
// one-bit steps when `inline_shift_ok` (src/passes/shift_cost.h) accepts
// it, and calls the `asl*` / `lsr*` / `asr*` helper otherwise. Under -O,
// `strength_reduction` rewrites the multiplies into shifts and an add or
// a subtract. Sweeping every count from 1 to width-1 runs both the inline
// and the helper paths, and the boundary between them.
//
// Each operation is its own function, which keeps every unoptimized
// frame small and keeps the argument a runtime value. The functions are
// split across programs of at most `kChunk` functions. The simulator
// loads code at $0800 and the soft stack grows down from $7FFF, and an
// unoptimized program holding all 148 of the 64-bit functions reaches
// the stack. Nothing checks for that, and the results come back wrong.
#include <algorithm>
#include <cstdint>
#include <string>
#include <tuple>
#include <vector>

#include "batch1_case.h"
#include "compile_case.h"
#include "doctest.h"
#include "sim/cpu_run.h"
#include "sim_case.h"

namespace c6502::suite {
namespace {

const std::vector<std::tuple<std::string, int>> kTypes = {
    {"int", 2}, {"unsigned int", 2}, {"long", 4}, {"unsigned long", 4}, {"long long", 8}, {"unsigned long long", 8}};
const std::vector<int> kMultipliers = {3,  5,  6,  7,  9,  10, 12,  15,  17,  24,  31,
                                       33, 40, 48, 63, 65, 96, 127, 129, 192, 255, 257};
// Bit patterns, truncated to each width: one with the sign bit set and
// one without, both with bits set in every byte.
const std::vector<std::uint64_t> kInputs = {0xF3A5C3961E7D4B82ULL, 0x5A3C69E1B2D4870FULL};
constexpr std::size_t kChunk = 40;

std::uint64_t mask_of(int size) { return size == 8 ? ~std::uint64_t{0} : (std::uint64_t{1} << (size * 8)) - 1; }

std::int64_t signed_of(std::uint64_t v, int size) {
  const int bits = size * 8;
  v &= mask_of(size);
  if (bits == 64) return static_cast<std::int64_t>(v);
  return (v >> (bits - 1)) ? static_cast<std::int64_t>(v) - (std::int64_t{1} << bits) : static_cast<std::int64_t>(v);
}

// A program with one function per shift or product, each storing its
// result into a global, and the value each global should hold
// afterwards (as an unsigned bit pattern), in the order of the globals.
struct Program {
  std::string source;
  std::vector<std::pair<std::string, std::uint64_t>> expected;
};

std::vector<Program> programs(const std::string& ctype, int size, std::uint64_t x) {
  const int bits = size * 8;
  const std::uint64_t mask = mask_of(size);
  const bool is_signed = ctype.rfind("unsigned", 0) != 0;
  std::vector<std::tuple<std::string, std::string, std::uint64_t>> ops;
  for (int k = 1; k < bits; ++k) {
    ops.emplace_back(cat("l", k), cat("x << ", k), (x << k) & mask);
    const std::int64_t sx = is_signed ? signed_of(x, size) : static_cast<std::int64_t>(x);
    // An unsigned 64-bit x shifts as an unsigned value.
    const std::uint64_t shifted = is_signed || bits < 64 ? static_cast<std::uint64_t>(sx >> k) : x >> k;
    ops.emplace_back(cat("r", k), cat("x >> ", k), shifted & mask);
  }
  for (int c : kMultipliers) ops.emplace_back(cat("m", c), cat("x * ", c), (x * c) & mask);
  const std::string lit = cat(x, size == 8 ? "ULL" : "UL");
  std::vector<Program> out;
  for (std::size_t start = 0; start < ops.size(); start += kChunk) {
    Program p;
    std::string calls;
    for (std::size_t k = start; k < std::min(ops.size(), start + kChunk); ++k) {
      const auto& [name, expr, want] = ops[k];
      p.source += cat(ctype, " ", name, ";\n", "void f_", name, "(", ctype, " x) { ", name, " = ", expr, "; }\n");
      calls += cat("    f_", name, "(v);\n");
      p.expected.emplace_back(name, want);
    }
    p.source += cat("int main(void) {\n    ", ctype, " v = (", ctype, ")", lit, ";\n") + calls + "    return 0;\n}\n";
    out.push_back(std::move(p));
  }
  return out;
}

std::uint64_t truncated(std::uint64_t raw, int size) { return raw & mask_of(size); }

void check(bool optimize) {
  for (const auto& [ctype, size] : kTypes) {
    for (std::uint64_t raw : kInputs) {
      const std::uint64_t x = truncated(raw, size);
      for (const Program& p : programs(ctype, size, x)) {
        INFO("type=" << ctype << " x=" << x << " first=" << p.expected.front().first);
        sim::Simulation s = build_sim(p.source, optimize);
        s.run();
        for (const auto& [name, want] : p.expected) {
          const std::int64_t addr = s.symbol(name);
          std::uint64_t got = 0;
          for (int b = 0; b < size; ++b) got |= std::uint64_t{s.cpu.memory[addr + b]} << (8 * b);
          CHECK_MESSAGE(got == want, name);
        }
      }
    }
  }
}

TEST_CASE("const_shift_mul_sim: the generator writes the programs the Python one wrote") {
  // tests/test_const_shift_mul_sim.py's `_programs` wrote 36 programs on
  // 27 Nov 2026, and this is zlib's CRC-32 of their sources concatenated
  // in the order `check` runs them.
  std::string all;
  std::size_t count = 0;
  for (const auto& [ctype, size] : kTypes) {
    for (std::uint64_t raw : kInputs) {
      for (const Program& p : programs(ctype, size, truncated(raw, size))) {
        all += p.source;
        ++count;
      }
    }
  }
  CHECK(count == 36);
  CHECK(sim::cpu_run::crc32(all) == 0x87799016u);
}

TEST_CASE("const_shift_mul_sim: every constant shift and product, unoptimized") { check(false); }

TEST_CASE("const_shift_mul_sim: every constant shift and product, optimized") { check(true); }

// `(uint8_t)(x << k)` narrows to a 1-byte shift, which runs in A.
TEST_CASE("const_shift_mul_sim: a narrowed left shift") {
  const std::vector<int> xs = {0x00, 0x01, 0x5A, 0x80, 0xA5, 0xFF};
  std::string src = "unsigned char r[56];\n";
  src += "unsigned char xs[6] = {0, 1, 90, 128, 165, 255};\n";
  for (int k = 1; k < 8; ++k) {
    src += cat("unsigned char s", k, "(unsigned char x) { return (unsigned char)(x << ", k, "); }\n");
  }
  src += "int main(void) {\n    for (unsigned char i = 0; i < 6; i++) {\n";
  for (int k = 1; k < 8; ++k) src += cat("        r[i * 7 + ", k - 1, "] = s", k, "(xs[i]);\n");
  src += "    }\n    return 0;\n}\n";
  // zlib's CRC-32 of the program tests/test_const_shift_mul_sim.py ran,
  // on 27 Nov 2026.
  CHECK(sim::cpu_run::crc32(src) == 0x6A69984Bu);
  for (bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    sim::Simulation s = build_sim(src, optimize);
    s.run();
    const std::int64_t base = s.symbol("r");
    for (std::size_t i = 0; i < xs.size(); ++i) {
      for (int k = 1; k < 8; ++k) {
        const int got = s.cpu.memory[base + static_cast<std::int64_t>(i) * 7 + k - 1];
        CHECK_MESSAGE(got == ((xs[i] << k) & 0xFF), cat(xs[i], " << ", k));
      }
    }
  }
}

}  // namespace
}  // namespace c6502::suite
