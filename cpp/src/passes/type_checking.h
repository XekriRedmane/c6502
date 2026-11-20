// The tables that passes/type_checking.py builds and code generation
// reads: the symbol table, with each symbol's type and attributes, and
// the type table of struct and union layouts. The type checker itself
// isn't ported, so for now these are filled from the capture harness's
// tagged JSON (src/tagged_json.h), and cpp/README.md ("The compile's
// tables") describes the representation.
#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

#include "asdl/runtime.h"
#include "c99_ast.h"
#include "ordered_map.h"
#include "passes/asm_context.h"
#include "tac_ast.h"

namespace c6502::passes {

// A static pointer initializer `&name + offset`, the address of another
// static object or of a function (Python's `AddressInit`).
struct AddressInit {
  std::string name;
  asdl::Int offset = 0;
  bool operator==(const AddressInit&) const = default;
};

// The value of an `Initial` initializer, a tree as Python's `value`
// field holds it: an integer, a float (a Python float, which is an
// IEEE 754 double, held as its bits so that a NaN's payload and the
// sign of a zero make the trip unchanged), an `AddressInit`, a tuple of
// trees for an array, a struct, or a union's first member, or a
// `UnionValue`, which is the index of the member a designator
// initializes and that member's tree. The tables of row addresses and
// the split tables that `-O` makes hold TAC initializers too, such as
// an `AddressByteInit` for one byte of an address
// (passes/optimization/split_tables.py).
struct ValueTree {
  enum class Kind { kInt, kFloat, kAddress, kTuple, kUnion, kTacInit };
  Kind kind = Kind::kInt;
  asdl::Int integer = 0;
  std::uint64_t float_bits = 0;
  AddressInit address;
  std::optional<tac_ast::Type_static_init> tac_init;
  // The elements of a tuple, or the one tree of a union's member.
  std::vector<ValueTree> items;
  // The member a `UnionValue` initializes.
  asdl::Int member = 0;

  // Returns the float this tree holds.
  double as_double() const;
  bool operator==(const ValueTree&) const = default;
};

// A file-scope object declared without an initializer (C99 §6.9.2.2).
struct Tentative {
  bool operator==(const Tentative&) const = default;
};
// An object declared with an initializer, with the constant value of
// it.
struct Initial {
  ValueTree value;
  bool operator==(const Initial&) const = default;
};
// `extern T x;`, whose definition is elsewhere.
struct NoInitializer {
  bool operator==(const NoInitializer&) const = default;
};
using InitialValue = std::variant<Tentative, Initial, NoInitializer>;

// An object with automatic storage (Python's `LocalAttr`). `register_`
// is the `register` keyword, and `was_bool` that its type was `bool`
// before `c99_to_tac` made it `unsigned char`.
struct LocalAttr {
  bool register_ = false;
  bool was_bool = false;
  bool operator==(const LocalAttr&) const = default;
};

// An object with static storage (Python's `StaticAttr`, whose
// docstring gives the meaning of each field). `planes` is None except
// on the tables of row addresses that `-O` makes.
struct StaticAttr {
  InitialValue initial_value;
  bool is_global = false;
  bool zeropage = false;
  std::optional<asdl::Int> zp_address;
  std::optional<asdl::Int> address;
  std::optional<std::string> section;
  std::optional<std::vector<std::string>> planes;
  bool mergeable = false;
  bool was_bool = false;
  bool operator==(const StaticAttr&) const = default;
};

// A function name (Python's `FunAttr`). `preserves` holds "X" and "Y"
// at most, and `param_zeropage` has an entry for each parameter, the
// address `zeropage(addr)` gives it or none, or is empty when no
// parameter has one.
struct FunAttr {
  bool defined = false;
  bool is_global = false;
  bool carry_return = false;
  std::optional<asdl::Int> address;
  NameSet preserves;
  std::optional<std::string> section;
  std::vector<std::optional<asdl::Int>> param_zeropage;
  bool returns_bool = false;
  bool noreturn = false;
  bool operator==(const FunAttr&) const = default;
};

using IdAttr = std::variant<LocalAttr, StaticAttr, FunAttr>;

// One row of the symbol table: the identifier's type and attributes.
struct Symbol {
  c99_ast::Type_data_type type;
  IdAttr attrs;
  bool operator==(const Symbol&) const = default;
};

// The symbol table, by resolved name, in the order Python's dict holds
// the names. Several passes walk it in that order (`plan_zero_page`
// places the zero-page objects in it), and `tac_to_asm` adds the
// `<param>.reg` locals at its end.
using SymbolTable = OrderedMap<std::string, Symbol>;

// One member of a struct or union's layout: its name, its declared
// type, and its byte offset (0 for every member of a union).
struct MemberInfo {
  std::string name;
  c99_ast::Type_data_type type;
  asdl::Int byte_offset = 0;
  bool operator==(const MemberInfo&) const = default;
};

// The layout of one struct or union tag. A tag declared without a body
// has `complete` false, no members, and size 0.
struct StructLayout {
  std::string tag;
  bool is_union = false;
  std::vector<MemberInfo> members;
  asdl::Int size = 0;
  bool complete = false;
  bool operator==(const StructLayout&) const = default;
};

// Every struct and union tag of the program with its layout, in the
// order of Python's table.
using TypeTable = OrderedMap<std::string, StructLayout>;

}  // namespace c6502::passes
