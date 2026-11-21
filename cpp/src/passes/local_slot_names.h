// `zp_slot_naming.local_slot_names` from passes/zp_slot_naming.py, the
// names of the bytes of a function's body-local pool. It has a file of
// its own because two parts of the port call it: `apply_coloring` in
// the asm optimizer, and `compute_local_slot_names` in
// src/passes/zp_slot_naming.h.
#pragma once

#include <string>
#include <string_view>
#include <vector>

#include "asdl/runtime.h"
#include "ordered_map.h"
#include "passes/optimization/register_allocation.h"

namespace c6502::passes {

// Returns the `__local_<fn>__<...>` symbol of each address of
// `local_pool`, in the pool's order. Each address takes the name of the
// first pseudo in `coloring.assignments` that colored to it and comes
// from the source, with a `_<k>` byte suffix when that source variable
// is wider than one byte, and the next free number otherwise
// (`__local_<fn>__<N>`). A name already given to another address also
// falls back to a number, so each symbol binds one address.
// `apply_coloring` and `build_local_slot_symbols` both read the names
// this gives, so the two must agree byte for byte.
OrderedMap<asdl::Int, std::string> local_slot_names(std::string_view fn_name,
                                                    const Coloring& coloring,
                                                    const std::vector<asdl::Int>& local_pool);

}  // namespace c6502::passes
