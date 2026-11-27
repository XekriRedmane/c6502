// tests/test_enum.py's programs on the TAC interpreter. The simulator's
// runs are in test_enum_sim.cpp.
#include <string>
#include <vector>

#include "compile_case.h"
#include "doctest.h"

namespace {

namespace suite = c6502::suite;

struct Program {
  const char* name;
  std::string source;
  long long expected;
};

// Returns the low 16 bits of `main`'s value on the TAC, as an `int`.
long long tac_int(const std::string& source) {
  const auto value = suite::run_program(source);
  REQUIRE(value.has_value());
  long long v = static_cast<long long>(*value & 0xFFFF);
  if (v & 0x8000) v -= 0x10000;
  return v;
}

}  // namespace

TEST_CASE("enum: the programs return their values on the TAC interpreter") {
  const std::vector<Program> programs = {
      {"enum", R"(
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
    )", 158},
      {"enum_negative_static", R"(
        enum { M = -32768, P = 32767 };
        long lo = M;
        int hi = P;
        int main(void) { return (lo == -32768L) + (hi == 32767) * 2; }
    )", 3},
      {"enum_one_byte", R"(
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
    )", 13167},
  };
  for (const Program& p : programs) {
    INFO(p.name);
    CHECK(tac_int(p.source) == p.expected);
  }
}
