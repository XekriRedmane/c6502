// The `-S -O` text of tests/test_in_place_member_update.py's updates of
// a member of a static object computed in its own bytes
// (cpp/src/passes/in_place_member_update.h).
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "compile_case.h"
#include "doctest.h"

namespace {

namespace suite = c6502::suite;

using Words = std::vector<std::string>;

constexpr const char* kPc = R"(
#include <stdint.h>
typedef struct { uint16_t lo; uint8_t hi; } uint24_t;
uint24_t P;
uint16_t W;
uint8_t flag;
uint16_t t[4];
)";

// Returns `text.split(sep)[n]`, and throws when `text` has fewer
// pieces.
std::string piece(const std::string& text, const std::string& sep, std::size_t n) {
  std::size_t start = 0;
  for (std::size_t k = 0; k < n; ++k) {
    const std::size_t at = text.find(sep, start);
    if (at == std::string::npos) throw std::runtime_error("no piece " + std::to_string(n));
    start = at + sep.size();
  }
  const std::size_t end = text.find(sep, start);
  return text.substr(start, end == std::string::npos ? std::string::npos : end - start);
}

// Returns the `-S -O` text of function `fn` of `kPc` and `body`, from
// after its `SUBROUTINE` through its first `RTS`.
std::string function_text(const std::string& body, const std::string& fn) {
  const std::string text = suite::codegen(std::string(kPc) + body, true);
  return piece(piece(piece(text, "\n" + fn + ":", 1), "SUBROUTINE", 1), "RTS", 0) + "RTS";
}

Words words(const std::string& text) {
  std::istringstream in(text);
  Words out;
  for (std::string w; in >> w;) out.push_back(w);
  return out;
}

}  // namespace

TEST_CASE("in_place_member_update: an increment of a member is INC") {
  CHECK(words(function_text("void f(void) { P.lo++; }", "f")) ==
        Words{"INC", "P", "BNE", ".inc_done@0", "INC", "P+1", ".inc_done@0:", "RTS"});
}

TEST_CASE("in_place_member_update: an increment after an indexed read is INC") {
  const std::string body = function_text(
      "uint8_t *A;\n"
      "uint8_t f(void) { uint8_t b = A[(uint8_t)P.lo]; P.lo++; return b; }",
      "f");
  CHECK(suite::contains(body, "LDY   P\n"));
  CHECK(suite::contains(body, "INC   P\n"));
  CHECK_FALSE(suite::contains(body, "STA   P"));
}

TEST_CASE("in_place_member_update: a byte member and an array element are INC") {
  CHECK(words(function_text("void f(void) { P.hi++; }", "f")) == Words{"INC", "P+2", "RTS"});
  CHECK(suite::contains(function_text("void f(void) { t[2]++; }", "f"), "INC   t+4\n"));
}

TEST_CASE("in_place_member_update: a value is computed into the member") {
  CHECK(words(function_text("void f(void) { P.lo = W << 1; }", "f")) ==
        Words{"LDA", "W", "ASL", "STA", "P", "LDA", "W+1", "ROL", "STA", "P+1", "RTS"});
}
