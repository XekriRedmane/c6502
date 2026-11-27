// The `-S -O` text of tests/test_static_const_fold.py's reads of
// `static const` scalars and arrays, which fold to immediates, with
// their constant adds reassociated.
#include <string>

#include "compile_case.h"
#include "doctest.h"

namespace suite = c6502::suite;

TEST_CASE("static_const_fold: a static const scalar plus one is an immediate") {
  const std::string text = suite::codegen(
      "static const int magic = 0x1234;\n"
      "int main(void) { return magic + 1; }\n",
      true);
  CHECK_FALSE(suite::contains(text, "magic:"));
  // 0x1235, low byte first.
  CHECK(suite::contains(text, "#$35"));
  CHECK(suite::contains(text, "#$12"));
}

TEST_CASE("static_const_fold: a const array at a constant index folds") {
  const std::string text = suite::codegen(
      "#include <stdint.h>\n"
      "static const uint16_t arr[3] = {0x1234, 0x5678, 0x9ABC};\n"
      "int main(void) { return arr[2]; }\n",
      true);
  CHECK_FALSE(suite::contains(text, "LDA   arr,X"));
  CHECK(suite::contains(text, "#$BC"));
  CHECK(suite::contains(text, "#$9A"));
}

TEST_CASE("static_const_fold: the constants of an add combine") {
  // a + b + col is (a + b) + col, 300 + col, so neither 100 ($64) nor
  // 200 ($C8) is an immediate.
  const std::string text = suite::codegen(
      "static const int a = 100;\n"
      "static const int b = 200;\n"
      "int main(int col) { return a + b + col; }\n",
      true);
  CHECK(suite::contains(text, "#$2C"));
  CHECK_FALSE(suite::contains(text, "#$64"));
  CHECK_FALSE(suite::contains(text, "#$C8"));
}
