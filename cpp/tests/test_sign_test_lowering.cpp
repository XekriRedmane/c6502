// The `-S -O` text of tests/test_sign_test_lowering.py's compares kept
// as values: a signed `x < 0` or `x >= 0` takes the sign bit into C,
// and an unsigned ordering is a subtract.
#include <string>

#include "compile_case.h"
#include "doctest.h"

namespace suite = c6502::suite;

TEST_CASE("sign_test_lowering: a sign test kept as a value has no subtract") {
  const std::string text = suite::codegen(
      "#include <stdint.h>\n"
      "#include <stdbool.h>\n"
      "uint16_t a, b;\n"
      "bool n, p;\n"
      "int main(void) {\n"
      "    n = (int16_t)(a ^ b) < 0;\n"
      "    p = (int16_t)a >= 0;\n"
      "    return 0;\n"
      "}\n",
      true);
  const std::size_t at = text.find("\nmain:");
  REQUIRE(at != std::string::npos);
  const std::string body = text.substr(at);
  CHECK_FALSE(suite::contains(body, "SBC"));
  CHECK_FALSE(suite::contains(body, "BVC"));
  CHECK(suite::count(body, "ROL") == 2);
}

TEST_CASE("sign_test_lowering: an unsigned ordering returned in the carry needs no ROL") {
  // `return level > l;` from a function that returns its `bool` in C is
  // the subtract alone, `LDA level; CLC; SBC l`.
  const std::string text =
      suite::codegen(R"(#include <stdbool.h>
#include <stdint.h>
uint8_t level, score;
[[c6502::return_bool_in_carry]]
static bool beats(uint8_t l, uint8_t s) {
    if (level != l)
        return level > l;
    return score > s;
}
int main(void) {
    int n = 0;
    level = 3; score = 7;
    if (beats(2, 9)) n += 1;
    if (!beats(4, 0)) n += 2;
    if (beats(3, 6)) n += 4;
    if (!beats(3, 7)) n += 8;
    return n;
}
)", true);
  const std::size_t start = text.find("\nbeats:");
  const std::size_t end = text.find("\nmain:");
  REQUIRE(start != std::string::npos);
  REQUIRE(end != std::string::npos);
  const std::string body = text.substr(start, end > start ? end - start : 0);
  CHECK_FALSE(suite::contains(body, "ROL"));
  CHECK_FALSE(suite::contains(body, "EOR"));
}
