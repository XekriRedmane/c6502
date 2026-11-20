// Drops the store of an adjacent `LDA M; STA M` pair when the
// surrounding state allows it.
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
//
// The LDA could go only when A and the flags are dead afterward.
// `dead_a_arith_elimination` already makes that check, so this pass
// leaves the LDA in place for that pass to collect in a later sweep of
// the fixed point.
//
// Why other passes don't cover it
//
//   * `redundant_store_elimination` looks for copies from memory to
//     memory that set up an equivalence already known. The first
//     `LDA M; STA M` of a block sets up `known[M] = M`, so only a later
//     identical pair would go.
//   * `redundant_load_elimination` drops `LDA M` after `STA M` when A
//     already holds M, but here the LDA comes first.
//   * `asm_dead_store` would drop the STA only if M were dead
//     afterward, and M is read later.
//
// The self-store is a pattern of its own: the STA writes the same value
// back, so it's redundant whether or not M is read later.
//
// The case that called for it
//
// c99_to_tac lowers `(uint16_t)hi << 8 | (uint16_t)lo`, the composition
// of a pointer from two bytes, with byte operations on each half. Once
// the byte-aligned shift fold turns the low byte of the `<< 8` into
// `Imm(0)` and the high half of the OR becomes `hi | 0 = hi`, the high
// byte's IR is
//
//     LDA hi          // A = hi
//     Or(Imm(0), A)   // dropped by const_arith_fold
//     STA hi          // a self-store
//     (hi is read later)
//
// This pass drops the STA, and dead_a_arith may then drop the LDA if A
// is dead.
//
// Where it runs
//
// Inside the peephole fixed point, after the passes that simplify
// arithmetic (mem_const_prop, const_arith_fold) have removed the
// identity operations in between.
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

// Returns whether `instr` is a move of a `Data` or `ZP` byte to itself
// that isn't volatile.
bool is_self_move(const ir::Type_instruction& instr) {
  const auto* m = as<ir::Mov>(instr);
  return m != nullptr && !m->is_volatile && is_stable_mem(m->src) &&
         is_stable_mem(m->dst) && operands_equal(m->src, m->dst);
}

// Returns whether `a; b` is `LDA M; STA M` on one `Data` or `ZP` byte,
// with neither move volatile, since a volatile LDA has to read again
// and a volatile STA has to write.
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
    // Drops a move of a byte to itself. SSA destruction leaves one
    // when a parallel copy's source and destination coalesce to the
    // same color. `asm_emit` drops it too, but dropping it here keeps
    // later passes (notably `asm_dead_store`) from taking its source
    // for a read of its destination.
    if (is_self_move(instrs[i])) {
      ++i;
      continue;
    }
    if (i + 1 < instrs.size() && is_self_store_pair(instrs[i], instrs[i + 1])) {
      // Keeps the LDA and drops the STA.
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
