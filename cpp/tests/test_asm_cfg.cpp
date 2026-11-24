// tests/test_asm_cfg.py's tests of the asm CFG and its dominators,
// ported by hand, one case per Python test with each of its assertions.
// The Python CFG's `blocks` is a dict by id and C++'s a vector indexed
// by id, and `immediate_dominators` returns the `idom` dict as
// `Dominators::idom`.
#include <algorithm>
#include <set>
#include <variant>
#include <vector>

#include "asm_ast.h"
#include "asdl/runtime.h"
#include "doctest.h"
#include "passes/optimization_asm/cfg.h"

namespace ir = c6502::asm_ast;
using c6502::passes::build_cfg;
using c6502::passes::Cfg;
using c6502::passes::cfg_to_function;
using c6502::passes::dominance_frontiers;
using c6502::passes::immediate_dominators;
using c6502::passes::kEntryId;
using c6502::passes::kExitId;
using Ids = std::vector<int>;

namespace {

ir::Type_operand imm(int v) { return ir::Imm{.value = v}; }
ir::Type_operand ps(const char* name) { return ir::Pseudo{.name = name}; }
const ir::Type_operand kA = ir::Reg{.reg = ir::A{}};

ir::Type_instruction mov(ir::Type_operand src, ir::Type_operand dst) {
  return ir::Mov{.src = src, .dst = dst};
}
ir::Type_instruction ret_bare() { return ir::Return{.save_a = true}; }
ir::Type_instruction ret_full() { return ir::Ret{.save_a = true}; }
ir::Type_instruction label(const char* name) { return ir::Label{.name = name}; }
ir::Type_instruction jump(const char* target) { return ir::Jump{.target = target}; }
// The condition doesn't change the CFG's shape.
ir::Type_instruction branch(const char* target) {
  return ir::Branch{.cond = ir::EQ{}, .target = target};
}

ir::Function fn(std::vector<ir::Type_instruction> instrs) {
  return ir::Function{.name = "main", .is_global = true, .instructions = std::move(instrs)};
}

}  // namespace

TEST_CASE("asm_cfg: TestBuildAsmCFG::test_empty_function_entry_to_exit") {
  const ir::Function f = fn({});
  const Cfg cfg = build_cfg(f);
  CHECK(cfg.block_order.empty());
  CHECK(cfg.blocks[kEntryId].successors == Ids{kExitId});
  CHECK(cfg.blocks[kExitId].predecessors == Ids{kEntryId});
}

TEST_CASE("asm_cfg: TestBuildAsmCFG::test_single_block_bare_return") {
  const ir::Function f = fn({mov(imm(1), ps("x")), ret_bare()});
  const Cfg cfg = build_cfg(f);
  REQUIRE(cfg.block_order.size() == 1);
  const int bid = cfg.block_order[0];
  CHECK(cfg.blocks[kEntryId].successors == Ids{bid});
  CHECK(cfg.blocks[bid].successors == Ids{kExitId});
  CHECK(cfg.blocks[kExitId].predecessors == Ids{bid});
}

TEST_CASE("asm_cfg: TestBuildAsmCFG::test_single_block_legacy_ret") {
  // The compound `Ret` ends a block too.
  const ir::Function f = fn({mov(imm(1), ps("x")), ret_full()});
  const Cfg cfg = build_cfg(f);
  REQUIRE(cfg.block_order.size() == 1);
  CHECK(cfg.blocks[cfg.block_order[0]].successors == Ids{kExitId});
}

TEST_CASE("asm_cfg: TestBuildAsmCFG::test_label_splits_block") {
  // B0 = [Mov] falls through to B1 = [Label, Return].
  const ir::Function f = fn({mov(imm(1), ps("x")), label("L"), ret_bare()});
  const Cfg cfg = build_cfg(f);
  REQUIRE(cfg.block_order.size() == 2);
  const int b0 = cfg.block_order[0];
  const int b1 = cfg.block_order[1];
  CHECK(cfg.blocks[b0].successors == Ids{b1});
  CHECK(cfg.blocks[b1].predecessors == Ids{b0});
  CHECK(cfg.blocks[b1].successors == Ids{kExitId});
  CHECK(c6502::asdl::isa<ir::Label>(cfg.instructions(b1)[0]));
}

TEST_CASE("asm_cfg: TestBuildAsmCFG::test_jump_to_forward_label") {
  // B0 = [Mov, Jump L], B1 = [Mov, Return] (unreachable), and
  // B2 = [Label L, Return].
  const ir::Function f = fn({mov(imm(1), ps("x")), jump("L"), mov(imm(2), ps("x")),
                             ret_bare(), label("L"), ret_bare()});
  const Cfg cfg = build_cfg(f);
  REQUIRE(cfg.block_order.size() == 3);
  const int b0 = cfg.block_order[0];
  const int b1 = cfg.block_order[1];
  const int b2 = cfg.block_order[2];
  CHECK(cfg.blocks[b0].successors == Ids{b2});
  // B1's Return goes to EXIT, so B2's only predecessor is B0.
  CHECK(cfg.blocks[b1].successors == Ids{kExitId});
  CHECK(cfg.blocks[b2].predecessors == Ids{b0});
}

TEST_CASE("asm_cfg: TestBuildAsmCFG::test_branch_two_successors") {
  // A branch's successors are its target, then the fall-through.
  const ir::Function f = fn({mov(imm(1), ps("x")), branch("L"), mov(imm(2), ps("x")),
                             ret_bare(), label("L"), ret_bare()});
  const Cfg cfg = build_cfg(f);
  REQUIRE(cfg.block_order.size() == 3);
  const int b0 = cfg.block_order[0];
  const int b1 = cfg.block_order[1];
  const int b2 = cfg.block_order[2];
  CHECK(cfg.blocks[b0].successors == Ids{b2, b1});
  CHECK(cfg.blocks[b1].predecessors == Ids{b0});
  CHECK(cfg.blocks[b2].predecessors == Ids{b0});
}

TEST_CASE("asm_cfg: TestBuildAsmCFG::test_call_does_not_split_block") {
  // A call returns to the next instruction, and AllocateStack doesn't
  // leave the block either.
  const ir::Function f = fn({ir::AllocateStack{.bytes = 2}, ir::Call{.name = "foo"},
                             mov(kA, ps("y")), ret_bare()});
  const Cfg cfg = build_cfg(f);
  REQUIRE(cfg.block_order.size() == 1);
  const int bid = cfg.block_order[0];
  CHECK(cfg.instructions(bid).size() == 4);
  CHECK(cfg.blocks[bid].successors == Ids{kExitId});
}

TEST_CASE("asm_cfg: TestBuildAsmCFG::test_loop_back_edge") {
  // B0 = [Label, Mov, Branch] and B1 = [Return].
  const ir::Function f = fn({label("L"), mov(imm(1), ps("x")), branch("L"), ret_bare()});
  const Cfg cfg = build_cfg(f);
  REQUIRE(cfg.block_order.size() == 2);
  const int b0 = cfg.block_order[0];
  const int b1 = cfg.block_order[1];
  CHECK(cfg.blocks[b0].successors == Ids{b0, b1});
  const std::set<int> preds(cfg.blocks[b0].predecessors.begin(),
                            cfg.blocks[b0].predecessors.end());
  CHECK(preds == std::set<int>{kEntryId, b0});
}

TEST_CASE("asm_cfg: TestBuildAsmCFG::test_no_terminator_falls_through_to_exit") {
  const ir::Function f = fn({mov(imm(1), ps("x"))});
  const Cfg cfg = build_cfg(f);
  REQUIRE(cfg.block_order.size() == 1);
  CHECK(cfg.blocks[cfg.block_order[0]].successors == Ids{kExitId});
}

TEST_CASE("asm_cfg: TestBuildAsmCFG::test_cfg_to_function_round_trips") {
  const std::vector<ir::Type_instruction> instrs = {
      mov(imm(1), ps("x")), branch("L"), mov(imm(2), ps("x")),
      ret_bare(),           label("L"),  ret_bare()};
  const ir::Function f = fn(instrs);
  const ir::Function out = cfg_to_function(f, build_cfg(f));
  CHECK(out.name == "main");
  CHECK(out.instructions == instrs);
}

TEST_CASE("asm_cfg: TestAsmDominance::test_diamond_dominance_frontiers") {
  // B0 branches to L1 or falls through, and both arms jump to L3.
  const ir::Function f = fn({mov(imm(1), ps("x")), branch("L1"), mov(imm(2), ps("x")),
                             jump("L3"), label("L1"), mov(imm(3), ps("x")), jump("L3"),
                             label("L3"), ret_bare()});
  const Cfg cfg = build_cfg(f);
  const auto doms = immediate_dominators(cfg);
  const int b0 = cfg.block_order[0];
  int b3 = -1;
  for (const int bid : cfg.block_order) {
    const auto* l = std::get_if<ir::Label>(&cfg.instructions(bid)[0]);
    if (l != nullptr && l->name == "L3") {
      b3 = bid;
      break;
    }
  }
  REQUIRE(b3 != -1);
  CHECK(doms.idom[b3] == b0);
  // Both arms, the blocks other than B0 that go to B3, have B3 as their
  // dominance frontier.
  const auto df = dominance_frontiers(cfg);
  Ids arms;
  for (const int bid : cfg.block_order) {
    const auto& succ = cfg.blocks[bid].successors;
    if (bid != b0 && std::find(succ.begin(), succ.end(), b3) != succ.end()) {
      arms.push_back(bid);
    }
  }
  REQUIRE(arms.size() == 2);
  for (const int arm : arms) {
    CHECK(df[arm] == std::set<int>{b3});
  }
}
