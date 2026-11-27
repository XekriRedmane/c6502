// The unit tests of tests/test_direct_index_load.py, ported. The
// simulator's runs are in test_direct_index_load_sim.cpp.
#include "asm_text.h"
#include "doctest.h"
#include "passes/direct_index_load.h"

namespace {

using c6502::asm_text::expect;
using c6502::asm_text::instructions;
using c6502::asm_text::program;

std::string run(std::string_view instrs) {
  return instructions(c6502::passes::apply_direct_index_load(program(instrs)));
}

void check_kept(std::string_view instrs) { CHECK(run(instrs) == expect(instrs)); }

}  // namespace

TEST_CASE("direct_index_load fuses LDA M; TAX when A is dead") {
  SUBCASE("ZP into X, with A written next") {
    CHECK(run("Mov(ZP(128 0) Reg(A) false) Mov(Reg(A) Reg(X) false) Mov(Imm(66) Reg(A) false)") ==
          expect("Mov(ZP(128 0) Reg(X) false) Mov(Imm(66) Reg(A) false)"));
  }
  SUBCASE("Data into Y, before a return") {
    CHECK(run(R"(Mov(Data("x" 0) Reg(A) false) Mov(Reg(A) Reg(Y) false) Return(false false false))") ==
          expect(R"(Mov(Data("x" 0) Reg(Y) false) Return(false false false))"));
  }
  SUBCASE("an immediate into X") {
    CHECK(run("Mov(Imm(5) Reg(A) false) Mov(Reg(A) Reg(X) false) Return(false false false)") ==
          expect("Mov(Imm(5) Reg(X) false) Return(false false false)"));
  }
  SUBCASE("a load before the pair doesn't matter") {
    // The pair folds, and the load of 99 before it, now dead, is left
    // for dead_a_arith.
    CHECK(run("Mov(Imm(99) Reg(A) false) Mov(ZP(128 0) Reg(A) false) Mov(Reg(A) Reg(X) false) "
              "Return(false false false)") ==
          expect("Mov(Imm(99) Reg(A) false) Mov(ZP(128 0) Reg(X) false) "
                 "Return(false false false)"));
  }
  SUBCASE("a branch whose both paths leave A dead") {
    CHECK(run(R"(Mov(ZP(128 0) Reg(A) false) Mov(Reg(A) Reg(X) false) Branch(NE "L")
                 Label("L") Return(false false false))") ==
          expect(R"(Mov(ZP(128 0) Reg(X) false) Branch(NE "L") Label("L")
                    Return(false false false))"));
  }
  SUBCASE("a call after the pair writes A") {
    CHECK(run(R"(Mov(ZP(128 0) Reg(A) false) Mov(Reg(A) Reg(X) false) Call("foo" [] [])
                 Return(false false false))") ==
          expect(R"(Mov(ZP(128 0) Reg(X) false) Call("foo" [] []) Return(false false false))"));
  }
  SUBCASE("an element indexed by Y into X") {
    // `LDX abs,Y` exists.
    CHECK(run(R"(Mov(IndexedData("t" 0 Y) Reg(A) false) Mov(Reg(A) Reg(X) false)
                 Return(false false false))") ==
          expect(R"(Mov(IndexedData("t" 0 Y) Reg(X) false) Return(false false false))"));
  }
}

TEST_CASE("direct_index_load keeps the pair when it can't fuse") {
  SUBCASE("A is stored after the pair") {
    check_kept("Mov(ZP(128 0) Reg(A) false) Mov(Reg(A) Reg(X) false) Mov(Reg(A) ZP(144 0) false) "
               "Return(false false false)");
  }
  SUBCASE("a Frame operand, which LDX can't address") {
    check_kept("Mov(Frame(4) Reg(A) false) Mov(Reg(A) Reg(X) false) Return(false false false)");
  }
  SUBCASE("an Indirect operand") {
    check_kept("Mov(Indirect(0) Reg(A) false) Mov(Reg(A) Reg(X) false) Return(false false false)");
  }
  SUBCASE("a second move into A") {
    check_kept("Mov(ZP(128 0) Reg(A) false) Mov(Reg(A) Reg(A) false) Return(false false false)");
  }
  SUBCASE("a first move into X") {
    check_kept("Mov(ZP(128 0) Reg(X) false) Mov(Reg(A) Reg(X) false) Return(false false false)");
  }
  SUBCASE("A is read at the branch target") {
    check_kept(R"(Mov(ZP(128 0) Reg(A) false) Mov(Reg(A) Reg(X) false) Branch(NE "L")
                  Mov(Imm(0) Reg(A) false) Return(false false false) Label("L")
                  Mov(Reg(A) ZP(144 0) false) Return(false false false))");
  }
  SUBCASE("an element indexed by X into X") {
    // There's no `LDX abs,X`.
    check_kept(R"(Mov(IndexedData("t" 0 X) Reg(A) false) Mov(Reg(A) Reg(X) false)
                  Return(false false false))");
  }
}
