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
    {"address_of_scalar", "int main(void) { int *p = &(int){5}; *p += 2; return *p; }", 7},
    {"array_decays",
     "int sum(int *a, int n) { int s = 0; for (int i = 0; i < n; i++) s += a[i]; return s; }int main(void) { return "
     "sum((int[]){1, 2, 3, 4}, 4); }",
     10},
    {"sizeof_array", "int main(void) { return sizeof((long[]){1, 2, 3}); }", 12},
    {"struct_member", "struct pt { int x; int y; };int main(void) { return (struct pt){3, 4}.y; }", 4},
    {"struct_return",
     "struct zc { int z; unsigned char ok; };struct zc f(int z) { return (struct zc){ z, 1 }; }int main(void) { struct "
     "zc r = f(9); return r.z + r.ok; }",
     10},
    {"struct_assign",
     "struct pt { int x; int y; };int main(void) { struct pt p; p = (struct pt){5, 6}; return p.x * p.y; }", 30},
    {"struct_argument",
     "typedef struct { int a; int b; } P;int add(P p) { return p.a + p.b; }int main(void) { return add((P){2, 3}); }",
     5},
    // §6.5.2.5.16: each evaluation initializes the object again.
    {"reinitialized_in_loop",
     "int main(void) { int s = 0; for (int i = 0; i < 3; i++) { int *p = (int[]){i, i * 10}; s += p[0] + p[1]; p[1] = "
     "100; } return s; }",
     33},
    {"missing_elements_are_zero", "int main(void) { int *p = (int[4]){1}; return p[0] + p[1] + p[2] + p[3]; }", 1},
    {"char_array_from_string",
     R"c(int main(void) { char *s = (char[]){"hey"}; s[0] = 'H'; return s[0] + sizeof((char[]){"hey"}); })c", 72 + 4},
    {"increment", "int main(void) { return (int){5}++ + 1; }", 6},
    {"const_elements", "int main(void) { const int *p = (const int[]){3, 4}; return p[1]; }", 4},
    {"not_evaluated_after_false_and",
     "int n; int bump(void) { n++; return 1; }int main(void) { int z = 0; int r = z && ((int[]){bump()})[0]; return n "
     "* 10 + r; }",
     0},
    {"evaluated_after_true_and",
     "int n; int bump(void) { n++; return 1; }int main(void) { int z = 1; int r = z && ((int[]){bump()})[0]; return n "
     "* 10 + r; }",
     11},
    {"conditional", "int main(void) { int c = 0; int *p = c ? (int[]){4} : (int[]){5}; return *p; }", 5},
    {"file_scope_array", "int *gp = (int[]){7, 8, 9};int main(void) { return gp[2]; }", 9},
    {"file_scope_struct",
     "struct pt { int x; int y; }; struct pt *sp = &(struct pt){1, 2};int main(void) { sp->y += 5; return sp->y; }",
     7}};
TEST_CASE("compound_literal: asm sim") {
  for (const auto& [name, src, expected] : kPrograms) {
    for (bool optimize : {false, true}) {
      INFO("case=" << name << " optimize=" << optimize);
      auto result = build_sim(src, optimize).run(2'000'000);
      CHECK_FALSE(result.timed_out);
      CHECK(result.return_int_signed() == expected);
    }
  }
}

TEST_CASE("compound_literal: static local pointer to static array") {
  // A block-scope static initializer decays an array, as a
  // file-scope one does.
  std::string src = "int a[2] = {3, 4};int main(void) { static int *p = a; return p[1]; }";
  CHECK(build_sim(src).run().return_int_signed() == 4);
}

}  // namespace
}  // namespace c6502::suite
