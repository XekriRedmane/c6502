// The tests of tests/test_cmp_sbc_fusion.py, ported.
#include <string>

#include "asm_text.h"
#include "doctest.h"
#include "passes/cmp_sbc_fusion.h"

namespace {

using c6502::asm_text::expect;
using c6502::asm_text::instructions;
using c6502::asm_text::program;

std::string run(const std::string& instrs) {
  return instructions(c6502::passes::apply_cmp_sbc_fusion(program(instrs)));
}

const std::string kLoad = R"(Mov(Data("M" 0) Reg(A) false) )";
const std::string kCmpN = R"(Compare(Reg(A) Data("N" 0)) )";
const std::string kSbcN = R"(Sub(Data("N" 0) Reg(A)) )";
const std::string kRts = "Return(false false false) ";
const std::string kRtsA = "Return(true false false) ";

// The shape of six with a `BCC .skip`, `RTS` with A after the SBC, and
// `target_tail` after `.skip`.
std::string shape(const std::string& target_tail) {
  return kLoad + kCmpN + R"(Branch(CC ".skip") Label(".cont") SetCarry )" + kSbcN +
         kRtsA + R"(Label(".skip") )" + target_tail;
}

}  // namespace

TEST_CASE("cmp_sbc_fusion moves the SBC ahead of the branch") {
  SUBCASE("LDA; CMP; BCC; label; SEC; SBC") {
    CHECK(run(kLoad + kCmpN + R"(Branch(CC ".skip") Label(".cont") SetCarry )" + kSbcN +
              kRts + R"(Label(".skip") )" + kRts) ==
          expect(kLoad + "SetCarry " + kSbcN + R"(Branch(CC ".skip") Label(".cont") )" +
                 kRts + R"(Label(".skip") )" + kRts));
  }
  SUBCASE("without a label") {
    const std::string sta = R"(Mov(Reg(A) Data("T" 0) false) )";
    CHECK(run(kLoad + kCmpN + R"(Branch(CC ".skip") SetCarry )" + kSbcN + sta +
              R"(Label(".skip") )" + kRts) ==
          expect(kLoad + "SetCarry " + kSbcN + R"(Branch(CC ".skip") )" + sta +
                 R"(Label(".skip") )" + kRts));
  }
  SUBCASE("after a BCC without the SEC that carry_known dropped") {
    // The fall-through of a BCC has C set, and the moved SBC gets the
    // SEC back.
    const std::string sta = R"(Mov(Reg(A) Data("T" 0) false) )";
    CHECK(run(kLoad + kCmpN + R"(Branch(CC ".skip") )" + kSbcN + sta +
              R"(Label(".skip") )" + kRts) ==
          expect(kLoad + "SetCarry " + kSbcN + R"(Branch(CC ".skip") )" + sta +
                 R"(Label(".skip") )" + kRts));
  }
  SUBCASE("when the target loads A again") {
    CHECK(run(shape("Mov(Imm(0) Reg(A) false) " + kRtsA)) ==
          expect(kLoad + "SetCarry " + kSbcN + R"(Branch(CC ".skip") Label(".cont") )" +
                 kRtsA + R"(Label(".skip") Mov(Imm(0) Reg(A) false) )" + kRtsA));
  }
}

TEST_CASE("cmp_sbc_fusion leaves other shapes alone") {
  SUBCASE("a BCS without the SEC") {
    // After a BCS the fall-through has C clear, so the SBC there
    // subtracts one more and isn't the compare's subtraction.
    const std::string text =
        kLoad + kCmpN + R"(Branch(CS ".skip") )" + kSbcN + R"(Label(".skip") )" + kRts;
    CHECK(run(text) == expect(text));
  }
  SUBCASE("A read at the target") {
    // `.skip` returns A, which the CMP left as M.
    const std::string text = kLoad + R"(Compare(Reg(A) Imm(3)) Branch(CC ".skip") )" +
                             "SetCarry Sub(Imm(3) Reg(A)) " + R"(Label(".skip") )" + kRtsA;
    CHECK(run(text) == expect(text));
  }
  SUBCASE("a label that a jump targets") {
    // Another path could reach `.cont` with another A.
    const std::string text = R"(Jump(".cont" []) )" + kLoad + kCmpN +
                             R"(Branch(CC ".skip") Label(".cont") SetCarry )" + kSbcN;
    CHECK(run(text) == expect(text));
  }
  SUBCASE("different operands") {
    const std::string text = kLoad + R"(Compare(Reg(A) Data("N1" 0)) )" +
                             R"(Branch(CC ".skip") Label(".cont") SetCarry )" +
                             R"(Sub(Data("N2" 0) Reg(A)))";
    CHECK(run(text) == expect(text));
  }
  SUBCASE("a BVC") {
    // BVC and BVS read V, which SBC sets and CMP doesn't.
    const std::string text =
        kLoad + kCmpN + R"(Branch(VC ".skip") Label(".cont") SetCarry )" + kSbcN;
    CHECK(run(text) == expect(text));
  }
  SUBCASE("A returned at the target") {
    // The taken path returns M in A, and after the fusion it would
    // return M - N.
    const std::string text = shape(kRtsA);
    CHECK(run(text) == expect(text));
  }
  SUBCASE("V read at the target") {
    // A `BVS` at the target reads the V that CMP leaves alone and SBC
    // sets.
    const std::string text =
        shape(R"(Branch(VS ".v") )" + kRts + R"(Label(".v") )" + kRts);
    CHECK(run(text) == expect(text));
  }
}
