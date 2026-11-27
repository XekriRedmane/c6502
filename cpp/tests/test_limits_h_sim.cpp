#include <cstdint>
#include <string>
#include <tuple>
#include <vector>

#include "batch1_case.h"
#include "compile_case.h"
#include "doctest.h"
#include "sim_case.h"

namespace c6502::suite {
namespace {

const std::string kHeader = R"c(#include <limits.h>
)c";
auto run_int_signed(const auto& body) {
  auto src = kHeader + cat("int main(void) { ", body, " }");
  return run_c_program(src).return_int_signed();
}

auto run_int_unsigned(const auto& body) {
  auto src = kHeader + cat("unsigned int main(void) { ", body, " }");
  return run_c_program(src).return_int();
}

auto run_long(const auto& body) {
  auto src = kHeader + cat("long main(void) { ", body, " }");
  return run_c_program(src).return_long_signed();
}

auto run_ulong(const auto& body) {
  auto src = kHeader + cat("unsigned long main(void) { ", body, " }");
  return run_c_program(src).return_long();
}

auto run_char(const auto& body) {
  auto src = kHeader + cat("int main(void) { ", body, " }");
  return run_c_program(src).return_int_signed();
}

TEST_CASE("limits_h: char bit") { CHECK(run_char("return CHAR_BIT;") == 8); }

TEST_CASE("limits_h: schar min") { CHECK(run_char("return SCHAR_MIN;") == -128); }

TEST_CASE("limits_h: schar max") { CHECK(run_char("return SCHAR_MAX;") == 127); }

TEST_CASE("limits_h: uchar max") { CHECK(run_char("return UCHAR_MAX;") == 255); }

TEST_CASE("limits_h: char min is unsigned") {
  // plain char is unsigned in c6502
  CHECK(run_char("return CHAR_MIN;") == 0);
}

TEST_CASE("limits_h: char max is uchar max") { CHECK(run_char("return CHAR_MAX;") == 255); }

TEST_CASE("limits_h: mb len max") { CHECK(run_char("return MB_LEN_MAX;") == 1); }

TEST_CASE("limits_h: int max") { CHECK(run_int_signed("return INT_MAX;") == 32767); }

TEST_CASE("limits_h: int min") { CHECK(run_int_signed("return INT_MIN;") == -32768); }

TEST_CASE("limits_h: uint max") { CHECK(run_int_unsigned("return UINT_MAX;") == 65535); }

TEST_CASE("limits_h: shrt aliases int") {
  // SHRT_MIN / SHRT_MAX / USHRT_MAX alias INT_*.
  CHECK(run_int_signed("return SHRT_MAX;") == 32767);
  CHECK(run_int_signed("return SHRT_MIN;") == -32768);
  CHECK(run_int_unsigned("return USHRT_MAX;") == 65535);
}

TEST_CASE("limits_h: long max") { CHECK(run_long("return LONG_MAX;") == 0x7FFFFFFF); }

TEST_CASE("limits_h: long min") { CHECK(run_long("return LONG_MIN;") == -0x80000000LL); }

TEST_CASE("limits_h: ulong max") { CHECK(run_ulong("return ULONG_MAX;") == 0xFFFFFFFFLL); }

TEST_CASE("limits_h: idempotent double include") {
  std::string src = R"c(#include <limits.h>
#include <limits.h>
int main(void) { return INT_MAX; }
)c";
  CHECK(run_c_program(src).return_int_signed() == 32767);
}

}  // namespace
}  // namespace c6502::suite
