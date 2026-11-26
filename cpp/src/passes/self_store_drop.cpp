// Drops the store of an adjacent `LDA M; STA M` pair.
//
// The pattern
//
// Two consecutive instructions:
//
//     Mov(<mem M>, Reg(A))    // LDA M, so A = M and the flags are M's
//     Mov(Reg(A), <mem M>)    // STA M, so M = A = M and nothing changes
//
// The STA writes the byte M already held, so the pair has no effect on
// M. The LDA sets A and the flags, and the STA leaves both alone.
//
// What can be dropped
//
// The STA is redundant whatever follows. M's value is unchanged, and
// STA writes neither A nor the flags, so the pass drops it.
#include "passes/self_store_drop.h"

#include <utility>
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

bool is_stable_mem(const ir::Type_operand& op) {
  return isa<ir::ZP, ir::Data>(op);
}

bool operands_equal(const ir::Type_operand& a, const ir::Type_operand& b) {
  if (const auto* da = as<ir::Data>(a)) {
    const auto* db = as<ir::Data>(b);
    return db != nullptr && da->name == db->name && da->offset == db->offset;
  }
  if (const auto* za = as<ir::ZP>(a)) {
    const auto* zb = as<ir::ZP>(b);
    return zb != nullptr && za->address == zb->address && za->offset == zb->offset;
  }
  return false;
}

bool is_self_move(const ir::Type_instruction& instr) {
  const auto* m = as<ir::Mov>(instr);
  return m != nullptr && !m->is_volatile && is_stable_mem(m->src) &&
         is_stable_mem(m->dst) && operands_equal(m->src, m->dst);
}

bool is_self_store_pair(const ir::Type_instruction& a, const ir::Type_instruction& b) {
  const auto* lda = as<ir::Mov>(a);
  const auto* sta = as<ir::Mov>(b);
  return lda != nullptr && !lda->is_volatile && is_reg_a(lda->dst) &&
         is_stable_mem(lda->src) && sta != nullptr && !sta->is_volatile &&
         is_reg_a(sta->src) && is_stable_mem(sta->dst) &&
         operands_equal(lda->src, sta->dst);
}

void rewrite_function(ir::Function& fn) {
  auto& instrs = fn.instructions;
  std::vector<ir::Type_instruction> out;
  std::size_t i = 0;
  while (i < instrs.size()) {
    if (is_self_move(instrs[i])) {
      ++i;
      continue;
    }
    if (i + 1 < instrs.size() && is_self_store_pair(instrs[i], instrs[i + 1])) {
      out.push_back(std::move(instrs[i]));
      i += 2;
      continue;
    }
    out.push_back(std::move(instrs[i]));
    ++i;
  }
  instrs = std::move(out);
}

}  // namespace

asm_ast::Program apply_self_store_drop(asm_ast::Program prog) {
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      rewrite_function(*fn);
    }
  }
  return prog;
}

}  // namespace c6502::passes
