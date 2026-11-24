// tests/test_pool.py's tests of the zero-page pool
// (src/passes/optimization/pool.h), ported by hand in step 8 (group G7),
// one case per Python test. Python's `range` has a length, which here
// is `stop - start`, since every range of a pool has a step of 1.
#include <stdexcept>

#include "doctest.h"
#include "passes/optimization/pool.h"

namespace {

using c6502::passes::Pool;
using c6502::passes::Range;

c6502::asdl::Int length(const Range& r) { return r.stop - r.start; }

}  // namespace

TEST_CASE("pool: TestDefaults::test_default_start") {
  const Pool p;
  CHECK(p.start == 0x80);
  CHECK(p.mid() == 0xC0);
}

TEST_CASE("pool: TestDefaults::test_default_caller_range") {
  const Pool p;
  CHECK(p.caller_saved() == Range{.start = 0x80, .stop = 0xC0});
  CHECK(length(p.caller_saved()) == 64);
}

TEST_CASE("pool: TestDefaults::test_default_callee_range") {
  const Pool p;
  CHECK(p.callee_saved() == Range{.start = 0xC0, .stop = 0x100});
  CHECK(length(p.callee_saved()) == 64);
}

TEST_CASE("pool: TestCustomStart::test_start_a0") {
  const Pool p{.start = 0xA0};
  p.validate();
  CHECK(p.mid() == 0xD0);
  CHECK(p.caller_saved() == Range{.start = 0xA0, .stop = 0xD0});
  CHECK(p.callee_saved() == Range{.start = 0xD0, .stop = 0x100});
  CHECK(length(p.caller_saved()) == 48);
  CHECK(length(p.callee_saved()) == 48);
}

TEST_CASE("pool: TestCustomStart::test_start_c0") {
  const Pool p{.start = 0xC0};
  p.validate();
  CHECK(p.mid() == 0xE0);
  CHECK(length(p.caller_saved()) == 32);
  CHECK(length(p.callee_saved()) == 32);
}

TEST_CASE("pool: TestCustomStart::test_start_zero") {
  // A start of 0 is allowed: the caller-saved bytes are [0, $80) and
  // the callee-saved ones [$80, $100).
  const Pool p{.start = 0x00};
  p.validate();
  CHECK(p.mid() == 0x80);
  CHECK(length(p.caller_saved()) == 128);
  CHECK(length(p.callee_saved()) == 128);
}

TEST_CASE("pool: TestValidation::test_odd_start_rejected") {
  CHECK_THROWS_WITH_AS(Pool{.start = 0x81}.validate(), "Pool.start must be even: 0x81",
                       std::invalid_argument);
}

TEST_CASE("pool: TestValidation::test_negative_start_rejected") {
  CHECK_THROWS_AS(Pool{.start = -2}.validate(), std::invalid_argument);
}

TEST_CASE("pool: TestValidation::test_overflow_start_rejected") {
  CHECK_THROWS_AS(Pool{.start = 0x100}.validate(), std::invalid_argument);
}

TEST_CASE("pool: TestRangeMembership::test_default_caller_membership") {
  const Pool p;
  CHECK(p.caller_saved().contains(0x80));
  CHECK(p.caller_saved().contains(0xBF));
  CHECK_FALSE(p.caller_saved().contains(0xC0));
  CHECK_FALSE(p.caller_saved().contains(0x7F));
}

TEST_CASE("pool: TestRangeMembership::test_default_callee_membership") {
  const Pool p;
  CHECK(p.callee_saved().contains(0xC0));
  CHECK(p.callee_saved().contains(0xFF));
  CHECK_FALSE(p.callee_saved().contains(0xBF));
  CHECK_FALSE(p.callee_saved().contains(0x100));
}
