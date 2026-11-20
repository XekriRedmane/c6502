// The tests of tests/test_via_a_store_fold.py of `apply_via_a_store_fold`,
// ported. Those of `apply_copy_through_index` go with its port.
#include <string>

#include "asm_text.h"
#include "doctest.h"
#include "passes/via_a_store_fold.h"

namespace {

using c6502::asm_text::expect;
using c6502::asm_text::instructions;
using c6502::asm_text::program;

std::string run(const std::string& instrs) {
  return instructions(c6502::passes::apply_via_a_store_fold(program(instrs)));
}

const std::string kRet = "Return(false false false) ";
const std::string kM = R"(Data("m" 0))";

}  // namespace

TEST_CASE("via_a_store_fold folds a transfer and a store") {
  SUBCASE("TXA; STA m") {
    // A and the flags are dead, since a return comes next.
    CHECK(run("Mov(Reg(X) Reg(A) false) Mov(Reg(A) " + kM + " false) " + kRet) ==
          expect("Mov(Reg(X) " + kM + " false) " + kRet));
  }
  SUBCASE("TYA; STA zp") {
    CHECK(run("Mov(Reg(Y) Reg(A) false) Mov(Reg(A) ZP(133 0) false) " + kRet) ==
          expect("Mov(Reg(Y) ZP(133 0) false) " + kRet));
  }
}

TEST_CASE("via_a_store_fold leaves the pair alone") {
  SUBCASE("A is read after") {
    const std::string text = "Mov(Reg(X) Reg(A) false) Mov(Reg(A) " + kM +
                             R"( false) Mov(Reg(A) Data("n" 0) false))";
    CHECK(run(text) == expect(text));
  }
  SUBCASE("the flags are read after") {
    // The branch reads the N and Z that the TXA set.
    const std::string text = "Mov(Reg(X) Reg(A) false) Mov(Reg(A) " + kM +
                             R"( false) Branch(NE ".skip") )" + kRet;
    CHECK(run(text) == expect(text));
  }
  SUBCASE("an indirect destination") {
    // STX has no indirect addressing mode.
    const std::string text = "Mov(Reg(X) Reg(A) false) Mov(Reg(A) Indirect(0) false) " + kRet;
    CHECK(run(text) == expect(text));
  }
  SUBCASE("a volatile transfer") {
    const std::string text = "Mov(Reg(X) Reg(A) true) Mov(Reg(A) " + kM + " false) " + kRet;
    CHECK(run(text) == expect(text));
  }
}
