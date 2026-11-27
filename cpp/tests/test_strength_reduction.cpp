// A byte times a constant of three signed powers of two is shifts and
// adds, from tests/test_strength_reduction.py. The simulator's runs are
// in test_strength_reduction_sim.cpp.
#include <string>

#include "compile_case.h"
#include "doctest.h"

namespace suite = c6502::suite;

TEST_CASE("strength_reduction: the PRNG step's multiply by 69 calls no helper") {
  const std::string text = suite::codegen(
      "#include <stdint.h>\n"
      "uint8_t st;\n"
      "uint8_t prng(void) { st = 69 * st + 0x53; return st >> 1; }\n",
      true);
  CHECK_FALSE(suite::contains(text, "mul8"));
}
