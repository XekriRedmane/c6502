// The simulator tests of tests/test_direct_member_access.py: programs
// whose accesses through `&obj + k`
// src/passes/optimization/direct_member_access.h makes direct accesses
// to byte `k` of `obj` give the same value on the unoptimized TAC and in
// both pipelines.
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

const std::string kS = R"c(struct s { unsigned char a; int b; unsigned char c; };
)c";
void check(const auto& src, const auto& expected) {
  // main()'s result on the unoptimized TAC.
  CHECK(run_program(src) == expected);
  for (bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    auto got = build_sim(src, optimize).run().return_int();
    CHECK(got == expected);
  }
}

TEST_CASE("direct_member_access: global and local members") {
  check(kS + R"c(struct s g;
unsigned char h(unsigned char x) {
    struct s r;
    g.c = x;
    g.b++;
    g.b += 300;
    r.a = x;
    r.c = g.a + 1;
    r.b = g.b;
    return r.a + r.c + (r.b == 301);
}
int main(void) { g.a = 4; return h(3) * 10 + g.c; }
)c",
        (3 + 5 + 1) * 10 + 3);
}

TEST_CASE("direct_member_access: copies in and out") {
  // A struct copied into a local, read by member, written by
  // member, and copied out again.
  check(kS + R"c(struct s g, out;
int main(void) {
    struct s a, b;
    g.a = 1; g.b = 1000; g.c = 3;
    a = g;
    b = a;
    b.b = a.b + 1;
    out = b;
    return out.a + out.b + out.c + a.b;
}
)c",
        1 + 1001 + 3 + 1000);
}

TEST_CASE("direct_member_access: nested struct and array members") {
  check(R"c(struct in { unsigned char x; int y; };
struct out { int k; struct in i; unsigned char t[3]; };
struct out g;
int main(void) {
    struct out o;
    unsigned char a[4];
    o.k = 7; o.i.x = 8; o.i.y = 900; o.t[2] = 5; o.t[0] = 6;
    a[0] = 1; a[3] = 2;
    g.i.y = o.i.y + a[3];
    g.t[1] = o.t[2] + a[0];
    return o.k + o.i.x + g.i.y + g.t[1] + o.t[0];
}
)c",
        7 + 8 + 902 + 6 + 6);
}

TEST_CASE("direct_member_access: address escapes after member writes") {
  // `r` is written by member, then passed by pointer, so its
  // bytes have to be in memory where `sum` reads them.
  check(kS + R"c(int sum(struct s *p) { return p->a + p->b + p->c; }
int main(void) {
    struct s r;
    r.a = 2; r.b = 40; r.c = 5;
    int t = sum(&r);
    r.b = 100;
    return t + sum(&r);
}
)c",
        47 + 107);
}

TEST_CASE("direct_member_access: union") {
  check(R"c(union u { unsigned int w; unsigned char b[2]; };
int main(void) {
    union u x;
    x.w = 0x1234;
    x.b[0] = 0x56;
    return x.w;
}
)c",
        0x1256);
}

TEST_CASE("direct_member_access: member in a loop") {
  check(kS + R"c(int main(void) {
    struct s r;
    r.b = 0; r.c = 0;
    for (unsigned char i = 0; i < 10; i++) {
        r.b += i;
        if (i & 1) r.c++;
    }
    return r.b * 10 + r.c;
}
)c",
        45 * 10 + 5);
}

}  // namespace
}  // namespace c6502::suite
