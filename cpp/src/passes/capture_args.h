// Decodes the tagged JSON of a captured call's arguments
// (tools/capture_passes.py describes the encoding) into the values the
// C++ passes take, for the entries of registry.cpp.
#pragma once

#include <optional>
#include <set>
#include <string_view>
#include <vector>

#include "json.h"
#include "passes/asm_context.h"
#include "passes/slot_coalescing.h"

namespace c6502::passes::capture_args {

// Returns the keyword argument `name` of the JSON object `kwargs`, or
// null when the call didn't pass it.
const json::Value* kwarg(const json::Value& kwargs, std::string_view name);

// Returns the map of symbol names to addresses that `v` encodes as
// `{"dict": [[name, address], ...]}`, or nothing when `v` is null or
// JSON's null, which is how a call that leaves a keyword argument such
// as `zp_slot_symbols` at its default of `None` is recorded. Throws
// `std::runtime_error` for any other value.
std::optional<SymbolAddrs> symbol_addrs(const json::Value* v);

// Returns the set of names that `v` encodes as `{"set": [name, ...]}`,
// or an empty set when `v` is null or JSON's null, which is how a call
// that leaves a keyword argument such as `noreturn` at its default of
// `None` is recorded. Throws `std::runtime_error` for any other value.
NameSet name_set(const json::Value* v);

// Returns the bool that `v` encodes, or `fallback` when `v` is null.
bool boolean(const json::Value* v, bool fallback);

// Returns the elements of a list, a tuple, a set, or a frozenset in the
// tagged JSON. Throws `std::runtime_error` for any other value.
const std::vector<json::Value>& items_of(const json::Value& v);

// Returns the names of a list, a tuple, a set, or a frozenset of
// strings.
NameSet names_of(const json::Value& v);

// Returns the integers of a list, a tuple, a set, or a frozenset of
// ints.
std::set<asdl::Int> ints_of(const json::Value& v);

// Returns the map of names to addresses of a tagged dict, as
// `symbol_addrs` does, for a value that must be present.
SymbolAddrs addrs_of(const json::Value& v);

// Returns each function's calling convention from the tagged dict
// `abi`, in the dict's order: a `ZpLayout`'s fields for a
// `passes.abi_selection.ZpLayout`, and nothing for any other class.
AbiLayouts abi_layouts(const json::Value& v);

// Returns the members of each `slot_sharing.SlotShare` of the list or
// tuple `v`.
std::vector<SlotShare> slot_shares(const json::Value& v);

}  // namespace c6502::passes::capture_args
