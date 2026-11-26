// The fold of the const scalar statics of internal linkage into
// immediates.
#pragma once

#include "asm_ast.h"
#include "passes/type_checking.h"

namespace c6502::passes {

// Returns `prog` with each read of a const-qualified scalar static of
// internal linkage replaced by an `Imm` (or a label's half) holding the
// static's byte, and with the statics that nothing names any more
// dropped. `symbols` gives the qualifier of each static, and without it
// (null) the program is returned unchanged.
//
// Internal linkage (`is_global` false) makes this sound, since C's
// `static` keeps the name out of the link, so once every reference here
// is folded, no other unit can read the storage, and it can go. A const
// static of external linkage is left alone, since another unit may read
// it by name.
asm_ast::Program fold_const_statics(asm_ast::Program prog, const SymbolTable* symbols);

}  // namespace c6502::passes
