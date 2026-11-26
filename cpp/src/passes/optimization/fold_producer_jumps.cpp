#include "passes/optimization/fold_producer_jumps.h"

#include <array>
#include <cstddef>
#include <optional>
#include <utility>

#include "passes/optimization/and_zero_jump_fold.h"
#include "passes/optimization/cmp_zero_jump_fold.h"
#include "passes/optimization/lnot_jump_fold.h"
#include "passes/optimization/nonnegative_compare.h"

namespace c6502::passes {

namespace {

using framework::Instrs;
using framework::PassContext;
using framework::RuleEnv;

using RuleFn = std::optional<Instrs> (*)(const Instrs&, std::size_t, RuleEnv&,
                                         const PassContext&);

struct RuleEntry {
  RuleFn fn;
  std::size_t window;
};

RuleEntry entry_of(JumpFoldRule rule) {
  switch (rule) {
    case JumpFoldRule::kCmpZero:
      return {&cmp_zero_jump_fold::fold_cmp_zero_jump_at, 2};
    case JumpFoldRule::kNarrowValueCompare:
      return {&cmp_zero_jump_fold::narrow_value_compare_at, 1};
    case JumpFoldRule::kNarrowJumpCompare:
      return {&cmp_zero_jump_fold::narrow_jump_compare_at, 1};
    case JumpFoldRule::kAndZero:
      return {&and_zero_jump_fold::fold_narrow_and_jump_at, 2};
    case JumpFoldRule::kLnot:
      return {&lnot_jump_fold::fold_lnot_jump_at, 2};
    case JumpFoldRule::kLnotOfZeroOrOne:
      return {&lnot_jump_fold::lnot_of_zero_or_one_at, 1};
    case JumpFoldRule::kNonnegativeCompare:
      return {&nonnegative_compare::nonnegative_compare_at, 1};
  }
  return {nullptr, 0};
}

constexpr std::array<JumpFoldRule, 7> kAllRules = {
    JumpFoldRule::kCmpZero,  JumpFoldRule::kNarrowValueCompare, JumpFoldRule::kNarrowJumpCompare,
    JumpFoldRule::kAndZero,  JumpFoldRule::kLnot,               JumpFoldRule::kLnotOfZeroOrOne,
    JumpFoldRule::kNonnegativeCompare,
};

}  // namespace

tac_ast::Function run_jump_fold_rules(tac_ast::Function fn, const PassContext& ctx,
                                      std::span<const JumpFoldRule> rules) {
  std::vector<RuleEntry> entries;
  for (const JumpFoldRule r : rules) {
    entries.push_back(entry_of(r));
  }
  return framework::run_rules(
      std::move(fn),
      [&](const Instrs& instrs, std::size_t i,
          RuleEnv& env) -> std::optional<std::pair<Instrs, std::size_t>> {
        for (const RuleEntry& e : entries) {
          if (i + e.window > instrs.size()) {
            continue;
          }
          if (std::optional<Instrs> rep = e.fn(instrs, i, env, ctx)) {
            return std::make_pair(std::move(*rep), e.window);
          }
        }
        return std::nullopt;
      });
}

tac_ast::Function fold_producer_jumps(tac_ast::Function fn, const PassContext& ctx) {
  return run_jump_fold_rules(std::move(fn), ctx, kAllRules);
}

}  // namespace c6502::passes
