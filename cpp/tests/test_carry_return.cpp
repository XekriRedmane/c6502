// The tests of tests/test_carry_return.py that run carry_return's
// peepholes on asm, ported. The ones that compile C, and those of other
// passes, stay in Python.
#include <string>

#include "asm_text.h"
#include "doctest.h"
#include "passes/carry_return.h"

namespace {

using c6502::asm_text::expect;
using c6502::asm_text::instructions;
using c6502::asm_text::program;
using c6502::passes::AsmContext;
using c6502::passes::SymbolAddrs;

std::string lda(int k) { return "Mov(Imm(" + std::to_string(k) + ") Reg(A) false) "; }
std::string cmp(int k) { return "Compare(Reg(A) Imm(" + std::to_string(k) + ")) "; }
const std::string kRetC = "Return(false false true) ";

std::string tails(const std::string& text) {
  return instructions(c6502::passes::apply_carry_return_tails(program(text)));
}

std::string const_fold(const std::string& text) {
  return instructions(c6502::passes::apply_carry_const_fold(program(text)));
}

std::string branch_fold(const std::string& text, const SymbolAddrs* slots = nullptr) {
  return instructions(
      c6502::passes::apply_carry_branch_fold(program(text), AsmContext{}, slots));
}

}  // namespace

TEST_CASE("carry_const_fold folds a compare of a known constant") {
  SUBCASE("to SEC and CLC") {
    CHECK(const_fold(lda(1) + cmp(1) + kRetC) == expect("SetCarry " + kRetC));
    CHECK(const_fold(lda(0) + cmp(1) + kRetC) == expect("ClearCarry " + kRetC));
  }
  SUBCASE("with A live, only the compare folds, from the 1 A holds") {
    CHECK(const_fold(lda(1) + cmp(1) + "Return(true false true)") ==
          expect(lda(1) + "SetCarry Return(true false true)"));
  }
  SUBCASE("from a register") {
    // `return false;` after `LDY #0`, as `CPY #1` once the 0 is loaded
    // with TYA and `TYA; CMP #1` is folded.
    CHECK(const_fold("Mov(Imm(0) Reg(Y) false) Compare(Reg(Y) Imm(1)) " + kRetC) ==
          expect("Mov(Imm(0) Reg(Y) false) ClearCarry " + kRetC));
  }
  SUBCASE("not across a label, since another path may bring another Y") {
    const std::string text =
        R"(Mov(Imm(0) Reg(Y) false) Label(".l") Compare(Reg(Y) Imm(1)) )" + kRetC;
    CHECK(const_fold(text) == expect(text));
  }
  SUBCASE("not after the register changes") {
    const std::string text =
        "Mov(Imm(0) Reg(Y) false) Inc(Reg(Y)) Compare(Reg(Y) Imm(1)) " + kRetC;
    CHECK(const_fold(text) == expect(text));
  }
}

TEST_CASE("carry_return_tails copies the tail through a jump and a fall-through") {
  CHECK(tails(R"(Branch(EQ "T") )" + lda(0) + R"(Jump("E" []) Label("T") )" + lda(1) +
              R"(Label("E") )" + cmp(1) + kRetC) ==
        expect(R"(Branch(EQ "T") )" + lda(0) + cmp(1) + kRetC + R"(Label("T") )" + lda(1) +
               cmp(1) + kRetC + R"(Label("E") )" + cmp(1) + kRetC));
}

TEST_CASE("carry_branch_fold branches on the captured carry") {
  const std::string capture = R"(Call("g" [] []) )" + lda(0) + "RotateLeft(Reg(A)) ";
  const std::string tail = R"(ClearCarry Label("L") Return(false false false))";
  SUBCASE("BNE becomes BCS") {
    CHECK(branch_fold(capture + R"(Branch(NE "L") )" + tail) ==
          expect(R"(Call("g" [] []) Branch(CS "L") )" + tail));
  }
  SUBCASE("BEQ becomes BCC") {
    CHECK(branch_fold(capture + R"(Branch(EQ "L") )" + tail) ==
          expect(R"(Call("g" [] []) Branch(CC "L") )" + tail));
  }
  SUBCASE("kept when the carry is read") {
    // After `ROL A` C is 0, and the ADC on the fall-through reads it.
    // Without the ROL, C would be the callee's.
    const std::string text = capture +
                             R"(Branch(NE "L") Mov(Data("d" 0) Reg(A) false)
                                Add(Imm(1) Reg(A)) Mov(Reg(A) Data("d" 0) false)
                                Label("L") Return(false false false))";
    CHECK(branch_fold(text) == expect(text));
  }
  SUBCASE("a capture turned back into C goes") {
    CHECK(branch_fold(capture + cmp(1) + kRetC) == expect(R"(Call("g" [] []) )" + kRetC));
  }
}

TEST_CASE("carry_branch_fold on a capture that is stored and loaded for the test") {
  const std::string v = R"(Data("__local_f__v" 0))";
  const std::string flag = R"(Data("__local_f__flag" 0))";
  const std::string out = R"(Data("out" 0))";
  const SymbolAddrs slots = {{"__local_f__v", 0x80}, {"__local_f__flag", 0x81}};
  auto shape = [&](const std::string& between, const std::string& after_branch) {
    return R"(Call("g" [] []) Mov(Reg(A) )" + v + " false) " + lda(0) +
           "RotateLeft(Reg(A)) Mov(Reg(A) " + flag + " false) Mov(" + v +
           " Reg(A) false) Mov(Reg(A) " + out + " false) " + between + " Mov(" + flag +
           R"( Reg(A) false) Branch(NE "T") )" + after_branch + " " + lda(1) +
           R"(Return(true false false) Label("T") )" + lda(2) + "Return(true false false)";
  };
  SUBCASE("branches on C after the other work") {
    CHECK(branch_fold(shape("", ""), &slots) ==
          expect(R"(Call("g" [] []) Mov(Reg(A) )" + v + " false) Mov(" + v +
                 " Reg(A) false) Mov(Reg(A) " + out + R"( false) Branch(CS "T") )" + lda(1) +
                 R"(Return(true false false) Label("T") )" + lda(2) +
                 "Return(true false false)"));
  }
  SUBCASE("kept when C changes in between") {
    const std::string text = shape("ClearCarry", "");
    CHECK(branch_fold(text, &slots) == expect(text));
  }
  SUBCASE("kept when the flag is written in between") {
    const std::string text = shape("Mov(Reg(A) " + flag + " false)", "");
    CHECK(branch_fold(text, &slots) == expect(text));
  }
  SUBCASE("kept when the flag is read again") {
    const std::string text = shape("", "Mov(" + flag + " Reg(A) false)");
    CHECK(branch_fold(text, &slots) == expect(text));
  }
  SUBCASE("kept across a call") {
    const std::string text = shape(R"(Call("h" [] []))", "");
    CHECK(branch_fold(text, &slots) == expect(text));
  }
}
