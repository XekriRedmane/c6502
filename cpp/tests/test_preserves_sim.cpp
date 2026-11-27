// The simulator tests of tests/test_preserves.py: loops that keep a
// counter, a `register` local, or an immediate in X or Y across calls of
// stand-in routines declared `[[c6502::preserves_x]]` or `preserves_y`
// add up what the routines are given, and a routine that breaks the
// declaration breaks the loop.
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

// A routine at $D000 that takes A, keeps X, and destroys Y.
const std::string kKeepsX = R"c([[c6502::zp_abi, c6502::preserves_x, c6502::address(0xD000)]]
void put(register unsigned char c);
)c";
// The same routine, declared without the promise.
const std::string kPlain = R"c([[c6502::zp_abi, c6502::address(0xD000)]]
void put(register unsigned char c);
)c";
// `buf` is summed into `total` by the stand-in, one byte per call.
const std::string kSumLoop = R"c(unsigned char buf[8] = {1, 2, 3, 4, 5, 6, 7, 8};
unsigned char n = 8;
void send(void) {
    for (unsigned char i = 0; i != n; i++)
        put(buf[i]);
}
int main(void) { send(); return 0; }
)c";
const auto kTotal = 0x3100;
// CLC ; ADC total ; STA total ; LDY #$FF ; RTS
const auto kStandin = Bytes{0x18, 0x6D, 0x00, 0x31, 0x8D, 0x00, 0x31, 0xA0, 0xFF, 0x60};
namespace count_up_loop {
TEST_CASE("preserves: count up loop: runs") {
  for (const auto& decl : {kKeepsX, kPlain}) {
    for (bool optimize : {false, true}) {
      INFO("preserves=" << (decl == kKeepsX) << " optimize=" << optimize);
      auto s = build_sim(decl + kSumLoop, optimize);
      poke(s.cpu.memory, 0xD000, kStandin);
      s.cpu.memory[kTotal] = 0;
      auto result = s.run();
      CHECK(result.memory[kTotal] == 36);
    }
  }
}

TEST_CASE("preserves: the declaration is trusted") {
  // A stand-in that changes X while declared `preserves_x` breaks
  // the loop, which is what shows the compiler relies on it.
  auto clobbers_x = Bytes{0x18, 0x6D, 0x00, 0x31, 0x8D, 0x00, 0x31, 0xA2, 0x07, 0x60};
  // ... LDX #7 ; RTS
  auto s = build_sim(kKeepsX + kSumLoop, Pipeline::kOptimized);
  poke(s.cpu.memory, 0xD000, clobbers_x);
  s.cpu.memory[kTotal] = 0;
  auto result = s.run();
  CHECK(result.memory[kTotal] != 36);
}

}  // namespace count_up_loop

auto run_total(const auto& src, const auto& standin, const auto& optimize) {
  auto s = build_sim(src, optimize);
  poke(s.cpu.memory, 0xD000, standin);
  s.cpu.memory[kTotal] = 0;
  return s.run().memory[kTotal];
}

namespace register_local_in_y {
// The register allocator keeps a value, here a `register` local,
// in Y across a call that preserves Y.
const std::string kKeepsY = R"c([[c6502::zp_abi, c6502::preserves_y, c6502::address(0xD000)]]
void put(register unsigned char c);
)c";
const std::string kSrc = R"c(unsigned char buf[8] = {1, 2, 3, 4, 5, 6, 7, 8};
unsigned char n = 8;
void send(void) {
    register unsigned char i;
    for (i = 0; i < n; i++) put(buf[i]);
}
int main(void) { send(); return 0; }
)c";
// CLC ; ADC total ; STA total ; LDX #$FF ; RTS (keeps Y)
const auto kStandin = Bytes{0x18, 0x6D, 0x00, 0x31, 0x8D, 0x00, 0x31, 0xA2, 0xFF, 0x60};
TEST_CASE("preserves: register local iny: runs") {
  for (bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    CHECK(run_total(kKeepsY + kSrc, kStandin, optimize) == 36);
  }
}

}  // namespace register_local_in_y

namespace immediate_in_x {
// `redundant_load` keeps an immediate it knows X holds across a
// call that preserves X, but not one that doesn't, and never a
// memory equivalence, since the callee may write the memory.
const std::string kDecls = R"c([[c6502::zp_abi, c6502::preserves_x, c6502::address(0xD000)]]
void keep(register unsigned char c [[c6502::in_x]]);
[[c6502::zp_abi, c6502::address(0xD000)]]
void lose(register unsigned char c [[c6502::in_x]]);
unsigned char g = 4;
)c";
// TXA ; CLC ; ADC total ; STA total ; LDY #$FF ; RTS (keeps X)
const auto kStandin = Bytes{0x8A, 0x18, 0x6D, 0x00, 0x31, 0x8D, 0x00, 0x31, 0xA0, 0xFF, 0x60};
TEST_CASE("preserves: immediate inx: runs") {
  auto src = kDecls + R"c(int main(void) {
    keep(5); keep(5); keep(7); keep(g); keep(g);
    return 0;
}
)c";
  for (bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    CHECK(run_total(src, kStandin, optimize) == 25);
  }
}

}  // namespace immediate_in_x

}  // namespace
}  // namespace c6502::suite
