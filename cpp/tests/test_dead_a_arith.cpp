// The tests of tests/test_dead_a_arith.py, ported.
#include "asm_text.h"
#include "doctest.h"
#include "passes/dead_a_arith.h"

namespace {

using c6502::asm_text::expect;
using c6502::asm_text::instructions;
using c6502::asm_text::program;

std::string run(std::string_view instrs) {
  return instructions(c6502::passes::apply_dead_a_arith_elimination(program(instrs)));
}

// Checks that the pass leaves `instrs` as they are.
void check_kept(std::string_view instrs) { CHECK(run(instrs) == expect(instrs)); }

constexpr const char* kJumpTail = R"(Jump("L" []) Label("L") Return(false false false))";

std::string with_tail(std::string_view head) { return std::string(head) + " " + kJumpTail; }

}  // namespace

TEST_CASE("dead_a_arith drops a load or ADC whose A and flags are dead") {
  SUBCASE("LDA #0 before a JMP") {
    CHECK(run(with_tail("Mov(Imm(0) Reg(A) false)")) == expect(kJumpTail));
  }
  SUBCASE("ADC #0 before a JMP") {
    CHECK(run(with_tail("Add(Imm(0) Reg(A))")) == expect(kJumpTail));
  }
  SUBCASE("TXA before a JMP") {
    CHECK(run(with_tail("Mov(Reg(X) Reg(A) false)")) == expect(kJumpTail));
  }
  SUBCASE("a load from ZP") {
    CHECK(run(with_tail("Mov(ZP(128 0) Reg(A) false)")) == expect(kJumpTail));
  }
  SUBCASE("a load from Data") {
    CHECK(run(with_tail(R"(Mov(Data("g" 0) Reg(A) false))")) == expect(kJumpTail));
  }
}

TEST_CASE("dead_a_arith drops LDA #0; ADC #0 in one call") {
  // The first round drops the ADC, whose A is dead after the JMP, and
  // the second the LDA, whose only reader was the ADC. A later pass of
  // the fixed point (such as volatile_void_read_cmp, which rewrites a
  // load through a pointer as a CMP) could otherwise lock in an LDA that
  // could still be dropped, by extending A's liveness across it.
  CHECK(run(with_tail("Mov(Imm(0) Reg(A) false) Add(Imm(0) Reg(A))")) ==
        expect(kJumpTail));
}

TEST_CASE("dead_a_arith keeps operands whose emission changes Y") {
  // `LDA (FP),Y` is written as `LDY #3; LDA (FP),Y`, and dropping it
  // would lose the write of Y.
  check_kept(with_tail("Mov(Frame(3) Reg(A) false)"));
  check_kept(with_tail("Mov(Indirect(0) Reg(A) false)"));
}

TEST_CASE("dead_a_arith follows the liveness of A and the flags") {
  SUBCASE("a store reads A") {
    check_kept("Mov(Imm(0) Reg(A) false) Mov(Reg(A) ZP(132 0) false) Return(false false false)");
  }
  SUBCASE("a branch reads the flags") {
    check_kept(R"(Mov(Imm(0) Reg(A) false) Branch(NE "L") Label("L") Return(false false false))");
  }
  SUBCASE("an ADC before a load and a BEQ") {
    // The `BEQ` reads only the Z the `LDA` sets, so C and V from the
    // `ADC` are dead too. `instr_insert_obj` in Zork's objects.c had
    // kept the dead high byte of `d + 6` this way.
    const char* tail =
        R"(Mov(Data("g" 0) Reg(A) false) Branch(EQ "L") Mov(Reg(A) Data("g" 0) false)
           Label("L") Return(false false false))";
    CHECK(run(std::string("Mov(Reg(X) Reg(A) false) Add(Imm(0) Reg(A)) ") + tail) ==
          expect(tail));
  }
  SUBCASE("an ADC whose C a BCS reads") {
    // The `LDA g` goes, since the `BCS` reads only C.
    CHECK(run(R"(Add(Imm(0) Reg(A)) Mov(Data("g" 0) Reg(A) false) Branch(CS "L")
                 Label("L") Return(false false false))") ==
          expect(R"(Add(Imm(0) Reg(A)) Branch(CS "L") Label("L") Return(false false false))"));
  }
  SUBCASE("a return that saves A") {
    check_kept("Mov(Imm(0) Reg(A) false) Return(true false false)");
  }
  SUBCASE("a second load writes A") {
    CHECK(run("Mov(Imm(0) Reg(A) false) Mov(Imm(42) Reg(A) false) "
              "Mov(Reg(A) ZP(144 0) false) Return(false false false)") ==
          expect("Mov(Imm(42) Reg(A) false) Mov(Reg(A) ZP(144 0) false) "
                 "Return(false false false)"));
  }
  SUBCASE("a call that takes A") {
    // The load that sets up an argument in A has to stay.
    check_kept(R"(Mov(Imm(16) Reg(A) false) Call("snd_delay_down" ["A" "X"] [])
                  Return(false false false))");
  }
  SUBCASE("a call that doesn't take A") {
    // A callee may return in A, so an LDA before the call is dead.
    CHECK(run(R"(Mov(Imm(16) Reg(A) false) Call("some_void_fn" [] [])
                 Return(false false false))") ==
          expect(R"(Call("some_void_fn" [] []) Return(false false false))"));
  }
  SUBCASE("a tail call that takes A") {
    check_kept(R"(Mov(Imm(16) Reg(A) false) Jump("snd_delay_down" ["A" "X"]))");
  }
}

TEST_CASE("dead_a_arith drops a TXA or TYA whose flags are already set") {
  // The last instruction before the transfer to set the flags wrote the
  // register the transfer reads, so the branch reads the same flags
  // without it. That is beam_target_tick's
  // `LDX beam_snd_ctr; TXA; BMI .if_end@0`.
  const char* tail = R"(Mov(Data("other" 0) Reg(A) false) Mov(Reg(A) Data("dst" 0) false)
                        Label(".L") Return(false false false))";
  SUBCASE("TXA after LDX") {
    CHECK(run(std::string(R"(Mov(Data("m" 0) Reg(X) false) Mov(Reg(X) Reg(A) false)
                             Branch(MI ".L") )") + tail) ==
          expect(std::string(R"(Mov(Data("m" 0) Reg(X) false) Branch(MI ".L") )") + tail));
  }
  SUBCASE("TYA after LDY") {
    CHECK(run(std::string(R"(Mov(Data("m" 0) Reg(Y) false) Mov(Reg(Y) Reg(A) false)
                             Branch(EQ ".L") )") + tail) ==
          expect(std::string(R"(Mov(Data("m" 0) Reg(Y) false) Branch(EQ ".L") )") + tail));
  }
  SUBCASE("TXA after LDX and a store of A") {
    // `STA other` sets no flags, so the LDX is still the last to set
    // them.
    CHECK(run(std::string(R"(Mov(Data("m" 0) Reg(X) false) Mov(Reg(A) Data("o" 0) false)
                             Mov(Reg(X) Reg(A) false) Branch(MI ".L") )") + tail) ==
          expect(std::string(R"(Mov(Data("m" 0) Reg(X) false) Mov(Reg(A) Data("o" 0) false)
                                Branch(MI ".L") )") + tail));
  }
  SUBCASE("TXA after LDX and INX") {
    // INX sets N and Z from X's new value, which X holds at the TXA.
    CHECK(run(std::string(R"(Mov(Data("m" 0) Reg(X) false) Inc(Reg(X))
                             Mov(Reg(X) Reg(A) false) Branch(MI ".L") )") + tail) ==
          expect(std::string(R"(Mov(Data("m" 0) Reg(X) false) Inc(Reg(X))
                                Branch(MI ".L") )") + tail));
  }
}

TEST_CASE("dead_a_arith keeps a TXA whose flags differ from the ones before it") {
  SUBCASE("a store to the frame sets Z from its LDY") {
    // `LDX M; LDY #3; STA (FP),Y; TXA; BEQ .L`. Without the TXA the BEQ
    // would test the 3 and never be taken.
    check_kept(R"(Mov(Data("m" 0) Reg(X) false) Mov(Reg(A) Frame(3) false)
                  Mov(Reg(X) Reg(A) false) Branch(EQ ".L") Mov(Imm(1) Reg(A) false)
                  Mov(Reg(A) Data("d" 0) false) Label(".L") Return(false false false))");
  }
  SUBCASE("an ADC sets the flags from A") {
    // The store of the ADC's result keeps the ADC, whose flags are the
    // ones the TXA replaces.
    check_kept(R"(Mov(Data("m" 0) Reg(X) false) Mov(Imm(0) Reg(A) false) Add(Imm(0) Reg(A))
                  Mov(Reg(A) Data("sum" 0) false) Mov(Reg(X) Reg(A) false) Branch(MI ".L")
                  Mov(Data("kill" 0) Reg(A) false) Mov(Reg(A) Data("dst" 0) false)
                  Label(".L") Return(false false false))");
  }
  SUBCASE("A is read on the fall-through") {
    check_kept(R"(Mov(Data("m" 0) Reg(X) false) Mov(Reg(X) Reg(A) false) Branch(MI ".L")
                  And(Imm(15) Reg(A)) Mov(Reg(A) Data("out" 0) false) Label(".L")
                  Return(false false false))");
  }
  SUBCASE("a label between the LDX and the TXA") {
    // Another predecessor may reach the TXA without running the LDX.
    check_kept(R"(Mov(Data("m" 0) Reg(X) false) Label(".L") Mov(Reg(X) Reg(A) false)
                  Branch(MI ".L2") Mov(Data("kill" 0) Reg(A) false)
                  Mov(Reg(A) Data("dst" 0) false) Label(".L2") Return(false false false))");
  }
  SUBCASE("a CMP sets the flags from A") {
    check_kept(R"(Mov(Data("m" 0) Reg(X) false) Mov(Imm(0) Reg(A) false)
                  Compare(Reg(A) Imm(5)) Mov(Reg(X) Reg(A) false) Branch(MI ".L")
                  Mov(Data("kill" 0) Reg(A) false) Mov(Reg(A) Data("dst" 0) false)
                  Label(".L") Return(false false false))");
  }
}

TEST_CASE("dead_a_arith drops a load into X or Y that nothing reads") {
  CHECK(run(R"(Mov(Imm(0) Reg(Y) false) Mov(Data("g" 0) Reg(A) false) Return(true false false))") ==
        expect(R"(Mov(Data("g" 0) Reg(A) false) Return(true false false))"));
  check_kept(R"(Mov(Imm(0) Reg(Y) false) Mov(Reg(Y) Data("g" 0) false) Return(false false false))");
}

TEST_CASE("dead_a_arith drops a load through a zero-page pointer that nothing reads") {
  SUBCASE("the low byte of a read of the high byte") {
    // `X1 = (uint8_t)((uint16_t)Q->name >> 8);`
    CHECK(run(R"(Mov(IndirectZp(250 0) Reg(A) false) Mov(IndirectZp(250 1) Reg(A) false)
                 Mov(Reg(A) Data("X1" 0) false) Return(false false false))") ==
          expect(R"(Mov(IndirectZp(250 1) Reg(A) false) Mov(Reg(A) Data("X1" 0) false)
                    Return(false false false))"));
  }
  SUBCASE("Y is read") {
    // The `LDY #0` of the load is the index of the store.
    check_kept(R"(Mov(IndirectZp(250 0) Reg(A) false) Mov(Reg(X) Reg(A) false)
                  Mov(Reg(A) IndirectZpY(250) false) Return(false false false))");
  }
  SUBCASE("a volatile load") {
    check_kept(R"(Mov(IndirectZp(250 0) Reg(A) true) Mov(IndirectZp(250 1) Reg(A) false)
                  Mov(Reg(A) Data("X1" 0) false) Return(false false false))");
  }
}
