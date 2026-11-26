#include "passes/sub1_test_zero_peephole.h"

#include <optional>
#include <utility>
#include <vector>

#include "passes/asm_liveness.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

bool is_reg_a_op(const ir::Type_operand& op) {
  const auto* r = as<ir::Reg>(op);
  return r != nullptr && isa<ir::A>(r->reg);
}

bool is_imm(const ir::Type_operand& op, int value) {
  const auto* imm = as<ir::Imm>(op);
  return imm != nullptr && imm->value == value;
}

bool is_compareable_operand(const ir::Type_operand& op) {
  return isa<ir::Data, ir::ZP, ir::Pseudo>(op);
}

bool outputs_dead_at(Instrs instrs, const LabelMap& labels, std::size_t idx) {
  if (idx >= instrs.size()) {
    return true;
  }
  return a_dead_at(instrs, labels, idx) && flags_dead_at(instrs, labels, idx) &&
         c_dead_at(instrs, labels, idx);
}

std::optional<ir::Branch> try_match(Instrs instrs, const LabelMap& labels, std::size_t start) {
  if (start + 7 > instrs.size()) {
    return std::nullopt;
  }
  const auto* i0 = as<ir::Mov>(instrs[start]);
  if (i0 == nullptr || i0->is_volatile || !is_reg_a_op(i0->dst) ||
      !is_compareable_operand(i0->src)) {
    return std::nullopt;
  }
  const ir::Type_operand& m = i0->src;
  if (!isa<ir::SetCarry>(instrs[start + 1])) {
    return std::nullopt;
  }
  const auto* i2 = as<ir::Sub>(instrs[start + 2]);
  if (i2 == nullptr || !is_imm(i2->src, 1) || !is_reg_a_op(i2->dst)) {
    return std::nullopt;
  }
  const auto* i3 = as<ir::Mov>(instrs[start + 3]);
  if (i3 == nullptr || !is_reg_a_op(i3->src) || i3->dst == m) {
    return std::nullopt;
  }
  const auto* i4 = as<ir::Mov>(instrs[start + 4]);
  if (i4 == nullptr || !is_imm(i4->src, 0) || !is_reg_a_op(i4->dst)) {
    return std::nullopt;
  }
  const auto* i5 = as<ir::Compare>(instrs[start + 5]);
  if (i5 == nullptr || !is_reg_a_op(i5->left) || i5->right != m) {
    return std::nullopt;
  }
  // SBC #1 leaves C clear exactly when M is 0, and CMP M with A = 0
  // leaves C set exactly when M is 0, so a branch on C flips. The SBC
  // sets Z when M is 1, so a branch on Z can't be moved.
  const auto* i6 = as<ir::Branch>(instrs[start + 6]);
  if (i6 == nullptr) {
    return std::nullopt;
  }
  ir::Type_condition flipped;
  if (isa<ir::CC>(i6->cond)) {
    flipped = ir::CS{};
  } else if (isa<ir::CS>(i6->cond)) {
    flipped = ir::CC{};
  } else {
    return std::nullopt;
  }
  const long taken = labels.find(i6->target);
  if (taken < 0) {
    return std::nullopt;
  }
  if (!outputs_dead_at(instrs, labels, start + 7) ||
      !outputs_dead_at(instrs, labels, static_cast<std::size_t>(taken))) {
    return std::nullopt;
  }
  return ir::Branch{.cond = flipped, .target = i6->target};
}

void rewrite_function(ir::Function& fn) {
  const auto& instrs = fn.instructions;
  const LabelMap labels(instrs);
  std::vector<ir::Type_instruction> out;
  bool changed = false;
  std::size_t i = 0;
  while (i < instrs.size()) {
    std::optional<ir::Branch> branch = try_match(instrs, labels, i);
    if (!branch) {
      out.push_back(instrs[i]);
      ++i;
      continue;
    }
    for (std::size_t k = 0; k < 4; ++k) {
      out.push_back(instrs[i + k]);
    }
    out.push_back(std::move(*branch));
    changed = true;
    i += 7;
  }
  if (changed) {
    fn.instructions = std::move(out);
  }
}

}  // namespace

asm_ast::Program apply_sub1_test_zero_peephole(asm_ast::Program prog) {
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      rewrite_function(*fn);
    }
  }
  return prog;
}

}  // namespace c6502::passes
