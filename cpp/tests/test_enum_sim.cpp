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

const std::vector<std::tuple<std::string, std::string, int>> kPrograms = {{"enum", R"c(
        enum color { RED, GREEN = 5, BLUE, LAST = BLUE + 10 };
        typedef enum { OFF, ON } state;
        enum { N = 4, NEG = -3, BIG = 32767 };
        int table[N];
        struct thing { enum color c; state s; };
        struct thing things[2] = { { GREEN, ON }, { LAST, OFF } };
        enum color pick(enum color c) { return c == RED ? BLUE : RED; }
        int main(void) {
            enum color c = GREEN;
            long l = NEG;
            int r = 0;
            switch (c) {
            case RED: r = 1; break;
            case GREEN: r = 2; break;
            default: r = 3;
            }
            { int GREEN = 100; r += GREEN; }
            { enum { GREEN = 7 }; r += GREEN; }
            for (int i = 0; i < N; i++) table[i] = i * BLUE;
            r += table[3];
            r += things[1].c + things[0].s;
            r += pick(RED) + pick(c);
            r += (int)sizeof(enum color) + (int)sizeof(state)
                 + (int)sizeof RED;
            r += (int)l + (BIG == 32767) + (NEG < 0);
            r += (enum color)3;
            return r;
        }
    )c",
                                                                           158},
                                                                          {"enum_negative_static", R"c(
        enum { M = -32768, P = 32767 };
        long lo = M;
        int hi = P;
        int main(void) { return (lo == -32768L) + (hi == 32767) * 2; }
    )c",
                                                                           3},
                                                                          {"enum_one_byte", R"c(
        typedef enum : unsigned char { IDLE, RUN, STOP = 250 } state;
        enum dir : signed char { LEFT = -1, NONE, RIGHT };
        struct actor { state s; enum dir d; unsigned char x; };
        struct actor actors[3] = {
            { RUN, RIGHT, 10 }, { STOP, LEFT, 20 }, { IDLE, NONE, 30 },
        };
        state next(state s) { return s == IDLE ? RUN : s == RUN ? STOP : IDLE; }
        int main(void) {
            int r = 0;
            for (unsigned char i = 0; i < 3; i++) {
                struct actor *a = &actors[i];
                a->x += a->d;
                a->s = next(a->s);
                switch (a->s) {
                case IDLE: r += 1; break;
                case RUN: r += 2; break;
                case STOP: r += 4; break;
                }
                if (a->d < 0) r += 100;
            }
            r += sizeof(struct actor) * 1000;
            r += actors[0].x + actors[1].x + actors[2].x;
            r += (actors[1].s == IDLE) * 10000;
            return r;
        }
    )c",
                                                                           13167}};
TEST_CASE("enum: asm sim") {
  for (const auto& [name, src, expected] : kPrograms) {
    for (bool optimize : {false, true}) {
      INFO("case=" << name << " optimize=" << optimize);
      auto result = build_sim(src, optimize).run();
      CHECK_FALSE(result.timed_out);
      CHECK(result.return_int_signed() == expected);
    }
  }
}

}  // namespace
}  // namespace c6502::suite
