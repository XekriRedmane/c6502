// The tests of tests/test_adc_commute.py, ported.
#include <string>

#include "asm_text.h"
#include "doctest.h"
#include "passes/adc_commute.h"
#include "passes/asm_context.h"

namespace {

using c6502::asm_text::expect;
using c6502::asm_text::instructions;
using c6502::asm_text::program;
using std::string;

string run(const string& instrs) {
  return instructions(c6502::passes::apply_adc_commute(program(instrs),
                                                       c6502::passes::AsmContext{}));
}

string zp(int addr) { return "ZP(" + std::to_string(addr) + " 0)"; }
string data(const string& name) { return "Data(\"" + name + "\" 0)"; }
string ix(const string& name, const char* index) {
  return "IndexedData(\"" + name + "\" 0 " + index + ")";
}
string lda(const string& src) { return "Mov(" + src + " Reg(A) false) "; }
string sta(const string& dst) { return "Mov(Reg(A) " + dst + " false) "; }
string ldx(const string& src) { return "Mov(" + src + " Reg(X) false) "; }
string ldy(const string& src) { return "Mov(" + src + " Reg(Y) false) "; }
string adc(const string& src) { return "Add(" + src + " Reg(A)) "; }
string and_(const string& src) { return "And(" + src + " Reg(A)) "; }
string or_(const string& src) { return "Or(" + src + " Reg(A)) "; }
const string kClc = "ClearCarry ";
const string kSec = "SetCarry ";
const string kRtsA = "Return(true false false) ";

}  // namespace

TEST_CASE("adc_commute reads mem in the operation and drops the load") {
  SUBCASE("ADC of a ZP spill, the instructions adjacent") {
    CHECK(run(sta(zp(0x80)) + lda(data("M")) + kClc + adc(zp(0x80)) + sta(data("M"))) ==
          expect(sta(zp(0x80)) + kClc + adc(data("M")) + sta(data("M"))));
  }
  SUBCASE("AND") {
    CHECK(run(sta(zp(0x80)) + lda(data("M")) + and_(zp(0x80)) + sta(data("M"))) ==
          expect(sta(zp(0x80)) + and_(data("M")) + sta(data("M"))));
  }
  SUBCASE("ORA") {
    CHECK(run(sta(zp(0x80)) + lda(data("M")) + or_(zp(0x80)) + sta(data("M"))) ==
          expect(sta(zp(0x80)) + or_(data("M")) + sta(data("M"))));
  }
  SUBCASE("SEC in place of CLC") {
    // The pass leaves C alone, so either sets it.
    CHECK(run(sta(zp(0x80)) + lda(data("M")) + kSec + adc(zp(0x80)) + sta(data("M"))) ==
          expect(sta(zp(0x80)) + kSec + adc(data("M")) + sta(data("M"))));
  }
  SUBCASE("no CLC or SEC, with the carry from before") {
    CHECK(run(sta(zp(0x80)) + lda(data("M")) + adc(zp(0x80)) + sta(data("M"))) ==
          expect(sta(zp(0x80)) + adc(data("M")) + sta(data("M"))));
  }
  SUBCASE("an element indexed by X, with its LDX in the band") {
    CHECK(run(sta(zp(0x82)) + ldx(data("slot")) + lda(ix("arr", "X")) + kClc +
              adc(zp(0x82)) + sta(ix("arr", "X"))) ==
          expect(sta(zp(0x82)) + ldx(data("slot")) + kClc + adc(ix("arr", "X")) +
                 sta(ix("arr", "X"))));
  }
  SUBCASE("an element indexed by Y") {
    CHECK(run(sta(zp(0x82)) + ldy(data("slot")) + lda(ix("arr", "Y")) + kClc +
              adc(zp(0x82)) + sta(ix("arr", "Y"))) ==
          expect(sta(zp(0x82)) + ldy(data("slot")) + kClc + adc(ix("arr", "Y")) +
                 sta(ix("arr", "Y"))));
  }
  SUBCASE("a second STA of the spill anchors the match") {
    // The second STA writes the spill, so the match from the first
    // ends there, and the match from the second fires.
    CHECK(run(sta(zp(0x80)) + sta(zp(0x80)) + lda(data("M")) + adc(zp(0x80)) +
              sta(data("M"))) ==
          expect(sta(zp(0x80)) + sta(zp(0x80)) + adc(data("M")) + sta(data("M"))));
  }
  SUBCASE("the result stored elsewhere") {
    // `ADC M` reads the byte the load did, so the sum is the same.
    CHECK(run(sta(zp(0x80)) + lda(data("M")) + adc(zp(0x80)) + sta(data("N"))) ==
          expect(sta(zp(0x80)) + adc(data("M")) + sta(data("N"))));
  }
  SUBCASE("two windows apart") {
    CHECK(run(sta(zp(0x80)) + lda(data("M")) + kClc + adc(zp(0x80)) + sta(data("M")) +
              "Label(\".mid\") " + sta(zp(0x82)) + lda(data("N")) + kClc +
              adc(zp(0x82)) + sta(data("N"))) ==
          expect(sta(zp(0x80)) + kClc + adc(data("M")) + sta(data("M")) +
                 "Label(\".mid\") " + sta(zp(0x82)) + kClc + adc(data("N")) +
                 sta(data("N"))));
  }
  SUBCASE("without a store of the result") {
    // `print_zstring`'s `get_alphabet() + z`.
    CHECK(run(sta(zp(0x80)) + lda(data("z")) + kClc + adc(zp(0x80)) + kRtsA) ==
          expect(sta(zp(0x80)) + kClc + adc(data("z")) + kRtsA));
  }
}

TEST_CASE("adc_commute leaves other shapes alone") {
  const string shapes[] = {
      // A load of A between: the first load is taken as the window's,
      // and the second isn't an operation.
      sta(zp(0x80)) + lda(data("X")) + lda(data("M")) + adc(zp(0x80)) + sta(data("M")),
      // A volatile load.
      sta(zp(0x80)) + "Mov(Data(\"M\" 0) Reg(A) true) " + adc(zp(0x80)) + sta(data("M")),
      // A volatile spill.
      "Mov(Reg(A) ZP(128 0) true) " + lda(data("M")) + adc(zp(0x80)) + sta(data("M")),
      // SBC isn't commutative.
      sta(zp(0x80)) + lda(data("M")) + kSec + "Sub(ZP(128 0) Reg(A)) " + sta(data("M")),
      // A label in the band would need an analysis across blocks.
      sta(zp(0x80)) + "Label(\".L1\") " + lda(data("M")) + adc(zp(0x80)) + sta(data("M")),
      // A call in the band.
      sta(zp(0x80)) + "Call(\"helper\" [] []) " + lda(data("M")) + adc(zp(0x80)) +
          sta(data("M")),
  };
  for (const auto& text : shapes) {
    CAPTURE(text);
    CHECK(run(text) == expect(text));
  }
}

namespace {

// STX t, the instructions `between`, a load of `load`, CLC, and ADC t.
string register_copy(const string& between, const string& load = "Imm(17)") {
  const string t = data("__local_f__t");
  return "Mov(Reg(X) " + t + " false) " + between + lda(load) + kClc + adc(t) + kRtsA;
}

}  // namespace

TEST_CASE("adc_commute reads X when X holds the byte added") {
  SUBCASE("a constant plus the stored X") {
    const string t = data("__local_f__t");
    CHECK(run(register_copy(ldy("Imm(5)"))) ==
          expect("Mov(Reg(X) " + t + " false) " + ldy("Imm(5)") +
                 "Mov(Reg(X) Reg(A) false) " + kClc + adc("Imm(17)") + kRtsA));
  }
  SUBCASE("a byte in memory plus the stored X") {
    const string t = data("__local_f__t");
    CHECK(run(register_copy("", data("g"))) ==
          expect("Mov(Reg(X) " + t + " false) Mov(Reg(X) Reg(A) false) " + kClc +
                 adc(data("g")) + kRtsA));
  }
  SUBCASE("but not when X changes") {
    const string text = register_copy("Inc(Reg(X)) ");
    CHECK(run(text) == expect(text));
  }
  SUBCASE("but not when the byte is written") {
    const string text = register_copy(sta(data("__local_f__t")));
    CHECK(run(text) == expect(text));
  }
  SUBCASE("but not across a label") {
    const string text = register_copy("Label(\".l\") ");
    CHECK(run(text) == expect(text));
  }
  SUBCASE("but not across a call") {
    const string text = register_copy("Call(\"g\" [] []) ");
    CHECK(run(text) == expect(text));
  }
}
