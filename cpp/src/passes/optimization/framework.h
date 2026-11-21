// The port of the run loops of passes/optimization/framework/, which
// most of the TAC optimizer's passes are built on: the context every
// pass takes (`PassContext`), the window scan of `WindowPass` and
// `RuleSet`, the SSA def-use walk of `DefUsePass`, the move of
// `SinkPass`, and the operand walk of `OperandRewritePass`.
//
// The Python framework matches instructions with the pattern DSL of
// framework/patterns.py and calls a pass's `rewrite` on a match. The
// C++ loops take the matching and the rewrite as one function, which
// tests the instruction's kind and fields directly and returns none
// where the Python pattern doesn't match or the rewrite declines. A
// `RuleSet`'s rules become one function that tries them in the Python
// order. The order in which the loops visit the instructions, and how
// far they advance after a rewrite, are the Python framework's, since
// a later rewrite in a sweep reads what an earlier one left.
#pragma once

#include <cstddef>
#include <map>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "ordered_map.h"
#include "passes/asm_context.h"
#include "passes/optimization/var_visit.h"
#include "passes/type_checking.h"
#include "tac_ast.h"

namespace c6502::passes::framework {

using Instrs = std::vector<tac_ast::Type_instruction>;

// What every pass of the TAC optimizer is given (Python's
// `PassContext`): the symbol table, which a pass may add temporaries
// to, and the names SSA construction renamed, which some passes add
// to. Either is null where Python's is None: the symbol table for the
// unit tests that run a pass without one, and the SSA names outside SSA
// form.
struct PassContext {
  SymbolTable* symbols = nullptr;
  NameSet* ssa_dsts = nullptr;

  // Returns whether `name` is one of the SSA names, which is false
  // outside SSA form.
  bool is_ssa(std::string_view name) const {
    return ssa_dsts != nullptr && ssa_dsts->contains(name);
  }
};

// Returns `fn` with its instructions replaced by `instructions`, as the
// framework's `tac_ast.Function(name=..., ..., instructions=out)` does.
tac_ast::Function with_instructions(const tac_ast::Function& fn, Instrs instructions);

// Returns the name of the `Var` in the `dst` field of `instr`, or none
// when it has no such field or the field isn't a Var. This is the test
// `DefUsePass` builds its index of definitions with, `hasattr(instr,
// "dst") and isinstance(instr.dst, Var)`. It differs from
// `var_visit.defs_in`, which `RuleSet` uses, in one instruction: it
// takes an `IndirectIndexedLoad`'s destination, which defs_in leaves
// out.
std::optional<std::string_view> dst_name(const tac_ast::Type_instruction& instr);

// --- WindowPass ---

// Runs a `WindowPass`: at each position `i` from the start, when
// `window_size` instructions are left, calls `rewrite(instrs, i)`, which
// returns the instructions that replace `instrs[i .. i + window_size)`
// or none when the window doesn't match or the rewrite declines. A
// replacement advances the scan past the window, and anything else
// advances it by one, so a rewrite's output isn't matched again in the
// same sweep. Returns `fn` itself when nothing was replaced, as Python
// returns the same object.
template <class Rewrite>
tac_ast::Function run_window(tac_ast::Function fn, std::size_t window_size, Rewrite rewrite) {
  const Instrs& instrs = fn.instructions;
  const std::size_t n = instrs.size();
  Instrs out;
  bool changed = false;
  std::size_t i = 0;
  while (i < n) {
    if (i + window_size <= n) {
      std::optional<Instrs> rep = rewrite(instrs, i);
      if (rep) {
        for (auto& ins : *rep) {
          out.push_back(std::move(ins));
        }
        i += window_size;
        changed = true;
        continue;
      }
    }
    out.push_back(instrs[i]);
    ++i;
  }
  if (!changed) {
    return fn;
  }
  return with_instructions(fn, std::move(out));
}

// --- DefUsePass ---

// The view of the function that a `DefUsePass` rewrite reads (Python's
// `DefUseEnv`): the index of each name's definition, and the
// instructions with the rewrites recorded so far in this sweep, so that
// a rewrite reads what an earlier one made of a definition.
struct DefUseEnv {
  std::map<std::string, std::size_t, std::less<>> def_idx;
  // The instructions as rewritten so far. An entry is none where an
  // earlier rewrite of this sweep returned a list without a `Rewrite`,
  // which Python stores as the list itself, so that `def_of` returns a
  // value no `isinstance` test matches.
  std::vector<std::optional<tac_ast::Type_instruction>> instructions;

  // Returns the instruction that defines `name`, or null when there is
  // none or it is one of those lists.
  const tac_ast::Type_instruction* def_of(std::string_view name) const;
  const tac_ast::Type_instruction* def_of(const tac_ast::Var& var) const {
    return def_of(var.name);
  }
};

// What a `DefUsePass` rewrite returns for the instruction it matched: a
// bare instruction, a bare list, or a `Rewrite`, whose replacement is
// one instruction or a list and whose `drop_defs` name the definitions
// to drop with it.
struct DefUseRewrite {
  Instrs replacement;
  // Whether the replacement is a list (several instructions, or one
  // or none in a list), where it is otherwise one instruction.
  bool is_list = false;
  // Whether the result is a `Rewrite`.
  bool is_rewrite = false;
  std::vector<std::string> drop_defs;

  // Returns a bare instruction.
  static DefUseRewrite one(tac_ast::Type_instruction instr);
  // Returns a `Rewrite` of one instruction that drops the definitions
  // of `drop_defs`.
  static DefUseRewrite dropping(tac_ast::Type_instruction instr, std::vector<std::string> drop_defs);
};

// Runs a `DefUsePass`: calls `rewrite(instr, env)` on each instruction
// of `fn` in order, which returns none where the Python pattern doesn't
// match or the rewrite declines (or returns the instruction itself).
// Outside SSA form (`ctx.ssa_dsts` null) the pass leaves `fn` alone. A
// `Rewrite` whose definitions to drop include one an earlier `Rewrite`
// of the sweep dropped is skipped, though anything its rewrite did, such
// as adding a symbol, stays. The dropped definitions go from the output,
// except that a list replacement of a dropped index keeps all but its
// last instruction. Returns `fn` itself when nothing changed.
template <class Rewrite>
tac_ast::Function run_def_use(tac_ast::Function fn, const PassContext& ctx, Rewrite rewrite);

// Returns the index of the definition of each name in `instrs` by
// `dst_name`, the last one for a name defined more than once.
std::map<std::string, std::size_t, std::less<>> build_def_idx(const Instrs& instrs);

// Builds the output of `run_def_use` from its rewrites by index and the
// indices dropped.
Instrs rebuild_def_use(const Instrs& instrs, std::map<std::size_t, DefUseRewrite>& rewrites,
                       const std::vector<bool>& dropped);

template <class Rewrite>
tac_ast::Function run_def_use(tac_ast::Function fn, const PassContext& ctx, Rewrite rewrite) {
  if (ctx.ssa_dsts == nullptr) {
    return fn;
  }
  const Instrs& instrs = fn.instructions;
  DefUseEnv env;
  env.def_idx = build_def_idx(instrs);
  env.instructions.assign(instrs.begin(), instrs.end());
  std::map<std::size_t, DefUseRewrite> rewrites;
  std::vector<bool> dropped(instrs.size(), false);
  bool changed = false;
  for (std::size_t i = 0; i < instrs.size(); ++i) {
    std::optional<DefUseRewrite> result = rewrite(instrs[i], env);
    if (!result) {
      continue;
    }
    if (result->is_rewrite) {
      std::vector<std::size_t> new_drops;
      bool conflict = false;
      for (const auto& name : result->drop_defs) {
        const auto it = env.def_idx.find(name);
        if (it == env.def_idx.end()) {
          continue;
        }
        if (dropped[it->second]) {
          conflict = true;
        }
        new_drops.push_back(it->second);
      }
      if (conflict) {
        // An earlier rewrite already consumed one of the same inner
        // definitions, so this one is skipped to keep from dropping it
        // twice.
        continue;
      }
      if (result->is_list) {
        if (result->replacement.empty()) {
          // Python's `replacement[-1]` of an empty list raises.
          throw std::runtime_error("a Rewrite's replacement is an empty list");
        }
        env.instructions[i] = result->replacement.back();
      } else {
        env.instructions[i] = result->replacement.front();
      }
      for (const std::size_t d : new_drops) {
        dropped[d] = true;
      }
    } else if (result->is_list) {
      env.instructions[i].reset();
    } else {
      env.instructions[i] = result->replacement.front();
    }
    rewrites.insert_or_assign(i, std::move(*result));
    changed = true;
  }
  if (!changed) {
    return fn;
  }
  return with_instructions(fn, rebuild_def_use(instrs, rewrites, dropped));
}

// --- RuleSet ---

// The analyses every rule of a `RuleSet` sweep shares (Python's
// `RuleEnv`): the use counts, the index of each name's definition by
// `var_visit.defs_in`, the instructions, and the names a use count
// doesn't cover, filled in on first need.
struct RuleEnv {
  OrderedMap<std::string, std::size_t> use_count;
  std::map<std::string, std::size_t, std::less<>> def_idx;
  const Instrs* instructions = nullptr;
  std::optional<NameSet> observable;

  // Returns the instruction that defines `v` when it is a Var, or null.
  const tac_ast::Type_instruction* def_of(const tac_ast::Type_val& v) const;
  // Returns whether `v` is a Var read exactly once in the function.
  bool is_single_use(const tac_ast::Type_val& v) const;
};

// Returns the `RuleEnv` of `instrs`.
RuleEnv make_rule_env(const Instrs& instrs);

// Returns the names in the instructions that can be read without a Var
// use there (Python's `_observable_names`): every name whose address a
// `GetAddress` takes, and every static the instructions read or write.
// Without a symbol table the statics can't be told apart, and only the
// taken addresses count.
const NameSet& observable_names(RuleEnv& env, const PassContext& ctx);

// The guard `single_use(capture)`: whether `v` is read exactly once in
// the function and nothing outside its instructions can read it, which
// is the case for an SSA name, and otherwise for a name that isn't a
// static and whose address isn't taken.
bool single_use(const tac_ast::Type_val& v, RuleEnv& env, const PassContext& ctx);

// Runs a `RuleSet`: at each position `i`, calls `try_rules(instrs, i,
// env)`, which tries the rules in their order (each with the window it
// matches, which must fit in what is left of the function) and returns
// the replacement of the first that fires with its window size, or
// none. A replacement advances the scan past the window, and anything
// else by one. Returns `fn` itself when nothing was replaced.
template <class TryRules>
tac_ast::Function run_rules(tac_ast::Function fn, TryRules try_rules) {
  const Instrs& instrs = fn.instructions;
  const std::size_t n = instrs.size();
  RuleEnv env = make_rule_env(instrs);
  Instrs out;
  bool changed = false;
  std::size_t i = 0;
  while (i < n) {
    std::optional<std::pair<Instrs, std::size_t>> rep = try_rules(instrs, i, env);
    if (rep) {
      for (auto& ins : rep->first) {
        out.push_back(std::move(ins));
      }
      i += rep->second;
      changed = true;
    } else {
      out.push_back(instrs[i]);
      ++i;
    }
  }
  if (!changed) {
    return fn;
  }
  return with_instructions(fn, std::move(out));
}

// --- SinkPass ---

// Runs a `SinkPass`: for each instruction `instrs[i]`, calls
// `find_target(instrs, i)`, which returns the index the instruction
// moves to (past `instrs[i + 1 .. target]`) or none where the pattern
// doesn't match or the instruction stays. A move advances the scan past
// the instruction's new place. Returns `fn` itself when nothing moved.
template <class FindTarget>
tac_ast::Function run_sink(tac_ast::Function fn, FindTarget find_target) {
  const Instrs& instrs = fn.instructions;
  const std::size_t n = instrs.size();
  Instrs out;
  bool changed = false;
  std::size_t i = 0;
  while (i < n) {
    const std::optional<std::size_t> target = find_target(instrs, i);
    if (target && *target > i) {
      for (std::size_t k = i + 1; k <= *target; ++k) {
        out.push_back(instrs[k]);
      }
      out.push_back(instrs[i]);
      i = *target + 1;
      changed = true;
      continue;
    }
    out.push_back(instrs[i]);
    ++i;
  }
  if (!changed) {
    return fn;
  }
  return with_instructions(fn, std::move(out));
}

// --- OperandRewritePass ---

// Runs an `OperandRewritePass`: rewrites every operand each instruction
// reads with `mapper` (`var_visit.rewrite_uses_in`), which returns the
// replacement, or none where the operand pattern doesn't match or the
// rewrite declines. Returns `fn` itself when nothing changed.
tac_ast::Function run_operand_rewrite(tac_ast::Function fn, const ValMapper& mapper);

}  // namespace c6502::passes::framework
