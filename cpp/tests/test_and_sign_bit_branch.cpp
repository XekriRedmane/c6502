// The tests of tests/test_and_sign_bit_branch.py, ported.
#include <set>
#include <string>
#include <vector>

#include "asm_text.h"
#include "doctest.h"
#include "passes/and_sign_bit_branch.h"

namespace {

namespace ir = c6502::asm_ast;
using c6502::asm_text::expect;
using c6502::asm_text::program;

// Runs the pass on `instrs` followed by a label and a return for each
// branch target that isn't a label of them, since the pass checks A at
// a branch's target and a target outside the function counts as live,
// and returns the result without that tail.
std::string run(const std::string& instrs) {
  ir::Program prog = program(instrs);
  auto& body = std::get<ir::Function>(prog.top_level[0]).instructions;
  std::set<std::string> labels;
  for (const auto& i : body) {
    if (const auto* l = c6502::asdl::as<ir::Label>(i)) {
      labels.insert(l->name);
    }
  }
  std::vector<ir::Type_instruction> tail;
  for (const auto& i : body) {
    if (const auto* b = c6502::asdl::as<ir::Branch>(i)) {
      if (labels.insert(b->target).second) {
        tail.push_back(ir::Label{.name = b->target});
        tail.push_back(ir::Return{});
      }
    }
  }
  body.insert(body.end(), tail.begin(), tail.end());
  prog = c6502::passes::apply_and_sign_bit_branch(std::move(prog));
  auto out = std::get<ir::Function>(prog.top_level[0]).instructions;
  out.resize(out.size() - tail.size());
  return c6502::asm_text::dump(out);
}

const std::string kRet = "Return(false false false) ";
const std::string kLdaX = R"(Mov(Data("x" 0) Reg(A) false) )";

std::string branch(const char* cond, const char* target) {
  return std::string("Branch(") + cond + " \"" + target + "\") ";
}

}  // namespace

TEST_CASE("and_sign_bit_branch folds LDA; AND #$80; BEQ/BNE") {
  SUBCASE("BEQ becomes BPL") {
    CHECK(run(kLdaX + "And(Imm(128) Reg(A)) " + branch("EQ", ".target") + kRet) ==
          expect(kLdaX + branch("PL", ".target") + kRet));
  }
  SUBCASE("BNE becomes BMI") {
    CHECK(run(kLdaX + "And(Imm(128) Reg(A)) " + branch("NE", ".target") + kRet) ==
          expect(kLdaX + branch("MI", ".target") + kRet));
  }
  SUBCASE("kept when A is read after the branch") {
    // The STA reads the AND's value.
    const std::string text = kLdaX + "And(Imm(128) Reg(A)) " + branch("EQ", ".target") +
                             R"(Mov(Reg(A) Data("y" 0) false) )" + kRet;
    CHECK(run(text) == expect(text));
  }
  SUBCASE("another mask") {
    // Only bits 7, 6, and 0 map onto a flag.
    const std::string text = kLdaX + "And(Imm(32) Reg(A)) " + branch("EQ", ".target") + kRet;
    CHECK(run(text) == expect(text));
  }
  SUBCASE("no branch after the AND") {
    const std::string text =
        kLdaX + R"(And(Imm(128) Reg(A)) Mov(Reg(A) Data("y" 0) false) )" + kRet;
    CHECK(run(text) == expect(text));
  }
}

TEST_CASE("and_sign_bit_branch folds the form with a store between the load and the AND") {
  // The form with a STA between the LDA and the AND arises after
  // split_mem_to_mem lowers `Mov(M, dst); AND #80; Branch` into `Mov(M,
  // A); Mov(A, dst); AND #80; Branch`.
  SUBCASE("BNE") {
    const std::string head = kLdaX + R"(Mov(Reg(A) Data("dst" 0) false) )";
    CHECK(run(head + "And(Imm(128) Reg(A)) " + branch("NE", ".target") + kRet) ==
          expect(head + branch("MI", ".target") + kRet));
  }
  SUBCASE("BEQ") {
    const std::string head = kLdaX + "Mov(Reg(A) ZP(133 0) false) ";
    CHECK(run(head + "And(Imm(128) Reg(A)) " + branch("EQ", ".target") + kRet) ==
          expect(head + branch("PL", ".target") + kRet));
  }
  SUBCASE("kept when A is read after the branch") {
    const std::string text = kLdaX + R"(Mov(Reg(A) Data("dst" 0) false) )" +
                             "And(Imm(128) Reg(A)) " + branch("NE", ".target") +
                             R"(Mov(Reg(A) Data("y" 0) false) )" + kRet;
    CHECK(run(text) == expect(text));
  }
}

TEST_CASE("and_sign_bit_branch checks the branch's target and the load") {
  // The AND goes on both paths, so A has to be dead at the branch's
  // target as well as on the fall-through, and the first instruction
  // has to load A.
  SUBCASE("A read at the target") {
    // `t = flags & 0x80; if (t) {...} return t;` reads t in A at the
    // branch's target.
    const std::string text = R"(Mov(Data("flags" 0) Reg(A) false) And(Imm(128) Reg(A)) )" +
                             branch("EQ", ".t") +
                             R"(Mov(Imm(7) Reg(A) false) Return(true false false) )" +
                             R"(Label(".t") Return(true false false))";
    CHECK(run(text) == expect(text));
  }
  SUBCASE("a load into X") {
    // `LDX M` leaves A alone, so the AND tests whatever A held.
    const std::string text =
        R"(Mov(Data("m" 0) Reg(X) false) And(Imm(128) Reg(A)) )" + branch("NE", ".t") + kRet;
    CHECK(run(text) == expect(text));
  }
}

TEST_CASE("and_sign_bit_branch tests the low bit through the carry") {
  // `AND #$01; BEQ` as `LSR; BCC` when A, C, N, and Z are dead on both
  // paths, as in Zork's `store_with_sign`.
  auto and1 = [](const char* cond, const std::string& after) {
    return kLdaX + "And(Imm(1) Reg(A)) " + branch(cond, ".target") + after + kRet;
  };
  SUBCASE("BEQ becomes LSR; BCC") {
    CHECK(run(and1("EQ", "")) ==
          expect(kLdaX + "LogicalShiftRight(Reg(A)) " + branch("CC", ".target") + kRet));
  }
  SUBCASE("BNE becomes LSR; BCS") {
    CHECK(run(and1("NE", "")) ==
          expect(kLdaX + "LogicalShiftRight(Reg(A)) " + branch("CS", ".target") + kRet));
  }
  SUBCASE("kept when C is read after") {
    // The ADC reads the C that the AND left alone.
    const std::string text = and1("EQ", R"(Add(Imm(1) Reg(A)) Mov(Reg(A) Data("y" 0) false) )");
    CHECK(run(text) == expect(text));
  }
  SUBCASE("kept when A is read after") {
    const std::string text = and1("EQ", R"(Mov(Reg(A) Data("y" 0) false) )");
    CHECK(run(text) == expect(text));
  }
}

TEST_CASE("and_sign_bit_branch tests bit 6 with BIT") {
  // `LDA m; AND #$40; BEQ` becomes `BIT m; BVC`, as in Zork's `branch`.
  const std::string m = R"(Data("__local_f__b" 0))";
  const std::string lda_m = "Mov(" + m + " Reg(A) false) ";
  auto test = [&](const char* cond, const std::string& after) {
    return lda_m + "And(Imm(64) Reg(A)) " + branch(cond, ".t") + after + kRet;
  };
  SUBCASE("BEQ becomes BIT; BVC") {
    CHECK(run(test("EQ", lda_m)) ==
          expect("BitTest(" + m + ") " + branch("VC", ".t") + lda_m + kRet));
  }
  SUBCASE("BNE becomes BVS") {
    CHECK(run(test("NE", lda_m)) ==
          expect("BitTest(" + m + ") " + branch("VS", ".t") + lda_m + kRet));
  }
  SUBCASE("kept when A is read after") {
    const std::string text = test("EQ", R"(Mov(Reg(A) Data("g" 0) false) )");
    CHECK(run(text) == expect(text));
  }
  SUBCASE("kept when V is read after") {
    const std::string text = test("EQ", branch("VS", ".u") + lda_m);
    CHECK(run(text) == expect(text));
  }
}

TEST_CASE("and_sign_bit_branch uses the N that an earlier write of A set") {
  // `AND #$80; BEQ` after any instruction that set N from A, with
  // stores between, is `BPL`, as for `a = x - 1; if (a & 0x80)`.
  auto shape = [](const std::string& first, const std::string& after) {
    return first + R"(Mov(Reg(A) Data("z" 0) false) And(Imm(128) Reg(A)) )" +
           branch("EQ", "L") + after + kRet;
  };
  SUBCASE("after a subtract and a store") {
    const std::string sub = "Sub(Imm(1) Reg(A)) ";
    CHECK(run(shape(sub, "")) ==
          expect(sub + R"(Mov(Reg(A) Data("z" 0) false) )" + branch("PL", "L") + kRet));
  }
  SUBCASE("kept after a call") {
    // A call's flags aren't A's.
    const std::string text = shape(R"(Call("g" [] []) )", "");
    CHECK(run(text) == expect(text));
  }
  SUBCASE("kept when Z is read after") {
    // Without the AND, Z comes from all of A.
    const std::string text = shape("Sub(Imm(1) Reg(A)) ", branch("NE", "M") + "Label(\"M\") ");
    CHECK(run(text) == expect(text));
  }
}
