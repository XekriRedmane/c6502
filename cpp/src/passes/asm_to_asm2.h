// The port of passes/asm_to_asm2.py, which lowers the asm IR to the
// asm2 IR, one node per 6502 instruction.
#pragma once

#include <vector>

#include "asm2_ast.h"
#include "asm_ast.h"

namespace c6502::passes {

// Returns the asm2 program of `prog`: each `AllocateStack`,
// `FunctionPrologue`, and `Ret` expanded into its atoms, each packed
// BCD `DecimalArith` into its decimal-mode chain, and every other node
// copied at the asm2 type. Throws `CompileError("FrameSizeError", ...)`
// for a frame that `LDY #offset` can't reach. With the IR checks on,
// `ir_check::check_asm2` then checks the result, as Python checks it
// under `C6502_CHECK_IR=1`, and throws `InternalError` for a decimal
// flag set outside a packed BCD chain.
asm2_ast::Program lower_to_asm2(const asm_ast::Program& prog);

// Returns the decimal-mode chain of a packed BCD add or subtract:
// `SED`, `CLC` (`SEC`), an `LDA left[k]`, `ADC right[k]` (`SBC`),
// `STA dst[k]` per byte, low byte first, and `CLD`.
std::vector<asm2_ast::Type_instruction> expand_decimal_arith(
    const asm_ast::DecimalArith& instr);

}  // namespace c6502::passes
