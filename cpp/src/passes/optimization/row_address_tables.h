#pragma once

#include <optional>

#include "asdl/runtime.h"
#include "passes/type_checking.h"
#include "tac_ast.h"

namespace c6502::passes {

// Rewrites each row address of a qualifying array in `prog` as a lookup
// in a table of row addresses, when the unit's accesses save more than
// the tables cost, and adds the tables to `prog` and to `symbols`.
// Without a symbol table (null) the program comes back unchanged.
// `types` is the type table, for the sizes of the rows, and may be
// null. `fixed_saving`, when given, is the bytes each lookup saves in
// place of the estimate, so that a test can force a table (a large
// value) or forbid one.
tac_ast::Program apply_row_address_tables(tac_ast::Program prog, SymbolTable* symbols,
                                          const TypeTable* types,
                                          std::optional<asdl::Int> fixed_saving = std::nullopt);

}  // namespace c6502::passes
