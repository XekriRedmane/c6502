// The `-S -O` text of tests/test_register_arguments.py's functions
// whose arguments arrive in A and X. The simulator's runs are in
// test_register_arguments_sim.cpp.
#include <string>
#include <vector>

#include "compile_case.h"
#include "doctest.h"

namespace suite = c6502::suite;

using Lines = std::vector<std::string>;

TEST_CASE("register_arguments: a 2-byte register parameter's high byte stays in X") {
  const std::string text = suite::codegen(
      "unsigned char k;"
      "unsigned int widen(unsigned int y) { return y - k; }",
      true);
  CHECK(suite::function_body(text, "widen") ==
        Lines{"SEC", "SBC k", "BCS .carry_inc@0", "DEX", ".carry_inc@0:", "RTS"});
}

TEST_CASE("register_arguments: two byte arguments passed on in the other order swap") {
  const std::string text = suite::codegen(
      "#include <stdint.h>\n"
      "void g(uint8_t a, uint8_t b);"
      "void f(uint8_t a, uint8_t b) { g(b, a); }",
      true);
  CHECK(suite::function_body(text, "f") ==
        Lines{"STA __local_f__a", "TXA", "LDX __local_f__a", "JMP g"});
}

TEST_CASE("register_arguments: a count down in Y is DEY") {
  const std::string text = suite::codegen(
      "#include <stdint.h>\n"
      "void wait(uint8_t delay) {"
      "    register volatile uint8_t y = delay;"
      "    while (--y != 0) { }"
      "}",
      true);
  CHECK(suite::function_body(text, "wait") ==
        Lines{"TAY", ".loop@0_continue:", "DEY", "BNE .loop@0_continue", "RTS"});
}
