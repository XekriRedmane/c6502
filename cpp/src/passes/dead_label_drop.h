#pragma once

#include <string>
#include <unordered_set>

#include "asm_ast.h"
#include "passes/asm_liveness.h"

namespace c6502::passes {

// Drops each `Label` of the program's functions that no `Jump`,
// `Branch`, or `Phi` argument names.
asm_ast::Program apply_dead_label_drop(asm_ast::Program prog);

namespace dead_label_drop {

// Returns the label names that a `Jump.target`, a `Branch.target`, or a
// `Phi` argument's `pred_label` in `instrs` names.
std::unordered_set<std::string> collect_targets(Instrs instrs);

}  // namespace dead_label_drop

}  // namespace c6502::passes
