// A program with switch, arrays, struct members, pointers, static
// initializers, and calls on bit-precise values, run in the simulator in
// both pipelines, from tests/test_bitint.py. (tests/test_bitint_sim.py is
// another module, which runs every operator on every width.)
#include <cstdint>
#include <vector>

#include "doctest.h"
#include "sim_case.h"

namespace c6502::suite {

TEST_CASE("bitint sim: a program on bit-precise values runs to the same result in both pipelines") {
  const char* src = R"c(
#include <stdbit.h>
typedef unsigned _BitInt(24) u24;
typedef _BitInt(12) s12;

struct rec { unsigned char tag; s12 x; u24 big; _BitInt(5) small; };
struct rec recs[3] = {{1, -5, 0xABCDEF, -16}, {2, 2047, 7, 15}};
static const _BitInt(40) table[4] = {1, -2, 0x7FFFFFFFFFwb, -0x8000000000wb};
u24 counter = 0xFFFFFE;
_BitInt(5) small = 100;
long results[16];

u24 add24(u24 a, u24 b) { return a + b; }
s12 twice(s12 *p) { *p *= 2; return *p; }
_BitInt(40) pick(_BitInt(3) i) { return table[i & 3]; }

int classify(_BitInt(5) v) {
    switch (v) {
    case -16: return 1;
    case 15: return 2;
    case 0: return 3;
    default: return 4;
    }
}

void more(long *r);

int main(void) {
    long *r = results;
    s12 x = recs[0].x;
    *r++ = add24(counter, 3);
    counter++;
    *r++ = counter;
    *r++ = twice(&x);
    *r++ = twice(&recs[1].x);
    *r++ = pick(2);
    *r++ = (long)(pick(3) >> 8);
    *r++ = classify(recs[0].small) * 10 + classify(recs[1].small);
    *r++ = classify(small) * 10 + classify(small - 4);
    more(r);
    return 0;
}

void more(long *r) {
    *r++ = recs[0].big >> 4;
    *r++ = (unsigned _BitInt(4))(recs[0].tag + 20);
    *r++ = stdc_bit_width((unsigned _BitInt(16))1000);
    *r++ = sizeof(struct rec);
    *r++ = (u24)-1 > 0;
    *r++ = (s12)-1 < 0;
    *r++ = (_BitInt(20))(double)-123456.75;
    *r++ = (long)(float)(u24)0x123456;
}
)c";
  const std::vector<std::int64_t> expected = {
      1,    0xFFFFFF, -10, -2, 0x7FFFFFFFFFLL & 0xFFFFFFFF, (-0x8000000000LL >> 8) & 0xFFFFFFFF, 12, 43, 0xABCDE, 5,
      10,   7,        1,   1,  -123456,                     0x123456,
  };
  std::vector<std::int64_t> want;
  for (const std::int64_t e : expected) want.push_back(static_cast<std::int32_t>(static_cast<std::uint32_t>(e)));
  for (const bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    sim::Simulation s = build_sim(src, optimize);
    const sim::SimResult r = s.run();
    const std::int64_t addr = s.symbol("results");
    std::vector<std::int64_t> got;
    for (std::size_t k = 0; k < want.size(); ++k) {
      got.push_back(static_cast<std::int32_t>(r.word(addr + 4 * static_cast<std::int64_t>(k), 4)));
    }
    CHECK(got == want);
  }
}

}  // namespace c6502::suite
