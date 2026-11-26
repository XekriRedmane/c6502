#include "passes/dead_label_drop.h"

#include <utility>
#include <vector>

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;

void rewrite_function(ir::Function& fn) {
  const auto targets = dead_label_drop::collect_targets(fn.instructions);
  std::vector<ir::Type_instruction> out;
  out.reserve(fn.instructions.size());
  for (auto& instr : fn.instructions) {
    const auto* label = as<ir::Label>(instr);
    if (label != nullptr && !targets.contains(label->name)) {
      continue;
    }
    out.push_back(std::move(instr));
  }
  fn.instructions = std::move(out);
}

}  // namespace

namespace dead_label_drop {

std::unordered_set<std::string> collect_targets(Instrs instrs) {
  std::unordered_set<std::string> out;
  for (const auto& instr : instrs) {
    if (const auto* jump = as<ir::Jump>(instr)) {
      out.insert(jump->target);
    } else if (const auto* branch = as<ir::Branch>(instr)) {
      out.insert(branch->target);
    } else if (const auto* phi = as<ir::Phi>(instr)) {
      for (const auto& arg : phi->args) {
        out.insert(std::get<ir::AsmPhiArg>(arg).pred_label);
      }
    }
  }
  return out;
}

}  // namespace dead_label_drop

asm_ast::Program apply_dead_label_drop(asm_ast::Program prog) {
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      rewrite_function(*fn);
    }
  }
  return prog;
}

}  // namespace c6502::passes
