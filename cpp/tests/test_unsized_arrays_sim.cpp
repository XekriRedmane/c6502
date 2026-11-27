// Arrays sized by their initializer (C99 §6.7.8.22), at file scope and
// block scope, static or automatic, in programs run in both pipelines,
// from tests/test_unsized_arrays.py.
#include <string>

#include "doctest.h"
#include "sim_case.h"

namespace c6502::suite {

namespace {

void check_returns(const std::string& src, std::int64_t want) {
  for (const bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    CHECK(build_sim(src, optimize).run().return_int() == want);
  }
}

}  // namespace

TEST_CASE("unsized_arrays sim: sizeof gives the size the initializer sets") {
  check_returns(R"c(
            int t[] = {1, 2, 3};
            char s[] = "abc";
            const unsigned char k[] = {9, 8};
            char b[] = {"xy"};
            int m[][2] = {{1, 2}, {3, 4}, {5, 6}};
            int main(void) {
                static int u[] = {4, 5, 6, 7};
                long v[] = {1, 2};
                return sizeof t + sizeof s + sizeof k + sizeof b
                    + sizeof m + sizeof u + sizeof v;
            }
        )c", 6 + 4 + 2 + 3 + 12 + 8 + 8);
}

TEST_CASE("unsized_arrays sim: an extern declaration is completed by the definition") {
  check_returns(R"c(
            extern int t[];
            int first(void) { return t[0]; }
            int t[] = {7, 8, 9};
            int main(void) { return first() + t[2] + sizeof t; }
        )c", 7 + 9 + 6);
}

TEST_CASE("unsized_arrays sim: the elements hold the initializer's values") {
  check_returns(R"c(
            unsigned char t[] = {10, 20, 30, 40};
            const char s[] = "hello";
            int main(void) {
                unsigned char local[] = {1, 2, 3};
                int sum = 0;
                for (unsigned char i = 0; i < sizeof t; i++) sum += t[i];
                for (unsigned char i = 0; i < sizeof local; i++)
                    sum += local[i];
                return sum + s[4] + s[5];
            }
        )c", 100 + 6 + 'o');
}

}  // namespace c6502::suite
