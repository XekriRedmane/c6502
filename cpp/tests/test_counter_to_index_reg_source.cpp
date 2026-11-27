// The `-S -O` text of tests/test_counter_to_index_reg.py's delay loop.
// The simulator's runs are in test_counter_to_index_reg_sim.cpp.
#include <string>

#include "compile_case.h"
#include "doctest.h"

namespace suite = c6502::suite;

TEST_CASE("counter_to_index_reg: the delay loop's counters count down in Y and X") {
  const std::string text =
      suite::codegen(R"(#include <stdint.h>
uint16_t passes;
void delay(uint8_t count) {
    do {
        [[c6502::keep]] for (uint8_t y = 3; --y;) {
        }
        passes++;
    } while (--count);
}
int main(void) {
    delay(5);
    delay(0);
    return passes;
}
)", true);
  const std::size_t start = text.find("\ndelay:");
  const std::size_t end = text.find("\nmain:");
  REQUIRE(start != std::string::npos);
  REQUIRE(end != std::string::npos);
  const std::string body = text.substr(start, end > start ? end - start : 0);
  CHECK(suite::contains(body, "DEY"));
  CHECK(suite::contains(body, "DEX"));
}
