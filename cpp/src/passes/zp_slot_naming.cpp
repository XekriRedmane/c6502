#include "passes/zp_slot_naming.h"

#include <algorithm>

#include "errors.h"
#include "passes/local_slot_names.h"

namespace c6502::passes {

namespace {

constexpr std::string_view kRegisterParamSuffix = ".reg";

bool all_digits(std::string_view s) {
  return !s.empty() && std::all_of(s.begin(), s.end(), [](char c) { return c >= '0' && c <= '9'; });
}

std::optional<std::pair<std::string_view, std::string_view>> rpartition(std::string_view s,
                                                                        std::string_view sep) {
  const auto at = s.rfind(sep);
  if (at == std::string_view::npos) {
    return std::nullopt;
  }
  return std::pair{s.substr(0, at), s.substr(at + sep.size())};
}

asdl::Int parse_digits(std::string_view s) {
  asdl::Int v = 0;
  for (const char c : s) {
    v = v * 10 + (c - '0');
  }
  return v;
}

}  // namespace

std::string register_param_local(std::string_view param) {
  return std::string(param) + std::string(kRegisterParamSuffix);
}

std::string source_spelling(std::string_view resolved_name) {
  if (!resolved_name.empty() && resolved_name.front() == '@') {
    const auto dot = resolved_name.find('.');
    if (dot != std::string_view::npos) {
      return std::string(resolved_name.substr(dot + 1));
    }
  }
  return std::string(resolved_name);
}

std::pair<std::optional<std::string>, asdl::Int> parse_pseudo_name(std::string_view name) {
  std::string_view base = name;
  if (const auto p = rpartition(base, ".v"); p && all_digits(p->second)) {
    base = p->first;
  }
  asdl::Int byte_index = 0;
  if (const auto p = rpartition(base, ".b"); p && all_digits(p->second)) {
    byte_index = parse_digits(p->second);
    base = p->first;
  }
  if (const auto p = rpartition(base, "."); p && all_digits(p->second)) {
    base = p->first;
  }
  if (base.size() >= kRegisterParamSuffix.size() &&
      base.substr(base.size() - kRegisterParamSuffix.size()) == kRegisterParamSuffix) {
    base = base.substr(0, base.size() - kRegisterParamSuffix.size());
  }
  if (!base.empty() && base.front() == '@') {
    const auto dot = base.find('.');
    if (dot != std::string_view::npos && dot + 1 < base.size()) {
      return {std::string(base.substr(dot + 1)), byte_index};
    }
  }
  return {std::nullopt, byte_index};
}

std::vector<std::string> param_slot_symbols(std::string_view fn_name,
                                            const std::vector<std::optional<std::string>>& param_names,
                                            const std::vector<asdl::Int>& param_sizes) {
  if (param_names.size() != param_sizes.size()) {
    throw InternalError("param_slot_symbols: the parameters' names and sizes differ in number");
  }
  std::vector<std::string> out;
  const std::string prefix = "__zpabi_" + std::string(fn_name) + "__";
  for (std::size_t index = 0; index < param_names.size(); ++index) {
    const std::string spelling =
        param_names[index] ? source_spelling(*param_names[index]) : std::to_string(index);
    const asdl::Int size = param_sizes[index];
    if (size == 1) {
      out.push_back(prefix + spelling);
      continue;
    }
    for (asdl::Int k = 0; k < size; ++k) {
      out.push_back(prefix + spelling + "_" + asdl::int_to_string(k));
    }
  }
  return out;
}

SlotNamesByFunction compute_local_slot_names(const LocalPools& local_pools,
                                             const ColoringsByFunction& colorings,
                                             const AddressTakenAssignments* assignments,
                                             const AddressTakenSymbols* symbols) {
  SlotNamesByFunction out;
  for (const auto& [fn_name, pool] : local_pools) {
    const Coloring* coloring = colorings.find(fn_name);
    OrderedMap<asdl::Int, std::string> overrides;
    if (assignments != nullptr && symbols != nullptr) {
      const auto* fn_assignments = assignments->find(fn_name);
      const auto* fn_symbols = symbols->find(fn_name);
      if (fn_assignments != nullptr) {
        for (const auto& [pname, addr] : *fn_assignments) {
          const std::string* sym = fn_symbols != nullptr ? fn_symbols->find(pname) : nullptr;
          if (sym != nullptr) {
            overrides.insert_or_assign(addr, *sym);
          }
        }
      }
    }
    std::vector<std::string> names;
    names.reserve(pool.size());
    if (coloring == nullptr) {
      for (std::size_t k = 0; k < pool.size(); ++k) {
        const std::string* sym = overrides.find(pool[k]);
        names.push_back(sym != nullptr ? *sym
                                       : "__local_" + fn_name + "__" + std::to_string(k));
      }
      out.insert_or_assign(fn_name, std::move(names));
      continue;
    }
    auto name_map = local_slot_names(fn_name, *coloring, pool);
    for (const auto& [addr, sym] : overrides) {
      name_map.insert_or_assign(addr, sym);
    }
    for (const asdl::Int& addr : pool) {
      names.push_back(name_map.at(addr));
    }
    out.insert_or_assign(fn_name, std::move(names));
  }
  return out;
}

LocalSlots local_slots(const LocalPools& local_pools, const ColoringsByFunction& colorings,
                       const AddressTakenAssignments* assignments,
                       const AddressTakenSymbols* symbols) {
  LocalSlots out;
  out.slot_names_by_fn = compute_local_slot_names(local_pools, colorings, assignments, symbols);
  out.slot_symbols = build_local_slot_symbols(local_pools, &out.slot_names_by_fn);
  return out;
}

}  // namespace c6502::passes
