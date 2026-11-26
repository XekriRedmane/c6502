#pragma once

#include <optional>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "asdl/runtime.h"
#include "ordered_map.h"
#include "passes/abi_selection.h"
#include "passes/optimization/pool.h"
#include "passes/slot_sharing.h"
#include "tac_ast.h"

namespace c6502::passes {

// The spill region the allocators fall back to, $0200-$FFFF.
inline constexpr asdl::Int kDefaultSpillStart = 0x0200;
inline constexpr asdl::Int kDefaultSpillEnd = 0x10000;

// The options of `allocate_zp_slots` and `allocate_function_locals`.
// `reserved` is the set of bytes no slot or pool may use, the zero-page
// globals' bytes. `window` is the zero-page range they come from, the
// pool's caller-saved half when none is given (the linker gives the
// whole pool). `fallback` is a zero-page range tried when the window has
// no room, before the spill region. `shares` are the `SlotShare`s whose
// slots get one address.
struct ZpAllocationOptions {
  Pool pool;
  asdl::Int spill_start = kDefaultSpillStart;
  asdl::Int spill_end = kDefaultSpillEnd;
  std::set<asdl::Int> reserved;
  std::optional<Range> window;
  std::optional<Range> fallback;
  std::vector<SlotShare> shares;
};

// Returns `abi` with the address of every `ZpLayout` slot symbol chosen
// so that no two functions on a common call path share a slot byte, and
// each slot symbol mapped to its address, in the order the allocator
// chose them, for the `EQU` lines. A byte passed in a register gets no
// address, and a pinned one its own. Throws
// `CompileError("ZpSlotAllocationError", ...)` when a function's or a
// share's slots fit nowhere, or when the zp_abi call graph has a cycle,
// which `select_abi` rejects first.
std::pair<Abi, OrderedMap<std::string, asdl::Int>> allocate_zp_slots(
    const tac_ast::Program& prog, const Abi& abi, const ZpAllocationOptions& options = {});

namespace zp_slot_allocation {

// Returns the lowest run of `n_bytes` addresses in `[lo, hi)` none of
// which is in `forbidden`, or none when no run fits.
std::optional<std::vector<asdl::Int>> find_free_range(const std::set<asdl::Int>& forbidden,
                                                      asdl::Int n_bytes, asdl::Int lo,
                                                      asdl::Int hi);

}  // namespace zp_slot_allocation

}  // namespace c6502::passes
