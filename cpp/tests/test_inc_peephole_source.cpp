// The `-S -O` text of tests/test_inc_peephole.py's 16-bit loop counter.
// The simulator's runs are in test_inc_peephole_sim.cpp.
#include <string>

#include "compile_case.h"
#include "doctest.h"

namespace suite = c6502::suite;

TEST_CASE("inc_peephole: a 16-bit loop counter's increment is an INC and BNE chain") {
  // The counter `i` lives in two zero-page bytes, and `i++` is in place
  // on each byte. The branch-through-jump peephole sends the BNE past
  // the high byte's INC to the loop's start, so the chain's own
  // `.inc_done@N` label goes.
  const std::string text = suite::codegen(
      "int sum10(void) {\n"
      "    int s = 0;\n"
      "    for (int i = 0; i < 10; i++) s += i;\n"
      "    return s;\n"
      "}\n"
      "int main(void) { return sum10(); }\n",
      true);
  CHECK(suite::contains(text, ".loop@"));
  // `ADC #$01` would only be the increment of `i`.
  CHECK_FALSE(suite::contains(text, "ADC   #$01"));
  CHECK(suite::search(R"(INC\s+__local_\w+__\w+)", text));
  CHECK(suite::search(R"(INC\s+__local_\w+__\w+\s*\n\s*BNE\s+)", text));
}
