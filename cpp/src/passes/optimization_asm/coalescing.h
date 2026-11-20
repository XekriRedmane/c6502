// The port of passes/optimization_asm/coalescing.py, the move
// coalescing of the asm register allocation.
#pragma once

#include <map>
#include <string>
#include <string_view>

#include "asdl/runtime.h"
#include "asm_ast.h"
#include "ordered_map.h"
#include "passes/optimization/interference.h"

namespace c6502::passes {

// The outcome of `coalesce_moves`, which changes the interference graph
// in place: each coalesced name that isn't a representative is gone,
// with its edges moved to its representative.
//
// `representative` maps every coalesced name that isn't its own
// representative to the name it was merged into, in the order of the
// merges, which the optimizer's projection of the coloring follows.
// `byte_offset` holds the byte position of a 1-byte name merged into
// one byte of a node of several bytes (see "Carry bytes" in
// coalescing.cpp), and a name missing from it sits at offset 0 of the
// name it was merged into. Python keeps it in an order that follows the
// graph's nodes, which nothing reads, so it is held sorted here.
struct CoalesceResult {
  OrderedMap<std::string, std::string> representative;
  std::map<std::string, asdl::Int, std::less<>> byte_offset;

  // Returns the representative of `name`, following the chain, or
  // `name` itself when it isn't coalesced.
  std::string resolve(std::string_view name) const;
  // Returns the byte position of `name` inside its representative's
  // storage, summed along the chain.
  asdl::Int offset_of(std::string_view name) const;
  bool operator==(const CoalesceResult&) const = default;
};

// Coalesces the move-related pairs of pseudos in `graph`, which it
// changes in place, and returns the map that projects each coalesced
// name onto its representative, which the caller uses to give every
// SSA name its color after the coloring.
CoalesceResult coalesce_moves(const asm_ast::Function& fn, InterferenceGraph& graph);

}  // namespace c6502::passes
