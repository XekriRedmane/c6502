// compile.py's code generation stage from the type-checked c99 program,
// as `_run_stage`, `_codegen`, `optimized_asm`, and the simulator harness
// run it.
//
// `plan_zero_page` comes first. It places the zero-page objects and the
// runtime's block, and when the block has to move, it sizes HARGS by
// lowering the unoptimized TAC with `tac_to_asm` and measuring the
// result. Python runs that lowering before `zero_page_in_effect` sets
// the plan for the compile, so it sees the classic layout and no user
// zero-page globals, and so does the measurement here. The plan's
// layout and objects then make the `AsmContext` of the rest of the
// compile, as `zero_page_in_effect` sets `runtime_layout.current()` and
// `asm_aliasing.set_user_zp_globals` for it.
//
// Python plans the zero page before the TAC optimizer runs and reads the
// symbol table at that point, and this stage reads it after the
// optimizer. The optimizer adds locals, the halves of the tables of row
// addresses, and the temporaries of its passes, and gives a split table
// a type of bytes, none of which `allocate_zp_globals` places or
// promotes (it places `zeropage` objects and promotes scalars with
// weight in the unoptimized TAC), and the order of the names it had
// stays the same. The lowering that measures HARGS adds no symbol
// without an ABI. So the plan comes out the same.
#include "passes/codegen_stage.h"

#include <algorithm>
#include <memory>
#include <stdexcept>
#include <utility>
#include <variant>

#include "errors.h"
#include "passes/abi_selection.h"
#include "passes/optimized_front.h"
#include "passes/replace_pseudoregisters.h"
#include "passes/slot_sharing.h"
#include "passes/translate_for_codegen.h"
#include "passes/zp_link_metadata.h"
#include "tac_to_asm.h"

namespace c6502::passes {

namespace {

namespace c99 = ::c6502::c99_ast;

// Returns `{**first, **second}` as a list of pairs in the dict's order.
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

// Returns every name the unit binds with an `EQU` line, the runtime's
// SSP, FP, HARGS, and DPTR first (`ZpGlobals.equs`).
OrderedSymbolAddrs zp_equs(const ZpGlobals& zp) {
  OrderedSymbolAddrs out = {{"SSP", zp.layout.ssp},
                            {"FP", zp.layout.fp},
                            {"HARGS", zp.layout.hargs},
                            {"DPTR", zp.layout.dptr}};
  out = merged(std::move(out), pairs_of(zp.addrs));
  return merged(std::move(out), pairs_of(zp.fixed));
}

// Returns the context of the compile that `zero_page_in_effect(zp)`
// sets: the plan's runtime layout, and its objects as the user
// zero-page globals with the bytes it reserves.
AsmContext context_of(const ZpGlobals& zp) {
  AsmContext ctx;
  ctx.layout = zp.layout;
  auto user = std::make_shared<UserZpGlobals>();
  user->addrs = map_of(zp.addrs);
  user->bytes = zp.reserved;
  ctx.user_zp = std::move(user);
  return ctx;
}

// Returns `plan_zero_page(tac, symbols, types, promote=promote)`, whose
// measurement of HARGS lowers `tac` in the classic context.
ZpGlobals plan(const tac_ast::Program& tac, StageTables& tables, bool promote) {
  const HargsNeed need = [&]() -> asdl::Int {
    const asm_ast::Program lowered =
        tac_to_asm::translate_program(tac, &tables.symbols, &tables.types, AsmContext{});
    return hargs_need(lowered, tables.footprints);
  };
  return plan_zero_page(tac, tables.symbols, &tables.types, promote, need);
}

// Returns whether an object of type `t` is read-only: `t` is `const`, or
// an array of `const` elements (`sections._is_const`).
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

// Returns what `sections.items` reads of the symbol of each top-level
// name of `prog` that has one (`sections.section_info`).
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

// Returns the names whose `FunAttr.carry_return` is set.
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

// Returns every name of static storage in `symbols`, which
// `replace_pseudoregisters` and the asm optimizer take as
// `extra_statics`, so that the pseudos of the externs the asm program
// doesn't otherwise know about aren't mistaken for locals
// (`compile._static_names`).
NameSet static_names(const SymbolTable& symbols) {
  NameSet out;
  for (const auto& [name, sym] : symbols) {
    if (std::holds_alternative<StaticAttr>(sym.attrs)) {
      out.insert(name);
    }
  }
  return out;
}

// Returns the changes that Python enters in its symbol table: the names
// added after the first `before` entries, each a `<param>.reg` local,
// and the functions whose carry-flag return wasn't set in `carry_before`.
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

// Returns `abi` as the region's `AbiLayouts`: each function's
// `ZpLayout`, or nothing for the soft stack.
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
    // The link metadata's records that read the TAC, the ABI, and the
    // symbol table, as `_cpp_optimized_text` built them in Python.
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
  // Calls to a zp_abi routine with a fixed address pass their arguments
  // in registers here too, since the routine's convention can't depend
  // on the pipeline.
  const OrderedMap<std::string, ZpLayout> fixed = fixed_address_layouts(tac, tables.c99, &tables.types);
  Abi fixed_abi;
  for (const auto& [name, layout] : fixed) {
    fixed_abi.insert_or_assign(name, layout);
  }
  const tac_to_asm::Options lowering{.abi = &fixed_abi};
  asm_ast::Program asm0 =
      tac_to_asm::translate_program(tac, &tables.symbols, &tables.types, ctx, lowering);
  asm_ast::Program asm1 =
      replace_program(std::move(asm0), tables.statics, &tables.symbols, &tables.types);
  const OrderedSymbolAddrs equs = merged(pairs_of(pinned_slot_symbols(fixed_abi)), zp_equs(zp));
  StageResult out;
  if (emit) {
    EmitArgs args;
    if (link_metadata) {
      // The routines with fixed addresses whose parameters the unit
      // passes in zero page (`compile._extern_records`), by name.
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
  // `_codegen` names the statics before the TAC passes add the halves
  // of the tables of row addresses.
  tables.statics = static_names(tables.symbols);
  StageResult out;
  if (!options.optimize) {
    out = unoptimized_stage(tac, tables, options.link_metadata, options.emit);
    out.tac = std::move(tac);
    return out;
  }
  // `plan_zero_page` reads the TAC from before the TAC passes.
  const tac_ast::Program unoptimized = tac;
  tac = optimized_tac(std::move(tac), tables.symbols, tables.types);
  out = optimized_stage(unoptimized, tac, tables, options.emit);
  out.tac = std::move(tac);
  return out;
}

}  // namespace c6502::passes
