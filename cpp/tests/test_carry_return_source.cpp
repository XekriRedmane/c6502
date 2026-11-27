// The tests of tests/test_carry_return.py that compile a source:
// `[[c6502::return_bool_in_carry]]` on a `bool` function and on a
// struct of a value and a `bool`, what the type checker rejects, and the
// code of the callee and the caller.
#include <sstream>
#include <string>
#include <vector>

#include "compile_case.h"
#include "doctest.h"

namespace {

namespace suite = c6502::suite;

const std::string kCarry = "[[c6502::return_bool_in_carry]] ";
const std::string kBool = "#include <stdbool.h>\n";
const std::string kZc = "struct zc { unsigned char v; bool end; };\n";
const std::string kWv = "struct wv { int v; bool end; };\n";

// Returns Python's `text.split(sep)[1]`, the text between the first
// `sep` and the next.
std::string piece_after(const std::string& text, const std::string& sep) {
  const std::size_t at = text.find(sep);
  REQUIRE(at != std::string::npos);
  const std::size_t start = at + sep.size();
  const std::size_t end = text.find(sep, start);
  return text.substr(start, end == std::string::npos ? std::string::npos : end - start);
}

// Returns Python's `text.split()`.
std::vector<std::string> words(const std::string& text) {
  std::istringstream in(text);
  std::vector<std::string> out;
  for (std::string w; in >> w;) out.push_back(w);
  return out;
}

// The paragraph after `name`'s SUBROUTINE line, the instructions of a
// function without a prologue comment.
std::string function_text(const std::string& asm_text, const std::string& name) {
  return piece_after(piece_after(piece_after(asm_text, "\n" + name + ":"), "SUBROUTINE"), "\n\n");
}

// The options of compile.py's `_run_stage`, which takes the source as
// the preprocessor's output.
suite::Options unpreprocessed() {
  suite::Options o;
  o.preprocess = false;
  return o;
}

void check_rejected(const std::string& src, const std::string& pattern) {
  const suite::Error e = suite::compile_error(suite::Stage::kTac, src, unpreprocessed());
  CHECK(suite::is_a(e.type, "TypeCheckError"));
  CHECK_MESSAGE(suite::search(pattern, e.message), e.message);
}

}  // namespace

TEST_CASE("carry_return: a non-bool return type is rejected") {
  for (const std::string ret : {"int", "unsigned char", "char"}) {
    INFO(ret);
    check_rejected(kCarry + ret + " f(void) { return 1; }", "return type");
  }
}

TEST_CASE("carry_return: the declarations must agree") {
  check_rejected("bool f(void);\n" + kCarry + "bool f(void) { return 1; }", "must agree");
}

TEST_CASE("carry_return: the function's address can't be taken") {
  check_rejected(kCarry + "bool f(void) { return 1; }\nbool (*p)(void) = f;", "address");
}

TEST_CASE("carry_return: a constant return is SEC or CLC") {
  const std::string asm_text =
      suite::codegen(kBool + kCarry + "bool yes(void) { return 1; }\n" + kCarry +
                         "bool no(void) { return 0; }\n"
                         "int main(void) { return yes() + no(); }\n",
                     true);
  CHECK(words(function_text(asm_text, "yes")) == std::vector<std::string>{"SEC", "RTS"});
  CHECK(words(function_text(asm_text, "no")) == std::vector<std::string>{"CLC", "RTS"});
}

TEST_CASE("carry_return: the caller branches on the carry") {
  const std::string asm_text = suite::codegen(kBool + "unsigned char g;\n" + kCarry +
                                                  "bool f(unsigned char x) { return x == 3; }\n"
                                                  "int main(void) { if (f(g)) g = 7; return g; }\n",
                                              true);
  const std::string main_text = piece_after(asm_text, "\nmain:");
  CHECK(suite::search("JSR   f\\n   BC[CS]   ", main_text));
  CHECK_FALSE(suite::contains(main_text, "ROL"));
}

TEST_CASE("carry_return: the soft-stack epilogue keeps the carry") {
  const std::string asm_text = suite::codegen(kBool + kCarry +
                                                  "bool f(unsigned char n) { if (n == 0) return 1; "
                                                  "return f(n - 1); }\n"
                                                  "int main(void) { return f(3); }\n",
                                              true);
  std::string f = piece_after(asm_text, "\nf:");
  f = f.substr(0, f.find("\nmain:"));
  CHECK(suite::contains(f, "PHP"));
  CHECK(suite::contains(f, "PLP"));
}

TEST_CASE("carry_return: a struct of a value and a flag is accepted") {
  for (const std::string value : {"unsigned char", "signed char", "char", "int", "unsigned int"}) {
    INFO(value);
    CHECK_NOTHROW(suite::run_source(suite::Stage::kTac,
                                    "struct s { " + value + " v; bool end; };\n" + kCarry +
                                        "struct s f(void) { struct s r; r.v = 1; "
                                        "r.end = 0; return r; }",
                                    unpreprocessed()));
  }
}

TEST_CASE("carry_return: a struct of another shape is rejected") {
  const std::vector<std::pair<std::string, std::string>> shapes = {
      {"three members", "unsigned char v; bool end; unsigned char x;"},
      {"one member", "unsigned char v;"},
      {"pointer value", "unsigned char *v; bool end;"},
      {"long value", "long v; bool end;"},
      {"int flag", "unsigned char v; int end;"},
      {"unsigned char flag", "unsigned char v; unsigned char end;"},
      {"char flag", "unsigned char v; char end;"},
      {"flag first", "bool end; int v;"},
  };
  for (const auto& [label, members] : shapes) {
    INFO(label);
    check_rejected("struct s { " + members + " };\n" + kCarry + "struct s f(void);",
                   "exactly two members");
  }
}

TEST_CASE("carry_return: a union is rejected") {
  check_rejected("union u { unsigned char v; bool end; };\n" + kCarry + "union u f(void);",
                 "exactly two members");
}

TEST_CASE("carry_return: a struct in the carry has no hidden pointer") {
  const std::string tac =
      suite::run_source(suite::Stage::kTac,
                        kZc + kCarry +
                            "struct zc f(void) { struct zc r; r.v = 3; r.end = 1; return r; }\n"
                            "int main(void) { struct zc c = f(); return c.v; }\n",
                        unpreprocessed());
  CHECK_FALSE(suite::contains(tac, ".sret"));
}

TEST_CASE("carry_return: the callee sets C and then loads the value") {
  const std::string asm_text =
      suite::codegen(kWv + "struct wv g;\n" + kCarry +
                         "struct wv f(void) { return g; }\n"
                         "int main(void) { struct wv r = f(); return r.v; }\n",
                     true);
  CHECK(words(function_text(asm_text, "f")) ==
        std::vector<std::string>{"LDA", "g+2", "CMP", "#$01", "LDX", "g+1", "LDA", "g", "RTS"});
}

TEST_CASE("carry_return: the caller branches on C after storing the value") {
  // `out = c.v;` before `if (c.end)` used to store the captured flag and
  // load it back.
  const std::string asm_text = suite::codegen(kZc + "unsigned char out;\n" + kCarry +
                                                  "struct zc f(void);\n"
                                                  "int main(void) {\n"
                                                  "    struct zc c = f();\n"
                                                  "    out = c.v;\n"
                                                  "    if (c.end) return 1;\n"
                                                  "    return 2;\n"
                                                  "}\n",
                                              true);
  const std::string main_text = function_text(asm_text, "main");
  CHECK_FALSE(suite::contains(main_text, "ROL"));
  CHECK(suite::search("JSR\\s+f\\s+STA\\s+out\\s+BC[CS]", main_text));
}

TEST_CASE("carry_return: a function returning a struct in the carry is zp_abi eligible") {
  // No hidden pointer, so `f` gets zp_abi under -O and the call passes
  // nothing on the soft stack.
  const std::string asm_text =
      suite::codegen(kZc + kCarry +
                         "struct zc f(unsigned char x) { struct zc r; r.v = x; "
                         "r.end = x == 3; return r; }\n"
                         "int main(void) { struct zc c = f(3); return c.v + c.end; }\n",
                     true);
  CHECK(suite::contains(asm_text, "__zpabi_f__x"));
  CHECK_FALSE(suite::contains(function_text(asm_text, "main"), "SSP"));
}
