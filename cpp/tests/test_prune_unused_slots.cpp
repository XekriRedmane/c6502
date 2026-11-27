// The test of tests/test_prune_unused_slots.py that compiled a program
// with `-O`: `prune_unused_locals` (src/passes/prune_unused_slots.h)
// leaves an `EQU` for every `__local_*` slot the `-S` text names.
#include <regex>
#include <set>
#include <string>

#include "compile_case.h"
#include "doctest.h"

namespace {

namespace suite = c6502::suite;

// Returns the first group of each match of `pattern` in `text`.
std::set<std::string> matches(const std::string& pattern, const std::string& text) {
  const std::regex re(pattern);
  std::set<std::string> out;
  for (auto it = std::sregex_iterator(text.begin(), text.end(), re); it != std::sregex_iterator();
       ++it) {
    out.insert((*it)[1].str());
  }
  return out;
}

}  // namespace

TEST_CASE("prune_unused_slots: every local the text names has an EQU") {
  suite::Options options;
  options.optimize = true;
  options.preprocess = false;
  const std::string out = suite::run_source(suite::Stage::kCodegen,
                                            R"(
            int main(void) {
                int a[4] = {1, 2, 3, 4}; int i = 0;
                a[i++] += 10; a[i++] *= 3; a[--i] <<= 1;
                return a[0] + a[1] + i + a[2] - 1;
            }
        )",
                                            options);
  std::string body;
  bool first = true;
  for (const std::string& line : suite::lines(out)) {
    if (line.rfind(";", 0) == 0) continue;
    if (!first) body += "\n";
    body += line;
    first = false;
  }
  // `(?:^|\n)` stands for Python's `^` under re.M.
  const std::set<std::string> defined = matches(R"((?:^|\n)(__local_\w+)\s+EQU)", body);
  const std::set<std::string> used = matches(R"(\b(__local_\w+?)(?:\+\d+)?\b)", body);
  CHECK_FALSE(used.empty());
  std::string undefined;
  for (const std::string& name : used) {
    if (!defined.count(name)) undefined += " " + name;
  }
  CHECK_MESSAGE(undefined.empty(), "used without an EQU:" << undefined);
}
