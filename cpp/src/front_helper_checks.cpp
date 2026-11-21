// The C++ side of the `c99types` records of tools/capture_helpers.py,
// the c99 type queries that the front end's passes share (step 6 of the
// port). The encoding of each value is `front_values`'s: the JSON list
// of one string per type, pair of types, declaration, or name.
#include <string>
#include <vector>

#include "c99_ast.h"
#include "helper_checks.h"
#include "passes/type_checking_types.h"
#include "tagged_json.h"

namespace c6502::helper_checks {

namespace {

namespace ir = ::c6502::c99_ast;
namespace tc = ::c6502::passes::type_checking;
using Type = ir::Type_data_type;

// The values `c99_coerce` reduces, `_COERCE_VALUES` in the harness.
std::vector<asdl::Int> coerce_values() {
  const asdl::Int one = 1;
  return {0,
          1,
          -1,
          2,
          127,
          128,
          -128,
          -129,
          255,
          256,
          32767,
          32768,
          -32768,
          65535,
          65536,
          -65536,
          (one << 31) - 1,
          one << 31,
          -(one << 31),
          (one << 32) - 1,
          one << 32,
          (one << 63) - 1,
          one << 63,
          -(one << 63),
          (one << 64) - 1,
          one << 64,
          -(one << 64),
          asdl::Int(12345678901LL)};
}

std::string list(const std::vector<std::string>& xs) {
  return tagged::encode<tagged::ListOf<tagged::Str>>(xs);
}

const char* bit(bool b) { return b ? "1" : "0"; }

// Returns whether `t` is one of the types `_rank` takes.
bool ranked(const Type& t) {
  return asdl::isa<ir::Int, ir::UInt, ir::Long, ir::ULong, ir::LongLong, ir::ULongLong,
                   ir::BitInt>(t);
}

}  // namespace

Values front_values(const json::Value& v) {
  std::vector<Type> types;
  for (const auto& d : tagged::ListOf<tagged::Str>::decode(*v.get("types"))) {
    types.push_back(asdl::load<Type>(d));
  }
  std::vector<ir::Type_declaration> decls;
  for (const auto& d : tagged::ListOf<tagged::Str>::decode(*v.get("decls"))) {
    auto prog = asdl::load<ir::Program>(d);
    decls.push_back(std::move(prog.declaration.at(0)));
  }
  const auto names = tagged::ListOf<tagged::Str>::decode(*v.get("names"));

  std::vector<std::string> repr_type, repr_decl, strip, promote, predicates, c_type, int_queries,
      rank, coerce, common, display;
  std::vector<Type> arith;
  std::vector<std::string> arith_keys;
  const auto values = coerce_values();
  for (const Type& t : types) {
    repr_type.push_back(asdl::py_repr(t));
    strip.push_back(asdl::py_repr(tc::strip_quals(t)));
    const Type promoted = tc::promote_integer(t);
    promote.push_back(asdl::py_repr(promoted));
    std::string p;
    p += bit(tc::is_integer_type(t));
    p += bit(tc::is_floating_type(t));
    p += bit(tc::is_pointer_type(t));
    p += bit(tc::is_struct_or_union(t));
    p += bit(tc::is_array_type(t));
    p += bit(tc::is_nullptr_type(t));
    p += bit(tc::is_void(t));
    p += bit(tc::is_void_pointer(t));
    predicates.push_back(p);
    c_type.push_back(tc::c_type(t));
    const bool is_int = tc::is_integer_type(t);
    if (is_int) {
      int_queries.push_back(std::to_string(tc::int_width(t)) + "," +
                            std::to_string(tc::int_bits(t)) + "," + bit(tc::is_signed(t)));
      std::string c;
      for (std::size_t i = 0; i < values.size(); ++i) {
        if (i != 0) {
          c += ",";
        }
        c += asdl::int_to_string(tc::coerce_int_to_type(values[i], t));
      }
      coerce.push_back(c);
    } else {
      int_queries.push_back(".");
      coerce.push_back(".");
    }
    if (ranked(t)) {
      const auto [w, o] = tc::rank(t);
      rank.push_back(std::to_string(w) + "," + std::to_string(o));
    } else {
      rank.push_back(".");
    }
    if (is_int || tc::is_floating_type(t)) {
      const std::string key = asdl::py_repr(promoted);
      bool seen = false;
      for (const auto& k : arith_keys) {
        seen = seen || k == key;
      }
      if (!seen) {
        arith_keys.push_back(key);
        arith.push_back(promoted);
      }
    }
  }
  for (const auto& d : decls) {
    repr_decl.push_back(asdl::py_repr(d));
  }
  for (const Type& a : arith) {
    for (const Type& b : arith) {
      common.push_back(asdl::py_repr(tc::common_type(a, b)));
    }
  }
  for (const auto& n : names) {
    display.push_back(tc::display_name(n));
  }
  return {
      {"c99_repr_type", list(repr_type)},
      {"c99_repr_decl", list(repr_decl)},
      {"c99_strip_quals", list(strip)},
      {"c99_promote", list(promote)},
      {"c99_predicates", list(predicates)},
      {"c99_c_type", list(c_type)},
      {"c99_int_queries", list(int_queries)},
      {"c99_rank", list(rank)},
      {"c99_coerce", list(coerce)},
      {"c99_common_type", list(common)},
      {"c99_display_name", list(display)},
  };
}

}  // namespace c6502::helper_checks
