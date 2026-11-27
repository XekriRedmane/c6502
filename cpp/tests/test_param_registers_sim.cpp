// Parameters that arrive in named registers (`[[c6502::in_y]]`, the
// pairs such as `in_xy`) and positional `register` parameters, run in the
// simulator in both pipelines, from tests/test_param_registers.py. The
// fixed-address routines are stand-ins written into memory before the
// run.
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "compile_case.h"
#include "doctest.h"
#include "sim_case.h"

namespace c6502::suite {

namespace {

struct Standin {
  std::int64_t addr;
  std::vector<std::uint8_t> code;
};

void check_runs(const std::string& src, std::int64_t expected, const std::optional<Standin>& standin = std::nullopt) {
  for (const bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    sim::Simulation s = build_sim(src, optimize);
    if (standin) {
      for (std::size_t k = 0; k < standin->code.size(); ++k) s.cpu.memory[standin->addr + k] = standin->code[k];
    }
    CHECK(s.run().return_int() == expected);
  }
}

const std::string kRom = R"c([[c6502::zp_abi, c6502::address(0xD000)]]
unsigned char rom(register unsigned char a [[c6502::in_a]],
                  register unsigned char y [[c6502::in_y]]);
)c";

// A routine at $D000 that returns A + 2 * Y, as `STY $3100; ASL $3100;
// CLC; ADC $3100; RTS`.
const Standin kRomCode = {0xD000, {0x8C, 0x00, 0x31, 0x0E, 0x00, 0x31, 0x18, 0x6D, 0x00, 0x31, 0x60}};

// A routine at $D100 that returns the byte after the one its pointer
// argument, low byte in Y and high byte in A, points at: `STY $F0; STA
// $F1; LDY #1; LDA ($F0),Y; RTS`.
const Standin kNextCode = {0xD100, {0x84, 0xF0, 0x85, 0xF1, 0xA0, 0x01, 0xB1, 0xF0, 0x60}};

}  // namespace

TEST_CASE("param_registers sim: a compiled function with parameters in A and Y") {
  check_runs(R"c(unsigned char out[2];
unsigned char g = 9;
void put(register unsigned char hi [[c6502::in_a]],
         register unsigned char lo [[c6502::in_y]]) {
    out[0] = hi; out[1] = lo;
}
int main(void) {
    unsigned char a = g + 1;
    put(a, g);
    return out[0] * 100 + out[1];
}
)c", 1009);
}

TEST_CASE("param_registers sim: a pair and a positional register parameter") {
  // `w` takes X and Y, so `k`, declared plain `register`, gets A.
  check_runs(R"c(unsigned int sum(register unsigned int w [[c6502::in_xy]],
                 register unsigned char k) {
    return w + k;
}
unsigned int swap(register unsigned int w [[c6502::in_ya]]) {
    return (w << 8) | (w >> 8);
}
int main(void) {
    unsigned int s = sum(0x1234, 3);
    return (s == 0x1237) + (swap(0xABCD) == 0xCDAB) * 2;
}
)c", 3);
}

TEST_CASE("param_registers sim: a fixed-address routine") {
  check_runs(kRom + R"c(unsigned char g = 20;
int main(void) { return rom(g, 3) + rom(1, g); }
)c", (20 + 6) + (1 + 40), kRomCode);
}

TEST_CASE("param_registers sim: pointer parameters") {
  // `second` and `sum` read through pointers that arrive in registers,
  // without `register`.
  check_runs(R"c(struct blk { unsigned char a, b, c; };
struct blk blk = {1, 2, 3};
unsigned char tab[4] = {10, 20, 30, 40};
unsigned char second(const unsigned char *p [[c6502::in_ya]]) {
    return p[1];
}
unsigned char sum(struct blk *b [[c6502::in_xy]],
                  unsigned char k [[c6502::in_a]]) {
    return b->a + b->c + k;
}
int main(void) { return second(tab) + sum(&blk, 100); }
)c", 20 + 1 + 3 + 100);
}

TEST_CASE("param_registers sim: a fixed-address routine with a pointer") {
  check_runs(R"c(struct blk { unsigned char a, b, c; };
struct blk blk = {7, 9, 11};
[[c6502::zp_abi, c6502::address(0xD100)]]
unsigned char next(struct blk *p [[c6502::in_ya]]);
int main(void) { return next(&blk); }
)c", 9, kNextCode);
}

TEST_CASE("param_registers sim: an argument for A from a frame slot") {
  // Without -O, `a` and `b` live on the soft stack, and loading `a` into
  // A is `LDY #off; LDA (FP),Y`, which would destroy `b`, already in Y.
  // So `a` is loaded first and kept with PHA and PLA.
  const std::string src = kRom + R"c(int main(void) {
    unsigned char a = 7, b = 5;
    for (unsigned char i = 0; i < 3; i++) { a++; b += 2; }
    return rom(a, b);
}
)c";
  CHECK(contains(codegen(src, Pipeline::kUnoptimized), "PHA"));
  check_runs(src, 10 + 2 * 11, kRomCode);
}

}  // namespace c6502::suite
