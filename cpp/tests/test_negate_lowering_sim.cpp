// The lowering of an integer negate, `0 - x` a byte at a time, run in
// both pipelines on values at the edges of each byte, from
// tests/test_negate_lowering.py.
#include <cstdint>
#include <cstdio>
#include <string>
#include <vector>

#include "doctest.h"
#include "sim/cpu_run.h"
#include "sim_case.h"

namespace c6502::suite {

namespace {

struct NegateCase {
  const char* type;
  int bits;
  std::vector<std::int64_t> values;
};

const std::vector<NegateCase> kCases = {
    {"int", 16, {0, 1, -1, 255, 256, -256, 32767, -32768}},
    {"unsigned int", 16, {0, 1, 0xFF, 0x100, 0x7FFF, 0x8000, 0xFFFF}},
    {"long", 32, {0, 1, -1, 0xFFFF, 0x10000, 2147483647LL, -2147483648LL}},
    {"long long", 64, {0, 1, -1, 4294967296LL, INT64_MAX, INT64_MIN}},
};

std::uint64_t mask_of(int bits) { return bits == 64 ? ~std::uint64_t{0} : (std::uint64_t{1} << bits) - 1; }

// Each value is written as its bit pattern, since C has no literal for
// the most negative value of a type.
std::string program(const NegateCase& c) {
  const std::string n = std::to_string(c.values.size());
  std::string table;
  for (std::size_t k = 0; k < c.values.size(); ++k) {
    char hex[24];
    std::snprintf(hex, sizeof hex, "%llX",
                  static_cast<unsigned long long>(static_cast<std::uint64_t>(c.values[k]) & mask_of(c.bits)));
    if (k > 0) table += ", ";
    table += std::string("(") + c.type + ")0x" + hex + "ULL";
  }
  return std::string(c.type) + " t[" + n + "] = {" + table + "};\n" + c.type + " r[" + n +
         "];\n"
         "int main(void) {\n"
         "    for (unsigned char i = 0; i < " +
         n +
         "; i++)\n"
         "        r[i] = -t[i];\n"
         "    return 0;\n"
         "}\n";
}

}  // namespace

TEST_CASE("negate_lowering sim: each negate gives the 2's complement") {
  // The CRC-32 of tests/test_negate_lowering.py's `_program` for each of
  // its cases, concatenated, computed on 27 Nov 2026, shows that the
  // generator writes the programs the Python module wrote.
  std::string all;
  for (const NegateCase& c : kCases) all += program(c);
  CHECK(sim::cpu_run::crc32(all) == 3468977128);
  for (const NegateCase& c : kCases) {
    const std::string src = program(c);
    for (const bool optimize : {false, true}) {
      INFO("type=" << c.type << " optimize=" << optimize);
      sim::Simulation s = build_sim(src, optimize);
      const sim::SimResult r = s.run();
      const int size = c.bits / 8;
      const std::int64_t base = s.symbol("r");
      for (std::size_t k = 0; k < c.values.size(); ++k) {
        INFO("-(" << c.values[k] << ")");
        const std::uint64_t want = (std::uint64_t{0} - static_cast<std::uint64_t>(c.values[k])) & mask_of(c.bits);
        CHECK(r.word(base + size * static_cast<std::int64_t>(k), size) == want);
      }
    }
  }
}

}  // namespace c6502::suite
