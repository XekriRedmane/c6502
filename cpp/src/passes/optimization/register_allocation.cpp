#include "passes/optimization/register_allocation.h"

namespace c6502::passes::register_allocation {

std::set<asdl::Int> blocked_bytes(std::string_view name, const InterferenceGraph& graph,
                                  const OrderedMap<std::string, asdl::Int>& assignments) {
  std::set<asdl::Int> out;
  for (const auto& m : graph.neighbors(name)) {
    const asdl::Int* base = assignments.find(m);
    if (base == nullptr) {
      continue;
    }
    const asdl::Int w = graph.nodes.at(m).width;
    for (asdl::Int b = *base; b < *base + w; ++b) {
      out.insert(b);
    }
  }
  return out;
}

std::optional<asdl::Int> find_fit(const Range& byte_range, const asdl::Int& width,
                                  const std::set<asdl::Int>& blocked) {
  const asdl::Int lo = byte_range.start;
  const asdl::Int hi = byte_range.stop;
  if (width <= 0 || hi - lo < width) {
    return std::nullopt;
  }
  for (asdl::Int base = lo; base < hi - width + 1; ++base) {
    bool free = true;
    for (asdl::Int b = base; b < base + width; ++b) {
      if (blocked.contains(b)) {
        free = false;
        break;
      }
    }
    if (free) {
      return base;
    }
  }
  return std::nullopt;
}

}  // namespace c6502::passes::register_allocation
