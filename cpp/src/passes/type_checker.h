// The port of passes/type_checking.py's checker: the `TypeChecker` class
// and the module's free functions, which the four parts of the port
// share (docs/cpp_port.md, "Step 6", group T). The type queries of step
// 6a are in passes/type_checking_types.h, and the symbol and type tables
// in passes/type_checking.h.
//
// The checker stamps the program it is given in place, as Python's
// does. Each method that Python gives an expression and that replaces
// fields of it takes a reference to the expression's slot. A Python
// function that returns a new expression wrapping its argument, such as
// `_convert_to` and `_decay_if_array`, takes the expression by value and
// returns the result, so a caller writes `e = convert_to(std::move(e),
// t)`.
//
// The parser's folds (`_fold_integer_constant`, `_expression_type`, and
// `_fold_named_constant`, parser.py:2422-2663) make a checker of their
// own, fill `symbols`, `types` (through `check_struct_decl`), and
// `constexpr_values`, install `named_constant` as the resolver of
// constant_expression.h, and call `check_exp` or `check_block_var`. Those
// members are public for that reason.
//
// The file of each part:
// - type_checker.cpp (T-A): the free functions of type_checking.py:127-490
//   and 1200-2406, and the class's tag scopes and `require_*` methods.
// - type_checker_init.cpp (T-B): static initializers and `constexpr`,
//   type_checking.py:491-1199 and 3637-4438, with designators.cpp.
// - type_checker_decls.cpp (T-C): `check_program`, the file-scope and
//   struct declarations, layouts, functions, calls and the builtins'
//   checks, and statements, type_checking.py:2538-3636 and 4439-4775,
//   with stdbit_functions.cpp.
// - type_checker_exp.cpp (T-D): expressions, type_checking.py:4776-6197.
#pragma once

#include <map>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <tuple>
#include <utility>
#include <vector>

#include "asdl/runtime.h"
#include "c99_ast.h"
#include "errors.h"
#include "passes/constant_expression.h"
#include "passes/type_checking.h"
#include "passes/type_checking_types.h"

namespace c6502::passes::type_checking {

using Exp = c99_ast::Type_exp;
using Int = asdl::Int;
using constant_expression::NamedConstant;

// Raised on any type-level inconsistency in the program (Python's
// `TypeCheckError`).
class TypeCheckError : public CompileError {
 public:
  explicit TypeCheckError(const std::string& message)
      : CompileError("TypeCheckError", message) {}
};

// ---------------------------------------------------------------------
// The free functions of T-A (type_checker.cpp).
// ---------------------------------------------------------------------

// Returns Python's repr of an optional type, "None" when it is none, as
// `{exp.data_type!r}` shows one in a message.
std::string repr_or_none(const std::optional<Type>& t);
// Returns `c_type` of an optional type, or "NoneType" for none, as
// Python's `_c_type(None)` gives.
std::string c_type_or_none(const std::optional<Type>& t);

// Returns the `data_type` field of `exp`, which a `String` and a
// `Designation` don't have, so they give none. Python reads the field
// of those two only where it raises AttributeError, which no program of
// the corpus reaches.
const std::optional<Type>& data_type_of(const Exp& exp);
// Returns the `data_type` field of `exp` to stamp, or null for a
// `String` or a `Designation`.
std::optional<Type>* data_type_slot(Exp& exp);

// Returns whether the unqualified `t` is a type the type-generic
// <stdbit.h> functions take (C23 §7.18.3p2): a standard unsigned integer
// type other than `bool`, or `unsigned _BitInt(N)` for N of 8, 16, 32,
// or 64.
bool is_stdbit_generic_type(const Type& t);
// Returns the parameter type of the <stdbit.h> function with the suffix
// `suffix` ("uc", "us", "ui", "ul", or "ull"), Python's
// `_STDBIT_PARAMETER_TYPES`.
Type stdbit_parameter_type(std::string_view suffix);

// Returns whether `a` and `b` are the same type, field for field.
bool types_equal(const Type& a, const Type& b);

// Returns Python's `str` of a value tree, as a message shows
// `Initial.value`: an int in decimal, a tuple as `(1, 2)` or `(1,)`,
// and an `AddressInit` or a `UnionValue` as its dataclass repr.
std::string value_str(const ValueTree& value);

// Returns the initial value of a file-scope object `name` after another
// declaration: an `Initial` over a `Tentative` over a `NoInitializer`.
// Two `Initial`s are a redefinition, which throws.
InitialValue merge_initial_value(const std::string& name, const InitialValue& old,
                                 const InitialValue& next);

// Returns whether `t` is a scalar value type that can be an operand
// without decay: the arithmetic types, pointers, and `nullptr_t` (C99
// §6.2.5, C23 §6.2.5p26). An array, a function, a struct, and a union
// aren't, since an array decays before every operand site checks this.
bool is_object_type(const Type& t);
// Returns `is_object_type`, or whether `t` is an array, a struct, or a
// union, the types a declaration may give an object.
bool is_complete_object_type(const Type& t);
// Returns whether `t` is an integer or floating type.
bool is_arithmetic_type(const Type& t);

// Returns whether `op` always gives 0 or 1: a comparison, `&&`, or `||`
// (Python's `_TRUTH_OPS`).
bool is_truth_op(const c99_ast::Type_binary_operator& op);
// Returns the spelling of the binary operator `op`, such as "<<".
std::string_view binary_op_spelling(const c99_ast::Type_binary_operator& op);
// Returns whether `exp`, not yet type-checked, always gives 0 or 1: a
// comparison, `&&`, `||`, `!`, or the constant 0 or 1, which a
// `return_bool_in_carry` function returns as it is.
bool is_truth_valued(const Exp& exp);

// Throws for a definition of an object with `[[c6502::address(...)]]`,
// which names memory the program doesn't own.
void check_unowned_object(const std::string& name, const InitialValue& initial,
                          const std::optional<Int>& address);
// Throws for `preserves_x` or `preserves_y` on a function without an
// address, and for `preserves_x` on a function whose result comes back
// with a high byte in X. `types` may be null.
void check_preserves(const std::string& name, const Type& ftype,
                     const std::optional<Int>& address, const NameSet& preserves,
                     const TypeTable* types);
// Returns the size of the value member, 1 or 2, when `ret` is a struct
// that a `return_bool_in_carry` function may return (exactly two
// members, an integer of 1 or 2 bytes and then a `bool`), or none.
// `types` may be null.
std::optional<int> carry_struct_value_bytes(const Type& ret, const TypeTable* types);
// Returns the section of `name` after another declaration, which has to
// agree with the earlier one when both give one.
std::optional<std::string> merge_section(const std::string& name,
                                         const std::optional<std::string>& prior,
                                         const std::optional<std::string>& next);
// Throws when a section goes with `address` or `zeropage`.
void check_section(const std::string& name, const std::optional<std::string>& section,
                   const std::optional<Int>& address,
                   const std::optional<c99_ast::Type_placement>& placement);
// Throws for `return_bool_in_carry` on a function whose return type
// isn't `bool` or a struct of a value and a `bool`. `types` may be null.
void check_carry_return_type(const std::string& name, const Type& ret, const TypeTable* types);

// Returns `t` with every `Const` and `Volatile` removed at any depth,
// for comparing types modulo qualifiers.
Type strip_quals_recursive(const Type& t);
// Returns whether `t` has a top-level `Const`, under a `Volatile` or
// not.
bool is_const_qualified(const Type& t);
// Returns whether `t` has a top-level `Volatile`, under a `Const` or
// not.
bool is_volatile_qualified(const Type& t);
// Returns the type of a member of type `member_t` reached through a
// container of type `container_t`, with the container's qualifiers
// (C99 §6.5.2.3p3). Qualifying an array qualifies its elements.
Type propagate_quals(const Type& member_t, const Type& container_t);
// Returns the type two pointer operands meet at, or none when their
// pointees aren't compatible (C99 §6.5.8p2, §6.5.9p2, §6.5.15p6).
std::optional<Type> composite_pointer(const Type& tl, const Type& tr);

// Returns the bytes a value of type `t` occupies. A struct or union
// needs a complete layout in `types`, which may be null. Throws for
// `void`, a function type, and an incomplete struct or union.
Int sizeof_type(const Type& t, const TypeTable* types = nullptr);

// Throws unless a `register` object of type `t` fits in at most
// `max_bytes` registers. `where` begins the message.
void check_register_object(const Type& t, int max_bytes, std::string_view where);
// Throws unless a parameter of type `t` with an `in_*` attribute naming
// the registers `regs` ("Y", "YA", ...) is an integer or a pointer with
// one byte for each register.
void check_named_register_param(const Type& t, std::string_view regs, std::string_view where);

// The checker's tag scopes as `check_well_formed_type` reads them:
// `visible` says whether a tag is in a live scope, and `introduce`
// declares a tag never seen before as incomplete in the innermost one.
class TypeChecker;

// Throws if the declared or named type `t` isn't well formed (C99
// §6.7.5.2p1): an array of `void`, of functions, or of an incomplete
// type, an incomplete array where `require_complete` is set, or a
// struct or union tag that is undeclared, out of scope, of the other
// kind, or incomplete where `require_complete` is set. `types` null
// skips the checks of tags. With `checker` set, a tag not in scope is
// rejected, and a tag never declared is declared incomplete in the
// checker's innermost tag scope (Python's `tag_visible` and
// `auto_introduce` arguments).
void check_well_formed_type(const Type& t, std::string_view where, const TypeTable* types,
                            bool require_complete = false, TypeChecker* checker = nullptr);

// Throws when `t` is a function type, which can't be the target of
// `what` (an assignment, a compound assignment, `++`, or `--`).
void require_not_function(const Type& t, std::string_view what);

// Returns `exp` converted from an array to a pointer to its first
// element, or from a function to a pointer to it (C99 §6.3.2.1p3-4), as
// an `AddressOf` stamped with the pointer type, or `exp` unchanged.
Exp decay_if_array(Exp exp);

// Returns whether the unqualified `t` is a pointer to a function.
bool is_pointer_to_function(const Type& t);
// Returns the name of the function or function pointer that the callee
// `exp` calls through `*` and `&` that don't change which function it
// is, or none.
std::optional<std::string> callee_name(const Exp& exp, const SymbolTable& symbols);

// Returns whether `exp` is a null pointer constant as the pointer
// conversions read one: an integer constant 0 under any casts,
// `nullptr`, or a named constant of integer type and value 0.
bool is_null_pointer_constant(const Exp& exp);
// Returns whether the type-checked `exp` is a null pointer constant as
// C23 §6.3.2.3p3 defines one, the test for converting to `nullptr_t`.
bool is_c23_null_pointer_constant(const Exp& exp);

// Returns the integer constant of `t`'s variant holding `value`, which
// `coerce_int_to_type` reduced, as a nonnegative bit pattern.
c99_ast::Type_const const_for_value(Int value, const Type& t);

// Returns `exp` converted to `target` as by assignment (C99 §6.5.16.1):
// unchanged when its type is `target` modulo qualifiers, and otherwise
// in an implicit `Cast` stamped with `target`. Throws for a conversion
// that only an explicit cast may make, or none may.
Exp convert_to(Exp exp, const Type& target);

// ---------------------------------------------------------------------
// The free functions of T-B (type_checker_init.cpp).
// ---------------------------------------------------------------------

// Python's `_linkage_to_is_global` is `linkage is Linkage.EXTERNAL`,
// which T-C writes where it computes the linkage.

// Returns the IEEE 754 bits or the integer that the constant `c`
// initializes an object of type `target_type` with.
Int coerce_init_const_value(const c99_ast::Type_const& c, const Type& target_type);
// Returns `value`, of type `source_type`, converted to `target_type` as
// a static initializer's value: an integer, or the IEEE 754 bits of a
// floating type.
Int coerce_init_value(Int value, const Type& source_type, const Type& target_type);
// Returns the type of the constant `c`.
Type const_source_type(const c99_ast::Type_const& c);
// Returns the address constant `exp` initializes a static pointer with.
// `symbols` and `types` may be null.
AddressInit address_constant(const Exp& exp, const std::string& name, const SymbolTable* symbols,
                             const TypeTable* types);
// Returns the offset of an address computed from a null pointer, as in
// `&((struct s *)0)->m`, or none.
std::optional<Int> null_based_offset(const Exp& exp, const TypeTable* types);
// Returns the value tree of the scalar initializer `exp` of the static
// object `name` of type `target_type`: an integer, the bits of a
// floating value, or an `AddressInit`.
ValueTree const_init_value(const Exp& exp, const Type& target_type, const std::string& name,
                           const SymbolTable* symbols = nullptr, const TypeTable* types = nullptr);
// Returns the value tree of an object of type `t` with every byte zero.
ValueTree zero_aggregate(const Type& t, const TypeTable* types = nullptr);
// Returns whether `t` is `char`, `signed char`, or `unsigned char`.
bool is_char_element(const Type& t);
// Returns `FunAttr.param_zeropage` from a function_decl's
// `param_registers`, where a `zeropage(addr)` parameter is "@addr".
std::vector<std::optional<Int>> param_zeropage(const std::vector<std::string>& markers);
// Returns `t` completed by its initializer when it is an array of
// unknown size (C99 §6.7.8p22). `init` may be null.
Type size_from_initializer(const Type& t, const Exp* init, std::string_view what);
// Returns the constant that `{}` gives a scalar of type `t`.
Exp empty_init_zero(const Type& t);
// Returns the expression that initializes a scalar of type `t` from its
// initializer `init` (C23 §6.7.11p12).
Exp unbrace_scalar_init(Exp init, const Type& t, std::string_view what);
// Returns the composite of two array types of one element type, one of
// which may have unknown size, or none.
std::optional<Type> composite_array(const Type& a, const Type& b);
// Returns the value tree of a string literal that initializes an array
// of `n` characters.
ValueTree string_to_value_tuple(const std::string& s, Int n);
// Returns the value tree of the initializer `init` of the static object
// `name` of type `var_type`.
ValueTree const_init_aggregate(const Exp& init, const Type& var_type, const std::string& name,
                               const SymbolTable* symbols = nullptr,
                               const TypeTable* types = nullptr);
// Returns the index in `layout` of the member a union's braced list
// initializes.
int union_member_index(const StructLayout& layout, const c99_ast::InitList& init);

// ---------------------------------------------------------------------
// The checker.
// ---------------------------------------------------------------------

// Walks one program and fills `symbols` and `types`, which accumulate
// over its declarations. Tag visibility is per block: `tag_scopes` is a
// stack of sets, pushed for each block scope (compound statements,
// `for` headers, and function bodies) and popped at its end, with the
// file scope at the bottom. A `Structure(tag)` is valid only while its
// tag is in a live scope. The layouts are flat in `types`, so a block
// can't shadow a tag.
class TypeChecker {
 public:
  SymbolTable symbols;
  TypeTable types;
  // The type the enclosing function returns, while a body is checked.
  std::optional<Type> return_type;
  // Whether the body being checked is a `return_bool_in_carry`
  // function's, whose `return e;` returns `e != 0`.
  bool carry_return = false;
  // The tag scopes, the file scope first.
  std::vector<std::set<std::string, std::less<>>> tag_scopes{1};
  // Each `constexpr` object checked so far, by name, with its type and
  // its value tree in the form of `Initial.value`, which
  // `named_constant` gives the evaluator (Python's `_constexpr`).
  std::map<std::string, NamedConstant, std::less<>> constexpr_values;

  // -------------------------------------------------------------------
  // T-A (type_checker.cpp).
  // -------------------------------------------------------------------
  void push_tag_scope();
  void pop_tag_scope();
  bool tag_visible(std::string_view tag) const;
  void record_tag_visible(const std::string& tag);
  // Declares the tag of the struct or union `t` as incomplete in the
  // innermost scope, the rule that a `struct foo` in any declaration
  // introduces the tag. Only called for a tag never seen.
  void auto_introduce_tag(const Type& t);
  // Throws unless the controlling expression `exp` of an `if`, a loop,
  // or `?:` has scalar type or is an array, which decays (C99
  // §6.8.4.1p1). `where` begins the message.
  void require_scalar_controlling(const Exp& exp, std::string_view where) const;
  // Throws when `exp` has an incomplete struct or union type, at sites
  // that read or write its value (C99 §6.7.2.1p8).
  void require_complete_value(const Exp& exp, std::string_view where) const;
  // Throws for pointer arithmetic (`where`) on a pointer to `void`, to a
  // function, or to an incomplete struct or union (C99 §6.5.6p2).
  void require_complete_pointee(const Type& ptr, std::string_view where) const;
  // Calls `check_well_formed_type` with this checker's types and tag
  // scopes.
  void check_well_formed(const Type& t, std::string_view where, bool require_complete = false);

  // -------------------------------------------------------------------
  // T-B (type_checker_init.cpp).
  // -------------------------------------------------------------------
  void check_block(c99_ast::Type_block& block);
  void check_block_item(c99_ast::Type_block_item& item);
  void check_block_declaration(c99_ast::Type_declaration& decl);
  // Returns the initializer `init` of the object `name` (none for a
  // compound literal) of type `t` with its designations in positional
  // form (designators.h).
  Exp normalize_init(Exp init, const Type& t, const std::optional<std::string>& name);
  void check_block_var(c99_ast::Type_var_decl& vd);
  void check_block_var_body(c99_ast::Type_var_decl& vd);
  // Returns the type and value of the named constant `exp`, a
  // `constexpr` object, a compound literal constant, or a `.` member of
  // one, or none. Throws ConstantExpressionError for a member of a union
  // constant other than the one its initializer sets.
  std::optional<NamedConstant> named_constant(const Exp& exp);
  std::optional<ValueTree> structure_constant_init(c99_ast::Type_var_decl& vd);
  void begin_constexpr(c99_ast::Type_var_decl& vd);
  void end_constexpr(c99_ast::Type_var_decl& vd);
  static bool has_address(const ValueTree& value);
  std::optional<std::string> volatile_part(const Type& t, std::set<std::string>* seen = nullptr);
  void check_constexpr_init(const Exp& init, const Type& t, const std::string& name);
  void check_constexpr_string(const c99_ast::String& s, const Type& element,
                              const std::string& name);
  Type check_compound_literal(c99_ast::CompoundLiteral& exp);
  void check_string_array_init(c99_ast::String& init, const c99_ast::Array& arr_type,
                               const std::string& name);
  void check_struct_init_list(c99_ast::InitList& init, const Type& struct_type,
                              const std::string& var_name);
  void check_array_init_list(c99_ast::InitList& init, const c99_ast::Array& arr_type,
                             const std::string& var_name);

  // -------------------------------------------------------------------
  // T-C (type_checker_decls.cpp).
  // -------------------------------------------------------------------
  void check_program(c99_ast::Program& prog);
  void check_file_scope_declaration(c99_ast::Type_declaration& decl);
  void check_struct_decl(c99_ast::Type_struct_decl& sd);
  StructLayout compute_layout(const c99_ast::Type_struct_decl& sd);
  bool contains_self_struct(const Type& t, const std::string& tag) const;
  MemberInfo lookup_member(const Type& t, const std::string& member, std::string_view where) const;
  void check_file_scope_var_well_formed(c99_ast::Type_var_decl& vd);
  void check_file_scope_var(c99_ast::Type_var_decl& vd);
  void check_file_scope_var_body(c99_ast::Type_var_decl& vd);
  void check_function_decl(c99_ast::Type_function_decl& fd, bool file_scope);
  void add_or_merge_static_object(const std::string& name, const Type& type,
                                  const InitialValue& initial, bool is_global,
                                  const std::optional<c99_ast::Type_placement>& placement = {},
                                  const std::optional<std::string>& section = {});
  // Checks and converts the arguments `args` of a call against
  // `fn_type`, stamps the call's type in `call_type`, and returns it.
  // `label` names the callee in messages.
  Type check_call_args(std::optional<Type>& call_type, std::string_view label,
                       const c99_ast::FunType& fn_type, std::vector<Exp>& args);
  Type check_builtin_call(c99_ast::FunctionCall& exp);
  Type check_stdbit_call(c99_ast::FunctionCall& exp);
  Type check_bin_to_bcd_call(c99_ast::FunctionCall& exp);
  Type check_bcd_call(c99_ast::FunctionCall& exp);
  c99_ast::FunType check_callee(c99_ast::IndirectCall& exp);
  bool is_bool_var(const Exp& exp) const;
  bool is_carry_call(const Exp& exp) const;
  void add_or_merge_function(const std::string& name, const c99_ast::FunType& ftype, bool defined,
                             bool is_global, bool carry_return = false,
                             const std::optional<Int>& address = {},
                             const NameSet& preserves = {},
                             const std::optional<std::string>& section = {},
                             const std::vector<std::optional<Int>>& param_zeropage = {},
                             bool noreturn = false);
  void check_statement(c99_ast::Type_statement& stmt);
  void check_switch(c99_ast::SwitchStmt& stmt);
  void check_for_init(c99_ast::Type_for_init& init);

  // -------------------------------------------------------------------
  // T-D (type_checker_exp.cpp).
  // -------------------------------------------------------------------
  Type pointer_equality_common_type(Exp& lhs, Exp& rhs, const Type& tl, const Type& tr);
  std::optional<Type> check_nullptr_binary(c99_ast::Binary& exp, const Type& tl, const Type& tr);
  // Type-checks `exp` in place: stamps its type and its operands',
  // wraps operands in the implicit conversions, and returns its type.
  Type check_exp(Exp& exp);
};

// The result of `check_program`: the program stamped in place, with the
// symbol table and the type table.
struct CheckedProgram {
  SymbolTable symbols;
  TypeTable types;
};

// Type-checks the program `prog` in place and returns its tables, or
// throws TypeCheckError (or ConstantExpressionError) for an error in it.
CheckedProgram check_program(c99_ast::Program& prog);

}  // namespace c6502::passes::type_checking
