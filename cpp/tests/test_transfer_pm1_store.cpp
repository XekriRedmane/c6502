// The tests of tests/test_transfer_pm1_store.py, ported.
#include <string>

#include "asm_text.h"
#include "doctest.h"
#include "passes/asm_context.h"
#include "passes/transfer_pm1_store.h"

namespace {

using c6502::asm_text::expect;
using c6502::asm_text::instructions;
using c6502::asm_text::program;
using std::string;

string run(const string& instrs, const string& statics = "") {
  return instructions(c6502::passes::apply_transfer_pm1_store(
      program(instrs, "f", statics), c6502::passes::AsmContext{}));
}

string mov(const string& src, const string& dst, bool is_volatile = false) {
  return "Mov(" + src + " " + dst + (is_volatile ? " true) " : " false) ");
}
const string kA = "Reg(A)";
const string kX = "Reg(X)";
const string kY = "Reg(Y)";
const string kRts = "Return(false false false) ";
const string kRtsA = "Return(true false false) ";
const string kSec = "SetCarry ";
const string kClc = "ClearCarry ";
string sbc(int k) { return "Sub(Imm(" + std::to_string(k) + ") Reg(A)) "; }
string adc(int k) { return "Add(Imm(" + std::to_string(k) + ") Reg(A)) "; }
string inc(const string& reg) { return "Inc(" + reg + ") "; }
string dec(const string& reg) { return "Dec(" + reg + ") "; }

}  // namespace

TEST_CASE("transfer_pm1_store folds a step through A into a step of X or Y") {
  const string ctr = R"(Data("ctr" 0))";
  const string zp = "ZP(133 0)";
  SUBCASE("TXA; SEC; SBC #1; STA M is DEX; STX M") {
    CHECK(run(mov(kX, kA) + kSec + sbc(1) + mov(kA, ctr) + kRts) ==
          expect(dec(kX) + mov(kX, ctr) + kRts));
  }
  SUBCASE("TYA; SEC; SBC #1; STA zp is DEY; STY zp") {
    CHECK(run(mov(kY, kA) + kSec + sbc(1) + mov(kA, zp) + kRts) ==
          expect(dec(kY) + mov(kY, zp) + kRts));
  }
  SUBCASE("TXA; CLC; ADC #1; STA M is INX; STX M") {
    CHECK(run(mov(kX, kA) + kClc + adc(1) + mov(kA, ctr) + kRts) ==
          expect(inc(kX) + mov(kX, ctr) + kRts));
  }
  SUBCASE("TYA; CLC; ADC #1; STA zp is INY; STY zp") {
    CHECK(run(mov(kY, kA) + kClc + adc(1) + mov(kA, zp) + kRts) ==
          expect(inc(kY) + mov(kY, zp) + kRts));
  }
  SUBCASE("with a spill of A around it, which the fold leaves for later sweeps") {
    const string local = R"(Data("__local_f__2" 0))";
    CHECK(run(mov("Imm(16)", kA) + mov(kA, local) + mov(kX, kA) + kSec + sbc(1) +
              mov(kA, ctr) + mov(local, kA) + mov("Imm(10)", kX) +
              R"(Call("snd_delay_up" ["A" "X"] []) )" + kRts) ==
          expect(mov("Imm(16)", kA) + mov(kA, local) + dec(kX) + mov(kX, ctr) +
                 mov(local, kA) + mov("Imm(10)", kX) +
                 R"(Call("snd_delay_up" ["A" "X"] []) )" + kRts));
  }
  SUBCASE("when a branch reads N/Z, which INY sets as the ADC did") {
    // `get_next_code_byte2`'s `if (++Z_PC2_L == 0)`, from when Zork's
    // print_zstring example kept Z_PC2 in three bytes.
    const string m = R"(Data("Z_PC2_L" 0))";
    CHECK(run(mov(kY, kA) + kClc + adc(1) + mov(kA, m) + R"(Branch(NE ".l") Label(".l") )" +
              kRts) ==
          expect(inc(kY) + mov(kY, m) + R"(Branch(NE ".l") Label(".l") )" + kRts));
  }
}

TEST_CASE("transfer_pm1_store keeps the store from A where the fold isn't sound") {
  const string m = R"(Data("m" 0))";
  SUBCASE("A read after, where the step moves to X before the TXA") {
    // A holds X - 1 for both stores.
    const string n = R"(Data("n" 0))";
    CHECK(run(mov(kX, kA) + kSec + sbc(1) + mov(kA, m) + mov(kA, n) + kRts) ==
          expect(dec(kX) + mov(kX, kA) + mov(kA, m) + mov(kA, n) + kRts));
  }
  SUBCASE("X read after by an indexed load") {
    const string text = mov(kX, kA) + kSec + sbc(1) + mov(kA, m) +
                        mov(R"(IndexedData("tbl" 0 X))", kA) + mov(kA, R"(Data("n" 0))") +
                        kRts;
    CHECK(run(text) == expect(text));
  }
  SUBCASE("a branch on Z, where X is live at the target") {
    // The SBC sets C too, which DEX leaves stale.
    const string text =
        mov(kX, kA) + kSec + sbc(1) + mov(kA, m) + R"(Branch(NE ".skip") )" + kRts;
    CHECK(run(text) == expect(text));
  }
  SUBCASE("a store through DPTR, which STX can't address") {
    CHECK(run(mov(kX, kA) + kSec + sbc(1) + mov(kA, "Indirect(0)") + kRts) ==
          expect(dec(kX) + mov(kX, kA) + mov(kA, "Indirect(0)") + kRts));
  }
  SUBCASE("a store to the frame, which STX can't address") {
    CHECK(run(mov(kX, kA) + kSec + sbc(1) + mov(kA, "Frame(1)") + kRts) ==
          expect(dec(kX) + mov(kX, kA) + mov(kA, "Frame(1)") + kRts));
  }
  SUBCASE("CLC before SBC #1, which doesn't step by 1") {
    const string text = mov(kX, kA) + kClc + sbc(1) + mov(kA, m) + kRts;
    CHECK(run(text) == expect(text));
  }
  SUBCASE("SEC before ADC #1, which doesn't step by 1") {
    const string text = mov(kX, kA) + kSec + adc(1) + mov(kA, m) + kRts;
    CHECK(run(text) == expect(text));
  }
  SUBCASE("SBC #3, which no step folds") {
    const string text = mov(kX, kA) + kSec + sbc(3) + mov(kA, m) + kRts;
    CHECK(run(text) == expect(text));
  }
  SUBCASE("ADC #2, where two INX go before the TXA") {
    CHECK(run(mov(kX, kA) + kClc + adc(2) + mov(kA, m) + kRts) ==
          expect(inc(kX) + inc(kX) + mov(kX, kA) + mov(kA, m) + kRts));
  }
  SUBCASE("SBC #2, which DEX can't do in one, where two DEX go before the TXA") {
    CHECK(run(mov(kX, kA) + kSec + sbc(2) + mov(kA, m) + kRts) ==
          expect(dec(kX) + dec(kX) + mov(kX, kA) + mov(kA, m) + kRts));
  }
  SUBCASE("a volatile store, where the step moves to X before the TXA") {
    CHECK(run(mov(kX, kA) + kSec + sbc(1) + mov(kA, m, true) + kRts) ==
          expect(dec(kX) + mov(kX, kA) + mov(kA, m, true) + kRts));
  }
  SUBCASE("C read by a BCS") {
    const string text = mov(kY, kA) + kClc + adc(1) + mov(kA, R"(Data("lo" 0))") +
                        R"(Branch(CS ".l") Label(".l") )" + kRts;
    CHECK(run(text) == expect(text));
  }
}

namespace {

const string kIdx = R"(Data("idx" 0))";
const string kP = "IndirectZpY(144)";

// `LDY idx; STA (p),Y; <between>; LDA idx; CLC; ADC #k; TAY; STX (p),Y;
// RTS`, with `reg` in place of Y.
string index_step(int k = 1, const string& between = "", const string& reg = kY) {
  return mov(kIdx, reg) + mov(kA, kP) + between + mov(kIdx, kA) + kClc + adc(k) +
         mov(kA, reg) + mov(kX, kP) + kRts;
}

// The rewrite of `index_step` for a register that doesn't hold `idx`:
// `CLC; ADC #k; TAY` as `TAY; INY` k times.
string stepped_after_transfer(int k = 1, const string& between = "",
                              const string& reg = kY) {
  string out = mov(kIdx, reg) + mov(kA, kP) + between + mov(kIdx, kA) + mov(kA, reg);
  for (int i = 0; i < k; ++i) {
    out += inc(reg);
  }
  return out + mov(kX, kP) + kRts;
}

// `idx` as a static the unit never takes the address of, or as a
// global another unit may point at.
string idx_static(bool is_global = false) {
  return string(R"(StaticVariable("idx" )") + (is_global ? "true" : "false") +
         " [ZeroInit(1)]) ";
}

}  // namespace

TEST_CASE("transfer_pm1_store steps an index register that holds the byte") {
  SUBCASE("INY") {
    CHECK(run(index_step(), idx_static()) ==
          expect(mov(kIdx, kY) + mov(kA, kP) + inc(kY) + mov(kX, kP) + kRts));
  }
  SUBCASE("two INYs") {
    CHECK(run(index_step(2), idx_static()) ==
          expect(mov(kIdx, kY) + mov(kA, kP) + inc(kY) + inc(kY) + mov(kX, kP) + kRts));
  }
  SUBCASE("but not when a store through a pointer may change a global") {
    CHECK(run(index_step(), idx_static(true)) == expect(stepped_after_transfer()));
  }
  SUBCASE("but not when the register changed since the load") {
    // INX changes X after `LDX idx`, which `kills_x` doesn't count
    // since INX also reads X. beam_target_tick read the wrong input
    // when this rewrote `LDA m; CLC; ADC #2; TAX` to `INX; INX`.
    CHECK(run(index_step(2, inc(kX), kX), idx_static()) ==
          expect(stepped_after_transfer(2, inc(kX), kX)));
  }
  SUBCASE("but not when the byte was written since the load") {
    CHECK(run(index_step(1, mov(kA, kIdx)), idx_static()) ==
          expect(stepped_after_transfer(1, mov(kA, kIdx))));
  }
}

TEST_CASE("transfer_pm1_store moves a step across a transfer") {
  SUBCASE("CLC; ADC #1; TAY is TAY; INY") {
    CHECK(run(kClc + adc(1) + mov(kA, kY) + kRts) == expect(mov(kA, kY) + inc(kY) + kRts));
  }
  SUBCASE("but not with A read after") {
    const string text = kClc + adc(1) + mov(kA, kY) + kRtsA;
    CHECK(run(text) == expect(text));
  }
  SUBCASE("TYA; CLC; ADC #2 is INY; INY; TYA") {
    const string m = R"(Data("m" 0))";
    CHECK(run(mov(kY, kA) + kClc + adc(2) + mov(kA, m) + kRts) ==
          expect(inc(kY) + inc(kY) + mov(kY, kA) + mov(kA, m) + kRts));
  }
  SUBCASE("but not with Y read after") {
    const string m = R"(Data("m" 0))";
    const string text = mov(kY, kA) + kClc + adc(1) + mov(kA, m) + mov(kY, m) + kRts;
    CHECK(run(text) == expect(text));
  }
  SUBCASE("TXA; CLC; ADC #1; TAX is INX") {
    const string tbl = R"(IndexedData("tbl" 0 X))";
    CHECK(run(mov(kX, kA) + kClc + adc(1) + mov(kA, kX) + mov(tbl, kA) + kRtsA) ==
          expect(inc(kX) + mov(tbl, kA) + kRtsA));
  }
}

TEST_CASE("transfer_pm1_store steps a byte stored and loaded into a register") {
  const string m = R"(Data("__local_f__x" 0))";
  const string v = R"(Data("v" 0))";
  SUBCASE("LDA M; SEC; SBC #2; STA M; LDX M is LDX M; DEX; DEX; STX M") {
    CHECK(run(mov(m, kA) + kSec + sbc(2) + mov(kA, m) + mov(m, kX) + mov(v, kA) + kRts) ==
          expect(mov(m, kX) + dec(kX) + dec(kX) + mov(kX, m) + mov(v, kA) + kRts));
  }
  SUBCASE("but not with A read after") {
    const string text =
        mov(m, kA) + kSec + sbc(2) + mov(kA, m) + mov(m, kX) + mov(kA, v) + kRts;
    CHECK(run(text) == expect(text));
  }
}
