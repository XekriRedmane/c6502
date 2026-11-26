#pragma once

#include "asm_ast.h"

namespace c6502::passes {

// Folds `LDA M; AND #$80; BEQ/BNE` into `LDA M; BPL/BMI`, `AND #$01;
// BEQ/BNE` into `LSR A; BCC/BCS`, and `LDA M; AND #$40; BEQ/BNE` into
// `BIT M; BVC/BVS` in every function, where the registers and flags the
// rewrite changes are dead.
asm_ast::Program apply_and_sign_bit_branch(asm_ast::Program prog);

}  // namespace c6502::passes
