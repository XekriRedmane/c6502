// The port of passes/prune_unused_slots.py, which leaves out of the EQU
// block the body-local slots that no instruction names.
#pragma once

#include <string>
#include <utility>
#include <vector>

#include "asm_ast.h"

namespace c6502::passes {

// Symbol names with their addresses, in the order of a Python dict.
using OrderedSymbolAddrs = std::vector<std::pair<std::string, asdl::Int>>;

// Returns `slot_symbols` without the `__local_*` entries that no
// instruction of `prog` names, in the same order. Every other entry
// stays, `__zpabi_*` slots among them, since they are part of the
// calling convention and other units may name them.
OrderedSymbolAddrs prune_unused_locals(const asm_ast::Program& prog,
                                       const OrderedSymbolAddrs& slot_symbols);

}  // namespace c6502::passes
