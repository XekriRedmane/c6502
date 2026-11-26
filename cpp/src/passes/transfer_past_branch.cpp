#include "passes/transfer_past_branch.h"

#include <utility>
#include <vector>

#include "passes/asm_liveness.h"
#include "passes/dead_a_arith.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

bool can_swap(Instrs instrs, std::size_t i, const LabelMap& labels) {
  if (i + 2 >= instrs.size()) {
    return false;
  }
  const ir::Type_instruction& op = instrs[i];
  const auto* mov = as<ir::Mov>(instrs[i + 1]);
  if (mov == nullptr || mov->is_volatile) {
    return false;
  }
  const auto* src = as<ir::Reg>(mov->src);
  if (src == nullptr || !isa<ir::X, ir::Y>(src->reg) || !is_reg_a(mov->dst)) {
    return false;
  }
  const auto* br = as<ir::Branch>(instrs[i + 2]);
  if (br == nullptr) {
    return false;
  }
  // TXA and TYA set N and Z from the value they copy, so the branch
  // sees the same flags whether it follows the transfer or [i].
  if (!dead_a_arith::writes_reg_setting_n_z(op, src->reg)) {
    return false;
  }
  const long target = labels.find(br->target);
  if (target < 0) {
    return false;
  }
  if (!a_dead_at(instrs, labels, static_cast<std::size_t>(target) + 1)) {
    return false;
  }
  return !a_dead_at(instrs, labels, i + 3);
}

void rewrite_function(ir::Function& fn) {
  const Instrs instrs = fn.instructions;
  const LabelMap labels(instrs);
  std::vector<ir::Type_instruction> out;
  bool changed = false;
  std::size_t i = 0;
  while (i < instrs.size()) {
    if (can_swap(instrs, i, labels)) {
      out.push_back(instrs[i]);
      out.push_back(instrs[i + 2]);
      out.push_back(instrs[i + 1]);
      i += 3;
      changed = true;
      continue;
    }
    out.push_back(instrs[i]);
    ++i;
  }
  if (changed) {
    fn.instructions = std::move(out);
  }
}

}  // namespace

asm_ast::Program apply_transfer_past_branch(asm_ast::Program prog) {
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      rewrite_function(*fn);
    }
  }
  return prog;
}

}  // namespace c6502::passes
