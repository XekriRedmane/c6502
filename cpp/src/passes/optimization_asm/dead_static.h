#pragma once

#include "asm_ast.h"

namespace c6502::passes {

// Returns `prog` without each `StaticVariable` of internal linkage that
// no function's operand and no static's `AddressInit` or
// `AddressByteInit` names.
asm_ast::Program apply_dead_static_elimination(asm_ast::Program prog);

}  // namespace c6502::passes
