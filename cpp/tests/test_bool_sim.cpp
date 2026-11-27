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

const std::vector<std::tuple<std::string, std::string, int>> kPrograms = {
    {"int_conversion", "int main(void) { int x = 512; bool b = x; return b; }", 1},
    {"long_conversion", "int main(void) { long x = 0x10000L; bool b = x; long z = 0; bool c = z; return b * 2 + c; }",
     2},
    {"char_conversion",
     "int main(void) { unsigned char x = 0; signed char y = -1; bool a = x, b = y; return a * 2 + b; }", 1},
    {"pointer_conversion", "int g; int main(void) { int *p = &g, *q = 0; bool a = p, b = q; return a * 2 + b; }", 2},
    {"floating_conversion",
     "double d = -0.0; double e = 1e-30; float f = 0.0f;int main(void) { bool a = d, b = e, c = f; return a * 4 + b * "
     "2 + c; }",
     2},
    {"explicit_cast", "int main(void) { int x = 2; return (bool)x + (bool)0 + (bool)3; }", 2},
    {"parameter_and_return", "bool id(bool b) { return b; }int main(void) { return id(256) + id(0) * 2 + id(7) * 4; }",
     5},
    {"increment_saturates", "int main(void) { bool b = 0; b++; ++b; int old = b++; return b * 2 + old; }", 3},
    {"decrement_toggles",
     "int main(void) { bool b = 1; int r = 0; b--; r += b; b--; r += b * 2; r += (--b) * 4; return r; }", 2},
    {"compound_assignment",
     "int main(void) { bool b = 0; b += 2; int r = b; b -= 1; r += b * 2; b |= 4; r += b * 4; b *= 0; r += b * 8; "
     "return r; }",
     5},
    {"through_pointer_and_array",
     "bool flags[3];void set(bool *p, int v) { *p = v; }int main(void) { set(&flags[0], 10); set(&flags[1], 0); "
     "flags[2] = 300; flags[2]++; return flags[0] + flags[1] * 2 + flags[2] * 4; }",
     5},
    {"struct_member",
     "struct s { int v; bool ok; };int main(void) { struct s x; x.ok = 0x100; x.v = 3; struct s *p = &x; p->ok += 1; "
     "return x.v * 10 + p->ok; }",
     31},
    {"arithmetic_promotes", "int main(void) { bool a = 1, b = 1; return a + b + (a << 3); }", 10},
    {"condition", "int main(void) { bool b = 5; int r = 0; if (b) r = 1; while (b) { r += 2; b = 0; } return r; }", 3},
    {"stdbool", R"c(#include <stdbool.h>
bool odd(int x) { return x & 1; }int main(void) { bool t = true, f = false; return t + f * 2 + odd(3) * 4 + odd(4) * 8; })c",
     5},
    {"carry_return", R"c(#include <stdbool.h>
[[c6502::return_bool_in_carry]] bool nz(int x) { return x; }int main(void) { int n = 0; if (nz(512)) n++; if (nz(0)) n += 2; bool b = nz(-1); return n + b * 4; })c",
     5},
    {"static_initializers",
     "int g; bool a = 256, b = &g, c = 0.0, d = -0.0, e = 0.5;int main(void) { static bool s = 7; return a + b * 2 + c "
     "* 4 + d * 8 + e * 16 + s * 32; }",
     51}};
TEST_CASE("bool: asm sim") {
  for (const auto& [name, src, expected] : kPrograms) {
    for (bool optimize : {false, true}) {
      INFO("case=" << name << " optimize=" << optimize);
      auto result = build_sim(src, optimize).run(2'000'000);
      CHECK_FALSE(result.timed_out);
      CHECK(result.return_int_signed() == expected);
    }
  }
}

}  // namespace
}  // namespace c6502::suite
