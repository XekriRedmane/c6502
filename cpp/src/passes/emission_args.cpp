#include "passes/emission_args.h"

#include <stdexcept>

#include "passes/capture_args.h"
#include "passes/registry_control_flow.h"
#include "tagged_json.h"

namespace c6502::passes::emission_args {

namespace {

using json::Value;
using capture_args::items_of;
using registry::json_quote;

// Returns the `fields` of a tagged object.
const Value& fields_of(const Value& v) {
  const Value* fields = v.get("fields");
  if (fields == nullptr) {
    throw std::runtime_error("expected a tagged object");
  }
  return *fields;
}

const Value& field(const Value& fields, std::string_view name) {
  const Value* f = fields.get(name);
  if (f == nullptr) {
    throw std::runtime_error("a tagged object has no field " + std::string(name));
  }
  return *f;
}

// Returns the `[key, value]` pairs of a tagged dict.
const std::vector<Value>& dict_pairs(const Value& v) {
  const Value* pairs = v.get("dict");
  if (pairs == nullptr) {
    throw std::runtime_error("expected a tagged dict");
  }
  return pairs->items;
}

std::vector<std::string> strings(const Value& v) {
  std::vector<std::string> out;
  for (const auto& x : items_of(v)) {
    out.push_back(x.string);
  }
  return out;
}

std::optional<asdl::Int> optional_int(const Value& v) {
  if (v.kind == Value::Kind::kNull) {
    return std::nullopt;
  }
  return v.integer;
}

std::vector<std::optional<asdl::Int>> pinned_of(const Value& v) {
  std::vector<std::optional<asdl::Int>> out;
  for (const auto& x : items_of(v)) {
    out.push_back(optional_int(x));
  }
  return out;
}

std::vector<std::pair<std::string, std::string>> string_pairs(const Value& v) {
  std::vector<std::pair<std::string, std::string>> out;
  for (const auto& pair : items_of(v)) {
    const auto& two = items_of(pair);
    out.emplace_back(two.at(0).string, two.at(1).string);
  }
  return out;
}

std::string int_json(asdl::Int v) { return asdl::int_to_string(v); }

const char* bool_json(bool b) { return b ? "true" : "false"; }

}  // namespace

OrderedSymbolAddrs ordered_addrs(const Value& v) {
  OrderedSymbolAddrs out;
  for (const auto& kv : dict_pairs(v)) {
    out.emplace_back(kv.items.at(0).string, kv.items.at(1).integer);
  }
  return out;
}

ZpGlobals zp_globals(const Value& v) { return tagged::ZpGlobalsC::decode(v); }

HelperFootprints footprints(const Value& v) {
  HelperFootprints out;
  for (const auto& kv : dict_pairs(v)) {
    out[kv.items.at(0).string] = kv.items.at(1).integer;
  }
  return out;
}

SectionInfos section_infos(const Value& v) {
  SectionInfos out;
  for (const auto& kv : dict_pairs(v)) {
    const auto& info = items_of(kv.items.at(1));
    SectionInfo s;
    if (info.at(0).kind != Value::Kind::kNull) {
      s.section = info.at(0).string;
    }
    s.is_const = info.at(1).boolean;
    s.merge = info.at(2).boolean;
    out[kv.items.at(0).string] = std::move(s);
  }
  return out;
}

LinkMetadata link_metadata(const Value& v) {
  const Value& f = fields_of(v);
  LinkMetadata meta;
  for (const auto& d : items_of(field(f, "defs"))) {
    const Value& df = fields_of(d);
    meta.defs.push_back(FunctionMeta{
        .name = field(df, "name").string,
        .params = strings(field(df, "params")),
        .locals = strings(field(df, "locals")),
        .indirect = field(df, "indirect").boolean,
        .in_cycle = field(df, "in_cycle").boolean,
        .param_regs = strings(field(df, "param_regs")),
        .pinned = pinned_of(field(df, "pinned")),
        .changes = strings(field(df, "changes")),
        .carry = field(df, "carry").boolean,
    });
  }
  for (const auto& e : items_of(field(f, "externs"))) {
    const Value& ef = fields_of(e);
    meta.externs.push_back(ExternMeta{
        .name = field(ef, "name").string,
        .params = strings(field(ef, "params")),
        .param_regs = strings(field(ef, "param_regs")),
        .pinned = pinned_of(field(ef, "pinned")),
        .carry = field(ef, "carry").boolean,
    });
  }
  meta.calls = string_pairs(field(f, "calls"));
  meta.saved_calls = string_pairs(field(f, "saved_calls"));
  for (const auto& z : items_of(field(f, "zp_objects"))) {
    meta.zp_objects.push_back(tagged::ZpObjectMetaC::decode(z));
  }
  meta.hargs_size = optional_int(field(f, "hargs_size"));
  meta.hargs_need = optional_int(field(f, "hargs_need"));
  for (const auto& it : items_of(field(f, "items"))) {
    const Value& itf = fields_of(it);
    meta.items.push_back(ItemMeta{
        .name = field(itf, "name").string,
        .section = field(itf, "section").string,
        .local = field(itf, "local").boolean,
        .zero = field(itf, "zero").boolean,
        .merge = field(itf, "merge").boolean,
    });
  }
  for (const auto& s : items_of(field(f, "shares"))) {
    meta.shares.push_back(tagged::SlotShareC::decode(s));
  }
  return meta;
}

std::map<std::string, NameSet, std::less<>> preferred_symbols(const Value& v) {
  std::map<std::string, NameSet, std::less<>> out;
  for (const auto& kv : dict_pairs(v)) {
    out[kv.items.at(0).string] = capture_args::names_of(kv.items.at(1));
  }
  return out;
}

std::string items_json(const std::vector<ItemMeta>& items) {
  std::string out = "[";
  for (std::size_t i = 0; i < items.size(); ++i) {
    const ItemMeta& it = items[i];
    if (i != 0) {
      out += ", ";
    }
    out += R"({"object": "passes.zp_link_metadata.ItemMeta", "fields": {"name": )" +
           json_quote(it.name) + R"(, "section": )" + json_quote(it.section) +
           R"(, "local": )" + bool_json(it.local) + R"(, "zero": )" + bool_json(it.zero) +
           R"(, "merge": )" + bool_json(it.merge) + "}}";
  }
  return out + "]";
}

std::string strings_json(const std::vector<std::string>& strings) {
  std::string out = "[";
  for (std::size_t i = 0; i < strings.size(); ++i) {
    if (i != 0) {
      out += ", ";
    }
    out += json_quote(strings[i]);
  }
  return out + "]";
}

std::string zp_objects_json(const std::vector<ZpObjectMeta>& objects) {
  std::string out = "[";
  for (std::size_t i = 0; i < objects.size(); ++i) {
    const ZpObjectMeta& z = objects[i];
    if (i != 0) {
      out += ", ";
    }
    out += R"({"object": "passes.zp_link_metadata.ZpObjectMeta", "fields": {"name": )" +
           json_quote(z.name) + R"(, "size": )" + int_json(z.size) + R"(, "at": )" +
           (z.at ? int_json(*z.at) : "null") + R"(, "defined": )" + bool_json(z.defined) +
           R"(, "init": )" + strings_json(z.init) + R"(, "weight": )" +
           (z.weight ? int_json(*z.weight) : "null") + R"(, "local": )" + bool_json(z.local) +
           "}}";
  }
  return out + "]";
}

std::string ordered_addrs_json(const OrderedSymbolAddrs& addrs) {
  std::string out = R"({"dict": [)";
  for (std::size_t i = 0; i < addrs.size(); ++i) {
    if (i != 0) {
      out += ", ";
    }
    out += '[';
    out += json_quote(addrs[i].first);
    out += ", ";
    out += int_json(addrs[i].second);
    out += ']';
  }
  return out + "]}";
}

}  // namespace c6502::passes::emission_args
