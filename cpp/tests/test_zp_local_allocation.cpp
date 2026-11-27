// The tests of tests/test_zp_local_allocation.py that read the `-S -O`
// text of a unit compiled on its own with its pool of zero page full.
// The simulator's runs are in test_zp_local_allocation_sim.cpp.
#include <map>
#include <string>

#include "compile_case.h"
#include "doctest.h"

namespace suite = c6502::suite;

TEST_CASE("zp_local_allocation: a unit's slots go below the pool, around the runtime's block") {
  // A unit compiled on its own takes the free bytes below the pool
  // before it spills out of zero page, as the linker does, so the asm
  // passes that follow values through zero page see its slots.
  const std::string text =
      suite::codegen(R"(
#include <stdint.h>
uint8_t pool [[c6502::zeropage(0x80)]][128];
static uint8_t g(uint8_t d [[c6502::in_x]], uint8_t x) { return x + d; }
uint8_t f(uint8_t d [[c6502::in_x]], uint8_t a, uint8_t b) {
    uint8_t s = g(d, a);
    uint8_t t = g(d, b);
    return s + t + a + b;
}
int main(void) { pool[0] = f(1, 3, 4); return pool[0]; }
)", true);
  std::map<std::string, int> equs;
  for (const std::string& line : suite::lines(text)) {
    if (!suite::contains(line, "\tEQU\t$")) continue;
    const std::string name = line.substr(0, line.find('\t'));
    const std::size_t dollar = line.find('$');
    const std::size_t end = line.find('$', dollar + 1);
    equs[name] = std::stoi(line.substr(dollar + 1, end == std::string::npos ? end : end - dollar - 1),
                           nullptr, 16);
  }
  REQUIRE(equs.count("SSP") == 1);
  REQUIRE(equs.count("DPTR") == 1);
  for (const std::string name : {"__zpabi_f__a", "__zpabi_f__b", "__zpabi_g__x"}) {
    INFO(name);
    REQUIRE(equs.count(name) == 1);
    CHECK(equs[name] < 0x80);
    // The runtime's block runs from SSP through DPTR's second byte.
    CHECK_FALSE((equs[name] >= equs["SSP"] && equs[name] < equs["DPTR"] + 2));
  }
}

TEST_CASE("zp_local_allocation: a function whose final allocation spills gets a larger pool") {
  // Lode Runner's `guard_find_candidate_row_above`. Its preliminary
  // allocation used 6 bytes, and with a pool of 6 the final allocation
  // had no two adjacent free bytes left for one of its row pointers, so
  // a byte value went to a soft-stack frame.
  const std::string text = suite::codegen(R"(
#include <stdint.h>
#include <stdbool.h>
extern uint8_t BG [[c6502::address(0x6000)]][16][28];
extern uint8_t LIMIT [[c6502::address(0x01)]];
bool is_floor(uint8_t sprite) { return sprite == 1 || sprite == 2; }
uint8_t f(uint8_t row, uint8_t col) {
    for (;;) {
        if (BG[row][col] != 3)
            return row;
        uint8_t ladder_row = row;
        row--;
        if (col != 0 && (is_floor(BG[ladder_row][col - 1]) ||
                         BG[row][col - 1] == 4)) {
            if (row <= LIMIT)
                return row;
        }
        if (col != 27 && (is_floor(BG[ladder_row][col + 1]) ||
                          BG[row][col + 1] == 4)) {
            if (row <= LIMIT)
                return row;
        }
        if (row == 0)
            return row;
    }
}
int main(void) {
    LIMIT = 9;
    for (uint8_t r = 0; r < 16; r++) BG[r][5] = 3;
    BG[9][4] = 4;
    return f(12, 5);
}
)", true);
  CHECK_FALSE(suite::contains(text, "prologue"));
}
