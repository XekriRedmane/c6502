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

std::int64_t run_int(const std::string& src, bool opt = false, bool is_signed = true) {
  const sim::SimResult res = opt ? build_sim(src, true).run() : run_c_program(src);
  return is_signed ? res.return_int_signed() : res.return_int();
}

std::int64_t run_long(const std::string& src, bool opt = false, bool is_signed = true) {
  const sim::SimResult res = opt ? build_sim(src, true).run() : run_c_program(src);
  return is_signed ? res.return_long_signed() : static_cast<std::int64_t>(res.return_long());
}

TEST_CASE("inline_shift_direct: uint16 lsl inplace") {
  // `x <<= 1` on a 2-byte unsigned. Compound assignment makes
  // x both src and dst at the TAC level; after SSA regalloc
  // they typically share a ZP slot, and the inline shift's
  // leading Mov drops via the self-Mov peephole, leaving a
  // bare `ASL $XX; ROL $XX` pair.
  std::string src = R"c(unsigned int f(unsigned int x) { x <<= 1; return x; }
unsigned int main(void) { return f(0x1234u); }
)c";
  CHECK(run_int(src, false, false) == 0x2468);
  CHECK(run_int(src, true, false) == 0x2468);
}

TEST_CASE("inline_shift_direct: int16 lsr signed") {
  std::string src = R"c(int f(int x) { x >>= 1; return x; }
int main(void) { return f(-5); }
)c";
  // -5 >> 1 = -3 (arithmetic right shift).
  CHECK(run_int(src, false) == -3);
  CHECK(run_int(src, true) == -3);
}

TEST_CASE("inline_shift_direct: uint16 lsr unsigned") {
  std::string src = R"c(unsigned int f(unsigned int x) { x >>= 1; return x; }
unsigned int main(void) { return f(0xFFFEu); }
)c";
  CHECK(run_int(src, false, false) == 0x7FFF);
  CHECK(run_int(src, true, false) == 0x7FFF);
}

TEST_CASE("inline_shift_direct: uint32 lsl") {
  // 4-byte left shift exercises ASL b0; ROL b1; ROL b2; ROL b3.
  std::string src = R"c(unsigned long f(unsigned long x) { x <<= 1; return x; }
unsigned long main(void) { return f(0x80000001UL); }
)c";
  // 0x80000001 << 1 = 0x100000002 → 0x00000002 (32-bit wrap).
  CHECK(run_long(src, false, false) == 0x2);
  CHECK(run_long(src, true, false) == 0x2);
}

TEST_CASE("inline_shift_direct: int32 lsr signed") {
  // 4-byte signed right shift: capture sign into carry, then
  // ROR through all 4 bytes high-to-low.
  std::string src = R"c(long f(long x) { x >>= 1; return x; }
long main(void) { return f(-100000L); }
)c";
  // -100000 >> 1 = -50000.
  CHECK(run_long(src, false) == -50000);
  CHECK(run_long(src, true) == -50000);
}

TEST_CASE("inline_shift_direct: strength reduced multiply") {
  // `x * 2` after strength reduction → `x << 1`, lowered via
  // the inline direct-shift pattern.
  std::string src = R"c(unsigned int f(unsigned int x) { return x * 2; }
unsigned int main(void) { return f(0x4321u); }
)c";
  CHECK(run_int(src, false, false) == 0x8642);
  CHECK(run_int(src, true, false) == 0x8642);
}

}  // namespace
}  // namespace c6502::suite
