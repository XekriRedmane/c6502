// The tests of tests/test_round_trip_load.py, ported.
#include <string>

#include "asm_text.h"
#include "doctest.h"
#include "passes/round_trip_load.h"

namespace {

using c6502::asm_text::expect;
using c6502::asm_text::instructions;
using c6502::asm_text::program;

std::string run(const char* instrs) {
  return instructions(c6502::passes::apply_round_trip_load_drop(program(instrs)));
}

}  // namespace

TEST_CASE("round_trip_load drops an LDA after an A writer and an STA") {
  CHECK(run("SetCarry Sub(Imm(4) Reg(A)) Mov(Reg(A) ZP(133 0) false) "
            "Mov(ZP(133 0) Reg(A) false)") ==
        expect("SetCarry Sub(Imm(4) Reg(A)) Mov(Reg(A) ZP(133 0) false)"));
}

TEST_CASE("round_trip_load rewrites a move from memory to memory") {
  SUBCASE("after SBC and STA, the shape of do_ascend") {
    // SBC #$04, STA col, and then Mov(col, player_col), which is emitted
    // as LDA col; STA player_col. The rewrite stores A in player_col.
    CHECK(run(R"(SetCarry Sub(Imm(4) Reg(A)) Mov(Reg(A) Data("__local_col" 0) false)
                 Mov(Data("__local_col" 0) Data("player_col" 0) false))") ==
          expect(R"(SetCarry Sub(Imm(4) Reg(A)) Mov(Reg(A) Data("__local_col" 0) false)
                    Mov(Reg(A) Data("player_col" 0) false))"));
  }
  SUBCASE("from a ZP byte") {
    CHECK(run(R"(Mov(Imm(7) Reg(A) false) Mov(Reg(A) ZP(133 0) false)
                 Mov(ZP(133 0) Data("player_col" 0) false))") ==
          expect(R"(Mov(Imm(7) Reg(A) false) Mov(Reg(A) ZP(133 0) false)
                    Mov(Reg(A) Data("player_col" 0) false))"));
  }
  SUBCASE("but not a volatile one, whose read has to happen") {
    const char* text = R"(Mov(Imm(1) Reg(A) false) Mov(Reg(A) Data("m" 0) false)
                          Mov(Data("m" 0) Data("dst" 0) true))";
    CHECK(run(text) == expect(text));
  }
  SUBCASE("but not one from another byte") {
    const char* text = R"(Mov(Imm(1) Reg(A) false) Mov(Reg(A) Data("m1" 0) false)
                          Mov(Data("m2" 0) Data("dst" 0) false))";
    CHECK(run(text) == expect(text));
  }
  SUBCASE("but not after an instruction that doesn't set N/Z from A") {
    const char* text = R"(SetCarry Mov(Reg(A) Data("m" 0) false)
                          Mov(Data("m" 0) Data("dst" 0) false))";
    CHECK(run(text) == expect(text));
  }
}

TEST_CASE("round_trip_load sets the flags from A with ORA #$00") {
  SUBCASE("when a branch reads them") {
    CHECK(run(R"(Call("g" [] []) Mov(Reg(A) Data("t" 0) false)
                 Mov(Data("t" 0) Reg(A) false) Branch(NE ".l") Label(".l")
                 Return(true false false))") ==
          expect(R"(Call("g" [] []) Mov(Reg(A) Data("t" 0) false) Or(Imm(0) Reg(A))
                    Branch(NE ".l") Label(".l") Return(true false false))"));
  }
  SUBCASE("and drops the load when nothing reads them") {
    CHECK(run(R"(Call("g" [] []) Mov(Reg(A) Data("t" 0) false)
                 Mov(Data("t" 0) Reg(A) false) ArithmeticShiftLeft(Reg(A))
                 Return(true false false))") ==
          expect(R"(Call("g" [] []) Mov(Reg(A) Data("t" 0) false)
                    ArithmeticShiftLeft(Reg(A)) Return(true false false))"));
  }
}
