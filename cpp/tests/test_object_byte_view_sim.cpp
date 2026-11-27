// The simulator tests of tests/test_object_byte_view.py: programs that
// read and write a static object a byte at a time through a cast of its
// address, `((uint8_t *)&S)[i]`, run in both pipelines, and a byte
// written before the whole object is read in the same function reaches
// the read.
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

const std::string kPrelude = R"c(#include <stdint.h>
extern uint32_t Z [[c6502::zeropage(0x8E)]];
static uint32_t S;
#define ZB ((uint8_t *)&Z)
#define SB ((uint8_t *)&S)
)c";
// Each check sets a bit of the result when it fails, so a result
// of 0 means every check passed.
void check(const auto& body) {
  auto src = kPrelude + body;
  for (bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    auto result = build_sim(src, optimize).run(5'000'000);
    CHECK_FALSE(result.timed_out);
    CHECK(result.return_int() == 0);
  }
}

TEST_CASE("object_byte_view: reads each byte") {
  check(R"c(uint8_t zrd(uint8_t i) { return ZB[i]; }
uint8_t zrev(uint8_t i) { return ZB[3 - i]; }
uint8_t srd(uint8_t i) { return *(SB + i); }
int main(void) {
    int bad = 0;
    Z = 0x44332211UL;
    S = 0x88776655UL;
    for (uint8_t i = 0; i < 4; i++) {
        if (zrd(i) != 0x11 * (i + 1)) bad |= 1;
        if (zrev(i) != 0x11 * (4 - i)) bad |= 2;
        if (srd(i) != 0x11 * (i + 5)) bad |= 4;
    }
    if (ZB[0] != 0x11 || ZB[1] != 0x22) bad |= 8;
    if (ZB[2] != 0x33 || ZB[3] != 0x44) bad |= 16;
    if (*SB != 0x55 || *(SB + 3) != 0x88) bad |= 32;
    if (((uint16_t *)&S)[1] != 0x8877) bad |= 64;
    return bad;
}
)c");
}

TEST_CASE("object_byte_view: byte writes reach whole object") {
  check(R"c(void zwr(uint8_t i, uint8_t v) { ZB[i] = v; }
void swr(uint8_t i, uint8_t v) { *(SB + i) = v; }
int main(void) {
    int bad = 0;
    Z = 0;
    S = 0;
    for (uint8_t i = 0; i < 4; i++) {
        zwr(i, 0x10 + i);
        swr(3 - i, 0xA0 + i);
    }
    if (Z != 0x13121110UL) bad |= 1;
    if (S != 0xA0A1A2A3UL) bad |= 2;
    ZB[1] = 0x55;
    *(SB + 2) = 0x66;
    if (Z != 0x13125510UL) bad |= 4;
    if (S != 0xA066A2A3UL) bad |= 8;
    ((uint16_t *)&Z)[1] = 0xBEEF;
    if (Z != 0xBEEF5510UL) bad |= 16;
    return bad;
}
)c");
}

TEST_CASE("object_byte_view: write after whole store in one function") {
  // The constant stored to the object must not reach the read
  // after the byte write.
  check(R"c(uint32_t g(uint8_t i, uint8_t v) {
    S = 0x01020304UL;
    SB[i] = v;
    return S;
}
uint32_t h(uint8_t v) {
    Z = 0x01020304UL;
    ZB[2] = v;
    return Z;
}
uint8_t k(uint8_t i) {
    uint8_t before = SB[i];
    S = 0;
    return before + SB[i];
}
int main(void) {
    int bad = 0;
    if (g(1, 0xEE) != 0x0102EE04UL) bad |= 1;
    if (S != 0x0102EE04UL) bad |= 2;
    if (h(0x77) != 0x01770304UL) bad |= 4;
    S = 0x00000500UL;
    if (k(1) != 5) bad |= 8;
    return bad;
}
)c");
}

TEST_CASE("object_byte_view: byte write from a register parameter") {
  // `v` arrives in A, and backward copy propagation moved its
  // store to `ZB[2]` up to the entry, in front of the stores of
  // `Z`, since it took `Pseudo(Z, 2)` and `Data(Z, 2)` for
  // different bytes.
  check(R"c(uint32_t h(register uint8_t v) {
    Z = 0x01020304UL;
    ZB[2] = v;
    return Z;
}
int main(void) { return h(0x77) != 0x01770304UL; }
)c");
}

TEST_CASE("object_byte_view: compound assignment and increment") {
  check(R"c(uint8_t m;
void orb(uint8_t i) { ZB[i] |= m; }
void inc(uint8_t i) { ++*(SB + i); }
void add(uint8_t i) { SB[i] += 0x10; }
int main(void) {
    int bad = 0;
    Z = 0x01010101UL;
    S = 0x000000FFUL;
    m = 0x80;
    orb(0);
    orb(3);
    if (Z != 0x81010181UL) bad |= 1;
    inc(0);
    inc(2);
    if (S != 0x00010000UL) bad |= 2;
    add(2);
    if (S != 0x00110000UL) bad |= 4;
    ZB[1] ^= 0x03;
    (*(ZB + 2))--;
    if (Z != 0x81000281UL) bad |= 8;
    return bad;
}
)c");
}

}  // namespace
}  // namespace c6502::suite
