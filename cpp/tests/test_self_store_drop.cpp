// The tests of tests/test_self_store_drop.py, ported.
#include "asm_text.h"
#include "doctest.h"
#include "passes/self_store_drop.h"

namespace {

using c6502::asm_text::expect;
using c6502::asm_text::instructions;
using c6502::asm_text::program;

std::string run(std::string_view instrs) {
  return instructions(c6502::passes::apply_self_store_drop(program(instrs)));
}

}  // namespace

TEST_CASE("self_store_drop drops the STA of LDA M; STA M") {
  SUBCASE("a Data byte") {
    CHECK(run(R"(Mov(Data("M" 0) Reg(A) false) Mov(Reg(A) Data("M" 0) false))") ==
          expect(R"(Mov(Data("M" 0) Reg(A) false))"));
  }
  SUBCASE("a ZP byte") {
    CHECK(run("Mov(ZP(144 0) Reg(A) false) Mov(Reg(A) ZP(144 0) false)") ==
          expect("Mov(ZP(144 0) Reg(A) false)"));
  }
  SUBCASE("a move of a byte to itself") {
    CHECK(run(R"(Mov(Data("M" 0) Data("M" 0) false) Return(false false false))") ==
          expect("Return(false false false)"));
  }
}

TEST_CASE("self_store_drop keeps stores to other bytes") {
  SUBCASE("another name") {
    const char* text = R"(Mov(Data("M" 0) Reg(A) false) Mov(Reg(A) Data("N" 0) false))";
    CHECK(run(text) == expect(text));
  }
  SUBCASE("another offset") {
    // M[0] and M[1] are different bytes, so the STA isn't a self-store.
    const char* text = R"(Mov(Data("M" 0) Reg(A) false) Mov(Reg(A) Data("M" 1) false))";
    CHECK(run(text) == expect(text));
  }
  SUBCASE("a volatile load") {
    const char* text = R"(Mov(Data("M" 0) Reg(A) true) Mov(Reg(A) Data("M" 0) false))";
    CHECK(run(text) == expect(text));
  }
}
