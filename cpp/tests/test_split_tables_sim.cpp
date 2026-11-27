// The simulator tests of tests/test_split_tables.py: programs whose
// tables of 2-byte or wider elements src/passes/optimization/split_tables.h
// stores as low bytes and then high bytes give the values C does in both
// pipelines.
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

std::string codegen_o(const std::string& src) { return codegen(src, Pipeline::kOptimized); }

void check(const auto& src, const auto& expected) {
  for (bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    auto got = build_sim(src, optimize).run().return_int();
    CHECK(got == (expected & 0xFFFF));
  }
}

TEST_CASE("split_tables: uint16 table") {
  std::string src = R"c(
        #include <stdint.h>
        static const uint16_t T[8] = {
            0x0102, 0x0304, 0x1000, 0x2000, 0x1234, 0xFF00, 7, 0x8001,
        };
        uint16_t get(uint8_t i) { return T[i]; }
        uint16_t next(uint8_t i) { return T[i + 1]; }
        int main(void) {
            uint16_t s = T[4];
            for (uint8_t i = 0; i < 8; i++) s += get(i);
            return s + next(6) + next(0);
        }
        )c";
  std::vector<int> values = {0x0102, 0x0304, 0x1000, 0x2000, 0x1234, 0xFF00, 7, 0x8001};
  int total = 0;
  for (int v : values) total += v;
  check(src, 0x1234 + total + 0x8001 + 0x0304);
  auto body = between(codegen_o(src), "\nget:\n", "RTS");
  CHECK(contains(body, "T+8,X"));
  CHECK(!contains(body, "ASL"));
}

TEST_CASE("split_tables: long table") {
  // 200 elements, 400 bytes: the 16-bit address path.
  std::string src = R"c(
        #include <stdint.h>
        static const uint16_t T[200] = {11, 0x1234, 0xBEEF};
        uint16_t get(uint8_t i) { return T[i]; }
        int main(void) { return get(0) + get(1) + get(2) + get(199); }
        )c";
  check(src, 11 + 0x1234 + 0xBEEF);
  auto body = between(codegen_o(src), "\nget:\n", "RTS");
  CHECK(contains(body, "T+200,X"));
}

TEST_CASE("split_tables: pointer table") {
  // Nine pointers, one more than the dispatch on a small
  // pointer table takes.
  std::string src = R"c(
        #include <stdint.h>
        static const uint8_t a[2] = {1, 2}, b[2] = {3, 4}, c[2] = {5, 6};
        static const uint8_t *const P[9] = {a, b, c, a + 1, b + 1, c + 1,
                                            a, b, c};
        uint8_t get(uint8_t i, uint8_t j) { return P[i][j]; }
        int main(void) {
            uint8_t s = 0;
            for (uint8_t i = 0; i < 9; i++) s += get(i, 0) * (i + 1);
            return s + get(2, 1);
        }
        )c";
  std::vector<int> firsts = {1, 3, 5, 2, 4, 6, 1, 3, 5};
  int total = 0;
  for (std::size_t k = 0; k < firsts.size(); ++k) total += firsts[k] * static_cast<int>(k + 1);
  check(src, total + 6);
  CHECK(contains(codegen_o(src), "DC.B  <(a+1)"));
}

TEST_CASE("split_tables: uint32 table") {
  std::string src = R"c(
        #include <stdint.h>
        static const uint32_t T[3] = {0x01020304, 0x0A0B0C0D, 0xFFFFFFFF};
        uint32_t get(uint8_t i) { return T[i]; }
        int main(void) {
            return (int)(get(1) >> 16) + (int)(get(0) & 0xFFFF)
                + (int)(get(2) >> 24);
        }
        )c";
  check(src, 0x0A0B + 0x0304 + 0xFF);
  CHECK(contains(codegen_o(src), "T+6,X"));
}

}  // namespace
}  // namespace c6502::suite
