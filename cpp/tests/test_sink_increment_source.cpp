// The `-S -O` text of tests/test_sink_increment.py's post-decrement
// test. The simulator's runs are in test_sink_increment_sim.cpp.
#include <string>

#include "compile_case.h"
#include "doctest.h"

namespace suite = c6502::suite;

TEST_CASE("sink_increment: a post-decrement test becomes DEX and a compare with $FF") {
  const std::string text = suite::codegen(
      "#include <stdint.h>\n"
      "uint8_t tab[8];\n"
      "uint8_t find(uint8_t c) {\n"
      "    uint8_t x = 7;\n"
      "    do { if (tab[x] == c) return x + 8; } while (x-- != 0);\n"
      "    return 0;\n"
      "}\n",
      true);
  CHECK(suite::contains(text, "CPX   #$FF"));
  CHECK_FALSE(suite::contains(text, "SBC"));
}
