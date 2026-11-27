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

const std::string kSrc = R"c(#include <stdint.h>
uint16_t passes;
void delay(uint8_t count) {
    do {
        [[c6502::keep]] for (uint8_t y = 3; --y;) {
        }
        passes++;
    } while (--count);
}
int main(void) {
    delay(5);
    delay(0);
    return passes;
}
)c";
TEST_CASE("counter_to_index_reg: runs") {
  for (bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    auto out = build_sim(kSrc, optimize).run();
    CHECK(out.return_int() == 5 + 256);
  }
}

}  // namespace
}  // namespace c6502::suite
