// The helpers of passes/optimization/register_allocation.py that the
// asm register allocator (passes/optimization_asm/regalloc.cpp) reuses.
// The TAC-level allocator that once lived in that module was removed,
// and the coloring is now done only on the asm IR, which has the
// precision of bytes. The module keeps `Coloring`, the result that the
// asm allocator returns and that `apply_coloring` and
// `replace_pseudoregisters_bare_exit` read, and these two pieces of
// arithmetic of blocked bytes that take the widths into account.
#include "passes/optimization/register_allocation.h"

namespace c6502::passes::register_allocation {

std::set<asdl::Int> blocked_bytes(std::string_view name, const InterferenceGraph& graph,
                                  const OrderedMap<std::string, asdl::Int>& assignments) {
  std::set<asdl::Int> out;
  for (const auto& m : graph.neighbors(name)) {
    const asdl::Int* base = assignments.find(m);
    if (base == nullptr) {
      // A neighbor that isn't colored or was spilled places no
      // constraint, since a spilled neighbor lives in the frame and not
      // in a zero-page slot.
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
