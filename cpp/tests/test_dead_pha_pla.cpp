// The tests of tests/test_dead_pha_pla.py, ported.
#include <string>

#include "asm_text.h"
#include "doctest.h"
#include "passes/dead_pha_pla.h"

namespace {

using c6502::asm_text::expect;
using c6502::asm_text::instructions;
using c6502::asm_text::program;

std::string run(std::string_view instrs) {
  return instructions(c6502::passes::apply_dead_pha_pla(program(instrs)));
}

void check_kept(std::string_view instrs) { CHECK(run(instrs) == expect(instrs)); }

// Returns `LDA #1; PHA; <body>; PLA; RTS`.
std::string around(std::string_view body) {
  return "Mov(Imm(1) Reg(A) false) Push(Reg(A)) " + std::string(body) +
         " Pop(Reg(A)) Return(false false false)";
}

}  // namespace

TEST_CASE("dead_pha_pla drops a pair whose body leaves A alone") {
  SUBCASE("around an LDY of a Data byte") {
    // The shape `direct_index_load` leaves of a store indexed through a
    // pointer.
    CHECK(run(R"(Mov(Imm(1) Reg(A) false) Push(Reg(A)) Mov(Data("zpabi_p0" 0) Reg(Y) false)
                 Pop(Reg(A)) Mov(Reg(A) IndirectY false) Return(false false false))") ==
          expect(R"(Mov(Imm(1) Reg(A) false) Mov(Data("zpabi_p0" 0) Reg(Y) false)
                    Mov(Reg(A) IndirectY false) Return(false false false))"));
  }
  SUBCASE("around an LDX of a ZP byte") {
    CHECK(run("Mov(Imm(66) Reg(A) false) Push(Reg(A)) Mov(ZP(128 0) Reg(X) false) Pop(Reg(A)) "
              "Mov(Reg(A) IndirectY false) Return(false false false)") ==
          expect("Mov(Imm(66) Reg(A) false) Mov(ZP(128 0) Reg(X) false) "
                 "Mov(Reg(A) IndirectY false) Return(false false false)"));
  }
  SUBCASE("two pairs in one function") {
    CHECK(run(R"(Mov(Imm(1) Reg(A) false) Push(Reg(A)) Mov(Data("a" 0) Reg(Y) false) Pop(Reg(A))
                 Mov(Reg(A) IndirectY false) Mov(Imm(2) Reg(A) false) Push(Reg(A))
                 Mov(Data("b" 0) Reg(Y) false) Pop(Reg(A)) Mov(Reg(A) IndirectY false)
                 Return(false false false))") ==
          expect(R"(Mov(Imm(1) Reg(A) false) Mov(Data("a" 0) Reg(Y) false)
                    Mov(Reg(A) IndirectY false) Mov(Imm(2) Reg(A) false)
                    Mov(Data("b" 0) Reg(Y) false) Mov(Reg(A) IndirectY false)
                    Return(false false false))"));
  }
  SUBCASE("nested pairs, the inner one first") {
    // The outer pair stops at the inner Push, and the scan goes on to
    // match the inner pair. The next sweep of the fixed point collapses
    // the outer pair, whose body is then empty.
    const std::string once = run(around("Push(Reg(A)) Pop(Reg(A))"));
    CHECK(once == expect(around("")));
    CHECK(run(around("")) == expect("Mov(Imm(1) Reg(A) false) Return(false false false)"));
  }
}

TEST_CASE("dead_pha_pla keeps a pair it can't drop") {
  SUBCASE("the body reads A") {
    // Dropping the pair would be sound, but the rule stays strict, so
    // that relaxing it is a deliberate change.
    check_kept(around("Mov(Reg(A) ZP(144 0) false)"));
  }
  SUBCASE("the body writes A") {
    check_kept(around("Mov(Imm(153) Reg(A) false)"));
  }
  SUBCASE("the body calls") {
    check_kept(around(R"(Call("some_helper" [] []))"));
  }
  SUBCASE("the body branches") {
    check_kept(around(R"(Branch(EQ ".elsewhere"))"));
  }
  SUBCASE("the body has a label") {
    check_kept(around(R"(Label(".midbody"))"));
  }
  SUBCASE("a branch reads the flags of the PLA") {
    check_kept(R"(Mov(Imm(1) Reg(A) false) Push(Reg(A)) Mov(Data("x" 0) Reg(Y) false) Pop(Reg(A))
                  Branch(EQ ".somewhere") Return(false false false))");
  }
}

TEST_CASE("dead_pha_pla reloads M in place of the pair") {
  // `LDA M; PHA; TXA; TAY; PLA` as `TXA; TAY; LDA M`, the store of M
  // through `(p),Y` at an index in X (Zork's `read_line`).
  const auto shape = [](std::string_view load, std::string_view body) {
    return std::string(load) + " Push(Reg(A)) " + std::string(body) +
           " Pop(Reg(A)) Mov(Reg(A) IndirectZpY(144) false) Return(false false false)";
  };
  const char* load = R"(Mov(Data("__local_f__c" 0) Reg(A) false))";
  const char* body = "Mov(Reg(X) Reg(A) false) Mov(Reg(A) Reg(Y) false)";
  SUBCASE("the reload") {
    CHECK(run(shape(load, body)) ==
          expect(std::string(body) + " " + load +
                 " Mov(Reg(A) IndirectZpY(144) false) Return(false false false)"));
  }
  SUBCASE("a volatile load") {
    check_kept(shape(R"(Mov(Data("__local_f__c" 0) Reg(A) true))", body));
  }
  SUBCASE("a load through a pointer") {
    check_kept(shape("Mov(IndirectZpY(146) Reg(A) false)", body));
  }
  SUBCASE("the body reads the pushed A") {
    check_kept(shape(load, "Mov(Reg(A) Reg(Y) false) Mov(Reg(X) Reg(A) false)"));
  }
  SUBCASE("the body writes memory") {
    check_kept(shape(load, R"(Mov(Reg(X) Reg(A) false) Mov(Reg(A) Data("__local_f__c" 0) false))"));
  }
}
