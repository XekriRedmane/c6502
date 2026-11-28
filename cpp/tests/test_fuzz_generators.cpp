// The fuzzers' generators (src/fuzz/exprs.h, calls.h, and stmts.h)
// against golden/fuzz/, which the Python generators wrote.
#include <optional>
#include <string>

#include "compile_case.h"
#include "doctest.h"
#include "fuzz/calls.h"
#include "fuzz/exprs.h"
#include "fuzz/stmts.h"

namespace fuzz = c6502::fuzz;
using c6502::suite::read_repo_file;

TEST_CASE("fuzz: the generators write golden/fuzz/'s programs") {
  for (int seed = 0; seed < 100; ++seed) {
    INFO("seed " << seed);
    CHECK(fuzz::exprs::make_program(seed).source ==
          read_repo_file("golden/fuzz/exprs" + std::to_string(seed) + ".c"));
    CHECK(fuzz::calls::make_program(seed).source ==
          read_repo_file("golden/fuzz/calls" + std::to_string(seed) + ".c"));
    const fuzz::GeneratedProgram stmts = fuzz::stmts::make_program(seed);
    CHECK(stmts.source == read_repo_file("golden/fuzz/stmts" + std::to_string(seed) + ".c"));
    REQUIRE(stmts.expected);
    CHECK(stmts.source.rfind("// expect: " + std::to_string(*stmts.expected) + "\n", 0) == 0);
  }
}

TEST_CASE("fuzz: exprs and calls know main's result only for exprs") {
  CHECK(fuzz::exprs::make_program(5).expected == 0);
  CHECK(fuzz::calls::make_program(5).expected == std::nullopt);
}

TEST_CASE("fuzz: a seed gives the same program twice") {
  CHECK(fuzz::exprs::make_program(7) == fuzz::exprs::make_program(7));
  CHECK(fuzz::calls::make_program(7) == fuzz::calls::make_program(7));
}
