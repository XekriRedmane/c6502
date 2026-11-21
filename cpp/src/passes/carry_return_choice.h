// The port of passes/carry_return_choice.py, which gives a `bool`
// function the carry-flag return of `[[c6502::return_bool_in_carry]]`
// without the annotation, when every call to it gains by it.
#pragma once

#include "passes/abi_selection.h"
#include "passes/asm_context.h"
#include "passes/type_checking.h"
#include "tac_ast.h"

namespace c6502::passes {

// Sets `carry_return` on each static `bool` function of `prog` that is
// chosen, in `symbols`, and returns their names.
NameSet choose_carry_returns(const tac_ast::Program& prog, SymbolTable& symbols);

// Sets `carry_return` on each function with external linkage that
// returns `bool` and is zp_abi in `abi`, whether this unit defines it or
// only declares it, and returns their names.
NameSet choose_global_carry_returns(const tac_ast::Program& prog, SymbolTable& symbols,
                                    const Abi& abi);

}  // namespace c6502::passes
