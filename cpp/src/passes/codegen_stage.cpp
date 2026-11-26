#include "passes/codegen_stage.h"

#include <algorithm>
#include <memory>
#include <stdexcept>
#include <utility>
#include <variant>

#include "errors.h"
#include "passes/abi_selection.h"
#include "passes/ir_check.h"
#include "passes/optimized_front.h"
#include "passes/replace_pseudoregisters.h"
#include "passes/slot_sharing.h"
#include "passes/translate_for_codegen.h"
#include "passes/zp_link_metadata.h"
#include "tac_to_asm.h"

namespace c6502::passes {

namespace {

namespace c99 = ::c6502::c99_ast;

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

OrderedSymbolAddrs pairs_of(const OrderedMap<std::string, asdl::Int>& map) {
  return OrderedSymbolAddrs(map.begin(), map.end());
}

SymbolAddrs map_of(const OrderedMap<std::string, asdl::Int>& map) {
  return SymbolAddrs(map.begin(), map.end());
}

OrderedSymbolAddrs zp_equs(const ZpGlobals& zp) {
  OrderedSymbolAddrs out = {{"SSP", zp.layout.ssp},
                            {"FP", zp.layout.fp},
                            {"HARGS", zp.layout.hargs},
                            {"DPTR", zp.layout.dptr}};
  out = merged(std::move(out), pairs_of(zp.addrs));
  return merged(std::move(out), pairs_of(zp.fixed));
}

AsmContext context_of(const ZpGlobals& zp) {
  AsmContext ctx;
  ctx.layout = zp.layout;
  auto user = std::make_shared<UserZpGlobals>();
  user->addrs = map_of(zp.addrs);
  user->bytes = zp.reserved;
  ctx.user_zp = std::move(user);
  return ctx;
}

ZpGlobals plan(const tac_ast::Program& tac, StageTables& tables, bool promote) {
  const HargsNeed need = [&]() -> asdl::Int {
    const asm_ast::Program lowered =
        tac_to_asm::translate_program(tac, &tables.symbols, &tables.types, AsmContext{});
    return hargs_need(lowered, tables.footprints);
  };
  return plan_zero_page(tac, tables.symbols, &tables.types, promote, need);
}

bool is_const(const c99::Type_data_type& t) {
  const c99::Type_data_type* at = &t;
  while (true) {
    if (asdl::isa<c99::Const>(*at)) {
      return true;
    }
    if (const auto* v = asdl::as<c99::Volatile>(*at)) {
      at = &*v->referenced_type;
    } else if (const auto* a = asdl::as<c99::Array>(*at)) {
      at = &*a->element_type;
    } else {
      return false;
    }
  }
}

SectionInfos section_info(const asm_ast::Program& prog, const SymbolTable& symbols) {
  SectionInfos out;
  for (const auto& tl : prog.top_level) {
    const std::string& name = asm_ast::kind(tl) == asm_ast::Kind_top_level::Function
                                  ? std::get<asm_ast::Function>(tl).name
                                  : std::get<asm_ast::StaticVariable>(tl).name;
    const Symbol* sym = symbols.find(name);
    if (sym == nullptr) {
      continue;
    }
    SectionInfo info;
    if (const auto* fun = std::get_if<FunAttr>(&sym->attrs)) {
      info.section = fun->section;
    } else if (const auto* st = std::get_if<StaticAttr>(&sym->attrs)) {
      info.section = st->section;
      info.merge = st->mergeable;
    }
    info.is_const = is_const(sym->type);
    out[name] = std::move(info);
  }
  return out;
}

NameSet carry_functions(const SymbolTable& symbols) {
  NameSet out;
  for (const auto& [name, sym] : symbols) {
    const auto* fun = std::get_if<FunAttr>(&sym.attrs);
    if (fun != nullptr && fun->carry_return) {
      out.insert(name);
    }
  }
  return out;
}

NameSet static_names(const SymbolTable& symbols) {
  NameSet out;
  for (const auto& [name, sym] : symbols) {
    if (std::holds_alternative<StaticAttr>(sym.attrs)) {
      out.insert(name);
    }
  }
  return out;
}

SymbolChanges symbol_changes(const SymbolTable& symbols, std::size_t before,
                             const NameSet& carry_before) {
  SymbolChanges out;
  std::size_t k = 0;
  for (const auto& [name, sym] : symbols) {
    if (k++ < before) {
      continue;
    }
    constexpr std::string_view kSuffix = ".reg";
    if (name.size() <= kSuffix.size() ||
        name.compare(name.size() - kSuffix.size(), kSuffix.size(), kSuffix) != 0 ||
        !std::holds_alternative<LocalAttr>(sym.attrs)) {
      throw InternalError("the stage added the symbol " + name +
                          ", which isn't a register parameter's local");
    }
    out.reg_locals.push_back(name);
  }
  for (const auto& name : carry_functions(symbols)) {
    if (!carry_before.contains(name)) {
      out.carry.push_back(name);
    }
  }
  return out;
}

AbiLayouts abi_layouts(const Abi& abi) {
  AbiLayouts out;
  for (const auto& [name, layout] : abi) {
    const auto* zp = std::get_if<ZpLayout>(&layout);
    out.emplace_back(name, zp != nullptr ? std::optional<ZpLayout>(*zp) : std::nullopt);
  }
  return out;
}

}  // namespace

StageResult optimized_stage(const tac_ast::Program& unoptimized_tac, const tac_ast::Program& tac,
                            StageTables& tables, bool emit) {
  const std::size_t symbols_before = tables.symbols.size();
  const NameSet carry_before = carry_functions(tables.symbols);
  const ZpGlobals zp = plan(unoptimized_tac, tables, /*promote=*/true);
  const AsmContext ctx = context_of(zp);
  OptimizedFront front = optimized_front(tac, tables.c99, tables.symbols, tables.types, zp,
                                         tables.statics, ctx);
  RegionArgs region;
  region.slot_symbols = map_of(front.slot_symbols);
  region.zp_addrs = map_of(zp.addrs);
  region.noreturn = front.noreturn;
  region.abi = abi_layouts(front.abi);
  region.shares = front.shares;
  region.ret_eligible = front.ret_eligible;
  region.zp_names = front.zp_names;
  region.slot_addrs = front.slot_addrs;
  StageResult out;
  if (emit) {
    const OrderedMap<std::string, std::vector<std::string>> changes =
        changed_param_slots(tac, front.abi, tables.symbols, &tables.types);
    const NameSet carry = carry_functions(tables.symbols);
    EmitArgs args;
    args.meta = build_metadata(tac, front.abi, front.local_pools, &front.slot_names_by_fn,
                               &changes, &carry);
    args.meta->shares = front.shares;
    args.meta->hargs_size = zp.layout.hargs_size;
    args.zp = zp;
    args.footprints = tables.footprints;
    args.sections = section_info(front.prog, tables.symbols);
    args.slot_symbols = pairs_of(front.slot_symbols);
    args.equs = zp_equs(zp);
    for (const auto& [fn, names] : preferred_symbols(front.bindings)) {
      args.preferred_symbols[fn] = names;
    }
    out.emitted = optimized_text(std::move(front.prog), ctx, region, args);
  } else {
    OptimizedProgram finished =
        optimized_program(std::move(front.prog), ctx, region, zp, tables.footprints);
    out.emitted.asm_prog = std::move(finished.prog);
    out.emitted.new_functions = std::move(finished.new_functions);
    out.harness_symbols = merged(pairs_of(front.slot_symbols), zp_equs(zp));
  }
  out.changes = symbol_changes(tables.symbols, symbols_before, carry_before);
  out.changes.new_functions = out.emitted.new_functions;
  return out;
}

StageResult unoptimized_stage(const tac_ast::Program& tac, StageTables& tables,
                              bool link_metadata, bool emit) {
  const std::size_t symbols_before = tables.symbols.size();
  const NameSet carry_before = carry_functions(tables.symbols);
  const ZpGlobals zp = plan(tac, tables, /*promote=*/false);
  const AsmContext ctx = context_of(zp);
  const OrderedMap<std::string, ZpLayout> fixed = fixed_address_layouts(tac, tables.c99, &tables.types);
  Abi fixed_abi;
  for (const auto& [name, layout] : fixed) {
    fixed_abi.insert_or_assign(name, layout);
  }
  const tac_to_asm::Options lowering{.abi = &fixed_abi};
  asm_ast::Program asm0 =
      tac_to_asm::translate_program(tac, &tables.symbols, &tables.types, ctx, lowering);
  if (emit) {
    ir_check::check_asm(asm0, "tac_to_asm", /*pseudos=*/true);
  }
  asm_ast::Program asm1 =
      replace_program(std::move(asm0), tables.statics, &tables.symbols, &tables.types);
  const OrderedSymbolAddrs equs = merged(pairs_of(pinned_slot_symbols(fixed_abi)), zp_equs(zp));
  StageResult out;
  if (emit) {
    EmitArgs args;
    if (link_metadata) {
      std::vector<std::pair<std::string, const ZpLayout*>> sorted;
      for (const auto& [name, layout] : fixed) {
        sorted.emplace_back(name, &layout);
      }
      std::sort(sorted.begin(), sorted.end(),
                [](const auto& a, const auto& b) { return a.first < b.first; });
      LinkMetadata meta;
      for (const auto& [name, layout] : sorted) {
        ExternMeta ext;
        ext.name = name;
        ext.params = layout->slot_symbols;
        for (const auto& reg : layout->param_registers) {
          ext.param_regs.push_back(reg.value_or(""));
        }
        ext.pinned = layout->pinned;
        meta.externs.push_back(std::move(ext));
      }
      meta.hargs_size = zp.layout.hargs_size;
      args.meta = std::move(meta);
    }
    args.zp = zp;
    args.footprints = tables.footprints;
    args.sections = section_info(asm1, tables.symbols);
    args.equs = equs;
    out.emitted = unoptimized_text(std::move(asm1), ctx, map_of(zp.addrs), args);
  } else {
    asm_ast::Program tail = unoptimized_region(std::move(asm1), ctx, map_of(zp.addrs));
    out.emitted.asm_prog = place_zp_globals(std::move(tail), zp, tables.footprints);
    out.harness_symbols = equs;
  }
  out.changes = symbol_changes(tables.symbols, symbols_before, carry_before);
  return out;
}

StageResult codegen(StageTables& tables, const CodegenOptions& options) {
  tac_ast::Program tac = translate_for_codegen(tables.c99, tables.symbols, tables.types,
                                               options.optimize, options.inline_calls);
  tables.statics = static_names(tables.symbols);
  StageResult out;
  if (!options.optimize) {
    out = unoptimized_stage(tac, tables, options.link_metadata, options.emit);
    out.tac = std::move(tac);
    return out;
  }
  const tac_ast::Program unoptimized = tac;
  tac = optimized_tac(std::move(tac), tables.symbols, tables.types);
  out = optimized_stage(unoptimized, tac, tables, options.emit);
  out.tac = std::move(tac);
  return out;
}

}  // namespace c6502::passes
