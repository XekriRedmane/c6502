// The tests of tests/test_shift_in_a.py, ported.
#include <string>

#include "asm_text.h"
#include "doctest.h"
#include "passes/shift_in_a.h"

namespace {

using c6502::asm_text::expect;
using c6502::asm_text::instructions;
using c6502::asm_text::program;

std::string run(std::string_view instrs) {
  return instructions(c6502::passes::apply_shift_in_a(program(instrs)));
}

void check_kept(std::string_view instrs) { CHECK(run(instrs) == expect(instrs)); }

// M is ZP $80 and N is ZP $81.
constexpr const char* kStaM = "Mov(Reg(A) ZP(128 0) false)";
constexpr const char* kLdaM = "Mov(ZP(128 0) Reg(A) false)";
constexpr const char* kRts = "Return(false false false)";
constexpr const char* kRtsA = "Return(true false false)";

std::string seq(std::initializer_list<std::string_view> parts) {
  std::string out;
  for (auto p : parts) {
    out += p;
    out += ' ';
  }
  return out;
}

}  // namespace

TEST_CASE("shift_in_a moves the shifts of a stored byte onto A") {
  SUBCASE("store, shift, reload") {
    CHECK(run(seq({kStaM, "ArithmeticShiftLeft(ZP(128 0))", kLdaM, kRtsA})) ==
          expect(seq({"ArithmeticShiftLeft(Reg(A))", kStaM, kRtsA})));
  }
  SUBCASE("several shifts and rotates") {
    CHECK(run(seq({kStaM, "LogicalShiftRight(ZP(128 0))", "RotateRight(ZP(128 0))",
                   "RotateLeft(ZP(128 0))", kLdaM, kRtsA})) ==
          expect(seq({"LogicalShiftRight(Reg(A))", "RotateRight(Reg(A))", "RotateLeft(Reg(A))",
                      kStaM, kRtsA})));
  }
  SUBCASE("no reload, with A dead") {
    CHECK(run(seq({kStaM, "ArithmeticShiftLeft(ZP(128 0))", "Mov(Imm(0) Reg(A) false)",
                   "Mov(ZP(128 0) ZP(129 0) false)", kRts})) ==
          expect(seq({"ArithmeticShiftLeft(Reg(A))", kStaM, "Mov(Imm(0) Reg(A) false)",
                      "Mov(ZP(128 0) ZP(129 0) false)", kRts})));
  }
  SUBCASE("a volatile reload is kept") {
    // The volatile LDA has to read M again, after the new store.
    CHECK(run(seq({kStaM, "ArithmeticShiftLeft(ZP(128 0))", "Mov(ZP(128 0) Reg(A) true)",
                   kRtsA})) ==
          expect(seq({"ArithmeticShiftLeft(Reg(A))", kStaM, "Mov(ZP(128 0) Reg(A) true)",
                      kRtsA})));
  }
  SUBCASE("a widened byte shifted") {
    // `(unsigned int)c << 3`: the store of the zero high byte moves up in
    // front of the load of c, and the high byte's ROLs stay between the
    // shifts, now of A.
    CHECK(run(seq({"Mov(ZP(144 0) Reg(A) false)", "ArithmeticShiftLeft(Reg(A))", kStaM,
                   "Mov(Imm(0) ZP(129 0) false)", "RotateLeft(ZP(129 0))",
                   "ArithmeticShiftLeft(ZP(128 0))", "RotateLeft(ZP(129 0))", kRts})) ==
          expect(seq({"Mov(Imm(0) ZP(129 0) false)", "Mov(ZP(144 0) Reg(A) false)",
                      "ArithmeticShiftLeft(Reg(A))", "RotateLeft(ZP(129 0))",
                      "ArithmeticShiftLeft(Reg(A))", kStaM, "RotateLeft(ZP(129 0))", kRts})));
  }
  SUBCASE("INC and DEC as an add or subtract of 1") {
    CHECK(run(seq({kStaM, "Inc(ZP(128 0))", kLdaM, kRtsA})) ==
          expect(seq({"ClearCarry", "Add(Imm(1) Reg(A))", kStaM, kRtsA})));
    CHECK(run(seq({kStaM, "Dec(ZP(128 0))", kLdaM, kRtsA})) ==
          expect(seq({"SetCarry", "Sub(Imm(1) Reg(A))", kStaM, kRtsA})));
  }
}

TEST_CASE("shift_in_a leaves a stored byte's shifts alone when it has to") {
  SUBCASE("TAX reads the unshifted value") {
    check_kept(seq({kStaM, "ArithmeticShiftLeft(ZP(128 0))", "Mov(Reg(A) Reg(X) false)", kRts}));
  }
  SUBCASE("the code that computed A reads the byte of a constant store") {
    // The store to N can't move above the load of N.
    check_kept(seq({"Mov(ZP(129 0) Reg(A) false)", "ArithmeticShiftLeft(Reg(A))", kStaM,
                    "Mov(Imm(0) ZP(129 0) false)", "RotateLeft(ZP(129 0))",
                    "ArithmeticShiftLeft(ZP(128 0))", kRts}));
  }
  SUBCASE("a volatile store") {
    check_kept(seq({"Mov(Reg(A) ZP(128 0) true)", "ArithmeticShiftLeft(ZP(128 0))", kLdaM,
                    kRtsA}));
  }
  SUBCASE("a shift of another byte") {
    check_kept(seq({kStaM, "ArithmeticShiftLeft(ZP(129 0))", "Mov(ZP(129 0) Reg(A) false)",
                    kRtsA}));
  }
  SUBCASE("INC with C read after") {
    check_kept(seq({kStaM, "Inc(ZP(128 0))", kLdaM, "Add(ZP(129 0) Reg(A))", kRtsA}));
  }
  SUBCASE("INC with no reload") {
    check_kept(seq({kStaM, "Inc(ZP(128 0))", kRts}));
  }
}

TEST_CASE("shift_in_a shifts a loaded byte in place") {
  SUBCASE("one shift") {
    CHECK(run(seq({kLdaM, "ArithmeticShiftLeft(Reg(A))", kStaM, kRts})) ==
          expect(seq({"ArithmeticShiftLeft(ZP(128 0))", kRts})));
  }
  SUBCASE("two shifts") {
    CHECK(run(seq({kLdaM, "LogicalShiftRight(Reg(A))", "RotateRight(Reg(A))", kStaM, kRts})) ==
          expect(seq({"LogicalShiftRight(ZP(128 0))", "RotateRight(ZP(128 0))", kRts})));
  }
  SUBCASE("an element indexed by X") {
    CHECK(run(seq({R"(Mov(IndexedData("t" 0 X) Reg(A) false))", "ArithmeticShiftLeft(Reg(A))",
                   R"(Mov(Reg(A) IndexedData("t" 0 X) false))", kRts})) ==
          expect(seq({R"(ArithmeticShiftLeft(IndexedData("t" 0 X)))", kRts})));
  }
  SUBCASE("three shifts are left alone") {
    check_kept(seq({kLdaM, "ArithmeticShiftLeft(Reg(A))", "ArithmeticShiftLeft(Reg(A))",
                    "ArithmeticShiftLeft(Reg(A))", kStaM, kRts}));
  }
  SUBCASE("A is read after") {
    check_kept(seq({kLdaM, "ArithmeticShiftLeft(Reg(A))", kStaM, kRtsA}));
  }
  SUBCASE("another byte") {
    check_kept(seq({kLdaM, "ArithmeticShiftLeft(Reg(A))", "Mov(Reg(A) ZP(129 0) false)", kRts}));
  }
  SUBCASE("a volatile byte") {
    // An NMOS 6502 read-modify-write writes the old value back first.
    check_kept(seq({"Mov(ZP(128 0) Reg(A) true)", "ArithmeticShiftLeft(Reg(A))",
                    "Mov(Reg(A) ZP(128 0) true)", kRts}));
  }
}

TEST_CASE("shift_in_a moves a loop of one-bit shifts onto A") {
  // STA M; <setup>; .l: LSR M; DEX; BNE .l; [.d:] LDA M; <after>; RTS.
  const auto loop = [](std::string_view setup, std::string_view after, bool done) {
    return seq({kStaM, setup, R"(Label(".l") LogicalShiftRight(ZP(128 0)) Dec(Reg(X)))",
                R"(Branch(NE ".l"))", done ? R"(Label(".d"))" : "", kLdaM, after, kRtsA});
  };
  const char* variable = R"(Compare(Reg(X) Imm(0)) Branch(EQ ".d"))";
  SUBCASE("a count in X") {
    CHECK(run(loop(variable, "", true)) ==
          expect(seq({kStaM, variable, R"(Label(".l") LogicalShiftRight(Reg(A)) Dec(Reg(X)))",
                      R"(Branch(NE ".l") Label(".d"))", kStaM, kRtsA})));
  }
  SUBCASE("a constant count") {
    CHECK(run(loop("Mov(Imm(3) Reg(X) false)", "", false)) ==
          expect(seq({kStaM, "Mov(Imm(3) Reg(X) false)",
                      R"(Label(".l") LogicalShiftRight(Reg(A)) Dec(Reg(X)) Branch(NE ".l"))",
                      kStaM, kRtsA})));
  }
  SUBCASE("the flags are read after the load") {
    check_kept(loop(variable, R"(Branch(EQ ".z") Label(".z"))", true));
  }
  SUBCASE("the setup writes A") {
    check_kept(loop(std::string("Mov(ZP(129 0) Reg(A) false) Mov(Reg(A) Reg(X) false) ") +
                        variable,
                    "", true));
  }
  SUBCASE("the loop's label has another way in") {
    check_kept(loop(variable, "", true) + R"(Jump(".l" []))");
  }
}
