// Folds `Mov(Imm(0), A); Or(X, A)` into `Mov(X, A)` on the asm SSA form.
//
// The idiom that builds a word from two bytes, `(uint16_t)hi << 8 |
// (uint16_t)lo`, lowers to byte-level `Mov` and `Or` instructions. The
// lowering of `<< 8` writes `Imm(0)` into the low byte of the shifted
// value, and the OR with the low byte's zero extension reads that 0
// back, which gives
//
//     Mov(Imm(0), Reg(A))      # LDA #0
//     Or(X, Reg(A))            # ORA X
//
// which is `Mov(X, Reg(A))` (LDA X), with the same value in A and the
// same effect on N and Z. Folding it before coalescing leaves direct
// chains of copies from the operand bytes to the bytes of the value
// built, so the move coalescing can give them one color.
//
// `apply_const_arith_fold` isn't used for this. It runs in the
// peephole fixed point after the coloring, and it drops `Or(Imm(0), A)`
// after any write of A as an identity that keeps the flags. That is
// sound when control flow at emission is straight within a block, but
// in the asm SSA form the IR still has `Mov(Reg(A), Pseudo)`
// definitions whose A is read by a later `Or(Imm(0), A)` whose effect
// on the flags sets N and Z from A again for a `Branch` past a join of
// two paths (the `!` operator's sequence that makes a boolean and then
// tests it, for one). Dropping that `ORA #0` here would lose the flags
// read after the join. Folding only the load leaves a `Mov(X, A)` with the
// flags the `LDA #0; ORA X` pair set, which is safe whatever the joins.
#include "passes/optimization_asm/or_zero_absorb.h"

#include <optional>
#include <vector>

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

bool is_reg_a(const ir::Type_operand& op) {
  const auto* r = as<ir::Reg>(op);
  return r != nullptr && isa<ir::A>(r->reg);
}

// Returns the `Mov(X, A)` that `Mov(Imm(0), A); Or(X, A)` folds into,
// or nothing. The new `Mov` isn't volatile: the first `Mov` wasn't, and
// an `Or` has no volatile flag.
std::optional<ir::Mov> try_absorb(const ir::Type_instruction& a, const ir::Type_instruction& b) {
  const auto* mov = as<ir::Mov>(a);
  if (mov == nullptr || mov->is_volatile || !is_reg_a(mov->dst)) {
    return std::nullopt;
  }
  const auto* imm = as<ir::Imm>(mov->src);
  if (imm == nullptr || imm->value != 0) {
    return std::nullopt;
  }
  const auto* orr = as<ir::Or>(b);
  if (orr == nullptr || !is_reg_a(orr->dst)) {
    return std::nullopt;
  }
  return ir::Mov{.src = orr->src, .dst = mov->dst, .is_volatile = false};
}

}  // namespace

ir::Function absorb_zero_load(ir::Function fn) {
  auto& instrs = fn.instructions;
  std::vector<ir::Type_instruction> out;
  out.reserve(instrs.size());
  std::size_t i = 0;
  while (i + 1 < instrs.size()) {
    if (auto folded = try_absorb(instrs[i], instrs[i + 1])) {
      out.push_back(std::move(*folded));
      i += 2;
      continue;
    }
    out.push_back(std::move(instrs[i]));
    ++i;
  }
  for (; i < instrs.size(); ++i) {
    out.push_back(std::move(instrs[i]));
  }
  instrs = std::move(out);
  return fn;
}

}  // namespace c6502::passes
