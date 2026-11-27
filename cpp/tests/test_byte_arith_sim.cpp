// `tac_to_asm` lowers an unsigned byte divided by a constant as a loop
// of subtractions, a 1- or 2-byte shift by a variable count, or by a
// constant count that `inline_shift_ok` rejects, as a loop of one-bit
// shifts, and a 16-bit product of two bytes as a shift-and-add loop.
// Under -O, `narrow_byte_range` makes the divides, remainders, right
// shifts, and products of promoted bytes into those forms, and strength
// reduction makes a signed divide or remainder of a nonnegative value by
// a power of two a shift or a mask.
//
// Each case is a function of one or two arguments whose results a
// `main` stores into an array, for every byte value of the first
// argument (or a spread of 16-bit values) and a list of values of the
// second. A count of the width or more is included, and the expected
// result for it is the shift helpers', 0 for a left or logical shift and
// every bit a copy of the sign for an arithmetic one, which C leaves
// undefined. An unsigned `_BitInt(8)` isn't promoted, so it reaches the
// byte lowerings in the unoptimized pipeline too.
#include <algorithm>
#include <cstdint>
#include <functional>
#include <optional>
#include <set>
#include <string>
#include <vector>

#include "doctest.h"
#include "sim/cpu_run.h"
#include "sim_case.h"

namespace c6502::suite {

namespace {

using Values = std::vector<std::int64_t>;
using Model = std::function<std::int64_t(std::int64_t, std::int64_t)>;

std::int64_t wrap(std::int64_t v, int bits) { return v & ((std::int64_t{1} << bits) - 1); }

std::int64_t to_signed(std::int64_t v, int bits) {
  v = wrap(v, bits);
  return (v >> (bits - 1)) ? v - (std::int64_t{1} << bits) : v;
}

std::int64_t shl(std::int64_t v, std::int64_t n, int bits) { return n >= bits ? 0 : wrap(v << n, bits); }
std::int64_t lsr(std::int64_t v, std::int64_t n, int bits) { return n >= bits ? 0 : wrap(v, bits) >> n; }
std::int64_t asr(std::int64_t v, std::int64_t n, int bits) {
  return wrap(to_signed(v, bits) >> std::min<std::int64_t>(n, bits - 1), bits);
}

// Python's `//` and `%`, for a divisor that is positive.
std::int64_t floordiv(std::int64_t a, std::int64_t b) { return a / b - ((a % b != 0) && ((a < 0) != (b < 0))); }
std::int64_t pymod(std::int64_t a, std::int64_t b) { return a - b * floordiv(a, b); }

struct Case {
  std::string name;
  std::string decl;
  int size;
  Values xs;
  std::optional<Values> ys;
  Model model;
};

Values range(std::int64_t n) {
  Values v;
  for (std::int64_t k = 0; k < n; ++k) v.push_back(k);
  return v;
}

// 16-bit values with every byte pattern that matters for a shift: the
// sign bit, carries between the bytes, and both bytes nonzero.
Values words() {
  std::set<std::int64_t> s = {0,      1,      2,      3,      0x7F,   0x80,   0xFF,   0x100,  0x101, 0x1234,
                              0x4000, 0x7FFF, 0x8000, 0x8001, 0xA5C3, 0xC000, 0xFF00, 0xFFFE, 0xFFFF};
  for (std::int64_t k = 0; k < 45; ++k) s.insert((k * 0x9E37 + 0x79B9) & 0xFFFF);
  return Values(s.begin(), s.end());
}

std::vector<Case> cases() {
  const Values bytes = range(256);
  const Values wds = words();
  std::vector<Case> out;
  const auto add = [&](std::string name, std::string decl, int size, Values xs, std::optional<Values> ys,
                       Model model) {
    out.push_back({std::move(name), std::move(decl), size, std::move(xs), std::move(ys), std::move(model)});
  };
  for (const std::int64_t c : {1, 2, 3, 5, 7, 8, 10, 20, 37, 100, 128, 255}) {
    const std::string cs = std::to_string(c);
    add("udiv" + cs, "uint8_t f(uint8_t x) { return x / " + cs + "; }", 1, bytes, std::nullopt,
        [c](std::int64_t x, std::int64_t) { return x / c; });
    add("umod" + cs, "uint8_t f(uint8_t x) { return x % " + cs + "; }", 1, bytes, std::nullopt,
        [c](std::int64_t x, std::int64_t) { return x % c; });
  }
  for (const std::int64_t c : {3, 10, 20}) {
    const std::string cs = std::to_string(c);
    // A quotient and a remainder of the same operands share one loop.
    add("udivmod" + cs,
        "uint16_t f(uint8_t x) { uint8_t q = x / " + cs + "; uint8_t r = x % " + cs +
            "; return r | (uint16_t)q << 8; }",
        2, bytes, std::nullopt, [c](std::int64_t x, std::int64_t) { return (x % c) | (x / c) << 8; });
    add("bdiv" + cs, "unsigned _BitInt(8) f(unsigned _BitInt(8) x) { return x / " + cs + "uwb; }", 1, bytes,
        std::nullopt, [c](std::int64_t x, std::int64_t) { return x / c; });
    add("bmod" + cs, "unsigned _BitInt(8) f(unsigned _BitInt(8) x) { return x % " + cs + "uwb; }", 1, bytes,
        std::nullopt, [c](std::int64_t x, std::int64_t) { return x % c; });
  }
  add("udiv_var", "uint8_t f(uint8_t x, uint8_t y) { return x / y; }", 1, bytes, Values{1, 2, 3, 7, 16, 200, 255},
      [](std::int64_t x, std::int64_t y) { return x / y; });
  add("umod_var", "uint8_t f(uint8_t x, uint8_t y) { return x % y; }", 1, bytes, Values{1, 2, 3, 7, 16, 200, 255},
      [](std::int64_t x, std::int64_t y) { return x % y; });

  // Signed divides of a nonnegative dividend.
  add("half_up", "uint8_t f(uint8_t s) { return (s + 1) / 2; }", 1, bytes, std::nullopt,
      [](std::int64_t s, std::int64_t) { return (s + 1) / 2; });
  add("mod_after_add", "int f(uint8_t s) { return (s + 3) % 4; }", 2, bytes, std::nullopt,
      [](std::int64_t s, std::int64_t) { return (s + 3) % 4; });
  add("div_wide", "int f(uint8_t s) { return (s + 100) / 16; }", 2, bytes, std::nullopt,
      [](std::int64_t s, std::int64_t) { return (s + 100) / 16; });

  const Values counts8 = range(11);
  const Values counts_promoted = range(16);
  const Values counts16 = range(18);

  add("shl_u8", "uint8_t f(uint8_t x, uint8_t n) { return x << n; }", 1, bytes, counts_promoted,
      [](std::int64_t x, std::int64_t n) { return shl(x, n, 8); });
  add("shr_u8", "uint8_t f(uint8_t x, uint8_t n) { return x >> n; }", 1, bytes, counts_promoted,
      [](std::int64_t x, std::int64_t n) { return lsr(x, n, 8); });
  add("shl_b8", "unsigned _BitInt(8) f(unsigned _BitInt(8) x, uint8_t n) { return x << n; }", 1, bytes, counts8,
      [](std::int64_t x, std::int64_t n) { return shl(x, n, 8); });
  add("lsr_b8", "unsigned _BitInt(8) f(unsigned _BitInt(8) x, uint8_t n) { return x >> n; }", 1, bytes, counts8,
      [](std::int64_t x, std::int64_t n) { return lsr(x, n, 8); });
  add("asr_b8", "_BitInt(8) f(_BitInt(8) x, uint8_t n) { return x >> n; }", 1, bytes, counts8,
      [](std::int64_t x, std::int64_t n) { return asr(x, n, 8); });
  add("asr_b8_3", "_BitInt(8) f(_BitInt(8) x) { return x >> 3; }", 1, bytes, std::nullopt,
      [](std::int64_t x, std::int64_t) { return asr(x, 3, 8); });
  for (const std::int64_t k : {2, 3, 4, 6}) {
    // A promoted `signed char` shifted and truncated narrows to a byte
    // shift unless only the wide one is inline.
    add("asr_s8_" + std::to_string(k), "int8_t f(int8_t x) { return x >> " + std::to_string(k) + "; }", 1, bytes,
        std::nullopt, [k](std::int64_t x, std::int64_t) { return asr(x, k, 8); });
  }
  add("shl_int_wide", "unsigned int f(uint8_t x, uint8_t n) { return x << n; }", 2, bytes, counts_promoted,
      [](std::int64_t x, std::int64_t n) { return shl(x, n, 16); });
  add("shl_u16", "unsigned int f(unsigned int x, uint8_t n) { return x << n; }", 2, wds, counts16,
      [](std::int64_t x, std::int64_t n) { return shl(x, n, 16); });
  add("lsr_u16", "unsigned int f(unsigned int x, uint8_t n) { return x >> n; }", 2, wds, counts16,
      [](std::int64_t x, std::int64_t n) { return lsr(x, n, 16); });
  add("asr_i16", "int f(int x, uint8_t n) { return x >> n; }", 2, wds, counts16,
      [](std::int64_t x, std::int64_t n) { return asr(x, n, 16); });
  add("asr_i16_5", "int f(int x) { return x >> 5; }", 2, wds, std::nullopt,
      [](std::int64_t x, std::int64_t) { return asr(x, 5, 16); });
  add("lsr_u16_6", "unsigned int f(unsigned int x) { return x >> 6; }", 2, wds, std::nullopt,
      [](std::int64_t x, std::int64_t) { return lsr(x, 6, 16); });
  add("shl_u16_6", "unsigned int f(unsigned int x) { return x << 6; }", 2, wds, std::nullopt,
      [](std::int64_t x, std::int64_t) { return shl(x, 6, 16); });
  add("mask_shift", "uint8_t f(uint8_t n) { return (0x7F >> n) << n; }", 1, range(10), std::nullopt,
      [](std::int64_t n, std::int64_t) { return wrap((0x7F >> n) << n, 8); });

  // A left shift whose result stays below 256 is a byte shift
  // (`narrow_byte_range`), as in Ali Baba's `hrcg_row_address`.
  add("row_address", "uint16_t f(uint8_t row) { return 0x80 * (row % 8) + 0x28 * (row / 8); }", 2, bytes,
      std::nullopt, [](std::int64_t r, std::int64_t) { return 0x80 * (r % 8) + 0x28 * (r / 8); });
  add("shift_back", "unsigned int f(uint8_t b) { return (b >> 3) << 3; }", 2, bytes, std::nullopt,
      [](std::int64_t b, std::int64_t) { return (b >> 3) << 3; });

  const Values multipliers = {0, 1, 2, 3, 4, 5, 7, 8, 15, 16, 100, 127, 128, 200, 254, 255};
  add("mul8x8", "uint16_t f(uint8_t a, uint8_t b) { return a * b; }", 2, bytes, multipliers,
      [](std::int64_t a, std::int64_t b) { return a * b; });
  add("mul_tier", "uint16_t f(uint8_t p, uint8_t s) { return p * (((s >> 5) & 3) + 1); }", 2, bytes,
      Values{0x00, 0x1F, 0x20, 0x45, 0x60, 0x9F, 0xE0, 0xFF},
      [](std::int64_t p, std::int64_t s) { return p * (((s >> 5) & 3) + 1); });

  // A 16-bit quotient and remainder of the same operands share one
  // helper call.
  add("udivmod16",
      "uint16_t q; uint16_t r;\n"
      "uint16_t f(uint16_t v, uint16_t p) { q = v / p; v %= p; return q ^ v; }",
      2, wds, Values{1, 3, 10, 255, 256, 1000, 0x7FFF, 0x8000, 0xFFFF},
      [](std::int64_t v, std::int64_t p) { return floordiv(v, p) ^ pymod(v, p); });
  return out;
}

std::string join(const Values& v) {
  std::string s;
  for (std::size_t k = 0; k < v.size(); ++k) s += (k ? ", " : "") + std::to_string(v[k]);
  return s;
}

std::string program(const Case& c) {
  const std::string rtype = c.size == 1 ? "uint8_t" : "uint16_t";
  const bool big = *std::max_element(c.xs.begin(), c.xs.end()) > 0xFF;
  const std::string xtype = big ? "uint16_t" : "uint8_t";
  const std::string nx = std::to_string(c.xs.size());
  std::string s = "#include <stdint.h>\n" + c.decl + "\n" + "static const " + xtype + " XS[" + nx + "] = {" +
                  join(c.xs) + "};\n";
  std::string body;
  if (!c.ys) {
    s += rtype + " OUT[" + nx + "];\n";
    body = "    for (uint16_t i = 0; i < " + nx + "; i++)\n        OUT[i] = f(XS[i]);\n";
  } else {
    const Values& ys = *c.ys;
    const std::string ytype = *std::max_element(ys.begin(), ys.end()) > 0xFF ? "uint16_t" : "uint8_t";
    s += "static const " + ytype + " YS[" + std::to_string(ys.size()) + "] = {" + join(ys) + "};\n";
    s += rtype + " OUT[" + std::to_string(c.xs.size() * ys.size()) + "];\n";
    body = "    uint16_t k = 0;\n    for (uint16_t i = 0; i < " + nx + "; i++)\n        for (uint8_t j = 0; j < " +
           std::to_string(ys.size()) + "; j++)\n            OUT[k++] = f(XS[i], YS[j]);\n";
  }
  s += "int main(void) {\n" + body + "    return 0;\n}\n";
  return s;
}

void check_cases(bool optimize) {
  for (const Case& c : cases()) {
    INFO("case=" << c.name);
    sim::Simulation s = build_sim(program(c), optimize);
    const sim::SimResult result = s.run(40'000'000);
    CHECK_FALSE(result.timed_out);
    const std::int64_t base = s.symbol("OUT");
    std::vector<std::pair<std::int64_t, std::optional<std::int64_t>>> pairs;
    for (const std::int64_t x : c.xs) {
      if (!c.ys) {
        pairs.emplace_back(x, std::nullopt);
      } else {
        for (const std::int64_t y : *c.ys) pairs.emplace_back(x, y);
      }
    }
    int failures = 0;
    for (std::size_t k = 0; k < pairs.size(); ++k) {
      const auto [x, y] = pairs[k];
      const std::uint64_t got = result.word(base + static_cast<std::int64_t>(k) * c.size, c.size);
      const std::int64_t want = wrap(c.model(x, y.value_or(0)), 8 * c.size);
      if (got != static_cast<std::uint64_t>(want) && failures++ < 1) {
        FAIL_CHECK(c.name << "(" << x << ", " << (y ? std::to_string(*y) : "None") << "): got " << got << ", want "
                          << want);
      }
    }
  }
}

}  // namespace

TEST_CASE("byte_arith_sim: the generator writes the programs the Python one wrote") {
  // The count and the CRC-32 of the programs concatenated, from
  // tests/test_byte_arith_sim.py's `_program` over `_CASES` on 27 Nov
  // 2026.
  std::string all;
  std::size_t n = 0;
  for (const Case& c : cases()) {
    all += program(c);
    ++n;
  }
  CHECK(n == 61);
  CHECK(sim::cpu_run::crc32(all) == 0x3DD7359Du);
}

TEST_CASE("byte_arith_sim: each lowering matches the model, unoptimized") { check_cases(false); }

TEST_CASE("byte_arith_sim: each lowering matches the model, optimized") { check_cases(true); }

}  // namespace c6502::suite
