// The port of passes/inc_peephole.py: the multi-byte INC peephole, and
// the operand tests that other passes import from it.
#pragma once

#include "asm_ast.h"

namespace c6502::passes {

// Rewrites every in-place add of 1 to a value in memory or in X or Y
// (the per-byte ADC chain `tac_to_asm` writes for it) in the program's
// functions to an `INC; BNE done; INC; ...; done:` chain, where the
// operands allow it. The labels it adds are numbered past every
// `.inc_done@N` already in the program. The comment at the top of
// inc_peephole.cpp gives the rules.
asm_ast::Program apply_inc_peephole(asm_ast::Program prog);

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
