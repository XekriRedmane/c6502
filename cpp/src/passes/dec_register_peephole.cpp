#include "passes/dec_register_peephole.h"

#include <cstddef>
#include <optional>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "passes/asm_liveness.h"
#include "passes/decimal_arith.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

using Instructions = std::vector<ir::Type_instruction>;

struct Match {
  ir::Type_reg reg;
  ir::Branch branch;
};

bool reads_reg_a(const ir::Type_instruction& instr) {
  switch (ir::kind(instr)) {
    case ir::Kind_instruction::Mov:
      return is_reg_a(std::get<ir::Mov>(instr).src);
    case ir::Kind_instruction::Add: {
      const auto& i = std::get<ir::Add>(instr);
      return is_reg_a(i.src) || is_reg_a(i.dst);
    }
    case ir::Kind_instruction::Sub: {
      const auto& i = std::get<ir::Sub>(instr);
      return is_reg_a(i.src) || is_reg_a(i.dst);
    }
    case ir::Kind_instruction::And: {
      const auto& i = std::get<ir::And>(instr);
      return is_reg_a(i.src) || is_reg_a(i.dst);
    }
    case ir::Kind_instruction::Or: {
      const auto& i = std::get<ir::Or>(instr);
      return is_reg_a(i.src) || is_reg_a(i.dst);
    }
    case ir::Kind_instruction::Xor: {
      const auto& i = std::get<ir::Xor>(instr);
      return is_reg_a(i.src1) || is_reg_a(i.src2);
    }
    case ir::Kind_instruction::Compare: {
      const auto& i = std::get<ir::Compare>(instr);
      return is_reg_a(i.left) || is_reg_a(i.right);
    }
    case ir::Kind_instruction::Push:
      return is_reg_a(std::get<ir::Push>(instr).src);
    case ir::Kind_instruction::ArithmeticShiftLeft:
      return is_reg_a(std::get<ir::ArithmeticShiftLeft>(instr).dst);
    case ir::Kind_instruction::LogicalShiftRight:
      return is_reg_a(std::get<ir::LogicalShiftRight>(instr).dst);
    case ir::Kind_instruction::RotateLeft:
      return is_reg_a(std::get<ir::RotateLeft>(instr).dst);
    case ir::Kind_instruction::RotateRight:
      return is_reg_a(std::get<ir::RotateRight>(instr).dst);
    default:
      return false;
  }
}

bool writes_reg_a(const ir::Type_instruction& instr) {
  switch (ir::kind(instr)) {
    case ir::Kind_instruction::DecimalArith:
      return true;
    case ir::Kind_instruction::Mov:
      return is_reg_a(std::get<ir::Mov>(instr).dst);
    case ir::Kind_instruction::Add:
      return is_reg_a(std::get<ir::Add>(instr).dst);
    case ir::Kind_instruction::Sub:
      return is_reg_a(std::get<ir::Sub>(instr).dst);
    case ir::Kind_instruction::And:
      return is_reg_a(std::get<ir::And>(instr).dst);
    case ir::Kind_instruction::Or:
      return is_reg_a(std::get<ir::Or>(instr).dst);
    case ir::Kind_instruction::Xor:
      return is_reg_a(std::get<ir::Xor>(instr).dst);
    case ir::Kind_instruction::Pop:
      return is_reg_a(std::get<ir::Pop>(instr).dst);
    case ir::Kind_instruction::ArithmeticShiftLeft:
      return is_reg_a(std::get<ir::ArithmeticShiftLeft>(instr).dst);
    case ir::Kind_instruction::LogicalShiftRight:
      return is_reg_a(std::get<ir::LogicalShiftRight>(instr).dst);
    case ir::Kind_instruction::RotateLeft:
      return is_reg_a(std::get<ir::RotateLeft>(instr).dst);
    case ir::Kind_instruction::RotateRight:
      return is_reg_a(std::get<ir::RotateRight>(instr).dst);
    default:
      return false;
  }
}

bool has_reg_arg_a(const std::vector<std::string>& reg_args) {
  for (const auto& r : reg_args) {
    if (r == "A") {
      return true;
    }
  }
  return false;
}

bool a_dead_from(const Instructions& instrs, std::size_t start, const LabelMap& labels) {
  std::size_t i = start;
  std::set<std::size_t> seen;
  while (i < instrs.size()) {
    if (seen.contains(i)) {
      return false;
    }
    seen.insert(i);
    const auto& instr = instrs[i];
    if (isa<ir::Label>(instr)) {
      ++i;
      continue;
    }
    if (const auto* mov = as<ir::Mov>(instr); mov != nullptr && mov->src == mov->dst) {
      ++i;
      continue;
    }
    if (reads_reg_a(instr)) {
      return false;
    }
    if (writes_reg_a(instr)) {
      return true;
    }
    if (const auto* jump = as<ir::Jump>(instr)) {
      if (has_reg_arg_a(jump->reg_args)) {
        return false;
      }
      const long tgt = labels.find(jump->target);
      if (tgt < 0) {
        return true;
      }
      i = static_cast<std::size_t>(tgt) + 1;
      continue;
    }
    if (isa<ir::Branch>(instr)) {
      return false;
    }
    if (const auto* call = as<ir::Call>(instr)) {
      return !has_reg_arg_a(call->reg_args);
    }
    if (isa<ir::Ret, ir::Return>(instr)) {
      return true;
    }
    ++i;
  }
  return true;
}

bool a_dead_at_successors(const Instructions& instrs, std::size_t branch_idx,
                          const LabelMap& labels) {
  const auto& branch = std::get<ir::Branch>(instrs[branch_idx]);
  if (!a_dead_from(instrs, branch_idx + 1, labels)) {
    return false;
  }
  const long tgt = labels.find(branch.target);
  if (tgt < 0) {
    return true;
  }
  return a_dead_from(instrs, static_cast<std::size_t>(tgt) + 1, labels);
}

template <class Fn>
void operand_roles(const ir::Type_instruction& instr, Fn&& fn) {
  switch (ir::kind(instr)) {
    case ir::Kind_instruction::DecimalArith: {
      const auto& d = std::get<ir::DecimalArith>(instr);
      for (const auto& s : decimal_arith::reads(d)) {
        fn(s, false);
      }
      for (const auto& w : decimal_arith::writes(d)) {
        fn(w, true);
      }
      break;
    }
    case ir::Kind_instruction::Mov: {
      const auto& i = std::get<ir::Mov>(instr);
      fn(i.src, false);
      fn(i.dst, true);
      break;
    }
    case ir::Kind_instruction::Add: {
      const auto& i = std::get<ir::Add>(instr);
      fn(i.src, false);
      fn(i.dst, false);
      break;
    }
    case ir::Kind_instruction::Sub: {
      const auto& i = std::get<ir::Sub>(instr);
      fn(i.src, false);
      fn(i.dst, false);
      break;
    }
    case ir::Kind_instruction::And: {
      const auto& i = std::get<ir::And>(instr);
      fn(i.src, false);
      fn(i.dst, false);
      break;
    }
    case ir::Kind_instruction::Or: {
      const auto& i = std::get<ir::Or>(instr);
      fn(i.src, false);
      fn(i.dst, false);
      break;
    }
    case ir::Kind_instruction::Xor: {
      const auto& i = std::get<ir::Xor>(instr);
      fn(i.src1, false);
      fn(i.src2, false);
      fn(i.dst, false);
      break;
    }
    case ir::Kind_instruction::Inc:
      fn(std::get<ir::Inc>(instr).dst, false);
      break;
    case ir::Kind_instruction::Dec:
      fn(std::get<ir::Dec>(instr).dst, false);
      break;
    case ir::Kind_instruction::ArithmeticShiftLeft:
      fn(std::get<ir::ArithmeticShiftLeft>(instr).dst, false);
      break;
    case ir::Kind_instruction::LogicalShiftRight:
      fn(std::get<ir::LogicalShiftRight>(instr).dst, false);
      break;
    case ir::Kind_instruction::RotateLeft:
      fn(std::get<ir::RotateLeft>(instr).dst, false);
      break;
    case ir::Kind_instruction::RotateRight:
      fn(std::get<ir::RotateRight>(instr).dst, false);
      break;
    case ir::Kind_instruction::Push:
      fn(std::get<ir::Push>(instr).src, false);
      break;
    case ir::Kind_instruction::Pop:
      fn(std::get<ir::Pop>(instr).dst, true);
      break;
    case ir::Kind_instruction::Compare: {
      const auto& i = std::get<ir::Compare>(instr);
      fn(i.left, false);
      fn(i.right, false);
      break;
    }
    case ir::Kind_instruction::LoadAddress: {
      const auto& i = std::get<ir::LoadAddress>(instr);
      fn(i.src, false);
      fn(i.dst, true);
      break;
    }
    case ir::Kind_instruction::BitTest:
      fn(std::get<ir::BitTest>(instr).src, false);
      break;
    default:
      break;
  }
}

bool operand_read_after(const ir::Type_operand& target, const Instructions& instrs,
                        std::size_t start) {
  for (std::size_t k = start; k < instrs.size(); ++k) {
    bool found = false;
    operand_roles(instrs[k], [&](const ir::Type_operand& op, bool is_dst) {
      if (!is_dst && op == target) {
        found = true;
      }
    });
    if (found) {
      return true;
    }
  }
  return false;
}

std::optional<ir::Type_reg> index_register(const ir::Type_operand& op) {
  const auto* reg = as<ir::Reg>(op);
  if (reg != nullptr && isa<ir::X, ir::Y>(reg->reg)) {
    return reg->reg;
  }
  return std::nullopt;
}

bool is_sub_one_from_a(const ir::Type_instruction& instr) {
  const auto* sub = as<ir::Sub>(instr);
  if (sub == nullptr) {
    return false;
  }
  const auto* imm = as<ir::Imm>(sub->src);
  return imm != nullptr && imm->value == 1 && is_reg_a(sub->dst);
}

bool is_ne_or_eq_branch(const ir::Type_instruction& instr) {
  const auto* b = as<ir::Branch>(instr);
  return b != nullptr && isa<ir::NE, ir::EQ>(b->cond);
}

std::optional<Match> match_dec_register_pattern(const Instructions& instrs, std::size_t i,
                                                const LabelMap& labels) {
  if (i + 5 >= instrs.size()) {
    return std::nullopt;
  }
  const auto* a = as<ir::Mov>(instrs[i]);
  if (a == nullptr) {
    return std::nullopt;
  }
  const auto reg = index_register(a->src);
  if (!reg || !is_reg_a(a->dst)) {
    return std::nullopt;
  }
  if (!isa<ir::SetCarry>(instrs[i + 1])) {
    return std::nullopt;
  }
  if (!is_sub_one_from_a(instrs[i + 2])) {
    return std::nullopt;
  }
  const auto* d = as<ir::Mov>(instrs[i + 3]);
  if (d == nullptr || !is_reg_a(d->src)) {
    return std::nullopt;
  }
  const ir::Type_operand& tmp = d->dst;
  const auto* e = as<ir::Mov>(instrs[i + 4]);
  if (e == nullptr || e->src != tmp) {
    return std::nullopt;
  }
  const auto* e_dst = as<ir::Reg>(e->dst);
  if (e_dst == nullptr || e_dst->reg.index() != reg->index()) {
    return std::nullopt;
  }
  if (!is_ne_or_eq_branch(instrs[i + 5])) {
    return std::nullopt;
  }
  if (!a_dead_at_successors(instrs, i + 5, labels)) {
    return std::nullopt;
  }
  if (operand_read_after(tmp, instrs, i + 6)) {
    return std::nullopt;
  }
  return Match{.reg = *reg, .branch = std::get<ir::Branch>(instrs[i + 5])};
}

// DEX and DEY set N and Z from the new value and leave A, C, and V.
std::optional<Match> match_transfer_back(const Instructions& instrs, std::size_t i,
                                         const LabelMap& labels) {
  if (i + 4 >= instrs.size()) {
    return std::nullopt;
  }
  const auto* a = as<ir::Mov>(instrs[i]);
  if (a == nullptr) {
    return std::nullopt;
  }
  const auto reg = index_register(a->src);
  if (!reg || !is_reg_a(a->dst)) {
    return std::nullopt;
  }
  if (!isa<ir::SetCarry>(instrs[i + 1])) {
    return std::nullopt;
  }
  if (!is_sub_one_from_a(instrs[i + 2])) {
    return std::nullopt;
  }
  const auto* d = as<ir::Mov>(instrs[i + 3]);
  if (d == nullptr || !is_reg_a(d->src)) {
    return std::nullopt;
  }
  const auto* d_dst = as<ir::Reg>(d->dst);
  if (d_dst == nullptr || d_dst->reg.index() != reg->index()) {
    return std::nullopt;
  }
  if (!is_ne_or_eq_branch(instrs[i + 4])) {
    return std::nullopt;
  }
  if (!(c_dead_at(instrs, labels, i + 4) && v_dead_at(instrs, labels, i + 4) &&
        a_dead_at(instrs, labels, i + 4))) {
    return std::nullopt;
  }
  return Match{.reg = *reg, .branch = std::get<ir::Branch>(instrs[i + 4])};
}

void rewrite_function(ir::Function& fn) {
  const auto& instrs = fn.instructions;
  const LabelMap labels(instrs);
  Instructions out;
  bool changed = false;
  std::size_t i = 0;
  while (i < instrs.size()) {
    if (auto m = match_transfer_back(instrs, i, labels)) {
      out.push_back(ir::Dec{.dst = ir::Reg{.reg = m->reg}});
      out.push_back(m->branch);
      i += 5;
      changed = true;
      continue;
    }
    if (auto m = match_dec_register_pattern(instrs, i, labels)) {
      out.push_back(ir::Dec{.dst = ir::Reg{.reg = m->reg}});
      out.push_back(m->branch);
      i += 6;
      changed = true;
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

asm_ast::Program apply_dec_register_peephole(asm_ast::Program prog) {
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      rewrite_function(*fn);
    }
  }
  return prog;
}

}  // namespace c6502::passes
