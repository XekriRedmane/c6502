// The result of the asm optimizer's register allocation, Python's
// `Coloring` in passes/optimization/register_allocation.py.
#pragma once

#include <string>

#include "asdl/runtime.h"
#include "ordered_map.h"
#include "passes/asm_context.h"
#include "passes/optimization/pool.h"

namespace c6502::passes {

// `assignments` maps each colored name to the zero-page address of its
// lowest byte, in the order the allocator colored them, which
// `zp_slot_naming.local_slot_names` follows (the first name from the
// source at an address names it). `spilled` holds the names that fit
// no pool, a Python set whose order nothing reads. `pool` is the pool
// the allocator used, and `hwreg_assignments` maps each name pinned to
// an index register to "X" or "Y". A name is in at most one of
// `assignments` and `hwreg_assignments`.
struct Coloring {
  OrderedMap<std::string, asdl::Int> assignments;
  NameSet spilled;
  Pool pool;
  OrderedMap<std::string, std::string> hwreg_assignments;
  bool operator==(const Coloring&) const = default;
};

}  // namespace c6502::passes
