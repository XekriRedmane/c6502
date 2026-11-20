#include "tagged_json.h"

#include <cstring>

#include "passes/registry_control_flow.h"

namespace c6502::tagged {

using json::Value;
using Kind = json::Value::Kind;

void write_string(std::string& out, std::string_view s) {
  out += passes::registry::json_quote(s);
}

void write_value(std::string& out, const Value& v) {
  switch (v.kind) {
    case Kind::kNull:
      out += "null";
      return;
    case Kind::kBool:
      out += v.boolean ? "true" : "false";
      return;
    case Kind::kInt:
      out += asdl::int_to_string(v.integer);
      return;
    case Kind::kString:
      write_string(out, v.string);
      return;
    case Kind::kArray:
      out += '[';
      for (std::size_t i = 0; i < v.items.size(); ++i) {
        if (i != 0) {
          out += ", ";
        }
        write_value(out, v.items[i]);
      }
      out += ']';
      return;
    case Kind::kObject:
      out += '{';
      for (std::size_t i = 0; i < v.items.size(); ++i) {
        if (i != 0) {
          out += ", ";
        }
        write_string(out, v.keys[i]);
        out += ": ";
        write_value(out, v.items[i]);
      }
      out += '}';
      return;
  }
}

std::string to_text(const Value& v) {
  std::string out;
  write_value(out, v);
  return out;
}

const std::vector<Value>& array_items(const Value& v) {
  if (v.kind != Kind::kArray) {
    throw DecodeError("expected a list, got " + to_text(v).substr(0, 60));
  }
  return v.items;
}

const std::vector<Value>& tagged_items(const Value& v, std::string_view tag) {
  const Value* inner = v.kind == Kind::kObject && v.keys.size() == 1 ? v.get(tag) : nullptr;
  if (inner == nullptr || inner->kind != Kind::kArray) {
    throw DecodeError("expected a " + std::string(tag) + ", got " + to_text(v).substr(0, 60));
  }
  return inner->items;
}

std::string_view object_class(const Value& v) {
  const Value* cls = v.kind == Kind::kObject ? v.get("object") : nullptr;
  return cls == nullptr ? std::string_view() : std::string_view(cls->string);
}

const Value& object_fields(const Value& v, std::string_view cls,
                           std::initializer_list<std::string_view> names) {
  if (object_class(v) != cls) {
    throw DecodeError("expected a " + std::string(cls) + ", got " + to_text(v).substr(0, 80));
  }
  const Value* fields = v.get("fields");
  if (fields == nullptr || fields->kind != Kind::kObject || fields->keys.size() != names.size() ||
      !std::equal(names.begin(), names.end(), fields->keys.begin())) {
    throw DecodeError("the fields of a " + std::string(cls) + " aren't the ones the C++ form holds");
  }
  return *fields;
}

namespace {

// Writes `{"object": cls, "fields": {...}}` one field at a time.
class ObjectWriter {
 public:
  ObjectWriter(std::string& out, std::string_view cls) : out_(out) {
    out_ += "{\"object\": ";
    write_string(out_, cls);
    out_ += ", \"fields\": {";
  }
  ~ObjectWriter() { out_ += "}}"; }

  // Writes the field's name and returns the text to append its value
  // to.
  std::string& field(std::string_view name) {
    if (!first_) {
      out_ += ", ";
    }
    first_ = false;
    write_string(out_, name);
    out_ += ": ";
    return out_;
  }

 private:
  std::string& out_;
  bool first_ = true;
};

const Value& at(const Value& fields, std::size_t i) { return fields.items[i]; }

constexpr std::string_view kTypeChecking = "passes.type_checking.";

std::string tc(std::string_view name) { return std::string(kTypeChecking) + std::string(name); }

}  // namespace

// --- Python's own values ---

IntC::type IntC::decode(const Value& v) {
  if (v.kind != Kind::kInt) {
    throw DecodeError("expected an int, got " + to_text(v).substr(0, 60));
  }
  return v.integer;
}
void IntC::encode(std::string& out, const type& x) { out += asdl::int_to_string(x); }

Str::type Str::decode(const Value& v) {
  if (v.kind != Kind::kString) {
    throw DecodeError("expected a str, got " + to_text(v).substr(0, 60));
  }
  return v.string;
}
void Str::encode(std::string& out, const type& x) { write_string(out, x); }

BoolC::type BoolC::decode(const Value& v) {
  if (v.kind != Kind::kBool) {
    throw DecodeError("expected a bool, got " + to_text(v).substr(0, 60));
  }
  return v.boolean;
}
void BoolC::encode(std::string& out, const type& x) { out += x ? "true" : "false"; }

FloatC::type FloatC::decode(const Value& v) {
  const Value* bits = v.kind == Kind::kObject && v.keys.size() == 1 ? v.get("float") : nullptr;
  if (bits == nullptr || bits->kind != Kind::kInt) {
    throw DecodeError("expected a float, got " + to_text(v).substr(0, 60));
  }
  return static_cast<std::uint64_t>(bits->integer);
}
void FloatC::encode(std::string& out, const type& x) {
  out += "{\"float\": ";
  out += asdl::int_to_string(static_cast<asdl::Int>(x));
  out += '}';
}

RangeC::type RangeC::decode(const Value& v) {
  const auto& items = tagged_items(v, "range");
  if (items.size() != 3) {
    throw DecodeError("a range isn't [start, stop, step]");
  }
  return passes::Range{.start = IntC::decode(items[0]),
                       .stop = IntC::decode(items[1]),
                       .step = IntC::decode(items[2])};
}
void RangeC::encode(std::string& out, const type& x) {
  out += "{\"range\": [" + asdl::int_to_string(x.start) + ", " + asdl::int_to_string(x.stop) +
         ", " + asdl::int_to_string(x.step) + "]}";
}

// --- The symbol table ---

namespace {

passes::ValueTree decode_tree(const Value& v) {
  using passes::ValueTree;
  ValueTree t;
  if (v.kind == Kind::kInt) {
    t.kind = ValueTree::Kind::kInt;
    t.integer = v.integer;
    return t;
  }
  if (v.kind == Kind::kObject && v.keys.size() == 1 && v.keys[0] == "float") {
    t.kind = ValueTree::Kind::kFloat;
    t.float_bits = FloatC::decode(v);
    return t;
  }
  if (v.kind == Kind::kObject && v.keys.size() == 1 && v.keys[0] == "tuple") {
    t.kind = ValueTree::Kind::kTuple;
    for (const auto& item : tagged_items(v, "tuple")) {
      t.items.push_back(decode_tree(item));
    }
    return t;
  }
  if (v.kind == Kind::kObject && v.get("asdl") != nullptr) {
    using TacInitC = Asdl<tac_ast::Type_static_init, kTacAst>;
    t.kind = ValueTree::Kind::kTacInit;
    t.tac_init = TacInitC::decode(v);
    return t;
  }
  const std::string_view cls = object_class(v);
  if (cls == tc("AddressInit")) {
    const Value& f = object_fields(v, cls, {"name", "offset"});
    t.kind = ValueTree::Kind::kAddress;
    t.address = passes::AddressInit{.name = Str::decode(at(f, 0)), .offset = IntC::decode(at(f, 1))};
    return t;
  }
  if (cls == tc("UnionValue")) {
    const Value& f = object_fields(v, cls, {"member", "value"});
    t.kind = ValueTree::Kind::kUnion;
    t.member = IntC::decode(at(f, 0));
    t.items.push_back(decode_tree(at(f, 1)));
    return t;
  }
  throw DecodeError("an initial value isn't one the C++ form holds: " + to_text(v).substr(0, 80));
}

void encode_tree(std::string& out, const passes::ValueTree& t) {
  using K = passes::ValueTree::Kind;
  switch (t.kind) {
    case K::kInt:
      IntC::encode(out, t.integer);
      return;
    case K::kFloat:
      FloatC::encode(out, t.float_bits);
      return;
    case K::kTuple:
      out += "{\"tuple\": [";
      for (std::size_t i = 0; i < t.items.size(); ++i) {
        if (i != 0) {
          out += ", ";
        }
        encode_tree(out, t.items[i]);
      }
      out += "]}";
      return;
    case K::kAddress: {
      ObjectWriter w(out, tc("AddressInit"));
      Str::encode(w.field("name"), t.address.name);
      IntC::encode(w.field("offset"), t.address.offset);
      return;
    }
    case K::kUnion: {
      ObjectWriter w(out, tc("UnionValue"));
      IntC::encode(w.field("member"), t.member);
      encode_tree(w.field("value"), t.items.at(0));
      return;
    }
    case K::kTacInit:
      Asdl<tac_ast::Type_static_init, kTacAst>::encode(out, t.tac_init.value());
      return;
  }
}

passes::InitialValue decode_initial(const Value& v) {
  const std::string_view cls = object_class(v);
  if (cls == tc("Tentative")) {
    object_fields(v, cls, {});
    return passes::Tentative{};
  }
  if (cls == tc("NoInitializer")) {
    object_fields(v, cls, {});
    return passes::NoInitializer{};
  }
  const Value& f = object_fields(v, tc("Initial"), {"value"});
  return passes::Initial{.value = decode_tree(at(f, 0))};
}

void encode_initial(std::string& out, const passes::InitialValue& iv) {
  if (std::holds_alternative<passes::Tentative>(iv)) {
    ObjectWriter w(out, tc("Tentative"));
    return;
  }
  if (std::holds_alternative<passes::NoInitializer>(iv)) {
    ObjectWriter w(out, tc("NoInitializer"));
    return;
  }
  ObjectWriter w(out, tc("Initial"));
  encode_tree(w.field("value"), std::get<passes::Initial>(iv).value);
}

using OptInt = Optional<IntC>;
using OptStr = Optional<Str>;

passes::IdAttr decode_attr(const Value& v) {
  const std::string_view cls = object_class(v);
  if (cls == tc("LocalAttr")) {
    const Value& f = object_fields(v, cls, {"register", "was_bool"});
    return passes::LocalAttr{.register_ = BoolC::decode(at(f, 0)),
                             .was_bool = BoolC::decode(at(f, 1))};
  }
  if (cls == tc("StaticAttr")) {
    const Value& f = object_fields(v, cls,
                                   {"initial_value", "is_global", "zeropage", "zp_address",
                                    "address", "section", "planes", "mergeable", "was_bool"});
    return passes::StaticAttr{
        .initial_value = decode_initial(at(f, 0)),
        .is_global = BoolC::decode(at(f, 1)),
        .zeropage = BoolC::decode(at(f, 2)),
        .zp_address = OptInt::decode(at(f, 3)),
        .address = OptInt::decode(at(f, 4)),
        .section = OptStr::decode(at(f, 5)),
        .planes = Optional<TupleOf<Str>>::decode(at(f, 6)),
        .mergeable = BoolC::decode(at(f, 7)),
        .was_bool = BoolC::decode(at(f, 8)),
    };
  }
  const Value& f = object_fields(v, tc("FunAttr"),
                                 {"defined", "is_global", "carry_return", "address", "preserves",
                                  "section", "param_zeropage", "returns_bool", "noreturn"});
  return passes::FunAttr{
      .defined = BoolC::decode(at(f, 0)),
      .is_global = BoolC::decode(at(f, 1)),
      .carry_return = BoolC::decode(at(f, 2)),
      .address = OptInt::decode(at(f, 3)),
      .preserves = NameSetC::decode(at(f, 4)),
      .section = OptStr::decode(at(f, 5)),
      .param_zeropage = TupleOf<OptInt>::decode(at(f, 6)),
      .returns_bool = BoolC::decode(at(f, 7)),
      .noreturn = BoolC::decode(at(f, 8)),
  };
}

void encode_attr(std::string& out, const passes::IdAttr& attr) {
  if (const auto* a = std::get_if<passes::LocalAttr>(&attr)) {
    ObjectWriter w(out, tc("LocalAttr"));
    BoolC::encode(w.field("register"), a->register_);
    BoolC::encode(w.field("was_bool"), a->was_bool);
    return;
  }
  if (const auto* a = std::get_if<passes::StaticAttr>(&attr)) {
    ObjectWriter w(out, tc("StaticAttr"));
    encode_initial(w.field("initial_value"), a->initial_value);
    BoolC::encode(w.field("is_global"), a->is_global);
    BoolC::encode(w.field("zeropage"), a->zeropage);
    OptInt::encode(w.field("zp_address"), a->zp_address);
    OptInt::encode(w.field("address"), a->address);
    OptStr::encode(w.field("section"), a->section);
    Optional<TupleOf<Str>>::encode(w.field("planes"), a->planes);
    BoolC::encode(w.field("mergeable"), a->mergeable);
    BoolC::encode(w.field("was_bool"), a->was_bool);
    return;
  }
  const auto& a = std::get<passes::FunAttr>(attr);
  ObjectWriter w(out, tc("FunAttr"));
  BoolC::encode(w.field("defined"), a.defined);
  BoolC::encode(w.field("is_global"), a.is_global);
  BoolC::encode(w.field("carry_return"), a.carry_return);
  OptInt::encode(w.field("address"), a.address);
  NameSetC::encode(w.field("preserves"), a.preserves);
  OptStr::encode(w.field("section"), a.section);
  TupleOf<OptInt>::encode(w.field("param_zeropage"), a.param_zeropage);
  BoolC::encode(w.field("returns_bool"), a.returns_bool);
  BoolC::encode(w.field("noreturn"), a.noreturn);
}

}  // namespace

SymbolC::type SymbolC::decode(const Value& v) {
  const Value& f = object_fields(v, tc("Symbol"), {"type", "attrs"});
  return passes::Symbol{.type = C99TypeC::decode(at(f, 0)), .attrs = decode_attr(at(f, 1))};
}
void SymbolC::encode(std::string& out, const type& x) {
  ObjectWriter w(out, tc("Symbol"));
  C99TypeC::encode(w.field("type"), x.type);
  encode_attr(w.field("attrs"), x.attrs);
}

SymbolTableC::type SymbolTableC::decode(const Value& v) {
  const Value& f = object_fields(v, tc("SymbolTable"), {"_table"});
  return DictOf<Str, SymbolC>::decode(at(f, 0));
}
void SymbolTableC::encode(std::string& out, const type& x) {
  ObjectWriter w(out, tc("SymbolTable"));
  DictOf<Str, SymbolC>::encode(w.field("_table"), x);
}

// --- The type table ---

namespace {

struct MemberInfoC {
  using type = passes::MemberInfo;
  static type decode(const Value& v) {
    const Value& f = object_fields(v, tc("MemberInfo"), {"name", "type", "byte_offset"});
    return passes::MemberInfo{.name = Str::decode(at(f, 0)),
                              .type = C99TypeC::decode(at(f, 1)),
                              .byte_offset = IntC::decode(at(f, 2))};
  }
  static void encode(std::string& out, const type& x) {
    ObjectWriter w(out, tc("MemberInfo"));
    Str::encode(w.field("name"), x.name);
    C99TypeC::encode(w.field("type"), x.type);
    IntC::encode(w.field("byte_offset"), x.byte_offset);
  }
};

}  // namespace

StructLayoutC::type StructLayoutC::decode(const Value& v) {
  const Value& f =
      object_fields(v, tc("StructLayout"), {"tag", "is_union", "members", "size", "complete"});
  return passes::StructLayout{.tag = Str::decode(at(f, 0)),
                              .is_union = BoolC::decode(at(f, 1)),
                              .members = ListOf<MemberInfoC>::decode(at(f, 2)),
                              .size = IntC::decode(at(f, 3)),
                              .complete = BoolC::decode(at(f, 4))};
}
void StructLayoutC::encode(std::string& out, const type& x) {
  ObjectWriter w(out, tc("StructLayout"));
  Str::encode(w.field("tag"), x.tag);
  BoolC::encode(w.field("is_union"), x.is_union);
  ListOf<MemberInfoC>::encode(w.field("members"), x.members);
  IntC::encode(w.field("size"), x.size);
  BoolC::encode(w.field("complete"), x.complete);
}

TypeTableC::type TypeTableC::decode(const Value& v) {
  const Value& f = object_fields(v, tc("TypeTable"), {"_table"});
  return DictOf<Str, StructLayoutC>::decode(at(f, 0));
}
void TypeTableC::encode(std::string& out, const type& x) {
  ObjectWriter w(out, tc("TypeTable"));
  DictOf<Str, StructLayoutC>::encode(w.field("_table"), x);
}

// --- Calling conventions ---

namespace {

constexpr std::string_view kZpLayout = "passes.abi_selection.ZpLayout";
constexpr std::string_view kSoftStack = "passes.abi_selection.SoftStackLayout";

}  // namespace

ZpLayoutC::type ZpLayoutC::decode(const Value& v) {
  const Value& f =
      object_fields(v, kZpLayout, {"slot_symbols", "addrs", "param_registers", "pinned"});
  return passes::ZpLayout{.slot_symbols = ListOf<Str>::decode(at(f, 0)),
                          .addrs = ListOf<OptInt>::decode(at(f, 1)),
                          .param_registers = ListOf<OptStr>::decode(at(f, 2)),
                          .pinned = ListOf<OptInt>::decode(at(f, 3))};
}
void ZpLayoutC::encode(std::string& out, const type& x) {
  ObjectWriter w(out, kZpLayout);
  ListOf<Str>::encode(w.field("slot_symbols"), x.slot_symbols);
  ListOf<OptInt>::encode(w.field("addrs"), x.addrs);
  ListOf<OptStr>::encode(w.field("param_registers"), x.param_registers);
  ListOf<OptInt>::encode(w.field("pinned"), x.pinned);
}

ParamLayoutC::type ParamLayoutC::decode(const Value& v) {
  if (object_class(v) == kSoftStack) {
    object_fields(v, kSoftStack, {});
    return passes::SoftStackLayout{};
  }
  return ZpLayoutC::decode(v);
}
void ParamLayoutC::encode(std::string& out, const type& x) {
  if (const auto* zp = std::get_if<passes::ZpLayout>(&x)) {
    ZpLayoutC::encode(out, *zp);
    return;
  }
  ObjectWriter w(out, kSoftStack);
}

// --- Pools and colorings ---

namespace {

constexpr std::string_view kPool = "passes.optimization.pool.Pool";
constexpr std::string_view kColoring = "passes.optimization.register_allocation.Coloring";
constexpr std::string_view kFrameDims = "passes.replace_pseudoregisters.FrameDims";

}  // namespace

PoolC::type PoolC::decode(const Value& v) {
  const Value& f = object_fields(v, kPool, {"start"});
  return passes::Pool{.start = IntC::decode(at(f, 0))};
}
void PoolC::encode(std::string& out, const type& x) {
  ObjectWriter w(out, kPool);
  IntC::encode(w.field("start"), x.start);
}

ColoringC::type ColoringC::decode(const Value& v) {
  const Value& f =
      object_fields(v, kColoring, {"assignments", "spilled", "pool", "hwreg_assignments"});
  return passes::Coloring{.assignments = DictOf<Str, IntC>::decode(at(f, 0)),
                          .spilled = NameSetC::decode(at(f, 1)),
                          .pool = PoolC::decode(at(f, 2)),
                          .hwreg_assignments = DictOf<Str, Str>::decode(at(f, 3))};
}
void ColoringC::encode(std::string& out, const type& x) {
  ObjectWriter w(out, kColoring);
  DictOf<Str, IntC>::encode(w.field("assignments"), x.assignments);
  NameSetC::encode(w.field("spilled"), x.spilled);
  PoolC::encode(w.field("pool"), x.pool);
  DictOf<Str, Str>::encode(w.field("hwreg_assignments"), x.hwreg_assignments);
}

FrameDimsC::type FrameDimsC::decode(const Value& v) {
  const Value& f = object_fields(v, kFrameDims, {"arg_bytes", "local_bytes", "callee_saved_addrs"});
  return passes::FrameDims{.arg_bytes = IntC::decode(at(f, 0)),
                           .local_bytes = IntC::decode(at(f, 1)),
                           .callee_saved_addrs = ListOf<IntC>::decode(at(f, 2))};
}
void FrameDimsC::encode(std::string& out, const type& x) {
  ObjectWriter w(out, kFrameDims);
  IntC::encode(w.field("arg_bytes"), x.arg_bytes);
  IntC::encode(w.field("local_bytes"), x.local_bytes);
  ListOf<IntC>::encode(w.field("callee_saved_addrs"), x.callee_saved_addrs);
}

// --- The zero-page plan ---

namespace {

constexpr std::string_view kRuntimeLayout = "passes.runtime_layout.RuntimeLayout";
constexpr std::string_view kZpGlobals = "passes.zp_globals.ZpGlobals";
constexpr std::string_view kZpObjectMeta = "passes.zp_link_metadata.ZpObjectMeta";

std::set<asdl::Int> int_set(const Value& v) {
  const auto s = SetOf<IntC>::decode(v);
  return std::set<asdl::Int>(s.begin(), s.end());
}

void encode_int_set(std::string& out, const std::set<asdl::Int>& s) {
  SetOf<IntC>::encode(out, SetOf<IntC>::type(s.begin(), s.end()));
}

}  // namespace

RuntimeLayoutC::type RuntimeLayoutC::decode(const Value& v) {
  const Value& f = object_fields(v, kRuntimeLayout, {"ssp", "fp", "hargs", "hargs_size", "dptr"});
  return passes::RuntimeLayout{.ssp = IntC::decode(at(f, 0)),
                               .fp = IntC::decode(at(f, 1)),
                               .hargs = IntC::decode(at(f, 2)),
                               .hargs_size = IntC::decode(at(f, 3)),
                               .dptr = IntC::decode(at(f, 4))};
}
void RuntimeLayoutC::encode(std::string& out, const type& x) {
  ObjectWriter w(out, kRuntimeLayout);
  IntC::encode(w.field("ssp"), x.ssp);
  IntC::encode(w.field("fp"), x.fp);
  IntC::encode(w.field("hargs"), x.hargs);
  IntC::encode(w.field("hargs_size"), x.hargs_size);
  IntC::encode(w.field("dptr"), x.dptr);
}

ZpGlobalsC::type ZpGlobalsC::decode(const Value& v) {
  const Value& f = object_fields(v, kZpGlobals,
                                 {"addrs", "reserved", "owned", "fixed", "layout", "sizes",
                                  "pinned", "external", "promoted", "param_bytes"});
  using Addrs = DictOf<Str, IntC>;
  return passes::ZpGlobals{.addrs = Addrs::decode(at(f, 0)),
                           .reserved = int_set(at(f, 1)),
                           .owned = NameSetC::decode(at(f, 2)),
                           .fixed = Addrs::decode(at(f, 3)),
                           .layout = RuntimeLayoutC::decode(at(f, 4)),
                           .sizes = Addrs::decode(at(f, 5)),
                           .pinned = NameSetC::decode(at(f, 6)),
                           .external = Addrs::decode(at(f, 7)),
                           .promoted = Addrs::decode(at(f, 8)),
                           .param_bytes = int_set(at(f, 9))};
}
void ZpGlobalsC::encode(std::string& out, const type& x) {
  using Addrs = DictOf<Str, IntC>;
  ObjectWriter w(out, kZpGlobals);
  Addrs::encode(w.field("addrs"), x.addrs);
  encode_int_set(w.field("reserved"), x.reserved);
  NameSetC::encode(w.field("owned"), x.owned);
  Addrs::encode(w.field("fixed"), x.fixed);
  RuntimeLayoutC::encode(w.field("layout"), x.layout);
  Addrs::encode(w.field("sizes"), x.sizes);
  NameSetC::encode(w.field("pinned"), x.pinned);
  Addrs::encode(w.field("external"), x.external);
  Addrs::encode(w.field("promoted"), x.promoted);
  encode_int_set(w.field("param_bytes"), x.param_bytes);
}

ZpObjectMetaC::type ZpObjectMetaC::decode(const Value& v) {
  const Value& f = object_fields(v, kZpObjectMeta,
                                 {"name", "size", "at", "defined", "init", "weight", "local"});
  return passes::ZpObjectMeta{.name = Str::decode(at(f, 0)),
                              .size = IntC::decode(at(f, 1)),
                              .at = OptInt::decode(at(f, 2)),
                              .defined = BoolC::decode(at(f, 3)),
                              .init = ListOf<Str>::decode(at(f, 4)),
                              .weight = OptInt::decode(at(f, 5)),
                              .local = BoolC::decode(at(f, 6))};
}
void ZpObjectMetaC::encode(std::string& out, const type& x) {
  ObjectWriter w(out, kZpObjectMeta);
  Str::encode(w.field("name"), x.name);
  IntC::encode(w.field("size"), x.size);
  OptInt::encode(w.field("at"), x.at);
  BoolC::encode(w.field("defined"), x.defined);
  ListOf<Str>::encode(w.field("init"), x.init);
  OptInt::encode(w.field("weight"), x.weight);
  BoolC::encode(w.field("local"), x.local);
}

// --- Shares ---

namespace {

constexpr std::string_view kSlotShare = "passes.slot_sharing.SlotShare";
using MembersC = TupleOf<Pair<Str, TupleOf<Str>>>;

}  // namespace

SlotShareC::type SlotShareC::decode(const Value& v) {
  const Value& f =
      object_fields(v, kSlotShare, {"members", "holders", "live", "forwards", "writers"});
  return passes::SlotShare{.members = MembersC::decode(at(f, 0)),
                           .holders = TupleOf<Str>::decode(at(f, 1)),
                           .live = MembersC::decode(at(f, 2)),
                           .forwards = TupleOf<Pair<Str, Str>>::decode(at(f, 3)),
                           .writers = TupleOf<Str>::decode(at(f, 4))};
}
void SlotShareC::encode(std::string& out, const type& x) {
  ObjectWriter w(out, kSlotShare);
  MembersC::encode(w.field("members"), x.members);
  TupleOf<Str>::encode(w.field("holders"), x.holders);
  MembersC::encode(w.field("live"), x.live);
  TupleOf<Pair<Str, Str>>::encode(w.field("forwards"), x.forwards);
  TupleOf<Str>::encode(w.field("writers"), x.writers);
}

}  // namespace c6502::tagged

namespace c6502::passes {

double ValueTree::as_double() const {
  double d;
  static_assert(sizeof d == sizeof float_bits);
  std::memcpy(&d, &float_bits, sizeof d);
  return d;
}

}  // namespace c6502::passes
