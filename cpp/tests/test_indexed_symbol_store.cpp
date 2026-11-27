// The `-S -O` text of tests/test_indexed_symbol_store.py's stores into a
// static array of at most 256 bytes, which are absolute,X on its label.
// The simulator runs stay in Python.
#include <string>

#include "compile_case.h"
#include "doctest.h"

namespace suite = c6502::suite;

TEST_CASE("indexed_symbol_store: a store into a static uint8_t array is absolute,X") {
  const std::string text = suite::codegen(
      "#include <stdint.h>\n"
      "static uint8_t arr[100];\n"
      "void f(uint8_t i, uint8_t v) { arr[i] = v; }\n"
      "int main(void) { f(3, 7); return arr[3]; }\n",
      true);
  CHECK(suite::contains(text, "STA   arr,X"));
}

TEST_CASE("indexed_symbol_store: a store into an extern array of unknown size is absolute,X") {
  // The array's size is 0 here, and the unit that defines it is
  // responsible for giving it at most 256 bytes.
  const std::string text = suite::codegen(
      "#include <stdint.h>\n"
      "extern uint8_t entity_active[];\n"
      "void f(uint8_t slot) { entity_active[slot] = 1; }\n"
      "int main(void) { return 0; }\n",
      true);
  CHECK(suite::contains(text, "STA   entity_active,X"));
}

TEST_CASE("indexed_symbol_store: a store into a static uint16_t array is two STAs") {
  const std::string text = suite::codegen(
      "#include <stdint.h>\n"
      "static uint16_t arr[10];\n"
      "void f(uint8_t i, uint16_t v) { arr[i] = v; }\n"
      "int main(void) { return 0; }\n",
      true);
  CHECK(suite::contains(text, "STA   arr,X"));
  CHECK(suite::contains(text, "STA   arr+1,X"));
}

TEST_CASE("indexed_symbol_store: a store into an array of more than 256 bytes goes through a pointer") {
  // Its byte index can exceed 255.
  const std::string text = suite::codegen(
      "#include <stdint.h>\n"
      "static uint8_t big[300];\n"
      "void f(uint16_t i, uint8_t v) { big[i] = v; }\n"
      "int main(void) { return 0; }\n",
      true);
  CHECK_FALSE(suite::contains(text, "STA   big,X"));
}
