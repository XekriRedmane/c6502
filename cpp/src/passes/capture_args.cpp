#include "passes/capture_args.h"

#include <stdexcept>
#include <string>
#include <utility>

#include "tagged_json.h"

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

NameSet name_set(const json::Value* v) {
  NameSet out;
  if (v == nullptr || v->kind == json::Value::Kind::kNull) {
    return out;
  }
  const json::Value* names = v->get("set");
  if (names == nullptr || names->kind != json::Value::Kind::kArray) {
    throw std::runtime_error("a name set's argument isn't a tagged set");
  }
  for (const auto& item : names->items) {
    if (item.kind != json::Value::Kind::kString) {
      throw std::runtime_error("a name set's entry isn't a name");
    }
    out.insert(item.string);
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

const std::vector<json::Value>& items_of(const json::Value& v) {
  if (v.kind == json::Value::Kind::kArray) {
    return v.items;
  }
  for (const char* tag : {"tuple", "set"}) {
    if (const json::Value* inner = v.get(tag)) {
      return inner->items;
    }
  }
  throw std::runtime_error("expected a list, a tuple, or a set");
}

NameSet names_of(const json::Value& v) {
  NameSet out;
  for (const auto& x : items_of(v)) {
    out.insert(x.string);
  }
  return out;
}

std::set<asdl::Int> ints_of(const json::Value& v) {
  std::set<asdl::Int> out;
  for (const auto& x : items_of(v)) {
    out.insert(x.integer);
  }
  return out;
}

SymbolAddrs addrs_of(const json::Value& v) {
  auto out = symbol_addrs(&v);
  if (!out) {
    throw std::runtime_error("a symbol map's argument is None");
  }
  return std::move(*out);
}

namespace {

// Returns a function's layout in `abi`: a `ZpLayout`'s fields, or
// nothing for any other class.
std::optional<ZpLayout> zp_layout(const json::Value& v) {
  const json::Value* cls = v.get("object");
  if (cls == nullptr || cls->string != "passes.abi_selection.ZpLayout") {
    return std::nullopt;
  }
  return tagged::ZpLayoutC::decode(v);
}

}  // namespace

AbiLayouts abi_layouts(const json::Value& v) {
  const json::Value* pairs = v.get("dict");
  if (pairs == nullptr) {
    throw std::runtime_error("the abi argument isn't a tagged dict");
  }
  AbiLayouts abi;
  for (const auto& kv : pairs->items) {
    abi.emplace_back(kv.items.at(0).string, zp_layout(kv.items.at(1)));
  }
  return abi;
}

std::vector<SlotShare> slot_shares(const json::Value& v) {
  std::vector<SlotShare> shares;
  for (const auto& share : items_of(v)) {
    shares.push_back(tagged::SlotShareC::decode(share));
  }
  return shares;
}

}  // namespace c6502::passes::capture_args
