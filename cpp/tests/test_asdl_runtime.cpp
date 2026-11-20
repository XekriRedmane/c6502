// Tests of the IR runtime's integer helpers (src/asdl/runtime.h).
#include "asdl/runtime.h"
#include "doctest.h"

namespace asdl = c6502::asdl;

TEST_CASE("int_to_string gives Python's digits") {
  CHECK(asdl::int_to_string(0) == "0");
  CHECK(asdl::int_to_string(-1) == "-1");
  CHECK(asdl::int_to_string(asdl::Int(1234567890123456789LL) * 100) ==
        "123456789012345678900");
  // 2**64 - 1, the largest `unsigned long long`.
  const asdl::Int u64 = (asdl::Int(1) << 64) - 1;
  CHECK(asdl::int_to_string(u64) == "18446744073709551615");
  CHECK(asdl::int_to_string(-u64) == "-18446744073709551615");
}

TEST_CASE("floor_div and floor_mod round as Python does") {
  CHECK(asdl::floor_div(7, 2) == 3);
  CHECK(asdl::floor_div(-7, 2) == -4);
  CHECK(asdl::floor_div(7, -2) == -4);
  CHECK(asdl::floor_mod(-7, 2) == 1);
  CHECK(asdl::floor_mod(7, -2) == -1);
  CHECK(asdl::floor_mod(-8, 2) == 0);
}

