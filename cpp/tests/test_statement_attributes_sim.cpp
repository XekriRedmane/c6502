// A loop marked `[[c6502::keep]]` with a `break` and a `continue`, run
// in both pipelines, from tests/test_statement_attributes.py.
#include "doctest.h"
#include "sim_case.h"

namespace c6502::suite {

TEST_CASE("statement_attributes sim: break and continue in a kept loop") {
  // A `break` or `continue` carries the loop's label without the mark of
  // a kept loop, and once jumped to a label the kept loop never defined,
  // which crashed the CFG build under -O.
  const char* src = R"c(#include <stdint.h>
uint8_t even;
uint8_t f(uint8_t x) {
    uint8_t n = 0;
    [[c6502::keep]] for (;;) {
        if (--x == 0) break;
        n++;
        if (x & 1) continue;
        even++;
    }
    return n;
}
int main(void) { return f(7) * 16 + even; }
)c";
  for (const bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    // x runs 6 down to 1, so n counts 6 passes, and even counts the 3
    // with x even.
    CHECK(build_sim(src, optimize).run().return_int() == 6 * 16 + 3);
  }
}

}  // namespace c6502::suite
