// The names of a function's body-local pool bytes, ported from
// `local_slot_names` in passes/zp_slot_naming.py.
//
// The naming takes three steps. The first inverts
// `coloring.assignments`, so that each pool address gets the list of
// pseudos that colored to it, in the order of the assignments, which
// the register allocation makes the same on every run. The second
// sweeps every colored pseudo to find the width of each source
// variable, the largest byte index seen plus one, which decides whether
// its slot symbols take a `_<k>` byte suffix. The third walks the
// addresses in the pool's order and names each by the first pseudo on
// it that comes from the source ("first source name wins" when move
// coalescing put several variables on one byte), or by the next free
// number when every pseudo on it is a compiler temporary.
//
// Each symbol must bind one address. When the preferred source name is
// already taken, which happens when the register allocation kept two
// TAC SSA versions of one source variable on different bytes, the later
// address takes a number, so that the peepholes keyed by name don't
// treat the two cells as one.

#include "passes/local_slot_names.h"

#include <map>
#include <optional>
#include <set>
#include <utility>

#include "passes/zp_slot_naming.h"

namespace c6502::passes {

OrderedMap<asdl::Int, std::string> local_slot_names(std::string_view fn_name,
                                                    const Coloring& coloring,
                                                    const std::vector<asdl::Int>& local_pool) {
  OrderedMap<asdl::Int, std::string> name_map;
  if (local_pool.empty()) {
    return name_map;
  }
  const std::set<asdl::Int> pool_set(local_pool.begin(), local_pool.end());
  std::map<asdl::Int, std::vector<std::string>> addr_to_pseudos;
  for (const auto& [pseudo_name, addr] : coloring.assignments) {
    if (pool_set.contains(addr)) {
      addr_to_pseudos[addr].push_back(pseudo_name);
    }
  }

  // The width pass records, for every pseudo from the source on any
  // pool address, the largest byte index seen for its source name.
  std::map<std::string, asdl::Int, std::less<>> source_widths;
  for (const auto& [addr, names] : addr_to_pseudos) {
    for (const auto& pname : names) {
      const auto [source, byte_index] = parse_pseudo_name(pname);
      if (source) {
        auto [it, inserted] = source_widths.try_emplace(*source, 0);
        if (byte_index + 1 > it->second) {
          it->second = byte_index + 1;
        }
      }
    }
  }

  const std::string prefix = "__local_" + std::string(fn_name) + "__";
  asdl::Int temp_counter = 0;
  std::set<std::string, std::less<>> used_names;
  const auto next_temp_name = [&]() {
    while (true) {
      std::string candidate = prefix + asdl::int_to_string(temp_counter);
      temp_counter += 1;
      if (!used_names.contains(candidate)) {
        return candidate;
      }
    }
  };

  for (const asdl::Int& addr : local_pool) {
    std::optional<std::pair<std::string, asdl::Int>> chosen;
    if (const auto it = addr_to_pseudos.find(addr); it != addr_to_pseudos.end()) {
      for (const auto& pname : it->second) {
        auto [source, byte_index] = parse_pseudo_name(pname);
        if (source) {
          chosen.emplace(std::move(*source), byte_index);
          break;
        }
      }
    }
    std::optional<std::string> chosen_name;
    if (chosen) {
      const auto& [source, byte_index] = *chosen;
      const asdl::Int width = source_widths.find(source)->second;
      std::string candidate =
          width == 1 ? prefix + source : prefix + source + "_" + asdl::int_to_string(byte_index);
      if (!used_names.contains(candidate)) {
        chosen_name = std::move(candidate);
      }
    }
    if (!chosen_name) {
      chosen_name = next_temp_name();
    }
    used_names.insert(*chosen_name);
    name_map.insert_or_assign(addr, std::move(*chosen_name));
  }
  return name_map;
}

}  // namespace c6502::passes
