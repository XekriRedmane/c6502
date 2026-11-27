// The `-S -O` text of tests/test_redundant_load_after_rmw.py's signed
// countdown loop.
#include <string>
#include <vector>

#include "compile_case.h"
#include "doctest.h"

namespace {

namespace suite = c6502::suite;

std::string strip(const std::string& s) {
  const std::size_t start = s.find_first_not_of(" \t\r\n");
  if (start == std::string::npos) return "";
  const std::size_t end = s.find_last_not_of(" \t\r\n");
  return s.substr(start, end - start + 1);
}

}  // namespace

TEST_CASE("redundant_load_after_rmw: a signed countdown's tail is DEC and BPL") {
  // The post-decrement of the `int8_t` is a 1-byte subtract that
  // `dec_peephole` makes a DEC, and the loop's test is a 1-byte test
  // against zero, so the load between the DEC and the BPL goes.
  const std::string text = suite::codegen(
      "#include <stdint.h>\n"
      "int sum;\n"
      "int main(void) {\n"
      "    for (int8_t x = 15; x >= 0; x--) sum += x;\n"
      "    return sum;\n"
      "}\n",
      true);
  INFO(text);
  const std::vector<std::string> lines = suite::lines(text);
  std::size_t dec = 0;
  while (dec < lines.size() && strip(lines[dec]).rfind("DEC   ", 0) != 0) ++dec;
  REQUIRE(dec < lines.size());
  std::size_t next = dec + 1;
  while (next < lines.size() && strip(lines[next]).empty()) ++next;
  REQUIRE(next < lines.size());
  CHECK(strip(lines[next]).rfind("BPL ", 0) == 0);
}
