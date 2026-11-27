// The tests of tests/test_leaf_zp_abi.py that read compile.py's `-S -O`
// output: a `[[c6502::zp_abi]]` leaf has no soft-stack frame, its
// callers write its arguments to its slots, and a zp_abi function on a
// call-graph cycle is rejected.
#include <string>

#include "compile_case.h"
#include "doctest.h"

namespace {

namespace suite = c6502::suite;

std::string compile_s(const std::string& src) {
  const auto o = suite::compile_main({"compile.py", "-", "-S", "-O"}, src);
  REQUIRE(o.status.has_value());
  CHECK_MESSAGE(*o.status == 0, o.err());
  return o.out();
}

const std::string kAdd =
    "[[c6502::zp_abi]] int add(int a, int b) { "
    "    return a + b; "
    "} "
    "int main(void) { return add(3, 4); }";

}  // namespace

TEST_CASE("leaf_zp_abi: a zp_abi leaf has no prologue") {
  const std::string out = compile_s(kAdd);
  const std::size_t add_at = out.find("add:");
  const std::size_t main_at = out.find("main:");
  REQUIRE(add_at != std::string::npos);
  REQUIRE(main_at != std::string::npos);
  const std::string add_body = out.substr(add_at, main_at - add_at);
  CHECK_FALSE(suite::contains(add_body, "STA   SSP"));
  CHECK_FALSE(suite::contains(add_body, "STA   FP"));
  CHECK(suite::contains(add_body, "RTS"));
}

TEST_CASE("leaf_zp_abi: the caller passes the arguments in registers and slots") {
  const std::string out = compile_s(kAdd);
  const std::size_t main_at = out.find("main:");
  REQUIRE(main_at != std::string::npos);
  const std::string main_body = out.substr(main_at);
  // `add`'s declaration names no register, so `a` arrives in A (low
  // byte) and X (high byte), and `b` in its slot bytes.
  CHECK(suite::contains(main_body, "LDA   #$03"));
  CHECK(suite::contains(main_body, "LDX   #$00"));
  CHECK_FALSE(suite::contains(main_body, "__zpabi_add__a"));
  CHECK(suite::contains(main_body, "STA   __zpabi_add__b_0"));
  CHECK(suite::contains(main_body, "STA   __zpabi_add__b_1"));
  CHECK_FALSE(suite::contains(main_body, "SSP"));
}

TEST_CASE("leaf_zp_abi: a zp_abi function on a cycle is rejected") {
  const std::string src =
      "[[c6502::zp_abi]] int rec(int x) { "
      "  return x ? rec(x - 1) : 0; "
      "} "
      "int main(void) { return rec(3); }";
  const auto o = suite::compile_main({"compile.py", "-", "-S", "-O"}, src);
  REQUIRE(o.status.has_value());
  CHECK(*o.status == 1);
  CHECK(suite::contains(o.err(), "rec"));
}

TEST_CASE("leaf_zp_abi: a call of an extern zp_abi function writes its slots") {
  // The annotation reaches the call site from the declaration. `base`
  // arrives in A and X, so `x` arrives in its slot.
  const std::string asm_text = compile_s(
      "[[c6502::zp_abi]] extern int helper(int base, int x); "
      "int main(void) { return helper(1, 7); }");
  CHECK(suite::contains(asm_text, "STA   __zpabi_helper__x_0"));
  CHECK(suite::contains(asm_text, "__zpabi_helper__x_0\tEQU\t$80"));
  CHECK_FALSE(suite::contains(asm_text, "STA   (SSP)"));
}
