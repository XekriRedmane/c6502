// Redundant load elimination. The pass itself isn't ported yet, only
// the CFG builder that `memory_value_propagation` takes from it.
#include "passes/redundant_load.h"

#include <set>
#include <string>
#include <utility>
#include <vector>

namespace c6502::passes {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;

Cfg build_cfg_tolerant(const ir::Function& fn) {
  std::set<std::string> labels;
  for (const auto& instr : fn.instructions) {
    if (const auto* label = as<ir::Label>(instr)) {
      labels.insert(label->name);
    }
  }
  std::vector<std::string> unresolved;
  std::set<std::string> seen;
  for (const auto& instr : fn.instructions) {
    const std::string* target = nullptr;
    if (const auto* branch = as<ir::Branch>(instr)) {
      target = &branch->target;
    } else if (const auto* jump = as<ir::Jump>(instr)) {
      target = &jump->target;
    }
    if (target != nullptr && labels.count(*target) == 0 &&
        seen.insert(*target).second) {
      unresolved.push_back(*target);
    }
  }
  std::vector<ir::Type_instruction> extra;
  for (const auto& target : unresolved) {
    extra.push_back(ir::Label{.name = target});
    extra.push_back(ir::Return{.save_a = false});
  }
  return build_cfg(fn.instructions, std::move(extra));
}

}  // namespace c6502::passes
