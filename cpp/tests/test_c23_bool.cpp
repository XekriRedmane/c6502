// The tests of tests/test_c23_bool.py that run no simulator: the warning
// at each `_Bool`, which compile.py prints before any error and with
// exit status 0 otherwise, C23's <stdbool.h>, and a program of `bool`,
// `true`, and `false` on the TAC interpreter.
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "compile_case.h"
#include "doctest.h"

namespace {

namespace suite = c6502::suite;

// The warning at each `_Bool` (`kAltBoolWarning` in
// cpp/src/preprocessor.cpp).
const std::string kAltBoolWarning =
    "`_Bool` is deprecated: C23 makes it an obsolescent spelling "
    "of the keyword `bool` (C23 §6.4.1). Use `bool` instead.";

// compile.py's exit status and stderr for `src` on stdin.
std::pair<long long, std::string> compile_s(const std::string& src,
                                            const std::vector<std::string>& flags = {}) {
  std::vector<std::string> argv{"compile.py", "-", "-S"};
  argv.insert(argv.end(), flags.begin(), flags.end());
  const auto o = suite::compile_main(argv, src);
  REQUIRE(o.status.has_value());
  return {*o.status, o.err()};
}

}  // namespace

TEST_CASE("c23_bool: compile.py warns at _Bool and succeeds") {
  for (const std::vector<std::string>& flags :
       {std::vector<std::string>{}, std::vector<std::string>{"-O"}}) {
    INFO((flags.empty() ? "" : "-O"));
    const auto [rc, err] = compile_s("int x;\n_Bool f(int v) { return v; }\n", flags);
    CHECK(rc == 0);
    CHECK(err == "<stdin>: warning: line 2: " + kAltBoolWarning + "\n");
  }
}

TEST_CASE("c23_bool: the warning names the line of the macro's use") {
  const auto [rc, err] = compile_s("#define B _Bool\n\nB b;\n");
  CHECK(rc == 0);
  CHECK(err == "<stdin>: warning: line 3: " + kAltBoolWarning + "\n");
}

TEST_CASE("c23_bool: the warning comes before an error") {
  const auto [rc, err] = compile_s("_Bool a;\nint true;\n");
  CHECK(rc == 1);
  const std::vector<std::string> lines = suite::lines(err);
  REQUIRE(lines.size() >= 2);
  CHECK(lines[0] == "<stdin>: warning: line 1: " + kAltBoolWarning);
  CHECK_MESSAGE(lines[1].rfind("<stdin>: error: ", 0) == 0, err);
}

TEST_CASE("c23_bool: <stdbool.h> includes without a warning") {
  // The tokens `compile.py --lex` prints are those of the preprocessor's
  // output.
  const suite::Compiled c =
      suite::compile(suite::Stage::kLex,
                     "#include <stdbool.h>\n"
                     "#if __bool_true_false_are_defined != 1\n"
                     "#error\n"
                     "#endif\n"
                     "#if defined(bool) || defined(true) || defined(false)\n"
                     "#error\n"
                     "#endif\n"
                     "bool b = true; int n = __bool_true_false_are_defined;\n");
  REQUIRE_FALSE(c.error.has_value());
  CHECK(c.warnings.empty());
  std::string values;
  for (const std::string& line : suite::lines(c.text)) {
    const std::size_t a = line.find('\t');
    REQUIRE(a != std::string::npos);
    const std::size_t b = line.find('\t', a + 1);
    REQUIRE(b != std::string::npos);
    const std::size_t e = line.find('\t', b + 1);
    values += (values.empty() ? "" : " ") +
              line.substr(b + 1, e == std::string::npos ? std::string::npos : e - b - 1);
  }
  CHECK(values == "bool b = true ; int n = 1 ;");
}

TEST_CASE("c23_bool: a program with <stdbool.h> compiles without a warning") {
  const auto result = compile_s(
      "#include <stdbool.h>\n"
      "bool odd(int x) { return x & 1; }\n"
      "int main(void) { return odd(3) == true; }\n");
  CHECK(result == std::pair<long long, std::string>{0, ""});
}

TEST_CASE("c23_bool: the TAC interpreter runs a program of bool, true, and false") {
  // test_c23_bool_sim.cpp runs the same program in the simulator.
  const std::string program = R"(#include <stdbool.h>

enum { TWO = true + true };
static bool flags[TWO + 1] = { true, false, true };
static int seen = false;
bool global = 7;

[[c6502::return_bool_in_carry]] bool positive(int x) { return x > 0; }

bool odd(unsigned char x) { return x & 1; }

int classify(bool b) {
    switch (b) {
    case true:
        return 10;
    case false:
        return 20;
    }
    return 0;
}

int main(void) {
    bool b = 256;
    int r = 0;
    for (unsigned char i = 0; i < sizeof flags; i++)
        if (flags[i] == true)
            r += 1;
    if (positive(5) && !positive(-5))
        r += 100;
    r += classify(odd(3)) + classify(odd(4));
    r += b + global + seen + TWO + sizeof(true) + (true << 9);
    return r;
}
)";
  // 2 flags + 100 + 10 + 20 + 1 + 1 + 0 + 2 + 1 + 512
  CHECK(suite::run_program(program) == std::optional<c6502::asdl::Int>(649));
}
