// The port of the asm back end's tail in compile.py: the region of
// `optimized_asm` from `apply_licm` through `apply_outline_runs`, and
// the unoptimized pipeline's peephole fixed point and long branches.
#pragma once

#include <set>
#include <vector>

#include "asm_ast.h"
#include "passes/asm_context.h"
#include "passes/shared_tails.h"
#include "passes/slot_coalescing.h"

namespace c6502::passes {

// The values compile.py computes for the region, beside the program
// and the compile's context.
struct RegionArgs {
  // `all_slot_symbols`: every zp_abi parameter slot and body-local slot
  // of the unit, with its address.
  SymbolAddrs slot_symbols;
  // `zp.addrs`: the user zero-page globals, with their addresses.
  SymbolAddrs zp_addrs;
  // `noreturn`: the functions that never return.
  NameSet noreturn;
  // What `apply_slot_coalescing` reads of `abi` and `shares`.
  AbiLayouts abi;
  std::vector<SlotShare> shares;
  // `_ret_coalescing_eligible(tac, pooled, recursion)`: the functions
  // `apply_ret_slot_coalescing` may change.
  NameSet ret_eligible;
  // The names that are zero-page bytes, and the addresses of the
  // slots in zero page, for `apply_shared_tails` and
  // `apply_outline_runs`.
  NameSet zp_names;
  std::set<asdl::Int> slot_addrs;
};

// What the region returns besides the program.
struct RegionResult {
  asm_ast::Program prog;
  // The slots `apply_slot_coalescing` made each function write, which
  // compile.py adds to the link metadata (`coalesced_params`).
  WrittenSlots coalesced_params;
  // The functions `apply_shared_tails` and `apply_outline_runs` made,
  // in the order they made them, each with the function whose symbol
  // compile.py copies for it.
  std::vector<NewFunction> new_functions;
};

// Runs the passes of compile.py's `optimized_asm` from `apply_licm`
// through `apply_outline_runs` on `prog`, the program after
// `synthesize_prologue`, with the arguments Python gives each pass.
RegionResult optimized_region(asm_ast::Program prog, const AsmContext& ctx,
                              const RegionArgs& args);

// Runs the unoptimized pipeline's `_peephole_fixedpoint(asm1,
// zp_global_symbols=zp.addrs)` and `expand_long_branches` on `prog`.
asm_ast::Program unoptimized_region(asm_ast::Program prog, const AsmContext& ctx,
                                    const SymbolAddrs& zp_global_symbols);

}  // namespace c6502::passes
