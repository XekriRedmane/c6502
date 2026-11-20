// The port of the walkers of passes/optimization/var_visit.py over the
// operands of a TAC instruction, which SSA construction, liveness, the
// interference graph, dead-store elimination, and the code generation
// passes share. They work on TAC in SSA form and out of it alike.
#pragma once

#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

#include "ordered_map.h"
#include "tac_ast.h"

namespace c6502::passes {

// Returns every `Type_val` operand of `instr`, read or written, in
// about the order of the source. The pointers are into `instr`.
std::vector<const tac_ast::Type_val*> vals_in(const tac_ast::Type_instruction& instr);

// Returns the names of the `Var` operands `instr` writes. The views are
// into `instr`.
std::vector<std::string_view> defs_in(const tac_ast::Type_instruction& instr);

// Returns the names of the `Var` operands `instr` reads, in order. A
// `GetAddress`'s operand isn't one, since it names a storage cell and
// reads no value. An `IndexedLoad` and an `IndexedSymbolStore` read the
// object they name, since the load reads it and the store rewrites only
// some of its bytes and leaves the rest of the earlier value. Without
// that, a struct local whose members are accessed in place looked
// unread, and copy folding dropped the copy that filled it. A `Phi`'s
// sources come back as reads too, which liveness handles as reads on
// the edges from the predecessors. The views are into `instr`.
std::vector<std::string_view> uses_in(const tac_ast::Type_instruction& instr);

// Returns how many times each name is read (`uses_in`) in `instrs`, by
// name in the order the names are first read, as Python's `Counter`
// holds them.
OrderedMap<std::string, std::size_t> count_uses(const std::vector<tac_ast::Type_instruction>& instrs);

}  // namespace c6502::passes
