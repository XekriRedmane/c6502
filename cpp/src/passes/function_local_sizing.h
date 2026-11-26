// Counts the zero-page bytes each function's body locals take after the
// asm optimizer's register allocation, and the bytes of its locals whose
// address is taken.
#pragma once

#include <string>

#include "asdl/runtime.h"
#include "asm_ast.h"
#include "ordered_map.h"
#include "passes/type_checking.h"

namespace c6502::passes {

// Returns the number of distinct zero-page bytes each function's body
// uses for the locals the register allocation colored, by function in
// the program's order, 0 for a function with none. It runs on the
// program `optimize_program` returns, before
// `replace_pseudoregisters_bare_exit`.
OrderedMap<std::string, asdl::Int> compute_local_bytes(const asm_ast::Program& prog);

// Returns the sum of the sizes of each function's locals whose address
// is taken, by function in the program's order. Such a local is a
// `Pseudo` that is the source of a `LoadAddress` in the function, other
// than a parameter or a static. `symbols` and `types` size each local
// by its declared type, and without `symbols` (null) every function
// counts 0.
OrderedMap<std::string, asdl::Int> compute_address_taken_bytes(const asm_ast::Program& prog,
                                                               const SymbolTable* symbols,
                                                               const TypeTable* types);

}  // namespace c6502::passes
