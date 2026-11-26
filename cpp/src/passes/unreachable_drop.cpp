#include "passes/unreachable_drop.h"

#include <utility>
#include <vector>

#include "passes/dead_label_drop.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

void rewrite_function(ir::Function& fn) {
  std::vector<ir::Type_instruction>& instrs = fn.instructions;
  while (true) {
    const auto targets = dead_label_drop::collect_targets(instrs);
    std::vector<ir::Type_instruction> out;
    out.reserve(instrs.size());
    bool dead = false;
    for (auto& instr : instrs) {
      const auto* label = as<ir::Label>(instr);
      if (label != nullptr && targets.contains(label->name)) {
        dead = false;
      } else if (dead) {
        continue;
      }
      const bool terminator = isa<ir::Jump, ir::Return, ir::Ret>(instr);
      out.push_back(std::move(instr));
      if (terminator) {
        dead = true;
      }
    }
    const bool same = out.size() == instrs.size();
    instrs = std::move(out);
    if (same) {
      break;
    }
  }
}

}  // namespace

asm_ast::Program apply_unreachable_drop(asm_ast::Program prog) {
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      rewrite_function(*fn);
    }
  }
  return prog;
}

}  // namespace c6502::passes
