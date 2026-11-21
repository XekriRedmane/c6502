// The port of passes/optimization/split_tables.py, which stores a table
// of 2-byte elements read by a byte index as its low bytes and then its
// high bytes, with the parts that other modules read: the static
// initializer of the byte planes of a table, which code generation and
// `row_address_tables` read, and the byte width of a scalar type, which
// `recognize_indirect_indexed` reads.
#pragma once

#include <optional>
#include <vector>

#include "c99_ast.h"
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

// Returns the byte width of the scalar type `t` (Python's `_width`), or
// none for any other type. A `_BitInt(N)` takes ceil(N/8) bytes. The
// qualifiers aren't stripped, so a qualified type is none.
std::optional<int> width(const c99_ast::Type_data_type& t);

}  // namespace c6502::passes::split_tables

namespace c6502::passes {

// Splits each qualifying table of `prog` into byte planes and rewrites
// its reads (the .cpp's comment), and returns the program. `symbols`
// gives each name's c99 type, and the split tables' entries are
// replaced. Without a table (null) the program comes back unchanged.
tac_ast::Program apply_split_tables(tac_ast::Program prog, SymbolTable* symbols);

}  // namespace c6502::passes
