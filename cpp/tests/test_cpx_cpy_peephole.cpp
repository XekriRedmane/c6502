// The tests of tests/test_cpx_cpy_peephole.py, ported.
#include "asm_text.h"
#include "doctest.h"
#include "passes/asm_context.h"
#include "passes/cpx_cpy_peephole.h"

namespace {

using c6502::asm_text::expect;
using c6502::asm_text::instructions;
using c6502::asm_text::program;

std::string run(std::string_view instrs) {
  return instructions(
      c6502::passes::apply_cpx_cpy_peephole(program(instrs), c6502::passes::AsmContext{}));
}

void check_kept(std::string_view instrs) { CHECK(run(instrs) == expect(instrs)); }

}  // namespace

TEST_CASE("cpx_cpy rewrites TXA; CMP R as CPX R") {
  SUBCASE("an immediate") {
    CHECK(run("Mov(Reg(X) Reg(A) false) Compare(Reg(A) Imm(40)) Return(false false false)") ==
          expect("Compare(Reg(X) Imm(40)) Return(false false false)"));
  }
  SUBCASE("TYA as CPY") {
    CHECK(run("Mov(Reg(Y) Reg(A) false) Compare(Reg(A) Imm(16)) Return(false false false)") ==
          expect("Compare(Reg(Y) Imm(16)) Return(false false false)"));
  }
  SUBCASE("a ZP byte") {
    CHECK(run("Mov(Reg(X) Reg(A) false) Compare(Reg(A) ZP(131 0)) Return(false false false)") ==
          expect("Compare(Reg(X) ZP(131 0)) Return(false false false)"));
  }
  SUBCASE("a Data byte") {
    CHECK(run(R"(Mov(Reg(X) Reg(A) false) Compare(Reg(A) Data("g" 0)) Return(false false false))") ==
          expect(R"(Compare(Reg(X) Data("g" 0)) Return(false false false))"));
  }
}

TEST_CASE("cpx_cpy keeps a right operand that CPX can't address") {
  // CPX and CPY have no indirect-Y mode.
  check_kept("Mov(Reg(X) Reg(A) false) Compare(Reg(A) Frame(3)) Return(false false false)");
  check_kept("Mov(Reg(X) Reg(A) false) Compare(Reg(A) Stack(0)) Return(false false false)");
  check_kept("Mov(Reg(X) Reg(A) false) Compare(Reg(A) Indirect(0)) Return(false false false)");
}

TEST_CASE("cpx_cpy follows the liveness of A") {
  SUBCASE("a store reads A") {
    check_kept("Mov(Reg(X) Reg(A) false) Compare(Reg(A) Imm(0)) Mov(Reg(A) ZP(132 0) false) "
               "Return(false false false)");
  }
  SUBCASE("a load writes A") {
    CHECK(run("Mov(Reg(X) Reg(A) false) Compare(Reg(A) Imm(0)) Mov(Imm(42) Reg(A) false) "
              "Return(false false false)") ==
          expect("Compare(Reg(X) Imm(0)) Mov(Imm(42) Reg(A) false) Return(false false false)"));
  }
  SUBCASE("a return that saves A") {
    check_kept("Mov(Reg(X) Reg(A) false) Compare(Reg(A) Imm(0)) Return(true false false)");
  }
  SUBCASE("a branch whose both paths leave A dead") {
    CHECK(run(R"(Mov(Reg(X) Reg(A) false) Compare(Reg(A) ZP(131 0)) Branch(NE "L")
                 Return(false false false) Label("L") Mov(Imm(0) Reg(A) false)
                 Return(false false false))") ==
          expect(R"(Compare(Reg(X) ZP(131 0)) Branch(NE "L") Return(false false false)
                    Label("L") Mov(Imm(0) Reg(A) false) Return(false false false))"));
  }
  SUBCASE("A is read at the branch target") {
    check_kept(R"(Mov(Reg(X) Reg(A) false) Compare(Reg(A) Imm(0)) Branch(NE "L")
                  Return(false false false) Label("L") Mov(Reg(A) ZP(132 0) false)
                  Return(false false false))");
  }
}

TEST_CASE("cpx_cpy rewrites LDA M; STA t; CPX t as CPX M") {
  const char* head = R"(Mov(Data("WNDWDTH" 0) Reg(A) true)
                        Mov(Reg(A) Data("__local_f__0" 0) false)
                        Compare(Reg(X) Data("__local_f__0" 0)) Branch(CS ".l") )";
  const char* tail = R"(Label(".l") Return(false false false))";
  SUBCASE("a volatile M is still read once, by the compare") {
    CHECK(run(std::string(head) + tail) ==
          expect(std::string(R"(Compare(Reg(X) Data("WNDWDTH" 0)) Branch(CS ".l") )") + tail));
  }
  SUBCASE("A is read after the compare") {
    check_kept(std::string(head) + R"(Mov(Reg(A) Data("q" 0) false) )" + tail);
  }
}
