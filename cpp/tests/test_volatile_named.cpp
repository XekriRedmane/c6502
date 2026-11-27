// The tests of tests/test_volatile_named.py that read the `-S` text of
// reads and writes of a named `volatile` object, each of which stays.
// The simulator's runs are in test_volatile_named_sim.cpp.
#include <regex>
#include <string>

#include "compile_case.h"
#include "doctest.h"

namespace {

namespace suite = c6502::suite;

constexpr const char* kKbd = R"(extern volatile unsigned char KBD [[c6502::address(0xC000)]];
extern volatile unsigned char KBDSTRB [[c6502::address(0xC010)]];
)";

// Returns how many instructions read `name`: LDA, CMP, BIT, or an ALU
// instruction such as `ADC g`, which reads it in place.
std::size_t reads(const std::string& text, const std::string& name) {
  const std::regex re(R"(\s+(LDA|CMP|BIT|ADC|SBC|AND|ORA|EOR)\s+)" + name);
  std::size_t n = 0;
  for (const std::string& line : suite::lines(text)) {
    if (std::regex_match(line, re)) ++n;
  }
  return n;
}

}  // namespace

TEST_CASE("volatile_named: a read as a statement is kept") {
  for (const std::string stmt : {"KBDSTRB;", "(void)KBDSTRB;"}) {
    for (const bool optimize : {false, true}) {
      INFO(stmt << " optimize=" << optimize);
      const std::string text =
          suite::codegen(std::string(kKbd) + "void clear(void) { " + stmt + " }\n", optimize);
      CHECK(reads(text, "KBDSTRB") == 1);
    }
  }
}

TEST_CASE("volatile_named: each read happens") {
  for (const bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    const std::string text = suite::codegen(
        "volatile unsigned char g;\n"
        "unsigned char twice(void) { return g + g; }\n",
        optimize);
    CHECK(reads(text, "g") == 2);
  }
}

TEST_CASE("volatile_named: the optimized wait loop is a load and a BPL back to it") {
  const std::string text =
      suite::codegen(std::string(kKbd) + "void wait(void) { while (!(KBD & 0x80)) ; }\n", true);
  CHECK(suite::search(R"(\n(\.\S+):\n\s+LDA\s+KBD\n\s+BPL\s+\1(\n|$))", "\n" + text));
}

TEST_CASE("volatile_named: a self-assignment reads and writes the object") {
  for (const bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    const std::string text = suite::codegen(
        "volatile unsigned char g;\n"
        "void touch(void) { g = g; }\n",
        optimize);
    CHECK(reads(text, "g") == 1);
    CHECK(suite::search(R"(\n\s+STA\s+g(\n|$))", "\n" + text));
  }
}
