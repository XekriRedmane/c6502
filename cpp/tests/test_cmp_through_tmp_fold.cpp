// The tests of tests/test_cmp_through_tmp_fold.py, ported.
#include <string>

#include "asm_text.h"
#include "doctest.h"
#include "passes/cmp_through_tmp_fold.h"

namespace {

using c6502::asm_text::expect;
using c6502::asm_text::instructions;
using c6502::asm_text::program;

std::string run(const std::string& instrs) {
  return instructions(
      c6502::passes::apply_cmp_through_tmp_fold(program(instrs), c6502::passes::AsmContext{}));
}

std::string lda(const std::string& src) { return "Mov(" + src + " Reg(A) false) "; }
std::string sta(const std::string& dst) { return "Mov(Reg(A) " + dst + " false) "; }
std::string cmp(const std::string& right) { return "Compare(Reg(A) " + right + ") "; }
std::string data(const char* name, int offset = 0) {
  return std::string("Data(\"") + name + "\" " + std::to_string(offset) + ")";
}

const std::string kS = data("S");
const std::string kT = data("T");
const std::string kO = data("O");
const std::string kRetA = "Return(true false false) ";
const std::string kRet = "Return(false false false) ";

}  // namespace

TEST_CASE("cmp_through_tmp_fold folds a compare through a temporary") {
  SUBCASE("an element indexed by X") {
    // The beam_target_tick shape: LDA floor_y_table,X; STA tmp; LDA
    // beam_y; CMP tmp.
    const std::string src = R"(IndexedData("floor_y_table" 0 X))";
    const std::string tmp = data("tmp");
    const std::string other = data("beam_y");
    CHECK(run(lda(src) + sta(tmp) + lda(other) + cmp(tmp)) == expect(lda(other) + cmp(src)));
  }
  SUBCASE("a volatile source") {
    // `++CURR_LINE >= WNDBTM`, with the Monitor's volatile WNDBTM. The
    // CMP still reads it once.
    const std::string src = data("WNDBTM");
    const std::string tmp = data("tmp");
    const std::string other = data("line");
    CHECK(run("Mov(" + src + " Reg(A) true) " + sta(tmp) + lda(other) + cmp(tmp)) ==
          expect(lda(other) + cmp(src)));
  }
  SUBCASE("two volatile loads stay") {
    // Folding would read the second volatile byte first.
    const std::string tmp = data("tmp");
    const std::string text = "Mov(" + data("WNDBTM") + " Reg(A) true) " + sta(tmp) + "Mov(" +
                             data("KBD") + " Reg(A) true) " + cmp(tmp);
    CHECK(run(text) == expect(text));
  }
  SUBCASE("Data, ZP, and immediate sources") {
    CHECK(run(lda(kS) + sta(kT) + lda(kO) + cmp(kT)) == expect(lda(kO) + cmp(kS)));
    CHECK(run(lda("ZP(144 0)") + sta("ZP(132 0)") + lda(kO) + cmp("ZP(132 0)")) ==
          expect(lda(kO) + cmp("ZP(144 0)")));
    CHECK(run(lda("Imm(66)") + sta("ZP(132 0)") + lda(kO) + cmp("ZP(132 0)")) ==
          expect(lda(kO) + cmp("Imm(66)")));
  }
  SUBCASE("an element indexed by Y with a Data other") {
    // Safe, because a load of Data doesn't write Y.
    const std::string src = R"(IndexedData("tbl" 0 Y))";
    CHECK(run(lda(src) + sta(kT) + lda(kO) + cmp(kT)) == expect(lda(kO) + cmp(src)));
  }
}

TEST_CASE("cmp_through_tmp_fold folds the arithmetic and logic forms") {
  SUBCASE("SBC after SEC") {
    // do_rwts_on_sector's `sector - SECTORS_PER_TRACK`, staged through
    // one of the function's own slots.
    const std::string src = data("SECTORS_PER_TRACK");
    const std::string tmp = data("__local_f__T");
    const std::string other = data("sector");
    CHECK(run(lda(src) + sta(tmp) + lda(other) + "SetCarry Sub(" + tmp + " Reg(A)) " + kRetA) ==
          expect(lda(other) + "SetCarry Sub(" + src + " Reg(A)) " + kRetA));
  }
  SUBCASE("a global temporary stays when it may be read elsewhere") {
    // `T` is a global, which other functions may read, so its store
    // stays although nothing else in this function reads it.
    const std::string text =
        lda(kS) + sta(kT) + lda(kO) + "SetCarry Sub(" + kT + " Reg(A)) " + kRetA;
    CHECK(run(text) == expect(text));
  }
  SUBCASE("AND, ORA, EOR, and ADC") {
    const std::string src = R"(IndexedData("tab" 0 X))";
    const std::string head = lda(src) + sta(kT) + lda(kO);
    CHECK(run(head + "And(" + kT + " Reg(A))") == expect(lda(kO) + "And(" + src + " Reg(A))"));
    CHECK(run(head + "Or(" + kT + " Reg(A))") == expect(lda(kO) + "Or(" + src + " Reg(A))"));
    CHECK(run(head + "Xor(Reg(A) " + kT + " Reg(A))") ==
          expect(lda(kO) + "Xor(Reg(A) " + src + " Reg(A))"));
    CHECK(run(head + "ClearCarry Add(" + kT + " Reg(A))") ==
          expect(lda(kO) + "ClearCarry Add(" + src + " Reg(A))"));
  }
  SUBCASE("the ALU needs a source it can address") {
    // SBC has no `(zp)` form without Y, which IndirectZp loads.
    const std::string text =
        lda("IndirectZp(144 0)") + sta(kT) + lda(kO) + "SetCarry Sub(" + kT + " Reg(A))";
    CHECK(run(text) == expect(text));
  }
  SUBCASE("a later read of the temporary") {
    const std::string text =
        lda(kS) + sta(kT) + lda(kO) + "SetCarry Sub(" + kT + " Reg(A)) " + lda(kT) + kRetA;
    CHECK(run(text) == expect(text));
  }
}

TEST_CASE("cmp_through_tmp_fold leaves the unsafe shapes alone") {
  SUBCASE("an element indexed by Y with a Frame other") {
    // A Frame other is written with an LDY #off that changes Y.
    const std::string text = lda(R"(IndexedData("tbl" 0 Y))") + sta(kT) + lda("Frame(4)") +
                             cmp(kT);
    CHECK(run(text) == expect(text));
  }
  SUBCASE("the store and the compare name different temporaries") {
    const std::string text = lda(kS) + sta(data("T1")) + lda(kO) + cmp(data("T2"));
    CHECK(run(text) == expect(text));
  }
  SUBCASE("other is the temporary") {
    // The LDA other reads the value the STA wrote (src), and the
    // rewrite would read the value from before it.
    const std::string text = lda(kS) + sta(kT) + lda(kT) + cmp(kT);
    CHECK(run(text) == expect(text));
  }
  SUBCASE("an indirect other") {
    // The byte it reads at run time could be the temporary.
    const std::string text = lda(kS) + sta(kT) + lda("Indirect(0)") + cmp(kT);
    CHECK(run(text) == expect(text));
  }
  SUBCASE("a volatile store") {
    // A volatile store to the temporary has to happen. A volatile first
    // load alone folds.
    const std::string text = lda(kS) + "Mov(Reg(A) " + kT + " true) " + lda(kO) + cmp(kT);
    CHECK(run(text) == expect(text));
  }
  SUBCASE("a second compare with the temporary") {
    // `LDA src; STA tmp; LDA o1; CMP tmp; LDA o2; CMP tmp`. Folding the
    // first would drop the only STA tmp and leave the second CMP reading
    // a stale slot, as in the back-to-back column-band test in
    // beam_target_draw (`screen_col > 0x25` and then `screen_col <=
    // 0x23`).
    const std::string text = lda(R"(IndexedData("proj_screen_col" 0 X))") + sta(kT) +
                             lda("Imm(37)") + cmp(kT) + lda("Imm(35)") + cmp(kT);
    CHECK(run(text) == expect(text));
  }
  SUBCASE("a later load of the temporary") {
    // Any later read of tmp blocks the fold.
    const std::string text =
        lda(kS) + sta(kT) + lda(kO) + cmp(kT) + lda(kT) + sta(data("dst"));
    CHECK(run(text) == expect(text));
  }
  SUBCASE("an EOR that reads the temporary after the candidate") {
    // The scan of the reads of tmp once read `src` on an Xor, which has
    // `src1` / `src2`. The Xor reads tmp, so the fold must not fire.
    const std::string text =
        lda(kS) + sta(kT) + lda(kO) + cmp(kT) + "Xor(Reg(A) " + kT + " Reg(A))";
    CHECK(run(text) == expect(text));
  }
}

TEST_CASE("cmp_through_tmp_fold compares with a byte read through a pointer") {
  // The `LDA (p),Y; STA tmp; LDA key; CMP tmp` shape of comparing a key
  // with a byte read through a pointer.
  const std::string src = "IndirectZpY(144)";
  const std::string tmp = "ZP(132 0)";
  const std::string key = data("key", 1);
  SUBCASE("it folds") {
    CHECK(run(lda(src) + sta(tmp) + lda(key) + cmp(tmp) + kRet) ==
          expect(lda(key) + cmp(src) + kRet));
  }
  SUBCASE("the slot is used again for another value") {
    // The slot is written again for an unrelated value and read after
    // that, but the value this CMP read is dead after it.
    const std::string tail = lda("Imm(3)") + sta(tmp) + lda(tmp) + kRetA;
    CHECK(run(lda(src) + sta(tmp) + lda(key) + cmp(tmp) + tail) ==
          expect(lda(key) + cmp(src) + tail));
  }
  SUBCASE("kept when the temporary is read after") {
    const std::string text = lda(src) + sta(tmp) + lda(key) + cmp(tmp) + lda(tmp) + kRetA;
    CHECK(run(text) == expect(text));
  }
  SUBCASE("SBC through a pointer") {
    // `40 - s[0]`: `LDA (s),Y; STA t; LDA #$28; SEC; SBC t` is `LDA #$28;
    // SEC; SBC (s),Y`, with Y loaded before as it was.
    CHECK(run(lda(src) + sta(tmp) + lda("Imm(40)") + "SetCarry Sub(" + tmp + " Reg(A)) " +
              kRetA) == expect(lda("Imm(40)") + "SetCarry Sub(" + src + " Reg(A)) " + kRetA));
  }
  SUBCASE("SBC through a pointer after a load from the frame") {
    // Loading `other` from the soft stack loads Y first.
    const std::string text =
        lda(src) + sta(tmp) + lda("Frame(3)") + "SetCarry Sub(" + tmp + " Reg(A)) " + kRetA;
    CHECK(run(text) == expect(text));
  }
  SUBCASE("an other with a Y setup") {
    // Loading `other` through an operand with an LDY setup would change
    // the Y that `(p),Y` reads.
    const std::string text = lda(src) + sta(tmp) + lda("Frame(3)") + cmp(tmp) + kRet;
    CHECK(run(text) == expect(text));
  }
  SUBCASE("an other read through a pointer or by Y") {
    // These load no Y, but each could read the temporary.
    for (const char* other : {"IndirectZpY(150)", "IndirectY", R"(IndexedData("t" 0 Y))"}) {
      const std::string text = lda(src) + sta(tmp) + lda(other) + cmp(tmp) + kRet;
      CHECK(run(text) == expect(text));
    }
  }
  SUBCASE("a BIT of the temporary") {
    // `BIT tmp` after the compare reads the staged byte, so the store
    // stays.
    const std::string text =
        lda(kS) + sta(kT) + lda(kO) + cmp(kT) + "BitTest(" + kT + ") " + kRet;
    CHECK(run(text) == expect(text));
  }
}
