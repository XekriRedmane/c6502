// The end of compile.py's `_codegen` in C++: the back end's region, the
// work between the region and emission, `asm_to_asm2`, and `asm_emit`,
// in one call, which gives the `-S` text.
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

// The values Python computes for the work after the region, besides the
// region's own arguments.
struct EmitArgs {
  // What the work reads of the unit's `ZpGlobals`.
  ZpGlobals zp;
  // `runtime_layout.helper_footprints()`.
  HelperFootprints footprints;
  // What `sections.items` reads of the symbol of each top-level name of
  // the program given (`sections.section_info`).
  SectionInfos sections;
  // Under `-O`, `all_slot_symbols` in its order, which
  // `prune_unused_locals` filters for the EQU block.
  OrderedSymbolAddrs slot_symbols;
  // The symbols the EQU block binds after the slots: `zp.equs()` under
  // `-O`, and without it `{**pinned_slot_symbols(fixed_abi),
  // **zp.equs()}`, the whole block.
  OrderedSymbolAddrs equs;
  // The link metadata that `build_metadata` (under `-O`) or the
  // `ExternMeta` records (with `-c`) give, without the items, the
  // zero-page records, and the HARGS need, which come from the finished
  // program. Without it the output has no metadata block.
  std::optional<LinkMetadata> meta;
  // `preferred_symbols(bindings)`, under `-O`.
  std::map<std::string, NameSet, std::less<>> preferred_symbols;
};

// What a run gives: the text, the functions the region made, and the
// finished asm and asm2 programs, which Python checks with
// `ir_check.check_asm` and `check_asm2` under `C6502_CHECK_IR=1`.
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

// Runs the end of `_codegen` under `-O` on `prog`, the program after
// `synthesize_prologue`: `optimized_program`, then `prune_unused_locals`,
// `lower_to_asm2`, the link metadata, and `emit_program`.
EmitResult optimized_text(asm_ast::Program prog, const AsmContext& ctx, const RegionArgs& region,
                          const EmitArgs& args);

// Runs the end of `_codegen` without `-O` on `prog`, the program after
// `replace_pseudoregisters`: the unoptimized fixed point and long
// branches (`unoptimized_tail`), `place_zp_globals`, `lower_to_asm2`,
// the link metadata when `args.meta` is given, and `emit_program`.
EmitResult unoptimized_text(asm_ast::Program prog, const AsmContext& ctx,
                            const SymbolAddrs& zp_addrs, const EmitArgs& args);

}  // namespace c6502::passes
