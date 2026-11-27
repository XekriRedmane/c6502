// The test of tests/test_hwreg_pinning.py that compiled a program with
// `-O`.
#include <string>

#include "compile_case.h"
#include "doctest.h"

namespace {

namespace suite = c6502::suite;

}  // namespace

TEST_CASE("hwreg_pinning: a loop counter that indexes stores is pinned to X") {
  // With the counter in X, each store of `buf[i] = 0` is `STA $4000,X`,
  // with no `LDX` of the counter's slot before it.
  const std::string text = suite::codegen(
      "#include <stdint.h>\n"
      "static uint8_t * const buf = (uint8_t * const)0x4000;\n"
      "void clear(void) {\n"
      "    for (uint8_t i = 0; i < 16; i++) buf[i] = 0;\n"
      "}\n",
      true);
  CHECK(suite::contains(text, "STA   $40"));
}
