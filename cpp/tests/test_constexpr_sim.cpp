// The simulator tests of tests/test_constexpr.py: programs that check
// C23's `constexpr` objects (§6.7.2), the storage-class specifiers of
// compound literals (§6.5.3.6), and the reads of `static const` objects
// return 0 or the value computed here in both pipelines.
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

auto run(const auto& src, const auto& optimize) {
  auto result = build_sim(src, optimize).run(4'000'000);
  REQUIRE(!result.timed_out);
  return result.return_int_signed();
}

const std::string kProgram = R"c(#include <stdint.h>
#include <stddef.h>
constexpr int K = 47;
enum { E = K + 1 };
static int b = K + 1;
int array[K];
constexpr unsigned char UC = 200;
constexpr signed char SC = -5;
constexpr long BIG = 100000L;
constexpr bool T = true;
constexpr double D = 2.5;
constexpr float F = 47u;
constexpr int *P = nullptr;
constexpr nullptr_t N = nullptr;
struct inner { uint8_t a; int16_t b; };
struct s { uint8_t x; struct inner in; long y; };
constexpr struct s S = { 3, { 4, -300 }, 70000 };
constexpr struct s S2 = S;
static struct s S3 = S;
static long c = S.y + S.in.b;
static int e = (int)D;
static int *q = P;
constexpr char STR[] = "hi";
constexpr int ARR[3] = { 1, 2, 3 };
union u { uint16_t w; uint8_t b[2]; };
constexpr union u U = { 0x1234 };
static unsigned d = U.w;
constexpr int W = 12;
_BitInt(W) bi;
int sized[S.in.a];

int depth(int n) {
    constexpr int STEP = 2;
    const int *p = &STEP;
    if (n == 0) return *p;
    return depth(n - 1) + STEP;
}

int main(void) {
    constexpr uint8_t M = 5;
    static constexpr int SM = -7;
    uint8_t buf[M];
    for (uint8_t i = 0; i < M; i++) buf[i] = i;
    if (sizeof array != 2 * K) return 1;
    if (E != 48) return 2;
    if (b != 48) return 3;
    if (UC + SC != 195) return 4;
    if (BIG / 4 != 25000) return 5;
    if (!T) return 6;
    if ((int)(D * 2) != 5) return 7;
    if (F != 47.0f) return 8;
    if (P != 0 || q != 0) return 9;
    if (S2.in.b != -300 || S3.y != 70000) return 10;
    if (c != 70000 - 300) return 11;
    if (e != 2) return 12;
    if (STR[1] != 'i' || sizeof STR != 3) return 13;
    if (ARR[2] != 3) return 14;
    if (d != 0x1234) return 15;
    if (sizeof bi != 2 || sizeof sized != 8) return 16;
    switch (buf[4]) { case M - 1: break; default: return 17; }
    switch (SM) { case SM: break; default: return 18; }
    if (depth(3) != 8) return 19;
    const int *pk = &K;
    if (*pk != 47) return 20;
    int x = 3;
    switch (x) { case S.x: break; default: return 21; }
    if (N != nullptr) return 22;
    return 0;
}
)c";
namespace programs {
TEST_CASE("constexpr: programs: asm sim") {
  for (bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    CHECK(run(kProgram, optimize) == 0);
  }
}

}  // namespace programs

const std::string kLiterals = R"c(#include <stdint.h>
struct pt { int x, y; };
constexpr int K = (constexpr int){5} + 1;
int arr[(constexpr int){3}];
static int si = (constexpr struct pt){7, 8}.y;
int *keep(void) { return (static int[]){10, 20, 30}; }
int count(void) {
    int *p = &(static int){0};
    return ++*p;
}
int main(void) {
    switch (6) { case (constexpr uint8_t){6}: break; default: return 1; }
    if (K != 6) return 2;
    if (sizeof arr != 6) return 3;
    if (si != 8) return 4;
    int *q = keep();
    if (q[2] != 30) return 5;
    count(); count();
    if (count() != 3) return 6;
    if ((register uint8_t){9} + 1 != 10) return 7;
    if ((constexpr struct pt){1, 2}.x != 1) return 8;
    return 0;
}
)c";
namespace compound_literal_storage {
// A compound literal with `constexpr` is a compound literal
// constant (C23 §6.6p6), one with `static` has static storage
// inside a function, and one with `register` has no address.
TEST_CASE("constexpr: compound literal storage: asm sim") {
  for (bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    CHECK(run(kLiterals, optimize) == 0);
  }
}

}  // namespace compound_literal_storage

namespace static_consts {
// A `const` float or double static stored its value's integer
// part as its bits, and `-O` read a const float, double, or element
// of a const array of them as the number its bits make. Taking the
// address of a `static const` scalar left `-O` reading the storage
// that the asm const-static fold had dropped.
TEST_CASE("constexpr: float and double values") {
  for (const auto& [src, want] : std::vector<std::tuple<std::string, int>>{
           {"static const double D = 2.5; int main(void) { return (int)(D * 2); }", 5},
           {"static const float D = 2.5f; int main(void) { return (int)(D * 2); }", 5},
           {"static const double D = -0.75; int main(void) { return D < 0; }", 1},
           {"static const double A[2] = {1.5, 2.5}; int main(void) { return (int)(A[1] * 2); }", 5},
           {"static const float A[2] = {1.5f, 2.5f}; int main(void) { return (int)(A[1] * 2); }", 5}}) {
    for (bool optimize : {false, true}) {
      INFO("src=" << src << " optimize=" << optimize);
      CHECK(run(src, optimize) == want);
    }
  }
}

TEST_CASE("constexpr: address of a const scalar") {
  std::string src = "static const int K = 47; int main(void) { const int *pk = &K; return *pk + K; }";
  for (bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    CHECK(run(src, optimize) == 94);
  }
}

}  // namespace static_consts

}  // namespace
}  // namespace c6502::suite
