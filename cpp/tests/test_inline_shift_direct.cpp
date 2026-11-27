// The `-S -O` text of tests/test_inline_shift_direct.py's shifts by one,
// which shift a byte in memory where the value is shifted in place. The
// simulator runs stay in Python.
#include <sstream>
#include <string>
#include <vector>

#include "compile_case.h"
#include "doctest.h"

namespace {

namespace suite = c6502::suite;

using Words = std::vector<std::string>;

bool starts_with(const std::string& s, const std::string& prefix) {
  return s.rfind(prefix, 0) == 0;
}

// Returns the lines of function `name`, from its label to the next
// function's.
std::vector<std::string> function_lines(const std::string& text, const std::string& name) {
  std::vector<std::string> out;
  bool in_fn = false;
  for (const std::string& line : suite::lines(text)) {
    if (starts_with(line, name + ":")) in_fn = true;
    if (!in_fn) continue;
    if (!line.empty() && line[0] != ' ' && line[0] != '\t' && line[0] != '.' &&
        line.back() == ':' && !starts_with(line, name + ":")) {
      break;
    }
    out.push_back(line);
  }
  return out;
}

Words words(const std::string& text) {
  std::istringstream in(text);
  Words out;
  for (std::string w; in >> w;) out.push_back(w);
  return out;
}

}  // namespace

TEST_CASE("inline_shift_direct: a global shifted in place is a direct ASL") {
  const std::string text = suite::codegen(
      "int g;\n"
      "void f(void) { g <<= 1; }\n"
      "int main(void) { g = 50; f(); return g; }\n",
      true);
  // The accumulator's form is `ASL A`, or a bare `ASL`.
  bool direct = false;
  for (const std::string& line : function_lines(text, "f")) {
    const Words w = words(line);
    if (suite::contains(line, "ASL") && !suite::contains(w.back(), "A")) direct = true;
  }
  CHECK(direct);
}

TEST_CASE("inline_shift_direct: a shift into another byte runs in A") {
  // A parameter shifted into a local isn't in place, so each byte goes
  // through A: `LDA x; ASL; STA t`, not `LDA x; STA t; ASL t`
  // (cpp/src/passes/shift_in_a.h). `f` takes an unused `d` in A, so
  // that `x` arrives in its slot.
  const std::string text = suite::codegen(
      "int f(unsigned char d [[c6502::in_a]], int x) {\n"
      "    x <<= 1;\n"
      "    return x;\n"
      "}\n"
      "int main(void) { return f(0, 50); }\n",
      true);
  bool asl = false;
  bool rol = false;
  bool jsr = false;
  for (const std::string& line : function_lines(text, "f")) {
    const Words w = words(line);
    asl = asl || w == Words{"ASL"};
    rol = rol || w == Words{"ROL"};
    jsr = jsr || suite::contains(line, "JSR");
  }
  CHECK(asl);
  CHECK(rol);
  CHECK_FALSE(jsr);
}
