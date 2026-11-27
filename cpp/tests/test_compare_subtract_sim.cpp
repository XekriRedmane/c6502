// The calls make each compare go both ways, and some signed operands'
// difference overflows on the side of the branch that doesn't use it.
#include <cctype>
#include <string>
#include <vector>

#include "compile_case.h"
#include "doctest.h"
#include "sim_case.h"

namespace c6502::suite {

namespace {

struct Call {
  const char* call;
  std::int64_t expected;
};

struct Case {
  const char* fn;
  std::vector<Call> calls;
};

std::string between(const std::string& text, const std::string& after, const std::string& before) {
  const std::size_t start = text.find(after);
  REQUIRE(start != std::string::npos);
  const std::size_t from = start + after.size();
  const std::size_t end = text.find(before, from);
  REQUIRE(end != std::string::npos);
  return text.substr(from, end - from);
}

}  // namespace

TEST_CASE("compare_subtract sim: each function returns what it should") {
  const std::vector<Case> cases = {
      {R"c(
     unsigned int f(unsigned int s, unsigned int k) {
         while (s >= k) s -= k;
         return s;
     })c",
       {{"f(1000, 16)", 8},
        {"f(65535, 1000)", 535},
        {"f(3, 7)", 3},
        {"f(7, 7)", 0}}},
      {R"c(
     int f(int s, int k) {
         while (s >= k) s -= k;
         return s;
     })c",
       {{"f(1000, 16)", 8},
        {"f(-5, 3)", -5},
        {"f(32767, 30000)", 2767},
        {"f(100, 1)", 0}}},
      {R"c(
     unsigned char f(unsigned char s, unsigned char k) {
         while (s >= k) s -= k;
         return s;
     })c",
       {{"f(250, 7)", 5},
        {"f(255, 255)", 0},
        {"f(0, 1)", 0}}},
      {R"c(
     int f(int s, int k) {
         if (s >= k) return s - k;
         return 0;
     })c",
       {{"f(-30000, 30000)", 0},
        {"f(100, -100)", 200},
        {"f(5, 3)", 2},
        {"f(-100, 100)", 0},
        {"f(3, 3)", 0}}},
      {R"c(
     signed char f(signed char s, signed char k) {
         if (s < k) return 0;
         return s - k;
     })c",
       {{"f(-100, 100)", 0},
        {"f(100, -27)", 127},
        {"f(5, 3)", 2},
        {"f(-128, 127)", 0}}},
      {R"c(
     unsigned long f(unsigned long a, unsigned long b) {
         if (a > b) return b - a;
         return 7;
     })c",
       {{"f(5, 3)", (3 - 5) & 0xFFFF},
        {"f(3, 5)", 7},
        {"f(70000, 69999)", (69999 - 70000) & 0xFFFF}}},
  };
  for (const Case& c : cases) {
    for (const Call& call : c.calls) {
      const std::string src =
          std::string(c.fn) + "\nint main(void) { return (int)(" + call.call + "); }\n";
      for (const bool optimize : {false, true}) {
        INFO(c.fn << "\ncall=" << call.call << " optimize=" << optimize);
        CHECK(build_sim(src, optimize).run().return_int() == (call.expected & 0xFFFF));
      }
    }
  }
}

TEST_CASE("compare_subtract sim: a 16-bit loop of compare and subtract stays in A and X") {
  // `while (s >= k) s -= k;` on a 16-bit `s`, as Zork's
  // `do_rwts_on_sector` does. `s` stays in A (low) and X (high), the exit
  // adds `k` back, and a pass through the loop is `SBC; BCS` when nothing
  // is borrowed from the high byte. `f` takes `n0` in A, so that `s`
  // arrives in its slot and is loaded into A and X before the loop.
  const std::string src = R"c(
        unsigned char k;
        unsigned char n;
        unsigned char f(unsigned char n0 [[c6502::in_a]], unsigned int s) {
            n = n0;
            while (s >= k) { s -= k; n++; }
            return (unsigned char)s;
        }
        int main(void) { k = 16; return f(2, 1000) + n; }
        )c";
  const std::string text = codegen(src, Pipeline::kOptimized);
  const std::string body = between(text, "\nf:\n", "\nmain:\n");
  const std::string loop = between(body, ".loop@0_continue:", ".loop@0_break:");
  std::vector<std::string> got;
  for (const std::string& line : lines(loop)) {
    std::string collapsed;
    std::size_t k = 0;
    while (k < line.size()) {
      while (k < line.size() && std::isspace(static_cast<unsigned char>(line[k]))) ++k;
      const std::size_t start = k;
      while (k < line.size() && !std::isspace(static_cast<unsigned char>(line[k]))) ++k;
      if (k > start) {
        if (!collapsed.empty()) collapsed += ' ';
        collapsed += line.substr(start, k - start);
      }
    }
    if (!collapsed.empty()) got.push_back(collapsed);
  }
  const std::vector<std::string> want = {
      "SBC k", "BCS .thread@0", "CPX #$01", "DEX", "BCC .loop@0_break", ".thread@0:", "INC n",
      "JMP .loop@0_continue",
  };
  CHECK(got == want);
  CHECK(contains(body.substr(body.find(".loop@0_break:")), "ADC   k"));
  for (const bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    CHECK(build_sim(src, optimize).run().return_int() == 1000 % 16 + 2 + 1000 / 16);
  }
}

}  // namespace c6502::suite
