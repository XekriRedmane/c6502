// The tail of the asm back end: under `-O` the passes from `apply_licm`
// through `apply_outline_runs`, and without it the peephole fixed point
// and the expansion of long branches.
#pragma once

#include <set>
#include <vector>

#include "asm_ast.h"
#include "passes/asm_context.h"
#include "passes/shared_tails.h"
#include "passes/slot_coalescing.h"

namespace c6502::passes {

// The values the region reads, beside the program and the compile's
// context.
struct RegionArgs {
  // Every zp_abi parameter slot and body-local slot of the unit, with
  // its address.
  SymbolAddrs slot_symbols;
  // The user zero-page globals, with their addresses.
  SymbolAddrs zp_addrs;
  NameSet noreturn;
  // What `apply_slot_coalescing` reads of `abi` and `shares`.
  AbiLayouts abi;
  std::vector<SlotShare> shares;
  // The functions `apply_ret_slot_coalescing` may change.
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
  // codegen.cpp adds to the link metadata.
  WrittenSlots coalesced_params;
  // The functions `apply_shared_tails` and `apply_outline_runs` made,
  // in the order they made them, each with the function whose symbol
  // codegen.cpp copies for it.
  std::vector<NewFunction> new_functions;
};

// Runs the passes from `apply_licm` through `apply_outline_runs` on
// `prog`, the program after `synthesize_prologue`.
RegionResult optimized_region(asm_ast::Program prog, const AsmContext& ctx,
                              const RegionArgs& args);

// Runs the unoptimized pipeline's peephole fixed point, with the user
// zero-page globals `zp_global_symbols`, and `expand_long_branches` on
// `prog`.
asm_ast::Program unoptimized_region(asm_ast::Program prog, const AsmContext& ctx,
                                    const SymbolAddrs& zp_global_symbols);

}  // namespace c6502::passes
