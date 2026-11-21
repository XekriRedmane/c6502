// The parts of passes/zp_local_allocation.py ported so far: which
// functions get a private zero-page pool for their body locals
// (`pool_eligible`) and which of those are on a cycle whose recursive
// calls save on the hardware stack (`recursive_pool_functions`). The
// allocation of the pools (`allocate_function_locals`) isn't ported
// yet.
#pragma once

#include "passes/abi_selection.h"
#include "passes/asm_context.h"
#include "passes/recursion_saves.h"
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

namespace zp_local_allocation {

// Returns the functions of `callgraph` (the program's `call_graph`)
// that are eligible for the private-pool model.
NameSet determine_eligibility(const tac_ast::Program& prog, const Abi& abi,
                              const CallGraph& callgraph);

// Returns the reverse of `callgraph`, each callee mapped to its callers.
CallGraph invert(const CallGraph& callgraph);

}  // namespace zp_local_allocation

}  // namespace c6502::passes
