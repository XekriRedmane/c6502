// The tests of tests/test_direct_member_access.py that read the `-S -O`
// text: a member store to a global is a plain `STA g+k`, and a local
// struct whose address doesn't escape has no pointer.
#include <stdexcept>
#include <string>

#include "compile_case.h"
#include "doctest.h"

namespace {

namespace suite = c6502::suite;

constexpr const char* kS = "struct s { unsigned char a; int b; unsigned char c; };\n";

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

// Returns the `-S -O` text of `f` in `source`, up to `main`.
std::string f_of(const std::string& source) {
  return piece(piece(suite::codegen(source, true), "\nf:", 1), "\nmain:", 0);
}

}  // namespace

TEST_CASE("direct_member_access: a member store to a global is absolute") {
  const std::string f = f_of(std::string(kS) +
                             "struct s g;\n"
                             "void f(unsigned char x) { g.c = x; }\n"
                             "int main(void) { f(1); return g.c; }\n");
  CHECK(suite::contains(f, "STA   g+3"));
  CHECK_FALSE(suite::contains(f, "("));
}

TEST_CASE("direct_member_access: both stores to a volatile element stay, in order") {
  const std::string f = f_of(
      "volatile unsigned char port[2];\n"
      "void f(void) { port[1] = 1; port[1] = 2; }\n"
      "int main(void) { f(); return 0; }\n");
  CHECK(suite::count(f, "STA   port+1") == 2);
  const std::size_t one = f.find("#$01");
  const std::size_t two = f.find("#$02");
  REQUIRE(one != std::string::npos);
  REQUIRE(two != std::string::npos);
  CHECK(one < two);
}

TEST_CASE("direct_member_access: a local struct has no pointer") {
  const std::string f = f_of(std::string(kS) +
                             "unsigned char f(unsigned char x) {\n"
                             "    struct s r;\n"
                             "    r.a = x; r.c = x + 1;\n"
                             "    return r.a + r.c;\n"
                             "}\n"
                             "int main(void) { return f(1); }\n");
  CHECK_FALSE(suite::contains(f, "#<"));
  CHECK_FALSE(suite::contains(f, "("));
}
