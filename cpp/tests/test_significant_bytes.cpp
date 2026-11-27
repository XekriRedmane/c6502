// The tests of tests/test_significant_bytes.py that read the `-S` text
// of shifts and compares of values with one significant byte, and the
// arithmetic identity behind the closed form of Zork's
// `please_insert_save_diskette`. The simulator's runs are in
// test_significant_bytes_sim.cpp.
#include <sstream>
#include <string>
#include <vector>

#include "compile_case.h"
#include "doctest.h"

namespace {

namespace suite = c6502::suite;

using Words = std::vector<std::string>;

Words words(const std::string& text) {
  std::istringstream in(text);
  Words out;
  for (std::string w; in >> w;) out.push_back(w);
  return out;
}

}  // namespace

TEST_CASE("significant_bytes: a widened byte shifted left calls no helper") {
  const std::string source = R"(
        unsigned int g;
        void f(unsigned char c) { g = (unsigned int)c << 5; }
        void h(unsigned char c) { g = 64 * (c & 7); }
        )";
  for (const bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    CHECK_FALSE(suite::contains(suite::codegen(source, optimize), "asl16"));
  }
}

TEST_CASE("significant_bytes: a sign-extended byte shifted left shifts every byte") {
  // A negative `signed char` widens to a nonzero high byte, so the
  // shift is a loop over both bytes.
  const std::string text = suite::codegen(R"(
        unsigned int g;
        void f(signed char c) { g = (unsigned int)c << 5; }
        )",
                                          true);
  CHECK(suite::contains(text, "ROL   g+1"));
}

TEST_CASE("significant_bytes: the loop of please_insert_save_diskette and its closed form agree") {
  // The loop sets SCRATCH1 to one less than 64 times the position, for
  // every byte the answer could hold.
  for (int answer = 0; answer < 256; ++answer) {
    INFO("answer=" << answer);
    unsigned loop = 0xFFFF;
    for (int k = 0; k < (answer & 0x07); ++k) loop = (loop + 64) & 0xFFFF;
    const unsigned closed = static_cast<unsigned>(64 * (answer & 0x07) - 1) & 0xFFFF;
    CHECK(loop == closed);
  }
}

TEST_CASE("significant_bytes: a byte of a right shift calls no helper") {
  const std::string text = suite::codegen(R"(
        unsigned int z;
        unsigned char o;
        void f(void) { o = (unsigned char)(z >> 5) & 0x1F; }
        )",
                                          true);
  CHECK_FALSE(suite::contains(text, "lsr16"));
}

TEST_CASE("significant_bytes: == and != compare only the bytes that can be nonzero") {
  // A widened byte masked to its top bit, compared with 0, tests one
  // byte, and a known-0 high byte isn't compared with 0.
  const std::string text = suite::codegen(R"(
        #include <stdbool.h>
        unsigned char get(void);
        bool f(void) { bool on = (get() & 0x80) != 0; return on; }
        )",
                                          true);
  std::vector<Words> lines;
  for (const std::string& line : suite::lines(text)) {
    const Words w = words(line);
    if (!w.empty()) lines.push_back(w);
  }
  bool zero_compare = false;
  bool and_then_beq = false;
  for (std::size_t k = 0; k + 1 < lines.size(); ++k) {
    zero_compare = zero_compare ||
                   (lines[k] == Words{"LDA", "#$00"} && lines[k + 1] == Words{"CMP", "#$00"});
    and_then_beq = and_then_beq ||
                   (lines[k] == Words{"AND", "#$80"} && lines[k + 1].front() == "BEQ");
  }
  CHECK_FALSE(zero_compare);
  CHECK(and_then_beq);
}
