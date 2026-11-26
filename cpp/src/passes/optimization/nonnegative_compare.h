#pragma once

#include <cstddef>
#include <optional>

#include "passes/optimization/framework.h"
#include "tac_ast.h"

namespace c6502::passes::nonnegative_compare {

// Returns the signed ordering `JumpIfCmp` or comparison kept as a value
// at `instrs[i]` as an unsigned one when `value_range::max_value` shows
// that neither operand can be negative, or none. A compare of two Vars
// adds the temporary `<src1>.u<k>` to the symbols and the SSA names.
std::optional<framework::Instrs> nonnegative_compare_at(const framework::Instrs& instrs,
                                                        std::size_t i, framework::RuleEnv& env,
                                                        const framework::PassContext& ctx);

}  // namespace c6502::passes::nonnegative_compare
