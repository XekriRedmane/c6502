// The `-S -O` text of tests/test_coalescing.py's byte loop. The
// simulator's runs are in test_coalescing_sim.cpp.
#include <string>

#include "compile_case.h"
#include "doctest.h"

namespace suite = c6502::suite;

TEST_CASE("coalescing: a byte loop's increment collapses to INC") {
  // Coalescing merges the Phi-related SSA names into one zero-page
  // slot, the round-trip Mov becomes a self-Mov dropped at emit, and
  // the INC peephole collapses the in-place ADC chain to a bare INC.
  const std::string text = suite::codegen(
      "#include <stdint.h>\n"
      "uint8_t total = 0;\n"
      "int main(void) {\n"
      "    for (uint8_t i = 0; i < 5; i++) total = total + i;\n"
      "    return total;\n"
      "}\n",
      true);
  // The loop runs from the body's label, which the bottom test branches
  // back to, to the label `.loop@0_break:`. The test is known to pass
  // on entry, so cpp/src/passes/rotate_loop_test.h leaves no `JMP` to
  // it.
  const std::size_t start = text.find(".main@rot@0:");
  REQUIRE(start != std::string::npos);
  const std::string rest = text.substr(start);
  const std::string block = rest.substr(0, rest.find(".loop@0_break:"));
  CHECK(suite::search(R"(INC\s+__local_\w+__\w+)", block));
  CHECK_FALSE(suite::contains(block, "ADC   #$01"));
}
