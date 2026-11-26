#pragma once

#include <map>
#include <optional>
#include <string>
#include <vector>

#include "asm2_ast.h"
#include "asm_ast.h"
#include "passes/asm_context.h"
#include "passes/optimized_asm.h"
#include "passes/prune_unused_slots.h"
#include "passes/sections.h"
#include "passes/shared_tails.h"
#include "passes/zp_globals.h"
#include "passes/zp_link_metadata.h"

namespace c6502::passes {

// The values the work after the region reads, besides the region's own
// arguments.
struct EmitArgs {
  ZpGlobals zp;
  HelperFootprints footprints;
  SectionInfos sections;
  // Under `-O`, the slot symbols of `optimized_front` in order, which
  // `prune_unused_locals` filters for the EQU block.
  OrderedSymbolAddrs slot_symbols;
  // The symbols the EQU block binds after the slots: the unit's EQU
  // bindings under `-O`, and without it the whole block, the pinned
  // parameter slots and then the unit's EQU bindings.
  OrderedSymbolAddrs equs;
  // The link metadata that `build_metadata` (under `-O`) or the
  // `ExternMeta` records (with `-c`) give, without the items, the
  // zero-page records, and the HARGS need, which come from the finished
  // program. Without it the output has no metadata block.
  std::optional<LinkMetadata> meta;
  std::map<std::string, NameSet, std::less<>> preferred_symbols;
};

// What a run gives: the text, the functions the region made, and the
// finished asm and asm2 programs.
struct EmitResult {
  std::string text;
  std::vector<NewFunction> new_functions;
  asm_ast::Program asm_prog;
  asm2_ast::Program asm2_prog;
};

// What `optimized_program` gives: the finished asm program, what the
// region returns besides it, and the link metadata's records of the
// zero-page objects, which `link_records` takes from the program before
// `place_zp_globals`.
struct OptimizedProgram {
  asm_ast::Program prog;
  WrittenSlots coalesced_params;
  std::vector<NewFunction> new_functions;
  std::vector<ZpObjectMeta> zp_records;
};

// Runs the end of `optimized_asm` on `prog`, the program after
// `synthesize_prologue`: the region, `link_records`, `place_zp_globals`,
// and `check_shares`. The simulator harness assembles the program it
// gives.
OptimizedProgram optimized_program(asm_ast::Program prog, const AsmContext& ctx,
                                   const RegionArgs& region, const ZpGlobals& zp,
                                   const HelperFootprints& footprints);

// Runs the rest of code generation under `-O` on `prog`, the program after
// `synthesize_prologue`: `optimized_program`, then `prune_unused_locals`,
// `lower_to_asm2`, the link metadata, and `emit_program`.
EmitResult optimized_text(asm_ast::Program prog, const AsmContext& ctx, const RegionArgs& region,
                          const EmitArgs& args);

// Runs the rest of code generation without `-O` on `prog`, the program
// after `replace_pseudoregisters`: the unoptimized fixed point and long
// branches (`unoptimized_region`), `place_zp_globals`, `lower_to_asm2`,
// the link metadata when `args.meta` is given, and `emit_program`.
EmitResult unoptimized_text(asm_ast::Program prog, const AsmContext& ctx,
                            const SymbolAddrs& zp_addrs, const EmitArgs& args);

}  // namespace c6502::passes
