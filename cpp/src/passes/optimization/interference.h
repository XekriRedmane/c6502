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

// The nodes and the adjacency are held sorted by name. A name may have
// an entry in `adj` and no node, when an edge reaches a name that never
// gets one, and an entry may hold an empty set after a merge removes its
// last neighbor.
struct InterferenceGraph {
  std::map<std::string, InterferenceNode, std::less<>> nodes;
  std::map<std::string, NameSet, std::less<>> adj;

  // Returns the neighbors of `name`, an empty set for a name with no
  // entry.
  const NameSet& neighbors(std::string_view name) const;
  bool has_edge(std::string_view a, std::string_view b) const;
  // Adds the edge between `a` and `b`, unless they are the same name.
  void add_edge(std::string_view a, std::string_view b);
  bool operator==(const InterferenceGraph&) const = default;
};

}  // namespace c6502::passes
