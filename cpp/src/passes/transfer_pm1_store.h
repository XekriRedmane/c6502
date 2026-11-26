#pragma once

#include "asm_ast.h"
#include "passes/asm_context.h"

namespace c6502::passes {

// Rewrites each function's steps of a byte by 1 or 2 that go through A
// as steps of X or Y: `TXA; SEC; SBC #1; STA M` as `DEX; STX M`, a step
// of a byte that an index register already holds as `INY` or `DEY`, a
// step on either side of a transfer as a step of the index register,
// and a step stored and loaded into an index register as a step of the
// register.
asm_ast::Program apply_transfer_pm1_store(asm_ast::Program prog,
                                          const AsmContext& ctx);

}  // namespace c6502::passes
