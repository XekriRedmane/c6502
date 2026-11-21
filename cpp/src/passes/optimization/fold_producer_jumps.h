// The jump-fold rule set of passes/optimization/optimizer.py,
// `RuleSet(CMP_ZERO_RULE, NARROW_VALUE_COMPARE_RULE,
// NARROW_JUMP_COMPARE_RULE, AND_ZERO_RULE, LNOT_RULE,
// LNOT_OF_ZERO_OR_ONE_RULE, NONNEGATIVE_COMPARE_RULE,
// name="fold_producer_jumps")`, whose rules are in cmp_zero_jump_fold.h,
// and_zero_jump_fold.h, lnot_jump_fold.h, and nonnegative_compare.h.
#pragma once

#include <span>

#include "passes/optimization/framework.h"
#include "tac_ast.h"

namespace c6502::passes {

// The rules of the jump-fold rule set, named after the Python `Rule`s.
enum class JumpFoldRule {
  kCmpZero,               // CMP_ZERO_RULE, "fold_cmp_zero_jump"
  kNarrowValueCompare,    // NARROW_VALUE_COMPARE_RULE
  kNarrowJumpCompare,     // NARROW_JUMP_COMPARE_RULE
  kAndZero,               // AND_ZERO_RULE, "fold_narrow_and_jump"
  kLnot,                  // LNOT_RULE, "fold_lnot_jump"
  kLnotOfZeroOrOne,       // LNOT_OF_ZERO_OR_ONE_RULE
  kNonnegativeCompare,    // NONNEGATIVE_COMPARE_RULE
};

// Returns `fn` with the rules `rules` applied in one sweep of a
// `RuleSet`: at each position the rules are tried in the given order,
// the first that fires replaces its window, and the scan goes on past
// it. Returns `fn` itself when no rule fires. The Python unit tests run
// rule sets of one or two of the rules, which this reproduces.
tac_ast::Function run_jump_fold_rules(tac_ast::Function fn, const framework::PassContext& ctx,
                                      std::span<const JumpFoldRule> rules);

// Returns `fn` with all seven rules applied in the order of the
// driver's rule set (Python's `fold_producer_jumps`). A comparison and
// the jump on it, a `&` and the jump on it, and a `!` and the jump on it
// each fold into one jump in the same sweep, since the three rules
// match different producers.
tac_ast::Function fold_producer_jumps(tac_ast::Function fn, const framework::PassContext& ctx);

}  // namespace c6502::passes
