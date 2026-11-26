#pragma once

#include <cstddef>
#include <optional>
#include <vector>

#include "c99_ast.h"
#include "passes/optimization/framework.h"
#include "tac_ast.h"

namespace c6502::passes {

// Returns whether the source of the extend at `ext_idx`, whose operands
// are `src` and `dst`, holds at `use_idx` the value it held at the
// extend, so that a read of the extend's dst there can read the source
// instead. An SSA-renamed source has one definition, which dominates the
// extend. Any other source qualifies only when the extend comes earlier
// in the same straight run as the use, and nothing between them writes
// the source by name or might write memory. When `ssa_dsts` isn't null,
// the extend's dst must be SSA-renamed too, since an index of definitions
// records a name's last one. A `use_idx` of -1 stands for a position that
// wasn't found, which only an SSA-renamed source passes.
bool extend_source_unchanged(const tac_ast::Type_val& src, const tac_ast::Type_val& dst,
                             std::ptrdiff_t ext_idx, std::ptrdiff_t use_idx,
                             const framework::Instrs& instrs, const NameSet* ssa_dsts);

// The same over a `DefUsePass`'s view of the instructions, in which an
// entry that an earlier rewrite made a list is none and stops the walk.
bool extend_source_unchanged(const tac_ast::Type_val& src, const tac_ast::Type_val& dst,
                             std::ptrdiff_t ext_idx, std::ptrdiff_t use_idx,
                             const std::vector<std::optional<tac_ast::Type_instruction>>& instrs,
                             const NameSet* ssa_dsts);

namespace cmp_zero_jump_fold {

// Returns whether `t` is `char` or `unsigned char` as written, with no
// qualifier.
bool is_narrow_unsigned_type(const c99_ast::Type_data_type& t);

// Folds `instrs[i]` and `instrs[i + 1]`: a comparison whose single-use
// result the jump after it tests becomes one jump. Returns the
// replacement of the two, or none where the rule doesn't apply.
std::optional<framework::Instrs> fold_cmp_zero_jump_at(const framework::Instrs& instrs,
                                                       std::size_t i, framework::RuleEnv& env,
                                                       const framework::PassContext& ctx);

// Narrows `instrs[i]`: a comparison whose operands narrow to unsigned
// bytes compares the bytes, into the same dst. Returns the replacement,
// or none.
std::optional<framework::Instrs> narrow_value_compare_at(const framework::Instrs& instrs,
                                                         std::size_t i, framework::RuleEnv& env,
                                                         const framework::PassContext& ctx);

// Narrows `instrs[i]`: a `JumpIfCmp` whose operands narrow to unsigned
// bytes compares the bytes. Returns the replacement, or none.
std::optional<framework::Instrs> narrow_jump_compare_at(const framework::Instrs& instrs,
                                                        std::size_t i, framework::RuleEnv& env,
                                                        const framework::PassContext& ctx);

}  // namespace cmp_zero_jump_fold

}  // namespace c6502::passes
