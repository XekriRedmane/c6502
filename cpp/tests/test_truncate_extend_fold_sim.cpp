// Byte loops compared against `sizeof`, whose extends
// src/passes/optimization/truncate_extend_fold.h folds, run in both
// pipelines, from tests/test_truncate_extend_fold.py.
#include "doctest.h"
#include "sim_case.h"

namespace c6502::suite {

TEST_CASE("truncate_extend_fold sim: byte loops against sizeof run") {
  // `i < sizeof a` compares at the width of `size_t`, `unsigned int`, so
  // `i` is promoted to `int` and then converted to `unsigned int` with a
  // copy, as it is for `i < 40U`. Copy propagation removes the copy after
  // the compare has become a `JumpIfCmp`, or keeps it when the extend has
  // other readers, as the address of `p[i]` is in the third loop, and the
  // narrowing looks through it.
  const char* src = R"c(#include <stdint.h>
uint8_t a[40];
uint8_t b[40];
uint8_t f(void) {
    for (uint8_t i = 0; i < sizeof a; i++)
        a[i] = b[i] + 1;
    for (uint8_t i = 0; i < 40U; i++)
        b[i] = a[i];
    uint8_t *p = (uint8_t *)a;
    for (uint8_t i = 0; i < sizeof a; i++)
        p[i] += 2;
    return b[39];
}
int main(void) {
    for (uint8_t i = 0; i < 40; i++)
        b[i] = i;
    return f();
}
)c";
  for (const bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    CHECK(build_sim(src, optimize).run().return_int() == 40);
  }
}

}  // namespace c6502::suite
