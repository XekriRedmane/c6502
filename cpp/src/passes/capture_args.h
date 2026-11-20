// Decodes the tagged JSON of a captured call's arguments
// (tools/capture_passes.py describes the encoding) into the values the
// C++ passes take, for the entries of registry.cpp.
#pragma once

#include <optional>
#include <string_view>

#include "json.h"
#include "passes/asm_context.h"

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

}  // namespace c6502::passes::capture_args
