#include "passes/cmp_through_tmp_fold.h"

#include <set>
#include <string>
#include <utility>
#include <vector>

#include "passes/asm_dead_store.h"
#include "passes/asm_liveness.h"
#include "passes/asm_operands.h"
#include "passes/decimal_arith.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

bool is_reg_a(const ir::Type_operand& op) {
  const auto* r = as<ir::Reg>(op);
  return r != nullptr && isa<ir::A>(r->reg);
}

bool is_stable_mem(const ir::Type_operand& op) { return isa<ir::Data, ir::ZP>(op); }

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

bool src_eligible(const ir::Type_operand& src) {
  return isa<ir::Imm, ir::ImmLabelLow, ir::ImmLabelHigh, ir::Data, ir::ZP, ir::IndexedData,
             ir::IndirectY, ir::IndirectZpY, ir::IndirectZp>(src);
}

bool other_alias_safe(const ir::Type_operand& other, const ir::Type_operand& tmp) {
  if (isa<ir::Imm, ir::ImmLabelLow, ir::ImmLabelHigh>(other)) {
    return true;
  }
  if (isa<ir::Data, ir::ZP>(other)) {
    return !operands_equal(other, tmp);
  }
  return false;
}

std::vector<ir::Type_operand> read_operands(const ir::Type_instruction& instr) {
  switch (ir::kind(instr)) {
    case ir::Kind_instruction::DecimalArith:
      return decimal_arith::reads(std::get<ir::DecimalArith>(instr));
    case ir::Kind_instruction::Mov:
      return {std::get<ir::Mov>(instr).src};
    case ir::Kind_instruction::Compare: {
      const auto& c = std::get<ir::Compare>(instr);
      return {c.left, c.right};
    }
    case ir::Kind_instruction::Add: {
      const auto& a = std::get<ir::Add>(instr);
      return {a.src, a.dst};
    }
    case ir::Kind_instruction::Sub: {
      const auto& a = std::get<ir::Sub>(instr);
      return {a.src, a.dst};
    }
    case ir::Kind_instruction::And: {
      const auto& a = std::get<ir::And>(instr);
      return {a.src, a.dst};
    }
    case ir::Kind_instruction::Or: {
      const auto& a = std::get<ir::Or>(instr);
      return {a.src, a.dst};
    }
    case ir::Kind_instruction::Xor: {
      const auto& x = std::get<ir::Xor>(instr);
      return {x.src1, x.src2, x.dst};
    }
    case ir::Kind_instruction::Push:
      return {std::get<ir::Push>(instr).src};
    case ir::Kind_instruction::Inc:
      return {std::get<ir::Inc>(instr).dst};
    case ir::Kind_instruction::Dec:
      return {std::get<ir::Dec>(instr).dst};
    case ir::Kind_instruction::ArithmeticShiftLeft:
      return {std::get<ir::ArithmeticShiftLeft>(instr).dst};
    case ir::Kind_instruction::LogicalShiftRight:
      return {std::get<ir::LogicalShiftRight>(instr).dst};
    case ir::Kind_instruction::RotateLeft:
      return {std::get<ir::RotateLeft>(instr).dst};
    case ir::Kind_instruction::RotateRight:
      return {std::get<ir::RotateRight>(instr).dst};
    case ir::Kind_instruction::BitTest:
      return {std::get<ir::BitTest>(instr).src};
    default:
      return {};
  }
}

bool reads_tmp(const ir::Type_instruction& instr, const ir::Type_operand& tmp) {
  for (const auto& op : read_operands(instr)) {
    if (operands_equal(op, tmp)) {
      return true;
    }
  }
  return false;
}

const ir::Type_operand* tmp_read(const ir::Type_instruction& d) {
  if (const auto* c = as<ir::Compare>(d)) {
    return is_reg_a(c->left) ? &c->right : nullptr;
  }
  if (const auto* x = as<ir::Xor>(d)) {
    if (!is_reg_a(x->dst)) {
      return nullptr;
    }
    if (is_reg_a(x->src1)) {
      return &x->src2;
    }
    if (is_reg_a(x->src2)) {
      return &x->src1;
    }
    return nullptr;
  }
  return std::visit(
      [](const auto& i) -> const ir::Type_operand* {
        using T = std::decay_t<decltype(i)>;
        if constexpr (std::is_same_v<T, ir::Add> || std::is_same_v<T, ir::Sub> ||
                      std::is_same_v<T, ir::And> || std::is_same_v<T, ir::Or>) {
          return is_reg_a(i.dst) ? &i.src : nullptr;
        } else {
          return nullptr;
        }
      },
      d);
}

ir::Type_instruction with_src(const ir::Type_instruction& d, const ir::Type_operand& src) {
  if (const auto* c = as<ir::Compare>(d)) {
    return ir::Compare{.left = c->left, .right = src};
  }
  if (const auto* x = as<ir::Xor>(d)) {
    if (is_reg_a(x->src1)) {
      return ir::Xor{.src1 = x->src1, .src2 = src, .dst = x->dst};
    }
    return ir::Xor{.src1 = src, .src2 = x->src2, .dst = x->dst};
  }
  ir::Type_instruction out = d;
  std::visit(
      [&](auto& i) {
        if constexpr (requires { i.src; i.dst; }) {
          i.src = src;
        }
      },
      out);
  return out;
}

bool alu_src_eligible(const ir::Type_operand& src) {
  return isa<ir::Imm, ir::ImmLabelLow, ir::ImmLabelHigh, ir::Data, ir::ZP, ir::IndexedData,
             ir::IndirectZpY>(src);
}

bool matches_pattern(const ir::Type_instruction& a, const ir::Type_instruction& b,
                     const ir::Type_instruction& c, const ir::Type_instruction& d) {
  const ir::Type_operand* tmp_cmp = tmp_read(d);
  const auto* lda = as<ir::Mov>(a);
  const auto* sta = as<ir::Mov>(b);
  const auto* ldo = as<ir::Mov>(c);
  if (!(lda != nullptr && (!lda->is_volatile || (ldo != nullptr && !ldo->is_volatile)) &&
        is_reg_a(lda->dst) && sta != nullptr && !sta->is_volatile && is_reg_a(sta->src) &&
        is_stable_mem(sta->dst) && ldo != nullptr && !ldo->is_volatile && is_reg_a(ldo->dst) &&
        tmp_cmp != nullptr && is_stable_mem(*tmp_cmp))) {
    return false;
  }
  const ir::Type_operand& src = lda->src;
  const ir::Type_operand& other = ldo->src;
  if (!operands_equal(sta->dst, *tmp_cmp) || !src_eligible(src)) {
    return false;
  }
  if (!isa<ir::Compare>(d) && !alu_src_eligible(src)) {
    return false;
  }
  return other_alias_safe(other, sta->dst);
}

std::set<std::string> own_private_slots(const ir::Function& fn) {
  const std::string prefix = "__local_" + fn.name + "__";
  std::set<std::string> names;
  std::set<std::string> taken;
  for (const auto& ins : fn.instructions) {
    for_each_operand<false>(ins, [&](const ir::Type_operand& op) {
      if (const auto* d = as<ir::Data>(op)) {
        if (d->name.starts_with(prefix)) {
          names.insert(d->name);
        }
      } else if (const auto* lo = as<ir::ImmLabelLow>(op)) {
        taken.insert(lo->name);
      } else if (const auto* hi = as<ir::ImmLabelHigh>(op)) {
        taken.insert(hi->name);
      }
    });
  }
  for (const auto& t : taken) {
    names.erase(t);
  }
  return names;
}

struct Candidate {
  std::size_t start;
  std::size_t op;
};

void rewrite_function(ir::Function& fn, const AsmContext& ctx,
                      const SymbolAddrs* zp_slot_symbols) {
  const Instrs instrs = fn.instructions;
  const std::size_t n = instrs.size();
  auto src_of = [&](const Candidate& c) -> const ir::Type_operand& {
    return std::get<ir::Mov>(instrs[c.start]).src;
  };
  auto tmp_of = [&](const Candidate& c) -> const ir::Type_operand& {
    return std::get<ir::Mov>(instrs[c.start + 1]).dst;
  };

  std::vector<Candidate> candidates;
  for (std::size_t i = 0; i + 3 < n; ++i) {
    std::size_t j = i + 3;
    if (isa<ir::SetCarry, ir::ClearCarry>(instrs[j]) && j + 1 < n &&
        !isa<ir::Compare>(instrs[j + 1])) {
      ++j;
    }
    if (matches_pattern(instrs[i], instrs[i + 1], instrs[i + 2], instrs[j])) {
      candidates.push_back({i, j});
    }
  }
  if (candidates.empty()) {
    return;
  }

  std::vector<std::pair<ir::Type_operand, std::set<std::size_t>>> groups;
  for (const auto& c : candidates) {
    const ir::Type_operand& tmp = tmp_of(c);
    auto it = groups.begin();
    while (it != groups.end() && it->first != tmp) {
      ++it;
    }
    if (it == groups.end()) {
      groups.emplace_back(tmp, std::set<std::size_t>{});
      it = groups.end() - 1;
    }
    it->second.insert(c.op);
  }
  const std::set<std::string> own = own_private_slots(fn);
  std::vector<ir::Type_operand> foldable;
  for (const auto& [tmp, owned] : groups) {
    const auto* d = as<ir::Data>(tmp);
    if (d == nullptr || own.count(d->name) == 0) {
      continue;
    }
    bool all_owned = true;
    for (std::size_t k = 0; k < n && all_owned; ++k) {
      all_owned = owned.count(k) > 0 || !reads_tmp(instrs[k], tmp);
    }
    if (all_owned) {
      foldable.push_back(tmp);
    }
  }
  auto is_foldable = [&](const ir::Type_operand& tmp) {
    for (const auto& f : foldable) {
      if (f == tmp) {
        return true;
      }
    }
    return false;
  };
  const LabelMap labels(instrs);
  std::vector<const Candidate*> by_start(n, nullptr);
  bool any = false;
  for (const auto& c : candidates) {
    if (is_foldable(tmp_of(c)) ||
        asm_dead_store::mem_dead_after(instrs, labels, c.op, tmp_of(c), ctx, fn.name,
                                       zp_slot_symbols)) {
      by_start[c.start] = &c;
      any = true;
    }
  }
  if (!any) {
    return;
  }

  std::vector<ir::Type_instruction> out;
  out.reserve(n);
  std::size_t i = 0;
  while (i < n) {
    if (const Candidate* c = by_start[i]) {
      out.push_back(ir::Mov{.src = std::get<ir::Mov>(instrs[i + 2]).src,
                            .dst = ir::Reg{.reg = ir::A{}},
                            .is_volatile = false});
      for (std::size_t k = i + 3; k < c->op; ++k) {
        out.push_back(instrs[k]);
      }
      out.push_back(with_src(instrs[c->op], src_of(*c)));
      i = c->op + 1;
      continue;
    }
    out.push_back(instrs[i]);
    ++i;
  }
  fn.instructions = std::move(out);
}

}  // namespace

asm_ast::Program apply_cmp_through_tmp_fold(asm_ast::Program prog, const AsmContext& ctx,
                                            const SymbolAddrs* zp_slot_symbols) {
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      rewrite_function(*fn, ctx, zp_slot_symbols);
    }
  }
  return prog;
}

}  // namespace c6502::passes
