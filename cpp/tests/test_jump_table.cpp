// The bounds of `apply_jump_tables`: where a table starts, and the
// largest case value it takes.
#include <string>

#include "asm_text.h"
#include "doctest.h"
#include "passes/jump_table.h"

namespace {

using c6502::asm_text::instructions;
using c6502::asm_text::program;

// A switch on `n` of nine cases, `first` to `first + 8`, each storing
// its value, and a default that returns.
std::string switch_of(int first) {
  std::string chain = R"(Mov(Data("n" 0) Reg(A) false) )";
  std::string cases;
  for (int k = first; k < first + 9; ++k) {
    const std::string v = std::to_string(k);
    chain += "Compare(Reg(A) Imm(" + v + ")) Branch(EQ \".c" + v + "\") ";
    cases += "Label(\".c" + v + "\") Mov(Imm(" + v + R"() Data("n" 0) false) )" +
             "Return(false false false) ";
  }
  return chain + R"(Jump(".def" []) )" + cases + R"(Label(".def") Return(false false false))";
}

std::string run(int first) {
  return instructions(c6502::passes::apply_jump_tables(program(switch_of(first))));
}

}  // namespace

TEST_CASE("jump_tables starts a table at 0 when that costs no more than a lower bound check") {
  const std::string out = run(2);
  CHECK(out.find("Compare(Reg(A) Imm(2))") == std::string::npos);
  CHECK(out.find(R"(JumpTable(".jtab@0" "Y" 0 [".def" ".def" ".c2")") != std::string::npos);
}

TEST_CASE("jump_tables starts a table at its first case past that") {
  const std::string out = run(3);
  CHECK(out.find("Compare(Reg(A) Imm(3))") != std::string::npos);
  CHECK(out.find(R"(Branch(CC ".def"))") != std::string::npos);
  CHECK(out.find(R"(JumpTable(".jtab@0" "Y" 3 [".c3")") != std::string::npos);
}

TEST_CASE("jump_tables takes a case value of 127 and none above") {
  CHECK(run(119).find(R"(".c127"])") != std::string::npos);
  CHECK(run(128).find("JumpTable") == std::string::npos);
}
