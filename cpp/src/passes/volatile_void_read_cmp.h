#pragma once

#include "asm_ast.h"

namespace c6502::passes {

// Rewrites each volatile indirect load into A whose value and flags go
// unread, `Mov(<indirect>, Reg(A), true)`, as `Compare(Reg(A),
// <indirect>)`, which reads the byte and leaves A alone.
asm_ast::Program apply_volatile_void_read_cmp(asm_ast::Program prog);

}  // namespace c6502::passes
