// The `-S -O` text of tests/test_direct_index_load.py's indexed read.
#include <string>

#include "compile_case.h"
#include "doctest.h"

namespace suite = c6502::suite;

TEST_CASE("direct_index_load: an indexed load in a loop reads with the counter in X") {
  const std::string text = suite::codegen(
      "#include <stdint.h>\n"
      "static const uint8_t arr[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};\n"
      "int main(void) {\n"
      "    int s = 0;\n"
      "    for (uint8_t i = 0; i < 10; i++) s += arr[i];\n"
      "    return s;\n"
      "}\n",
      true);
  // The read is the `ADC arr,X` of `s += arr[i]` (cmp_through_tmp_fold),
  // with the counter in X, incremented with INX.
  CHECK(suite::contains(text, "ADC   arr,X"));
  CHECK(suite::contains(text, "INX"));
  // The only transfer into X is the return value's high byte, not an
  // index set up from a slot.
  CHECK_FALSE(suite::contains(text, "TAX\n   LDA   arr,X"));
  CHECK_FALSE(suite::search(R"(LDA   __local[^\n]*\n   TAX)", text));
}
