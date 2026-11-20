// Helpers for asm `DecimalArith`, the packed BCD add or subtract.
//
// `DecimalArith(subtract, left, right, dst)` is one asm instruction for
// the whole `SED; CLC; LDA left[0]; ADC right[0]; STA dst[0]; ...; CLD`
// chain (asm.asdl). Keeping the chain in one instruction until
// `asm_to_asm2` expands it is what confines the decimal flag. No pass
// sees the SED or the CLD, so no pass can put an instruction between
// them, and no pass can move the chain's ADC or SBC out from between
// them. The asm2 check in `passes/ir_check.py` checks the expansion.
//
// Each pass still has to know which operands the instruction reads and
// writes, and this module gives them in one place.
//
//   * `reads` are the bytes of `left` and `right`, and `writes` are the
//     bytes of `dst`. A and the flags N, Z, C, and V are clobbered, and
//     nothing in A or a flag is read.
//   * `writes_y` is true when an operand's access begins with an
//     `LDY #off` (a soft-stack slot), which clobbers Y. X is never
//     touched.
//   * `map_operands` rebuilds the instruction with each operand passed
//     through a function, for the passes that rename or resolve
//     operands.
//
// The rest of the Python module (the Pseudo reads and writes, the
// early-write pairs, and the form check) serves the asm SSA round trip
// and the IR check, which are ported with those stages.
#include "passes/decimal_arith.h"

namespace c6502::passes::decimal_arith {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::isa;

std::vector<ir::Type_operand> reads(const ir::DecimalArith& instr) {
  std::vector<ir::Type_operand> out(instr.left);
  out.insert(out.end(), instr.right.begin(), instr.right.end());
  return out;
}

std::vector<ir::Type_operand> writes(const ir::DecimalArith& instr) {
  return instr.dst;
}

std::vector<ir::Type_operand> operands(const ir::DecimalArith& instr) {
  std::vector<ir::Type_operand> out = reads(instr);
  out.insert(out.end(), instr.dst.begin(), instr.dst.end());
  return out;
}

bool writes_y(const ir::DecimalArith& instr) {
  for (const auto* list : {&instr.left, &instr.right, &instr.dst}) {
    for (const auto& op : *list) {
      if (isa<ir::Frame, ir::Stack>(op)) {
        return true;
      }
    }
  }
  return false;
}

}  // namespace c6502::passes::decimal_arith
