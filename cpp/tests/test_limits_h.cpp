// A `limits.h` in a directory given with `-I` comes before the bundled
// one, from tests/test_limits_h.py.
#include <string>
#include <vector>

#include "compile_case.h"
#include "doctest.h"

namespace suite = c6502::suite;

TEST_CASE("limits_h: a user include path shadows the bundled header") {
  const suite::TempDir tmp;
  suite::write_file(tmp.file("limits.h"), "#define INT_MAX 99\n");
  suite::Options options;
  options.pcpp_args = {"-I", tmp.path().generic_string()};
  // The tokens `compile.py --lex` prints for the preprocessor's output.
  const std::string out =
      suite::run_source(suite::Stage::kLex, "#include <limits.h>\nint x = INT_MAX;\n", options);
  std::vector<std::string> values;
  for (const std::string& line : suite::lines(out)) {
    std::size_t at = line.find('\t');
    REQUIRE(at != std::string::npos);
    at = line.find('\t', at + 1);
    REQUIRE(at != std::string::npos);
    const std::size_t end = line.find('\t', at + 1);
    values.push_back(line.substr(at + 1, end == std::string::npos ? end : end - at - 1));
  }
  CHECK(values == std::vector<std::string>{"int", "x", "=", "99", ";"});
}
