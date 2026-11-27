// The tests of tests/test_compile.py: compile.py's command line with
// its stages, its outputs, and the preprocessor's flags, and the `-S`
// and `--tac` text of void, sizeof, and `-O`'s register allocation.
#include <algorithm>
#include <cctype>
#include <filesystem>
#include <string>
#include <utility>
#include <variant>
#include <vector>

#include "asm_ast.h"
#include "compile_case.h"
#include "doctest.h"
#include "passes/codegen_stage.h"
#include "passes/front_end.h"
#include "passes/ir_check.h"
#include "passes/zp_globals.h"

namespace {

namespace suite = c6502::suite;
namespace asm_ast = c6502::asm_ast;
namespace passes = c6502::passes;

constexpr const char* kSource = "int main(void) { return 42; }";

struct Run {
  long long rc;
  std::string out;
  std::string err;
};

// Runs compile.py's `main` on `argv` with `stdin_text`, which must
// return a status, as the Python tests' `_run` did, without raising
// SystemExit.
Run run(const std::vector<std::string>& argv, const std::string& stdin_text = "") {
  const auto o = suite::compile_main(argv, stdin_text);
  REQUIRE(o.status.has_value());
  CHECK_FALSE(o.exited);
  return {*o.status, o.out(), o.err()};
}

// Returns the `-S` text of `src`, compiled from stdin.
std::string codegen(const std::string& src) {
  const Run r = run({"compile.py", "-", "-S"}, src);
  REQUIRE_MESSAGE(r.rc == 0, r.err);
  return r.out;
}

// Returns the `--tac` text of `src`, compiled from stdin.
std::string tac(const std::string& src) {
  const Run r = run({"compile.py", "-", "--tac"}, src);
  REQUIRE_MESSAGE(r.rc == 0, r.err);
  return r.out;
}

bool starts_with(const std::string& s, const std::string& prefix) {
  return s.rfind(prefix, 0) == 0;
}

// Returns stderr of `-S` on `src`, which must fail with one line that
// reports the problem with the program.
std::string expect_failure(const std::string& src) {
  const Run r = run({"compile.py", "-", "-S"}, src);
  REQUIRE_MESSAGE(r.rc != 0, "expected compilation to fail for: " << src);
  CHECK_MESSAGE(starts_with(r.err, "<stdin>: error: "), r.err);
  return r.err;
}

std::string lower(std::string s) {
  std::transform(s.begin(), s.end(), s.begin(),
                 [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
  return s;
}

std::vector<std::string> split_tabs(const std::string& s) {
  std::vector<std::string> out;
  std::size_t start = 0;
  for (;;) {
    const std::size_t tab = s.find('\t', start);
    out.push_back(s.substr(start, tab - start));
    if (tab == std::string::npos) return out;
    start = tab + 1;
  }
}

std::string strip(const std::string& s) {
  const std::size_t first = s.find_first_not_of(" \t\r\n");
  if (first == std::string::npos) return "";
  return s.substr(first, s.find_last_not_of(" \t\r\n") - first + 1);
}

// Returns the lines of `out` from the first `label` on.
std::vector<std::string> section_from(const std::string& out, const std::string& label) {
  const std::size_t idx = out.find(label);
  REQUIRE_MESSAGE(idx != std::string::npos, label << " isn't in the text");
  return suite::lines(out.substr(idx));
}

// Returns the finished asm program that `sim.harness.compile_to_asm`
// returned for `source`, which is where `codegen` stops without `emit`.
asm_ast::Program compile_to_asm(const std::string& source, bool optimize) {
  const passes::ir_check::Enabled checks(true);
  passes::FrontEndOptions front;
  front.include_dir = suite::repo_path("include");
  front.optimize = optimize;
  std::string err;
  passes::StageTables tables;
  passes::FrontEndResult checked = passes::front_end(source, front, err);
  tables.c99 = std::move(checked.prog);
  tables.symbols = std::move(checked.symbols);
  tables.types = std::move(checked.types);
  tables.footprints = passes::helper_footprints();
  passes::CodegenOptions options;
  options.optimize = optimize;
  options.emit = false;
  return passes::codegen(tables, options).emitted.asm_prog;
}

}  // namespace

// --- The driver ---

TEST_CASE("compile: --lex on stdin prints the tokens") {
  const Run r = run({"compile.py", "-", "--lex"}, kSource);
  CHECK(r.rc == 0);
  const auto lines = suite::lines(r.out);
  REQUIRE_FALSE(lines.empty());
  CHECK(split_tabs(lines.front()) == std::vector<std::string>{"1:1", "keyword", "int"});
  const auto last = split_tabs(lines.back());
  CHECK(std::vector<std::string>(last.begin() + 1, last.end()) ==
        std::vector<std::string>{"symbol", "}"});
}

TEST_CASE("compile: --parse on stdin prints the tree") {
  const Run r = run({"compile.py", "-", "--parse"}, kSource);
  CHECK(r.rc == 0);
  for (const char* needle : {"Program(", "FunctionDecl(", "Constant(", "ConstInt(", "value=42"}) {
    INFO(needle);
    CHECK(suite::contains(r.out, needle));
  }
}

TEST_CASE("compile: a construct c6502 doesn't take is a one-line error") {
  const std::vector<std::pair<std::string, std::string>> cases = {
      {"int n; int a[n];", "array size must be an integer constant expression"},
      {"int f(a, b);", "K&R-style identifier-list functions aren't supported"},
      {"int g(int x[static 3]);", "only plain array sizes are supported"},
      // A qualifier with no size used to reach the size's fold as if it
      // were the size, which raised TypeError.
      {"int h(int x[const]);", "type-qualified array sizes aren't supported"},
      {"int a[volatile];", "type-qualified array sizes aren't supported"},
  };
  for (const auto& [src, message] : cases) {
    INFO(src);
    const Run r = run({"compile.py", "-", "-S"}, src);
    CHECK(r.rc == 1);
    CHECK_MESSAGE(starts_with(r.err, "<stdin>: error: "), r.err);
    CHECK(suite::contains(r.err, message));
    CHECK_FALSE(suite::contains(r.err, "internal error"));
  }
}

TEST_CASE("compile: --tac on stdin prints the TAC") {
  const Run r = run({"compile.py", "-", "--tac"}, "int main(void) { return -42; }");
  CHECK(r.rc == 0);
  for (const char* needle : {"Unary(", "Negate(", "'%0'", "Ret("}) {
    INFO(needle);
    CHECK(suite::contains(r.out, needle));
  }
}

TEST_CASE("compile: -S on stdin prints the assembly") {
  const Run r = run({"compile.py", "-", "-S"}, kSource);
  CHECK(r.rc == 0);
  for (const char* needle : {"main:", "SUBROUTINE", "LDA   #$2A", "RTS"}) {
    INFO(needle);
    CHECK(suite::contains(r.out, needle));
  }
}

TEST_CASE("compile: if-else branches to translator-made local labels") {
  // The leading dot is dasm's mark of a local label, and `@` marks a
  // label the translator made, which a name in the program can't be.
  // Arms that each `return` would leave the JMP after a return, where
  // `unreachable_drop` removes it.
  const std::string out = codegen(
      "int main(void) { int a = 0; int b; if (a) b = 1; else b = 2; return b; }");
  for (const char* needle : {"BEQ   .if_else@", "JMP   .if_end@", ".if_else@", ".if_end@"}) {
    INFO(needle);
    CHECK(suite::contains(out, needle));
  }
}

TEST_CASE("compile: a block's shadowing object gets a slot of its own") {
  const std::string out = codegen("int main(void) { int a = 1; { int a = 2; } return a; }");
  CHECK(suite::contains(out, "LDA   #$01"));
  CHECK(suite::contains(out, "LDA   #$02"));
}

TEST_CASE("compile: a backward goto jumps to the function's label") {
  // A forward `goto` over a statement makes the statement unreachable,
  // and once it's gone the JMP is a jump to the next instruction, which
  // is dropped too.
  const std::string out = codegen(
      "int main(void) { int a = 0; foo: a = a + 1; "
      "if (a < 5) { a = a + 2; goto foo; } return a; }");
  CHECK(suite::contains(out, "JMP   .main@foo"));
  CHECK(suite::contains(out, ".main@foo:"));
}

TEST_CASE("compile: postfix ++ adds one") {
  CHECK(suite::contains(codegen("int main(void) { int a = 0; a++; return a; }"), "ADC   #$01"));
}

TEST_CASE("compile: prefix -- subtracts one") {
  CHECK(suite::contains(codegen("int main(void) { int a = 5; --a; return a; }"), "SBC   #$01"));
}

TEST_CASE("compile: a compound assignment adds its constant") {
  const std::string out = codegen("int main(void) { int a = 5; a += 3; return a; }");
  CHECK(suite::contains(out, "LDA   #$05"));
  CHECK(suite::contains(out, "ADC   #$03"));
}

TEST_CASE("compile: a file-scope static is a labeled DC.W read absolutely") {
  const std::string out = codegen("static int g = 7; int main(void) { return g; }");
  CHECK(suite::contains(out, "g:"));
  CHECK(suite::contains(out, "DC.W  $0007"));
  CHECK(suite::contains(out, "LDA   g"));
}

TEST_CASE("compile: a block-scope static is zeroed under its unique name") {
  // identifier_resolution renames the object `@N.x`.
  const std::string out = codegen("int main(void) { static int x; return x; }");
  CHECK(suite::contains(out, "DS.B  2"));
  CHECK(suite::contains(out, "@0.x:"));
  CHECK(suite::contains(out, "LDA   @0.x"));
}

TEST_CASE("compile: a file-scope tentative definition is zeroed") {
  const std::string out = codegen("int x; int main(void) { return x; }");
  CHECK(suite::contains(out, "x:"));
  CHECK(suite::contains(out, "DS.B  2"));
  CHECK(suite::contains(out, "LDA   x"));
}

TEST_CASE("compile: an extern without an initializer defines nothing") {
  // The definition is in another unit.
  const std::string out = codegen("extern int x; int main(void) { return x; }");
  CHECK(suite::contains(out, "LDA   x"));
  CHECK_FALSE(suite::contains(out, "x:\n   DC.B"));
}

TEST_CASE("compile: a static 2-D array's initializer is laid down in source order") {
  const std::string out = codegen(
      "int main(void) { static int nested[3][2] = {{1,2},{3,4},{5,6}}; "
      "return nested[1][1]; }");
  const auto section = section_from(out, "@0.nested:");
  REQUIRE(section.size() >= 7);
  CHECK(std::vector<std::string>(section.begin(), section.begin() + 7) ==
        std::vector<std::string>{"@0.nested:", "   DC.W  $0001", "   DC.W  $0002",
                                 "   DC.W  $0003", "   DC.W  $0004", "   DC.W  $0005",
                                 "   DC.W  $0006"});
}

TEST_CASE("compile: a static array's missing elements are one zero run") {
  const std::string out =
      codegen("int main(void) { static int a[5] = {1, 2}; return a[2]; }");
  const auto section = section_from(out, "@0.a:");
  REQUIRE(section.size() >= 4);
  CHECK(std::vector<std::string>(section.begin() + 1, section.begin() + 4) ==
        std::vector<std::string>{"   DC.W  $0001", "   DC.W  $0002", "   DS.B  6"});
}

TEST_CASE("compile: a static 2-D array's separate holes are separate zero runs") {
  // a[0][1] is missing (4 bytes), and so is the row a[2] (8 bytes). The
  // row {200, 300} lies between them.
  const std::string out =
      codegen("int main(void) { static long a[3][2] = {{100}, {200, 300}}; return 0; }");
  const auto section = section_from(out, "@0.a:");
  REQUIRE(section.size() >= 6);
  CHECK(std::vector<std::string>(section.begin() + 1, section.begin() + 6) ==
        std::vector<std::string>{"   DC.L  $00000064", "   DS.B  4", "   DC.L  $000000C8",
                                 "   DC.L  $0000012C", "   DS.B  8"});
}

TEST_CASE("compile: a file-scope long array's initializer is DC.L directives") {
  const std::string out =
      codegen("long a[3] = {1, 2, 3}; int main(void) { return (int)a[0]; }");
  const auto section = section_from(out, "a:");
  REQUIRE(section.size() >= 4);
  CHECK(std::vector<std::string>(section.begin() + 1, section.begin() + 4) ==
        std::vector<std::string>{"   DC.L  $00000001", "   DC.L  $00000002",
                                 "   DC.L  $00000003"});
}

TEST_CASE("compile: the preprocessor strips comments") {
  CHECK(suite::contains(codegen("/* hello */ int main(void) { return 1; } // bye"),
                        "LDA   #$01"));
}

TEST_CASE("compile: -S -o writes the assembly to the file") {
  const suite::TempDir tmp;
  const std::string out_path = tmp.file("hello.asm");
  const Run r = run({"compile.py", "-", "-S", "-o", out_path}, "int main(void) { return 7; }");
  CHECK(r.rc == 0);
  CHECK(suite::contains(suite::read_file(out_path), "LDA   #$07"));
}

TEST_CASE("compile: -S output must end in .s or .asm") {
  const Run r = run({"compile.py", "-", "-S", "-o", "out.txt"}, kSource);
  CHECK(r.rc == 2);
  CHECK(suite::contains(r.err, ".s or .asm suffix"));
}

TEST_CASE("compile: the other stages' outputs can have any suffix") {
  const suite::TempDir tmp;
  const std::string out_path = tmp.file("tokens.txt");
  const Run r = run({"compile.py", "-", "--lex", "-o", out_path}, kSource);
  CHECK(r.rc == 0);
  CHECK(suite::contains(suite::read_file(out_path), "keyword\tint"));
}

TEST_CASE("compile: the stage flags are mutually exclusive") {
  const auto o = suite::compile_main({"compile.py", "-", "--lex", "--parse"}, kSource);
  CHECK(o.exited);
  CHECK(o.status == 2);
}

TEST_CASE("compile: an input is required") {
  const auto o = suite::compile_main({"compile.py", "-S"}, kSource);
  CHECK(o.exited);
  CHECK(o.status == 2);
}

TEST_CASE("compile: the input can be a file") {
  const suite::TempDir tmp;
  const std::string in_path = tmp.file("hello.c");
  suite::write_file(in_path, kSource);
  const Run r = run({"compile.py", in_path, "-S", "-o", "-"});
  CHECK(r.rc == 0);
  CHECK(suite::contains(r.out, "LDA   #$2A"));
}

TEST_CASE("compile: -S names the assembly after the source, in the current directory") {
  // As gcc does, `-S` on `src/hello.c` writes `hello.s` in the current
  // directory.
  const suite::TempDir tmp;
  std::filesystem::create_directory(tmp.path() / "src");
  const std::string src = tmp.file("src/hello.c");
  suite::write_file(src, kSource);
  Run r;
  {
    const suite::ChangeDir in_tmp(tmp.path());
    r = run({"compile.py", src, "-S"});
  }
  CHECK(r.rc == 0);
  CHECK(r.out == "");
  CHECK(suite::contains(suite::read_file(tmp.file("hello.s")), "LDA   #$2A"));
}

TEST_CASE("compile: -D passes a macro to the preprocessor") {
  const Run r = run({"compile.py", "-", "-S", "-D", "MAX=42"}, "int main(void) { return MAX; }");
  CHECK(r.rc == 0);
  CHECK(suite::contains(r.out, "LDA   #$2A"));
}

TEST_CASE("compile: -D without a value defines the macro as 1") {
  const Run r = run({"compile.py", "-", "-S", "-D", "FOO"}, "int main(void) { return FOO; }");
  CHECK(r.rc == 0);
  CHECK(suite::contains(r.out, "LDA   #$01"));
}

TEST_CASE("compile: __PCPP_VERSION__ is predefined") {
  // The macro holds pcpp's version string ("1.30"), which the parser
  // would reject as a numeric expression, so only `#ifdef` tests it.
  const Run r = run({"compile.py", "-", "-S"},
                    "#ifdef __PCPP_VERSION__\n"
                    "int main(void) { return 99; }\n"
                    "#else\n"
                    "int main(void) { return 0; }\n"
                    "#endif\n");
  CHECK(r.rc == 0);
  CHECK(suite::contains(r.out, "LDA   #$63"));
}

TEST_CASE("compile: a flag the preprocessor doesn't know is reported and ignored") {
  const Run r = run({"compile.py", "-", "-S", "--no-such-pcpp-flag"}, kSource);
  CHECK(r.rc == 0);
  CHECK(suite::contains(r.err, "--no-such-pcpp-flag"));
  CHECK(suite::contains(r.err, "not known"));
}

// --- void ---

TEST_CASE("compile: a void function with an empty body is a bare RTS") {
  // Falling off the end is legal (C99 §6.9.1.12). A void `Ret` doesn't
  // save A, so there's no PHA or PLA.
  const std::string out = codegen("void f(void) { }");
  for (const char* needle : {"f:", "SUBROUTINE", "RTS"}) {
    INFO(needle);
    CHECK(suite::contains(out, needle));
  }
  CHECK_FALSE(suite::contains(out, "PHA"));
  CHECK_FALSE(suite::contains(out, "PLA"));
}

TEST_CASE("compile: a void function's bare return is an RTS") {
  const std::string out = codegen("void f(void) { return; }");
  CHECK(suite::contains(out, "f:"));
  CHECK(suite::contains(out, "RTS"));
}

TEST_CASE("compile: a call of a void function captures no result") {
  // The line after `JSR f` stages `return 0` with an immediate load,
  // where a capture of the call's result would store A or HARGS in a
  // frame slot. `return 0` from `int main` stages both bytes in
  // registers (`LDX #$00` high, `LDA #$00` low).
  const std::string out = codegen("void f(void) { } int main(void) { f(); return 0; }");
  CHECK(suite::contains(out, "JSR   f"));
  std::vector<std::string> lines;
  for (const std::string& ln : suite::lines(out)) {
    const std::string s = strip(ln);
    if (!s.empty() && s[0] != ';') lines.push_back(s);
  }
  const auto it = std::find(lines.begin(), lines.end(), "JSR   f");
  REQUIRE(it != lines.end());
  REQUIRE(it + 1 != lines.end());
  CHECK((it[1] == "LDA   #$00" || it[1] == "LDX   #$00"));
}

TEST_CASE("compile: a cast to void still evaluates its operand") {
  const std::string out = codegen("int main(void) { (void)(1+2); return 0; }");
  CHECK(suite::contains(out, "LDA   #$01"));
  CHECK(suite::contains(out, "ADC   #$02"));
}

TEST_CASE("compile: a conditional of two void calls copies no result") {
  const std::string out = codegen(
      "void f(void); void g(void); int main(int flag) { flag ? f() : g(); return 0; }");
  for (const char* needle : {"JSR   f", "JSR   g", ".cond_else@", ".cond_end@"}) {
    INFO(needle);
    CHECK(suite::contains(out, needle));
  }
}

TEST_CASE("compile: void * converts to and from another pointer without a helper") {
  // Both pointers are 2 bytes, so each conversion is a 2-byte copy.
  const std::string out = codegen(
      "int main(void) { void *p; int *ip; ip = (int*)0; p = ip; ip = p; return 0; }");
  CHECK_FALSE(suite::contains(out, "JSR   mul"));
}

TEST_CASE("compile: a null pointer constant can be assigned to void *") {
  // The integer 0 is a null pointer constant (C99 §6.3.2.3.3).
  CHECK(suite::contains(codegen("int main(void) { void *p = 0; return p == 0; }"),
                        "LDA   #$00"));
}

TEST_CASE("compile: void calls work in a for's init and post") {
  const std::string out = codegen(
      "void f(void); void g(void); int main(void) { for (f(); 0; g()) {} return 0; }");
  CHECK(suite::contains(out, "JSR   f"));
  CHECK(suite::contains(out, "JSR   g"));
}

TEST_CASE("compile: a void function can't return a value") {
  CHECK(suite::contains(expect_failure("void f(void) { return 1; }"), "void"));
}

TEST_CASE("compile: a non-void function can't return without a value") {
  CHECK(suite::contains(expect_failure("int f(void) { return; }"), "return"));
}

TEST_CASE("compile: an object can't be void") {
  CHECK(suite::contains(expect_failure("int main(void) { void x; return 0; }"), "Void"));
}

TEST_CASE("compile: a named parameter can't be void") {
  // `int f(void)`, with no name, is the empty parameter list.
  CHECK(suite::contains(lower(expect_failure("int f(void x) { return 0; }")), "void"));
}

TEST_CASE("compile: a void value can't be an operand") {
  CHECK(suite::contains(
      expect_failure("void f(void); int main(void) { f() + 1; return 0; }"), "Void"));
}

TEST_CASE("compile: a void value can't be an argument") {
  CHECK(suite::contains(
      lower(expect_failure("void f(void); void g(int); int main(void) { g(f()); return 0; }")),
      "void"));
}

TEST_CASE("compile: arithmetic on void * is rejected") {
  // sizeof(void) is undefined.
  CHECK(suite::contains(lower(expect_failure("int main(void) { void *p; p + 1; return 0; }")),
                        "void"));
}

TEST_CASE("compile: pointers to different types still can't be assigned") {
  // void * must not loosen the rule that the pointed-to types match.
  CHECK(suite::contains(
      lower(expect_failure("int main(void) { int *ip; long *lp; ip = lp; return 0; }")),
      "pointer"));
}

// --- sizeof ---

TEST_CASE("compile: sizeof gives c6502's size of each scalar type") {
  const std::string out = tac(
      "int main(void) { "
      "long sizes = sizeof(int) + sizeof(long) + sizeof(long long) "
      "+ sizeof(char) + sizeof(float) + sizeof(double) "
      "+ sizeof(int *); "
      "return 0; }");
  for (const int size : {1, 2, 4, 1, 4, 8, 2}) {
    INFO(size);
    CHECK(suite::contains(out, "value=" + std::to_string(size)));
  }
}

TEST_CASE("compile: sizeof's array operand doesn't decay") {
  // C99 §6.3.2.1.3. The array is 10 bytes, where a pointer would be 2.
  CHECK(suite::contains(tac("int main(void) { char a[10]; return (int)sizeof a; }"),
                        "value=10"));
}

TEST_CASE("compile: sizeof's string literal operand doesn't decay") {
  // Three characters and the NUL.
  CHECK(suite::contains(tac("int main(void) { return (int)sizeof \"abc\"; }"), "value=4"));
}

TEST_CASE("compile: sizeof of a 2-D array is the whole array") {
  // 3 * 5 * 4, since a long is 4 bytes.
  CHECK(suite::contains(tac("int main(void) { long a[3][5]; return (int)sizeof a; }"),
                        "value=60"));
}

TEST_CASE("compile: sizeof doesn't evaluate its operand") {
  // `i++` would lower to a Binary(Add) into a temporary.
  const std::string out =
      tac("int main(void) { int i = 0; long s = sizeof (i++); return i; }");
  CHECK(suite::contains(out, "value=2"));
  CHECK_FALSE(suite::contains(out, "Add("));
}

TEST_CASE("compile: sizeof doesn't call its operand") {
  CHECK_FALSE(suite::contains(
      codegen("int foo(void); int main(void) { long s = sizeof foo(); return 0; }"),
      "JSR   foo"));
}

TEST_CASE("compile: sizeof's type is unsigned int") {
  // An unsigned int converts to an int with no change of width, and to a
  // long with a ZeroExtend.
  const std::string out =
      tac("int main(void) { long l = sizeof(int); int i = sizeof(int); return 0; }");
  CHECK(suite::contains(out, "ZeroExtend("));
  CHECK_FALSE(suite::contains(out, "Truncate("));
}

TEST_CASE("compile: sizeof of sizeof is 2") {
  CHECK(suite::contains(tac("int main(void) { return (int)sizeof sizeof(int); }"), "value=2"));
}

TEST_CASE("compile: sizeof of a pointer type is 2 whatever it points to") {
  CHECK(suite::contains(tac("int main(void) { return (int)sizeof(int (*)[100]); }"),
                        "value=2"));
}

TEST_CASE("compile: sizeof of a nested array type multiplies the sizes") {
  // 3 * 6 * 17 * 9 bytes.
  CHECK(suite::contains(tac("int main(void) { return (int)sizeof(char[3][6][17][9]); }"),
                        "value=2754"));
}

TEST_CASE("compile: sizeof(void) is rejected") {
  CHECK(suite::contains(lower(expect_failure("int main(void) { return sizeof(void); }")),
                        "void"));
}

TEST_CASE("compile: sizeof of a void expression is rejected") {
  CHECK(suite::contains(
      lower(expect_failure("void f(void); int main(void) { return sizeof f(); }")), "void"));
}

TEST_CASE("compile: sizeof of a function type is rejected") {
  // `(int(int))` is the type name of a function type.
  CHECK(suite::contains(lower(expect_failure("int main(void) { return sizeof(int(int)); }")),
                        "function"));
}

TEST_CASE("compile: sizeof of a type can be a case label") {
  // sizeof folds, so it's an integer constant expression (C99 §6.6p6).
  // The labels are 1, 4, and 10.
  const std::string out = codegen(
      "int main(int x) { "
      "switch (x) { "
      "case sizeof(char): return 1; "
      "case sizeof(long): return 2; "
      "case sizeof(int[5]): return 5; "
      "default: return 0; "
      "} }");
  for (const char* needle : {"CMP   #$01", "CMP   #$04", "CMP   #$0A"}) {
    INFO(needle);
    CHECK(suite::contains(out, needle));
  }
}

TEST_CASE("compile: sizeof of an expression can be a case label") {
  // The type checker gives the operand its type, so the constant
  // evaluator can fold it.
  const std::string out = codegen(
      "int main(int x) { "
      "long y; long long z; "
      "switch (x) { "
      "case sizeof y: return 2; "
      "case sizeof z: return 4; "
      "default: return 0; "
      "} }");
  CHECK(suite::contains(out, "CMP   #$04"));
  CHECK(suite::contains(out, "CMP   #$08"));
}

TEST_CASE("compile: case labels that sizeof makes equal are duplicates") {
  // `y` and `y+y` are both long, so both labels are 4.
  CHECK(suite::contains(lower(expect_failure("int main(int x) { "
                                             "long y; "
                                             "switch (x) { "
                                             "case sizeof y: return 1; "
                                             "case sizeof(y+y): return 2; "
                                             "default: return 0; "
                                             "} }")),
                        "duplicate"));
}

// --- -O ---

TEST_CASE("compile: -O puts a register-allocated local in zero page") {
  // Two locals that depend on a parameter keep constant folding from
  // removing them. The EQU binds each in the caller-saved range, so dasm
  // encodes the loads as zero-page ones.
  const Run r = run({"compile.py", "-", "-S", "-O"},
                    "int main(int p) { int a = p + 1; int b = a + p; return b; }");
  CHECK(r.rc == 0);
  CHECK(suite::search(R"(LDA\s+__local_main__\w+)", r.out));
  CHECK(suite::search(R"(__local_main__\w+\s+EQU\s+\$[89AB][0-9A-F])", r.out));
}

TEST_CASE("compile: without -O the locals stay in the frame") {
  // A static could be read absolutely too, but by its symbol, not by a
  // literal zero-page address.
  const Run r = run({"compile.py", "-", "-S"},
                    "int main(int p) { int a = p + 1; int b = a + p; return b; }");
  CHECK(r.rc == 0);
  CHECK(suite::contains(r.out, "(FP),Y"));
  CHECK_FALSE(suite::search(R"(LDA\s+\$[89A-F][0-9A-F])", r.out));
}

TEST_CASE("compile: -O gives an eligible function's parameter a zero-page slot") {
  const Run r = run({"compile.py", "-", "-S", "-O"}, "int main(int p) { return p + 1; }");
  CHECK(r.rc == 0);
  CHECK(suite::contains(r.out, "__zpabi_main__p_0"));
  CHECK_FALSE(suite::contains(r.out, "(FP),Y"));
}

TEST_CASE("compile: -O leaves a recursive function's parameter in the frame") {
  // A recursive call would overwrite the outer activation's slots, so
  // the function falls back to the soft stack.
  const Run r = run({"compile.py", "-", "-S", "-O"},
                    "int r(int x) { return x ? r(x - 1) : 0; } int main(void) { return r(3); }");
  CHECK(r.rc == 0);
  CHECK(suite::contains(r.out, "(FP),Y"));
}

TEST_CASE("compile: -O compiles representative programs") {
  const std::vector<std::string> sources = {
      "int main(int p) { int a = p + 1; int b = a + p; return b; }",
      "int add(int a, int b) { return a + b; } int main(void) { return add(3, 4); }",
      "long main(void) { long x = 100; long y = 200; return x + y; }",
      "int main(int p) { if (p) return 1; else return 2; }",
      "static int g = 7; int main(void) { return g; }",
  };
  for (const std::string& src : sources) {
    INFO(src);
    const Run r = run({"compile.py", "-", "-S", "-O"}, src);
    CHECK(r.rc == 0);
    CHECK(suite::contains(r.out, "main:"));
    CHECK(suite::contains(r.out, "RTS"));
  }
}

TEST_CASE("compile: -O collapses a function with no frame to a bare Return") {
  // With no arguments, locals, or callee-saved bytes, prologue
  // synthesis leaves the `Return` atoms as they are.
  const asm_ast::Program prog = compile_to_asm("int main(void) { return 0; }", true);
  std::vector<const asm_ast::Function*> mains;
  for (const auto& tl : prog.top_level) {
    if (const auto* fn = std::get_if<asm_ast::Function>(&tl); fn && fn->name == "main") {
      mains.push_back(fn);
    }
  }
  REQUIRE(mains.size() == 1);
  const auto& instrs = mains[0]->instructions;
  CHECK(std::none_of(instrs.begin(), instrs.end(), [](const asm_ast::Type_instruction& i) {
    return c6502::asdl::isa<asm_ast::FunctionPrologue>(i);
  }));
  CHECK(std::none_of(instrs.begin(), instrs.end(), [](const asm_ast::Type_instruction& i) {
    return c6502::asdl::isa<asm_ast::Ret>(i);
  }));
  REQUIRE_FALSE(instrs.empty());
  CHECK(c6502::asdl::isa<asm_ast::Return>(instrs.back()));
}
