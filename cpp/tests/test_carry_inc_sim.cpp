// The carry-INC peephole (src/passes/carry_inc.h) and the coalescing that
// sets it up, run in both pipelines, from tests/test_carry_inc.py. Each
// add and subtract runs across values where the carry or borrow does
// and doesn't happen, since the two branches of the rewrite are
// different code paths.
#include <string>
#include <utility>
#include <vector>

#include "compile_case.h"
#include "doctest.h"
#include "sim/cpu_run.h"
#include "sim_case.h"

namespace c6502::suite {

namespace {

const std::vector<std::uint64_t> kBytes = {0x00, 0x01, 0x7F, 0x80, 0xFE, 0xFF};
const std::vector<std::uint64_t> kWords = {0x0000, 0x00FF, 0x0100, 0x7FFF, 0x80F0, 0xFF01, 0xFFFF};

// Returns the program that computes `w op c` for every pair of `words`
// and kBytes, through `g op= c` into `r` and through a return into `q`.
std::string program(const std::string& ctype, char op, const std::vector<std::uint64_t>& words) {
  std::string ws;
  std::string bs;
  int n = 0;
  for (const std::uint64_t w : words) {
    for (const std::uint64_t b : kBytes) {
      if (n++ > 0) {
        ws += ", ";
        bs += ", ";
      }
      ws += std::to_string(w) + "UL";
      bs += std::to_string(b);
    }
  }
  const std::string ns = std::to_string(n);
  const std::string o(1, op);
  return ctype + " g;\n" + ctype + " r[" + ns + "];\n" + ctype + " q[" + ns + "];\n" + ctype + " ws[" + ns +
         "] = {" + ws + "};\n" + "unsigned char bs[" + ns + "] = {" + bs + "};\n" +
         "void f(unsigned char c) { g " + o + "= c; }\n" + ctype + " h(" + ctype +
         " t, unsigned char c) { return t " + o + " c; }\n" + "int main(void) {\n" +
         "    for (int i = 0; i < " + ns + "; i++) {\n" + "        g = ws[i]; f(bs[i]); r[i] = g;\n" +
         "        q[i] = h(ws[i], bs[i]);\n" + "    }\n" + "    return 0;\n}\n";
}

// Runs `src` and checks each element of `r` and `q`.
void check(const std::string& src, int size, char op, const std::vector<std::uint64_t>& words, bool optimize) {
  const std::uint64_t mask = size == 8 ? ~0ULL : (1ULL << (size * 8)) - 1;
  sim::Simulation s = build_sim(src, optimize);
  const sim::SimResult r = s.run();
  for (const char* name : {"r", "q"}) {
    const std::int64_t base = s.symbol(name);
    int i = 0;
    for (const std::uint64_t w : words) {
      for (const std::uint64_t b : kBytes) {
        const std::uint64_t want = (op == '+' ? w + b : w - b) & mask;
        INFO(name << ": " << w << " " << op << " " << b);
        CHECK(r.word(base + i * size, size) == want);
        ++i;
      }
    }
  }
}

}  // namespace

TEST_CASE("carry_inc sim: unsigned int add and subtract") {
  std::vector<std::string> srcs;
  for (const bool optimize : {false, true}) {
    for (const char op : {'+', '-'}) {
      INFO("optimize=" << optimize << " op=" << op);
      const std::string src = program("unsigned int", op, kWords);
      if (!optimize) srcs.push_back(src);
      check(src, 2, op, kWords, optimize);
    }
  }
  // The CRC-32s of the Python test's programs, computed on 27 Nov 2026.
  REQUIRE(srcs.size() == 2);
  CHECK(sim::cpu_run::crc32(srcs[0]) == 3159309657u);
  CHECK(sim::cpu_run::crc32(srcs[1]) == 1842831742u);
}

TEST_CASE("carry_inc sim: unsigned long add and subtract") {
  // The carry runs through bytes 1 and 2, which are followed by another
  // ADC and keep the ADC form, before the top byte.
  const std::vector<std::uint64_t> longs = {0x00000000, 0x00FFFFFF, 0x0000FFFF,
                                            0xFFFFFFFF, 0x12345678, 0x01000000};
  std::vector<std::string> srcs;
  for (const bool optimize : {false, true}) {
    for (const char op : {'+', '-'}) {
      INFO("optimize=" << optimize << " op=" << op);
      const std::string src = program("unsigned long", op, longs);
      if (!optimize) srcs.push_back(src);
      check(src, 4, op, longs, optimize);
    }
  }
  // The CRC-32s of the Python test's programs, computed on 27 Nov 2026.
  REQUIRE(srcs.size() == 2);
  CHECK(sim::cpu_run::crc32(srcs[0]) == 3044944600u);
  CHECK(sim::cpu_run::crc32(srcs[1]) == 3758117062u);
}

TEST_CASE("carry_inc sim: times nine") {
  // `(x << 3) + x` puts the sum's high byte in the shift temp's high
  // byte, which the BCC / INC then updates.
  const std::string src = R"c(unsigned int r[256];
unsigned int f(unsigned char x) { return x * 9; }
int main(void) {
    for (unsigned int i = 0; i < 256; i++) r[i] = f(i);
    return 0;
}
)c";
  const std::string text = codegen(src, true);
  const std::size_t at = text.find("\nf:");
  REQUIRE(at != std::string::npos);
  const std::string after = text.substr(at + 3);
  const std::string body = after.substr(0, after.find("RTS"));
  CHECK(contains(body, "BCC   .carry_inc@"));
  sim::Simulation s = build_sim(src, true);
  const sim::SimResult r = s.run();
  const std::int64_t base = s.symbol("r");
  for (std::uint64_t i = 0; i < 256; ++i) {
    INFO("x=" << i);
    CHECK(r.word(base + 2 * i) == i * 9);
  }
}

}  // namespace c6502::suite
