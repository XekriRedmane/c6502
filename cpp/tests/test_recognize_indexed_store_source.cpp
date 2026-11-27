// The `-S -O` text of tests/test_recognize_indexed_store.py's stores
// through a `static T * const` at a byte index, which are `STA
// $XXXX,X`. The simulator's runs are in
// test_recognize_indexed_store_sim.cpp.
#include <string>

#include "compile_case.h"
#include "doctest.h"

namespace suite = c6502::suite;

TEST_CASE("recognize_indexed_store: a store at a byte index collapses to STA abs,X") {
  // The read of the const static folds, the offsets reassociate, and
  // the recognizer makes the IndexedStore.
  const std::string text = suite::codegen(
      "#include <stdint.h>\n"
      "static uint8_t * const buf = (uint8_t * const)0x4123;\n"
      "void put(uint8_t value, uint8_t col) {\n"
      "    buf[col] = value;\n"
      "}\n"
      "int main(void) { put(0x42, 5); return 0; }\n",
      true);
  CHECK(suite::contains(text, "STA   $4123,X"));
}

TEST_CASE("recognize_indexed_store: a constant offset folds into the base") {
  const std::string text = suite::codegen(
      "#include <stdint.h>\n"
      "static uint8_t * const buf = (uint8_t * const)0x4000;\n"
      "void put(uint8_t value, uint8_t col) {\n"
      "    buf[100 + col] = value;\n"
      "}\n"
      "int main(void) { put(0x77, 3); return 0; }\n",
      true);
  CHECK(suite::contains(text, "STA   $4064,X"));
}

TEST_CASE("recognize_indexed_store: an int index doesn't fold") {
  // Its high byte may be nonzero, so absolute,X can't reach the byte.
  const std::string text = suite::codegen(
      "#include <stdint.h>\n"
      "static uint8_t * const buf = (uint8_t * const)0x4000;\n"
      "void put(uint8_t value, int col) {\n"
      "    buf[col] = value;\n"
      "}\n"
      "int main(void) { put(0x42, 5); return 0; }\n",
      true);
  CHECK_FALSE(suite::contains(text, "STA   $4000,X"));
}
