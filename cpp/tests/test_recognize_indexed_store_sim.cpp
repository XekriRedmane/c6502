// Stores that the IndexedStore recognizer makes `STA $C,X`, run in the
// simulator, from tests/test_recognize_indexed_store.py.
#include "doctest.h"
#include "sim_case.h"

namespace c6502::suite {

TEST_CASE("recognize_indexed_store sim: the byte lands at the indexed address") {
  const sim::SimResult r = build_sim(R"c(#include <stdint.h>
static uint8_t * const buf = (uint8_t * const)0x4100;
void put(uint8_t value, uint8_t col) {
    buf[col] = value;
}
int main(void) { put(0x55, 17); return 0; }
)c", Pipeline::kOptimized).run(5'000'000);
  CHECK_FALSE(r.timed_out);
  CHECK(r.memory[0x4100 + 17] == 0x55);
}

TEST_CASE("recognize_indexed_store sim: a store at a constant offset from the index lands") {
  const sim::SimResult r = build_sim(R"c(#include <stdint.h>
static uint8_t * const buf = (uint8_t * const)0x4000;
void put(uint8_t value, uint8_t col) {
    buf[200 + col] = value;
}
int main(void) { put(0x99, 50); return 0; }
)c", Pipeline::kOptimized).run(5'000'000);
  CHECK_FALSE(r.timed_out);
  // 0x4000 + 200 + 50 = 0x40FA.
  CHECK(r.memory[0x40FA] == 0x99);
}

TEST_CASE("recognize_indexed_store sim: both pipelines store at the address") {
  const char* src = R"c(#include <stdint.h>
static uint8_t * const buf = (uint8_t * const)0x4200;
void put(uint8_t value, uint8_t col) {
    buf[col] = value;
}
int main(void) { put(0x33, 9); return 0; }
)c";
  for (const bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    const sim::SimResult r = build_sim(src, optimize).run(5'000'000);
    CHECK_FALSE(r.timed_out);
    CHECK(r.memory[0x4200 + 9] == 0x33);
  }
}

TEST_CASE("recognize_indexed_store sim: a negative signed char index") {
  // `buf[i]` with `int8_t i = -1` writes $41FF. The fold accepted the
  // SignExtend of i, and `STA $4200,X` wrote $42FF.
  const char* src = R"c(#include <stdint.h>
static uint8_t * const buf = (uint8_t * const)0x4200;
void put(uint8_t value, int8_t col) {
    buf[col] = value;
}
int main(void) { put(0x33, -1); return 0; }
)c";
  for (const bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    const sim::SimResult r = build_sim(src, optimize).run(5'000'000);
    CHECK(r.memory[0x41FF] == 0x33);
    CHECK(r.memory[0x42FF] == 0);
  }
}

}  // namespace c6502::suite
