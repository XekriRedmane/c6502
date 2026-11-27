// The simulator tests of tests/test_zp_globals.py: zero-page globals
// (`[[c6502::zeropage]]`, src/passes/zp_globals.h) are initialized and
// aliased right in both pipelines, busy globals promoted under `-O` keep
// their values, and linked units share zero-page objects, including a
// promoted object that doesn't fit and goes back.
#include <cstdint>
#include <string>
#include <tuple>
#include <vector>

#include "compile_case.h"
#include "doctest.h"
#include "sim_case.h"
#include "sim_helpers.h"

namespace c6502::suite {
namespace {

// A pointer and a counter used in a loop, a constant, an array, and a
// variable read once.
const std::string kPromote = R"c(#include <stdint.h>
uint8_t buf[8] = {1, 2, 3, 4, 5, 6, 7, 8};
uint8_t *p = buf;
uint16_t total;
const uint8_t step = 1;
uint8_t once = 9;
int main(void) {
    for (uint8_t i = 0; i < 8; i += step) total += p[i] + buf[i];
    return total + once;
}
)c";
namespace promotion {
// `-O` moves busy scalar globals into zero page.
TEST_CASE("zp_globals: runs") {
  for (bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    auto got = build_sim(kPromote, optimize).run();
    CHECK(got.return_int() == 2 * 36 + 9);
  }
}

}  // namespace promotion

namespace sim_runs {
void both(const auto& src, const auto& want) {
  for (bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    auto got = build_sim(src, optimize).run().return_int();
    CHECK(got == want);
  }
}

TEST_CASE("zp_globals: initial values") {
  std::string src = R"c(struct s { unsigned char a; unsigned int b; };
unsigned char tab [[c6502::zeropage]][3] = {7, 8, 9};
struct s st [[c6502::zeropage]] = {1, 300};
unsigned long l [[c6502::zeropage]] = 70000;
unsigned char z [[c6502::zeropage]];
unsigned char other = 5;
unsigned char *ptr [[c6502::zeropage]] = &other;
int main(void) {
    if (tab[0] + tab[1] + tab[2] != 24) return 1;
    if (st.a != 1 || st.b != 300) return 2;
    if (l != 70000) return 3;
    if (z != 0) return 4;
    if (*ptr != 5) return 5;
    return 0;
}
)c";
  both(src, 0);
}

TEST_CASE("zp_globals: write through pointer to zp global") {
  // `n` is written through a pointer, so a cached copy of it
  // can't be reused across the store.
  std::string src = R"c(unsigned char n [[c6502::zeropage(0xA0)]] = 1;
unsigned char *p [[c6502::zeropage]];
int main(void) {
    unsigned char before = n;
    p = &n;
    *p = 5;
    return before * 10 + n;
}
)c";
  both(src, 15);
}

TEST_CASE("zp_globals: pointer global walks buffer") {
  std::string src = R"c(unsigned char buf[6] = {1, 2, 3, 4, 5, 6};
unsigned char *p [[c6502::zeropage(0xBA)]];
unsigned char sum [[c6502::zeropage]];
void add(unsigned char k) { sum += p[k]; }
int main(void) {
    p = buf;
    for (unsigned char i = 0; i < 6; i++) add(i);
    p = buf + 3;
    add(0);
    return sum;
}
)c";
  both(src, 21 + 4);
}

TEST_CASE("zp_globals: block static counter") {
  std::string src = R"c(unsigned char tick(void) {
    static unsigned char c [[c6502::zeropage]] = 10;
    return ++c;
}
int main(void) { tick(); tick(); return tick(); }
)c";
  both(src, 13);
}

TEST_CASE("zp_globals: global in pool range survives calls") {
  // The global sits where a function's locals would otherwise
  // go, and keeps its value across calls that use locals.
  std::string src = R"c(unsigned int keep [[c6502::zeropage(0x80)]] = 0;
unsigned int f(unsigned int a) {
    unsigned int x = a * 5, y = a + 7;
    return x ^ y;
}
int main(void) {
    keep = 1234;
    unsigned int r = f(3) + f(4);
    return keep - 1234 + (r == ((15u ^ 10u) + (20u ^ 11u)));
}
)c";
  both(src, 1);
}

}  // namespace sim_runs

const std::string kHeader = R"c(#include <stdint.h>
extern uint8_t *src [[c6502::zeropage]];
extern uint8_t total [[c6502::zeropage]];
extern uint16_t big [[c6502::zeropage(0x90)]];
uint8_t sum(uint8_t n);
)c";
// Defines `src`, `total`, and `big`, and calls `sum`.
const auto kUnitA = kHeader + R"c(uint8_t *src [[c6502::zeropage]];
uint8_t total [[c6502::zeropage]] = 5;
uint16_t big [[c6502::zeropage(0x90)]] = 1000;
uint8_t buf[4] = {1, 2, 3, 4};
int main(void) { src = buf; return sum(4) + big; }
)c";
// Defines `count`, which it places at $26 on its own, where unit A
// puts `src`; and reads and writes A's objects.
const auto kUnitB = kHeader + R"c(uint8_t count [[c6502::zeropage]] = 2;
uint8_t sum(uint8_t n) {
    for (uint8_t i = 0; i < n; i++) total += src[i] * count;
    big += total;
    return total;
}
)c";
namespace linking {

std::int64_t link_and_run(const std::vector<std::string>& sources) {
  TempDir tmp;
  std::vector<std::string> paths;
  for (std::size_t k = 0; k < sources.size(); ++k) {
    const std::string path = tmp.file(cat("u", k, ".o"));
    CHECK(compile_object(sources[k], path) == 0);
    paths.push_back(path);
  }
  const std::string out = tmp.file("out");
  const auto o = link(paths, out);
  REQUIRE_MESSAGE(o.status == 0, o.err());
  return build_linked_sim(out).run().return_int();
}

TEST_CASE("zp_globals: a linked program runs") {
  // total = 5 + (1+2+3+4) * 2 = 25, big = 1000 + 25.
  CHECK(link_and_run({kUnitA, kUnitB}) == 25 + 1025);
}

// Unit A leaves 4 bytes below $80; unit B promotes three busy
// 2-byte counters, so one goes back in the data segment.
const std::string kFull = R"c(#include <stdint.h>
uint8_t fill [[c6502::zeropage]][86];
uint16_t count(void);
int main(void) { fill[85] = 3; return count() + fill[85]; }
)c";
const std::string kCounters = R"c(#include <stdint.h>
uint16_t a = 1, b = 2, c = 3;
uint16_t count(void) {
    for (uint8_t i = 0; i < 10; i++) { a += 1; b += a; c += b; }
    return a + b + c;
}
)c";
TEST_CASE("zp_globals: promoted object that doesnt fit runs") {
  std::int64_t a = 1;
  std::int64_t b = 2;
  std::int64_t c = 3;
  for (int k = 0; k < 10; ++k) {
    a += 1;
    b += a;
    c += b;
  }
  CHECK(link_and_run({kFull, kCounters}) == ((a + b + c + 3) & 0xFFFF));
}

}  // namespace linking

}  // namespace
}  // namespace c6502::suite
