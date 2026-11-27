// Programs that use typedef names (C99 §6.7.7), run in both pipelines,
// from tests/test_typedef.py.
#include "doctest.h"
#include "sim_case.h"

namespace c6502::suite {

namespace {

void check_returns(const char* src, std::int64_t expected) {
  for (const bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    const sim::SimResult r = build_sim(src, optimize).run(2'000'000);
    CHECK_FALSE(r.timed_out);
    CHECK(r.return_int_signed() == expected);
  }
}

}  // namespace

TEST_CASE("typedef sim: a struct and a pointer to it") {
  check_returns(R"c(typedef unsigned char u8;typedef struct { u8 x; int y; } point;typedef point *ppoint;int add(ppoint p) { return p->x + p->y; }int main(void) { point pt; ppoint pp = &pt; pt.x = 3; pt.y = 4; return add(pp) + sizeof(point); })c", 10);
}

TEST_CASE("typedef sim: a parameter, a for variable, and a block object hide the name") {
  check_returns(R"c(typedef int T;int f(int T) { return T * 2; }int main(void) { int s = 0; for (int T = 0; T < 3; T++) s += T; { int T = 10; s += T * f(1); } T y = s; return y; })c", 23);
}

TEST_CASE("typedef sim: function and function pointer typedefs") {
  check_returns(R"c(typedef int F(int); typedef int (*FP)(int);F inc; int inc(int x) { return x + 1; }int apply(F *f, FP g, int v) { return g(f(v)); }int main(void) { return apply(inc, inc, 4); })c", 6);
}

TEST_CASE("typedef sim: array typedefs") {
  check_returns(R"c(typedef int Row[3]; typedef Row Grid[2];int sum(Row r) { return r[0] + r[1] + r[2]; }int main(void) { Grid g = {{1, 2, 3}, {4, 5, 6}}; return sum(g[0]) * 10 + sum(g[1]); })c", 75);
}

TEST_CASE("typedef sim: a linked list through the tag the typedef introduces") {
  // The typedef introduces `struct node` at file scope, so the pointer in
  // `first`, declared before the struct's definition, refers to the same
  // tag as `head` does. Had it introduced a new tag in `first`'s block,
  // `return p;` wouldn't type-check.
  check_returns(R"c(typedef struct node node_t;node_t *head;node_t *first(void) { node_t *p = head; return p; }struct node { int v; node_t *next; };int sum(void) { int s = 0; node_t *p; for (p = first(); p; p = p->next) s += p->v; return s; }int main(void) { node_t a, b; a.v = 2; b.v = 5; a.next = &b; b.next = 0; head = &a; return sum(); })c", 7);
}

TEST_CASE("typedef sim: the types of <stdint.h>") {
  check_returns(R"c(#include <stdint.h>
uint8_t lo(uint16_t w) { return (uint8_t)w; }int main(void) { int32_t big = 70000; uint8_t uint8 = lo(0x1234); return (int)(big / 1000) + uint8 + sizeof(int64_t); })c", 70 + 0x34 + 8);
}

}  // namespace c6502::suite
