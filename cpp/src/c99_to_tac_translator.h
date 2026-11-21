// The `Translator` of c99_to_tac.py and the helpers its lowerings
// share. The lowering is split over four files, in the parts of
// docs/cpp_port.md's step 5:
//
// - src/c99_to_tac.cpp: the module's free functions (the erasure of
//   `short`, `nullptr_t`, `bool`, and the bit-precise types, the TAC
//   constants, the static initializers, and the index scales), the
//   queries every part reads, and the Translator's driver, builtins,
//   <stdbit.h> loops, functions, statics, blocks, and declarations
//   (part A1);
// - src/c99_to_tac_exp.cpp: statements, calls, expressions, and pointer
//   arithmetic (part A2);
// - src/c99_to_tac_lvalues.cpp: initializers, lvalues, and subscripts
//   (part A2);
// - src/c99_to_tac_conversions.cpp: increments, compound assignments,
//   casts, the bit-precise padding, conversions to `bool`, the
//   short-circuit operators, and the maps of the operators (part A2).
//
// The struct declares every lowering, since the parts call each other
// throughout.
#pragma once

#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "asdl/runtime.h"
#include "c99_ast.h"
#include "c99_to_tac.h"
#include "passes/type_checking.h"
#include "tac_ast.h"

namespace c6502::c99_to_tac {

namespace c99 = ::c6502::c99_ast;
namespace tac = ::c6502::tac_ast;
using passes::SymbolTable;
using passes::TypeTable;
using Type = c99::Type_data_type;
using Exp = c99::Type_exp;
using Instrs = std::vector<tac::Type_instruction>;
using Val = tac::Type_val;

// --- Queries on c99 types and expressions ---

// Returns the type checker's `data_type` of `exp`, which `String` and
// `Designation` don't have, as none.
const std::optional<Type>& data_type_of(const Exp& exp);

// Returns `t` without the `const` and `volatile` around it (Python's
// `_unqualified`), as a reference into `t`.
const Type& unqualified(const Type& t);

// Returns `t` with every `Const` and `Volatile` anywhere in it removed
// (Python's `_strip_quals_c99`).
Type strip_quals(const Type& t);

// Returns whether `t` has a top-level `Volatile`, possibly under a
// `Const`. A missing type is not volatile.
bool is_volatile_quals(const Type& t);
bool is_volatile_quals(const std::optional<Type>& t);

// Returns whether `t`, without its qualifiers, is an integer type,
// `bool` and the bit-precise types among them.
bool is_integer_type(const Type& t);

// Returns whether `t` is an unsigned integer type or a pointer. Plain
// `char` is unsigned in c6502.
bool is_unsigned_integer(const Type& t);

// Returns the least and greatest values of the integer or pointer type
// `t`, or none for any other type.
std::optional<std::pair<asdl::Int, asdl::Int>> int_range(const Type& t);

// Returns `value`'s low `width` bytes read as a signed or unsigned
// integer, the value a cast of the same width produces.
asdl::Int reinterpret_int(asdl::Int value, int width, bool is_signed);

// Returns whether `t` is one of the three character types, with no
// qualifier.
bool is_char_element(const Type& t);

// Returns the value of the integer constant under any number of casts,
// or none.
std::optional<asdl::Int> const_int_or_none(const Exp* exp);

// Returns whether `exp` is a constant whose bytes are all zero, under
// any casts.
bool is_zero_constant(const Exp& exp);

// Returns whether the type-checked `exp` can only be 0 or 1.
bool is_zero_or_one(const Exp& exp);

// Returns whether `lval OP= rval` evaluates `rval` before the address
// and value of `lval`.
bool calls_first(const Exp& lval, const Exp& rval);

// Returns the value of `exp` when it's an integer constant or a
// `sizeof`, under integer casts that convert the value as C does.
std::optional<asdl::Int> exact_const_index(const Exp& exp, const TypeTable* types);

// Returns `(e, k)` when the subscript `index` into a static array of
// `total` bytes is `e + k` or `k + e` with `e` a widened unsigned byte
// and `k` a constant within the array.
std::optional<std::pair<const Exp*, asdl::Int>> split_index_addend(const Exp& index,
                                                                   asdl::Int total,
                                                                   asdl::Int elem_size,
                                                                   const TypeTable* types);

// Returns the `Var` of `obj` when `ptr` is a cast of `obj`'s address.
const c99::Var* cast_address_object(const Exp& ptr);

// The object, the element size, and the object's size of a cast of an
// object's address that reads the object as an array.
struct ObjectView {
  const c99::Var* obj = nullptr;
  asdl::Int elem = 0;
  asdl::Int total = 0;
};
std::optional<ObjectView> object_view(const Exp& ptr, const TypeTable* types);

// Returns `(p, k)` when `*ptr` is `p[k]` for a cast of an object's
// address `p`.
std::optional<std::pair<Exp, Exp>> deref_as_subscript(const Exp& ptr);

// Returns the element size of `array` when its offset is scaled in a
// byte.
std::optional<asdl::Int> byte_offset_scale(const Exp& array, const TypeTable* types);

// --- TAC values ---

// Returns `Constant(tac_const_for(t, value))`.
Val tac_const_val(const Type& t, asdl::Int value);

// Returns a constant of the storage of the bit-precise `t` holding the
// bit pattern `bits`, which may set padding bits.
Val tac_storage_const(const c99::BitInt& t, asdl::Int bits);

// Returns the TAC constant of `c`.
tac::Type_const to_tac_const(const c99::Type_const& c);

// Returns the compile-time value of a cast of the constant `c` with a
// floating source or target, as a constant of `target`'s type.
tac::Type_const fold_fp_cast_constant(const Type& target, const tac::Type_const& c);

// Returns the value `v` holds, and throws `InternalError` for none,
// where Python would put None in an instruction's field.
const Val& need(const std::optional<Val>& v);

// Returns `type`, and throws `InternalError` for none, where Python
// would read a missing `data_type`.
const Type& need_type(const std::optional<Type>& type);

std::string start_label(std::string_view loop_label);
std::string continue_label(std::string_view loop_label);
std::string break_label(std::string_view loop_label);

// One entry of what an automatic aggregate's initializer stores
// (`_init_leaves`): `n` zero bytes at `offset`, or a store at `offset`
// of type `type` of a c99 expression (`exp`) or a TAC constant
// (`constant`).
struct InitLeaf {
  bool zero = false;
  asdl::Int offset = 0;
  asdl::Int n = 0;
  const Exp* exp = nullptr;
  std::optional<Val> constant;
  std::optional<Type> type;
};

// The Translator of c99_to_tac.py. It owns the two counters that
// number the temporaries (`%0`, `%1`, ...) and the labels
// (`.and_false@0`, ...) over the program, so the order of the lowering
// is part of its output.
struct Translator {
  SymbolTable& symbols;
  // The struct and union layouts, which the erasures of the types change
  // in place, or null.
  TypeTable* types = nullptr;
  int temp_counter = 0;
  int label_counter = 0;
  // The resolved name of the hidden sret parameter while a function
  // that returns a struct is translated.
  std::optional<std::string> sret_param;
  // The labels of the loops marked `[[c6502::keep]]`.
  std::set<std::string, std::less<>> keep_loops;

  Translator(SymbolTable& symbols, TypeTable* types) : symbols(symbols), types(types) {}

  // --- Shared queries (src/c99_to_tac.cpp) ---
  std::string loop_targets(std::string_view label) const;
  std::string make_temporary_variable_name(const Type* t);
  std::string make_temp(const std::optional<Type>& t) {
    return make_temporary_variable_name(t ? &*t : nullptr);
  }
  std::string make_temp(const Type& t) { return make_temporary_variable_name(&t); }
  tac::Var temp_var(const Type& t) { return tac::Var{.name = make_temp(t)}; }
  tac::Var temp_var(const std::optional<Type>& t) { return tac::Var{.name = make_temp(t)}; }
  std::string make_label(std::string_view prefix);
  bool returns_carry_struct(std::string_view fn_name) const;
  bool is_volatile_var(std::string_view name) const;
  bool volatile_scalar(const Exp& exp) const;
  tac::Var read_volatile_var(const std::string& name, Instrs& instrs);
  void write_volatile_var(const std::string& name, const Val& src, Instrs& instrs);
  void init_scalar(const c99::Type_var_decl& vd, Instrs& instrs);

  // --- The driver (src/c99_to_tac.cpp) ---
  tac::Program translate_program(c99::Program& prog);
  std::optional<tac::Type_top_level> translate_top_level_declaration(
      const c99::Type_declaration& decl);
  tac::Function translate_function(const c99::Function& fn);
  tac::Function translate_function_decl(const c99::Type_function_decl& fd);
  std::vector<tac::Type_top_level> emit_static_variables();
  void translate_block(const c99::Type_block& block, Instrs& instrs);
  void translate_block_item(const c99::Type_block_item& item, Instrs& instrs);
  void translate_declaration(const c99::Type_declaration& decl, Instrs& instrs);
  void translate_var_init(const c99::Type_var_decl& vd, Instrs& instrs);
  Val translate_builtin(const c99::FunctionCall& exp, Instrs& instrs);
  Val translate_stdbit(const c99::FunctionCall& exp, std::string_view operation, Instrs& instrs);
  Val stdbit_count_result(const tac::Var& n, const Type& result_t, Instrs& instrs);

  // --- Statements, calls, expressions (src/c99_to_tac_exp.cpp) ---
  void translate_statement(const c99::Type_statement& stmt, Instrs& instrs);
  void translate_for_init(const c99::Type_for_init& init, Instrs& instrs);
  c99::Var materialize_compound_literal(const c99::CompoundLiteral& cl, Instrs& instrs);
  std::optional<Val> translate_call(bool hw_stack_save, const std::optional<Type>& data_type,
                                    const std::string* name, std::optional<Val> ptr,
                                    const std::vector<Exp>& args, Instrs& instrs);
  std::optional<Val> translate_exp(const Exp& exp, Instrs& instrs);
  Val translate_value(const Exp& exp, Instrs& instrs) { return need(translate_exp(exp, instrs)); }
  Val translate_pointer_arithmetic(const c99::Type_binary_operator& op, const Val& src1,
                                   const Val& src2, const Type& lt, const Type& rt,
                                   const Type& result_type, Instrs& instrs);

  // --- Initializers, lvalues, subscripts (src/c99_to_tac_lvalues.cpp) ---
  void emit_initializer(const c99::Type_var_decl& vd, Instrs& instrs);
  void init_leaves(const Type& t, const Exp* init, asdl::Int offset, std::vector<InitLeaf>& out);
  void emit_zero_bytes(const std::string& name, std::optional<Val>& base, asdl::Int offset,
                       asdl::Int n, Instrs& instrs);
  Val base_address(const std::string& name, std::optional<Val>& base, Instrs& instrs);
  asdl::Int member_offset(const Type& struct_type, std::string_view member) const;
  const Type& member_type(const Type& struct_type, std::string_view member) const;
  Val add_offset(const Val& base, asdl::Int offset, const Type& elem_type, Instrs& instrs);
  Val translate_lvalue_address(const Exp& lval, Instrs& instrs);
  Val translate_dot_address(const Exp& operand, std::string_view member, Instrs& instrs);
  Val translate_arrow_address(const Exp& operand, std::string_view member, Instrs& instrs);
  std::optional<Val> try_indexed_load_subscript(const Exp& array, const Exp& index,
                                                const std::optional<Type>& data_type,
                                                Instrs& instrs);
  struct StaticView {
    std::string name;
    asdl::Int elem = 0;
    asdl::Int total = 0;
    bool obj_volatile = false;
  };
  std::optional<StaticView> static_object_view(const Exp& array) const;
  std::optional<std::pair<Val, asdl::Int>> byte_index(const Exp& index, asdl::Int total,
                                                      asdl::Int elem_size, Instrs& instrs);
  bool try_indexed_store_subscript(const Exp& array, const Exp& index, const Val& rval_val,
                                   bool is_volatile, Instrs& instrs);
  Val translate_subscript_address(const Exp& array, const Exp& index, Instrs& instrs);
  Val translate_deref_address(const Exp& ptr, Instrs& instrs);

  // --- Conversions and the rest (src/c99_to_tac_conversions.cpp) ---
  Val translate_incdec(const c99::Type_incdec_op& op, const Exp& operand, Instrs& instrs,
                       bool return_old);
  Val emit_incdec_step(const c99::Type_incdec_op& op, const Val& src, const Type& op_type,
                       Instrs& instrs);
  Val translate_compound_assign(const c99::Type_binary_operator& op, const Exp& lval,
                                const Exp& rval, const std::optional<Type>& intermediate_type,
                                Instrs& instrs);
  Val compute_compound_step(const c99::Type_binary_operator& op, const Val& lval_val,
                            const Type& lv_type, const Exp& rval_exp,
                            const Type& intermediate_type, bool is_pointer_arith,
                            Instrs& instrs, std::optional<Val> rval_val = std::nullopt);
  Val emit_cast(const Val& inner_val, const Type& source, const Type& target, Instrs& instrs);
  Val restore_padding(const Val& val, const std::optional<Type>& t,
                      const tac::Type_binary_operator* binop, const tac::Type_unary_operator* unop,
                      Instrs& instrs);
  Val wrap_bit_precise(const Val& val, const c99::BitInt& t, Instrs& instrs);
  Val emit_bit_precise_cast(const Val& inner_val, const Type& source, const Type& target,
                            Instrs& instrs);
  Val emit_to_bool(const Val& val, const Type& source, Instrs& instrs);
  Val emit_zero_or_one_to_bool(const Val& val, const Type& source, Instrs& instrs);
  Val emit_widen_or_narrow(const Val& src_val, const Type& src_type, const Type& dst_type,
                           Instrs& instrs);
  Val translate_short_circuit(const Exp& left, const Exp& right, Instrs& instrs,
                              bool short_circuit_on_true);
};

tac::Type_unary_operator translate_unop(const c99::Type_unary_operator& op);
tac::Type_binary_operator translate_incdec_op(const c99::Type_incdec_op& op);
tac::Type_binary_operator translate_binop(const c99::Type_binary_operator& op);

}  // namespace c6502::c99_to_tac
