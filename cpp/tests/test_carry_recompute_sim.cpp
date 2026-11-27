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

TEST_CASE("carry_recompute: page steps") {
  std::string src = R"c(
        unsigned int p;
        unsigned long q;
        unsigned int r;
        void up(void) { p += 0x100; }
        void down(void) { p -= 0x100; }
        void far(void) { q += 0x20000; }
        void copy(void) { r = p + 0x300; }
        int main(void) {
            p = 0x12FF; up(); up(); down();
            q = 0x1FFFF; far();
            copy();
            p = 0x0050; down();
            return (int)(p ^ (unsigned)(q >> 16) ^ r);
        }
        )c";
  const std::int64_t p0 = (0x12FF + 0x100) & 0xFFFF;
  const std::int64_t q = (0x1FFFF + 0x20000) & 0xFFFFFFFFLL;
  const std::int64_t r = (p0 + 0x300) & 0xFFFF;
  const std::int64_t p = (0x0050 - 0x100) & 0xFFFF;
  const std::int64_t want = (p ^ (q >> 16) ^ r) & 0xFFFF;
  for (bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    CHECK(build_sim(src, optimize).run().return_int() == want);
  }
  const std::string asm_text = codegen(src, true);
  CHECK(contains(between(asm_text, "\nup:\n", "RTS"), "INC   p+1"));
  CHECK(contains(between(asm_text, "\ndown:\n", "RTS"), "DEC   p+1"));
}

}  // namespace
}  // namespace c6502::suite
