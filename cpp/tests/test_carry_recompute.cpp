// The tests of tests/test_carry_recompute.py that run the pass on asm,
// ported.
#include <string>

#include "asm_text.h"
#include "doctest.h"
#include "passes/carry_recompute.h"

namespace {

using c6502::asm_text::expect;
using c6502::asm_text::instructions;
using c6502::asm_text::program;

const std::string kHead =
    R"(Mov(Data("S" 0) Reg(A) false) ArithmeticShiftLeft(Reg(A)) Mov(Reg(A) Data("L" 0) false) )";
const std::string kRecompute = R"(Mov(Data("S" 0) Reg(A) false) ArithmeticShiftLeft(Reg(A)) )";
const std::string kTail =
    R"(Mov(Data("S" 1) Reg(A) false) RotateLeft(Reg(A)) Mov(Reg(A) Data("H" 0) false)
       Return(false false false))";

// Returns `LDA S; ASL; STA L; <between>; LDA S; ASL; LDA S+1; ROL; STA
// H`.
std::string chain(const std::string& between = "") {
  return kHead + between + " " + kRecompute + kTail;
}

std::string run(const std::string& instrs) {
  return instructions(c6502::passes::apply_carry_recompute_drop(program(instrs)));
}

}  // namespace

TEST_CASE("carry_recompute_drop drops a shift that sets C to what it holds") {
  CHECK(run(chain()) == expect(kHead + kTail));
}

TEST_CASE("carry_recompute_drop keeps the shift") {
  SUBCASE("the byte changes") {
    CHECK(run(chain(R"(Inc(Data("S" 0)))")) == expect(chain(R"(Inc(Data("S" 0)))")));
  }
  SUBCASE("a store through a pointer") {
    const std::string text = chain("Mov(Reg(A) IndirectZpY(128) false)");
    CHECK(run(text) == expect(text));
  }
  SUBCASE("C changes") {
    CHECK(run(chain("ClearCarry")) == expect(chain("ClearCarry")));
  }
  SUBCASE("a label") {
    CHECK(run(chain(R"(Label(".l"))")) == expect(chain(R"(Label(".l"))")));
  }
  SUBCASE("A is read") {
    // The shifted value is stored, so the shift has to stay.
    const std::string text = kHead + kRecompute +
                             R"(Mov(Reg(A) Data("H" 0) false) Return(false false false))";
    CHECK(run(text) == expect(text));
  }
}
