// The tests of tests/test_dec_inc_branch_fold.py, ported. Each branch
// target that a test's instructions don't define is followed by a label
// and a return, since the pass checks A at the target and counts a
// target outside the function as reading it.
#include <string>
#include <string_view>

#include "asm_text.h"
#include "doctest.h"
#include "passes/dec_inc_branch_fold.h"

namespace {

using c6502::asm_text::expect;
using c6502::asm_text::instructions;
using c6502::asm_text::program;

std::string run(std::string_view instrs) {
  return instructions(c6502::passes::apply_dec_inc_branch_fold(program(instrs)));
}

const std::string kRet = "Return(false false false) ";
const std::string kLdaM = R"(Mov(Data("M" 0) Reg(A) false) )";
// The labels of the targets the tests branch to, each with a return.
const std::string kLoop = R"(Label(".loop") )" + kRet;
const std::string kEnd = R"(Label(".end") )" + kRet;

}  // namespace

TEST_CASE("dec_inc_branch_fold drops the load before a branch on N or Z") {
  SUBCASE("DEC; LDA; BPL") {
    CHECK(run(R"(Dec(Data("M" 0)) )" + kLdaM + R"(Branch(PL ".loop") )" + kRet + kLoop) ==
          expect(R"(Dec(Data("M" 0)) Branch(PL ".loop") )" + kRet + kLoop));
  }
  SUBCASE("INC; LDA; BEQ") {
    CHECK(run(R"(Inc(Data("M" 0)) )" + kLdaM + R"(Branch(EQ ".end") )" + kRet + kEnd) ==
          expect(R"(Inc(Data("M" 0)) Branch(EQ ".end") )" + kRet + kEnd));
  }
  SUBCASE("through a label nothing branches to") {
    CHECK(run(R"(Dec(Data("M" 0)) Label(".passive") )" + kLdaM + R"(Branch(PL ".loop") )" +
              kRet + kLoop) ==
          expect(R"(Dec(Data("M" 0)) Label(".passive") Branch(PL ".loop") )" + kRet + kLoop));
  }
}

TEST_CASE("dec_inc_branch_fold keeps the load") {
  auto unchanged = [](const std::string& text) { CHECK(run(text) == expect(text)); };
  SUBCASE("after a label that a jump targets") {
    unchanged(R"(Jump(".tgt" []) Dec(Data("M" 0)) Label(".tgt") )" + kLdaM +
              R"(Branch(PL ".loop") )" + kRet + kLoop);
  }
  SUBCASE("before a BCC") {
    // BCC reads C, which neither DEC nor the load sets.
    unchanged(R"(Dec(Data("M" 0)) )" + kLdaM + R"(Branch(CC ".loop") )" + kRet + kLoop);
  }
  SUBCASE("when A is read on the fall-through") {
    unchanged(R"(Dec(Data("M" 0)) )" + kLdaM + R"(Branch(PL ".loop")
                 Mov(Reg(A) Data("X" 0) false) )" +
              kRet + kLoop);
  }
  SUBCASE("when A is read at the target") {
    // `t = --timer; if (t == 0) {...} return t;` reads t in A at the
    // branch target.
    unchanged(R"(Dec(Data("timer" 0)) Mov(Data("timer" 0) Reg(A) false) Branch(NE ".t")
                 Mov(Imm(7) Reg(A) false) Return(true false false) Label(".t")
                 Return(true false false))");
  }
}
