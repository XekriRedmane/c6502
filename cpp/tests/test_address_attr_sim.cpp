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

// A stand-in for a ROM routine, placed at $D000 before the run:
//   CLC ; ADC #1 ; RTS
const auto kIncRoutine = Bytes{0x18, 0x69, 0x01, 0x60};
// Swaps the bytes of the 2-byte argument in A (low) and X (high)
// and returns the result the same way. $D0F0 is scratch.
//   STA $D0F0 ; TXA ; LDX $D0F0 ; RTS
const auto kSwapRoutine = Bytes{0x8D, 0xF0, 0xD0, 0x8A, 0xAE, 0xF0, 0xD0, 0x60};
TEST_CASE("address_attr: call fixed routine") {
  std::string src = R"c([[c6502::zp_abi, c6502::address(0xD000)]]
extern unsigned char inc(register unsigned char c);
int main(void) {
  unsigned char t = 0;
  for (unsigned char i = 0; i < 3; i++) t += inc(i);
  return t;
}
)c";
  for (bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    auto s = build_sim(src, optimize);
    poke(s.cpu.memory, 0xD000, kIncRoutine);
    CHECK(s.run().return_int() == 1 + 2 + 3);
  }
}

TEST_CASE("address_attr: carry flag result") {
  // A routine that reports its result in the carry flag:
  //   CMP #5 ; RTS        C set iff A >= 5
  std::string src = R"c([[c6502::zp_abi, c6502::return_bool_in_carry, c6502::address(0xD000)]]
extern bool at_least_5(register unsigned char a);
int main(void) {
  int n = 0;
  for (unsigned char i = 0; i < 9; i++)
    if (at_least_5(i)) n += 10; else n += 1;
  return n;
}
)c";
  for (bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    auto s = build_sim(src, optimize);
    poke(s.cpu.memory, 0xD000, Bytes{0xC9, 0x05, 0x60});
    CHECK(s.run().return_int() == 4 * 10 + 5);
  }
}

TEST_CASE("address_attr: two byte argument and result") {
  std::string src = R"c([[c6502::zp_abi, c6502::address(0xD000)]]
extern unsigned int swap(register unsigned int v);
unsigned int k = 0x1234;
int main(void) { return swap(k) + 1; }
)c";
  for (bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    auto s = build_sim(src, optimize);
    poke(s.cpu.memory, 0xD000, kSwapRoutine);
    CHECK(s.run().return_int() == 0x3412 + 1);
  }
}

TEST_CASE("address_attr: pointer result") {
  // A pointer comes back as an `unsigned int` does, low byte in
  // A and high byte in X:
  //   LDA #$34 ; LDX #$12 ; RTS
  std::string src = R"c([[c6502::zp_abi, c6502::address(0xD000)]]
extern unsigned char *where(void);
int main(void) { return (int)(unsigned int)where(); }
)c";
  for (bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    auto s = build_sim(src, optimize);
    poke(s.cpu.memory, 0xD000, Bytes{0xA9, 0x34, 0xA2, 0x12, 0x60});
    CHECK(s.run().return_int() == 0x1234);
  }
}

TEST_CASE("address_attr: fixed objects") {
  std::string src = R"c(extern volatile unsigned char port [[c6502::address(0xC800)]];
extern unsigned int word [[c6502::address(0xC810)]];
extern unsigned char tbl [[c6502::address(0xC820)]][4];
int main(void) {
  port = 5;
  unsigned int t = word;
  for (unsigned char i = 0; i < 4; i++) t += tbl[i];
  tbl[0] = port + 1;
  return t;
}
)c";
  for (bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    auto s = build_sim(src, optimize);
    auto& mem = s.cpu.memory;
    mem[0xC810] = 0x34;
    mem[0xC811] = 0x12;
    poke(mem, 0xC820, Bytes{1, 2, 3, 4});
    auto result = s.run();
    CHECK(result.return_int() == 0x1234 + 10);
    CHECK(result.memory[0xC800] == 5);
    CHECK(result.memory[0xC820] == 6);
  }
}

TEST_CASE("address_attr: poll key") {
  // The example in docs/attributes.md. Clearing the strobe on
  // one path used to leave A = 0 at the return under
  // -O, because two passes lost track of the `LDA #0`
  // inside a volatile `STA`.
  std::string src = R"c(extern volatile unsigned char KBD [[c6502::address(0xC000)]];
extern volatile unsigned char KBDSTRB [[c6502::address(0xC010)]];
unsigned char poll_key(void) {
  unsigned char k = KBD;
  if (k & 0x80) KBDSTRB = 0;
  return k;
}
int main(void) { return poll_key(); }
)c";
  for (bool optimize : {false, true}) {
    for (const auto& key : {0xC1, 0x41}) {
      INFO("optimize=" << optimize << " key=" << key);
      auto s = build_sim(src, optimize);
      s.cpu.memory[0xC000] = key;
      s.cpu.memory[0xC010] = 0x55;
      auto result = s.run();
      CHECK(result.return_int() == key);
      CHECK(result.memory[0xC010] == (key & 0x80 ? 0 : 0x55));
    }
  }
}

TEST_CASE("address_attr: wait through pointer") {
  // With no key in KBD the loop has to keep waiting, so the run
  // reaches the cycle cap. It used to test a stale A and exit.
  std::string src = R"c(int main(void) {
  while (!(*(volatile unsigned char *)0xC000 & 0x80)) ;
  return 7;
}
)c";
  for (bool optimize : {false, true}) {
    for (const auto& [key, waits] : std::vector<std::tuple<int, bool>>{{0x00, true}, {0xC1, false}}) {
      INFO("optimize=" << optimize << " key=" << key);
      auto s = build_sim(src, optimize);
      s.cpu.memory[0xC000] = key;
      auto result = s.run(20'000);
      CHECK(result.timed_out == waits);
      if (!waits) {
        CHECK(result.return_int() == 7);
      }
    }
  }
}

TEST_CASE("address_attr: address of fixed object") {
  std::string src = R"c(extern unsigned char port [[c6502::address(0xC800)]];
int main(void) { return (int)&port; }
)c";
  for (bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    auto got = build_sim(src, optimize).run().return_int();
    CHECK((got & 0xFFFF) == 0xC800);
  }
}

}  // namespace
}  // namespace c6502::suite
