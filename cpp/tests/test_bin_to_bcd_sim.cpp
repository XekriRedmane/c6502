// `__c6502_bin_to_bcd` and its runtime helper `bin16_to_bcd` in the
// simulator, from test_bin_to_bcd.py: the model against the decimal
// digits of every value, the helper's 6502 code on every input with its
// cycles, and every value through the builtin in both pipelines.
#include <cstdio>
#include <string>
#include <vector>

#include "builtin_functions.h"
#include "doctest.h"
#include "sim_case.h"

namespace c6502::suite {
namespace {

// The five decimal digits of `v` read as hexadecimal, which is what
// packed BCD is.
std::uint32_t decimal_digits(unsigned v) {
  char buf[8];
  std::snprintf(buf, sizeof buf, "%05u", v);
  return static_cast<std::uint32_t>(std::stoul(buf, nullptr, 16));
}

// A CPU with the helpers at $E000 and `JSR bin16_to_bcd; BRK` at $0200.
struct HelperRunner {
  sim::Cpu cpu;
  std::uint16_t hargs = 0x04;
  std::int64_t addr = 0;

  HelperRunner() {
    const sim::Runtime rt = sim::build_runtime();
    std::copy(rt.helper_image.begin(), rt.helper_image.end(), cpu.memory.begin() + rt.helper_origin);
    addr = rt.symbols.at("bin16_to_bcd");
    const std::uint8_t call[] = {0x20, static_cast<std::uint8_t>(addr), static_cast<std::uint8_t>(addr >> 8), 0x00};
    std::copy(std::begin(call), std::end(call), cpu.memory.begin() + 0x200);
  }

  struct Ran {
    std::vector<std::uint8_t> hargs;
    std::uint8_t p;
    std::uint64_t cycles;
  };

  // Runs the helper on `v` from HARGS bytes that aren't zero, and returns
  // HARGS+0..6, P, and the cycles of the call with its JSR and RTS.
  Ran run(unsigned v) {
    const std::uint8_t start[] = {static_cast<std::uint8_t>(v), static_cast<std::uint8_t>(v >> 8), 0x5A, 0xA5,
                                  0x3C, 0xC3, 0x99};
    std::copy(std::begin(start), std::end(start), cpu.memory.begin() + hargs);
    cpu.pc = 0x200;
    cpu.sp = 0xFF;
    cpu.p = sim::Cpu::kUnused;
    const std::uint64_t before = cpu.cycles;
    while (cpu.pc != 0x203) cpu.step();
    return {std::vector<std::uint8_t>(cpu.memory.begin() + hargs, cpu.memory.begin() + hargs + 7), cpu.p,
            cpu.cycles - before};
  }
};

// What the helper leaves in HARGS+0..6: the input, its three BCD bytes,
// and the copy the loop shifts to zero.
std::vector<std::uint8_t> helper_result(unsigned v) {
  const std::uint32_t b = decimal_digits(v);
  return {static_cast<std::uint8_t>(v),       static_cast<std::uint8_t>(v >> 8), static_cast<std::uint8_t>(b),
          static_cast<std::uint8_t>(b >> 8), static_cast<std::uint8_t>(b >> 16), 0, 0};
}

void helper_shard(unsigned k) {
  HelperRunner runner;
  int bad = 0;
  for (unsigned v = k << 14; v < (k + 1) << 14; ++v) {
    const HelperRunner::Ran r = runner.run(v);
    if (r.hargs != helper_result(v) || (r.p & sim::Cpu::kDecimal) != 0) {
      if (++bad <= 5) FAIL_CHECK("bin16_to_bcd of " << v);
    }
  }
  CHECK(bad == 0);
}

constexpr unsigned kBatch = 8192;

// A program that stores the three BCD bytes of each value from `start`
// to `start + kBatch - 1` in `out`.
std::string batch_source(unsigned start) {
  const std::string n = std::to_string(start);
  return "#include <stdint.h>\n"
         "uint8_t out[" + std::to_string(3 * kBatch) + "];\n"
         "int main(void) {\n"
         "    uint8_t *p = out;\n"
         "    uint16_t v = " + n + ";\n"
         "    do {\n"
         "        uint32_t b = __c6502_bin_to_bcd(v);\n"
         "        p[0] = (uint8_t)b;\n"
         "        p[1] = (uint8_t)(b >> 8);\n"
         "        p[2] = (uint8_t)(b >> 16);\n"
         "        if ((uint8_t)(b >> 24) != 0) return 1;\n"
         "        p += 3;\n"
         "        v++;\n"
         "    } while (v != (uint16_t)" + std::to_string(start + kBatch) + ");\n"
         "    return 0;\n"
         "}\n";
}

void batch(unsigned k, bool optimize) {
  const unsigned start = k * kBatch;
  sim::Simulation s = build_sim(batch_source(start), optimize);
  const sim::SimResult r = s.run(40'000'000);
  CHECK_FALSE(r.timed_out);
  CHECK((s.cpu.p & sim::Cpu::kDecimal) == 0);
  CHECK(r.return_int() == 0);
  const std::int64_t base = s.symbol("out");
  for (unsigned v = start; v < start + kBatch; ++v) {
    const std::uint32_t b = decimal_digits(v);
    const std::size_t at = static_cast<std::size_t>(base + 3 * (v - start));
    if (r.memory[at] != (b & 0xFF) || r.memory[at + 1] != ((b >> 8) & 0xFF) || r.memory[at + 2] != (b >> 16)) {
      FAIL("first difference at " << v);
    }
  }
}

}  // namespace

TEST_CASE("bin_to_bcd sim: the model gives every value's decimal digits") {
  for (unsigned v = 0; v < 0x10000; ++v) {
    if (builtin_functions::bin_to_bcd(v) != decimal_digits(v)) FAIL("bin_to_bcd(" << v << ")");
  }
}

TEST_CASE("bin_to_bcd sim: the model takes the low 16 bits") {
  CHECK(builtin_functions::bin_to_bcd(-1) == 0x65535);
  CHECK(builtin_functions::bin_to_bcd(70000) == 0x04464);
}

TEST_CASE("bin_to_bcd sim: the helper on the inputs $0000-$3FFF") { helper_shard(0); }
TEST_CASE("bin_to_bcd sim: the helper on the inputs $4000-$7FFF") { helper_shard(1); }
TEST_CASE("bin_to_bcd sim: the helper on the inputs $8000-$BFFF") { helper_shard(2); }
TEST_CASE("bin_to_bcd sim: the helper on the inputs $C000-$FFFF") { helper_shard(3); }

TEST_CASE("bin_to_bcd sim: the helper's cycles") {
  // docs/builtins.md gives the cycles. The loop takes the same time for
  // every input: 712 cycles with the JSR and the RTS, and 15 more when
  // the loop's BNE crosses a page, as it does where the simulator puts
  // the helper. The loop starts 19 bytes in, and its BNE ends 44 bytes
  // in.
  HelperRunner runner;
  const bool crosses = (runner.addr + 19) >> 8 != (runner.addr + 44) >> 8;
  for (const unsigned v : {0u, 1u, 9999u, 65535u}) CHECK(runner.run(v).cycles == 712 + 15 * (crosses ? 1 : 0));
}

TEST_CASE("bin_to_bcd sim: a sum of the builtin's results in both pipelines") {
  const std::string source =
      "#include <stdint.h>\n"
      "uint16_t v[4] = {0, 9, 65535, 31415};\n"
      "uint32_t f(uint16_t x) { return __c6502_bin_to_bcd(x); }\n"
      "int main(void) {\n"
      "    uint32_t s = 0;\n"
      "    for (uint8_t i = 0; i < 4; i++) s ^= f(v[i]) + i;\n"
      "    return (int)(s ^ (s >> 16));\n"
      "}\n";
  std::uint32_t s = 0;
  const unsigned values[] = {0, 9, 65535, 31415};
  for (std::uint32_t i = 0; i < 4; ++i) s ^= decimal_digits(values[i]) + i;
  const std::int64_t want = (s ^ (s >> 16)) & 0xFFFF;
  for (const bool optimize : {false, true}) {
    INFO("optimize " << optimize);
    CHECK(build_sim(source, optimize).run().return_int() == want);
  }
}

TEST_CASE("shard: bin_to_bcd batch 0") { for (bool o : {false, true}) batch(0, o); }
TEST_CASE("shard: bin_to_bcd batch 1") { for (bool o : {false, true}) batch(1, o); }
TEST_CASE("shard: bin_to_bcd batch 2") { for (bool o : {false, true}) batch(2, o); }
TEST_CASE("shard: bin_to_bcd batch 3") { for (bool o : {false, true}) batch(3, o); }
TEST_CASE("shard: bin_to_bcd batch 4") { for (bool o : {false, true}) batch(4, o); }
TEST_CASE("shard: bin_to_bcd batch 5") { for (bool o : {false, true}) batch(5, o); }
TEST_CASE("shard: bin_to_bcd batch 6") { for (bool o : {false, true}) batch(6, o); }
TEST_CASE("shard: bin_to_bcd batch 7") { for (bool o : {false, true}) batch(7, o); }

}  // namespace c6502::suite
