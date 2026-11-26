#pragma once

#include <optional>
#include <set>
#include <string>
#include <string_view>

#include "asdl/runtime.h"
#include "ordered_map.h"
#include "passes/asm_context.h"
#include "passes/optimization/interference.h"
#include "passes/optimization/pool.h"

namespace c6502::passes {

// `assignments` maps each colored name to the zero-page address of its
// lowest byte, in the order the allocator colored them, which
// `local_slot_names` follows (the first name from the source at an
// address names it). `spilled` holds the names that fit no pool, in no
// order that anything reads. `pool` is the pool the allocator used, and
// `hwreg_assignments` maps each name pinned to an index register to "X"
// or "Y". A name is in at most one of `assignments` and
// `hwreg_assignments`.
struct Coloring {
  OrderedMap<std::string, asdl::Int> assignments;
  NameSet spilled;
  Pool pool;
  OrderedMap<std::string, std::string> hwreg_assignments;
  bool operator==(const Coloring&) const = default;
};

namespace register_allocation {

// Returns the zero-page bytes that the colored neighbors of `name` in
// `graph` occupy, each neighbor's base in `assignments` and the bytes of
// its width after it. A neighbor that isn't colored, or was spilled,
// adds nothing, since a spilled value lives in the frame.
std::set<asdl::Int> blocked_bytes(std::string_view name, const InterferenceGraph& graph,
                                  const OrderedMap<std::string, asdl::Int>& assignments);

// Returns the lowest base in `byte_range` such that the `width` bytes
// from it are all inside the range and none is in `blocked`, or nothing
// when there is no such base. The range's step isn't read.
std::optional<asdl::Int> find_fit(const Range& byte_range, const asdl::Int& width,
                                  const std::set<asdl::Int>& blocked);

}  // namespace register_allocation

}  // namespace c6502::passes
