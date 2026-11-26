// The registry's entries (registry.cpp) for the work after the back
// end's region and for emission: `place_zp_globals`, `link_records`,
// `check_shares`, `prune_unused_locals`, `hargs_need`, `section_items`,
// `lower_to_asm2`, `format_metadata`, and `emit_program`, under the
// names the captures record them by.
#pragma once

#include <vector>

#include "passes/registry.h"

namespace c6502::passes::registry {

// Returns the entries, in the order codegen calls the functions.
const std::vector<RegisteredPass>& emission_passes();

}  // namespace c6502::passes::registry
