#include "passes/and_sign_bit_branch.h"

#include <optional>
#include <utility>
#include <vector>

#include "passes/asm_liveness.h"
#include "passes/redundant_compare.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

bool is_reg_a(const ir::Type_operand& op) {
  const auto* r = as<ir::Reg>(op);
  return r != nullptr && isa<ir::A>(r->reg);
}

// Returns the index of the first label named `name`, or -1.
long first_label(Instrs instrs, const std::string& name) {
  for (std::size_t k = 0; k < instrs.size(); ++k) {
    const auto* l = as<ir::Label>(instrs[k]);
    if (l != nullptr && l->name == name) {
      return static_cast<long>(k);
    }
  }
  return -1;
}

// Returns whether `dead(k)` holds after the branch at `branch_idx` on
// the fall-through and at the branch's target. A target that isn't a
// label of the function counts as live.
template <class Dead>
bool dead_both_ways(Instrs instrs, std::size_t branch_idx, Dead dead) {
  if (!dead(branch_idx + 1)) {
    return false;
  }
  const long k = first_label(instrs, std::get<ir::Branch>(instrs[branch_idx]).target);
  return k >= 0 && dead(static_cast<std::size_t>(k));
}

// Returns whether A is dead after the branch at `branch_idx`, both on
// the fall-through and at the target.
bool a_dead_both_ways(Instrs instrs, const LabelMap& labels, std::size_t branch_idx) {
  return dead_both_ways(instrs, branch_idx,
                        [&](std::size_t k) { return a_dead_at(instrs, labels, k); });
}

// Returns whether N and Z are dead on both paths after the branch at
// `branch_idx`.
bool flags_dead_both_ways(Instrs instrs, const LabelMap& labels, std::size_t branch_idx) {
  return dead_both_ways(instrs, branch_idx,
                        [&](std::size_t k) { return flags_dead_at(instrs, labels, k); });
}

// Returns whether A, C, N, and Z are all dead on both paths after the
// branch at `branch_idx`.
bool all_dead_both_ways(Instrs instrs, const LabelMap& labels, std::size_t branch_idx) {
  return dead_both_ways(instrs, branch_idx, [&](std::size_t k) {
    return a_dead_at(instrs, labels, k) && c_dead_at(instrs, labels, k) &&
           flags_dead_at(instrs, labels, k);
  });
}

// Returns whether A, N, Z, and V are all dead on both paths after the
// branch at `branch_idx`.
bool bit6_dead_both_ways(Instrs instrs, const LabelMap& labels, std::size_t branch_idx) {
  return dead_both_ways(instrs, branch_idx, [&](std::size_t k) {
    return a_dead_at(instrs, labels, k) && flags_dead_at(instrs, labels, k) &&
           v_dead_at(instrs, labels, k);
  });
}

// Returns whether `instr` is `Mov(Reg(A), <memory>)`, a STA that keeps
// A's value and leaves N and Z alone.
bool is_a_store_to_mem(const ir::Type_instruction& instr) {
  const auto* m = as<ir::Mov>(instr);
  return m != nullptr && !m->is_volatile && is_reg_a(m->src) && !isa<ir::Reg>(m->dst);
}

// Returns whether `instr` is a Mov that `asm_emit` writes with an `LDA`,
// which leaves A holding the source and N and Z set from it, or a
// `TXA` or `TYA`, which set N and Z too.
bool is_lda_to_a(const ir::Type_instruction& instr) {
  const auto* m = as<ir::Mov>(instr);
  if (m == nullptr) {
    return false;
  }
  if (const auto* r = as<ir::Reg>(m->src)) {
    return isa<ir::X, ir::Y>(r->reg) && is_reg_a(m->dst);
  }
  return !isa<ir::Reg>(m->dst) || is_reg_a(m->dst);
}

bool is_and_imm(const ir::Type_instruction& instr, int value) {
  const auto* a = as<ir::And>(instr);
  if (a == nullptr) {
    return false;
  }
  const auto* imm = as<ir::Imm>(a->src);
  return imm != nullptr && (imm->value & 0xFF) == value && is_reg_a(a->dst);
}

bool is_eq_ne_branch(const ir::Type_instruction& instr) {
  const auto* b = as<ir::Branch>(instr);
  return b != nullptr && isa<ir::EQ, ir::NE>(b->cond);
}

// Returns the branch to the target of the branch `br` on `if_eq` when
// `br` is a BEQ and on `if_ne` when it is a BNE.
ir::Type_instruction flipped(const ir::Type_instruction& br, ir::Type_condition if_eq,
                             ir::Type_condition if_ne) {
  const auto& b = std::get<ir::Branch>(br);
  return ir::Branch{.cond = isa<ir::EQ>(b.cond) ? std::move(if_eq) : std::move(if_ne),
                    .target = b.target};
}

void rewrite_function(ir::Function& fn) {
  const Instrs instrs = fn.instructions;
  const std::size_t n = instrs.size();
  const LabelMap labels(instrs);
  std::optional<redundant_compare::LabelRefs> refs;
  // Returns whether N and Z at `instrs[i]` were set from A's value by
  // the instruction that last wrote A (`redundant_compare::flags_from`,
  // which passes over stores and conditional branches and follows
  // labels to every way in).
  auto flags_set_from_a = [&](std::size_t i) {
    if (!refs) {
      refs = redundant_compare::refs_with_tables(instrs);
    }
    return redundant_compare::flags_from(instrs, i, redundant_compare::kA, &*refs);
  };
  std::vector<ir::Type_instruction> out;
  out.reserve(n);
  std::size_t i = 0;
  while (i < n) {
    if (i + 2 < n && is_lda_to_a(instrs[i]) && is_and_imm(instrs[i + 1], 0x80) &&
        is_eq_ne_branch(instrs[i + 2]) && a_dead_both_ways(instrs, labels, i + 2)) {
      out.push_back(instrs[i]);
      out.push_back(flipped(instrs[i + 2], ir::PL{}, ir::MI{}));
      i += 3;
      continue;
    }
    if (i + 3 < n && is_lda_to_a(instrs[i]) && is_a_store_to_mem(instrs[i + 1]) &&
        is_and_imm(instrs[i + 2], 0x80) && is_eq_ne_branch(instrs[i + 3]) &&
        a_dead_both_ways(instrs, labels, i + 3)) {
      out.push_back(instrs[i]);
      out.push_back(instrs[i + 1]);
      out.push_back(flipped(instrs[i + 3], ir::PL{}, ir::MI{}));
      i += 4;
      continue;
    }
    if (i + 1 < n && is_and_imm(instrs[i], 0x80) && is_eq_ne_branch(instrs[i + 1]) &&
        flags_set_from_a(i) && a_dead_both_ways(instrs, labels, i + 1) &&
        flags_dead_both_ways(instrs, labels, i + 1)) {
      out.push_back(flipped(instrs[i + 1], ir::PL{}, ir::MI{}));
      i += 2;
      continue;
    }
    if (i + 2 < n) {
      const auto* load = as<ir::Mov>(instrs[i]);
      const auto* mask = as<ir::And>(instrs[i + 1]);
      if (load != nullptr && !load->is_volatile && isa<ir::Data, ir::ZP>(load->src) &&
          is_reg_a(load->dst) && mask != nullptr &&
          mask->src == ir::Type_operand{ir::Imm{.value = 0x40}} && is_reg_a(mask->dst) &&
          is_eq_ne_branch(instrs[i + 2]) && bit6_dead_both_ways(instrs, labels, i + 2)) {
        out.push_back(ir::BitTest{.src = load->src});
        out.push_back(flipped(instrs[i + 2], ir::VC{}, ir::VS{}));
        i += 3;
        continue;
      }
    }
    if (i + 1 < n && is_and_imm(instrs[i], 0x01) && is_eq_ne_branch(instrs[i + 1]) &&
        all_dead_both_ways(instrs, labels, i + 1)) {
      out.push_back(ir::LogicalShiftRight{.dst = std::get<ir::And>(instrs[i]).dst});
      out.push_back(flipped(instrs[i + 1], ir::CC{}, ir::CS{}));
      i += 2;
      continue;
    }
    out.push_back(instrs[i]);
    ++i;
  }
  fn.instructions = std::move(out);
}

}  // namespace

asm_ast::Program apply_and_sign_bit_branch(asm_ast::Program prog) {
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      rewrite_function(*fn);
    }
  }
  return prog;
}

}  // namespace c6502::passes
