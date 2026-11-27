// Functions of a unit that end alike, whose shared tail
// src/passes/shared_tails.h puts in a function of its own, run in both
// pipelines and compiled on their own and linked, from
// tests/test_shared_tails.py.
#include <string>

#include "compile_case.h"
#include "doctest.h"
#include "sim_case.h"

namespace c6502::suite {

namespace {

// `store` takes its multiplier in X, so that `v` arrives in its slot and
// each caller ends in the same stores into it, `STA v_0; LDA #0; STA
// v_1; LDX #3; JMP store`. Passed in A and X, `v` left only `LDX #0; JMP
// store`, which is too short to share.
const char* const kSource = R"c(
#include <stdint.h>
uint16_t acc;
uint8_t tab[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };
void store(uint8_t m [[c6502::in_x]], uint16_t v) { acc = acc * m + v; }
void f1(uint8_t i) { store(3, tab[i]); }
void f2(uint8_t i) { store(3, (uint8_t)(tab[i] + 1)); }
void f3(uint8_t i) { store(3, (uint8_t)(tab[i] << 1)); }
int main(void) {
    f1(2); f2(5); f3(7); f1(0);
    return acc;
}
)c";
constexpr std::int64_t kExpected = ((3 * 3 + 7) * 3 + 16) * 3 + 1;

}  // namespace

TEST_CASE("shared_tails sim: a tail is shared and the program runs") {
  CHECK(contains(codegen(kSource, true), "@tail0:"));
  for (const bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    CHECK(build_sim(kSource, optimize).run().return_int() == kExpected);
  }
}

TEST_CASE("shared_tails sim: the program compiled on its own and linked runs") {
  // The linker puts one function right before the tail and drops its
  // jump.
  TempDir tmp;
  const std::string obj = tmp.file("a.o");
  const std::string exe = tmp.file("a.out");
  REQUIRE(compile_object(kSource, obj) == 0);
  const compile_command::Outcome linked = link({obj}, exe);
  REQUIRE(linked.status.has_value());
  REQUIRE(*linked.status == 0);
  bool tails = false;
  for (const auto& [name, addr] : symbols(exe)) {
    if (contains(name, "@tail")) tails = true;
  }
  CHECK(tails);
  CHECK(build_linked_sim(exe).run().return_int() == kExpected);
}

}  // namespace c6502::suite
