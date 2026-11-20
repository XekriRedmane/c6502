// The tests of tests/test_volatile_void_read_cmp.py, ported.
#include <string>

#include "asm_text.h"
#include "doctest.h"
#include "passes/volatile_void_read_cmp.h"

namespace {

using c6502::asm_text::expect;
using c6502::asm_text::instructions;
using c6502::asm_text::program;

const std::string kLoad = "Mov(Indirect(0) Reg(A) true) ";

std::string run(const std::string& instrs) {
  return instructions(c6502::passes::apply_volatile_void_read_cmp(program(instrs)));
}

}  // namespace

TEST_CASE("volatile_void_read_cmp makes a discarded read a compare") {
  CHECK(run(kLoad + "Mov(Imm(1) Reg(A) false) Return(true false false)") ==
        expect("Compare(Reg(A) Indirect(0)) Mov(Imm(1) Reg(A) false) Return(true false false)"));
}

TEST_CASE("volatile_void_read_cmp keeps a read whose value or flags are read") {
  SUBCASE("an AND reads the byte") {
    // `while (!(*p & 0x80)) ;`
    const std::string text = R"(Label(".top") )" + kLoad +
                             R"(And(Imm(128) Reg(A)) Branch(EQ ".top") Return(false false false))";
    CHECK(run(text) == expect(text));
  }
  for (const char* reader : {"Compare(Reg(A) Imm(5))", "Add(Imm(1) Reg(A))",
                             "Or(Imm(1) Reg(A))"}) {
    CAPTURE(reader);
    const std::string text =
        "ClearCarry " + kLoad + reader + " Return(true false false)";
    CHECK(run(text) == expect(text));
  }
  SUBCASE("BPL reads the N flag the load set") {
    const std::string text = R"(Label(".top") )" + kLoad +
                             R"(Branch(PL ".top") Mov(Imm(1) Reg(A) false) Return(true false false))";
    CHECK(run(text) == expect(text));
  }
}
