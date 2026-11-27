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

auto sim_return(const auto& src) {
  auto s = build_sim(src, true);
  auto result = s.run(5'000'000);
  CHECK_FALSE_MESSAGE(result.timed_out, "simulator timed out");
  return result.return_int();
}

TEST_CASE("indexed_symbol_store: uint8 store then read back") {
  std::string src = R"c(#include <stdint.h>
static uint8_t arr[10];
int main(void) {
    arr[3] = 42;
    arr[7] = 99;
    return arr[3] + arr[7];
}
)c";
  CHECK(sim_return(src) == 42 + 99);
}

TEST_CASE("indexed_symbol_store: uint16 store then read back") {
  // 2-byte element — both bytes of the store reach memory.
  std::string src = R"c(#include <stdint.h>
static uint16_t arr[10];
int main(void) {
    arr[5] = 0x1234;
    return arr[5];
}
)c";
  CHECK((sim_return(src) & 0xFFFF) == 0x1234);
}

TEST_CASE("indexed_symbol_store: runtime index store") {
  std::string src = R"c(#include <stdint.h>
static uint8_t arr[20];
void put(uint8_t i, uint8_t v) { arr[i] = v; }
int main(void) {
    put(5, 50);
    put(12, 120);
    return arr[5] + arr[12];
}
)c";
  CHECK(sim_return(src) == 50 + 120);
}

}  // namespace
}  // namespace c6502::suite
