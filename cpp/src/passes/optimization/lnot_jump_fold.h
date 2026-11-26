#pragma once

#include <cstddef>
#include <optional>

#include "passes/optimization/framework.h"
#include "tac_ast.h"

namespace c6502::passes::lnot_jump_fold {

// Returns the jump with the opposite sense on `src` in place of
// `Unary(LogicalNot, src, %t)` at `instrs[i]` and a jump on a single-use
// `%t` at `instrs[i + 1]`, or none.
std::optional<framework::Instrs> fold_lnot_jump_at(const framework::Instrs& instrs,
                                                   std::size_t i, framework::RuleEnv& env,
                                                   const framework::PassContext& ctx);

// Returns `x ^ 1` in place of a `!` at `instrs[i]` that no jump tests,
// of a value `x` that is 0 or 1, or none. A result of another width than
// `x` adds the temporary `<dst>.not<k>` to the symbols, and to the SSA
// names in SSA form.
std::optional<framework::Instrs> lnot_of_zero_or_one_at(const framework::Instrs& instrs,
                                                        std::size_t i, framework::RuleEnv& env,
                                                        const framework::PassContext& ctx);

}  // namespace c6502::passes::lnot_jump_fold
