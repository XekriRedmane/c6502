// The tests of tests/test_carry_in_add.py that run the pass on asm,
// ported. The ones that compile C and run it stay in Python.
#include <string>

#include "asm_text.h"
#include "doctest.h"
#include "passes/carry_in_add.h"

namespace {

using c6502::asm_text::expect;
using c6502::asm_text::instructions;
using c6502::asm_text::program;

const std::string kM = R"(Data("m" 0))";
const std::string kH = R"(Data("__local_f__h" 0))";
const std::string kT0 = R"(Data("__local_f__t0" 0))";
const std::string kT1 = R"(Data("__local_f__t1" 0))";
const std::string kR0 = R"(Data("r" 0))";
const std::string kR1 = R"(Data("r" 1))";
const std::string kRet = " Return(false false false)";

std::string mov(const std::string& src, const std::string& dst) {
  return "Mov(" + src + " " + dst + " false) ";
}

// Returns the two 16-bit adds of `p + b + 1` through `t0` and `t1`.
std::string two_adds(const std::string& h = kH, const std::string& t1 = kT1) {
  return "ClearCarry Add(" + kM + " Reg(A)) " + mov("Reg(A)", kT0) + mov(h, "Reg(A)") +
         "Add(Imm(0) Reg(A)) " + mov("Reg(A)", t1) + mov(kT0, "Reg(A)") +
         "ClearCarry Add(Imm(1) Reg(A)) " + mov("Reg(A)", kR0) + mov(t1, "Reg(A)") +
         "Add(Imm(0) Reg(A)) " + mov("Reg(A)", kR1);
}

std::string run(const std::string& instrs) {
  return instructions(
      c6502::passes::apply_carry_in_add(program(instrs), c6502::passes::AsmContext{}));
}

}  // namespace

TEST_CASE("carry_in_add adds the 1 through the carry") {
  SUBCASE("the two adds become one") {
    CHECK(run(two_adds() + kRet) ==
          expect("SetCarry Add(" + kM + " Reg(A)) " + mov("Reg(A)", kR0) + mov(kH, "Reg(A)") +
                 "Add(Imm(0) Reg(A)) " + mov("Reg(A)", kR1) + kRet));
  }
  SUBCASE("the high byte added in place") {
    CHECK(run(two_adds(kH, kH) + kRet) ==
          expect("SetCarry Add(" + kM + " Reg(A)) " + mov("Reg(A)", kR0) + mov(kH, "Reg(A)") +
                 "Add(Imm(0) Reg(A)) " + mov("Reg(A)", kR1) + kRet));
  }
}

TEST_CASE("carry_in_add keeps the two adds") {
  SUBCASE("C is read") {
    const std::string text = two_adds() + R"(Branch(CS ".l") Label(".l"))" + kRet;
    CHECK(run(text) == expect(text));
  }
  SUBCASE("the high byte is a global, which the store to r0 might be") {
    const std::string text = two_adds(R"(Data("g" 0))") + kRet;
    CHECK(run(text) == expect(text));
  }
  SUBCASE("a temporary is read later") {
    const std::string text = two_adds() + mov(kT1, R"(Data("g" 0))") + kRet;
    CHECK(run(text) == expect(text));
  }
}
