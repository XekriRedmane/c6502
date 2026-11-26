#include "passes/address_taken_zp.h"

#include <set>
#include <vector>

#include "errors.h"
#include "passes/replace_pseudoregisters.h"
#include "passes/zp_slot_naming.h"

namespace c6502::passes {

namespace ir = ::c6502::asm_ast;

namespace {

NameSet statics_set(const SymbolTable& symbols) {
  NameSet out;
  for (const auto& [name, sym] : symbols) {
    if (std::holds_alternative<StaticAttr>(sym.attrs)) {
      out.insert(name);
    }
  }
  return out;
}

std::set<asdl::Int> coloring_used_bytes(const Coloring* coloring, const SymbolTable* symbols,
                                        const TypeTable* types) {
  std::set<asdl::Int> out;
  if (coloring == nullptr || coloring->assignments.empty()) {
    return out;
  }
  for (const auto& [name, addr] : coloring->assignments) {
    const asdl::Int size = size_of_name(name, symbols, types);
    for (asdl::Int k = 0; k < size; ++k) {
      out.insert(addr + k);
    }
  }
  return out;
}

std::vector<std::string> address_taken_names(const ir::Function& fn, const NameSet& statics) {
  std::set<std::string, std::less<>> seen;
  std::vector<std::string> out;
  const std::set<std::string, std::less<>> param_set(fn.params.begin(), fn.params.end());
  for (const auto& instr : fn.instructions) {
    const auto* la = asdl::as<ir::LoadAddress>(instr);
    if (la == nullptr) {
      continue;
    }
    const auto* pseudo = asdl::as<ir::Pseudo>(la->src);
    if (pseudo == nullptr) {
      continue;
    }
    const std::string& name = pseudo->name;
    if (param_set.contains(name) || statics.contains(name) || seen.contains(name)) {
      continue;
    }
    seen.insert(name);
    out.push_back(name);
  }
  return out;
}

OrderedMap<std::string, asdl::Int> allocate_in_pool(const ir::Function& fn,
                                                    const std::vector<asdl::Int>& pool,
                                                    const std::set<asdl::Int>& used,
                                                    const NameSet& statics,
                                                    const SymbolTable* symbols,
                                                    const TypeTable* types) {
  OrderedMap<std::string, asdl::Int> out;
  const auto candidates = address_taken_names(fn, statics);
  if (candidates.empty()) {
    return out;
  }
  const std::set<asdl::Int> pool_set(pool.begin(), pool.end());
  std::vector<std::vector<asdl::Int>> runs;
  for (const asdl::Int& a : pool_set) {
    if (used.contains(a)) {
      continue;
    }
    if (!runs.empty() && runs.back().back() + 1 == a) {
      runs.back().push_back(a);
    } else {
      runs.push_back({a});
    }
  }
  for (const auto& name : candidates) {
    const asdl::Int size = size_of_name(name, symbols, types);
    if (size < 1) {
      throw InternalError("address_taken_zp: the address-taken local '" + name +
                          "' has no bytes");
    }
    for (auto& run : runs) {
      if (static_cast<asdl::Int>(run.size()) >= size) {
        out.insert_or_assign(name, run.front());
        run.erase(run.begin(), run.begin() + static_cast<std::ptrdiff_t>(size));
        break;
      }
    }
  }
  return out;
}

void check_no_frame_under_y(const ir::Function& fn, const OrderedMap<std::string, asdl::Int>& placed,
                            const Coloring* coloring, const NameSet& statics) {
  if (coloring == nullptr) {
    return;
  }
  bool y_used = false;
  for (const auto& [name, letter] : coloring->hwreg_assignments) {
    y_used = y_used || letter == "Y";
  }
  if (!y_used) {
    return;
  }
  std::string missing;
  for (const auto& name : address_taken_names(fn, statics)) {
    if (placed.find(name) == nullptr) {
      missing += (missing.empty() ? "'" : ", '") + name + "'";
    }
  }
  if (!missing.empty()) {
    throw InternalError("function '" + fn.name + "': the address-taken locals " + missing +
                        " didn't fit in the function's pool, and the coloring keeps a value "
                        "in Y, which an access to their frame slots would overwrite");
  }
}

}  // namespace

AddressTakenAssignments compute_address_taken_assignments(const ir::Program& prog,
                                                          const LocalPools& local_pools,
                                                          const ColoringsByFunction& colorings,
                                                          const SymbolTable* symbols,
                                                          const TypeTable* types) {
  AddressTakenAssignments out;
  const NameSet statics = symbols != nullptr ? statics_set(*symbols) : NameSet{};
  for (const auto& tl : prog.top_level) {
    const auto* fn = asdl::as<ir::Function>(tl);
    if (fn == nullptr) {
      continue;
    }
    const auto* pool = local_pools.find(fn->name);
    if (pool == nullptr) {
      out.insert_or_assign(fn->name, {});
      continue;
    }
    const Coloring* coloring = colorings.find(fn->name);
    const auto used = coloring_used_bytes(coloring, symbols, types);
    auto placed = allocate_in_pool(*fn, *pool, used, statics, symbols, types);
    check_no_frame_under_y(*fn, placed, coloring, statics);
    out.insert_or_assign(fn->name, std::move(placed));
  }
  return out;
}

namespace address_taken_zp {

OrderedMap<std::string, std::string> slot_symbols(std::string_view fn_name,
                                                  const OrderedMap<std::string, asdl::Int>& assignments,
                                                  const SymbolTable*, const TypeTable*) {
  OrderedMap<std::string, std::string> out;
  OrderedMap<std::string, asdl::Int> used;
  const std::string prefix = "__local_" + std::string(fn_name) + "__";
  for (const auto& [pname, addr] : assignments) {
    const auto [source, byte_index] = parse_pseudo_name(pname);
    if (!source) {
      out.insert_or_assign(pname, prefix + "addr_" + pname);
      continue;
    }
    std::string name = prefix + *source;
    asdl::Int& count = used[name];
    count += 1;
    if (count > 1) {
      name += "@" + asdl::int_to_string(count);
    }
    out.insert_or_assign(pname, std::move(name));
  }
  return out;
}

}  // namespace address_taken_zp

}  // namespace c6502::passes
