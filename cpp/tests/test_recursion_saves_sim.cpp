// The simulator tests of tests/test_recursion_saves.py: recursive
// programs whose recursive calls are marked
// `[[c6502::save_hardware_stack]]` (src/passes/recursion_saves.h) return
// what the unoptimized TAC returns, in both pipelines.
#include <cstdint>
#include <optional>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include "compile_case.h"
#include "doctest.h"
#include "sim_case.h"
#include "sim_helpers.h"

namespace c6502::suite {
namespace {

// The attribute, before a statement or declaration. In the programs
// below, `SAVE ` stands where it goes.
const std::string kSave = "[[c6502::save_hardware_stack]] ";
const std::vector<std::pair<std::string, std::string>> kPrograms = {
    {"globals", R"c(
unsigned char level;
unsigned char deepest;
unsigned char remaining;

void descend(void) {
    unsigned char here = level;
    if (here > deepest) deepest = here;
    if (remaining != 0) {
        remaining = remaining - 1;
        level = here + 1;
        SAVE descend();
        level = here;
    }
}

int main(void) {
    remaining = 5;
    descend();
    return deepest * 10 + level;
}
)c"},
    {"locals and parameters", R"c(
unsigned char depth;
unsigned char out[16];
unsigned char n;
void walk(unsigned char k) {
    unsigned char a = k * 3;
    unsigned char b = depth;
    unsigned int w = k * 100;
    out[n++] = a;
    if (k > 0) {
        depth = depth + 1;
        SAVE walk(k - 1);
        depth = b;
    }
    out[n++] = a + (unsigned char)w + b;
}
int fact(int x) {
    int y = x;
    if (x <= 1) return 1;
    SAVE y = y * fact(x - 1);
    return y;
}
int main(void) {
    walk(4);
    int s = 0;
    for (unsigned char i = 0; i < n; i++) s = s * 3 + out[i];
    return (s ^ fact(6)) & 0x7FFF;
}
)c"},
    {"return and declaration", R"c(
unsigned char weights[8] = {3, 1, 4, 1, 5, 9, 2, 6};
unsigned int sum_from(unsigned char i) {
    if (i >= 8) return 0;
    unsigned char w = weights[i];
    SAVE return w + sum_from(i + 1);
}
int even(unsigned char n);
int odd(unsigned char n) {
    if (n == 0) return 0;
    unsigned char k = n * 2;
    SAVE int r = even(n - 1);
    return r + k;
}
int even(unsigned char n) {
    if (n == 0) return 1;
    unsigned char k = n + 7;
    SAVE int r = odd(n - 1);
    return r * 3 + k;
}
int main(void) { return (int)sum_from(0) * 100 + even(6); }
)c"},
    {"in a loop", R"c(
unsigned int count_paths(unsigned char n) {
    unsigned int total = 0;
    if (n < 2) return 1;
    for (unsigned char k = 1; k <= 2; k++) {
        SAVE total += count_paths(n - k);
    }
    return total;
}
int main(void) { return count_paths(9); }
)c"},
    {"address taken", R"c(
void bump(unsigned char *p) { *p += 1; }
unsigned char walk2(unsigned char n) {
    unsigned char local = n;
    unsigned char keep = n * 3;
    bump(&local);
    if (n == 0) return local;
    SAVE unsigned char r = walk2(n - 1);
    return r + local + keep;
}
int main(void) { return walk2(5); }
)c"},
    {"long", R"c(
long acc(unsigned char n) {
    long big = (long)n * 70000;
    if (n == 0) return 1;
    SAVE long r = acc(n - 1);
    return r + big;
}
int main(void) { return (int)(acc(5) & 0x7FFF); }
)c"},
    {"carry flag", R"c(
#include <stdbool.h>
[[c6502::return_bool_in_carry]]
bool any_bit(unsigned char n, unsigned char mask) {
    unsigned char h = n & mask;
    if (n == 0) return 0;
    SAVE bool r = any_bit(n >> 1, mask);
    return r || h == 3;
}
int main(void) {
    int s = 0;
    for (unsigned char i = 0; i < 40; i++) s = s * 2 + any_bit(i, 3);
    return s & 0x7FFF;
}
)c"},
    {"zp_abi caller", R"c(
int rec(int d) {
    int k = d * 3;
    if (d <= 0) return 0;
    SAVE int r = rec(d - 1);
    return r + 1 + (k & 1);
}
int f0(unsigned char d [[c6502::in_a]], int p0) {
    int l = rec(3);
    return p0 + l + d;
}
int main(void) { return f0(2, 4); }
)c"},
    {"unmarked call on the cycle", R"c(
int pong(int n);
int ping(int n) {
    int a = n * 2;
    if (n <= 0) return 0;
    SAVE int r = pong(n - 1);
    return r + a;
}
int pong(int n) {
    int b = n + 5;
    if (n <= 0) return 1;
    int r = ping(n - 1);
    return r * 2 + b;
}
int main(void) { return ping(5); }
)c"},
};
TEST_CASE("recursion_saves: programs") {
  for (const auto& [name, marked] : kPrograms) {
    const std::string src = replace_all(marked, "SAVE ", kSave);
    const std::optional<asdl::Int> tac = run_program(src);
    REQUIRE(tac.has_value());
    const std::int64_t expected = static_cast<std::int64_t>(*tac) & 0xFFFF;
    for (bool optimize : {false, true}) {
      INFO(name << " optimize=" << optimize);
      auto result = build_sim(src, optimize).run(5'000'000);
      CHECK_FALSE(result.timed_out);
      CHECK((result.return_int_signed() & 0xFFFF) == expected);
    }
  }
}

}  // namespace
}  // namespace c6502::suite
