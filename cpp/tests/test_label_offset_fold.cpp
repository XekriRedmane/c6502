// The tests of tests/test_label_offset_fold.py, ported.
#include <string>

#include "asm_text.h"
#include "doctest.h"
#include "passes/label_offset_fold.h"

namespace {

using c6502::asm_text::expect;
using c6502::asm_text::instructions;
using c6502::asm_text::program;

const std::string kRet = " Return(false false false)";

std::string run(const std::string& instrs) {
  return instructions(c6502::passes::apply_label_offset_fold(program(instrs)));
}

}  // namespace

TEST_CASE("label_offset_fold adds a constant to a label at link time") {
  SUBCASE("both bytes") {
    CHECK(run(R"(Mov(Imm(47) Reg(A) false) ClearCarry Add(ImmLabelLow("LOCS" 0) Reg(A))
                 Mov(Reg(A) Data("t" 0) false) Mov(Imm(1) Reg(A) false)
                 Add(ImmLabelHigh("LOCS" 0) Reg(A)) Mov(Reg(A) Data("t" 1) false))" +
              kRet) ==
          expect(R"(Mov(ImmLabelLow("LOCS" 303) Reg(A) false) Mov(Reg(A) Data("t" 0) false)
                    Mov(ImmLabelHigh("LOCS" 303) Reg(A) false) Mov(Reg(A) Data("t" 1) false))" +
                 kRet));
  }
  SUBCASE("the low byte alone") {
    // `(uint8_t)(uint16_t)&LOCS[1]` on records of 9 bytes.
    CHECK(run(R"(Mov(ImmLabelLow("LOCS" 0) Reg(A) false) ClearCarry Add(Imm(9) Reg(A))
                 Mov(Reg(A) Data("t" 0) false))" +
              kRet) ==
          expect(R"(Mov(ImmLabelLow("LOCS" 9) Reg(A) false) Mov(Reg(A) Data("t" 0) false))" +
                 kRet));
  }
  SUBCASE("the high byte into X") {
    // `return T + 7;` returns the high byte in X.
    CHECK(run(R"(Mov(ImmLabelLow("LOCS" 0) Reg(A) false) ClearCarry Add(Imm(7) Reg(A))
                 Mov(Reg(A) Data("t" 0) false) Mov(ImmLabelHigh("LOCS" 0) Reg(A) false)
                 Add(Imm(0) Reg(A)) Mov(Reg(A) Reg(X) false) Mov(Data("t" 0) Reg(A) false)
                 Return(true false false))") ==
          expect(R"(Mov(ImmLabelLow("LOCS" 7) Reg(A) false) Mov(Reg(A) Data("t" 0) false)
                    Mov(ImmLabelHigh("LOCS" 7) Reg(A) false) Mov(Reg(A) Reg(X) false)
                    Mov(Data("t" 0) Reg(A) false) Return(true false false))"));
  }
}

TEST_CASE("label_offset_fold keeps the low byte when its carry is read") {
  // The carry out goes into the high byte of another value.
  const std::string text =
      R"(Mov(ImmLabelLow("LOCS" 0) Reg(A) false) ClearCarry Add(Imm(9) Reg(A))
         Mov(Reg(A) Data("t" 0) false) Mov(Data("u" 0) Reg(A) false) Add(Imm(0) Reg(A))
         Mov(Reg(A) Data("t" 1) false))" +
      kRet;
  CHECK(run(text) == expect(text));
}
