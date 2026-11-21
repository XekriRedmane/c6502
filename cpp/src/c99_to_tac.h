// The parts of c99_to_tac.py that the TAC optimizer's passes read
// before c99_to_tac itself is ported: `_tac_const_for`, which
// static_const_fold and constant folding's loads from constant arrays
// call. Group A of step 5 ports the rest of the module (docs/cpp_port.md,
// "Step 5"), and this file is to merge with its port.
#pragma once

#include "asdl/runtime.h"
#include "c99_ast.h"
#include "tac_ast.h"

namespace c6502::c99_to_tac {

// Returns the TAC constant of `t`'s width and signedness (and for a
// floating type its precision) holding `value` (Python's
// `_tac_const_for`). A pointer is a `ConstUInt`, `signed char` a
// `ConstChar`, and plain `char`, `unsigned char`, and `bool` a
// `ConstUChar`, since plain `char` is unsigned in c6502 (C99 §6.2.5.15).
// A bit-precise type holds `value` modulo 2^N in the integer of its
// storage width. A `float` or `double` holds `value` converted with
// `fp_arith`. The qualifiers of `t` are ignored. Throws `InternalError`
// for any other type, where Python raises `TypeError`. Python also takes
// a float `value`, which it truncates with `int()` first, as a caller
// does here.
tac_ast::Type_const tac_const_for(const c99_ast::Type_data_type& t, asdl::Int value);

}  // namespace c6502::c99_to_tac
