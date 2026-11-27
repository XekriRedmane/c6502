// The `-S -O` text of tests/test_ret_slot_coalescing.py's program, in
// which a local holding a copy of `F@ret` becomes `F@ret`. The
// simulator's runs are in test_ret_slot_coalescing_sim.cpp.
#include <string>

#include "compile_case.h"
#include "doctest.h"

namespace suite = c6502::suite;

TEST_CASE("ret_slot_coalescing: the result of next isn't copied") {
  suite::Options options;
  options.optimize = true;
  options.inline_calls = false;
  const std::string text = suite::codegen(R"(
#include <stdint.h>
static const uint8_t text[] = {2, 'a', 'b', 1, 'c', 0, 3, 'd', 'e', 'f', 0};
uint16_t total;
static const uint8_t *next(const uint8_t *s) { return s + s[0] + 1; }
static void add(uint8_t c) { total = total * 3 + c; }
static const uint8_t *group(uint8_t index) {
    const uint8_t *p = text;
    for (; index != 0; index--) {
        while (*p != 0)
            p = next(p);
        p++;
    }
    return p;
}
static void walk(const uint8_t *p) {
    while (*p != 0) {
        add(*p);
        add(p[1]);
        p = next(p);
        if (*p == 0)
            add(0xFF);
    }
}
uint16_t main(void) {
    walk(group(0));
    walk(group(1));
    return total;
}
)", options);
  const std::size_t start = text.find("\ngroup:");
  const std::size_t end = text.find("\nmain:");
  REQUIRE(start != std::string::npos);
  REQUIRE(end != std::string::npos);
  const std::string body = text.substr(start, end > start ? end - start : 0);
  CHECK_FALSE(suite::contains(body, "LDA   next@ret\n   STA   __local"));
  CHECK(suite::contains(body, "(next@ret),Y"));
}
