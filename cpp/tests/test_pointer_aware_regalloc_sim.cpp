// Programs whose `LoadAddress` destinations the asm regalloc gives two
// zero-page bytes, run in the simulator, from
// tests/test_pointer_aware_regalloc.py. Under -O they give what the
// unoptimized pipeline gives, and a zp_abi leaf whose only frame need
// was such a destination has no prologue.
#include <sstream>
#include <string>
#include <utility>

#include "compile_case.h"
#include "doctest.h"
#include "sim_case.h"

namespace c6502::suite {

namespace {

std::pair<std::int64_t, std::int64_t> both_paths(const std::string& src) {
  return {run_c_program(src).return_int_signed(), build_sim(src, Pipeline::kOptimized).run().return_int_signed()};
}

std::string extract_function_body(const std::string& text, const std::string& name) {
  std::string out;
  bool in_fn = false;
  bool first = true;
  for (const std::string& line : lines(text)) {
    const bool starts_with_name = line.rfind(name + ":", 0) == 0;
    if (starts_with_name) {
      in_fn = true;
    } else if (in_fn) {
      if (!line.empty() && line[0] != ' ' && line[0] != '\t' && line.back() == ':' && line[0] != '.') break;
    }
    if (in_fn) {
      out += (first ? "" : "\n") + line;
      first = false;
    }
  }
  return out;
}

}  // namespace

TEST_CASE("pointer_aware_regalloc sim: a pointer to a local, dereferenced") {
  const auto [a, b] = both_paths(R"c(int main(void) {
    int x = 42;
    int *p = &x;
    return *p;
}
)c");
  CHECK(a == 42);
  CHECK(b == 42);
}

TEST_CASE("pointer_aware_regalloc sim: an array subscript in a loop") {
  // The interlace example's shape, smaller: the `LoadAddress`
  // destination gets two zero-page bytes in the loop.
  const auto [a, b] = both_paths(R"c(#include <stdint.h>
static uint16_t table[3] = {0xAA, 0xBB, 0xCC};
int main(void) {
    uint16_t sum = 0;
    int8_t i;
    for (i = 0; i < 3; i++) sum += table[i];
    return sum;
}
)c");
  const std::int64_t expected = 0xAA + 0xBB + 0xCC;
  CHECK(a == expected);
  CHECK(b == expected);
}

TEST_CASE("pointer_aware_regalloc sim: a zp_abi leaf with a LoadAddress has no prologue") {
  // interlace_fill_p1's shape: a zp_abi leaf that subscripts a static
  // array. The `LoadAddress` destination had forced a frame slot, and so
  // a prologue, before it was given two zero-page bytes.
  const std::string src = R"c(#include <stdint.h>
static uint8_t table[8] = {0,1,2,3,4,5,6,7};
static uint8_t out[8];
[[c6502::zp_abi]]
void copy(uint8_t mask) {
    for (int8_t i = 0; i < 8; i++)
        out[i] = table[i] & mask;
}
int main(void) { copy(0xFF); return out[3]; }
)c";
  const std::string body = extract_function_body(codegen(src, Pipeline::kOptimized), "copy");
  CHECK_FALSE(contains(body, "; prologue:"));
  CHECK_FALSE(contains(body, "; epilogue"));
  CHECK_FALSE(contains(body, "STA   SSP"));
  CHECK_FALSE(contains(body, "STA   FP"));
  CHECK(build_sim(src, Pipeline::kOptimized).run().return_int_signed() == 3);
}

}  // namespace c6502::suite
