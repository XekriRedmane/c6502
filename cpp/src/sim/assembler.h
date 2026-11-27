// The simulator's in-process assembler (sim/assembler.py), which lays
// an asm program down in a 64K memory image, and the byte size of each
// asm instruction, which the compiler's passes read.
#pragma once

#include <cstdint>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

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

// Raised for an undefined symbol, a branch out of range, a duplicate
// symbol, an operand that doesn't fit its field, an instruction with no
// encoding, or a program whose bytes lie outside $0000-$FFFF, with
// sim/assembler.py's message.
class AssemblerError : public std::runtime_error {
 public:
  using std::runtime_error::runtime_error;
};

struct AssembledProgram {
  // 65,536 bytes, zero where the program put nothing.
  std::vector<std::uint8_t> image;
  // Every label, function, and static, and the zero-page and extra
  // symbols `assemble` started from.
  std::map<std::string, std::int64_t> symbols;
  std::int64_t origin = 0;
  // One past the program's last byte.
  std::int64_t code_end = 0;
};

// Lays the program's functions and statics down in source order from
// `origin`, with `extra_symbols` over the classic layout's zero-page
// symbols (SSP $00, FP $02, HARGS $04, DPTR $24). The soft-stack,
// frame, and DPTR operands, the prologues, and the epilogues use the
// SSP, FP, and DPTR that result. Throws `AssemblerError` for a program
// it can't assemble, and `InternalError` for an instruction no pass
// writes.
AssembledProgram assemble(const asm_ast::Program& program, std::int64_t origin = 0x0800,
                          const std::map<std::string, std::int64_t>& extra_symbols = {});

int check_sim_assembler(const std::vector<std::string>& args);

}  // namespace c6502::sim
