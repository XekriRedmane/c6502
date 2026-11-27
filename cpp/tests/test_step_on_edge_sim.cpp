// The simulator tests of tests/test_step_on_edge.py: loops whose step
// src/passes/optimization/step_on_edge.h moves onto an edge store the
// bytes computed here in both pipelines.
#include <cstdint>
#include <string>
#include <tuple>
#include <vector>

#include "batch1_case.h"
#include "compile_case.h"
#include "doctest.h"
#include "sim/cpu_run.h"
#include "sim_case.h"

namespace c6502::suite {
namespace {

const std::string kSrc = R"c(
#include <stdint.h>
uint8_t seen[300];
uint16_t n;
void g(uint8_t i) { seen[n++] = i; }
void down(uint8_t first, uint8_t last) {
    uint8_t i = last;
    do g(i); while (i-- != first);
}
uint8_t up(uint8_t i, uint8_t last) {
    while (i++ != last)
        g(i);
    return i;
}
int main(void) {
    down(%d, %d);
    seen[n++] = up(%d, %d);
    return n;
}
)c";
// Returns the bytes `main` stores in `seen`.
Bytes model(int a, int b, int c, int d) {
  Bytes out;
  int i = b;
  while (true) {
    out.push_back(static_cast<std::uint8_t>(i));
    int old = i;
    i = (i - 1) & 0xFF;
    if (old == a) {
      break;
    }
  }
  i = c;
  while (true) {
    int old = i;
    i = (i + 1) & 0xFF;
    if (old == d) {
      break;
    }
    out.push_back(static_cast<std::uint8_t>(i));
  }
  out.push_back(static_cast<std::uint8_t>(i));
  return out;
}

TEST_CASE("step_on_edge: runs") {
  std::string all;
  for (const auto& [a, b, c, d] : std::vector<std::tuple<int, int, int, int>>{
           {0, 0, 0, 0}, {3, 7, 250, 255}, {0, 5, 4, 9}, {255, 255, 254, 255}, {10, 9, 0, 3}}) {
    auto want = model(a, b, c, d);
    if (std::ssize(want) > 300) {
      continue;
    }
    all += format_d(kSrc, {a, b, c, d});
    for (bool optimize : {false, true}) {
      INFO("args=" << a << ", " << b << ", " << c << ", " << d << " optimize=" << optimize);
      auto s = build_sim(format_d(kSrc, {a, b, c, d}), optimize);
      auto result = s.run(5'000'000);
      CHECK(result.return_int() == std::ssize(want));
      auto base = s.symbol("seen");
      auto got = slice(s.cpu.memory, base, base + std::ssize(want));
      CHECK(got == want);
    }
  }
  // zlib's CRC-32 of the 5 programs tests/test_step_on_edge.py ran, on
  // 27 Nov 2026.
  CHECK(sim::cpu_run::crc32(all) == 0xD4B90902u);
}

}  // namespace
}  // namespace c6502::suite
