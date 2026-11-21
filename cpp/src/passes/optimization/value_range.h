// The port of passes/optimization/value_range.py: the largest value a
// TAC operand can hold, when it can't be negative, and the queries of
// integer types it is built on. Strength reduction, the narrowing of
// byte ranges, the unsigned compare of nonnegative values, and the
// jump threads read it.
#pragma once

#include <functional>
#include <optional>

#include "asdl/runtime.h"
#include "c99_ast.h"
#include "passes/asm_context.h"
#include "passes/type_checking.h"
#include "tac_ast.h"

namespace c6502::passes::value_range {

// Returns the instruction that defines a Var, or null when there is
// none. `max_value` consults it only for the names in `ssa_dsts`.
using DefOf = std::function<const tac_ast::Type_instruction*(const tac_ast::Var&)>;

// Returns the byte width of the integer type `t`, or none for any other
// type. A pointer, a floating type, an aggregate, and a bit-precise
// type are none.
std::optional<int> int_width(const c99_ast::Type_data_type& t);

// Returns the largest value of the integer type `t`, or none for a type
// that isn't one.
std::optional<asdl::Int> type_max(const c99_ast::Type_data_type& t);

// Returns whether `t`, without its qualifiers, is `char` or one of the
// unsigned integer types.
bool is_unsigned_type(const c99_ast::Type_data_type& t);

// Returns the largest value `val` can hold when it is never negative,
// or none. A Var is bounded by its type when the type is unsigned, and
// for a name in `ssa_dsts` by the instruction `def_of` gives it, by the
// rules of the Python module's docstring. `symbols` and `ssa_dsts` may
// be null, as Python's may be None.
std::optional<asdl::Int> max_value(const tac_ast::Type_val& val, const DefOf& def_of,
                                   const SymbolTable* symbols, const NameSet* ssa_dsts);

}  // namespace c6502::passes::value_range
