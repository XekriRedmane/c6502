// The types and helpers of part R2 of the parser's descent, the
// declarations (parser_descent.h): the leaves of a specifier list, the
// declarator trees that parser.py keeps as Lark trees and walks, and
// the functions over them that the files of R2 share.
#pragma once

#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "asdl/runtime.h"
#include "c99_ast.h"
#include "lark_lexer.h"
#include "parser_descent.h"

namespace c6502::parser {

// One leaf of a specifier list: a keyword token, or one of parser.py's
// wrappers, `_TagSpecifier` (a struct or union specifier, with its body
// when it has one), `_TypedefSpecifier` (a typedef name, with a copy of
// its type), `_TypeofSpecifier` and `_EnumSpecifier` (whole types of
// their own, as a typedef name is), and `_BitIntSpecifier`. `type` is
// the terminal that Python's `.type` gives each: the token's own, STRUCT
// or UNION for a tag, TYPEDEF_NAME for a typedef name or a typeof, ENUM
// for an enum, and BITINT for `_BitInt(N)`. A `_Bool` token is a BOOL
// token that keeps its spelling, as `type_specifier` makes it.
struct Spec {
  enum class Kind { kToken, kTag, kTypedef, kTypeof, kEnum, kBitInt };
  Kind kind = Kind::kToken;
  int type = -1;
  // A token's spelling, `str(token)`.
  std::string text;
  // A tag specifier's tag, and its body as (member, type) pairs, which
  // `drain_inline_struct_bodies` empties once it has made the body's
  // StructDecl.
  std::string tag;
  bool is_union = false;
  std::optional<std::vector<std::pair<std::string, Type>>> body;
  // A typedef name, typeof, or enum specifier: the name messages give
  // it, its type, whether that type is an enumerated type, and for an
  // enum, whether it has its list of constants.
  std::string name;
  std::optional<Type> data_type;
  bool enumerated = false;
  bool defines = false;
  // A `_BitInt(N)` specifier's N.
  asdl::Int width = 0;

  // Whether Python's `isinstance(spec, _TypedefSpecifier)` holds, which
  // it does for its subclasses `_TypeofSpecifier` and `_EnumSpecifier`.
  bool is_typedef_like() const {
    return kind == Kind::kTypedef || kind == Kind::kTypeof || kind == Kind::kEnum;
  }
};

// parser.py's `_DeclSpecs`: the specifiers of one declaration, which the
// parse context keeps on a stack from the end of `decl_specs` to the
// action of the declaration that consumes them.
struct DeclSpecs {
  std::vector<Spec> specs;
  bool is_typedef() const;
};

// The storage-class specifier `_split_specifiers` finds: none, one of
// the c99 storage classes, or `typedef`, which parser.py's
// `_TypedefStorage` stands for and which never reaches the AST.
enum class Storage { kNone, kStatic, kExtern, kRegister, kTypedef };

// What `_split_specifiers` returns: the type, or for `auto` with no type
// specifier (parser.py's `_Inferred`), no type and the qualifiers that
// apply to the type the initializer gives, and the storage class.
struct SplitSpecs {
  std::optional<Type> type;
  bool inferred = false;
  std::vector<Spec> qualifiers;
  Storage storage = Storage::kNone;
};

// One parameter of a function suffix, as `parameter_declaration`
// returns it: its name, or none for an unnamed one, its adjusted type,
// and its `param_registers` marker.
struct Param {
  std::optional<std::string> name;
  Type type;
  std::string marker;
};

// An `array_suffix` between brackets: the alias of its alternative
// (`array_size_plain`, `array_size_static`, `array_size_quals_static`,
// or `array_size_unspec`), and for the plain form, its qualifier tokens
// and its size expression, which are its children in Python.
struct ArraySuffix {
  std::string alias;
  std::vector<lark::Token> qualifiers;
  std::optional<Exp> size;
};

// A function suffix's parentheses: empty, `(void)`, a parameter list, or
// a K&R identifier list.
struct FunctionSuffix {
  enum class Kind { kEmpty, kVoid, kParams, kIdentifierList };
  Kind kind = Kind::kEmpty;
  std::vector<Param> params;
};

struct Suffix {
  bool is_array = false;
  ArraySuffix array;
  FunctionSuffix function;
};

struct Declarator;

// A `direct_declarator` or `direct_abstract_declarator`: its core, the
// declared name with the attributes after it, a declarator in
// parentheses, or nothing for an abstract one that starts with a
// suffix, and then its suffixes in source order. Lark nests a suffix's
// direct declarator inside it, so the walk applies the last suffix
// first.
struct Direct {
  enum class Core { kName, kParen, kNone };
  Core core = Core::kNone;
  std::string name;
  std::optional<Attributes> attrs;
  std::shared_ptr<const Declarator> inner;
  std::vector<Suffix> suffixes;
};

// A `declarator` or an `abstract_declarator`: its pointer, as the
// qualifier tokens of each `*` from the first one written, and its
// direct declarator, which an abstract declarator of pointers alone
// doesn't have.
struct Declarator {
  bool abstract = false;
  std::optional<std::vector<std::vector<lark::Token>>> pointer;
  std::optional<Direct> direct;
};

// What `_apply_declarator` returns: the name, the composed type, and the
// names and register markers of the parameters of the function suffix
// closest to the name, which are none when the declarator has no
// function suffix.
struct Applied {
  std::string name;
  Type type;
  std::optional<std::vector<std::optional<std::string>>> params;
  std::optional<std::vector<std::string>> registers;
};

// --- Specifiers and qualifiers (parser_specifiers.cpp) ---

// Returns parser.py's `_resolve_data_type` of the type specifiers
// `type_specs`.
Type resolve_data_type(const std::vector<Spec>& type_specs);
// Returns parser.py's `_split_specifiers` of a declaration's specifiers.
SplitSpecs split_specifiers(const std::vector<Spec>& specs);
// Returns `data_type` with the `const` and `volatile` of `qualifiers`
// (parser.py's `_apply_qualifiers`). Qualifying an array qualifies its
// elements (C99 §6.7.3.8).
Type apply_qualifiers(const Type& data_type, const std::vector<int>& qualifiers);
// Returns the qualifier terminals of `specs`' tokens.
std::vector<int> qualifier_kinds(const std::vector<Spec>& specs);
std::vector<int> qualifier_kinds(const std::vector<lark::Token>& tokens);
// Returns whether the specifiers include `auto`, and `constexpr`.
bool has_auto(const std::vector<Spec>& specs);
bool has_constexpr(const std::vector<Spec>& specs);
// Returns the keyword of a storage class, for messages.
std::string storage_name(Storage storage);
// Returns the c99 storage class of `storage`, which isn't `typedef`.
std::optional<c99_ast::Type_storage_class> storage_class(Storage storage);
// Returns whether `t` is one of the terminals of
// `_TYPE_SPECIFIER_TOKEN_TYPES` or of `_TYPE_QUALIFIER_TOKEN_TYPES`.
bool is_type_specifier_type(int t);
bool is_type_qualifier_type(int t);
// Returns `(tag, is_union)` for each struct and union type in `t`, in
// the order of parser.py's `_type_tags`.
std::vector<std::pair<std::string, bool>> type_tags(const Type& t);
// Returns parser.py's `_unqualified_type`: `t` without its qualifiers,
// at the top and on an array's elements at any depth.
Type unqualified_type(const Type& t);

// --- Attributes (parser_specifiers.cpp) ---

std::optional<Attributes> merge_attributes(const std::optional<Attributes>& a,
                                           const std::optional<Attributes>& b);
// Returns the c6502 attributes of `attrs` after checking each standard
// one against `kind` (parser.py's `_c6502_attributes`), or none.
std::optional<std::vector<std::pair<std::string, AttributeArg>>> c6502_attributes(
    const std::optional<Attributes>& attrs, std::string_view kind, std::string_view where);
void no_c6502_attributes(const std::optional<Attributes>& attrs, std::string_view kind,
                         std::string_view where);
void no_enum_attributes(const std::optional<Attributes>& attrs);
bool is_noreturn(const std::optional<Attributes>& attrs);
// Removes `save_hardware_stack` from a declaration's leading attributes
// and returns whether it was there.
bool take_hw_stack_save(std::optional<Attributes>& attrs);
// Returns the `param_registers` marker of a parameter.
std::string register_marker(
    const std::optional<std::vector<std::pair<std::string, AttributeArg>>>& specs,
    bool is_register);

struct FunctionAttributes {
  std::optional<std::string> abi_annotation;
  bool carry_return = false;
  std::optional<asdl::Int> address;
  std::vector<std::string> preserves;
};
FunctionAttributes extract_function_attributes(
    const std::optional<std::vector<std::pair<std::string, AttributeArg>>>& specs,
    std::string_view where);
std::optional<c99_ast::Type_placement> extract_object_placement(
    const std::optional<std::vector<std::pair<std::string, AttributeArg>>>& specs,
    std::string_view name);
std::optional<std::string> extract_section(
    const std::optional<std::vector<std::pair<std::string, AttributeArg>>>& specs,
    std::string_view where);

// --- Declarators (parser_declarators.cpp) ---

std::string declarator_name(const Declarator& d);
const std::optional<Attributes>& declarator_attributes(const Declarator& d);
Applied apply_declarator(Descent& p, const Declarator& d, Type base);
Type apply_abstract_declarator(Descent& p, const Declarator& d, Type base);
// Returns the parameter type `t` after C99's adjustments of an array and
// a function to a pointer (§6.7.5.3p7-8).
Type adjust_param_type(const Type& t);

}  // namespace c6502::parser
