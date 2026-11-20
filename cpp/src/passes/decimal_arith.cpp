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
// The asm SSA round trip reads `pseudo_reads`, `pseudo_writes`,
// `map_reads`, and `map_writes`, and the interference graphs
// `early_write_pairs`. The form check serves the IR check, which isn't
// ported.
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

std::vector<ir::Pseudo> pseudo_reads(const ir::DecimalArith& instr) {
  std::vector<ir::Pseudo> out;
  for (const auto* list : {&instr.left, &instr.right}) {
    for (const auto& op : *list) {
      if (const auto* p = asdl::as<ir::Pseudo>(op)) {
        out.push_back(*p);
      }
    }
  }
  return out;
}

std::vector<ir::Pseudo> pseudo_writes(const ir::DecimalArith& instr) {
  std::vector<ir::Pseudo> out;
  for (const auto& op : instr.dst) {
    if (const auto* p = asdl::as<ir::Pseudo>(op)) {
      out.push_back(*p);
    }
  }
  return out;
}

std::vector<ir::Type_operand> later_sources(const ir::DecimalArith& instr, std::size_t k) {
  std::vector<ir::Type_operand> out;
  for (std::size_t j = k + 1; j < instr.left.size(); ++j) {
    out.push_back(instr.left[j]);
  }
  for (std::size_t j = k + 1; j < instr.right.size(); ++j) {
    out.push_back(instr.right[j]);
  }
  return out;
}

std::vector<std::pair<std::string, std::string>> early_write_pairs(
    const ir::DecimalArith& instr) {
  std::vector<std::pair<std::string, std::string>> out;
  for (std::size_t k = 0; k < instr.dst.size(); ++k) {
    const auto* d = asdl::as<ir::Pseudo>(instr.dst[k]);
    if (d == nullptr) {
      continue;
    }
    for (const auto& s : later_sources(instr, k)) {
      const auto* sp = asdl::as<ir::Pseudo>(s);
      if (sp != nullptr && sp->name != d->name) {
        out.emplace_back(d->name, sp->name);
      }
    }
  }
  return out;
}

}  // namespace c6502::passes::decimal_arith
