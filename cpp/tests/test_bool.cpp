// The tests of tests/test_bool.py that read the `-S` text or run the
// TAC interpreter. The simulator's runs are in test_bool_sim.cpp.
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "compile_case.h"
#include "doctest.h"

namespace {

namespace suite = c6502::suite;

struct Program {
  const char* name;
  std::string source;
  long long expected;
};

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

std::vector<std::string> words(const std::string& text) {
  std::istringstream in(text);
  std::vector<std::string> out;
  for (std::string w; in >> w;) out.push_back(w);
  return out;
}

// Returns the words of function `name` up to its first `RTS`.
std::vector<std::string> function_words(const std::string& text, const std::string& name) {
  return words(piece(piece(text, "\n" + name + ":", 1), "RTS", 0));
}

// Returns the low 16 bits of `main`'s value on the TAC, as an `int`.
long long tac_int(const std::string& source) {
  const auto value = suite::run_program(source);
  REQUIRE(value.has_value());
  long long v = static_cast<long long>(*value & 0xFFFF);
  if (v & 0x8000) v -= 0x10000;
  return v;
}

}  // namespace

TEST_CASE("bool: a conversion to bool is a test against zero") {
  // Static, so that they return in A: main keeps their results as
  // values (cpp/src/passes/carry_return_choice.h). `x` arrives in A
  // (and X), so f tests A with `ORA #$00`, and g stores the low byte
  // and ORs the high byte into it.
  suite::Options options;
  options.optimize = true;
  options.inline_calls = false;
  const std::string text = suite::codegen(
      "static bool f(unsigned char x) { return x; }"
      "static bool g(int x) { return x; }"
      "int main(void) { return f(3) + g(256); }",
      options);
  CHECK(function_words(text, "f") ==
        std::vector<std::string>{"SUBROUTINE", "ORA", "#$00", "BEQ", ".cmp_end@0", "LDA", "#$01",
                                 ".cmp_end@0:"});
  CHECK(function_words(text, "g") ==
        std::vector<std::string>{"SUBROUTINE", "STA", "__local_g__x_0", "TXA", "ORA",
                                 "__local_g__x_0", "BEQ", ".cmp_end@1", "LDA", "#$01",
                                 ".cmp_end@1:"});
}

TEST_CASE("bool: the programs return their values on the TAC interpreter") {
  const std::vector<Program> programs = {
      {"int_conversion", R"(int main(void) { int x = 512; bool b = x; return b; })", 1},
      {"long_conversion", R"(int main(void) { long x = 0x10000L; bool b = x; long z = 0; bool c = z; return b * 2 + c; })", 2},
      {"char_conversion", R"(int main(void) { unsigned char x = 0; signed char y = -1; bool a = x, b = y; return a * 2 + b; })", 1},
      {"pointer_conversion", R"(int g; int main(void) { int *p = &g, *q = 0; bool a = p, b = q; return a * 2 + b; })", 2},
      {"floating_conversion", R"(double d = -0.0; double e = 1e-30; float f = 0.0f;int main(void) { bool a = d, b = e, c = f; return a * 4 + b * 2 + c; })", 2},
      {"explicit_cast", R"(int main(void) { int x = 2; return (bool)x + (bool)0 + (bool)3; })", 2},
      {"parameter_and_return", R"(bool id(bool b) { return b; }int main(void) { return id(256) + id(0) * 2 + id(7) * 4; })", 5},
      {"increment_saturates", R"(int main(void) { bool b = 0; b++; ++b; int old = b++; return b * 2 + old; })", 3},
      {"decrement_toggles", R"(int main(void) { bool b = 1; int r = 0; b--; r += b; b--; r += b * 2; r += (--b) * 4; return r; })", 2},
      {"compound_assignment", R"(int main(void) { bool b = 0; b += 2; int r = b; b -= 1; r += b * 2; b |= 4; r += b * 4; b *= 0; r += b * 8; return r; })", 5},
      {"through_pointer_and_array", R"(bool flags[3];void set(bool *p, int v) { *p = v; }int main(void) { set(&flags[0], 10); set(&flags[1], 0); flags[2] = 300; flags[2]++; return flags[0] + flags[1] * 2 + flags[2] * 4; })", 5},
      {"struct_member", R"(struct s { int v; bool ok; };int main(void) { struct s x; x.ok = 0x100; x.v = 3; struct s *p = &x; p->ok += 1; return x.v * 10 + p->ok; })", 31},
      {"arithmetic_promotes", R"(int main(void) { bool a = 1, b = 1; return a + b + (a << 3); })", 10},
      {"condition", R"(int main(void) { bool b = 5; int r = 0; if (b) r = 1; while (b) { r += 2; b = 0; } return r; })", 3},
      {"stdbool", R"(#include <stdbool.h>
bool odd(int x) { return x & 1; }int main(void) { bool t = true, f = false; return t + f * 2 + odd(3) * 4 + odd(4) * 8; })", 5},
      {"carry_return", R"(#include <stdbool.h>
[[c6502::return_bool_in_carry]] bool nz(int x) { return x; }int main(void) { int n = 0; if (nz(512)) n++; if (nz(0)) n += 2; bool b = nz(-1); return n + b * 4; })", 5},
      {"static_initializers", R"(int g; bool a = 256, b = &g, c = 0.0, d = -0.0, e = 0.5;int main(void) { static bool s = 7; return a + b * 2 + c * 4 + d * 8 + e * 16 + s * 32; })", 51},
  };
  for (const Program& p : programs) {
    INFO(p.name);
    CHECK(tac_int(p.source) == p.expected);
  }
}
