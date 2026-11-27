#include <cstdint>
#include <string>
#include <tuple>
#include <vector>

#include "compile_case.h"
#include "doctest.h"
#include "sim_case.h"
#include "sim_helpers.h"

namespace c6502::suite {
namespace {

TEST_CASE("direct_index_load: indexed store lands correctly") {
  std::string src = R"c(#include <stdint.h>
static uint8_t * const buf = (uint8_t * const)0x4100;
void put(uint8_t value, uint8_t col) {
    buf[col] = value;
}
int main(void) { put(0x55, 17); return 0; }
)c";
  auto s = build_sim(src, Pipeline::kOptimized);
  auto result = s.run(5'000'000);
  CHECK_FALSE(result.timed_out);
  CHECK(result.memory[0x4100 + 17] == 0x55);
}

}  // namespace
}  // namespace c6502::suite
