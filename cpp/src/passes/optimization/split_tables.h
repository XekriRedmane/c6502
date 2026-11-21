// The part of passes/optimization/split_tables.py that code generation
// reads: the static initializer of the byte planes of a table.
#pragma once

#include <vector>

#include "passes/type_checking.h"
#include "tac_ast.h"

namespace c6502::passes::split_tables {

// Returns the static initializer that lays `planes` down one after
// another. Each cell is an integer byte (a `ValueTree` of kind `kInt`)
// or the `AddressByteInit` of one byte of a link-time address (kind
// `kTacInit`). A run of integer bytes becomes one `StringInit`, which
// `asm_emit` writes as one `DC.B` line of hex bytes. Python's
// `plane_init` also returns the tuple of the cells for the table's
// `Initial`, which `tac_to_asm` doesn't read, so this leaves it out.
std::vector<tac_ast::Type_static_init> plane_init(const std::vector<std::vector<ValueTree>>& planes);

}  // namespace c6502::passes::split_tables
