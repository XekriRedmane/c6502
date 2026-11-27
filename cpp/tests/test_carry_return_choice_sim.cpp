// The simulator tests of tests/test_carry_return_choice.py:
// src/passes/carry_return_choice.h returns a `bool` in the carry flag
// without the annotation, for a `static` function every call of which
// suits the carry and for a zp_abi function with external linkage, and
// the programs run in both pipelines and when linked from two units.
#include <cstdint>
#include <string>
#include <tuple>
#include <vector>

#include "batch1_case.h"
#include "compile_case.h"
#include "doctest.h"
#include "sim_case.h"

namespace c6502::suite {
namespace {

const std::string kHead = R"c(#include <stdint.h>
#include <stdbool.h>
uint8_t g;
)c";
namespace choice {
TEST_CASE("carry_return_choice: choice: values") {
  auto src = kHead + R"c(uint8_t s[6] = {'a', 'x', 'b', 'y', 'b', 'a'};
static bool is_a(uint8_t c) { return c == 'a'; }
static bool is_b(uint8_t c) { return c == 'b'; }
static bool either(uint8_t c) {
    if (is_a(c)) return true;
    return is_b(c);
}
int main(void) {
    int n = 0;
    for (uint8_t i = 0; i != 6; i++) {
        if (either(s[i])) n += 10;
        if (!is_a(s[i])) n += 1;
    }
    return n;
}
)c";
  // Four of the six are 'a' or 'b', and four aren't 'a'.
  for (bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    SimOptions options;
    options.optimize = optimize;
    options.inline_calls = false;
    auto out = build_sim(src, options).run(5'000'000);
    CHECK_FALSE(out.timed_out);
    CHECK(out.return_int() == 40 + 4);
  }
}

}  // namespace choice

namespace global_choice {
// A `bool` function with external linkage returns in C whenever
// it's zp_abi, whatever its callers do with the result, since the
// other units decide from the declaration too.
TEST_CASE("carry_return_choice: global choice: values") {
  // Each way a caller can use the result: branched on, kept as a
  // value, negated, and returned by another such function.
  auto src = kHead + R"c(uint8_t s[6] = {'a', 'x', 'b', 'y', 'b', 'a'};
bool is_a(uint8_t c) { return c == 'a'; }
bool is_b(uint8_t c) { return c == 'b'; }
bool either(uint8_t c) {
    if (is_a(c)) return true;
    return is_b(c);
}
int main(void) {
    int n = 0;
    for (uint8_t i = 0; i != 6; i++) {
        if (either(s[i])) n += 10;
        if (!is_a(s[i])) n += 1;
        n += 100 * is_b(s[i]);
    }
    return n;
}
)c";
  // Four are 'a' or 'b', four aren't 'a', and two are 'b'.
  for (bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    auto out = build_sim(src, optimize).run(5'000'000);
    CHECK_FALSE(out.timed_out);
    CHECK(out.return_int() == 40 + 4 + 200);
  }
}

}  // namespace global_choice

const std::string kLib = R"c(#include <stdint.h>
#include <stdbool.h>
bool is_digit(uint8_t c) { return c >= '0' && c <= '9'; }
)c";
const std::string kMain = R"c(#include <stdint.h>
#include <stdbool.h>
bool is_digit(uint8_t c);
uint8_t s[5] = {'1', 'x', '7', ' ', '0'};
int main(void) {
    int n = 0;
    for (uint8_t i = 0; i != 5; i++) {
        if (is_digit(s[i])) n += 10;
        n += is_digit(s[i]);
    }
    return n;
}
)c";
namespace linked {
// Compiles `src` from stdin to the `-S -O` text at `path`, and returns
// the text.
std::string compile(const std::string& src, const std::string& path) {
  const auto o = compile_main({"compile.py", "-", "-S", "-O", "-o", path}, src);
  CHECK(o.status == 0);
  return read_file(path);
}

TEST_CASE("carry_return_choice: units agree on the convention and run") {
  TempDir tmp;
  const std::string lib = compile(kLib, tmp.file("lib.asm"));
  const std::string main_text = compile(kMain, tmp.file("main.asm"));
  // The defining unit and the calling unit both record it.
  CHECK(search("; def is_digit .*carry=true", lib));
  CHECK(search("; ext is_digit .*carry=true", main_text));
  const std::string out = tmp.file("out");
  CHECK(compile_main({"compile.py", tmp.file("lib.asm"), tmp.file("main.asm"), "-o", out}).status == 0);
  // Three of the five are digits.
  CHECK(build_linked_sim(out).run().return_int() == 33);
}

}  // namespace linked

}  // namespace
}  // namespace c6502::suite
