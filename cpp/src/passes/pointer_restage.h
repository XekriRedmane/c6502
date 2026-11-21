// The port of passes/pointer_restage.py, which decides when
// `tac_to_asm` copies a pointer into DPTR again before each byte of a
// wide access through it. The .cpp's comment gives the reasons.
#pragma once

#include <optional>

#include "asdl/runtime.h"
#include "asm_ast.h"
#include "c99_ast.h"
#include "passes/asm_context.h"
#include "passes/type_checking.h"

namespace c6502::passes::pointer_restage {

// Returns the scalar type that an object of type `t` has at exactly
// bytes `k` to `k + n - 1`: a member of a structure, an element of an
// array, or `t` itself, or null when no one scalar covers exactly those
// bytes, or when they are in a union, whose members overlap. The
// pointer is into `t` or `types`.
const c99_ast::Type_data_type* scalar_at(const c99_ast::Type_data_type& t, asdl::Int k,
                                         asdl::Int n, const TypeTable* types);

// Returns whether a store of the scalar type `stored` is undefined when
// it overlaps an object of the pointer type `ptr_type` (C23 §6.5p7).
bool store_keeps_pointer(const c99_ast::Type_data_type& ptr_type,
                         const c99_ast::Type_data_type& stored);

// Returns whether the pointer operand `op` is, or will be under `-O`, a
// pair of zero-page bytes that `(zp),Y` can read through: a `Pseudo`,
// a parameter's or local's zero-page slot, or a user zero-page global
// of the compile (`ctx`).
bool pointer_in_zero_page(const asm_ast::Type_operand& op, const AsmContext& ctx);

// Returns whether a store of `n` bytes at `offset` through a pointer of
// type `ptr_type` may read the pointer again for each byte. `ptr_type`
// may be null, as Python's None.
bool restage_store(const c99_ast::Type_data_type* ptr_type, asdl::Int offset, asdl::Int n,
                   const TypeTable* types);

}  // namespace c6502::passes::pointer_restage
