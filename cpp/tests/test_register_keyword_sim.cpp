// The tests of tests/test_register_keyword_sim.py: functions with
// `register` parameters, a `register` local, and returns in A and X,
// run under `-O`; the caller of a function with `register` parameters
// loads the second byte into X; and the uses of `register` that the
// convention can't give are one-line errors.
#include <string>

#include "compile_case.h"
#include "doctest.h"
#include "sim_case.h"

namespace {

namespace suite = c6502::suite;

std::int64_t sim_return_int(const std::string& src) {
  const c6502::sim::SimResult r = suite::build_sim(src, suite::Pipeline::kOptimized).run(5'000'000);
  CHECK_FALSE(r.timed_out);
  return r.return_int_signed();
}

// Checks that `compile.py - -S -O` reports a problem with `src` in one
// line that names `needle`, with exit status 1.
void check_compile_error(const std::string& src, const std::string& needle) {
  const auto o = suite::compile_main({"c6502", "-", "-S", "-O"}, src);
  REQUIRE(o.status.has_value());
  CHECK(*o.status == 1);
  CHECK(o.err().rfind("<stdin>: error: ", 0) == 0);
  CHECK_MESSAGE(suite::contains(o.err(), needle), o.err());
}

}  // namespace

TEST_CASE("register_keyword_sim: the caller loads the second argument into X") {
  const auto o = suite::compile_main({"c6502", "-", "-S", "-O"},
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

TEST_CASE("register_keyword_sim: two register parameters in A and X") {
  CHECK(sim_return_int(R"c(char add2(register char a, register char b) {    return (char)(a + b);}int main(void) { return add2(30, 12); })c") == 42);
}

TEST_CASE("register_keyword_sim: a 2-byte register parameter spreads over A and X") {
  CHECK((sim_return_int(R"c(unsigned int echo(register unsigned int a) { return a; }int main(void) { return (int)echo(0x1234); })c") & 0xFFFF) == 0x1234);
}

TEST_CASE("register_keyword_sim: a 1-byte return in A") {
  CHECK(sim_return_int(R"c(char add_one(register char x) { return (char)(x + 1); }int main(void) { return add_one(41); })c") == 42);
}

TEST_CASE("register_keyword_sim: a 2-byte return in A and X") {
  CHECK((sim_return_int(R"c(unsigned int widen(register char x) {    return (unsigned int)((unsigned int)x << 8 | 0x07);}int main(void) { return (int)widen(0x12); })c") & 0xFFFF) == 0x1207);
}

TEST_CASE("register_keyword_sim: a register local is kept in Y") {
  // The `register` parameter `n` comes in A, so the function must stay
  // without a soft-stack frame, whose prologue would overwrite A.
  const std::string src = R"c(char sum_n(register char n) {    register char acc = 0;    while (n > 0) {        acc = (char)(acc + n);        n = (char)(n - 1);    }    return acc;}int main(void) { return sum_n(5); })c";
  CHECK(sim_return_int(src) == 5 + 4 + 3 + 2 + 1);
  const auto o = suite::compile_main({"c6502", "-", "-S", "-O"}, src);
  REQUIRE(o.status.has_value());
  REQUIRE(*o.status == 0);
  const std::string out = o.out();
  const std::size_t sum_n_at = out.find("sum_n:");
  const std::size_t main_at = out.find("main:");
  REQUIRE(sum_n_at != std::string::npos);
  REQUIRE(main_at != std::string::npos);
  const std::string sum_n_body = out.substr(sum_n_at, main_at - sum_n_at);
  CHECK_FALSE(suite::contains(sum_n_body, "__local_sum_n__acc"));
  CHECK(suite::search("\\b(TYA|TAY|INY|DEY)\\b", sum_n_body));
}
