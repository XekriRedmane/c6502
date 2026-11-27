#include <string>

#include "compile_case.h"
#include "doctest.h"
#include "sim/cpu_run.h"
#include "sim_case.h"

namespace c6502::suite {

TEST_CASE("asm_regalloc sim: a shift count survives a parameter read") {
  // `f0` calls through a pointer, so it's a soft-stack function and
  // reads `p0` from its frame. The count `l1 & 7` of the second shift
  // was kept in Y across the read of `p0` for the first shift's count,
  // whose `LDY #4` replaced it, so the result was shifted by 4 more
  // places and came out 0.
  const char* src = R"c(unsigned _BitInt(58) f3(long p0) { return 1; }
signed char f0(unsigned _BitInt(58) p0) {
    signed char l1 = p0;
    l1 ^= 5;
    unsigned _BitInt(58) (*fp)(long) = f3;
    fp(~p0);
    return ((100 << (p0 & 7)) << (l1 & 7));
}
int main(void) { return f0(236); }
)c";
  for (const bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    CHECK(build_sim(src, optimize).run().return_int_signed() == -128);
  }
}

TEST_CASE("asm_regalloc sim: an index survives the reads of spilled values") {
  // Eighteen 8-byte values live at once are more than the 128 bytes of
  // the default pool, so some are spilled to `f`'s frame. The index `c2`
  // was kept in Y across the reads of the spilled values for `gout`,
  // whose `LDY #off` replaced it, and `f` returned 4.
  std::string loads;
  std::string mix;
  for (int k = 0; k < 18; ++k) {
    const std::string v = "v" + std::to_string(k);
    loads += "    unsigned long long " + v + " = g[" + std::to_string(k) + "];\n";
    mix += (k > 0 ? " ^ " : "") + v;
  }
  const std::string src =
      "volatile unsigned long long g[18];\n"
      "volatile unsigned char gc[2];\n"
      "unsigned long long gout;\n"
      "unsigned char t[8] = {1, 2, 3, 4, 5, 6, 7, 8};\n"
      "unsigned char h(unsigned char x) { return x; }\n"
      "int f(void) {\n"
      "    unsigned char (*fp)(unsigned char) = h;\n"
      "    fp(0);\n" +
      loads +
      "    unsigned char c1 = gc[0] & 7;\n"
      "    unsigned char c2 = (gc[1] ^ 5) & 7;\n"
      "    gout = " + mix + ";\n"
      "    return t[c1] + t[c2] * 3;\n"
      "}\n"
      "int main(void) {\n"
      "    for (int k = 0; k < 18; k++)\n"
      "        g[k] = 0x0101010101010101ULL * (k + 1);\n"
      "    gc[0] = 3;\n"
      "    gc[1] = 7;\n"
      "    return f();\n"
      "}\n";
  // The CRC-32 of the Python test's program, computed on 27 Nov 2026.
  CHECK(sim::cpu_run::crc32(src) == 3645101959u);
  // Without -O the frame is over the 253-byte limit, so the TAC
  // interpreter gives the reference.
  const auto value = run_program(src);
  REQUIRE(value.has_value());
  CHECK(static_cast<long long>(*value) == 13);
  CHECK(build_sim(src, Pipeline::kOptimized).run().return_int_signed() == 13);
}

}  // namespace c6502::suite
