// The port of passes/zp_local_allocation.py: which functions get a
// private zero-page pool for their body locals (`pool_eligible`), which
// of those are on a cycle whose recursive calls save on the hardware
// stack (`recursive_pool_functions`), the allocation of the pools
// (`allocate_function_locals`), and the `EQU` symbols of their bytes
// (`build_local_slot_symbols`).
#pragma once

#include <string>
#include <vector>

#include "ordered_map.h"
#include "passes/abi_selection.h"
#include "passes/asm_context.h"
#include "passes/recursion_saves.h"
#include "passes/zp_slot_allocation.h"
#include "tac_ast.h"

namespace c6502::passes {

// Returns the functions `allocate_function_locals` gives pools to (see
// "Eligibility" in the .cpp). It reads `abi` only for `extern` callees,
// so `select_abi` can ask before it has decided the rest.
NameSet pool_eligible(const tac_ast::Program& prog, const Abi& abi);

// Returns the functions on a cycle that `allocate_function_locals` gives
// pools, each mapped to its cycle's members, in the order of
// `hw_stack_cycles`. Every call from one to another must save the
// caller's live values (recursion_saves.h).
Cycles recursive_pool_functions(const tac_ast::Program& prog, const Abi& abi);

// Each eligible function's pool, its bytes in order, in the order the
// allocator placed them.
using LocalPools = OrderedMap<std::string, std::vector<asdl::Int>>;

// Returns a private pool of body-local bytes for every eligible
// function, `local_bytes[fn]` addresses long (none for a function
// without an entry). An ineligible function gets no entry, and its
// register allocation uses the conservative caller-saved and
// callee-saved partition. `options` are as for `allocate_zp_slots`, and
// its `shares` are the ones `allocate_zp_slots` placed, whose addresses
// are in `abi`. Throws `CompileError("ZpLocalAllocationError", ...)`
// when a pool fits nowhere.
LocalPools allocate_function_locals(const tac_ast::Program& prog, const Abi& abi,
                                    const OrderedMap<std::string, asdl::Int>& local_bytes,
                                    const ZpAllocationOptions& options = {});

// Returns the `EQU` symbol of every byte of every pool, mapped to its
// address: the name `slot_names_by_fn` gives the byte, one per pool byte
// in pool order (`compute_local_slot_names`), or `__local_<fn>__<k>` for
// a function it has no entry for, or when it is null. Python can compute
// the names from the colorings when it isn't given them, which is
// `compute_local_slot_names`, so a caller with colorings calls
// `local_slots` (src/passes/zp_slot_naming.h), which computes the names
// and then calls this.
OrderedMap<std::string, asdl::Int> build_local_slot_symbols(
    const LocalPools& local_pools,
    const OrderedMap<std::string, std::vector<std::string>>* slot_names_by_fn);

namespace zp_local_allocation {

// Returns the functions of `callgraph` (the program's `call_graph`)
// that are eligible for the private-pool model.
NameSet determine_eligibility(const tac_ast::Program& prog, const Abi& abi,
                              const CallGraph& callgraph);

// Returns the reverse of `callgraph`, each callee mapped to its callers.
CallGraph invert(const CallGraph& callgraph);

}  // namespace zp_local_allocation

}  // namespace c6502::passes
