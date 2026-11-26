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

// DEC M and INC M set N and Z from M's new value, as LDA M does.
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
