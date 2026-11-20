// Drops labels that no `Jump`, `Branch`, or `Phi` names.
//
// Motivating shape
//
// Several earlier passes leave `Label`s behind whose names no
// control-flow target names any more. The asm SSA construction emits a
// `.<fn>@asm_ssa_block@N` marker for each block, the parallel-copy
// edges of SSA destruction sometimes have no other target, and
// `apply_branch_invert` drops the `Jump` of its pattern but keeps its
// former target's `Label`, which may now have no reference.
//
// Such labels are noise in the emitted assembly, and they also block
// other rewrites. `apply_branch_invert` matches a `Branch`, a `Jump`,
// and a `Label` in a row, so a label with no reference between the
// `Branch` and the `Jump` keeps it from firing. Dropping those labels
// first exposes the pattern.
//
// Algorithm
//
// For each function, the pass collects every target name (each
// `Jump.target`, `Branch.target`, and `Phi` argument's `pred_label`),
// and then copies the instructions in order, leaving out each `Label`
// whose name isn't among them.
//
// A function's entry is the `Function`'s `name` field and isn't an
// `asm_ast::Label`, so dropping a label is never observable from
// outside the function.
//
// Soundness
//
// A `Label` is only a marker, with no effect when the program runs.
// Control reaches the instruction after it either by falling through,
// where the label plays no part, or by a `Jump` or `Branch` that names
// it. With no such jump, falling through is the only way in, and the
// label can go. A `Phi` names its predecessors' labels, so its
// arguments count as references too, in case the pass runs while Phis
// remain.
//
// Where it runs
//
// Inside the asm peephole fixed point, early, so that the later
// peepholes (notably `apply_branch_invert`) see the cleaned-up labels.
// It runs in both pipelines, since it never adds code.
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
