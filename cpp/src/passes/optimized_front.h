// The `-O` pipeline from the optimized TAC, `select_abi` through
// `synthesize_prologue`, which ends where the back end's region
// (`optimized_region`) starts.
#pragma once

#include <set>
#include <string>
#include <vector>

#include "asdl/runtime.h"
#include "asm_ast.h"
#include "c99_ast.h"
#include "ordered_map.h"
#include "passes/abi_selection.h"
#include "passes/asm_context.h"
#include "passes/slot_sharing.h"
#include "passes/type_checking.h"
#include "passes/zp_globals.h"
#include "passes/zp_local_allocation.h"
#include "passes/zp_slot_naming.h"
#include "tac_ast.h"

namespace c6502::passes {

// What `optimized_front` returns: the program after
// `synthesize_prologue`, and the values computed for it that the region
// and the link metadata read.
struct OptimizedFront {
  asm_ast::Program prog;
  // The ABI with the addresses `allocate_zp_slots` gave the slots.
  Abi abi;
  LocalPools local_pools;
  SlotNamesByFunction slot_names_by_fn;
  // The zp_abi parameter slots and then the pools' bytes, each with its
  // address.
  OrderedMap<std::string, asdl::Int> slot_symbols;
  std::vector<SlotShare> shares;
  Bindings bindings;
  // The functions that never return, for memory value propagation's
  // call summaries.
  NameSet noreturn;
  // The functions `ret_slot_coalescing` may change.
  NameSet ret_eligible;
  // The names that are zero-page bytes, and the addresses of the slots
  // in zero page, for `apply_shared_tails` and `apply_outline_runs`.
  NameSet zp_names;
  std::set<asdl::Int> slot_addrs;
};

// Runs the pipeline on `tac`, the TAC program after
// `apply_split_tables`. `c99` is the type-checked program `select_abi`
// reads, `zp` the unit's zero-page plan, whose layout and objects `ctx`
// holds, and `statics` every name of static storage in the symbol table
// before the TAC optimizer ran. `symbols` gets the carry-flag returns
// the two choices make and the `<param>.reg` locals of `tac_to_asm`.
OptimizedFront optimized_front(const tac_ast::Program& tac, const c99_ast::Program& c99,
                               SymbolTable& symbols, const TypeTable& types, const ZpGlobals& zp,
                               const NameSet& statics, const AsmContext& ctx);

}  // namespace c6502::passes
