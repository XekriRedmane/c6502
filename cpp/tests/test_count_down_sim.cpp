// Loops that src/passes/optimization/count_down.h counts down to 0, run
// in both pipelines, from tests/test_count_down.py.
#include <string>

#include "doctest.h"
#include "sim_case.h"

namespace c6502::suite {

TEST_CASE("count_down sim: loops counted down run as many times as before") {
  const std::string src = R"c(
#include <stdint.h>
uint16_t ticks;
void tick(void) { ticks++; }
uint8_t K;
void a(void) { for (uint8_t n = 0; n < 20; n++) tick(); }
void b(uint8_t k) { for (uint8_t n = 0; n < k; n++) tick(); }
void c(void) { for (uint8_t n = 3; n != 3; n++) tick(); }
void d(void) { for (uint8_t n = 7; n <= 254; n++) tick(); }
int main(void) {
    a(); uint16_t ra = ticks; ticks = 0;
    b(K); uint16_t rb = ticks; ticks = 0;
    c(); uint16_t rc = ticks; ticks = 0;
    d();
    return ra + rb * 2 + rc * 1000 + ticks * 4;
}
)c";
  for (const int k : {0, 1, 200, 255}) {
    std::string with_k = src;
    with_k.replace(with_k.find("uint8_t K;"), 10, "uint8_t K = " + std::to_string(k) + ";");
    for (const bool optimize : {false, true}) {
      INFO("k=" << k << " optimize=" << optimize);
      const sim::SimResult r = build_sim(with_k, optimize).run(5'000'000);
      CHECK((r.return_int() & 0xFFFF) == ((20 + 2 * k + 4 * 248) & 0xFFFF));
    }
  }
}

}  // namespace c6502::suite
