#pragma once

#include <string>
#include <string_view>

#include "asdl/runtime.h"
#include "asm_ast.h"
#include "ordered_map.h"
#include "passes/optimization/register_allocation.h"
#include "passes/type_checking.h"
#include "passes/zp_local_allocation.h"

namespace c6502::passes {

// The colorings of the asm optimizer, by function.
using ColoringsByFunction = OrderedMap<std::string, Coloring>;

// The address of the first byte of each address-taken local, by
// function and then by pseudo name.
using AddressTakenAssignments = OrderedMap<std::string, OrderedMap<std::string, asdl::Int>>;

// The slot symbol of each address-taken local, by function and then by
// pseudo name.
using AddressTakenSymbols = OrderedMap<std::string, OrderedMap<std::string, std::string>>;

// Returns, for every function of `prog`, the first zero-page byte of
// each of its address-taken locals that fits in the bytes of its pool
// (`local_pools`) that its coloring leaves free. A local that doesn't
// fit is left out, and `replace_pseudoregisters` gives it a frame slot.
// A function without a pool gets an empty map, so its address-taken
// locals stay on the soft stack. `symbols` and `types` size each local
// and may be null. Throws `InternalError` when a local of a function
// whose coloring keeps a value in Y doesn't fit.
AddressTakenAssignments compute_address_taken_assignments(const asm_ast::Program& prog,
                                                          const LocalPools& local_pools,
                                                          const ColoringsByFunction& colorings,
                                                          const SymbolTable* symbols,
                                                          const TypeTable* types);

namespace address_taken_zp {

// Returns the slot symbol of each pseudo in `assignments`, a function's
// address-taken locals: `__local_<fn>__<source>`, where `<source>` is
// the source name `parse_pseudo_name` recovers, or
// `__local_<fn>__addr_<pseudo>` for a compiler temporary. Two locals of
// one function can have the same source name, as two compound literals
// or a name declared again in an inner block do, so the second takes
// `@2` after its name, the third `@3`, and so on, since `@` can't occur
// in a C identifier.
OrderedMap<std::string, std::string> slot_symbols(std::string_view fn_name,
                                                  const OrderedMap<std::string, asdl::Int>& assignments,
                                                  const SymbolTable* symbols, const TypeTable* types);

}  // namespace address_taken_zp

}  // namespace c6502::passes
