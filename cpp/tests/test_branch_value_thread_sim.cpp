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

const std::string kProgram = R"c(
#include <stdint.h>
uint16_t t[8] = {0, 2, 4, 6, 0x102, 0x104, 0x200, 0xFFFF};
int r[8];
static int scale(uint16_t a, uint16_t b) {
    if (b == 2)
        return a << 1;
    else if (b == 4)
        return a << 2;
    return a + b;
}
int main(void) {
    for (uint8_t i = 0; i != 8; i++)
        r[i] = scale(3, t[i]);
    return 0;
}
)c";
TEST_CASE("branch_value_thread: values") {
  std::vector<int> want = {3, 6, 12, 9, 0x105, 0x107, 0x203, 2};
  for (bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    auto s = build_sim(kProgram, optimize);
    s.run();
    auto base = s.symbol("r");
    std::vector<int> got;
    for (int k = 0; k < 8; ++k)
      got.push_back(static_cast<int>(s.cpu.memory[base + 2 * k] | s.cpu.memory[base + 2 * k + 1] << 8));
    CHECK(got == want);
  }
}

}  // namespace
}  // namespace c6502::suite
