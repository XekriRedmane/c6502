#include "passes/optimization/interference.h"

namespace c6502::passes {

const NameSet& InterferenceGraph::neighbors(std::string_view name) const {
  static const NameSet kEmpty;
  auto it = adj.find(name);
  return it == adj.end() ? kEmpty : it->second;
}

bool InterferenceGraph::has_edge(std::string_view a, std::string_view b) const {
  auto it = adj.find(a);
  return it != adj.end() && it->second.find(b) != it->second.end();
}

void InterferenceGraph::add_edge(std::string_view a, std::string_view b) {
  if (a == b) {
    return;
  }
  auto ia = adj.find(a);
  if (ia == adj.end()) {
    ia = adj.emplace(std::string(a), NameSet{}).first;
  }
  ia->second.emplace(b);
  auto ib = adj.find(b);
  if (ib == adj.end()) {
    ib = adj.emplace(std::string(b), NameSet{}).first;
  }
  ib->second.emplace(a);
}

}  // namespace c6502::passes
