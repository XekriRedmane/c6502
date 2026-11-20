#include "passes/capture_args.h"

#include <stdexcept>
#include <string>

namespace c6502::passes::capture_args {

const json::Value* kwarg(const json::Value& kwargs, std::string_view name) {
  return kwargs.get(name);
}

std::optional<SymbolAddrs> symbol_addrs(const json::Value* v) {
  if (v == nullptr || v->kind == json::Value::Kind::kNull) {
    return std::nullopt;
  }
  const json::Value* pairs = v->get("dict");
  if (pairs == nullptr || pairs->kind != json::Value::Kind::kArray) {
    throw std::runtime_error("a symbol map's argument isn't a tagged dict");
  }
  SymbolAddrs out;
  for (const auto& pair : pairs->items) {
    if (pair.items.size() != 2 ||
        pair.items[0].kind != json::Value::Kind::kString ||
        pair.items[1].kind != json::Value::Kind::kInt) {
      throw std::runtime_error("a symbol map's entry isn't [name, address]");
    }
    out[pair.items[0].string] = pair.items[1].integer;
  }
  return out;
}

bool boolean(const json::Value* v, bool fallback) {
  if (v == nullptr) {
    return fallback;
  }
  if (v->kind != json::Value::Kind::kBool) {
    throw std::runtime_error("an argument that should be a bool isn't one");
  }
  return v->boolean;
}

}  // namespace c6502::passes::capture_args
