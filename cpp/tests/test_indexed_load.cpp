// The `-S -O` text of tests/test_indexed_load.py's reads of a static
// array of at most 256 bytes, which are absolute,X on its label. The
// simulator runs stay in Python.
#include <string>

#include "compile_case.h"
#include "doctest.h"

namespace suite = c6502::suite;

TEST_CASE("indexed_load: a static uint16_t array is read with absolute,X") {
  // The 2-byte return rides in A and X, so the high byte is loaded with
  // `LDY arr+1,X` (there's no `LDX abs,X`) and moved to X.
  const std::string text = suite::codegen(
      "#include <stdint.h>\n"
      "static uint16_t arr[10] = {0,1,2,3,4,5,6,7,8,9};\n"
      "int main(int i) { return arr[i]; }\n",
      true);
  CHECK(suite::contains(text, "LDA   arr,X"));
  CHECK(suite::contains(text, "arr+1,X"));
}

TEST_CASE("indexed_load: a static uint8_t array is read with absolute,X") {
  const std::string text = suite::codegen(
      "#include <stdint.h>\n"
      "static uint8_t arr[100];\n"
      "int main(int i) { return arr[i]; }\n",
      true);
  CHECK(suite::contains(text, "LDA   arr,X"));
}

TEST_CASE("indexed_load: an array of more than 256 bytes is read through a pointer") {
  const std::string text = suite::codegen(
      "#include <stdint.h>\n"
      "static uint16_t arr[200];\n"
      "int main(int i) { return arr[i]; }\n",
      true);
  CHECK_FALSE(suite::contains(text, ",X"));
}

TEST_CASE("indexed_load: a local array is read through a pointer") {
  // It's in the frame, with no label to index.
  const std::string text = suite::codegen(
      "#include <stdint.h>\n"
      "int main(int i) { uint16_t arr[10] = {0}; return arr[i]; }\n",
      true);
  CHECK_FALSE(suite::contains(text, "arr,X"));
}
