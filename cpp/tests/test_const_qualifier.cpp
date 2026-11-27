// The tests of tests/test_const_qualifier.py that read the `-S -O`
// text: what a `volatile` read or write keeps. The simulator runs stay
// in Python.
#include <string>

#include "compile_case.h"
#include "doctest.h"

namespace suite = c6502::suite;

TEST_CASE("const_qualifier: a discarded read through a pointer to volatile is kept") {
  // The motivating case is memory-mapped I/O like the Apple II's $C030
  // speaker soft switch, whose read has a side effect.
  // `volatile_void_read_cmp` turns the `LDA (DPTR),Y` into `CMP
  // (DPTR),Y` when nothing reads A or the flags after it, which reads
  // the same byte.
  const std::string text = suite::codegen(
      "#include <stdint.h>\n"
      "extern const volatile uint8_t *p;\n"
      "[[c6502::zp_abi]]\n"
      "void poke(void) { (void)*p; }\n",
      true);
  CHECK(suite::search(R"((LDA|CMP)   \(DPTR\),Y)", text));
}

TEST_CASE("const_qualifier: a discarded read through a pointer to non-volatile is dropped") {
  const std::string text = suite::codegen(
      "#include <stdint.h>\n"
      "extern const uint8_t *p;\n"
      "[[c6502::zp_abi]]\n"
      "void poke(void) { (void)*p; }\n",
      true);
  CHECK_FALSE(suite::contains(text, "LDA   (DPTR),Y"));
}

TEST_CASE("const_qualifier: a store through a pointer to volatile is kept") {
  const std::string text = suite::codegen(
      "#include <stdint.h>\n"
      "extern volatile uint8_t *p;\n"
      "[[c6502::zp_abi]]\n"
      "void poke(uint8_t v) { *p = v; }\n",
      true);
  CHECK(suite::contains(text, "STA   (DPTR),Y"));
}

TEST_CASE("const_qualifier: a volatile local's decrement loop keeps one load and one store") {
  // A DEC would give `y` the same values, but volatile semantics take
  // one explicit load and one explicit store per pass. The decrement
  // happens on a non-volatile copy, between one `LDA y` and one `STA y`
  // in the loop, and the other `STA y` is the initialization.
  const std::string text = suite::codegen(
      "#include <stdint.h>\n"
      "[[c6502::zp_abi]]\n"
      "void delay(uint8_t n) {\n"
      "    volatile uint8_t y = n;\n"
      "    while (--y != 0) { }\n"
      "}\n",
      true);
  CHECK_FALSE(suite::contains(text, "DEC   __local_delay__y"));
  CHECK(suite::count(text, "LDA   __local_delay__y") == 1);
  CHECK(suite::count(text, "STA   __local_delay__y") == 2);
}

TEST_CASE("const_qualifier: a non-volatile local's decrement loop goes") {
  // The dead pure loop and its store are removed, which leaves the
  // function an `RTS`.
  const std::string text = suite::codegen(
      "#include <stdint.h>\n"
      "[[c6502::zp_abi]]\n"
      "void delay(uint8_t n) {\n"
      "    uint8_t y = n;\n"
      "    while (--y != 0) { }\n"
      "}\n",
      true);
  CHECK_FALSE(suite::contains(text, "SBC"));
}
