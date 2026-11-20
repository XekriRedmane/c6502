// The tests of tests/test_transfer_past_branch.py, ported.
#include <string>

#include "asm_text.h"
#include "doctest.h"
#include "passes/transfer_past_branch.h"

namespace {

using c6502::asm_text::expect;
using c6502::asm_text::instructions;
using c6502::asm_text::program;

std::string run(const std::string& instrs) {
  return instructions(c6502::passes::apply_transfer_past_branch(program(instrs)));
}

const std::string kRet = "Return(false false false) ";
// A fall-through that reads A, so A is live there.
const std::string kUseA = "And(Imm(15) Reg(A)) Mov(Reg(A) ZP(130 0) false) " + kRet;
// A target that writes A before reading it, so A is dead there.
const std::string kKillA = R"(Label(".tgt") Mov(Imm(0) Reg(A) false) )" + kRet;

std::string load(const char* reg) {
  return std::string(R"(Mov(Data("state" 0) Reg()") + reg + ") false) ";
}
std::string transfer(const char* reg) { return std::string("Mov(Reg(") + reg + ") Reg(A) false) "; }
std::string branch(const char* cond, const char* target) {
  return std::string("Branch(") + cond + " \"" + target + "\") ";
}

}  // namespace

TEST_CASE("transfer_past_branch moves the transfer after the branch") {
  SUBCASE("LDX; TXA; BPL") {
    // LDX M; TXA; BPL T with A read on the fall-through and dead at T
    // becomes LDX M; BPL T; TXA.
    CHECK(run(load("X") + transfer("X") + branch("PL", ".tgt") + kUseA + kKillA) ==
          expect(load("X") + branch("PL", ".tgt") + transfer("X") + kUseA + kKillA));
  }
  SUBCASE("LDY; TYA; BEQ") {
    CHECK(run(load("Y") + transfer("Y") + branch("EQ", ".tgt") + kUseA + kKillA) ==
          expect(load("Y") + branch("EQ", ".tgt") + transfer("Y") + kUseA + kKillA));
  }
}

TEST_CASE("transfer_past_branch leaves the transfer where it is") {
  SUBCASE("A is read at the target") {
    // The target would lose the TXA's A := X.
    const std::string text = load("X") + transfer("X") + branch("PL", ".tgt") + kUseA +
                             R"(Label(".tgt") Mov(Reg(A) ZP(130 0) false) )" + kRet;
    CHECK(run(text) == expect(text));
  }
  SUBCASE("A is dead on both arms") {
    // dead_a_arith drops the TXA then, and nothing has to move.
    const std::string text = load("X") + transfer("X") + branch("PL", ".tgt") +
                             "Mov(Imm(0) Reg(A) false) " + kRet + kKillA;
    CHECK(run(text) == expect(text));
  }
  SUBCASE("the instruction before doesn't set the flags from X") {
    // LDA M sets the flags from M, so the TXA's flags are the ones the
    // branch reads.
    const std::string text =
        load("A") + transfer("X") + branch("PL", ".tgt") + kUseA + kKillA;
    CHECK(run(text) == expect(text));
  }
  SUBCASE("the target is outside the function") {
    // A tail call or the trampoline of a long branch, where A can't be
    // shown dead.
    const std::string text = load("X") + transfer("X") + branch("PL", "external_label") + kUseA;
    CHECK(run(text) == expect(text));
  }
}
