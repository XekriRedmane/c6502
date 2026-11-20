// The port of passes/decimal_arith.py: the operands that asm
// `DecimalArith`, the packed BCD add or subtract, reads and writes.
#pragma once

#include <string>
#include <utility>
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

// Returns the `Pseudo` operands among those `instr` reads, in order.
std::vector<asm_ast::Pseudo> pseudo_reads(const asm_ast::DecimalArith& instr);

// Returns the `Pseudo` operands among those `instr` writes, in order.
std::vector<asm_ast::Pseudo> pseudo_writes(const asm_ast::DecimalArith& instr);

// Returns `instr` with every operand it reads replaced by `fn(operand)`
// and its `dst` left as it is.
template <class Fn>
asm_ast::DecimalArith map_reads(const asm_ast::DecimalArith& instr, Fn&& fn) {
  asm_ast::DecimalArith out{.subtract = instr.subtract};
  for (const auto& op : instr.left) {
    out.left.push_back(fn(op));
  }
  for (const auto& op : instr.right) {
    out.right.push_back(fn(op));
  }
  out.dst = instr.dst;
  return out;
}

// Returns `instr` with every `dst` operand replaced by `fn(operand)`.
template <class Fn>
asm_ast::DecimalArith map_writes(const asm_ast::DecimalArith& instr, Fn&& fn) {
  asm_ast::DecimalArith out{.subtract = instr.subtract, .left = instr.left, .right = instr.right};
  for (const auto& op : instr.dst) {
    out.dst.push_back(fn(op));
  }
  return out;
}

// Returns the source bytes read after `dst[k]` is written, which are
// `left[j]` and `right[j]` for j > k. `dst[k]` must not share a byte
// with any of them.
std::vector<asm_ast::Type_operand> later_sources(const asm_ast::DecimalArith& instr,
                                                 std::size_t k);

// Returns the `(dst, source)` Pseudo name pairs that must not share a
// byte, because `dst[k]` is written before the source byte of index
// j > k is read. Liveness after the instruction doesn't show these, so
// the interference graphs add them.
std::vector<std::pair<std::string, std::string>> early_write_pairs(
    const asm_ast::DecimalArith& instr);

}  // namespace c6502::passes::decimal_arith
