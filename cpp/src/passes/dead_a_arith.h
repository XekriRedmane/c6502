// The helpers of passes/dead_a_arith.py that other passes import. The
// pass itself isn't ported yet.
#pragma once

#include <cstddef>

#include "asm_ast.h"
#include "passes/asm_liveness.h"

namespace c6502::passes::dead_a_arith {

// Returns whether `instrs[i]` is `Mov(Reg(X|Y), Reg(A))` and the N/Z
// flags at `i` already reflect the source register's value, so the
// move's own effect on the flags changes nothing. The scan goes back
// within the block from `i - 1` to the first instruction that sets the
// flags, and the answer is whether that instruction wrote the register
// the move reads. A later write of the register would have set the
// flags and been found first. The scan gives up at a label, a
// terminator, a call, a prologue, an `AllocateStack`, or a
// `LoadAddress`.
bool flags_reflect_src_reg(Instrs instrs, std::size_t i);

// Returns whether `instr` writes the register `reg` and sets N/Z from
// its new value, as every register-writing opcode the IR models does
// (LDA/LDX/LDY, TAX/TAY/TXA/TYA, INX/INY/DEX/DEY, ADC/SBC/AND/ORA/EOR
// on A, ASL/LSR/ROL/ROR on A, PLA). A store, and a move from memory to
// memory, whose flags come from the loaded byte, don't.
bool writes_reg_setting_n_z(const asm_ast::Type_instruction& instr,
                            const asm_ast::Type_reg& reg);

}  // namespace c6502::passes::dead_a_arith
