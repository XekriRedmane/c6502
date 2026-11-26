// The tests of tests/test_inc_peephole.py that call the pass on asm,
// ported. The ones that compile C or run the simulator test the Python
// pipeline and stay there.
#include <string>
#include <string_view>

#include "asm_text.h"
#include "doctest.h"
#include "passes/inc_peephole.h"

namespace {

using c6502::asm_text::expect;
using c6502::asm_text::instructions;
using c6502::asm_text::program;

std::string run(std::string_view instrs) {
  return instructions(c6502::passes::apply_inc_peephole(program(instrs)));
}

// `LDA m; CLC; ADC #1; STA m`, the first byte of an add of 1.
std::string first_byte(const std::string& m) {
  return "Mov(" + m + " Reg(A) false) ClearCarry Add(Imm(1) Reg(A)) Mov(Reg(A) " + m +
         " false) ";
}

// `LDA m; ADC #0; STA m`, a later byte.
std::string next_byte(const std::string& m) {
  return "Mov(" + m + " Reg(A) false) Add(Imm(0) Reg(A)) Mov(Reg(A) " + m + " false) ";
}

}  // namespace

TEST_CASE("inc_peephole folds an add of 1 into INC chains") {
  SUBCASE("one byte becomes a bare INC") {
    CHECK(run(first_byte("ZP(128 0)")) == expect("Inc(ZP(128 0))"));
  }
  SUBCASE("two bytes become INC; BNE; INC; label") {
    CHECK(run(first_byte("ZP(128 0)") + next_byte("ZP(129 0)")) ==
          expect(R"(Inc(ZP(128 0)) Branch(NE ".inc_done@0") Inc(ZP(129 0))
                    Label(".inc_done@0"))"));
  }
  SUBCASE("Data operands") {
    CHECK(run(first_byte(R"(Data("ctr" 0))") + next_byte(R"(Data("ctr" 1))")) ==
          expect(R"(Inc(Data("ctr" 0)) Branch(NE ".inc_done@0") Inc(Data("ctr" 1))
                    Label(".inc_done@0"))"));
  }
  SUBCASE("bytes at addresses that aren't consecutive") {
    // The byte-granular register allocation may put the two bytes of a
    // value in slots that aren't adjacent. The shape of the chain is
    // what marks them as one add of 1.
    CHECK(run(first_byte("ZP(129 0)") + next_byte("ZP(128 0)")) ==
          expect(R"(Inc(ZP(129 0)) Branch(NE ".inc_done@0") Inc(ZP(128 0))
                    Label(".inc_done@0"))"));
  }
  SUBCASE("two unrelated one-byte adds") {
    // Each has its own CLC, which ends the chain, so each becomes an
    // INC of its own.
    CHECK(run(first_byte("ZP(128 0)") + first_byte("ZP(129 0)")) ==
          expect("Inc(ZP(128 0)) Inc(ZP(129 0))"));
  }
  SUBCASE("A read afterward is loaded from the top byte") {
    CHECK(run(first_byte(R"(Data("c" 0))") + "Return(true false false)") ==
          expect(R"(Inc(Data("c" 0)) Mov(Data("c" 0) Reg(A) false)
                    Return(true false false))"));
  }
}

TEST_CASE("inc_peephole leaves other shapes alone") {
  SUBCASE("a store to another byte") {
    // INC would change $80 where the chain writes $81.
    const std::string text =
        "Mov(ZP(128 0) Reg(A) false) ClearCarry Add(Imm(1) Reg(A)) Mov(Reg(A) ZP(129 0) false)";
    CHECK(run(text) == expect(text));
  }
  SUBCASE("a Frame operand") {
    // Frame bytes are reached with (ind),Y, which INC doesn't have.
    const std::string text = first_byte("Frame(4)");
    CHECK(run(text) == expect(text));
  }
  SUBCASE("an add of 2") {
    const std::string text =
        "Mov(ZP(128 0) Reg(A) false) ClearCarry Add(Imm(2) Reg(A)) Mov(Reg(A) ZP(128 0) false)";
    CHECK(run(text) == expect(text));
  }
  SUBCASE("a subtract of 1") {
    const std::string text =
        "Mov(ZP(128 0) Reg(A) false) SetCarry Sub(Imm(1) Reg(A)) Mov(Reg(A) ZP(128 0) false)";
    CHECK(run(text) == expect(text));
  }
  SUBCASE("a later ADC that reads the carry") {
    // The high byte goes to X, where an update in place would store it,
    // and its ADC reads the carry the INC chain wouldn't produce.
    const std::string text = first_byte("ZP(128 0)") +
                             "Mov(ZP(129 0) Reg(A) false) Add(Imm(0) Reg(A)) "
                             "Mov(Reg(A) Reg(X) false) Return(false false false)";
    CHECK(run(text) == expect(text));
  }
}

TEST_CASE("inc_peephole numbers its labels past those in the program") {
  // The peephole fixed point runs the pass many times. A count started
  // at 0 each run gave one function two `.inc_done@0` labels (fuzz.stmts
  // seed 987).
  const std::string out = run(R"(Label(".inc_done@0") )" + first_byte("ZP(128 0)") +
                              next_byte("ZP(129 0)") + "Return(false false false)");
  CHECK(out == expect(R"(Label(".inc_done@0") Inc(ZP(128 0)) Branch(NE ".inc_done@1")
                         Inc(ZP(129 0)) Label(".inc_done@1") Return(false false false))"));
}

TEST_CASE("inc_peephole counts only the labels that end in a number") {
  const std::string chain = first_byte("ZP(128 0)") + next_byte("ZP(129 0)");
  const std::string labels = R"(Label(".inc_done@7") Label(".inc_done@") Label(".inc_done@9x") )"
                             R"(Label(".inc_done@12a") Label(".other@40") )";
  const std::string out = run(labels + chain + "Return(false false false)");
  CHECK(out == expect(labels + R"(Inc(ZP(128 0)) Branch(NE ".inc_done@8")
                         Inc(ZP(129 0)) Label(".inc_done@8") Return(false false false))"));
}
