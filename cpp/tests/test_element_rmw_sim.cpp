// The read-modify-write of an array element indexed by X, `INC t,X` and
// the rest, run in the simulator in both pipelines, from
// tests/test_element_rmw.py.
#include <vector>

#include "doctest.h"
#include "sim_case.h"

namespace c6502::suite {

TEST_CASE("element_rmw sim: each operation on an element indexed by X") {
  // Each operation runs across 0 and 255, with the result used and not,
  // and branched on. `timer` and `timers` have `handle_timers`'s shape.
  const char* src = R"c(
#include <stdint.h>
uint8_t a[16];
uint8_t out[24];
[[c6502::zeropage]] uint8_t flag;
void dec_unused(uint8_t i) { --a[i]; }
uint8_t dec_used(uint8_t i) { return --a[i]; }
void inc_unused(uint8_t i) { a[i]++; }
uint8_t inc_used(uint8_t i) { return ++a[i]; }
uint8_t post_dec(uint8_t i) { return a[i]--; }
uint8_t post_inc(uint8_t i) { return a[i]++; }
void shl(uint8_t i) { a[i] <<= 1; }
void shr(uint8_t i) { a[i] >>= 1; }
uint8_t shl_used(uint8_t i) { return a[i] <<= 1; }
uint8_t dec_branch(uint8_t i) { if (--a[i] == 0) return 7; return 9; }
uint8_t inc_branch(uint8_t i) { if (++a[i] == 0) return 7; return 9; }
uint8_t timer(uint8_t i) {
    if (a[i] == 0)
        return 1;
    uint8_t t = --a[i];
    if (t == 0) {
        flag = 6;
        return 2;
    }
    flag = 5;
    if (t == 20 || t == 10)
        return t;
    return 3;
}
void timers(uint8_t n) {
    for (uint8_t i = n; (int8_t)i >= 0; i--) {
        if (a[i] == 0)
            continue;
        uint8_t t = --a[i];
        if (t == 0) {
            flag = 1;
            continue;
        }
        flag = 2;
        if (t == 20 || t == 10)
            out[16 + i] = t;
    }
}
int main(void) {
    uint8_t k = 0;
    a[0] = 0;    dec_unused(0);
    a[1] = 1;    out[k++] = dec_used(1);
    a[2] = 255;  inc_unused(2);
    a[3] = 0x7F; out[k++] = inc_used(3);
    a[4] = 0;    out[k++] = post_dec(4);
    a[5] = 255;  out[k++] = post_inc(5);
    a[6] = 0x81; shl(6);
    a[7] = 0x81; shr(7);
    a[8] = 0xC0; out[k++] = shl_used(8);
    a[9] = 1;    out[k++] = dec_branch(9);
    out[k++] = dec_branch(9);
    a[10] = 255; out[k++] = inc_branch(10);
    out[k++] = inc_branch(10);
    a[11] = 0;   out[k++] = timer(11);
    a[12] = 1;   out[k++] = timer(12);
    a[13] = 21;  out[k++] = timer(13);
    a[14] = 11;  out[k++] = timer(14);
    a[15] = 6;   out[k++] = timer(15);
    out[k++] = flag;
    a[0] = 0; a[1] = 1; a[2] = 21; a[3] = 11; a[4] = 200;
    timers(4);
    out[k++] = flag;
    return k;
}
)c";
  // The results main stores in out[], in order.
  const std::vector<int> want_out = {0, 0x80, 0, 255, 0x80, 7, 9, 7, 9, 1, 2, 20, 10, 3, 5, 1};
  for (const bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    sim::Simulation s = build_sim(src, optimize);
    const sim::SimResult r = s.run();
    CHECK_FALSE(r.timed_out);
    const auto bytes = [&](const char* name, int from, int to) {
      std::vector<int> v;
      for (int k = from; k < to; ++k) v.push_back(r.memory[s.symbol(name) + k]);
      return v;
    };
    CHECK(r.return_int() == static_cast<std::int64_t>(want_out.size()));
    CHECK(bytes("out", 0, static_cast<int>(want_out.size())) == want_out);
    // The second batch, by timers(4): a[0] stays 0, a[1] runs out, a[2]
    // and a[3] reach 20 and 10, and a[4] steps.
    CHECK(bytes("a", 0, 5) == std::vector<int>{0, 0, 20, 10, 199});
    CHECK(bytes("out", 16, 21) == std::vector<int>{0, 0, 20, 10, 0});
    CHECK(bytes("a", 5, 16) == std::vector<int>{0, 0x02, 0x40, 0x80, 0xFF, 1, 0, 0, 20, 10, 5});
  }
}

TEST_CASE("element_rmw sim: an element store over a copy of a pointed-to byte") {
  // `pointer_self_store` took `buf[1]` to hold `p->v` after `buf[i] =
  // 9` and dropped the store back through `p`, so this returned 3 under
  // -O before 19 Nov 2026.
  const char* src = R"c(
        #include <stdint.h>
        struct s { uint8_t v; uint8_t w; };
        static uint8_t buf[4];
        struct s obj;
        void f(struct s *p, uint8_t i) {
            buf[1] = p->v;
            buf[i] = 9;
            p->v = buf[1];
        }
        int main(void) {
            obj.v = 3;
            f(&obj, 1);
            return obj.v;
        }
        )c";
  for (const bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    CHECK(build_sim(src, optimize).run().return_int() == 9);
  }
}

}  // namespace c6502::suite
