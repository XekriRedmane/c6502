// The port of passes/optimization/interference.py, the interference
// graph that the asm register allocator builds, coalesces, and colors.
//
// Two names interfere when both are live at one program point. The
// graph maps each name to its node and holds an undirected adjacency.
// A node carries a `width` (1, 2, 4, or 8 bytes) and a
// `lives_across_call` flag, which the allocator reads to choose between
// the caller-saved and callee-saved halves of the pool and to size the
// contiguous slot of a value of several bytes. The builder is
// `build_interference` in passes/optimization_asm/interference.h.
#pragma once

#include <map>
#include <string>
#include <string_view>

#include "asdl/runtime.h"
#include "passes/asm_context.h"

namespace c6502::passes {

struct InterferenceNode {
  std::string name;
  asdl::Int width = 1;
  bool lives_across_call = false;
  bool operator==(const InterferenceNode&) const = default;
};

// The nodes and the adjacency are held sorted by name, where Python
// keeps dicts in the order the builder adds to them. That order follows
// the iteration of Python sets, and nothing reads it. A name may have
// an entry in `adj` and no node, as in Python, when an edge reaches a
// name that never gets one, and an entry may hold an empty set after a
// merge removes its last neighbor.
struct InterferenceGraph {
  std::map<std::string, InterferenceNode, std::less<>> nodes;
  std::map<std::string, NameSet, std::less<>> adj;

  // Returns the neighbors of `name`, an empty set for a name with no
  // entry.
  const NameSet& neighbors(std::string_view name) const;
  // Returns whether `a` and `b` are neighbors.
  bool has_edge(std::string_view a, std::string_view b) const;
  // Adds the edge between `a` and `b`, unless they are the same name.
  void add_edge(std::string_view a, std::string_view b);
  bool operator==(const InterferenceGraph&) const = default;
};

}  // namespace c6502::passes
