// The tests of tests/test_carry_inc.py that call the pass on asm,
// ported. The simulator's runs are in test_carry_inc_sim.cpp.
#include <string>
#include <string_view>

#include "asm_text.h"
#include "doctest.h"
#include "passes/carry_inc.h"

namespace {

using c6502::asm_text::expect;
using c6502::asm_text::instructions;
using c6502::asm_text::program;

std::string run(std::string_view instrs) {
  return instructions(c6502::passes::apply_carry_inc_peephole(program(instrs)));
}

// `LDA src; ADC #0; STA dst`, or the same with SBC.
std::string carry_byte(const std::string& src, const std::string& dst, bool sub = false) {
  return "Mov(" + src + " Reg(A) false) " + (sub ? "Sub" : "Add") +
         "(Imm(0) Reg(A)) Mov(Reg(A) " + dst + " false) ";
}

// `LDA m; CLC; ADC #5; STA m`, the low byte that sets C.
std::string low_byte(const std::string& m) {
  return "Mov(" + m + " Reg(A) false) ClearCarry Add(Imm(5) Reg(A)) Mov(Reg(A) " + m +
         " false) ";
}

const std::string kRet = "Return(false false false)";

}  // namespace

TEST_CASE("carry_inc rewrites carry bytes in place") {
  SUBCASE("an add becomes BCC; INC") {
    CHECK(run(low_byte("ZP(128 0)") + carry_byte("ZP(129 0)", "ZP(129 0)") + kRet) ==
          expect(low_byte("ZP(128 0)") +
                 R"(Branch(CC ".carry_inc@0") Inc(ZP(129 0)) Label(".carry_inc@0") )" + kRet));
  }
  SUBCASE("a subtract becomes BCS; DEC") {
    CHECK(run(carry_byte(R"(Data("g" 1))", R"(Data("g" 1))", true) + kRet) ==
          expect(R"(Branch(CS ".carry_inc@0") Dec(Data("g" 1)) Label(".carry_inc@0") )" + kRet));
  }
  SUBCASE("a run of adds becomes an INC chain") {
    CHECK(run(carry_byte("ZP(129 0)", "ZP(129 0)") + carry_byte("ZP(130 0)", "ZP(130 0)") +
              carry_byte("ZP(131 0)", "ZP(131 0)") + kRet) ==
          expect(R"(Branch(CC ".carry_inc@0") Inc(ZP(129 0)) Branch(NE ".carry_inc@0")
                    Inc(ZP(130 0)) Branch(NE ".carry_inc@0") Inc(ZP(131 0))
                    Label(".carry_inc@0") )" +
                 kRet));
  }
  SUBCASE("a run of subtracts has only its top byte rewritten") {
    const std::string b1 = carry_byte("ZP(129 0)", "ZP(129 0)", true);
    CHECK(run(b1 + carry_byte("ZP(130 0)", "ZP(130 0)", true) + kRet) ==
          expect(b1 + R"(Branch(CS ".carry_inc@0") Dec(ZP(130 0)) Label(".carry_inc@0") )" +
                 kRet));
  }
}

TEST_CASE("carry_inc keeps C for a register whose carry is read") {
  // do_rwts_on_sector's high byte in X, with the loop's BCC after it.
  const std::string tail =
      R"(Branch(CC ".done") Return(false false false) Label(".done") Return(false false false))";
  CHECK(run(carry_byte("Reg(X)", "Reg(X)", true) + tail) ==
        expect(R"(Branch(CS ".carry_inc@0") Compare(Reg(X) Imm(1)) Dec(Reg(X))
                  Label(".carry_inc@0") )" +
               tail));
  CHECK(run(carry_byte("Reg(X)", "Reg(X)") + tail) ==
        expect(R"(Branch(CC ".carry_inc@0") Inc(Reg(X)) Branch(EQ ".carry_inc@0") ClearCarry
                  Label(".carry_inc@0") )" +
               tail));
}

TEST_CASE("carry_inc leaves other shapes alone") {
  SUBCASE("another destination") {
    const std::string text = carry_byte("ZP(129 0)", "ZP(130 0)") + kRet;
    CHECK(run(text) == expect(text));
  }
  SUBCASE("A read afterward") {
    // A holds the new high byte, and the STA after reads it.
    const std::string text =
        carry_byte("ZP(129 0)", "ZP(129 0)") + "Mov(Reg(A) ZP(144 0) false) " + kRet;
    CHECK(run(text) == expect(text));
  }
  SUBCASE("the carry read afterward") {
    // A third byte continues the chain into another destination and
    // reads the carry out of this one, which the INC form doesn't give.
    const std::string text =
        carry_byte("ZP(129 0)", "ZP(129 0)") + carry_byte("ZP(130 0)", "ZP(131 0)") + kRet;
    CHECK(run(text) == expect(text));
  }
  SUBCASE("a Frame operand") {
    // INC has no (zp),Y mode.
    const std::string text = carry_byte("Frame(3)", "Frame(3)") + kRet;
    CHECK(run(text) == expect(text));
  }
  SUBCASE("a volatile store") {
    const std::string text =
        "Mov(ZP(129 0) Reg(A) false) Add(Imm(0) Reg(A)) Mov(Reg(A) ZP(129 0) true) " + kRet;
    CHECK(run(text) == expect(text));
  }
}

TEST_CASE("carry_inc numbers its labels across the program") {
  SUBCASE("past an existing label") {
    CHECK(run(R"(Label(".carry_inc@4") )" + carry_byte("ZP(129 0)", "ZP(129 0)") + kRet) ==
          expect(R"(Label(".carry_inc@4") Branch(CC ".carry_inc@5") Inc(ZP(129 0))
                    Label(".carry_inc@5") )" +
                 kRet));
  }
  SUBCASE("distinct in two functions") {
    // The simulator's assembler keeps one label table for the program.
    const std::string body = carry_byte("ZP(129 0)", "ZP(129 0)") + kRet;
    const auto prog = c6502::passes::apply_carry_inc_peephole(
        program(body, "g", "Function(\"f\" false [] [" + body + "]) "));
    const auto& f = std::get<c6502::asm_ast::Function>(prog.top_level[0]);
    const auto& g = std::get<c6502::asm_ast::Function>(prog.top_level[1]);
    CHECK(c6502::asm_text::dump(f.instructions) ==
          expect(R"(Branch(CC ".carry_inc@0") Inc(ZP(129 0)) Label(".carry_inc@0") )" + kRet));
    CHECK(c6502::asm_text::dump(g.instructions) ==
          expect(R"(Branch(CC ".carry_inc@1") Inc(ZP(129 0)) Label(".carry_inc@1") )" + kRet));
  }
}
