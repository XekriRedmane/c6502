// tests/test_compound_literal.py's programs on the TAC interpreter. The
// simulator's runs are in test_compound_literal_sim.cpp.
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

// Returns the low 16 bits of `main`'s value on the TAC, as an `int`.
long long tac_int(const std::string& source) {
  const auto value = suite::run_program(source);
  REQUIRE(value.has_value());
  long long v = static_cast<long long>(*value & 0xFFFF);
  if (v & 0x8000) v -= 0x10000;
  return v;
}

}  // namespace

TEST_CASE("compound_literal: the programs return their values on the TAC interpreter") {
  const std::vector<Program> programs = {
      {"address_of_scalar", R"(int main(void) { int *p = &(int){5}; *p += 2; return *p; })", 7},
      {"array_decays", R"(int sum(int *a, int n) { int s = 0; for (int i = 0; i < n; i++) s += a[i]; return s; }int main(void) { return sum((int[]){1, 2, 3, 4}, 4); })", 10},
      {"sizeof_array", R"(int main(void) { return sizeof((long[]){1, 2, 3}); })", 12},
      {"struct_member", R"(struct pt { int x; int y; };int main(void) { return (struct pt){3, 4}.y; })", 4},
      {"struct_return", R"(struct zc { int z; unsigned char ok; };struct zc f(int z) { return (struct zc){ z, 1 }; }int main(void) { struct zc r = f(9); return r.z + r.ok; })", 10},
      {"struct_assign", R"(struct pt { int x; int y; };int main(void) { struct pt p; p = (struct pt){5, 6}; return p.x * p.y; })", 30},
      {"struct_argument", R"(typedef struct { int a; int b; } P;int add(P p) { return p.a + p.b; }int main(void) { return add((P){2, 3}); })", 5},
      {"reinitialized_in_loop", R"(int main(void) { int s = 0; for (int i = 0; i < 3; i++) { int *p = (int[]){i, i * 10}; s += p[0] + p[1]; p[1] = 100; } return s; })", 33},
      {"missing_elements_are_zero", R"(int main(void) { int *p = (int[4]){1}; return p[0] + p[1] + p[2] + p[3]; })", 1},
      {"char_array_from_string", R"(int main(void) { char *s = (char[]){"hey"}; s[0] = 'H'; return s[0] + sizeof((char[]){"hey"}); })", 76},
      {"increment", R"(int main(void) { return (int){5}++ + 1; })", 6},
      {"const_elements", R"(int main(void) { const int *p = (const int[]){3, 4}; return p[1]; })", 4},
      {"not_evaluated_after_false_and", R"(int n; int bump(void) { n++; return 1; }int main(void) { int z = 0; int r = z && ((int[]){bump()})[0]; return n * 10 + r; })", 0},
      {"evaluated_after_true_and", R"(int n; int bump(void) { n++; return 1; }int main(void) { int z = 1; int r = z && ((int[]){bump()})[0]; return n * 10 + r; })", 11},
      {"conditional", R"(int main(void) { int c = 0; int *p = c ? (int[]){4} : (int[]){5}; return *p; })", 5},
      {"file_scope_array", R"(int *gp = (int[]){7, 8, 9};int main(void) { return gp[2]; })", 9},
      {"file_scope_struct", R"(struct pt { int x; int y; }; struct pt *sp = &(struct pt){1, 2};int main(void) { sp->y += 5; return sp->y; })", 7},
  };
  for (const Program& p : programs) {
    INFO(p.name);
    CHECK(tac_int(p.source) == p.expected);
  }
}
