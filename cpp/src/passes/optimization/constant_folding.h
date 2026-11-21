// The port of passes/optimization/constant_folding.py, the TAC
// optimizer's constant folding, with the loads from constant arrays and
// from tables split into byte planes. `fold` and `is_integer_const`
// are shared: `fold_into_constant_phi` folds an operation for each
// argument of a Phi with `fold`, and jump threading tests constants with
// `is_integer_const`.
#pragma once

#include <optional>

#include "passes/optimization/framework.h"
#include "passes/type_checking.h"
#include "tac_ast.h"

namespace c6502::passes {

// Returns `fn` with each instruction that constant folding folds
// replaced by its folded form, and each conditional jump on constants
// that is never taken dropped (Python's `ConstantFold`), or `fn` itself
// when nothing folds. The folds of the integer casts, the conversions
// to integers, and the loads from constant arrays read the
// destination's type from `ctx.symbols`, and without it those aren't
// folded.
tac_ast::Function constant_fold(tac_ast::Function fn, const framework::PassContext& ctx);

namespace constant_folding {

// What `fold` does with an instruction.
enum class FoldOutcome {
  // Nothing folds, where Python returns the instruction itself.
  kUnchanged,
  // The instruction is a conditional jump that is never taken, where
  // Python returns None.
  kDropped,
  // The instruction folds to `FoldResult::instr`.
  kReplaced,
};

struct FoldResult {
  FoldOutcome outcome = FoldOutcome::kUnchanged;
  // The folded instruction, for `kReplaced`.
  std::optional<tac_ast::Type_instruction> instr;
};

// Returns what constant folding makes of `instr` (Python's `_fold`): a
// `Unary`, `Binary`, or cast of constants becomes a `Copy` of the
// result, a `Copy` of an integer constant takes the class of its
// destination's type, a conditional jump on constants becomes a `Jump`
// or goes, a load from a constant array at a constant index becomes a
// `Copy` of the element, and a `Phi` whose arguments are all one value
// becomes a `Copy` of it. The module comment of the .cpp gives the
// arithmetic. `symbols` may be null.
FoldResult fold(const tac_ast::Type_instruction& instr, const SymbolTable* symbols);

// Returns whether `c` is an integer constant of any width, which is
// every class but `ConstFloat` and `ConstDouble`.
bool is_integer_const(const tac_ast::Type_const& c);

}  // namespace constant_folding

}  // namespace c6502::passes
