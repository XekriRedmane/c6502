#include "doctest.h"
#include "sim_case.h"

namespace c6502::suite {

TEST_CASE("asm_dead_store sim: a pointer kept in numbered zero-page bytes") {
  // A function without a private pool keeps a pointer in numbered
  // zero-page bytes, `ZP(addr)`. A load through it reads both bytes,
  // though the bytes the load reaches can't be in the pool. The stores
  // of a pointer an indirect call returned were dropped, and `pick()[0]`
  // read through whatever the bytes held.
  const char* src = R"c(
            unsigned char table[8] = {10, 20, 30, 40, 50, 60, 70, 80};
            static unsigned char *last(void) { return &table[7]; }
            static unsigned char *(*pick)(void) = last;
            int main(void) { unsigned char *c = pick(); return *c; }
        )c";
  for (const bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    CHECK(build_sim(src, optimize).run().return_int() == 80);
  }
}

TEST_CASE("asm_dead_store sim: a tail call keeps the stores to an address-taken local") {
  // A `JMP` to another function (a tail call) leaves the function's own
  // `__local_*` bytes dead, except one whose address the function takes,
  // since the callee may have been given it. Before this, `return
  // sum(a, 3);` with a local array `a` lost the stores that fill `a`,
  // and `sum` read whatever was there.
  const char* src = R"c(unsigned char sum(unsigned char *a, unsigned char n) {
    unsigned char s = 0; while (n--) s += a[n]; return s; }
unsigned char wrap(void) {
    unsigned char a[3]; a[0] = 4; a[1] = 5; a[2] = 6;
    return sum(a, 3); }
int main(void) { return wrap(); }
)c";
  for (const bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    CHECK(build_sim(src, optimize).run().return_int() == 15);
  }
}

}  // namespace c6502::suite
