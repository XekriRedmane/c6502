// A function returning a pointer returns it in A and X, from
// tests/test_pointer_return.py. The simulator's runs are in
// test_pointer_return_sim.cpp.
#include <stdexcept>
#include <string>

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

TEST_CASE("pointer_return: a returned pointer doesn't go through HARGS") {
  const std::string text =
      suite::codegen(R"(
    #include <stdint.h>
    uint8_t table[8] = {10, 20, 30, 40, 50, 60, 70, 80};
    uint8_t *gp;
    /* zp_abi under -O. */
    static uint8_t *entry(uint8_t i) { return table + i; }
    /* A soft-stack function in both pipelines, since it recurses. */
    static uint8_t *walk(uint8_t *p, uint8_t n) {
        if (n == 0) return p;
        return walk(p + 1, n - 1);
    }
    /* Called through a pointer. */
    static uint8_t *last(void) { return &table[7]; }
    static uint8_t *(*pick)(void) = last;
    int main(void) {
        uint8_t *a = entry(2);
        uint8_t *b = walk(table, 5);
        uint8_t *c = pick();
        gp = entry(1);
        *gp += 1;
        return *a + *b + *c + table[1] + (int)(b - a);
    }
    )", true);
  CHECK_FALSE(suite::contains(piece(text, "@zp-link-meta-end", 1), "HARGS+1"));
}
