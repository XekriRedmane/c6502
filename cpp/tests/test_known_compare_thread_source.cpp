// The `-S -O` text of tests/test_known_compare_thread.py's shape of
// `handle_timers` (examples/lode_runner/timers.c). The simulator's runs
// are in test_known_compare_thread_sim.cpp.
#include <string>

#include "compile_case.h"
#include "doctest.h"

namespace suite = c6502::suite;

TEST_CASE("known_compare_thread: the second compare with 20 goes") {
  suite::Options options;
  options.optimize = true;
  options.inline_calls = false;
  const std::string text = suite::codegen(
      R"(#include <stdint.h>
volatile uint8_t sink;
static uint8_t f(uint8_t t) {
    uint8_t r = 0;
    if (t == 20 || t == 10) r = t == 20 ? 0x37 : 0x38;
    return r;
}
int main(void) {
    uint16_t n = 0;
    for (uint16_t v = 0; v < 600; v++)
        n = (uint16_t)(n * 3u + f((uint8_t)v));
    return n;
}
)",
      options);
  const std::size_t start = text.find("\nf:");
  REQUIRE(start != std::string::npos);
  const std::size_t end = text.find("\nmain:", start);
  REQUIRE(end != std::string::npos);
  const std::string body = text.substr(start, end - start);
  CHECK(suite::count(body, "CMP   #$14") == 1);
  CHECK(suite::count(body, "JMP") == 0);
}
