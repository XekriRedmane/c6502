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

const std::string kCarry = "[[c6502::return_bool_in_carry]] ";
const std::string kBool = R"c(#include <stdbool.h>
)c";
namespace sim_runs {
// Each program runs in both pipelines. Without -O every
// callee has a frame, so its return goes through the `PHP` / `PLP`
// epilogue.
auto check(const auto& src, const auto& expected) {
  for (bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    auto got = build_sim(src, optimize).run();
    CHECK(got.return_int() == expected);
  }
}

TEST_CASE("carry_return: branch and value uses") {
  auto src = kBool + kCarry + (R"c(bool is_small(unsigned char x) { return x < 10; }
)c" + kCarry + R"c(bool always(void) { return 1; }
unsigned char g;
int main(void) {
    unsigned char n = 0;
    if (is_small(3)) n += 1;
    if (!is_small(30)) n += 2;
    if (always()) n += 4;
    g = is_small(5) + is_small(50) * 2;
    return n * 10 + g;
}
)c");
  check(src, 71);
}

TEST_CASE("carry_return: search loop") {
  // The shape the attribute is for: `return true` from inside
  // a loop, `return false` after it.
  auto src = kBool + (R"c(unsigned char tab[5] = {4, 9, 16, 25, 36};
)c" + kCarry + R"c(bool contains(unsigned char k) {
    for (unsigned char i = 0; i < 5; i++)
        if (tab[i] == k) return 1;
    return 0;
}
int main(void) {
    int n = 0;
    for (unsigned char k = 0; k < 40; k++)
        if (contains(k)) n += k;
    return n;
}
)c");
  check(src, 4 + 9 + 16 + 25 + 36);
}

TEST_CASE("carry_return: nonzero value is true") {
  // `return x & 4;` returns `(x & 4) != 0`, so a caller sees 1.
  auto src = kBool + kCarry + (R"c(bool bit2(unsigned int x) { return x & 4; }
)c" + kCarry + R"c(bool nonnull(int *p) { return p; }
int v;
int main(void) {
    return bit2(4) + bit2(3) * 10 + bit2(0x104) * 100
        + nonnull(&v) * 1000 + nonnull(0) * 10000;
}
)c");
  check(src, 1 + 100 + 1000);
}

TEST_CASE("carry_return: sim: recursive soft stack") {
  // Recursion keeps `is_even` on the soft stack even with
  // -O, so its return goes through the frame teardown.
  auto src = kBool + kCarry + R"c(bool is_even(unsigned char n) {
    if (n == 0) return 1;
    return !is_even(n - 1);
}
int main(void) {
    int r = 0;
    for (unsigned char i = 0; i < 9; i++)
        r = r * 2 + is_even(i);
    return r;
}
)c";
  check(src, 0b101010101);
}

TEST_CASE("carry_return: carry to carry") {
  auto src = kBool + kCarry + (R"c(bool odd(unsigned char x) { return x & 1; }
)c" + kCarry + R"c(bool odd_too(unsigned char x) { return odd(x); }
)c" + kCarry + R"c(bool neither(unsigned char x) { return !odd(x) && !odd(x >> 1); }
int main(void) {
    int r = 0;
    for (unsigned char i = 0; i < 8; i++)
        r = r * 4 + odd_too(i) * 2 + neither(i);
    return r;
}
)c");
  auto expected = 0;
  for (std::int64_t i = 0; i < 8; ++i) {
    expected = expected * 4 + (i & 1) * 2 + ((i & 3) == 0);
  }
  check(src, expected & 0xFFFF);
}

TEST_CASE("carry_return: loop condition") {
  auto src = kBool + kCarry + R"c(bool done(unsigned char i) { return i * 3 > 20; }
int main(void) {
    unsigned char i = 0;
    while (!done(i)) i++;
    return i;
}
)c";
  check(src, 7);
}

}  // namespace sim_runs

const std::string kZc = R"c(struct zc { unsigned char v; bool end; };
)c";
const std::string kWv = R"c(struct wv { int v; bool end; };
)c";

namespace struct_sim {
auto check(const auto& src, const auto& expected) {
  CHECK(run_program(src) == expected);
  for (bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    auto got = build_sim(src, optimize).run();
    CHECK(got.return_int() == expected);
  }
}

TEST_CASE("carry_return: byte value") {
  // The shape of Zork's get_next_zchar: a value, and a flag that
  // marks the end of the data.
  auto src = kZc + (R"c(unsigned char data[6] = {5, 7, 9, 0xFF, 3, 4};
unsigned char pos;
)c" + kCarry + R"c(struct zc next(void) {
    struct zc r;
    r.v = data[pos++];
    r.end = r.v == 0xFF;
    return r;
}
int main(void) {
    int sum = 0;
    for (;;) {
        struct zc c = next();
        if (c.end) break;
        sum += c.v;
    }
    return sum * 10 + pos;
}
)c");
  check(src, 21 * 10 + 4);
}

TEST_CASE("carry_return: int value") {
  auto src = kWv + kCarry + R"c(struct wv triple(int x) {
    struct wv r;
    r.v = x * 3;
    r.end = x < 0;
    return r;
}
int main(void) {
    struct wv a = triple(300);
    struct wv b = triple(-7);
    return a.v + a.end * 10000 + b.v * 2 + b.end * 20000;
}
)c";
  check(src, 900 + 20000 - 42);
}

TEST_CASE("carry_return: flag is zero or one") {
  // The caller rebuilds the flag from C, so any nonzero flag in
  // the callee arrives as 1.
  auto src = kZc + kCarry + R"c(struct zc f(unsigned char x) {
    struct zc r;
    r.v = x;
    r.end = x & 6;
    return r;
}
int main(void) {
    struct zc a = f(4);
    struct zc b = f(9);
    return a.end * 100 + b.end * 10 + a.v + b.v;
}
)c";
  check(src, 100 + 13);
}

TEST_CASE("carry_return: struct sim: recursive soft stack") {
  // Recursion keeps `count` on the soft stack, so A, X, and C all
  // cross its frame teardown.
  auto src = kWv + kCarry + R"c(struct wv count(unsigned char n) {
    struct wv r;
    if (n == 0) { r.v = 1000; r.end = 1; return r; }
    r = count(n - 1);
    r.v = r.v + n;
    r.end = !r.end;
    return r;
}
int main(void) {
    struct wv a = count(4);
    struct wv b = count(5);
    return a.v + a.end * 10000 + b.end * 20000;
}
)c";
  check(src, 1010 + 10000);
}

TEST_CASE("carry_return: returned call") {
  // `return f();` passes the callee's A and C straight through.
  auto src = kZc + kCarry + (R"c(struct zc inner(unsigned char x) {
    struct zc r; r.v = x + 1; r.end = x > 5; return r;
}
)c" + kCarry + R"c(struct zc outer(unsigned char x) { return inner(x * 2); }
int main(void) {
    struct zc a = outer(2);
    struct zc b = outer(3);
    return a.v * 100 + a.end * 10 + b.end + b.v * 1000;
}
)c");
  check(src, 500 + 0 + 1 + 7000);
}

TEST_CASE("carry_return: value used before the flag") {
  // Zork's `PAGE_TABLE_INDEX2 = found.slot; if (found.not_found)`.
  auto src = kZc + (R"c(unsigned char data[6] = {5, 7, 9, 0xFF, 3, 4};
unsigned char pos, last;
)c" + kCarry + R"c(struct zc next(void) {
    struct zc r;
    r.v = data[pos++];
    r.end = r.v == 0xFF;
    return r;
}
int main(void) {
    int sum = 0;
    for (;;) {
        struct zc c = next();
        last = c.v;
        if (c.end) break;
        sum += c.v;
    }
    return sum * 10 + pos + last;
}
)c");
  check(src, 21 * 10 + 4 + 0xFF);
}

TEST_CASE("carry_return: fixed address routine") {
  // A routine at $D000 that returns 42 in A with C set.
  auto src = kZc + R"c([[c6502::zp_abi, c6502::return_bool_in_carry, c6502::address(0xD000)]]
struct zc rom(void);
int main(void) { struct zc c = rom(); return c.v * 2 + c.end; }
)c";
  auto lda_sec_rts = Bytes{0xA9, 42, 0x38, 0x60};
  for (bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    auto s = build_sim(src, optimize);
    poke(s.cpu.memory, 0xD000, lda_sec_rts);
    CHECK(s.run().return_int() == 85);
  }
}

}  // namespace struct_sim

}  // namespace
}  // namespace c6502::suite
