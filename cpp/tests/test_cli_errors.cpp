// The tests of tests/test_cli_errors.py: compile.py reports a problem
// with the program as one line, `<file>: error: <message>`, with exit
// status 1 and no internal error, and an input that can't be read the
// same way.
#include <string>
#include <utility>
#include <vector>

#include "compile_case.h"
#include "doctest.h"

namespace {

namespace suite = c6502::suite;

// Runs `compile.py - -S <flags>` on `src` and returns the status and
// stderr.
std::pair<long long, std::string> compile_s(const std::string& src,
                                            const std::vector<std::string>& flags = {}) {
  std::vector<std::string> argv{"compile.py", "-", "-S"};
  argv.insert(argv.end(), flags.begin(), flags.end());
  const auto o = suite::compile_main(argv, src);
  REQUIRE(o.status.has_value());
  return {*o.status, o.err()};
}

std::pair<long long, std::string> run(const std::vector<std::string>& args) {
  std::vector<std::string> argv{"compile.py"};
  argv.insert(argv.end(), args.begin(), args.end());
  const auto o = suite::compile_main(argv);
  REQUIRE(o.status.has_value());
  return {*o.status, o.err()};
}

bool starts_with(const std::string& s, const std::string& prefix) {
  return s.rfind(prefix, 0) == 0;
}

}  // namespace

TEST_CASE("cli_errors: a frame too large is a one-line error") {
  // Only without -O, since the optimizer folds this array away.
  const auto [rc, err] = compile_s(
      "unsigned char f(void) { unsigned char b[300]; b[0] = 1; return b[0]; }");
  CHECK(rc == 1);
  CHECK(starts_with(err, "<stdin>: error: function 'f': "));
  CHECK(suite::contains(err, "a frame holds at most 253"));
}

TEST_CASE("cli_errors: an argument read past the frame's reach is a one-line error") {
  // The locals fit in 253 bytes, but `d` sits above them, the saved
  // frame pointer, and the other arguments, past Frame(255), and the
  // function reads it (`fuzz.exprs` seed 43).
  const auto [rc, err] = compile_s(
      "unsigned char f(unsigned long long a, unsigned long long b, "
      "unsigned long long c, unsigned char d) "
      "{ unsigned char t[225]; t[0] = d; return t[0]; }");
  CHECK(rc == 1);
  CHECK(starts_with(err, "<stdin>: error: function 'f': "));
  CHECK(suite::contains(err, "frame offset 261 out of range 1..255"));
  CHECK_FALSE(suite::contains(err, "internal error"));
}

TEST_CASE("cli_errors: an argument past the frame's reach that nothing reads compiles") {
  const auto [rc, err] = compile_s(
      "unsigned char f(unsigned long long a, unsigned long long b) "
      "{ unsigned char t[230]; t[0] = 1; return t[0]; }");
  CHECK_MESSAGE(rc == 0, err);
}

TEST_CASE("cli_errors: misused attributes are one-line errors with and without -O") {
  const std::vector<std::pair<std::string, std::string>> cases = {
      {"[[c6502::bogus]] void f(void);", "unknown attribute"},
      {"[[c6502::zp_abi, c6502::zp_abi]] void f(void);", "`c6502::zp_abi` is given twice"},
      {"[[c6502::preserves_x]] void f(void);",
       "has `preserves_x` but no `[[c6502::address(...)]]`"},
      {"[[c6502::preserves_x, c6502::address(0x1000)]] int f(void);",
       "whose high byte comes back in X"},
      {"[[c6502::preserves_y]] void f(void) {}", "can't have `preserves_x` or `preserves_y`"},
      {"struct s { int v; bool end; };\n"
       "[[c6502::preserves_x, c6502::return_bool_in_carry, c6502::"
       "address(0x1000)]] struct s f(void);",
       "whose value member is 2 bytes"},
      {"struct s { long v; bool end; };\n"
       "[[c6502::return_bool_in_carry]] struct s f(void);",
       "exactly two members"},
      {"[[c6502::return_bool_in_carry]] int f(void);",
       "a struct of a value and a `bool`, not `int`"},
      {"int g(int x) { return x; }\nint f(int x) {\n"
       "[[c6502::save_hardware_stack]] return g(x);\n}\n",
       "'g' can't call 'f' again"},
      {"int f(int x) {\n[[c6502::save_hardware_stack]]\n"
       "if (x) return f(x - 1);\nreturn 0;\n}\n",
       "goes before an expression statement"},
      {"#include <stdint.h>\nint f(void) {\n"
       "#pragma c6502 loop unroll(enable)\n"
       "for (int i = 0; i < 2; i++) ;\nreturn 0;\n}\n",
       "<stdin>: error: line 3: c6502 no longer takes `#pragma "
       "c6502 loop unroll(enable)`; write the statement attribute "
       "`[[c6502::unroll]]`"},
      {"int main(void) { unsigned char a [[c6502::zeropage]]; return a; }", "'a' is automatic"},
      {"[[c6502::address(0x1000)]] void f(void);\n"
       "[[c6502::address(0x2000)]] void f(void);",
       "different addresses, $1000 and $2000"},
      {"[[c6502::address(0x1000)]] void f(register unsigned char c);\n"
       "void g(void) { f(1); }",
       "no `[[c6502::zp_abi]]`"},
  };
  for (const auto& [src, needle] : cases) {
    for (const std::vector<std::string>& flags :
         {std::vector<std::string>{}, std::vector<std::string>{"-O"}}) {
      INFO(src << (flags.empty() ? "" : " with -O"));
      const auto [rc, err] = compile_s(src, flags);
      CHECK(rc == 1);
      CHECK(starts_with(err, "<stdin>: error: "));
      CHECK(suite::contains(err, needle));
      CHECK_FALSE(suite::contains(err, "internal error"));
    }
  }
}

TEST_CASE("cli_errors: a message names a block-scope object by its own name") {
  // A block-scope object is renamed `@N.x` inside the compiler.
  const auto [rc, err] = compile_s(
      "int main(void) { static unsigned char x [[c6502::address(0xC000)]]; return x; }");
  CHECK(rc == 1);
  CHECK(suite::contains(err, "'x'"));
  CHECK_FALSE(suite::contains(err, "@"));
}

TEST_CASE("cli_errors: a missing input or linker script is a one-line error") {
  const suite::TempDir tmp;
  const std::string out = tmp.file("out");
  const std::string obj = tmp.file("a.o");
  CHECK(run({suite::repo_path("examples/sfx_tone.c"), "-c", "-o", obj}).first == 0);
  const std::vector<std::pair<std::vector<std::string>, std::string>> cases = {
      {{"missing.c", "-S", "-o", out + ".s"}, "missing.c: error: No such file or directory\n"},
      {{"missing.c", "-c", "-o", out}, "missing.c: error: No such file or directory\n"},
      {{"missing.asm", "-c", "-o", out}, "missing.asm: error: No such file or directory\n"},
      {{"missing.o", "-o", out}, "missing.o: error: No such file or directory\n"},
      {{"-T", "missing.ld", obj, "-o", out},
       "compile.py: link error: missing.ld: No such file or directory\n"},
  };
  for (const auto& [argv, expected] : cases) {
    INFO(argv[0] << " " << argv[1]);
    CHECK(run(argv) == std::pair<long long, std::string>{1, expected});
  }
}

TEST_CASE("cli_errors: a source that isn't UTF-8 is a one-line error") {
  const suite::TempDir tmp;
  const std::string src = tmp.file("latin1.c");
  suite::write_file(src, "int main(void) { return 0; } /* \xe9 */\n");
  const auto [rc, err] = run({src, "-S", "-o", tmp.file("x.s")});
  CHECK(rc == 1);
  CHECK(err == src + ": error: 'utf-8' codec can't decode byte 0xe9 in "
                     "position 32: invalid continuation byte\n");
}
