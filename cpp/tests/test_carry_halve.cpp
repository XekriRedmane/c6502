// The tests of tests/test_carry_halve.py that run the pass on asm,
// ported. The ones that compile C and run it stay in Python.
#include <string>

#include "asm_text.h"
#include "doctest.h"
#include "passes/carry_halve.h"

namespace {

using c6502::asm_text::expect;
using c6502::asm_text::instructions;
using c6502::asm_text::program;

const std::string kT = R"(Data("__local_f__t" 0))";
const std::string kS = R"(Data("s" 0))";
const std::string kRet = " Return(true false false)";

// Returns `CLC; ADC s; STA t; LDA #0; ADC #0; LSR; LDA t; ROR`, with
// `zero_load` in place of the `LDA #0`.
std::string halve(const std::string& zero_load = "Mov(Imm(0) Reg(A) false)") {
  return "ClearCarry Add(" + kS + " Reg(A)) Mov(Reg(A) " + kT + " false) " + zero_load +
         " Add(Imm(0) Reg(A)) LogicalShiftRight(Reg(A)) Mov(" + kT +
         " Reg(A) false) RotateRight(Reg(A))";
}

std::string run(const std::string& instrs) {
  return instructions(
      c6502::passes::apply_carry_halve(program(instrs), c6502::passes::AsmContext{}));
}

}  // namespace

TEST_CASE("carry_halve takes bit 8 from the carry of the low byte's add") {
  SUBCASE("the whole high byte goes") {
    CHECK(run(halve() + kRet) ==
          expect("ClearCarry Add(" + kS + " Reg(A)) RotateRight(Reg(A))" + kRet));
  }
  SUBCASE("TYA of a Y that holds 0") {
    CHECK(run("Mov(Imm(0) Reg(Y) false) " + halve("Mov(Reg(Y) Reg(A) false)") + kRet) ==
          expect("Mov(Imm(0) Reg(Y) false) ClearCarry Add(" + kS +
                 " Reg(A)) RotateRight(Reg(A))" + kRet));
  }
}

TEST_CASE("carry_halve keeps the high byte") {
  SUBCASE("TYA of a Y that may hold anything") {
    const std::string text = halve("Mov(Reg(Y) Reg(A) false)") + kRet;
    CHECK(run(text) == expect(text));
  }
  SUBCASE("the slot is read later") {
    const std::string text = halve() + " Mov(" + kT + R"( Data("g" 0) false))" + kRet;
    CHECK(run(text) == expect(text));
  }
  SUBCASE("V is read") {
    const std::string text = halve() + R"( Branch(VS ".l") Label(".l"))" + kRet;
    CHECK(run(text) == expect(text));
  }
}
