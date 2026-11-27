// The simulator tests of tests/test_loop_counter_to_x.py: loops whose
// counter the loop-counter-to-X promotion moves end, after a call and
// across pages, with the values C gives.
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

namespace loop_counter_to_x_sim {
TEST_CASE("loop_counter_to_x: pivot after call keeps loop finite") {
  // After `JSR g`, `out2 = spr_w[k]` loads `LDX k` and runs on
  // into `DEC i`. The optimized build used to loop forever.
  std::string src = R"c(
        #include <stdint.h>
        uint8_t ent_y[8];
        uint8_t spr_w[8];
        volatile uint8_t out1, out2;
        uint8_t calls;
        void g(void) { calls++; }
        void f(uint8_t n, uint8_t k)
        {
            uint8_t i = n;
            do {
                out1 = ent_y[i];
                g();
                out2 = spr_w[k];
                i--;
            } while ((i & 0x80) == 0);
        }
        int main(void) { f(3, 1); return calls; }
        )c";
  for (bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    auto r = build_sim(src, optimize).run();
    CHECK_FALSE(r.timed_out);
    CHECK(r.return_int() == 4);
  }
}

}  // namespace loop_counter_to_x_sim

namespace pointer_high_byte_sim {
// `msg = msg + msg[0] + 1` steps the pointer's high byte with one
// `INC`, after `SEC; ADC (msg),Y` (cpp/src/passes/carry_in_add.h). The pass
// once took that byte for a loop counter and stepped it in X, while
// the reads through `(msg),Y` still found the old high byte in
// memory, so a step across a page read the byte 256 below.
const std::string kSrc = R"c(
    #include <stdint.h>
    uint8_t buf[1100];
    uint8_t count;
    uint8_t calls;
    uint16_t seen;
    void ext(const uint8_t *p) { seen += p[1]; }
    void ext2(void) { calls++; }
    void walk(const uint8_t *msg) {
        while (*msg != 0) {
            ext(msg);
            count++;
            msg = msg + msg[0] + 1;
            if (count > 1 || *msg == 0)
                ext2();
        }
    }
    int main(void) {
        for (uint16_t i = 0; i < 3; i++) {
            buf[OFF + i * 255] = 254;
            for (uint16_t j = 1; j < 255; j++)
                buf[OFF + i * 255 + j] = (uint8_t)(i + 1);
        }
        walk(buf + OFF);
        return calls * 256 + seen * 16 + count;
    }
    )c";
TEST_CASE("loop_counter_to_x: steps across pages") {
  // Each string is 255 bytes long, so a step from any start other
  // than a page boundary crosses a page.
  for (const auto& off : {0, 5, 200}) {
    for (bool optimize : {false, true}) {
      INFO("off=" << off << " optimize=" << optimize);
      auto src = replace_all(kSrc, "OFF", std::to_string(off));
      auto r = build_sim(src, optimize).run();
      CHECK_FALSE(r.timed_out);
      CHECK(r.return_int() == 2 * 256 + 6 * 16 + 3);
    }
  }
}

}  // namespace pointer_high_byte_sim

}  // namespace
}  // namespace c6502::suite
