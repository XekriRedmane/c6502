// The part of sim/assembler.py that the compiler's passes read: the
// byte size of each asm instruction. The simulator's assembler itself
// stays in Python.
#pragma once

#include "asm_ast.h"
#include "passes/asm_context.h"

namespace c6502::sim {

// Returns the number of bytes `instr` assembles to, by the encoding
// rules of the simulator's assembler, which `asm_emit` and
// `asm_to_asm2` agree with. `zp_names` names the `Data` symbols that
// resolve to zero-page addresses, whose operands take one byte, and is
// the runtime's four symbols (HARGS, SSP, FP, DPTR) when null. Throws
// `InternalError` for an instruction the assembler can't encode, such
// as one with a `Pseudo` operand or a `Phi`.
int instruction_size(const asm_ast::Type_instruction& instr,
                     const passes::NameSet* zp_names = nullptr);

}  // namespace c6502::sim
