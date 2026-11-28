#include <string>

#include "compile_case.h"
#include "doctest.h"
#include "sim_case.h"

namespace c6502::suite {

namespace {

std::string codegen_main(const std::string& source) {
  const auto o = compile_main({"c6502", "-", "-S", "-O"}, source);
  REQUIRE(o.status.value_or(-1) == 0);
  return o.out();
}

std::int64_t sim_return_int(const std::string& source) {
  const sim::SimResult r = build_sim(source, Pipeline::kOptimized).run(5'000'000);
  CHECK_FALSE(r.timed_out);
  return r.return_int_signed();
}

}  // namespace

TEST_CASE("leaf_zp_abi sim: a zp_abi leaf returns the right value") {
  CHECK(sim_return_int(R"c([[c6502::zp_abi]] int add(int a, int b) {     return a + b; } int main(void) { return add(3, 4); })c") == 7);
}

TEST_CASE("leaf_zp_abi sim: a caller with locals calls a zp_abi function") {
  // The caller has body locals of its own, x and y, and the regalloc
  // keeps them off the callee's parameter bytes, or the writes of the
  // arguments would clobber them. `n` is 0 in the simulator, so x is
  // 10, y is 20, and the sum is 30.
  CHECK(sim_return_int(R"c([[c6502::zp_abi]] int add(int a, int b) {     return a + b; } int main(int n) {     int x = n + 10;     int y = n + 20;     return add(x, y); })c") == 30);
}

TEST_CASE("leaf_zp_abi sim: a zp_abi function may make a direct non-recursive call") {
  CHECK(sim_return_int(R"c(int helper(int x) { return x + 1; } [[c6502::zp_abi]] int wrap(int x) { return helper(x); } int main(void) { return wrap(3); })c") == 4);
}

TEST_CASE("leaf_zp_abi sim: an eligible function that makes a call has no prologue") {
  // The private-pool allocator gives `x`, live across the call of
  // `touch`, a zero-page slot disjoint from every function that can be
  // live with it, so no callee-saved prologue is needed.
  const std::string src = R"c([[c6502::zp_abi]] void touch(unsigned char x) { (void)x; } [[c6502::zp_abi]] unsigned char wrap(unsigned char a, unsigned char b) {   unsigned char x = a;   touch(b);   return x; } int main(void) { return wrap(7, 3); })c";
  const std::string text = codegen_main(src);
  const std::size_t wrap_at = text.find("\nwrap:");
  const std::size_t main_at = text.find("\nmain:");
  REQUIRE(wrap_at != std::string::npos);
  REQUIRE(main_at != std::string::npos);
  REQUIRE(wrap_at < main_at);
  const std::string wrap_body = text.substr(wrap_at, main_at - wrap_at);
  CHECK_FALSE(contains(wrap_body, "STA   SSP"));
  CHECK_FALSE(contains(wrap_body, "STA   FP"));
  CHECK_FALSE(contains(wrap_body, ";"));
  CHECK(sim_return_int(src) == 7);
}

TEST_CASE("leaf_zp_abi sim: a zp_abi caller and its callee get disjoint slots") {
  // src/passes/zp_slot_allocation.h keeps them apart, or the callee's
  // writes of its arguments would clobber the caller's live parameters.
  // Each function's leading `v` or `w` arrives in A and X, so the
  // parameters after it arrive in their slots, one per byte.
  const std::string src = R"c([[c6502::zp_abi]] void noop(int w, int x, int y) { (void)w; (void)x; (void)y; } [[c6502::zp_abi]] int caller(int v, int a, int b) {   noop(v, a + 100, 0);   return b; } int main(void) { return caller(1, 2, 5); })c";
  CHECK(sim_return_int(src) == 5);
  const std::string text = codegen_main(src);
  for (const char* line : {"__zpabi_caller__a_0\tEQU\t$80", "__zpabi_caller__a_1\tEQU\t$81",
                           "__zpabi_caller__b_0\tEQU\t$82", "__zpabi_caller__b_1\tEQU\t$83",
                           "__zpabi_noop__x_0\tEQU\t$84", "__zpabi_noop__x_1\tEQU\t$85",
                           "__zpabi_noop__y_0\tEQU\t$86", "__zpabi_noop__y_1\tEQU\t$87"}) {
    INFO(line);
    CHECK(contains(text, line));
  }
}

}  // namespace c6502::suite
