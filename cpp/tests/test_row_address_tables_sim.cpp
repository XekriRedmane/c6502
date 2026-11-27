// The simulator tests of tests/test_row_address_tables.py: programs whose
// 2-D arrays src/passes/optimization/row_address_tables.h reads through
// a table of row addresses give the values computed here in both
// pipelines, and units that share the low bytes of their tables keep one
// copy.
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

auto cell(const auto& r, const auto& c) {
  return (r * 16 + c) & 0xFF;
}

// Each program reads its array often enough that the accesses save
// more than the tables cost, so that `-O` makes them.
void check(const auto& src, const auto& expected, const auto& table) {
  CHECK(contains(codegen_o(src), cat("\n", table, "@lo:\n")));
  for (bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    auto got = build_sim(src, optimize).run().return_int();
    CHECK(got == (expected & 0xFFFF));
  }
}

const std::string kFill = R"c(
        for (uint8_t r = 0; r < ROWS; r++)
            for (uint8_t c = 0; c < COLS; c++)
                A[r][c] = (uint8_t)(r * 16 + c);
    )c";
TEST_CASE("row_address_tables: fixed address array") {
  auto src = R"c(
        #include <stdint.h>
        #define ROWS 16
        #define COLS 28
        extern uint8_t A [[c6502::address(0x6000)]][ROWS][COLS];
        uint8_t get(uint8_t r, uint8_t c) { return A[r][c]; }
        uint8_t above(uint8_t r, uint8_t c) { return A[r - 1][c]; }
        uint8_t below(uint8_t r, uint8_t c) { return A[r + 1][c]; }
        void put(uint8_t r, uint8_t c, uint8_t v) { A[r][c] = v; }
        uint8_t *row(uint8_t r) { return A[r]; }
        uint8_t *cell(uint8_t r, uint8_t c) { return &A[r][c]; }
        uint8_t (*whole(uint8_t r))[COLS] { return &A[r]; }
        int main(void) {
            )c" +
             kFill + R"c(
            put(5, 6, 200);
            int s = get(5, 6) + get(15, 27) + above(1, 3) + below(14, 0);
            s += row(2)[4] + *cell(3, 7) + (*whole(4))[1];
            // One past the last row.
            if ((uint8_t *)whole(ROWS) != &A[0][0] + ROWS * COLS) return -1;
            if (row(ROWS) != &A[0][0] + ROWS * COLS) return -2;
            return s;
        }
        )c";
  check(src, 200 + cell(15, 27) + cell(0, 3) + cell(15, 0) + cell(2, 4) + cell(3, 7) + cell(4, 1), "A");
}

TEST_CASE("row_address_tables: static array") {
  auto src = R"c(
        #include <stdint.h>
        #define ROWS 20
        #define COLS 14
        static uint8_t A[ROWS][COLS];
        uint8_t get(uint8_t r, uint8_t c) { return A[r][c]; }
        uint8_t above(uint8_t r, uint8_t c) { return A[r - 1][c]; }
        uint8_t *end(uint8_t r) { return A[r]; }
        int main(void) {
            )c" +
             kFill + R"c(
            if (end(ROWS) != &A[0][0] + ROWS * COLS) return -1;
            return get(19, 13) + get(0, 0) + above(1, 2) + get(7, 1);
        }
        )c";
  check(src, cell(19, 13) + cell(0, 0) + cell(0, 2) + cell(7, 1), "A");
}

TEST_CASE("row_address_tables: two byte elements") {
  // Rows of 7 uint16_t, 14 bytes.
  std::string src = R"c(
        #include <stdint.h>
        static uint16_t W[30][7];
        uint16_t get(uint8_t r, uint8_t c) { return W[r][c]; }
        void put(uint8_t r, uint8_t c, uint16_t v) { W[r][c] = v; }
        int main(void) {
            for (uint8_t r = 0; r < 30; r++)
                for (uint8_t c = 0; c < 7; c++)
                    put(r, c, (uint16_t)(r * 1000 + c));
            return get(29, 6) - get(3, 2) + get(0, 1);
        }
        )c";
  check(src, 29 * 1000 + 6 - (3 * 1000 + 2) + 1, "W");
}

TEST_CASE("row_address_tables: array of structs") {
  std::string src = R"c(
        #include <stdint.h>
        struct cell { uint8_t a, b, c; };
        static struct cell S[90];
        uint8_t a_of(uint8_t i) { return S[i].a; }
        uint8_t b_of(uint8_t i) { return S[i].b; }
        uint8_t c_of(uint8_t i) { return S[i].c; }
        void set_a(uint8_t i, uint8_t v) { S[i].a = v; }
        void set_b(uint8_t i, uint8_t v) { S[i].b = v; }
        void set_c(uint8_t i, uint8_t v) { S[i].c = v; }
        int main(void) {
            for (uint8_t i = 0; i < 90; i++) {
                set_a(i, i); set_b(i, i + 1); set_c(i, i + 2);
            }
            return a_of(89) + b_of(0) + c_of(45);
        }
        )c";
  check(src, 89 + 1 + 47, "S");
}

TEST_CASE("row_address_tables: shared low bytes") {
  std::string src = R"c(
        #include <stdint.h>
        extern uint8_t M [[c6502::address(0x6000)]][16][28];
        extern uint8_t B [[c6502::address(0x6200)]][16][28];
        uint8_t both(uint8_t r, uint8_t c) { return M[r][c] + B[r][c]; }
        int main(void) {
            M[3][4] = 10; B[3][4] = 20; M[15][27] = 1; B[15][27] = 2;
            return both(3, 4) * 10 + both(15, 27);
        }
        )c";
  check(src, 300 + 3, "B");
  auto text = codegen_o(src);
  CHECK(!contains(text, "M@lo"));
  auto tables = second_field(text, "\nB@lo:\n");
  CHECK(contains(tables, R"c(
B@hi:
)c"));
  CHECK(contains(tables, R"c(
M@hi:
)c"));
  CHECK(count(tables, "$") == 51);
}

}  // namespace
}  // namespace c6502::suite
