// Programs whose `static T const` objects the asm-level const-static
// fold replaces with immediates under -O, run in the simulator, from
// tests/test_const_static_fold.py. The folded programs compute what the
// unfolded ones do.
#include "doctest.h"
#include "sim_case.h"

namespace c6502::suite {

TEST_CASE("const_static_fold sim: a write through a folded pointer") {
  // The folded pointer's bytes are the address the unfolded program
  // reads, well above the code.
  const char* src = R"c(static unsigned char * const buf = (unsigned char * const)0x4000;
int main(void) { *buf = 0x77; return 0; }
)c";
  for (const bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    const sim::SimResult r = build_sim(src, optimize).run(5'000'000);
    CHECK_FALSE(r.timed_out);
    CHECK(r.memory[0x4000] == 0x77);
  }
}

TEST_CASE("const_static_fold sim: an indexed write through a folded pointer") {
  // The 16-bit add of the pointer and the index adds immediates where it
  // had read the pointer's bytes.
  const char* src = R"c(static unsigned char * const buf = (unsigned char * const)0x4000;
int main(void) {
    buf[300] = 0x42;
    return 0;
}
)c";
  for (const bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    const sim::SimResult r = build_sim(src, optimize).run(5'000'000);
    CHECK_FALSE(r.timed_out);
    CHECK(r.memory[0x4000 + 300] == 0x42);
  }
}

TEST_CASE("const_static_fold sim: a folded static const int returned") {
  const sim::SimResult r = build_sim(R"c(static const int magic = 0x1234;
int main(void) { return magic; }
)c", true).run(5'000'000);
  CHECK_FALSE(r.timed_out);
  CHECK((r.return_int() & 0xFFFF) == 0x1234);
}

TEST_CASE("const_static_fold sim: a static const pointer to an element") {
  // The pointer is read as the address's halves, and an access through
  // it as one through the address, where both had read the pointer from
  // memory.
  const char* src = R"c(#include <stdint.h>
uint8_t buf[8] = {1, 2, 3, 4, 5, 6, 7, 8};
static const uint8_t *const B = buf + 3;
uint8_t at(uint8_t i) { return B[i]; }
const uint8_t *ptr(void) { return B; }
int main(void) {
    int bad = 0;
    if (at(0) != 4 || at(4) != 8) bad |= 1;
    if (ptr() != &buf[3]) bad |= 2;
    return bad;
}
)c";
  for (const bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    CHECK(build_sim(src, optimize).run().return_int() == 0);
  }
}

}  // namespace c6502::suite
