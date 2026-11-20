// Drops instructions that no path reaches.
//
// An instruction right after a `Jump`, `Return`, or `Ret`, with no
// referenced `Label` in between, can't be reached: nothing falls into
// it, and no branch can target it. This pass removes every such
// instruction, along with any unreferenced label among them, up to the
// next referenced label.
//
// Removing a dead block can remove the only jump to a later label,
// which makes the code after that label dead too. The pass repeats
// until nothing changes, because the peephole fixed point would
// otherwise spend a round per label on such a chain, and a chain
// longer than its 16-round cap fails the compile (fuzz.stmts seeds
// 4411, 4593, and 4793, without -O).
//
// Copying a return tail ahead of its label (`carry_return.py`) is what
// leaves such code behind. When the last path that jumped to the label
// gets its own copy, `dead_label_drop` removes the label, and the
// original tail is left after a return with no label in front of it.
//
// It runs in the asm peephole fixed point, in both pipelines, since it
// only ever removes code.
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
