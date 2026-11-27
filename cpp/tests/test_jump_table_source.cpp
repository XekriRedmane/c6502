// The `-S -O` text of tests/test_jump_table.py's switches. Their
// simulator runs stay in Python.
#include <string>

#include "compile_case.h"
#include "doctest.h"

namespace suite = c6502::suite;

TEST_CASE("jump_table: each switch dispatches through a table") {
  const std::string text = suite::codegen(R"(
#include <stdint.h>
uint8_t r[25];
static uint8_t pick(uint8_t n) {
    switch (n) {
    case 0: return 7;
    case 1: return 11;
    case 2: return 13;
    case 4: return 17;
    case 5: return 19;
    case 6: return 23;
    case 7: return 29;
    case 9: return 31;
    case 10: return 37;
    case 11: return 41;
    default: return 1;
    }
}
static uint8_t pick_high(uint8_t n) {
    switch (n) {
    case 100: return 2;
    case 101: return 3;
    case 102: return 5;
    case 103: return 7;
    case 104: return 11;
    case 105: return 13;
    case 106: return 17;
    case 107: return 19;
    case 108: return 23;
    default: return 0;
    }
}
int main(void) {
    for (uint8_t i = 0; i != 13; i++)
        r[i] = pick(i);
    r[13] = pick(200);
    for (uint8_t i = 0; i != 11; i++)
        r[14 + i] = pick_high((uint8_t)(99 + i));
    return 0;
}
)", true);
  // 12 entries for pick (0 to 11) and 9 for pick_high.
  CHECK(suite::count(text, "DC.W") == 21);
  CHECK(suite::count(text, "RTS\n.jtab@") == 2);
}
