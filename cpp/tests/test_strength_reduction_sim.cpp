// Programs that src/passes/optimization/strength_reduction.h and the
// inline shifts of tac_to_asm rewrite, run in the simulator, from
// tests/test_strength_reduction.py. The rewritten programs under -O give
// what the unoptimized pipeline's helper calls give.
#include <string>
#include <utility>

#include "doctest.h"
#include "sim/cpu_run.h"
#include "sim_case.h"

namespace c6502::suite {

namespace {

std::pair<std::int64_t, std::int64_t> both_paths(const std::string& src) {
  return {run_c_program(src).return_int_signed(), build_sim(src, true).run().return_int_signed()};
}

}  // namespace

TEST_CASE("strength_reduction sim: two multiplies into one global") {
  // A global keeps its name in SSA form, so each multiply's temporaries
  // need names of their own (fuzzer stmts22250).
  const auto [a, b] = both_paths(R"c(
        unsigned long z [[c6502::zeropage]] = 128;
        int main(void) {
            unsigned char w;
            z *= 3;
            w = 0;
            do {
                if (w & 1) z *= 129;
            } while (++w < 4);
            return (int)(z & 0x7FFF);
        }
        )c");
  const std::int64_t want = (128LL * 3 * 129 * 129) & 0x7FFF;
  CHECK(a == want);
  CHECK(b == want);
}

TEST_CASE("strength_reduction sim: unsigned multiply by 2") {
  const auto [a, b] = both_paths(R"c(unsigned int f(unsigned int x) { return x * 2; }
int main(void) { return (int)f(13579u); }
)c");
  CHECK(a == b);
}

TEST_CASE("strength_reduction sim: unsigned multiply by 8") {
  const auto [a, b] = both_paths(R"c(unsigned int f(unsigned int x) { return x * 8; }
int main(void) { return (int)f(100u); }
)c");
  CHECK(a == b);
}

TEST_CASE("strength_reduction sim: signed multiply by 4") {
  const auto [a, b] = both_paths(R"c(int f(int x) { return x * 4; }
int main(void) { return f(7); }
)c");
  CHECK(a == b);
}

TEST_CASE("strength_reduction sim: unsigned divide by 2") {
  const auto [a, b] = both_paths(R"c(unsigned int f(unsigned int x) { return x / 2; }
int main(void) { return (int)f(15u); }
)c");
  CHECK(a == b);
}

TEST_CASE("strength_reduction sim: unsigned modulo by 4") {
  const auto [a, b] = both_paths(R"c(unsigned int f(unsigned int x) { return x % 4; }
int main(void) { return (int)f(13u); }
)c");
  CHECK(a == b);
}

TEST_CASE("strength_reduction sim: signed divide by 4 keeps truncation") {
  // Signed division isn't strength-reduced, since -7 / 4 rounds toward
  // zero to -1.
  const auto [a, b] = both_paths(R"c(int f(int x) { return x / 4; }
int main(void) { return f(-7); }
)c");
  CHECK(a == -1);
  CHECK(b == -1);
}

TEST_CASE("strength_reduction sim: uint16 shift left by 1") {
  // The unoptimized pipeline calls asl16 and -O shifts inline. 0x8000
  // is -32768 as an int.
  const auto [a, b] = both_paths(R"c(unsigned int f(unsigned int x) { return x << 1; }
int main(void) { return (int)f(0x4000u); }
)c");
  CHECK(a == -32768);
  CHECK(b == -32768);
}

TEST_CASE("strength_reduction sim: uint16 shift right by 1") {
  const auto [a, b] = both_paths(R"c(unsigned int f(unsigned int x) { return x >> 1; }
int main(void) { return (int)f(0xFFFEu); }
)c");
  CHECK(a == 0x7FFF);
  CHECK(b == 0x7FFF);
}

TEST_CASE("strength_reduction sim: int16 shift right by 1 keeps the sign") {
  const auto [a, b] = both_paths(R"c(int f(int x) { return x >> 1; }
int main(void) { return f(-2); }
)c");
  CHECK(a == -1);
  CHECK(b == -1);
}

TEST_CASE("strength_reduction sim: int16 shift right of a negative odd value") {
  // -3 >> 1 is -2, since an arithmetic right shift rounds toward minus
  // infinity.
  const auto [a, b] = both_paths(R"c(int f(int x) { return x >> 1; }
int main(void) { return f(-3); }
)c");
  CHECK(a == -2);
  CHECK(b == -2);
}

TEST_CASE("strength_reduction sim: a byte times every byte constant") {
  // A byte times a constant of three signed powers of two is shifts and
  // adds, where it had called `mul8`. Each constant from 0 to 255 times
  // a few values is checked against the product computed by `check`,
  // for the constants this rule takes and the others.
  const int values[] = {0, 1, 2, 3, 0x45, 0x80, 0xAB, 0xFF};
  std::string list;
  for (const int v : values) list += (list.empty() ? "" : ", ") + std::to_string(v);
  std::string src = "#include <stdint.h>\nvolatile uint8_t v[8] = {" + list +
                    "};\nuint16_t check(uint8_t c, uint8_t k);\nint main(void) {\n    uint16_t bad = 0;\n";
  for (int c = 0; c < 256; ++c) {
    const std::string cs = std::to_string(c);
    src += "    for (uint8_t k = 0; k < 8; k++) if ((uint8_t)(v[k] * " + cs + ") != (uint8_t)check(" + cs +
           ", k)) bad++;\n";
  }
  src += "    return bad;\n}\nuint16_t check(uint8_t c, uint8_t k) { return (uint8_t)(c * v[k]); }\n";
  // The size and CRC-32 of the program tests/test_strength_reduction.py
  // generated on 27 Nov 2026.
  CHECK(src.size() == 24597);
  CHECK(sim::cpu_run::crc32(src) == 0xcd41f0f2u);
  const sim::SimResult r = build_sim(src, true).run(200'000'000);
  CHECK_FALSE(r.timed_out);
  CHECK(r.return_int() == 0);
}

}  // namespace c6502::suite
