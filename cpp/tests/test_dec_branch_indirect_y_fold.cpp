// Tests of dec_branch_indirect_y_fold, which has no Python unit tests of
// its own.
#include <string>

#include "asm_text.h"
#include "doctest.h"
#include "passes/dec_branch_indirect_y_fold.h"

namespace {

using c6502::asm_text::expect;
using c6502::asm_text::instructions;
using c6502::asm_text::program;

std::string run(const std::string& instrs) {
  return instructions(c6502::passes::apply_dec_branch_indirect_y_fold(program(instrs)));
}

}  // namespace

TEST_CASE("dec_branch_indirect_y_fold uses the Y that DEY left at 0") {
  CHECK(run(R"(Label(".l") Dec(Reg(Y)) Branch(NE ".l") Compare(Reg(A) Indirect(0))
               Mov(Indirect(0) Reg(A) false) Return(true false false))") ==
        expect(R"(Label(".l") Dec(Reg(Y)) Branch(NE ".l") Compare(Reg(A) IndirectY)
                  Mov(Indirect(0) Reg(A) false) Return(true false false))"));
}

TEST_CASE("dec_branch_indirect_y_fold leaves other shapes alone") {
  SUBCASE("a label after the branch") {
    const std::string text =
        R"(Label(".l") Dec(Reg(Y)) Branch(NE ".l") Label(".m") Compare(Reg(A) Indirect(0))
           Return(true false false))";
    CHECK(run(text) == expect(text));
  }
  SUBCASE("an offset other than 0") {
    const std::string text = R"(Label(".l") Dec(Reg(Y)) Branch(NE ".l")
                                Mov(Reg(A) Indirect(1) false) Return(false false false))";
    CHECK(run(text) == expect(text));
  }
  SUBCASE("a decrement of X") {
    const std::string text = R"(Label(".l") Dec(Reg(X)) Branch(NE ".l")
                                Compare(Reg(A) Indirect(0)) Return(true false false))";
    CHECK(run(text) == expect(text));
  }
}
