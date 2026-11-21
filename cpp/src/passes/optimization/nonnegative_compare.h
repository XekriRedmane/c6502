// The port of passes/optimization/nonnegative_compare.py, the rule of
// the jump-fold rule set that does a signed ordering compare of two
// values that can't be negative as an unsigned one
// (`NONNEGATIVE_COMPARE_RULE`). fold_producer_jumps.h runs it.
#pragma once

#include <cstddef>
#include <optional>

#include "passes/optimization/framework.h"
#include "tac_ast.h"

namespace c6502::passes::nonnegative_compare {

// `NONNEGATIVE_COMPARE_RULE` at `instrs[i]`: a `JumpIfCmp` or a
// comparison kept as a value, of two values that
// `value_range::max_value` bounds, compares them unsigned. Returns the
// replacement, or none. A compare of two Vars adds the temporary
// `<src1>.u<k>` to the symbols and the SSA names.
std::optional<framework::Instrs> nonnegative_compare_at(const framework::Instrs& instrs,
                                                        std::size_t i, framework::RuleEnv& env,
                                                        const framework::PassContext& ctx);

}  // namespace c6502::passes::nonnegative_compare
