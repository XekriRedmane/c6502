// The simulator tests of tests/test_pointer_return.py: functions that
// return a pointer in A and X, under both calling conventions and
// through a function pointer, give the values C does.
#include <cstdint>
#include <string>
#include <tuple>
#include <vector>

#include "batch1_case.h"
#include "compile_case.h"
#include "doctest.h"
#include "sim_case.h"

namespace c6502::suite {
namespace {

const std::string kProgram = R"c(
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
    )c";
TEST_CASE("pointer_return: values") {
  // *a = 30, *b = 60, *c = 80, table[1] = 21, b - a = 3.
  for (bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    auto out = build_sim(kProgram, optimize).run(5'000'000);
    CHECK_FALSE(out.timed_out);
    CHECK(out.return_int() == 30 + 60 + 80 + 21 + 3);
  }
}

}  // namespace
}  // namespace c6502::suite
