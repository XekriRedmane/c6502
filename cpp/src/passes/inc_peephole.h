// The helpers of passes/inc_peephole.py that other passes import. The
// pass itself isn't ported yet.
#pragma once

#include "asm_ast.h"

namespace c6502::passes {

// Returns whether `op` is an array element indexed by X,
// `IndexedData(name, k, X)`, the one indexed form that `INC`, `DEC`,
// `ASL`, `LSR`, `ROL`, and `ROR` take. There's no abs,Y form of them.
bool is_x_indexed(const asm_ast::Type_operand& op);

namespace inc_peephole {

// Returns whether `op` is an operand that INC / INX / INY can address
// directly: `Data(name, k)` (absolute), `ZP(addr, k)` (zero page), an
// element indexed by X, or `Reg(X)` / `Reg(Y)`. The register case lets
// a one-byte ADC chain on an index register that the register
// allocator colored become `INX` / `INY`. `Frame`, `Stack`, and the
// indirect forms are indirect-Y, which INC doesn't have.
bool is_inc_eligible_operand(const asm_ast::Type_operand& op);

// Returns whether `a` and `b` denote the same byte or register. Two
// elements indexed by one register count as the same byte, which holds
// only while the index register is unchanged, as it is within the
// adjacent instructions the peepholes match.
bool operands_equal(const asm_ast::Type_operand& a, const asm_ast::Type_operand& b);

}  // namespace inc_peephole

}  // namespace c6502::passes
