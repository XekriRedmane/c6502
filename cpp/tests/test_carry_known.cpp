// The tests of tests/test_carry_known.py, ported.
#include <string>

#include "asm_text.h"
#include "doctest.h"
#include "passes/carry_known.h"

namespace {

using c6502::asm_text::expect;
using c6502::asm_text::instructions;
using c6502::asm_text::program;

const std::string kS = R"(Data("S" 0))";
const std::string kLdaS = "Mov(" + kS + " Reg(A) false) ";
const std::string kSbcS = "Sub(" + kS + " Reg(A)) ";
const std::string kAdcS = "Add(" + kS + " Reg(A)) ";
const std::string kRet = "Return(true false false) ";

std::string run(const std::string& text) {
  return instructions(c6502::passes::apply_carry_known(program(text)));
}

}  // namespace

TEST_CASE("carry_known drops a setter where C is known") {
  CHECK(run("SetCarry " + kLdaS + "SetCarry " + kSbcS + R"(Branch(CC ".lt") SetCarry )" +
            kSbcS + kRet + R"(Label(".lt") ClearCarry )" + kAdcS + kRet) ==
        expect("SetCarry " + kLdaS + kSbcS + R"(Branch(CC ".lt") )" + kSbcS + kRet +
               R"(Label(".lt") )" + kAdcS + kRet));
}

TEST_CASE("carry_known moves a setter in front of a loop") {
  // do_rwts_on_sector: C is set on the back edge.
  const std::string loop = kSbcS + R"(Branch(CC ".done") Inc(Data("track" 0))
                                      Jump(".loop" []) Label(".done") )" +
                           kRet;
  CHECK(run("Mov(" + kS + R"( Reg(X) false) Label(".loop") SetCarry )" + loop) ==
        expect("Mov(" + kS + R"( Reg(X) false) SetCarry Label(".loop") )" + loop));
}

TEST_CASE("carry_known keeps the setter") {
  SUBCASE("a path back changes C") {
    const std::string text = "Mov(" + kS + R"( Reg(X) false) Label(".loop") SetCarry )" +
                             kSbcS + R"(Branch(CC ".done") Compare(Reg(A) )" + kS +
                             R"() Jump(".loop" []) Label(".done") )" + kRet;
    CHECK(run(text) == expect(text));
  }
  SUBCASE("after a call") {
    // The first SEC is dead, since the call overwrites C. The second is
    // needed, since C is unknown after the call.
    const std::string rest = R"(Call("g" [] []) SetCarry )" + kSbcS + kRet;
    CHECK(run("SetCarry " + rest) == expect(rest));
  }
  SUBCASE("a dead setter before the one the add reads") {
    // One CLC stays before the ADC. Done in one step, the first went as
    // dead and the second as redundant after it.
    CHECK(run(R"(Call("g" [] []) ClearCarry )" + kLdaS + "ClearCarry " + kAdcS + kRet) ==
          expect(R"(Call("g" [] []) ClearCarry )" + kLdaS + kAdcS + kRet));
  }
}

TEST_CASE("carry_known drops an add of 0 that leaves A and C as they were") {
  SUBCASE("the high byte of an address plus 0") {
    // `P = &LOCS[0]` after the low byte's `ADC #0` was folded.
    CHECK(run(R"(Mov(ImmLabelLow("LOCS" 0) Reg(A) false) ClearCarry
                 Mov(Reg(A) Data("P" 0) false) Mov(ImmLabelHigh("LOCS" 0) Reg(A) false)
                 Add(Imm(0) Reg(A)) Mov(Reg(A) Data("P" 1) false) Return(false false false))") ==
          expect(R"(Mov(ImmLabelLow("LOCS" 0) Reg(A) false) Mov(Reg(A) Data("P" 0) false)
                    Mov(ImmLabelHigh("LOCS" 0) Reg(A) false) Mov(Reg(A) Data("P" 1) false)
                    Return(false false false))"));
  }
  SUBCASE("a subtract of 0 with C set") {
    CHECK(run("SetCarry " + kLdaS + R"(Sub(Imm(0) Reg(A)) Branch(EQ ".z") Label(".z") )" +
              kRet) == expect(kLdaS + R"(Branch(EQ ".z") Label(".z") )" + kRet));
  }
  SUBCASE("kept with C unknown") {
    const std::string text = kLdaS + "Add(Imm(0) Reg(A)) " + kRet;
    CHECK(run(text) == expect(text));
  }
  SUBCASE("kept with V read") {
    const std::string text = "ClearCarry " + kLdaS +
                             R"(Add(Imm(0) Reg(A)) Branch(VC ".v") Label(".v") )" + kRet;
    CHECK(run(text) == expect(text));
  }
}
