// The decoders of the keyword arguments that the registered passes take,
// from the tagged JSON of tools/capture_passes.py.
#pragma once

#include <optional>
#include <string_view>

#include "json.h"
#include "passes/asm_context.h"

namespace c6502::passes {

// Returns the keyword argument `name`, a dict from names to addresses
// in the tagged JSON (`{"dict": [[name, address], ...]}`), or nothing
// when it is `None` or missing.
inline std::optional<SymbolAddrs> symbol_addrs_kwarg(const json::Value& kwargs,
                                                     std::string_view name) {
  const json::Value* v = kwargs.get(name);
  if (v == nullptr || v->kind == json::Value::Kind::kNull) {
    return std::nullopt;
  }
  SymbolAddrs out;
  for (const auto& pair : v->get("dict")->items) {
    out[pair.items[0].string] = pair.items[1].integer;
  }
  return out;
}

// Returns the keyword argument `name`, a set of names in the tagged
// JSON (`{"set": [name, ...]}`), or an empty set when it is `None` or
// missing.
inline NameSet name_set_kwarg(const json::Value& kwargs, std::string_view name) {
  NameSet out;
  const json::Value* v = kwargs.get(name);
  if (v == nullptr || v->kind == json::Value::Kind::kNull) {
    return out;
  }
  for (const auto& item : v->get("set")->items) {
    out.insert(item.string);
  }
  return out;
}

}  // namespace c6502::passes
