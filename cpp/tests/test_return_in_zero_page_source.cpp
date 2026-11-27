// The `-S -O` text of a read through a `static` function's pointer
// result right after the call, from tests/test_return_in_zero_page.py.
#include <string>

#include "compile_case.h"
#include "doctest.h"

namespace suite = c6502::suite;

TEST_CASE("return_in_zero_page: a read right after the call goes through the object") {
  suite::Options options;
  options.optimize = true;
  options.inline_calls = false;
  const std::string text = suite::codegen(
      "#include <stdint.h>\n"
      "uint8_t *base;\n"
      "static uint8_t *addr(uint8_t n) { return base + n * 9; }\n"
      "uint8_t get(uint8_t n) { return addr(n)[4]; }\n",
      options);
  const std::size_t at = text.find("\nget:\n");
  REQUIRE(at != std::string::npos);
  const std::string get = text.substr(at);
  CHECK(suite::contains(get, "LDA   (addr@ret),Y"));
  const std::size_t rts = get.find("RTS");
  REQUIRE(rts != std::string::npos);
  CHECK_FALSE(suite::contains(get.substr(0, rts), "STA"));
}
