// The tagged JSON of tools/capture_passes.py's `encode`, read into the
// C++ forms of the compile's tables and written back as Python's
// `json.dumps(encode(v))` writes it, byte for byte. cpp/README.md ("The
// compile's tables") describes the representation, and the capture
// harness's docstring each form: JSON's own values for None, bools,
// ints, strings, and lists, `{"tuple": [...]}`, `{"set": [...]}` sorted
// by the elements' JSON, `{"dict": [[k, v], ...]}`, `{"range": [start,
// stop, step]}`, `{"float": bits}`, `{"asdl": module, "dump": text}`,
// and `{"object": "module.Class", "fields": {...}}`.
//
// Each form has a codec, a struct with a `type`, a static `decode`
// from a `json::Value`, and a static `encode` that appends the text.
// The codecs of containers take their elements' codecs as template
// arguments, so `DictOf<Str, ListOf<IntC>>` reads and writes a
// `dict[str, list[int]]` as an `OrderedMap<std::string,
// std::vector<asdl::Int>>`. A value that doesn't have the form its
// codec reads throws `tagged::DecodeError`.
#pragma once

#include <algorithm>
#include <optional>
#include <set>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "asdl/runtime.h"
#include "asm_ast.h"
#include "c99_ast.h"
#include "json.h"
#include "ordered_map.h"
#include "passes/abi_selection.h"
#include "passes/asm_context.h"
#include "passes/optimization/pool.h"
#include "passes/optimization/register_allocation.h"
#include "passes/replace_pseudoregisters.h"
#include "passes/slot_sharing.h"
#include "passes/type_checking.h"
#include "passes/zp_globals.h"
#include "passes/zp_link_metadata.h"
#include "tac_ast.h"

namespace c6502::tagged {

class DecodeError : public std::runtime_error {
 public:
  using std::runtime_error::runtime_error;
};

// --- Writing ---

// Appends `s` as Python's `json.dumps` writes a `str`, with every
// character outside printable ASCII escaped.
void write_string(std::string& out, std::string_view s);
// Appends any JSON value as `json.dumps` writes it, with ", " and ": "
// between items and the members of an object in their order.
void write_value(std::string& out, const json::Value& v);
// Returns `write_value`'s text of `v`.
std::string to_text(const json::Value& v);

// --- Reading ---

// Returns the elements of a JSON array.
const std::vector<json::Value>& array_items(const json::Value& v);
// Returns the elements of the tagged form `{tag: [...]}`.
const std::vector<json::Value>& tagged_items(const json::Value& v, std::string_view tag);
// Returns the `fields` object of `{"object": cls, "fields": {...}}`,
// checking the class and that the fields are `names`, in that order.
const json::Value& object_fields(const json::Value& v, std::string_view cls,
                                 std::initializer_list<std::string_view> names);
// Returns the class name of an object's encoding, or empty for any
// other value.
std::string_view object_class(const json::Value& v);

// --- Codecs of Python's own values ---

struct IntC {
  using type = asdl::Int;
  static type decode(const json::Value& v);
  static void encode(std::string& out, const type& x);
};

struct Str {
  using type = std::string;
  static type decode(const json::Value& v);
  static void encode(std::string& out, const type& x);
};

struct BoolC {
  using type = bool;
  static type decode(const json::Value& v);
  static void encode(std::string& out, const type& x);
};

// A Python float, as the bits of the IEEE 754 double.
struct FloatC {
  using type = std::uint64_t;
  static type decode(const json::Value& v);
  static void encode(std::string& out, const type& x);
};

struct RangeC {
  using type = passes::Range;
  static type decode(const json::Value& v);
  static void encode(std::string& out, const type& x);
};

// A value that may be None.
template <class C>
struct Optional {
  using type = std::optional<typename C::type>;
  static type decode(const json::Value& v) {
    if (v.kind == json::Value::Kind::kNull) {
      return std::nullopt;
    }
    return C::decode(v);
  }
  static void encode(std::string& out, const type& x) {
    if (!x) {
      out += "null";
    } else {
      C::encode(out, *x);
    }
  }
};

namespace detail {

template <class C, class Items>
std::vector<typename C::type> decode_items(const Items& items) {
  std::vector<typename C::type> out;
  out.reserve(items.size());
  for (const auto& item : items) {
    out.push_back(C::decode(item));
  }
  return out;
}

template <class C, class Seq>
void encode_items(std::string& out, const Seq& xs) {
  out += '[';
  bool first = true;
  for (const auto& x : xs) {
    if (!first) {
      out += ", ";
    }
    first = false;
    C::encode(out, x);
  }
  out += ']';
}

}  // namespace detail

template <class C>
struct ListOf {
  using type = std::vector<typename C::type>;
  static type decode(const json::Value& v) { return detail::decode_items<C>(array_items(v)); }
  static void encode(std::string& out, const type& x) { detail::encode_items<C>(out, x); }
};

template <class C>
struct TupleOf {
  using type = std::vector<typename C::type>;
  static type decode(const json::Value& v) {
    return detail::decode_items<C>(tagged_items(v, "tuple"));
  }
  static void encode(std::string& out, const type& x) {
    out += "{\"tuple\": ";
    detail::encode_items<C>(out, x);
    out += '}';
  }
};

// A set or a frozenset, held sorted by the C++ order of its elements
// and written sorted by their JSON text, as the harness writes it.
// Sorting by text agrees with Python's `json.dumps(e, sort_keys=True)`
// for the strings, ints, and tuples of them that the tables hold.
template <class C>
struct SetOf {
  using type = std::set<typename C::type, std::less<>>;
  static type decode(const json::Value& v) {
    type out;
    for (const auto& item : tagged_items(v, "set")) {
      out.insert(C::decode(item));
    }
    return out;
  }
  static void encode(std::string& out, const type& x) {
    std::vector<std::string> texts;
    texts.reserve(x.size());
    for (const auto& e : x) {
      std::string t;
      C::encode(t, e);
      texts.push_back(std::move(t));
    }
    std::sort(texts.begin(), texts.end());
    out += "{\"set\": [";
    for (std::size_t i = 0; i < texts.size(); ++i) {
      if (i != 0) {
        out += ", ";
      }
      out += texts[i];
    }
    out += "]}";
  }
};

template <class K, class V>
struct DictOf {
  using type = OrderedMap<typename K::type, typename V::type>;
  static type decode(const json::Value& v) {
    type out;
    for (const auto& kv : tagged_items(v, "dict")) {
      const auto& pair = array_items(kv);
      if (pair.size() != 2) {
        throw DecodeError("a dict entry isn't a [key, value] pair");
      }
      out.insert_or_assign(K::decode(pair[0]), V::decode(pair[1]));
    }
    return out;
  }
  static void encode(std::string& out, const type& x) {
    out += "{\"dict\": [";
    bool first = true;
    for (const auto& [k, v] : x) {
      if (!first) {
        out += ", ";
      }
      first = false;
      out += '[';
      K::encode(out, k);
      out += ", ";
      V::encode(out, v);
      out += ']';
    }
    out += "]}";
  }
};

// A tuple of two values of different forms.
template <class A, class B>
struct Pair {
  using type = std::pair<typename A::type, typename B::type>;
  static type decode(const json::Value& v) {
    const auto& items = tagged_items(v, "tuple");
    if (items.size() != 2) {
      throw DecodeError("a pair isn't a tuple of two");
    }
    return {A::decode(items[0]), B::decode(items[1])};
  }
  static void encode(std::string& out, const type& x) {
    out += "{\"tuple\": [";
    A::encode(out, x.first);
    out += ", ";
    B::encode(out, x.second);
    out += "]}";
  }
};

// An IR value of module `Module` and C++ type `T`, as its dump.
template <class T, const char* Module>
struct Asdl {
  using type = T;
  static type decode(const json::Value& v) {
    const json::Value* m = v.get("asdl");
    const json::Value* d = v.get("dump");
    if (m == nullptr || d == nullptr || m->string != Module) {
      throw DecodeError(std::string("expected an IR value of ") + Module);
    }
    return asdl::load<T>(d->string);
  }
  static void encode(std::string& out, const type& x) {
    out += "{\"asdl\": \"";
    out += Module;
    out += "\", \"dump\": ";
    write_string(out, asdl::dump(x));
    out += '}';
  }
};

inline constexpr char kAsmAst[] = "asm_ast";
inline constexpr char kTacAst[] = "tac_ast";
inline constexpr char kC99Ast[] = "c99_ast";

using AsmProgramC = Asdl<asm_ast::Program, kAsmAst>;
using TacProgramC = Asdl<tac_ast::Program, kTacAst>;
using C99ProgramC = Asdl<c99_ast::Program, kC99Ast>;
using C99TypeC = Asdl<c99_ast::Type_data_type, kC99Ast>;

// --- Codecs of the compile's tables ---

#define C6502_TAGGED_CODEC(Name, Type)                    \
  struct Name {                                           \
    using type = Type;                                    \
    static type decode(const json::Value& v);             \
    static void encode(std::string& out, const type& x); \
  }

C6502_TAGGED_CODEC(SymbolC, passes::Symbol);
C6502_TAGGED_CODEC(SymbolTableC, passes::SymbolTable);
C6502_TAGGED_CODEC(StructLayoutC, passes::StructLayout);
C6502_TAGGED_CODEC(TypeTableC, passes::TypeTable);
C6502_TAGGED_CODEC(ParamLayoutC, passes::ParamLayout);
C6502_TAGGED_CODEC(ZpLayoutC, passes::ZpLayout);
C6502_TAGGED_CODEC(PoolC, passes::Pool);
C6502_TAGGED_CODEC(ColoringC, passes::Coloring);
C6502_TAGGED_CODEC(FrameDimsC, passes::FrameDims);
C6502_TAGGED_CODEC(RuntimeLayoutC, passes::RuntimeLayout);
C6502_TAGGED_CODEC(ZpGlobalsC, passes::ZpGlobals);
C6502_TAGGED_CODEC(ZpObjectMetaC, passes::ZpObjectMeta);
C6502_TAGGED_CODEC(SlotShareC, passes::SlotShare);

#undef C6502_TAGGED_CODEC

using AbiC = DictOf<Str, ParamLayoutC>;
using NameSetC = SetOf<Str>;
using BindingsC = DictOf<Str, DictOf<Str, TupleOf<Str>>>;

// Returns the text `C` writes for the value `C` reads from `v`, which
// is the text Python wrote when the value makes the trip unchanged.
template <class C>
std::string round_trip(const json::Value& v) {
  std::string out;
  C::encode(out, C::decode(v));
  return out;
}

// Returns the text `C` writes for `x`.
template <class C>
std::string encode(const typename C::type& x) {
  std::string out;
  C::encode(out, x);
  return out;
}

}  // namespace c6502::tagged
