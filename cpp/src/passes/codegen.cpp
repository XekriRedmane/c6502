#include "passes/codegen.h"

#include <utility>

#include "asm_emit.h"
#include "passes/asm_to_asm2.h"
#include "passes/ir_check.h"
#include "passes/slot_sharing.h"

namespace c6502::passes {

namespace {

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
  out.zp_records = link_records(r.prog, zp);
  out.prog = place_zp_globals(std::move(r.prog), zp, footprints);
  ir_check::check_asm(out.prog, "the -O pipeline");
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
  const OrderedSymbolAddrs emit_slot_symbols = prune_unused_locals(out.asm_prog, args.slot_symbols);
  out.asm2_prog = lower_to_asm2(out.asm_prog);
  LinkMetadata meta = args.meta.value_or(LinkMetadata{});
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
  ir_check::check_asm(out.asm_prog, "the pipeline");
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
