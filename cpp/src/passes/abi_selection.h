// The calling conventions that passes/abi_selection.py chooses for each
// function, which the code generation passes and the back end read.
// `select_abi` itself isn't ported yet.
#pragma once

#include <optional>
#include <string>
#include <variant>
#include <vector>

#include "asdl/runtime.h"
#include "ordered_map.h"

namespace c6502::passes {

// The soft-stack convention, in which each parameter byte sits in the
// callee's frame at `Frame(M + 3 + j_offset + k)` and the caller pushes
// the arguments (Python's `SoftStackLayout`).
struct SoftStackLayout {
  bool operator==(const SoftStackLayout&) const = default;
};

// The zero-page convention (Python's `ZpLayout`, whose docstring this
// follows). `slot_symbols[i]` names the slot of the i-th parameter
// byte, low byte of the first parameter first. `addrs[i]` is the
// address `zp_slot_allocation` gives that slot, or none for a slot no
// instruction uses, and the list is empty before the allocator runs.
// `param_registers[i]` is the register the byte arrives in at the call
// ("A", "X", or "Y"), or none for a byte that arrives in its slot.
// `pinned[i]` is the address `[[c6502::zeropage(addr)]]` fixes the byte
// at, or none. An empty `param_registers` or `pinned` means that no
// byte has one.
struct ZpLayout {
  std::vector<std::string> slot_symbols;
  std::vector<std::optional<asdl::Int>> addrs;
  std::vector<std::optional<std::string>> param_registers;
  std::vector<std::optional<asdl::Int>> pinned;
  bool operator==(const ZpLayout&) const = default;
};

using ParamLayout = std::variant<SoftStackLayout, ZpLayout>;

// Each function's convention, in the order of Python's `abi` dict,
// which the allocators and `tac_to_asm` walk.
using Abi = OrderedMap<std::string, ParamLayout>;

}  // namespace c6502::passes
