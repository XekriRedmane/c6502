// The tests of tests/test_join_store.py, ported.
#include <string>

#include "asm_text.h"
#include "doctest.h"
#include "passes/join_store.h"

namespace {

using c6502::asm_text::expect;
using c6502::asm_text::instructions;
using c6502::asm_text::program;

std::string run(const std::string& instrs) {
  return instructions(
      c6502::passes::apply_join_store(program(instrs), c6502::passes::AsmContext{}));
}

const std::string kT = R"(Data("__local_f__t" 0))";
const std::string kM = R"(Data("m" 1))";
const std::string kLo = R"(Data("m" 0))";

// `f`'s two arms, one storing X into `t` (or `x_dst`) and one a
// constant (into `k_dst`), with `x_tail` between the first arm's store
// and its exit `x_exit`, and the join, which runs `join` and returns.
std::string diamond(const std::string& join, const std::string& x_dst = kT,
                    const std::string& k_dst = kT, const std::string& x_tail = "",
                    const std::string& x_exit = R"(Jump(".end" []) )") {
  return R"(Mov(Data("c" 0) Reg(A) false) Branch(EQ ".else") Call("g" [] []) )"
         "Mov(Reg(X) " + x_dst + " false) " + x_tail + x_exit +
         R"(Label(".else") Call("h" [] []) )"
         "Mov(Imm(0) " + k_dst + " false) " + R"(Label(".end") )" + join +
         "Return(false false false)";
}

const std::string kJoin = "Mov(Reg(A) " + kLo + " false) Mov(" + kT + " Reg(A) false) Mov(Reg(A) " +
                          kM + " false) ";

}  // namespace

TEST_CASE("join_store moves the join's copy into the arms' stores") {
  CHECK(run(diamond(kJoin)) == expect(diamond("Mov(Reg(A) " + kLo + " false) ", kM, kM)));
}

TEST_CASE("join_store leaves the join alone") {
  SUBCASE("the join reads M first") {
    // The join reads M before it copies t there, so M must keep its old
    // value until then.
    const std::string text = diamond("Compare(Reg(A) " + kM + ") " + kJoin);
    CHECK(run(text) == expect(text));
  }
  SUBCASE("t is read later") {
    const std::string text = diamond(kJoin + "Mov(" + kT + " " + kLo + " false) ");
    CHECK(run(text) == expect(text));
  }
  SUBCASE("A is read later") {
    // A holds t after the copy, and the next store reads it.
    const std::string text = diamond(kJoin + R"(Mov(Reg(A) Data("n" 0) false) )");
    CHECK(run(text) == expect(text));
  }
  SUBCASE("a branch into the join") {
    // A branch's store to t would reach its other path too.
    const std::string text = diamond(kJoin, kT, kT, "", R"(Branch(NE ".end") )");
    CHECK(run(text) == expect(text));
  }
  SUBCASE("a call after an arm's store") {
    // The call might read M, which would now be written before it.
    const std::string text = diamond(kJoin, kT, kT, R"(Call("k" [] []) )");
    CHECK(run(text) == expect(text));
  }
}
