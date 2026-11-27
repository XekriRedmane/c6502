// The `-S -O` text of tests/test_short_circuit_jump_fold.py's `&&` and
// `||` consumed by an `if`, which leave no 0-or-1 value. The
// simulator's runs are in test_short_circuit_jump_fold_sim.cpp.
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

TEST_CASE("short_circuit_jump_fold: an && in an if collapses to a chain of branches") {
  const std::string body = function_text(suite::codegen(
                                             "#include <stdint.h>\n"
                                             "int f(uint8_t a) {\n"
                                             "    if (a >= 0x40 && a < 0x47) return 1;\n"
                                             "    return 2;\n"
                                             "}\n"
                                             "int main(void) { return f(0x44); }\n",
                                             true),
                                         "f");
  CHECK_FALSE(suite::contains(body, ".and_false"));
  CHECK_FALSE(suite::contains(body, ".and_end"));
}

TEST_CASE("short_circuit_jump_fold: an || in an if collapses to a chain of branches") {
  const std::string body = function_text(suite::codegen(
                                             "#include <stdint.h>\n"
                                             "int f(uint8_t a) {\n"
                                             "    if (a < 0x40 || a >= 0x50) return 1;\n"
                                             "    return 2;\n"
                                             "}\n"
                                             "int main(void) { return f(0x44); }\n",
                                             true),
                                         "f");
  CHECK_FALSE(suite::contains(body, ".or_true"));
  CHECK_FALSE(suite::contains(body, ".or_end"));
}
