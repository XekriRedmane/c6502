// `x < 0` and `x >= 0` kept as values for a signed `x`, and the unsigned
// orderings kept as values, run in the simulator in both pipelines, from
// tests/test_sign_test_lowering.py.
#include <cstdint>
#include <cstdio>
#include <iterator>
#include <string>
#include <vector>

#include "doctest.h"
#include "sim/cpu_run.h"
#include "sim_case.h"

namespace c6502::suite {

namespace {

struct SignedCase {
  const char* ctype;
  int bits;
  std::vector<std::int64_t> values;
};

// For each type, values at the edges of the sign bit.
const std::vector<SignedCase> kCases = {
    {"signed char", 8, {0, 1, -1, 127, -128}},
    {"int", 16, {0, 1, -1, 255, 256, -256, 32767, -32768}},
    {"long", 32, {0, 1, -1, 0xFFFF, (1LL << 31) - 1, -(1LL << 31)}},
};

// Each value is written as its bit pattern, since C has no literal for
// the most negative value of a type. `lt` and `ge` are stored through
// globals, so the compares are kept as values.
std::string signed_program(const SignedCase& c) {
  const std::uint64_t mask = (std::uint64_t{1} << c.bits) - 1;
  const std::string n = std::to_string(c.values.size());
  std::string table;
  for (const std::int64_t v : c.values) {
    char hex[32];
    std::snprintf(hex, sizeof hex, "%llX", static_cast<unsigned long long>(static_cast<std::uint64_t>(v) & mask));
    table += (table.empty() ? "" : ", ") + std::string("(") + c.ctype + ")0x" + hex + "UL";
  }
  return std::string(c.ctype) + " t[" + n + "] = {" + table + "};\n" + "int lt[" + n + "];\n" + "bool ge[" + n +
         "];\n" + "int main(void) {\n" + "    for (unsigned char i = 0; i < " + n + "; i++) {\n" +
         "        lt[i] = t[i] < 0;\n" + "        ge[i] = 0 <= t[i];\n" + "    }\n" + "    return 0;\n" + "}\n";
}

const std::pair<std::uint64_t, std::uint64_t> kUnsignedPairs[] = {
    {0, 0}, {0, 1}, {1, 0}, {5, 5}, {0x7F, 0x80}, {0x80, 0x7F}, {0xFF, 0}, {0, 0xFF}, {0xFF, 0xFF}};

// The pair `x`, `y` in the top bytes of a type of `bits` bits, with
// equal low bytes, so the high bytes decide for the wider types.
std::pair<std::uint64_t, std::uint64_t> unsigned_values(int bits, std::uint64_t x, std::uint64_t y) {
  if (bits == 8) return {x, y};
  const std::uint64_t scale = std::uint64_t{1} << (bits - 8);
  return {x * scale + x, y * scale + x};
}

std::string unsigned_program(const std::string& ctype, int bits) {
  std::string a, b;
  for (const auto& [x, y] : kUnsignedPairs) {
    const auto [va, vb] = unsigned_values(bits, x, y);
    a += (a.empty() ? "" : ", ") + ("(" + ctype + ")" + std::to_string(va) + "U");
    b += (b.empty() ? "" : ", ") + ("(" + ctype + ")" + std::to_string(vb) + "U");
  }
  const std::string n = std::to_string(std::size(kUnsignedPairs));
  return ctype + " a[" + n + "] = {" + a + "};\n" + ctype + " b[" + n + "] = {" + b + "};\n" +
         "unsigned char r[" + n + "][4];\n" + "int main(void) {\n" + "    for (unsigned char i = 0; i < " + n +
         "; i++) {\n" + "        r[i][0] = a[i] < b[i];\n" + "        r[i][1] = a[i] > b[i];\n" +
         "        r[i][2] = a[i] <= b[i];\n" + "        r[i][3] = a[i] >= b[i];\n" + "    }\n" +
         "    return 0;\n" + "}\n";
}

}  // namespace

TEST_CASE("sign_test_lowering sim: x < 0 and x >= 0 at the edges of the sign bit") {
  std::string all;
  for (const SignedCase& c : kCases) {
    const std::string src = signed_program(c);
    all += src;
    for (const bool optimize : {false, true}) {
      INFO("type=" << c.ctype << " optimize=" << optimize);
      sim::Simulation s = build_sim(src, optimize);
      const sim::SimResult r = s.run();
      const std::int64_t lt = s.symbol("lt");
      const std::int64_t ge = s.symbol("ge");
      for (std::size_t k = 0; k < c.values.size(); ++k) {
        const std::int64_t v = c.values[k];
        INFO("v=" << v);
        CHECK(r.word(lt + 2 * static_cast<std::int64_t>(k)) == (v < 0 ? 1u : 0u));
        CHECK(r.memory[ge + k] == (v >= 0 ? 1 : 0));
      }
    }
  }
  // The CRC-32 of the three programs tests/test_sign_test_lowering.py
  // generated on 27 Nov 2026.
  CHECK(sim::cpu_run::crc32(all) == 0x917ba95cu);
}

TEST_CASE("sign_test_lowering sim: the unsigned orderings kept as values") {
  std::string all;
  const std::pair<const char*, int> types[] = {{"unsigned char", 8}, {"unsigned int", 16}, {"unsigned long", 32}};
  for (const auto& [ctype, bits] : types) {
    const std::string src = unsigned_program(ctype, bits);
    all += src;
    for (const bool optimize : {false, true}) {
      INFO("type=" << ctype << " optimize=" << optimize);
      sim::Simulation s = build_sim(src, optimize);
      const sim::SimResult r = s.run();
      const std::int64_t base = s.symbol("r");
      for (std::size_t k = 0; k < std::size(kUnsignedPairs); ++k) {
        const auto [va, vb] = unsigned_values(bits, kUnsignedPairs[k].first, kUnsignedPairs[k].second);
        INFO("va=" << va << " vb=" << vb);
        const std::vector<int> got = {r.memory[base + 4 * k], r.memory[base + 4 * k + 1], r.memory[base + 4 * k + 2],
                                      r.memory[base + 4 * k + 3]};
        const std::vector<int> want = {va < vb, va > vb, va <= vb, va >= vb};
        CHECK(got == want);
      }
    }
  }
  // The CRC-32 of the three programs tests/test_sign_test_lowering.py
  // generated on 27 Nov 2026.
  CHECK(sim::cpu_run::crc32(all) == 0x86881620u);
}

TEST_CASE("sign_test_lowering sim: a function that returns a compare in the carry") {
  const char* src = R"c(#include <stdbool.h>
#include <stdint.h>
uint8_t level, score;
[[c6502::return_bool_in_carry]]
static bool beats(uint8_t l, uint8_t s) {
    if (level != l)
        return level > l;
    return score > s;
}
int main(void) {
    int n = 0;
    level = 3; score = 7;
    if (beats(2, 9)) n += 1;
    if (!beats(4, 0)) n += 2;
    if (beats(3, 6)) n += 4;
    if (!beats(3, 7)) n += 8;
    return n;
}
)c";
  for (const bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    CHECK(build_sim(src, optimize).run().return_int() == 15);
  }
}

}  // namespace c6502::suite
