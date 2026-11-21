// Checks of recorded calls of a rule set made of some of the jump-fold
// rules, which the Python unit tests run on their own (`RuleSet(CMP_ZERO_RULE)`
// in `fold_cmp_zero_jump`, for one). The record holds the function, the
// symbols the call read and the SSA names, and what the Python rule set
// returned and changed, as `run_tac_pass` in tac_stage.cpp reads and
// writes them for a pass of the driver.
#pragma once

#include <initializer_list>
#include <optional>
#include <ostream>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "asdl/runtime.h"
#include "doctest.h"
#include "json.h"
#include "passes/optimization/fold_producer_jumps.h"
#include "tac_ast.h"
#include "tagged_json.h"

namespace c6502::testing {

// Runs the rules `rules` in that order on `input` with the call's
// arguments (`args_json`, the harness's `{"args": [], "kwargs":
// {"symbols": ..., "ssa_dsts": ...}}`), and checks that the rule set
// returns `output` and changes the symbols and the SSA names as `mutated`
// records (empty when it changes neither).
inline void check_rules_case(std::initializer_list<passes::JumpFoldRule> rules,
                             std::string_view args_json, std::string_view input,
                             std::string_view output, std::string_view mutated) {
  using namespace ::c6502::tagged;
  namespace ir = ::c6502::tac_ast;
  const json::Value args = json::parse(args_json);
  const json::Value& kwargs = *args.get("kwargs");
  auto fn = std::get<ir::Function>(asdl::load<ir::Type_top_level>(std::string(input)));
  std::optional<passes::SymbolTable> symbols =
      Optional<SymbolTableC>::decode(*kwargs.get("symbols"));
  std::optional<passes::NameSet> ssa = Optional<NameSetC>::decode(*kwargs.get("ssa_dsts"));
  const std::string symbols_before = encode<Optional<SymbolTableC>>(symbols);
  const std::string ssa_before = encode<Optional<NameSetC>>(ssa);
  const passes::framework::PassContext ctx{
      .symbols = symbols ? &*symbols : nullptr,
      .ssa_dsts = ssa ? &*ssa : nullptr,
  };
  const std::vector<passes::JumpFoldRule> list(rules);
  ir::Function out = passes::run_jump_fold_rules(std::move(fn), ctx, list);
  CHECK(asdl::dump(ir::Type_top_level{std::move(out)}) == output);
  std::string changed;
  const std::string symbols_after = encode<Optional<SymbolTableC>>(symbols);
  if (symbols_after != symbols_before) {
    changed += "\"symbols\": " + symbols_after;
  }
  const std::string ssa_after = encode<Optional<NameSetC>>(ssa);
  if (ssa_after != ssa_before) {
    if (!changed.empty()) {
      changed += ", ";
    }
    changed += "\"ssa_dsts\": " + ssa_after;
  }
  const std::string got_mutated =
      changed.empty() ? std::string() : "{\"args\": {}, \"kwargs\": {" + changed + "}}";
  CHECK(got_mutated == mutated);
}

}  // namespace c6502::testing
