// The unit tests of tests/test_redundant_load_after_rmw.py, ported. The
// simulator's runs are in test_redundant_load_after_rmw_sim.cpp.
#include <string>

#include "asm_text.h"
#include "doctest.h"
#include "passes/redundant_load_after_rmw.h"

namespace {

using c6502::asm_text::expect;
using c6502::asm_text::instructions;
using c6502::asm_text::program;

std::string run(std::string_view instrs) {
  return instructions(c6502::passes::apply_redundant_load_after_rmw(program(instrs)));
}

void check_kept(std::string_view instrs) { CHECK(run(instrs) == expect(instrs)); }

// Returns the loop `.top: LDA #0; <rmw>; <load>; <branch> .top; LDA #0;
// RTS`, whose both paths after the branch write A before reading it.
std::string loop(std::string_view rmw, std::string_view load, std::string_view cond) {
  return std::string(R"(Label(".top") Mov(Imm(0) Reg(A) false) )") + std::string(rmw) + " " +
         std::string(load) + " Branch(" + std::string(cond) +
         R"( ".top") Mov(Imm(0) Reg(A) false) Return(false false false))";
}

}  // namespace

TEST_CASE("redundant_load_after_rmw drops the load after a read-modify-write") {
  SUBCASE("DEC before a branch whose both paths write A") {
    // The tail of the rotated countdown loop.
    CHECK(run(R"(Label(".top") Mov(Imm(0) Reg(A) false) Mov(Reg(A) ZP(192 0) false)
                 Dec(ZP(198 0)) Mov(ZP(198 0) Reg(A) false) Branch(PL ".top")
                 Mov(Imm(1) Reg(A) false) Return(false false false))") ==
          expect(R"(Label(".top") Mov(Imm(0) Reg(A) false) Mov(Reg(A) ZP(192 0) false)
                    Dec(ZP(198 0)) Branch(PL ".top") Mov(Imm(1) Reg(A) false)
                    Return(false false false))"));
  }
  SUBCASE("INC of a ZP byte") {
    CHECK(run(R"(Inc(ZP(128 0)) Mov(ZP(128 0) Reg(A) false) Branch(NE ".end") Label(".end")
                 Mov(Imm(0) Reg(A) false) Return(false false false))") ==
          expect(R"(Inc(ZP(128 0)) Branch(NE ".end") Label(".end") Mov(Imm(0) Reg(A) false)
                    Return(false false false))"));
  }
  SUBCASE("INC of a Data byte") {
    CHECK(run(R"(Inc(Data("counter" 0)) Mov(Data("counter" 0) Reg(A) false) Branch(NE ".end")
                 Label(".end") Mov(Imm(0) Reg(A) false) Return(false false false))") ==
          expect(R"(Inc(Data("counter" 0)) Branch(NE ".end") Label(".end")
                    Mov(Imm(0) Reg(A) false) Return(false false false))"));
  }
  SUBCASE("TXA after DEX") {
    CHECK(run(loop("Dec(Reg(X))", "Mov(Reg(X) Reg(A) false)", "PL")) ==
          expect(R"(Label(".top") Mov(Imm(0) Reg(A) false) Dec(Reg(X)) Branch(PL ".top")
                    Mov(Imm(0) Reg(A) false) Return(false false false))"));
  }
  SUBCASE("TYA after DEY") {
    CHECK(run(loop("Dec(Reg(Y))", "Mov(Reg(Y) Reg(A) false)", "PL")) ==
          expect(R"(Label(".top") Mov(Imm(0) Reg(A) false) Dec(Reg(Y)) Branch(PL ".top")
                    Mov(Imm(0) Reg(A) false) Return(false false false))"));
  }
  SUBCASE("TXA after INX") {
    CHECK(run(loop("Inc(Reg(X))", "Mov(Reg(X) Reg(A) false)", "NE")) ==
          expect(R"(Label(".top") Mov(Imm(0) Reg(A) false) Inc(Reg(X)) Branch(NE ".top")
                    Mov(Imm(0) Reg(A) false) Return(false false false))"));
  }
  SUBCASE("a loop back to itself") {
    // The walk stops at the branch it has already visited.
    CHECK(run(R"(Label(".top") Mov(Imm(0) Reg(A) false) Dec(ZP(198 0))
                 Mov(ZP(198 0) Reg(A) false) Branch(PL ".top") Return(false false false))") ==
          expect(R"(Label(".top") Mov(Imm(0) Reg(A) false) Dec(ZP(198 0)) Branch(PL ".top")
                    Return(false false false))"));
  }
  SUBCASE("an element indexed by X") {
    CHECK(run(R"(Dec(IndexedData("t" 0 X)) Mov(IndexedData("t" 0 X) Reg(A) false)
                 Return(false false false))") ==
          expect(R"(Dec(IndexedData("t" 0 X)) Return(false false false))"));
  }
}

TEST_CASE("redundant_load_after_rmw keeps the load when it has to") {
  SUBCASE("A is read on the fall-through") {
    check_kept(R"(Label(".top") Mov(Imm(0) Reg(A) false) Dec(ZP(198 0))
                  Mov(ZP(198 0) Reg(A) false) Branch(PL ".top") Mov(Reg(A) ZP(128 0) false)
                  Return(false false false))");
  }
  SUBCASE("A is read at the branch target") {
    check_kept(R"(Label(".top") Mov(Reg(A) ZP(128 0) false) Mov(Imm(0) Reg(A) false)
                  Dec(ZP(198 0)) Mov(ZP(198 0) Reg(A) false) Branch(PL ".top")
                  Mov(Imm(0) Reg(A) false) Return(false false false))");
  }
  SUBCASE("another byte") {
    check_kept("Dec(ZP(198 0)) Mov(ZP(199 0) Reg(A) false) Mov(Imm(0) Reg(A) false) "
               "Return(false false false)");
  }
  SUBCASE("a load into X") {
    check_kept("Dec(ZP(198 0)) Mov(ZP(198 0) Reg(X) false) Return(false false false)");
  }
  SUBCASE("no read-modify-write") {
    check_kept("Mov(Imm(7) ZP(198 0) false) Mov(ZP(198 0) Reg(A) false) Return(false false false)");
  }
  SUBCASE("a volatile load") {
    check_kept("Dec(ZP(198 0)) Mov(ZP(198 0) Reg(A) true) Return(false false false)");
  }
}
