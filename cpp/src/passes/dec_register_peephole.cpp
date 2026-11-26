#include "passes/dec_register_peephole.h"

#include <cstddef>
#include <optional>
#include <utility>
#include <vector>

#include "passes/asm_dead_store.h"
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
  std::size_t length = 0;
};

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

// Returns whether a path from the successors of `instrs[start]` reads
// `tmp` before writing it.
bool tmp_read_after(const ir::Type_operand& tmp, const Instructions& instrs,
                    std::size_t start, const LabelMap& labels) {
  std::vector<bool> seen(instrs.size(), false);
  std::vector<std::size_t> work = asm_dead_store::successors(instrs, start, labels);
  while (!work.empty()) {
    const std::size_t k = work.back();
    work.pop_back();
    if (seen[k]) {
      continue;
    }
    seen[k] = true;
    bool read = false;
    bool written = false;
    operand_roles(instrs[k], [&](const ir::Type_operand& op, bool is_dst) {
      if (op == tmp) {
        (is_dst ? written : read) = true;
      }
    });
    if (read) {
      return true;
    }
    if (!written) {
      for (const std::size_t next : asm_dead_store::successors(instrs, k, labels)) {
        work.push_back(next);
      }
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

// Matches `TXA; SEC; SBC #1; TAX; B<NE|EQ>` (or the same with Y), and
// `TXA; SEC; SBC #1; STA tmp; LDX tmp; B<NE|EQ>`, at `instrs[i]`.
// DEX and DEY set N and Z from the new value and leave A, C, and V.
std::optional<Match> match_decrement(const Instructions& instrs, std::size_t i,
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
  auto is_reg = [&](const ir::Type_operand& op) {
    const auto* r = as<ir::Reg>(op);
    return r != nullptr && r->reg.index() == reg->index();
  };
  std::optional<ir::Type_operand> tmp;
  std::size_t branch_idx = i + 4;
  if (!is_reg(d->dst)) {
    tmp = d->dst;
    const auto* e = as<ir::Mov>(instrs[i + 4]);
    if (e == nullptr || e->src != *tmp || !is_reg(e->dst)) {
      return std::nullopt;
    }
    branch_idx = i + 5;
  }
  if (branch_idx >= instrs.size() || !is_ne_or_eq_branch(instrs[branch_idx])) {
    return std::nullopt;
  }
  if (!(a_dead_at(instrs, labels, branch_idx) && c_dead_at(instrs, labels, branch_idx) &&
        v_dead_at(instrs, labels, branch_idx))) {
    return std::nullopt;
  }
  if (tmp && tmp_read_after(*tmp, instrs, branch_idx, labels)) {
    return std::nullopt;
  }
  return Match{.reg = *reg,
               .branch = std::get<ir::Branch>(instrs[branch_idx]),
               .length = branch_idx + 1 - i};
}

void rewrite_function(ir::Function& fn) {
  const auto& instrs = fn.instructions;
  const LabelMap labels(instrs);
  Instructions out;
  bool changed = false;
  std::size_t i = 0;
  while (i < instrs.size()) {
    if (auto m = match_decrement(instrs, i, labels)) {
      out.push_back(ir::Dec{.dst = ir::Reg{.reg = m->reg}});
      out.push_back(m->branch);
      i += m->length;
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
