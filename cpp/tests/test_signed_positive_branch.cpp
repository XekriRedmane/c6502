// The `-S -O` text of a branch on a signed `x <= 0`, which tests the
// sign bit and ORs the bytes, from
// tests/test_signed_positive_branch.py.
#include <string>

#include "compile_case.h"
#include "doctest.h"

namespace suite = c6502::suite;

TEST_CASE("signed_positive_branch: an int <= 0 tests the sign and ORs the bytes") {
  const std::string text = suite::codegen(
      "#include <stdint.h>\n"
      "extern uint16_t W [[c6502::address(0x5A02)]];\n"
      "void f(void);\n"
      "void d(void) { if ((int16_t)W <= 0) f(); }\n",
      true);
  const std::size_t at = text.find("\nd:");
  REQUIRE(at != std::string::npos);
  const std::string body = text.substr(at);
  CHECK(suite::contains(body, "LDA   W+1"));
  CHECK(suite::contains(body, "ORA   W"));
  CHECK_FALSE(suite::contains(body, "SBC"));
}
