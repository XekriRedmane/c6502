// The queries of c99 types that the front end's passes share. A query
// given a type it doesn't apply to throws `InternalError`.
#pragma once

#include <string>
#include <string_view>
#include <utility>

#include "asdl/runtime.h"
#include "c99_ast.h"

namespace c6502::passes::type_checking {

using Type = c99_ast::Type_data_type;

// Returns `t` without any top-level `Const` and `Volatile`, in either
// order, as a reference into `t`. Qualifiers inside a `Pointer`, an
// `Array`, or a `FunType` stay. C99 §6.3.2.1p2 strips both qualifiers
// in a conversion of an lvalue, so almost every predicate reads the
// type through this.
const Type& strip_quals(const Type& t);

// The predicates of the type checker, each on `t` without its
// top-level qualifiers.
bool is_integer_type(const Type& t);
bool is_floating_type(const Type& t);
bool is_pointer_type(const Type& t);
bool is_struct_or_union(const Type& t);
bool is_array_type(const Type& t);
bool is_nullptr_type(const Type& t);
bool is_void(const Type& t);
bool is_void_pointer(const Type& t);

// Returns `name` as the program spelled it. identifier_resolution
// renames a block-scope object `x` to `@N.x`.
std::string display_name(std::string_view name);

// Returns `t` spelled the way C writes it, for error messages: `const
// char *`, `int[3]`, `struct s`, `unsigned _BitInt(5)`, or "a function
// type".
std::string c_type(const Type& t);

// Returns the size in bytes of the integer type `t`. Throws
// InternalError for any other type.
int int_width(const Type& t);

// Returns the width of the integer type `t` in bits (C23 §6.2.6.2): N
// for `_BitInt(N)`, and 8 bits a byte for the others.
int int_bits(const Type& t);

// Returns whether the integer type `t` is signed. Plain `char` is
// unsigned in c6502 (0..255), as C99 §6.2.5p15 lets an implementation
// choose.
bool is_signed(const Type& t);

// Returns the key that orders the promoted integer types by their
// integer conversion rank (C23 §6.3.1.1p1): the width, and then 0 for a
// bit-precise type or the standard type's place in the order `short <
// int < long < long long`. A bit-precise type's rank is above that of
// any type of less width and below that of a standard type of the same
// width. An unsigned type ranks with its signed counterpart. Throws
// InternalError for a type left out of the promotions' results.
std::pair<int, int> rank(const Type& t);

// Returns the type of `t` after the integer promotions (C99
// §6.3.1.1p2): `int` for `char`, `signed char`, `unsigned char`,
// `bool`, and `short`, which `int` holds in c6502, `unsigned int` for
// `unsigned short`, which it doesn't, and every other type as it is,
// without its top-level qualifiers, since the promoted value is an
// rvalue.
Type promote_integer(const Type& t);

// Returns `value` reduced modulo 2^N for the N bits of the integer type
// `t` and read with `t`'s signedness, the rule of the casts that
// tac_to_asm lowers. A `bool` target gives 0 or 1 (C99 §6.3.1.2).
asdl::Int coerce_int_to_type(asdl::Int value, const Type& t);

// Returns the type of the usual arithmetic conversions (C99 §6.3.1.8p1)
// of the arithmetic types `a` and `b`: `double` when either is,
// `float` when either is, and otherwise the integer rules, with a
// bit-precise type among them handled by `common_bit_precise_type`.
// Both are read without their top-level qualifiers.
Type common_type(const Type& a, const Type& b);

// Returns the usual arithmetic conversions of C23 §6.3.1.8p1 for two
// promoted integer types, at least one of them bit-precise. The
// promotions leave a bit-precise type as it is (§6.3.1.1p2), so
// `_BitInt(5) + _BitInt(5)` is a `_BitInt(5)` and `_BitInt(24) + int` a
// `_BitInt(24)`, while `_BitInt(16) + int` is an `int`, whose rank is
// greater at the same width.
Type common_bit_precise_type(const Type& a, const Type& b);

}  // namespace c6502::passes::type_checking
