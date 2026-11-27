// The `-S -O` text of an integer negate, `0 - x` a byte at a time, from
// tests/test_negate_lowering.py.
#include <string>

#include "compile_case.h"
#include "doctest.h"

namespace suite = c6502::suite;

TEST_CASE("negate_lowering: a negate subtracts from zero") {
  const std::string text = suite::codegen(
      "unsigned int g, h;\n"
      "int main(void) { h = -g; return 0; }\n",
      true);
  const std::size_t at = text.find("\nmain:");
  REQUIRE(at != std::string::npos);
  const std::string body = text.substr(at);
  CHECK_FALSE(suite::contains(body, "EOR"));
  CHECK(suite::contains(body, "SBC   g"));
}
