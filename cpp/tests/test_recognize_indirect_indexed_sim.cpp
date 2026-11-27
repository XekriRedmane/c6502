// The simulator tests of tests/test_recognize_indirect_indexed.py: loads
// and stores through a pointer and a byte index, which the TAC pass
// `recognize_indirect_indexed` turns into `(zp),Y` accesses, read and
// write the right bytes in both pipelines.
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

namespace recognize_indirect_indexed_const_offset {
// `Load(ptr + k)` / `Store(v, ptr + k)` with a constant byte
// offset `0 <= k <= 255` folds to an indirect-indexed node whose
// index is `ConstUChar(k)`, lowered as `LDY #k`.
TEST_CASE("recognize_indirect_indexed: offset read and write end to end") {
  std::string src = R"c(#include <stdint.h>
static uint8_t data[4] = {1, 2, 3, 4};
uint8_t *gp;
void f(uint8_t *p) { p[1]++; p[2] += 3; gp[3]--; }
int main(void) {
    gp = data;
    f(data);
    return data[0] + 10 * data[1] + 100 * data[2]
        + 1000 * data[3];
}
)c";
  for (bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    auto out = build_sim(src, optimize).run(5'000'000);
    CHECK_FALSE(out.timed_out);
    CHECK(out.return_int() == 1 + 30 + 600 + 3000);
  }
}

TEST_CASE("recognize_indirect_indexed: offset access end to end") {
  // Loads at offsets 0 and 255, and a store of a negative
  // `signed char` constant, which must be stored as its 2's
  // complement byte.
  std::string src = R"c(#include <stdint.h>
static uint8_t data[256];
static volatile uint8_t a, b;
void f(uint8_t *p, int8_t *q) {
    a = p[0];
    b = p[255];
    q[7] = -122;
}
int main(void) {
    data[0] = 0x11; data[255] = 0x99;
    f(data, (int8_t *)data);
    return 0;
}
)c";
  for (bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    auto s = build_sim(src, optimize);
    auto out = s.run(5'000'000);
    CHECK_FALSE(out.timed_out);
    CHECK(out.memory[s.symbol("a")] == 0x11);
    CHECK(out.memory[s.symbol("b")] == 0x99);
    CHECK(out.memory[s.symbol("data") + 7] == 0x100 - 122);
  }
}

TEST_CASE("recognize_indirect_indexed: wide member end to end") {
  // A 2-byte member read, written, and updated in place through a
  // pointer, and through a pointer the function computes, which
  // stays in DPTR for the last byte of the store while the reads
  // go through the computed pointer's own bytes. asm_remat had
  // taken IndirectZp's offset as part of the pointer's address,
  // and dropped the store of that pointer's high byte.
  std::string src = R"c(#include <stdint.h>
struct rec { uint8_t a[13]; uint16_t gold; uint8_t b; };
static struct rec r[2];
struct rec *P;
static uint8_t less(uint16_t price) { return P->gold < price; }
static void clear(void) { P->gold = 0; }
static void add(struct rec *q) { q->gold += 5; }
static int bump(int *ptr) {
    if ((--ptr)[3] += 4) return 4;
    return 0;
}
int main(void) {
    static int arr[4] = {-1, -2, -3, -4};
    P = &r[1];
    P->gold = 1000;
    if (!less(1001) || less(1000)) return 1;
    add(P);
    if (r[1].gold != 1005) return 2;
    clear();
    if (r[1].gold != 0 || r[1].b != 0) return 3;
    if (bump(arr + 1) != 0 || arr[3] != 0) return 4;
    return 0;
}
)c";
  for (bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    auto out = build_sim(src, optimize).run(5'000'000);
    CHECK_FALSE(out.timed_out);
    CHECK(out.return_int() == 0);
  }
}

}  // namespace recognize_indirect_indexed_const_offset

namespace byte_index_sum {
// `p[i + k]` with a byte `i` whose largest value plus `k` fits in
// a byte is `(p),Y` with Y = `i + k`, so a word table read through a
// pointer at an even index, `p[i]` and `p[i + 1]` with `i = j << 1`,
// is `LDY i; LDA (p),Y; INY; LDA (p),Y`.
const std::string kSrc = R"c(#include <stdint.h>
static uint8_t data[300];
uint8_t *table;
uint16_t word(uint8_t j) {
    uint8_t i = j << 1;
    return ((uint16_t)table[i] << 8) | table[i + 1];
}
uint16_t odd(uint8_t i) {
    return ((uint16_t)table[i] << 8) | table[i + 1];
}
int main(void) {
    table = data;
    data[254] = 0x12; data[255] = 0x34; data[256] = 0x56;
    return (word(127) == 0x1234) + 2 * (odd(255) == 0x3456);
}
)c";
TEST_CASE("recognize_indirect_indexed: byte index sum: runs") {
  // `odd(255)` reads `table + 256`, past the byte Y can hold, so
  // it keeps its 16-bit add.
  for (bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    auto out = build_sim(kSrc, optimize).run(5'000'000);
    CHECK_FALSE(out.timed_out);
    CHECK(out.return_int() == 3);
  }
}

}  // namespace byte_index_sum

namespace shared_byte_index {
// `p + ZeroExtend(i)` read by several 1-byte loads and stores, as
// in `p[i] &= m`, is `(p),Y` with Y = `i` in each of them, and a
// reader `(p + i) + k` with `k` outside 0-255 is `(t),Y` with `t =
// p + k`.
const std::string kSrc = R"c(#include <stdint.h>
static uint8_t screen[0x2100];
static const uint8_t mask[2] = {0xF0, 0x0F};
uint8_t *row;
void close(uint8_t *line, uint8_t x, uint8_t m) {
    line[x] &= mask[m];
}
void open(uint8_t *line, uint8_t x, uint8_t m) {
    const uint8_t *background = line + 0x2000;
    line[x] |= background[x] & mask[m];
}
void open_global(uint8_t x) {
    uint8_t v = row[x];
    row = screen;
    row[x] = v | 1;
}
int main(void) {
    for (uint16_t k = 0; k < 0x100; k++) {
        screen[k] = (uint8_t)k;
        screen[k + 0x2000] = 0xFF;
    }
    close(screen + 0x10, 0xF5, 0);
    open(screen + 0x10, 0x05, 1);
    row = screen + 0x80;
    open_global(0x20);
    return screen[0x105] + screen[0x15] * 256 + screen[0x20];
}
)c";
TEST_CASE("recognize_indirect_indexed: shared byte index: runs") {
  // close: 0x05 & 0xF0 = 0x00 at 0x105. open: 0x15 | (0xFF &
  // 0x0F) = 0x1F at 0x15. open_global reads row[0x20] through
  // the old row, 0xA0, and writes it with bit 0 set through the
  // new one, at 0x20.
  for (bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    auto out = build_sim(kSrc, optimize).run(20'000'000);
    CHECK_FALSE(out.timed_out);
    CHECK(out.return_int() == 0x1F00 + 0xA1);
  }
}

}  // namespace shared_byte_index

namespace indirect_indexed_correctness {
// End-to-end: the optimized program reads/writes the right
// addresses via the indirect-indexed path.
TEST_CASE("recognize_indirect_indexed: load reads correct byte") {
  // Set up an 8-byte source array; have a zp_abi function
  // read byte 5 of it into a global. The optimized indirect-
  // indexed path should produce the same answer as the
  // unoptimized DPTR path.
  std::string src = R"c(#include <stdint.h>
static uint8_t data[8] = {0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80};
static volatile uint8_t result;
[[c6502::zp_abi]]
void load(uint8_t *p, uint8_t i) {
    result = p[i];
}
int main(void) { load(data, 5); return 0; }
)c";
  auto s = build_sim(src, Pipeline::kOptimized);
  auto out = s.run(5'000'000);
  CHECK_FALSE(out.timed_out);
  auto result_addr = s.symbol("result");
  CHECK(out.memory[result_addr] == 0x60);
}

TEST_CASE("recognize_indirect_indexed: store writes correct byte") {
  std::string src = R"c(#include <stdint.h>
static uint8_t dest[8];
[[c6502::zp_abi]]
void store(uint8_t *p, uint8_t i, uint8_t v) {
    p[i] = v;
}
int main(void) { store(dest, 3, 0xAB); return 0; }
)c";
  auto s = build_sim(src, Pipeline::kOptimized);
  auto out = s.run(5'000'000);
  CHECK_FALSE(out.timed_out);
  auto dest_addr = s.symbol("dest");
  CHECK(out.memory[dest_addr + 3] == 0xAB);
}

TEST_CASE("recognize_indirect_indexed: unoptimized still works") {
  // The pass only fires under -O. Verify both modes
  // produce the same result.
  std::string src = R"c(#include <stdint.h>
static uint8_t data[8] = {1, 2, 3, 4, 5, 6, 7, 8};
static volatile uint8_t result;
[[c6502::zp_abi]]
void load(uint8_t *p, uint8_t i) {
    result = p[i];
}
int main(void) { load(data, 4); return 0; }
)c";
  for (bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    auto s = build_sim(src, optimize);
    auto out = s.run(5'000'000);
    CHECK_FALSE(out.timed_out);
    auto result_addr = s.symbol("result");
    CHECK(out.memory[result_addr] == 5);
  }
}

TEST_CASE("recognize_indirect_indexed: negative signed char index") {
  // `p[i]` with `int8_t i = -1` addresses p - 1. The fold
  // accepted the SignExtend of i, and `(p),Y` read p + 255.
  std::string src = R"c(#include <stdint.h>
uint8_t data[8] = {10, 11, 12, 13, 14, 15, 16, 17};
uint8_t ld(const uint8_t *p, int8_t i) { return p[i]; }
void st(uint8_t *p, int8_t i, uint8_t v) { p[i] = v; }
int main(void) {
    uint8_t r = ld(data + 4, -1);
    st(data + 4, -2, 99);
    return r + (data[2] == 99) * 100;
}
)c";
  for (bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    auto out = build_sim(src, optimize).run(5'000'000);
    CHECK(out.return_int() == 113);
  }
}

}  // namespace indirect_indexed_correctness

}  // namespace
}  // namespace c6502::suite
