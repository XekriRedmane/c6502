// c99_to_tac.py's `translate_program`, which lowers the type-checked
// c99 program to three-address code, and the helpers of the module that
// other passes import. src/c99_to_tac.cpp opens with the module's
// description.
#pragma once

#include <string_view>

#include "asdl/runtime.h"
#include "c99_ast.h"
#include "passes/type_checking.h"
#include "tac_ast.h"

namespace c6502::c99_to_tac {

// The part of the label names of a loop marked `[[c6502::keep]]`,
// which dead-loop elimination reads.
inline constexpr std::string_view kKeepLoopMark = "_keep";

// Translates the type-checked program `prog` to TAC. `symbols` and
// `types` are the tables `check_program` built for it, and `types` may
// be null, as Python's may be None. The translation changes all three
// in place, as Python's does: `short`, `unsigned short`, `nullptr_t`,
// and `nullptr` are replaced in the program and the tables before the
// lowering, each temporary is added to `symbols`, and after it `bool`
// and the bit-precise types are replaced in the tables and in the types
// of the top-level declarations. Throws `CompileError`
// ("RecursionSaveError") for a misplaced
// `[[c6502::save_hardware_stack]]`.
tac_ast::Program translate_program(c99_ast::Program& prog, passes::SymbolTable& symbols,
                                   passes::TypeTable* types);

// Returns a TAC constant of `t`'s width and signedness holding `value`
// (Python's `_tac_const_for`): `ConstChar` for `signed char`,
// `ConstUChar` for `char`, `unsigned char`, and `bool`, `ConstUInt` for
// a pointer, the class of the storage for a bit-precise type, with the
// value reduced modulo 2^N, and the IEEE 754 bits of `value` for
// `float` and `double`. `Const` and `Volatile` are transparent. Throws
// `InternalError` for a type that isn't scalar, as Python raises
// `TypeError`.
tac_ast::Type_const tac_const_for(const c99_ast::Type_data_type& t, asdl::Int value);

// Returns the bytes a value of type `t` occupies (Python's `_sizeof`):
// the element's size times the count for an array, and the layout's
// size for a struct or union, which needs `types`. Throws
// `InternalError` for a struct or union without a type table or with
// an incomplete layout, and for a type that has no size, as Python
// raises `TypeError`.
asdl::Int sizeof_c99(const c99_ast::Type_data_type& t, const passes::TypeTable* types);

// Returns the byte width of the scalar type `t` (Python's
// `_byte_width_of`), with `Const` and `Volatile` transparent. Throws
// `InternalError` for a type that isn't scalar.
int byte_width_of(const c99_ast::Type_data_type& t);

}  // namespace c6502::c99_to_tac
