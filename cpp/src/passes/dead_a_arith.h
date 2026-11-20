// The port of passes/dead_a_arith.py: the pass, and the helpers that
// other passes import from it.
#pragma once

#include <cstddef>

#include "asm_ast.h"
#include "passes/asm_liveness.h"

namespace c6502::passes {

// Drops each instruction of the program's functions whose only effects
// are writes to A and the flags when A and those flags are dead after
// it, each load into X or Y, and each load into A through a zero-page
// pointer, whose value and flags nothing reads. The comment at the top of
// dead_a_arith.cpp gives the shapes and the reasons.
asm_ast::Program apply_dead_a_arith_elimination(asm_ast::Program prog);

}  // namespace c6502::passes

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
