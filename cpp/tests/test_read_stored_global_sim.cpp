// Runs of `*++P`, whose stored global
// src/passes/optimization/read_stored_global.h reads in place of the
// temporary, in both pipelines, from tests/test_read_stored_global.py.
#include "doctest.h"
#include "sim_case.h"

namespace c6502::suite {

TEST_CASE("read_stored_global sim: each pre-increment reads the next element") {
  const int t[] = {3, 1, 4, 1, 5, 9, 2, 6};
  std::int64_t want = 0;
  int p = 0;
  for (int k = 0; k < 7; ++k) {
    ++p;
    want = (want * 3 + t[p] + 2 * t[p]) & 0xFFFF;
  }
  want = (want + p * 1000) & 0xFFFF;
  const char* src = R"c(
#include <stdint.h>
const uint8_t T[] = {3, 1, 4, 1, 5, 9, 2, 6};
extern const uint8_t *P [[c6502::zeropage(0xEC)]];
const uint8_t *Q;
uint8_t next(void) { return *++P; }
uint8_t nextq(void) { return *++Q + *Q; }
int main(void) {
    P = T; Q = T;
    uint16_t sum = 0;
    for (uint8_t k = 0; k < 7; k++)
        sum = sum * 3 + next() + nextq();
    return sum + (P - T) * 1000;
}
)c";
  for (const bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    CHECK((build_sim(src, optimize).run().return_int() & 0xFFFF) == want);
  }
}

}  // namespace c6502::suite
