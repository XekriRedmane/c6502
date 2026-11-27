// The `-S -O` text of tests/test_constant_store_order.py's program of
// the shape of Lode Runner's `fill_hole`.
#include <algorithm>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "compile_case.h"
#include "doctest.h"

namespace {

namespace suite = c6502::suite;

// Returns `text.split(sep)[n]`, and throws when `text` has fewer
// pieces.
std::string piece(const std::string& text, const std::string& sep, std::size_t n) {
  std::size_t start = 0;
  for (std::size_t k = 0; k < n; ++k) {
    const std::size_t at = text.find(sep, start);
    if (at == std::string::npos) throw std::runtime_error("no piece " + std::to_string(n));
    start = at + sep.size();
  }
  const std::size_t end = text.find(sep, start);
  return text.substr(start, end == std::string::npos ? std::string::npos : end - start);
}

}  // namespace

TEST_CASE("constant_store_order: each table byte is loaded once and stored into the local and the global") {
  const std::string text = suite::codegen(
      R"(
#include <stdint.h>
const uint8_t ROWS[4] = {3, 0, 255, 7};
const uint8_t COLS[4] = {9, 128, 1, 0};
uint8_t TIMERS[4] = {1, 2, 1, 1};
uint8_t MAP[8][10];
uint8_t GROW, GCOL;
uint16_t sum;

static void fill(uint8_t i) {
    uint8_t row = ROWS[i];
    uint8_t col = COLS[i];
    GROW = row;
    GCOL = col;
    uint8_t r = row & 7;
    uint8_t c = col % 10;
    MAP[r][c] += 1;
    sum += (uint16_t)(row * 3) + col + MAP[r][c];
}

int main(void) {
    for (uint8_t i = 3; (int8_t)i >= 0; i--) {
        if (--TIMERS[i] != 0)
            continue;
        fill(i);
    }
    return sum + GROW + GCOL;
}
)",
      true);
  const std::vector<std::string> lines = suite::lines(piece(text, "\nmain:\n", 1));
  for (const auto& c : {std::pair<std::string, std::string>{"ROWS", "GROW"},
                        std::pair<std::string, std::string>{"COLS", "GCOL"}}) {
    const std::string& table = c.first;
    const std::string& global = c.second;
    INFO(table);
    const std::string load = "   LDA   " + table + ",X";
    CHECK(std::count(lines.begin(), lines.end(), load) == 1);
    const auto at = std::find(lines.begin(), lines.end(), load);
    REQUIRE(at != lines.end());
    REQUIRE(lines.end() - at > 2);
    CHECK(at[1].rfind("   STA   __local_main__", 0) == 0);
    CHECK(at[2] == "   STA   " + global);
  }
}
