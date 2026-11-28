// The fuzzers' runner and reducer (src/fuzz/runner.h and reduce.h),
// with the checks of tests/test_fuzz_tools.py, which ran a few seeds on
// every path and tested the reducer's check for a function that falls
// off its end.
#include <optional>
#include <string>
#include <vector>

#include "compile_case.h"
#include "doctest.h"
#include "fuzz/calls.h"
#include "fuzz/exprs.h"
#include "fuzz/reduce.h"
#include "fuzz/runner.h"
#include "fuzz/yarpgen.h"

namespace {

namespace fuzz = c6502::fuzz;
using c6502::suite::repo_path;

fuzz::Runner runner() { return fuzz::Runner(repo_path("include")); }

std::string bad_paths(const std::vector<fuzz::Mode>& bad) {
  std::string out;
  for (fuzz::Mode m : bad) out += std::string(out.empty() ? "" : " ") + fuzz::mode_name(m);
  return out;
}

// Checks that every path gives the program's result.
void check_agree(const fuzz::GeneratedProgram& p) {
  const fuzz::Results res = runner().run_all(p.source);
  INFO(fuzz::format_result(res) << "\n" << p.source);
  CHECK(bad_paths(fuzz::disagreements(res, p.expected)) == "");
}

}  // namespace

TEST_CASE("fuzz: seeds 0 to 2 of exprs agree on every path") {
  for (int seed = 0; seed < 3; ++seed) {
    INFO("seed " << seed);
    check_agree(fuzz::exprs::make_program(seed));
  }
}

TEST_CASE("fuzz: seeds 1 and 44 of calls agree on every path") {
  // Seed 44 has every feature: the recursive function, the
  // pointer-called function, `register`, pointer parameters, and a call
  // in a loop. Seed 1 has `register` only.
  for (int seed : {1, 44}) {
    INFO("seed " << seed);
    check_agree(fuzz::calls::make_program(seed));
  }
}

// The reducer never keeps a candidate that reaches the end of a function
// returning a value, whose value C99 6.9.1p12 leaves undefined. Reducing
// `fuzz.calls` seed 335 emptied three functions whose results were still
// used, which made the reduced program's disagreement meaningless.
TEST_CASE("fuzz: falls_off_end tells whether a run reaches the end of a function") {
  struct Case {
    std::string source;
    bool falls_off;
  };
  const Case cases[] = {
      {"int f(int x) { return x + 1; }\nint main(void) { return f(2); }\n", false},
      {"int f(int x) { x = x + 1; }\nint main(void) { return f(2); }\n", true},
      // Only the path the run takes counts.
      {"int f(int x) { if (x) return 1; }\nint main(void) { return f(2); }\n", false},
      {"int f(int x) { if (x) return 1; }\nint main(void) { return f(0); }\n", true},
      {"int g; void f(void) { g = 1; }\nint main(void) { f(); return g; }\n", false},
      // C99 5.1.2.2.3 defines `main` falling off as returning 0.
      {"int main(void) { }\n", false},
  };
  const fuzz::Runner r = runner();
  for (const Case& c : cases) {
    INFO(c.source);
    CHECK(fuzz::falls_off_end(r, c.source) == c.falls_off);
  }
}

TEST_CASE("fuzz: the reducer's predicate rejects a candidate that falls off the end") {
  // A program that fails on every path, so that any candidate failing the
  // same way would be kept.
  const std::string src =
      "int f(int x) { return x; }\nint main(void) { return f(3); }\n// expect: 4\n";
  const fuzz::Runner r = runner();
  const fuzz::Predicate still_fails = fuzz::make_predicate(r, src, fuzz::Failure::kPaths);
  CHECK(still_fails(src));
  CHECK_FALSE(still_fails("int f(int x) { x = 3; }\nint main(void) { return f(3); }\n// expect: 4\n"));
}

TEST_CASE("fuzz: the reducer deletes lines and then replaces binary expressions by an operand") {
  const std::string src =
      "// expect: 1\n"
      "int a;\n"
      "int q = ((a) + ((b) * (c)));\n"
      "int z;\n";
  const fuzz::Predicate has_q = [](const std::string& cand) {
    return cand.find("int q") != std::string::npos;
  };
  CHECK(fuzz::reduce(src, has_q) == "// expect: 1\nint q = (a);\n");
}

TEST_CASE("fuzz: a program wrong on every path reduces to a main that returns nothing") {
  // main falling off its end returns 0 (C99 5.1.2.2.3), which is as wrong
  // on every path as the original's 3.
  const std::string src =
      "// expect: 4\n"
      "int g;\n"
      "int h(int x) { return ((x) + (1)); }\n"
      "int main(void) {\n"
      "    g = 7;\n"
      "    return ((h(2)) * (1));\n"
      "}\n";
  const fuzz::Runner r = runner();
  const fuzz::Predicate still_fails = fuzz::make_predicate(r, src, fuzz::Failure::kPaths);
  REQUIRE(still_fails(src));
  CHECK(fuzz::reduce(src, still_fails) == "// expect: 4\nint main(void) {\n}\n");
}

TEST_CASE("fuzz: a path's error is its Python class and the message's first 120 characters") {
  const fuzz::Runner r = runner();
  const fuzz::Result tac = r.run_tac("int main(void) { return x; }\n");
  REQUIRE(fuzz::is_error(tac));
  CHECK(std::get<std::string>(tac).rfind("ERR IdentifierResolutionError: ", 0) == 0);
  const std::string long_name(200, 'v');
  const fuzz::Result asm_error = r.run_asm("int main(void) { return " + long_name + "; }\n", false);
  REQUIRE(fuzz::is_error(asm_error));
  const std::string text = std::get<std::string>(asm_error);
  CHECK(text.size() == std::string("ERR IdentifierResolutionError: ").size() + 120);
}

TEST_CASE("fuzz: an unoptimized frame over 253 bytes is a backend limit") {
  const fuzz::Result r =
      runner().run_asm("int main(void) { volatile long a[70]; a[0] = 1; return (int)a[0]; }\n", false);
  INFO(fuzz::format_result(fuzz::Results{{r, r, r}}));
  CHECK(fuzz::is_backend_limit(r));
  CHECK_FALSE(fuzz::is_backend_limit(fuzz::Result(std::string("ERR TypeCheckError: x"))));
  CHECK_FALSE(fuzz::is_backend_limit(fuzz::Result(std::int64_t{3})));
}

TEST_CASE("fuzz: disagreements take the TAC result as the reference without an expected value") {
  using fuzz::Result;
  const fuzz::Results agree{{Result(std::int64_t{5}), Result(std::int64_t{5}), Result(std::int64_t{5})}};
  CHECK(fuzz::disagreements(agree, std::nullopt).empty());
  CHECK(bad_paths(fuzz::disagreements(agree, 4)) == "tac unopt opt");
  const fuzz::Results opt_wrong{{Result(std::int64_t{5}), Result(std::int64_t{5}), Result(std::int64_t{6})}};
  CHECK(bad_paths(fuzz::disagreements(opt_wrong, std::nullopt)) == "opt");
  const fuzz::Results tac_error{{Result(std::string("ERR TacSimError: x")), Result(std::int64_t{5}),
                                 Result(std::int64_t{5})}};
  CHECK(bad_paths(fuzz::disagreements(tac_error, std::nullopt)) == "tac unopt opt");
  CHECK(fuzz::format_result(tac_error) == "tac=ERR TacSimError: x unopt=5 opt=5");
}

TEST_CASE("fuzz: expected_of reads the first // expect: line as a signed 16-bit int") {
  CHECK(fuzz::expected_of("int main(void) { return 0; }\n") == std::nullopt);
  CHECK(fuzz::expected_of("//expect:7\n// expect: 8\n") == 7);
  CHECK(fuzz::expected_of("// expect: -3\n") == -3);
  CHECK(fuzz::expected_of("// expect: 40000\n") == 40000 - 65536);
}

TEST_CASE("fuzz: merge makes one program of YARPGen's three files") {
  const std::string init = "extern int a;\n";
  const std::string func = "#include \"init.h\"\nvoid test(void) { a = 1; }\n";
  const std::string driver =
      "#include <stdio.h>\n#include \"init.h\"\nint a;\n"
      "int main(void) { unsigned long long seed = a; printf(\"%llu\\n\", seed); }\n";
  CHECK(fuzz::yarpgen::merge(init, func, driver) ==
        "typedef unsigned int size_t;\nextern int a;\n\nvoid test(void) { a = 1; }\n\n\nint a;\n"
        "int main(void) { unsigned long long seed = a; return (int)((seed ^ (seed >> 16) ^ "
        "(seed >> 32) ^ (seed >> 48)) & 0x7FFF); }\n");
  CHECK_THROWS_WITH(fuzz::yarpgen::merge(init, func, "int main(void) { }\n"),
                    "driver.c doesn't print the checksum");
}
