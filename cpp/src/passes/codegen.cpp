// The end of compile.py's `_codegen`, from the back end's region to the
// `-S` text.
//
// Under `-O` compile.py runs `optimized_asm`, whose last steps after the
// region list the zero-page objects for the link metadata, drop their
// storage for `__zp_init`, and check the shared parameter slots. Then
// `_codegen` drops the EQU lines of the body-local slots no instruction
// names, lowers the program to asm2, completes the link metadata with
// what the finished program gives (its items, its zero-page objects, and
// its HARGS need), and writes the text. Without `-O` the same steps run
// after the unoptimized fixed point, with the link metadata only for
// `-c`. The steps here run in that order with the arguments Python gives
// them, and each throws where its Python function raises, so a program
// that fails fails with the same message.
#include "passes/codegen.h"

#include <utility>

#include "asm_emit.h"
#include "passes/asm_to_asm2.h"
#include "passes/slot_sharing.h"

namespace c6502::passes {

namespace {

// Returns `{**first, **second}`: the keys of `first` and then the new
// keys of `second`, each with the value of the last dict that has it.
OrderedSymbolAddrs merged(OrderedSymbolAddrs first, const OrderedSymbolAddrs& second) {
  for (const auto& [name, addr] : second) {
    bool found = false;
    for (auto& entry : first) {
      if (entry.first == name) {
        entry.second = addr;
        found = true;
        break;
      }
    }
    if (!found) {
      first.emplace_back(name, addr);
    }
  }
  return first;
}

}  // namespace

OptimizedProgram optimized_program(asm_ast::Program prog, const AsmContext& ctx,
                                   const RegionArgs& region, const ZpGlobals& zp,
                                   const HelperFootprints& footprints) {
  RegionResult r = optimized_region(std::move(prog), ctx, region);
  OptimizedProgram out;
  out.coalesced_params = std::move(r.coalesced_params);
  out.new_functions = std::move(r.new_functions);
  // Zero-page globals: their records for the link metadata, then their
  // storage dropped and `__zp_init` added (passes/zp_globals.py).
  out.zp_records = link_records(r.prog, zp);
  out.prog = place_zp_globals(std::move(r.prog), zp, footprints);
  check_shares(out.prog, region.shares);
  return out;
}

EmitResult optimized_text(asm_ast::Program prog, const AsmContext& ctx, const RegionArgs& region,
                          const EmitArgs& args) {
  OptimizedProgram r = optimized_program(std::move(prog), ctx, region, args.zp, args.footprints);
  EmitResult out;
  out.new_functions = r.new_functions;
  std::vector<ZpObjectMeta> zp_records = std::move(r.zp_records);
  out.asm_prog = std::move(r.prog);
  // The `__local_*` slots that the peepholes left unread lose their EQU
  // lines. The bytes stay reserved by each function's `local_bytes`.
  const OrderedSymbolAddrs emit_slot_symbols = prune_unused_locals(out.asm_prog, args.slot_symbols);
  out.asm2_prog = lower_to_asm2(out.asm_prog);
  LinkMetadata meta = args.meta.value_or(LinkMetadata{});
  // The parameter slots each function assigns are those its TAC assigns
  // (`changed_param_slots`, which Python passes in each `def`) and those
  // `slot_coalescing` made it write (`compile._with_coalesced`).
  for (auto& def : meta.defs) {
    for (const auto& [fn, slots] : r.coalesced_params) {
      if (fn != def.name) {
        continue;
      }
      const std::vector<std::string> before = def.changes;
      for (const auto& s : slots) {
        bool seen = false;
        for (const auto& b : before) {
          if (b == s) {
            seen = true;
            break;
          }
        }
        if (!seen) {
          def.changes.push_back(s);
        }
      }
    }
  }
  meta.zp_objects = std::move(zp_records);
  meta.hargs_need = hargs_need(out.asm_prog, args.footprints);
  // Each function `apply_shared_tails` and `apply_outline_runs` made has
  // a copy of its owner's symbol, so its section is its owner's.
  SectionInfos sections = args.sections;
  for (const auto& made : r.new_functions) {
    if (const auto it = sections.find(made.owner); it != sections.end()) {
      SectionInfo info = it->second;
      sections[made.name] = std::move(info);
    }
  }
  meta.items = section_items(out.asm_prog, sections);
  asm_emit::EmitOptions options;
  options.zp_slot_symbols = merged(emit_slot_symbols, args.equs);
  options.link_metadata_lines = format_metadata(meta);
  options.preferred_symbols = args.preferred_symbols;
  out.text = asm_emit::emit_program(out.asm2_prog, ctx.layout, options);
  return out;
}

EmitResult unoptimized_text(asm_ast::Program prog, const AsmContext& ctx,
                            const SymbolAddrs& zp_addrs, const EmitArgs& args) {
  asm_ast::Program unplaced = unoptimized_region(std::move(prog), ctx, zp_addrs);
  EmitResult out;
  out.asm_prog = place_zp_globals(unplaced, args.zp, args.footprints);
  out.asm2_prog = lower_to_asm2(out.asm_prog);
  asm_emit::EmitOptions options;
  options.zp_slot_symbols = args.equs;
  if (args.meta) {
    LinkMetadata meta = *args.meta;
    meta.items = section_items(out.asm_prog, args.sections);
    meta.zp_objects = link_records(unplaced, args.zp);
    meta.hargs_need = hargs_need(out.asm_prog, args.footprints);
    options.link_metadata_lines = format_metadata(meta);
  }
  out.text = asm_emit::emit_program(out.asm2_prog, ctx.layout, options);
  return out;
}

}  // namespace c6502::passes
