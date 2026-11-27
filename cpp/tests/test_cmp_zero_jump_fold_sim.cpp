// Programs whose comparisons src/passes/optimization/cmp_zero_jump_fold.h
// folds into their jumps or narrows, run in the simulator, from
// tests/test_cmp_zero_jump_fold.py.
#include <cstdint>
#include <functional>
#include <string>
#include <utility>
#include <vector>

#include "doctest.h"
#include "sim/cpu_run.h"
#include "sim_case.h"

namespace c6502::suite {

namespace {

// What `main` returns, read as signed, unoptimized and under -O.
std::pair<std::int64_t, std::int64_t> both_paths(const std::string& src) {
  return {run_c_program(src).return_int_signed(), build_sim(src, true).run().return_int_signed()};
}

}  // namespace

TEST_CASE("cmp_zero_jump_fold sim: a compare stored to a static another function reads is kept") {
  // The compare's result goes to a static that another function reads.
  // Its one use in `check` is the jump, but the store must stay. The fold
  // had dropped it, so `apply` never negated and returned -7.
  CHECK(both_paths(R"c(#include <stdint.h>
#include <stdbool.h>
static bool neg;
uint16_t r;
static uint16_t check(uint16_t a, uint16_t b) {
    neg = (int16_t)(a ^ b) < 0;
    if (neg) a = -a;
    return a;
}
static void apply(uint16_t n) {
    if (neg) n = -n;
    r = n;
}
int main(void) { apply(check(7, 0xFFFE)); return (int)r; }
)c") == std::pair<std::int64_t, std::int64_t>{7, 7});
}

TEST_CASE("cmp_zero_jump_fold sim: a uint8_t == 0 that holds") {
  CHECK(both_paths(R"c(#include <stdint.h>
int f(uint8_t a) { if (a == 0) return 1; return 2; }
int main(void) { return f(0); }
)c") == std::pair<std::int64_t, std::int64_t>{1, 1});
}

TEST_CASE("cmp_zero_jump_fold sim: a uint8_t == 0 that doesn't hold") {
  CHECK(both_paths(R"c(#include <stdint.h>
int f(uint8_t a) { if (a == 0) return 1; return 2; }
int main(void) { return f(5); }
)c") == std::pair<std::int64_t, std::int64_t>{2, 2});
}

TEST_CASE("cmp_zero_jump_fold sim: a uint8_t != 0") {
  CHECK(both_paths(R"c(#include <stdint.h>
int f(uint8_t a) { if (a != 0) return 1; return 2; }
int main(void) { return f(5); }
)c") == std::pair<std::int64_t, std::int64_t>{1, 1});
}

TEST_CASE("cmp_zero_jump_fold sim: a uint16_t == 0 in a conditional expression") {
  // The conditional expression lowers to the same compare and jump.
  CHECK(both_paths(R"c(#include <stdint.h>
int f(uint16_t a) { return a == 0 ? 10 : 20; }
int main(void) { return f(0) + f(7); }
)c") == std::pair<std::int64_t, std::int64_t>{30, 30});
}

TEST_CASE("cmp_zero_jump_fold sim: a loop on a narrow ordering compare ends with its count") {
  const sim::SimResult r = build_sim(R"c(#include <stdint.h>
int main(void) {
    uint8_t i;
    int sum = 0;
    for (i = 0; i < 105; i = i + 1) sum = sum + 1;
    return sum;
}
)c", true).run(5'000'000);
  CHECK_FALSE(r.timed_out);
  CHECK(r.return_int() == 105);
}

TEST_CASE("cmp_zero_jump_fold sim: a store between an extend and its compare") {
  // A ZeroExtend or SignExtend of a static that's stored to before the
  // compare. The narrowing read the static at the jump, after the store,
  // and compiled `int t = g; g = 5; if (t < 3)` to `LDA #$05; STA g;
  // CMP #$03; BCS`.
  const char* src = R"c(#include <stdint.h>
uint8_t g;
int8_t sg;
uint8_t hits;
void lt(void) { int t = g; g = 5; if (t < 3) hits |= 1; }
void eq0(void) { int t = g; g = 5; if (t == 0) hits |= 2; }
void lt_c(void) { int t = g; g = 5; int c = t < 3; if (c) hits |= 4; }
void eq0_c(void) { int t = g; g = 5; int c = t == 0; if (c) hits |= 8; }
void ge0(void) { int t = sg; sg = -5; if (t >= 0) hits |= 16; }
int main(void) {
    g = 1; lt(); g = 0; eq0(); g = 1; lt_c(); g = 0; eq0_c();
    sg = 1; ge0();
    return hits;
}
)c";
  for (const bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    CHECK(build_sim(src, optimize).run().return_int() == 31);
  }
}

TEST_CASE("cmp_zero_jump_fold sim: a switch on a byte") {
  // A switch on a byte compares the one extended value with every case.
  // When that value is SSA-renamed, each compare narrows to the byte, and
  // the extend stays for any other use.
  const char* src = R"c(#include <stdint.h>
uint8_t pick(uint8_t n) {
    switch (n) {
    case 0: return 10;
    case 1: return 11;
    case 2: return 12;
    case 7: return 17;
    case 200: return 99;
    case 300: return 1;
    default: return 50;
    }
}
int main(void) {
    return pick(0) + pick(1) + pick(2) + pick(7) + pick(200)
        + pick(44);
}
)c";
  for (const bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    CHECK(build_sim(src, optimize).run().return_int() == 10 + 11 + 12 + 17 + 99 + 50);
  }
}

TEST_CASE("cmp_zero_jump_fold sim: a compare kept as a value narrows through its ZeroExtends") {
  // `(int)x < 10` on a `uint8_t x` subtracts one byte. The tests ran
  // with inlining off.
  const std::vector<int> values = {0, 9, 10, 11, 127, 128, 200, 255};
  const auto source = [&](const std::string& expr) {
    std::string table;
    for (std::size_t i = 0; i < values.size(); ++i) table += (i ? ", " : "") + std::to_string(values[i]);
    const std::string n = std::to_string(values.size());
    return "#include <stdint.h>\n"
           "uint8_t g[" + n + "] = {" + table + "};\n"
           "static uint8_t test(uint8_t x) { return " + expr + "; }\n"
           "int main(void) {\n"
           "    uint16_t n = 0;\n"
           "    for (uint8_t i = 0; i < " + n + "; i++)\n"
           "        n = (uint16_t)(n << 1) | test(g[i]);\n"
           "    return n;\n"
           "}\n";
  };
  const std::vector<std::pair<std::string, std::function<bool(int)>>> cases = {
      {"x < 10", [](int x) { return x < 10; }},
      {"x <= 10", [](int x) { return x <= 10; }},
      {"x > 127", [](int x) { return x > 127; }},
      {"x >= 200", [](int x) { return x >= 200; }},
      {"10 < x", [](int x) { return 10 < x; }},
      {"255 > x", [](int x) { return 255 > x; }},
      {"x < 256", [](int) { return true; }},
      {"x > -1", [](int) { return true; }},
  };
  // The count and CRC-32 of the programs the Python test wrote, on
  // 27 Nov 2026.
  std::string all;
  for (const auto& [expr, pred] : cases) all += source(expr);
  CHECK(cases.size() == 8);
  CHECK(sim::cpu_run::crc32(all) == 0x2382E661u);
  for (const auto& [expr, pred] : cases) {
    std::int64_t want = 0;
    for (const int v : values) want = ((want << 1) | (pred(v) ? 1 : 0)) & 0xFFFF;
    for (const bool optimize : {false, true}) {
      INFO("expr=" << expr << " optimize=" << optimize);
      SimOptions options;
      options.optimize = optimize;
      options.inline_calls = false;
      CHECK(build_sim(source(expr), options).run().return_int() == want);
    }
  }
}

}  // namespace c6502::suite
