#pragma once

#include <span>

#include "passes/optimization/framework.h"
#include "tac_ast.h"

namespace c6502::passes {

enum class JumpFoldRule {
  kCmpZero,
  kNarrowValueCompare,
  kNarrowJumpCompare,
  kAndZero,
  kLnot,
  kLnotOfZeroOrOne,
  kNonnegativeCompare,
};

// Returns `fn` with the rules `rules` applied in one sweep of a
// `RuleSet`: at each position the rules are tried in the given order,
// the first that fires replaces its window, and the scan goes on past
// it. Returns `fn` itself when no rule fires.
tac_ast::Function run_jump_fold_rules(tac_ast::Function fn, const framework::PassContext& ctx,
                                      std::span<const JumpFoldRule> rules);

// Returns `fn` with all seven rules applied in the order of the driver's
// rule set. A comparison and the jump on it, a `&` and the jump on it,
// and a `!` and the jump on it each fold into one jump in the same sweep,
// since the three rules match different producers.
tac_ast::Function fold_producer_jumps(tac_ast::Function fn, const framework::PassContext& ctx);

}  // namespace c6502::passes
