// The tests of tests/test_redundant_compare.py, ported.
#include <string>
#include <vector>

#include "asm_text.h"
#include "doctest.h"
#include "passes/redundant_compare.h"

namespace {

using c6502::asm_text::expect;
using c6502::asm_text::instructions;
using c6502::asm_text::program;

std::string run(const std::string& instrs) {
  return instructions(
      c6502::passes::apply_redundant_compare(program(instrs), c6502::passes::AsmContext{}));
}

const std::string kCmp80 = "Compare(Reg(A) ZP(128 0)) ";
const std::string kCmp81 = "Compare(Reg(A) ZP(129 0)) ";
const std::string kRet = "Return(false false false) ";
const std::string kLda = "Mov(ZP(128 0) Reg(A) false) ";
const std::string kZero = "Compare(Reg(A) Imm(0)) ";

std::string label(const char* name) { return std::string("Label(\"") + name + "\") "; }
std::string branch(const char* cond, const char* target) {
  return std::string("Branch(") + cond + " \"" + target + "\") ";
}

}  // namespace

TEST_CASE("redundant_compare drops a compare an identical one already made") {
  SUBCASE("the only branch into the label follows the first compare") {
    // CMP M; BCS L; RTS; L: CMP M; BNE ..., so the second CMP goes.
    const std::string head = kCmp80 + branch("CS", "L") + kRet + label("L");
    const std::string tail = branch("NE", "M") + kRet + label("M") + kRet;
    CHECK(run(head + kCmp80 + tail) == expect(head + tail));
  }
  SUBCASE("falling through branches after the first compare") {
    // CMP M; BCC L1; BNE L2; CMP M. Falling through the branches
    // changes nothing, so the second CMP goes.
    const std::string head = kCmp80 + branch("CC", "L1") + branch("NE", "L2");
    const std::string tail =
        branch("EQ", "L1") + kRet + label("L1") + kRet + label("L2") + kRet;
    CHECK(run(head + kCmp80 + tail) == expect(head + tail));
  }
  SUBCASE("the label is reached by the second of two branches") {
    // CMP M; BCC L1; BCS L; ... L: CMP M. The second CMP goes, since
    // the run of branches after the first compare reaches L.
    const std::string head =
        kCmp80 + branch("CC", "L1") + branch("CS", "L") + kRet + label("L");
    const std::string tail = branch("NE", "L1") + kRet + label("L1") + kRet;
    CHECK(run(head + kCmp80 + tail) == expect(head + tail));
  }
}

TEST_CASE("redundant_compare keeps a compare with another way in") {
  SUBCASE("after the branch of another compare") {
    const std::string text = kCmp80 + branch("CC", "L1") + kCmp81 + kRet + label("L1") + kRet;
    CHECK(run(text) == expect(text));
  }
  SUBCASE("a fall-through into the label") {
    // The label is also reached by falling through, where the flags may
    // differ.
    const std::string text = kCmp80 + branch("CS", "L") + "Mov(Imm(1) Reg(A) false) " +
                             label("L") + kCmp80 + kRet;
    CHECK(run(text) == expect(text));
  }
  SUBCASE("a second branch to the label") {
    const std::string text = kCmp80 + branch("CS", "L") + kCmp81 + branch("EQ", "L") + kRet +
                             label("L") + kCmp80 + kRet;
    CHECK(run(text) == expect(text));
  }
  SUBCASE("another operand") {
    const std::string text = kCmp80 + branch("CS", "L") + kRet + label("L") + kCmp81 + kRet;
    CHECK(run(text) == expect(text));
  }
}

TEST_CASE("redundant_compare drops a compare with zero after the register's write") {
  // `CMP #$00` after the instruction that set N and Z from the same
  // register, when C is dead.
  SUBCASE("after a load") {
    const std::string tail = branch("NE", "L") + kRet + label("L") + kRet;
    CHECK(run(kLda + kZero + tail) == expect(kLda + tail));
  }
  SUBCASE("with a store between") {
    const std::string sta = "Mov(Reg(A) ZP(129 0) false) ";
    const std::string tail = branch("NE", "L") + kRet + label("L") + kRet;
    CHECK(run(kLda + sta + kZero + tail) == expect(kLda + sta + tail));
  }
  SUBCASE("kept when C is read") {
    const std::string text = kLda + kZero + branch("CS", "L") + kRet + label("L") + kRet;
    CHECK(run(text) == expect(text));
  }
  SUBCASE("kept after a store with a Y setup") {
    // `STA (FP),Y` loads Y first, which sets N and Z from Y.
    const std::string text = kLda + "Mov(Reg(A) Frame(3) false) " + kZero + branch("NE", "L") +
                             kRet + label("L") + kRet;
    CHECK(run(text) == expect(text));
  }
  SUBCASE("kept after a label") {
    const std::string text = kLda + label("M") + kZero + branch("NE", "L") + kRet + label("L") +
                             "Jump(\"M\" []) ";
    CHECK(run(text) == expect(text));
  }
}

TEST_CASE("redundant_compare drops the test of a join whose every way in set the flags") {
  // An `ORA #$00` after a label, where every way in set N and Z from A,
  // as `equal = a == b; if (!equal) ...` leaves it: `LDA #$00; BEQ end;
  // LDA #$01; end: STA equal; ORA #$00`.
  const std::string ora0 = "Or(Imm(0) Reg(A)) ";
  auto join = [&](const std::string& first, bool with_ora) {
    return first + branch("EQ", "end") + "Mov(Imm(1) Reg(A) false) " + label("end") +
           "Mov(Reg(A) ZP(129 0) false) " + (with_ora ? ora0 : "") + branch("NE", "L") + kRet +
           label("L") + kRet;
  };
  SUBCASE("every way in set the flags") {
    const std::string first = "Mov(Imm(0) Reg(A) false) ";
    CHECK(run(join(first, true)) == expect(join(first, false)));
  }
  SUBCASE("kept when a way in set them otherwise") {
    // The branch to `end` follows a call, whose flags aren't A's.
    const std::string text = join("Call(\"g\" [] []) ", true);
    CHECK(run(text) == expect(text));
  }
  SUBCASE("kept at a jump table's target") {
    const std::string text = kLda + "JumpTable(\"t\" \"Y\" 0 [\"end\"]) " + label("end") + ora0 +
                             branch("NE", "L") + kRet + label("L") + kRet;
    CHECK(run(text) == expect(text));
  }
}

TEST_CASE("redundant_compare takes the carry of an add for a compare with its operand") {
  // `CMP m` after `LDA m; CLC; ADC x` sets C opposite to the add's
  // carry, so a branch on C takes the opposite condition without it.
  // That's `if (new_lo < lo)` after `new_lo = lo + offset` in Zork's
  // `branch_to_offset`.
  auto instrs = [](const std::string& store, const std::string& tail, bool folded) {
    std::string text = "Mov(ZP(138 0) Reg(A) false) ClearCarry Add(ZP(145 0) Reg(A)) Mov(Reg(A) " +
                       store + " false) ";
    text += folded ? branch("CC", ".l") : "Compare(Reg(A) ZP(138 0)) " + branch("CS", ".l");
    return text + "Inc(ZP(146 0)) " + label(".l") + tail + kRet;
  };
  SUBCASE("the compare goes and the branch flips") {
    CHECK(run(instrs("ZP(144 0)", "", false)) == expect(instrs("ZP(144 0)", "", true)));
  }
  SUBCASE("kept when the store writes m") {
    const std::string text = instrs("ZP(138 0)", "", false);
    CHECK(run(text) == expect(text));
  }
  SUBCASE("kept when Z is read after") {
    const std::string text = instrs("ZP(144 0)", branch("EQ", ".m") + label(".m"), false);
    CHECK(run(text) == expect(text));
  }
}

TEST_CASE("redundant_compare's label_refs counts the branches, jumps, and label operands") {
  namespace ir = c6502::asm_ast;
  const std::vector<ir::Type_instruction> instrs = {
      ir::Branch{.cond = ir::NE{}, .target = ".a"},
      ir::Jump{.target = ".a", .reg_args = {}},
      ir::Mov{.src = ir::ImmLabelLow{.name = ".b", .offset = 0},
              .dst = ir::Reg{.reg = ir::A{}},
              .is_volatile = false},
      ir::DecimalArith{.subtract = false,
                       .left = {ir::ZP{.address = 128, .offset = 0}},
                       .right = {ir::ImmLabelHigh{.name = ".c", .offset = 0}},
                       .dst = {ir::ZP{.address = 130, .offset = 0}}},
      ir::Label{.name = ".a"},
  };
  const auto refs = c6502::passes::redundant_compare::label_refs(instrs);
  REQUIRE(refs.get(".a") != nullptr);
  CHECK(*refs.get(".a") == std::vector<long>{0, 1});
  REQUIRE(refs.get(".b") != nullptr);
  CHECK(*refs.get(".b") == std::vector<long>{-1});
  REQUIRE(refs.get(".c") != nullptr);
  CHECK(*refs.get(".c") == std::vector<long>{-1});
  CHECK(refs.get(".d") == nullptr);
  CHECK(refs.table_targets.empty());
}
