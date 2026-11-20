// The tests of tests/test_dec_peephole.py, ported.
#include <string>
#include <string_view>

#include "asm_text.h"
#include "doctest.h"
#include "passes/dec_peephole.h"

namespace {

using c6502::asm_text::expect;
using c6502::asm_text::instructions;
using c6502::asm_text::program;

std::string run(std::string_view instrs) {
  return instructions(c6502::passes::apply_dec_peephole(program(instrs)));
}

// `LDA m; SEC; SBC #1; STA m`.
std::string sub1(const std::string& m) {
  return "Mov(" + m + " Reg(A) false) SetCarry Sub(Imm(1) Reg(A)) Mov(Reg(A) " + m +
         " false) ";
}

const std::string kRet = "Return(false false false)";

// `sp--` on a 2-byte pointer, as carry_inc leaves it, followed by
// `after`.
std::string two_byte(const std::string& after) {
  return sub1(R"(Data("sp" 0))") +
         R"(Branch(CS ".carry_inc@0") Dec(Data("sp" 1)) Label(".carry_inc@0") )" + after;
}

}  // namespace

TEST_CASE("dec_peephole folds a one-byte subtract of 1") {
  SUBCASE("into DEC") {
    CHECK(run(sub1("ZP(128 0)") + kRet) == expect("Dec(ZP(128 0)) " + kRet));
  }
  SUBCASE("when C is written again before it is read") {
    const std::string tail =
        "Mov(ZP(130 0) Reg(A) false) ClearCarry Add(Imm(1) Reg(A)) Return(true false false)";
    CHECK(run(sub1("ZP(128 0)") + tail) == expect("Dec(ZP(128 0)) " + tail));
  }
}

TEST_CASE("dec_peephole keeps a subtract whose borrow is read") {
  // A 16-bit `x - 1` whose high byte isn't stored back in place. The
  // `LDA hi; SBC #0; TAX` reads the low byte's borrow, which DEC doesn't
  // produce.
  const std::string text = sub1("ZP(128 0)") +
                           "Mov(ZP(129 0) Reg(A) false) Sub(Imm(0) Reg(A)) "
                           "Mov(Reg(A) Reg(X) false) " +
                           kRet;
  CHECK(run(text) == expect(text));
}

TEST_CASE("dec_peephole tests the low byte of a 2-byte decrement first") {
  SUBCASE("when A and the flags are dead") {
    const std::string load = R"(Mov(Data("v" 0) Reg(A) false) )";
    CHECK(run(two_byte(load + kRet)) ==
          expect(R"(Mov(Data("sp" 0) Reg(A) false) Branch(NE ".carry_inc@0")
                    Dec(Data("sp" 1)) Label(".carry_inc@0") Dec(Data("sp" 0)) )" +
                 load + kRet));
  }
  SUBCASE("kept when A is read") {
    // A holds the new low byte after the SBC form, and the old one after
    // the DEC form.
    const std::string text = two_byte("Return(true false false)");
    CHECK(run(text) == expect(text));
  }
  SUBCASE("kept after a store of the low byte") {
    // `v = f() - 1`. A holds the low byte from the call, whose flags
    // aren't A's, so the `BNE` form would need `ORA #$00`.
    const std::string text =
        R"(Call("f" [] []) Mov(Reg(A) Data("sp" 0) false) )" + two_byte(kRet);
    CHECK(run(text) == expect(text));
  }
  SUBCASE("after a store of a value that set the flags") {
    // `v = (x & 0xC0) - 1`. The AND set N and Z from A.
    const std::string out =
        run(R"(And(Imm(192) Reg(A)) Mov(Reg(A) Data("sp" 0) false) )" + two_byte(kRet));
    CHECK(out.find("Branch(NE \".carry_inc@0\")") != std::string::npos);
  }
}
