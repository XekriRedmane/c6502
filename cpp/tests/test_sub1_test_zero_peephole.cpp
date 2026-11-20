// The tests of tests/test_sub1_test_zero_peephole.py, ported.
#include <string>
#include <string_view>

#include "asm_text.h"
#include "doctest.h"
#include "passes/sub1_test_zero_peephole.h"

namespace {

using c6502::asm_text::expect;
using c6502::asm_text::instructions;
using c6502::asm_text::program;

std::string run(std::string_view instrs) {
  return instructions(c6502::passes::apply_sub1_test_zero_peephole(program(instrs)));
}

// `LDA n; SEC; SBC #1; TAX`, the four instructions the rewrite keeps.
const std::string kHead =
    R"(Mov(Data("n" 0) Reg(A) false) SetCarry Sub(Imm(1) Reg(A)) Mov(Reg(A) Reg(X) false) )";
// `LDA #0; CMP n; BCC .l`.
const std::string kTest = R"(Mov(Imm(0) Reg(A) false) Compare(Reg(A) Data("n" 0))
                             Branch(CC ".l") )";
const std::string kReload = "Mov(Imm(5) Reg(A) false) ";
const std::string kRet = "Return(false false false) ";

}  // namespace

TEST_CASE("sub1_test_zero drops the compare and flips the branch") {
  const std::string tail =
      kReload + R"(Mov(Reg(A) Data("out" 0) false) Label(".l") )" + kRet;
  CHECK(run(kHead + kTest + tail) == expect(kHead + R"(Branch(CS ".l") )" + tail));
}

TEST_CASE("sub1_test_zero keeps the compare when its outputs are read") {
  auto unchanged = [](const std::string& text) { CHECK(run(text) == expect(text)); };
  SUBCASE("A read on the fall-through") {
    // `STA out` reads the 0 that `LDA #0` left in A. After the rewrite A
    // would hold n - 1.
    unchanged(kHead + kTest + R"(Mov(Reg(A) Data("out" 0) false) Label(".l") )" + kRet);
  }
  SUBCASE("A read at the target") {
    unchanged(kHead + kTest + kRet + R"(Label(".l") Return(true false false))");
  }
  SUBCASE("Z read at the target") {
    // Z comes from the `CMP` in the original and from the `TAX` after
    // the rewrite.
    unchanged(kHead + kTest + kRet + R"(Label(".l") Branch(EQ ".z") )" + kRet +
              R"(Label(".z") )" + kRet);
  }
  SUBCASE("C read on the fall-through") {
    // C is set exactly when n == 0 after the `CMP`, and exactly when
    // n != 0 after the `SBC`, so a later `ADC` would add another carry.
    unchanged(kHead + kTest + kReload + "Add(Imm(0) Reg(A)) Return(true false false) " +
              R"(Label(".l") )" + kRet);
  }
}
