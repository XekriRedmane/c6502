#include <string>

#include "doctest.h"
#include "sim_case.h"

namespace c6502::suite {

namespace {

void check_both(const std::string& src, std::int64_t expected) {
  for (const bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    CHECK(build_sim(src, optimize).run(200'000).return_int() == expected);
  }
}

}  // namespace

TEST_CASE("dispatch_pointer_array sim: a table that another function stores into") {
  // swap() repoints frames[0]. The dispatch read walk[1] = 2 from the
  // initial contents in place of other[1] = 9.
  check_both(R"c(#include <stdint.h>
static uint8_t walk[4] = {1, 2, 3, 4};
static uint8_t jump[4] = {5, 6, 7, 8};
static uint8_t other[4] = {9, 9, 9, 9};
static uint8_t *frames[2] = {walk, jump};
void swap(void) { frames[0] = other; }
uint8_t frame(uint8_t anim, uint8_t step) {
    return frames[anim][step];
}
int main(void) { swap(); return frame(0, 1) + frame(1, 2) * 16; }
)c", 9 + 7 * 16);
}

TEST_CASE("dispatch_pointer_array sim: a const table") {
  check_both(R"c(#include <stdint.h>
static uint8_t walk[4] = {1, 2, 3, 4};
static uint8_t jump[4] = {5, 6, 7, 8};
static uint8_t * const frames[2] = {walk, jump};
uint8_t frame(uint8_t anim, uint8_t step) {
    return frames[anim][step];
}
int main(void) { walk[1] = 11; return frame(0, 1) + frame(1, 2) * 16; }
)c", 11 + 7 * 16);
}

}  // namespace c6502::suite
