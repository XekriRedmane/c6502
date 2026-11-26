#pragma once

#include <cstddef>
#include <optional>

#include "passes/optimization/framework.h"
#include "tac_ast.h"

namespace c6502::passes::and_zero_jump_fold {

// Folds `instrs[i]` and `instrs[i + 1]`: a `Binary(BitwiseAnd, ...)`
// whose single-use result the jump after it tests becomes a
// `JumpIfMasked`, when one operand is a byte or the mask falls in one
// byte of a wide operand. Returns the replacement of the two, or none
// where the rule doesn't apply. The wide form adds the temporaries
// `<dst>.byte` and `<dst>.b` to the symbols, and to the SSA names in SSA
// form.
std::optional<framework::Instrs> fold_narrow_and_jump_at(const framework::Instrs& instrs,
                                                         std::size_t i, framework::RuleEnv& env,
                                                         const framework::PassContext& ctx);

}  // namespace c6502::passes::and_zero_jump_fold
