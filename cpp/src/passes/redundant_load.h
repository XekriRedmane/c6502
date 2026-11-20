// The part of passes/redundant_load.py that other passes share so far:
// the CFG builder that tolerates jumps out of the function.
#pragma once

#include "asm_ast.h"
#include "passes/optimization_asm/cfg.h"

namespace c6502::passes {

// Returns the CFG of `fn` (`build_cfg`), with a synthetic block of
// `Label(target); Return` after the function's instructions for each
// `Branch` / `Jump` target the function doesn't define, in the order
// the targets first appear. A tail call is such a jump, so it reaches
// EXIT through its synthetic block. The synthetic instructions are in
// the CFG's `extra` list and never in a rewritten function.
Cfg build_cfg_tolerant(const asm_ast::Function& fn);

}  // namespace c6502::passes
