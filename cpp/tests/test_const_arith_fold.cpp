// The tests of tests/test_const_arith_fold.py, ported.
#include <string>

#include "asm_text.h"
#include "doctest.h"
#include "passes/const_arith_fold.h"

namespace {

using c6502::asm_text::expect;
using c6502::asm_text::instructions;
using c6502::asm_text::program;

const std::string kM = R"(Data("m" 0))";
const std::string kLdaM = "Mov(" + kM + " Reg(A) false) ";
const std::string kRet = "Return(false false false)";
const std::string kRetA = "Return(true false false)";

std::string imm(int k) { return "Imm(" + std::to_string(k) + ")"; }
std::string lda(int k) { return "Mov(" + imm(k) + " Reg(A) false) "; }

std::string run(const std::string& text) {
  return instructions(c6502::passes::apply_const_arith_fold(program(text)));
}

// Checks that the pass leaves `text` as it is.
void check_kept(const std::string& text) { CHECK(run(text) == expect(text)); }

}  // namespace

TEST_CASE("const_arith_fold folds a bitwise operation of two constants") {
  CHECK(run(lda(0x80) + "And(" + imm(0x0F) + " Reg(A))") == expect(lda(0)));
  CHECK(run(lda(0x80) + "Or(" + imm(0x0F) + " Reg(A))") == expect(lda(0x8F)));
}

TEST_CASE("const_arith_fold drops an identity operation after a write of A") {
  CHECK(run(kLdaM + "Or(" + imm(0) + " Reg(A))") == expect(kLdaM));
  CHECK(run(kLdaM + "And(" + imm(0xFF) + " Reg(A))") == expect(kLdaM));
  // ORA #$01 isn't an identity.
  check_kept(kLdaM + "Or(" + imm(1) + " Reg(A))");
  // A compare doesn't write A.
  check_kept("Compare(Reg(A) " + imm(5) + ") Or(" + imm(0) + " Reg(A))");
}

TEST_CASE("const_arith_fold absorbs a load of the operation's identity") {
  CHECK(run(lda(0) + "Or(" + kM + " Reg(A))") == expect(kLdaM));
  CHECK(run(lda(0xFF) + "And(" + kM + " Reg(A))") == expect(kLdaM));
}

TEST_CASE("const_arith_fold makes AND #0 a load of 0") {
  // `(w & 0xFF00) | b` keeps nothing of `w`'s low byte.
  CHECK(run(kLdaM + "And(" + imm(0) + " Reg(A)) Mov(Reg(A) " + kM + " false) " + kRet) ==
        expect(kLdaM + lda(0) + "Mov(Reg(A) " + kM + " false) " + kRet));
}

TEST_CASE("const_arith_fold adds two constants when C and V are dead") {
  const std::string head = lda(0) + "ClearCarry Add(" + imm(1) + " Reg(A)) Mov(Reg(A) " + kM +
                           " false) ";
  CHECK(run(head + "ClearCarry " + kRet) ==
        expect(lda(1) + "Mov(Reg(A) " + kM + " false) ClearCarry " + kRet));
  check_kept(head + lda(0) + "Add(" + imm(0) + " Reg(A)) " + kRet);
}

TEST_CASE("const_arith_fold drops an add of 0 with the carry known") {
  auto zero_add = [](const std::string& flag, const std::string& op, const std::string& after) {
    return kLdaM + flag + " " + op + "(" + imm(0) + R"( Reg(A)) Mov(Reg(A) Data("d" 0) false) )" +
           after + " " + kRet;
  };
  SUBCASE("ADC #0 after CLC") {
    // `p + (n << 8)`'s low byte: the add of the shift's 0 leaves A and C
    // as they were, and the high byte's ADC reads the C.
    const std::string high = R"(Mov(Data("m" 1) Reg(A) false) Add(Data("n" 0) Reg(A)))";
    CHECK(run(zero_add("ClearCarry", "Add", high)) ==
          expect(kLdaM + R"(ClearCarry Mov(Reg(A) Data("d" 0) false) )" + high + " " + kRet));
  }
  SUBCASE("SBC #0 after SEC") {
    CHECK(run(zero_add("SetCarry", "Sub", "")) ==
          expect(kLdaM + R"(SetCarry Mov(Reg(A) Data("d" 0) false) )" + kRet));
  }
  SUBCASE("kept when V is read, since the ADC clears it") {
    check_kept(zero_add("ClearCarry", "Add", R"(Branch(VC ".t") Label(".t"))"));
  }
  SUBCASE("kept with the other carry, since SEC; ADC #0 adds 1") {
    check_kept(zero_add("SetCarry", "Add", ""));
  }
}

TEST_CASE("const_arith_fold adds a constant to a label's high byte") {
  auto high_add = [](const std::string& flag, const std::string& op, int offset) {
    return R"(Mov(ImmLabelHigh("t" )" + std::to_string(offset) + ") Reg(A) false) " + flag +
           " " + op + "(" + imm(1) + R"( Reg(A)) Mov(Reg(A) Data("d" 0) false) )" + kRet;
  };
  // `t + 0x100` on a pointer whose low byte is `t`'s.
  CHECK(run(high_add("ClearCarry", "Add", 128)) ==
        expect(R"(Mov(ImmLabelHigh("t" 384) Reg(A) false) Mov(Reg(A) Data("d" 0) false) )" +
               kRet));
  // `t - 0x100` would be written `t+-256`.
  check_kept(high_add("SetCarry", "Sub", 0));
}

TEST_CASE("const_arith_fold rotates a 1 in for a shift and an add of 1") {
  const std::string head = kLdaM + "ArithmeticShiftLeft(Reg(A)) ClearCarry Add(" + imm(1) +
                           " Reg(A)) ";
  CHECK(run(head + kRetA) == expect(kLdaM + "SetCarry RotateLeft(Reg(A)) " + kRetA));
  // The add leaves C clear, and the rotate leaves the old bit 7.
  check_kept(head + R"(Branch(CS ".t") Label(".t") )" + kRetA);
}

TEST_CASE("const_arith_fold folds two adds of constants into one") {
  auto two_adds = [](const std::string& first, const std::string& flag,
                     const std::string& second, const std::string& after) {
    return kLdaM + first + "(" + imm(0x15) + " Reg(A)) " + flag + " " + second + "(" + imm(1) +
           " Reg(A)) " + after + " " + kRetA;
  };
  SUBCASE("ADC; CLC; ADC") {
    // `a % 21 + 1` after the division loop: ADC #$15; CLC; ADC #$01.
    CHECK(run(two_adds("Add", "ClearCarry", "Add", "")) ==
          expect(kLdaM + "Add(" + imm(0x16) + " Reg(A)) " + kRetA));
  }
  SUBCASE("SBC; CLC; ADC") {
    CHECK(run(two_adds("Sub", "ClearCarry", "Add", "")) ==
          expect(kLdaM + "Sub(" + imm(0x14) + " Reg(A)) " + kRetA));
  }
  SUBCASE("ADC; SEC; SBC") {
    CHECK(run(two_adds("Add", "SetCarry", "Sub", "")) ==
          expect(kLdaM + "Add(" + imm(0x14) + " Reg(A)) " + kRetA));
  }
  SUBCASE("kept when C is read") {
    check_kept(two_adds("Add", "ClearCarry", "Add", R"(Branch(CS ".t") Label(".t"))"));
  }
  SUBCASE("kept with the wrong carry, since SEC; ADC #1 adds 2") {
    check_kept(two_adds("Add", "SetCarry", "Add", ""));
  }
}
