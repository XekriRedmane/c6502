// The port of passes/optimization/row_address_tables.py, which looks up
// the address of a row of a 2-D array in a table of row addresses.
#pragma once

#include "passes/type_checking.h"
#include "tac_ast.h"

namespace c6502::passes {

// Rewrites each row address of a qualifying array in `prog` as a lookup
// in a table of row addresses, when the unit's accesses save more than
// the tables cost (the .cpp's comment), and adds the tables to `prog`
// and to `symbols`. Without a symbol table (null) the program comes back
// unchanged. `types` is the type table, for the sizes of the rows, and
// may be null.
tac_ast::Program apply_row_address_tables(tac_ast::Program prog, SymbolTable* symbols,
                                          const TypeTable* types);

}  // namespace c6502::passes
