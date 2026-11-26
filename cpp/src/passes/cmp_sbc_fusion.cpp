#include "passes/cmp_sbc_fusion.h"

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

bool taken_path_safe(Instrs instrs, const LabelMap& labels,
                     const std::string& target) {
  const long idx = labels.find(target);
  if (idx < 0) {
    return false;
  }
  const auto at = static_cast<std::size_t>(idx);
  return a_dead_at(instrs, labels, at) && v_dead_at(instrs, labels, at);
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

bool writes_a_with_flag_effect(const ir::Type_instruction& instr) {
  switch (ir::kind(instr)) {
    case ir::Kind_instruction::Mov:
      return is_reg_a(std::get<ir::Mov>(instr).dst);
    case ir::Kind_instruction::And:
      return is_reg_a(std::get<ir::And>(instr).dst);
    case ir::Kind_instruction::Or:
      return is_reg_a(std::get<ir::Or>(instr).dst);
    case ir::Kind_instruction::Add:
      return is_reg_a(std::get<ir::Add>(instr).dst);
    case ir::Kind_instruction::Sub:
      return is_reg_a(std::get<ir::Sub>(instr).dst);
    case ir::Kind_instruction::Xor:
      return is_reg_a(std::get<ir::Xor>(instr).dst);
    default:
      return false;
  }
}

const ir::Compare* as_cmp_against(const ir::Type_instruction& instr) {
  const auto* c = as<ir::Compare>(instr);
  return c != nullptr && is_reg_a(c->left) ? c : nullptr;
}

const ir::Sub* as_sub_against(const ir::Type_instruction& instr) {
  const auto* s = as<ir::Sub>(instr);
  return s != nullptr && is_reg_a(s->dst) ? s : nullptr;
}

const ir::Branch* as_eligible_branch(const ir::Type_instruction& instr) {
  const auto* b = as<ir::Branch>(instr);
  if (b != nullptr &&
      isa<ir::CC, ir::CS, ir::EQ, ir::NE, ir::MI, ir::PL>(b->cond)) {
    return b;
  }
  return nullptr;
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
  if (const auto* ia = as<ir::Imm>(a)) {
    const auto* ib = as<ir::Imm>(b);
    return ib != nullptr && ia->value == ib->value;
  }
  return false;
}

void rewrite_function(ir::Function& fn) {
  const std::vector<ir::Type_instruction>& instrs = fn.instructions;
  const std::size_t n = instrs.size();
  const auto branch_targets = collect_branch_targets(instrs);
  const LabelMap labels(instrs);
  std::vector<ir::Type_instruction> out;
  bool changed = false;
  std::size_t i = 0;
  while (i < n) {
    std::size_t j = i + 3;
    if (j < n) {
      if (const auto* l = as<ir::Label>(instrs[j]);
          l != nullptr && !branch_targets.contains(l->name)) {
        ++j;
      }
    }
    const bool sec = j < n && isa<ir::SetCarry>(instrs[j]);
    if (!sec) {
      const ir::Branch* b = i + 2 < n ? as<ir::Branch>(instrs[i + 2]) : nullptr;
      if (b == nullptr || !isa<ir::CC>(b->cond)) {
        j = n;
      }
    }
    const std::size_t sub = sec ? j + 1 : j;
    if (sub < n && writes_a_with_flag_effect(instrs[i])) {
      const ir::Compare* cmp = as_cmp_against(instrs[i + 1]);
      const ir::Branch* branch = as_eligible_branch(instrs[i + 2]);
      const ir::Sub* sbc = as_sub_against(instrs[sub]);
      if (cmp != nullptr && branch != nullptr && sbc != nullptr &&
          operands_equal(cmp->right, sbc->src) &&
          taken_path_safe(instrs, labels, branch->target)) {
        out.push_back(instrs[i]);
        out.push_back(ir::SetCarry{});
        out.push_back(instrs[sub]);
        out.push_back(instrs[i + 2]);
        for (std::size_t k = i + 3; k < j; ++k) {
          out.push_back(instrs[k]);
        }
        i = sub + 1;
        changed = true;
        continue;
      }
    }
    out.push_back(instrs[i]);
    ++i;
  }
  if (changed) {
    fn.instructions = std::move(out);
  }
}

}  // namespace

asm_ast::Program apply_cmp_sbc_fusion(asm_ast::Program prog) {
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      rewrite_function(*fn);
    }
  }
  return prog;
}

}  // namespace c6502::passes
