// The port of passes/optimization/lnot_jump_fold.py, the rules of the
// jump-fold rule set that fold a `!` and the jump on its result into a
// jump on the `!`'s operand (`LNOT_RULE`), and that make a `!` of a value
// that is 0 or 1 an exclusive or with 1 (`LNOT_OF_ZERO_OR_ONE_RULE`).
// fold_producer_jumps.h runs them.
#pragma once

#include <cstddef>
#include <optional>

#include "passes/optimization/framework.h"
#include "tac_ast.h"

namespace c6502::passes::lnot_jump_fold {

// `LNOT_RULE` at `instrs[i]` and `instrs[i + 1]`: `Unary(LogicalNot, src,
// %t)` and a jump on a single-use `%t` become the jump with the opposite
// sense on `src`. Returns the replacement of the two, or none.
std::optional<framework::Instrs> fold_lnot_jump_at(const framework::Instrs& instrs,
                                                   std::size_t i, framework::RuleEnv& env,
                                                   const framework::PassContext& ctx);

// `LNOT_OF_ZERO_OR_ONE_RULE` at `instrs[i]`: a `!` that no jump tests, of
// a value that is 0 or 1, becomes `x ^ 1`. Returns the replacement, or
// none. A result of another width than `x` adds the temporary
// `<dst>.not<k>` to the symbols, and to the SSA names in SSA form.
std::optional<framework::Instrs> lnot_of_zero_or_one_at(const framework::Instrs& instrs,
                                                        std::size_t i, framework::RuleEnv& env,
                                                        const framework::PassContext& ctx);

}  // namespace c6502::passes::lnot_jump_fold
