// The tests of tests/test_register_keyword_sim.py that read compile.py's
// `-S -O` output or errors: the caller of a function with `register`
// parameters loads the second byte into X, and the uses of `register`
// that the convention can't give are one-line errors.
#include <string>

#include "compile_case.h"
#include "doctest.h"

namespace {

namespace suite = c6502::suite;

// Checks that `compile.py - -S -O` reports a problem with `src` in one
// line that names `needle`, with exit status 1.
void check_compile_error(const std::string& src, const std::string& needle) {
  const auto o = suite::compile_main({"compile.py", "-", "-S", "-O"}, src);
  REQUIRE(o.status.has_value());
  CHECK(*o.status == 1);
  CHECK(o.err().rfind("<stdin>: error: ", 0) == 0);
  CHECK_MESSAGE(suite::contains(o.err(), needle), o.err());
}

}  // namespace

TEST_CASE("register_keyword_sim: the caller loads the second argument into X") {
  const auto o = suite::compile_main({"compile.py", "-", "-S", "-O"},
                                     "char add2(register char a, register char b) {"
                                     "    return (char)(a + b);"
                                     "}"
                                     "int main(void) { return add2(5, 7); }");
  REQUIRE(o.status.has_value());
  CHECK(*o.status == 0);
  const std::string out = o.out();
  const std::size_t main_at = out.find("main:");
  REQUIRE(main_at != std::string::npos);
  const std::string main_body = out.substr(main_at);
  CHECK(suite::search("LDX\\s+#\\$07", main_body));
  // The register arguments get no slot store at the call site.
  CHECK_FALSE(suite::search("STA\\s+__zpabi_add2__b", main_body));
}

TEST_CASE("register_keyword_sim: a register pointer parameter is rejected") {
  check_compile_error(
      "char deref(register char *p) { return *p; }"
      "int main(void) { char c = 7; return deref(&c); }",
      "pointer");
}

TEST_CASE("register_keyword_sim: a third register argument byte is rejected") {
  // Only A and X carry arguments.
  check_compile_error(
      "char add3("
      "    register char a, register char b, register char c) {"
      "    return (char)(a + b + c);"
      "}"
      "int main(void) { return add3(1, 2, 3); }",
      "register");
}

TEST_CASE("register_keyword_sim: a 2-byte register local is rejected") {
  // A `register` local gets only Y.
  check_compile_error(
      "int main(void) {"
      "    register int x = 7;"
      "    return x;"
      "}",
      "register");
}

TEST_CASE("register_keyword_sim: the address of a register parameter is rejected") {
  check_compile_error(
      "char fst(register char x) {"
      "    char *p = &x;"
      "    return *p;"
      "}"
      "int main(void) { return fst(5); }",
      "register");
}

TEST_CASE("register_keyword_sim: a register parameter of a recursive function is rejected") {
  // A `register` parameter requires zp_abi eligibility, which recursion
  // takes away.
  check_compile_error(
      "int fib(register int n) {"
      "    if (n < 2) return n;"
      "    return fib(n - 1) + fib(n - 2);"
      "}"
      "int main(void) { return fib(5); }",
      "register");
}
