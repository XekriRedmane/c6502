// Folds `DEC M` (or `INC M`), `LDA M`, `B<cond>` into `DEC M`,
// `B<cond>`.
//
// The pattern
//
// Three consecutive instructions:
//
//     Dec(M) or Inc(M)        ; sets N and Z from M's new value
//     Mov(M, Reg(A))          ; LDA M, so A = M, and N and Z from M
//     Branch(<cond>, target)  ; B<cond> reads N and Z
//
// become
//
//     Dec(M) or Inc(M)
//     Branch(<cond>, target)
//
// Labels that no jump or branch targets may sit between the `Dec` or
// `Inc` and the load.
//
// Soundness
//
// `Dec(M)` and `Inc(M)` read, modify, and write M in place and set N
// and Z from its new value. `Mov(M, Reg(A))` reads the same value into
// A and sets N and Z to the same bits. So the flags at the branch are
// the same with or without the LDA.
//
// The LDA's write of A is the only difference that can be seen, so the
// LDA can go when A is dead at both successors of the branch, which
// `a_dead_at` checks.
//
// The branch must depend only on N and Z, so that C and V can hold
// anything. That leaves out BCC, BCS, BVC, and BVS, and allows BEQ,
// BNE, BMI, and BPL.
//
// The case that called for it
//
// The end of a loop after a counter's decrement:
//
//     DEC counter         ; counter--, setting N and Z
//     LDA counter         ; A = counter, setting N and Z the same way
//     BPL .loop_start     ; loop while counter >= 0
//
// becomes `DEC counter; BPL .loop_start`, which saves a 2-byte,
// 3-cycle LDA on every iteration. The hand-written `DEX; BPL .loop`
// would be shorter still (DEX is one byte), but it needs the counter in
// X, which is the register allocator's decision.
//
// Where it runs
//
// Inside the asm peephole fixed point, after the arithmetic passes that
// give the DEC / INC form.
#include "passes/dec_inc_branch_fold.h"

#include <optional>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "passes/asm_liveness.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

bool is_reg_a_op(const ir::Type_operand& op) {
  const auto* r = as<ir::Reg>(op);
  return r != nullptr && isa<ir::A>(r->reg);
}

bool stable_mem_eq(const ir::Type_operand& a, const ir::Type_operand& b) {
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

std::set<std::string, std::less<>> collect_branch_targets(Instrs instrs) {
  std::set<std::string, std::less<>> out;
  for (const auto& instr : instrs) {
    if (const auto* j = as<ir::Jump>(instr)) {
      out.insert(j->target);
    } else if (const auto* b = as<ir::Branch>(instr)) {
      out.insert(b->target);
    }
  }
  return out;
}

struct Match {
  std::size_t lda_idx = 0;
  std::size_t branch_idx = 0;
};

// Returns the indices of the load and the branch when `instrs[i]` is an
// `Inc(M)` or `Dec(M)` followed, after any labels that nothing jumps or
// branches to, by `Mov(M, A)` and a branch on N or Z, with A dead
// afterward.
std::optional<Match> try_match(Instrs instrs, const LabelMap& labels, std::size_t i,
                               const std::set<std::string, std::less<>>& branch_targets) {
  const ir::Type_operand* mem = nullptr;
  if (const auto* inc = as<ir::Inc>(instrs[i])) {
    mem = &inc->dst;
  } else if (const auto* dec = as<ir::Dec>(instrs[i])) {
    mem = &dec->dst;
  } else {
    return std::nullopt;
  }
  if (!isa<ir::ZP, ir::Data>(*mem)) {
    return std::nullopt;
  }
  std::size_t j = i + 1;
  // Labels that no branch or jump targets are skipped.
  while (j < instrs.size()) {
    const auto* label = as<ir::Label>(instrs[j]);
    if (label == nullptr) {
      break;
    }
    if (branch_targets.contains(label->name)) {
      return std::nullopt;
    }
    ++j;
  }
  if (j >= instrs.size()) {
    return std::nullopt;
  }
  const auto* lda = as<ir::Mov>(instrs[j]);
  if (lda == nullptr || lda->is_volatile || !is_reg_a_op(lda->dst) ||
      !stable_mem_eq(lda->src, *mem)) {
    return std::nullopt;
  }
  const std::size_t lda_idx = j;
  ++j;
  if (j >= instrs.size()) {
    return std::nullopt;
  }
  const auto* br = as<ir::Branch>(instrs[j]);
  if (br == nullptr || !isa<ir::EQ, ir::NE, ir::MI, ir::PL>(br->cond)) {
    return std::nullopt;
  }
  // A must be dead afterward, on the fall-through and at the branch's
  // target. Without the check at the target, `t = --timer; if (t == 0)
  // { ... } return t;` returned A's old value on the taken path.
  if (!a_dead_at(instrs, labels, j + 1)) {
    return std::nullopt;
  }
  std::optional<std::size_t> target;
  for (std::size_t k = 0; k < instrs.size(); ++k) {
    const auto* label = as<ir::Label>(instrs[k]);
    if (label != nullptr && label->name == br->target) {
      target = k;
      break;
    }
  }
  if (!target || !a_dead_at(instrs, labels, *target)) {
    return std::nullopt;
  }
  return Match{lda_idx, j};
}

void rewrite_function(ir::Function& fn) {
  const auto& instrs = fn.instructions;
  const LabelMap labels(instrs);
  const auto branch_targets = collect_branch_targets(instrs);
  std::vector<ir::Type_instruction> out;
  bool changed = false;
  std::size_t i = 0;
  while (i < instrs.size()) {
    if (const std::optional<Match> match = try_match(instrs, labels, i, branch_targets)) {
      // Everything but the LDA is kept.
      for (std::size_t j = i; j <= match->branch_idx; ++j) {
        if (j != match->lda_idx) {
          out.push_back(instrs[j]);
        }
      }
      changed = true;
      i = match->branch_idx + 1;
      continue;
    }
    out.push_back(instrs[i]);
    ++i;
  }
  if (changed) {
    fn.instructions = std::move(out);
  }
}

}  // namespace

asm_ast::Program apply_dec_inc_branch_fold(asm_ast::Program prog) {
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      rewrite_function(*fn);
    }
  }
  return prog;
}

}  // namespace c6502::passes
