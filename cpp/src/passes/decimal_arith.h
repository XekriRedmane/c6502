// The port of passes/decimal_arith.py: the operands that asm
// `DecimalArith`, the packed BCD add or subtract, reads and writes.
#pragma once

#include <vector>

#include "asm_ast.h"

namespace c6502::passes::decimal_arith {

// Returns the operands `instr` reads, `left` then `right`.
std::vector<asm_ast::Type_operand> reads(const asm_ast::DecimalArith& instr);

// Returns the operands `instr` writes.
std::vector<asm_ast::Type_operand> writes(const asm_ast::DecimalArith& instr);

// Returns every operand of `instr`, `left`, `right`, and then `dst`.
std::vector<asm_ast::Type_operand> operands(const asm_ast::DecimalArith& instr);

// Returns whether the expansion loads Y to address a soft-stack slot,
// which it does for a `Frame` or `Stack` operand.
bool writes_y(const asm_ast::DecimalArith& instr);

// Returns `instr` with every operand replaced by `fn(operand)`.
template <class Fn>
asm_ast::DecimalArith map_operands(const asm_ast::DecimalArith& instr, Fn&& fn) {
  asm_ast::DecimalArith out{.subtract = instr.subtract};
  for (const auto& op : instr.left) {
    out.left.push_back(fn(op));
  }
  for (const auto& op : instr.right) {
    out.right.push_back(fn(op));
  }
  for (const auto& op : instr.dst) {
    out.dst.push_back(fn(op));
  }
  return out;
}

}  // namespace c6502::passes::decimal_arith
