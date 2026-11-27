// The `-S -O` text of tests/test_lnot_jump_fold.py's `if (!a)`, which is
// one inverted branch with no 0-or-1 value. The runs stay in Python.
#include <string>

#include "compile_case.h"
#include "doctest.h"

namespace {

namespace suite = c6502::suite;

bool starts_with(const std::string& s, const std::string& prefix) {
  return s.rfind(prefix, 0) == 0;
}

// Returns the lines of function `name`, from its label to the next
// function's.
std::string function_text(const std::string& text, const std::string& name) {
  std::string out;
  bool in_fn = false;
  bool first = true;
  for (const std::string& line : suite::lines(text)) {
    if (starts_with(line, name + ":")) in_fn = true;
    if (!in_fn) continue;
    if (!line.empty() && line[0] != ' ' && line[0] != '\t' && line[0] != '.' &&
        line.back() == ':' && !starts_with(line, name + ":")) {
      break;
    }
    out += (first ? "" : "\n") + line;
    first = false;
  }
  return out;
}

}  // namespace

TEST_CASE("lnot_jump_fold: !a of a byte collapses to a single branch") {
  const std::string text = suite::codegen(
      "#include <stdint.h>\n"
      "int f(uint8_t a) { if (!a) return 1; return 2; }\n"
      "int main(void) { return f(0); }\n",
      true);
  const std::string body = function_text(text, "f");
  CHECK_FALSE(suite::contains(body, ".lnot_true"));
  CHECK_FALSE(suite::contains(body, ".lnot_end"));
}
