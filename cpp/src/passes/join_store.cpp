#include "passes/join_store.h"

#include <optional>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "passes/asm_aliasing.h"
#include "passes/asm_dead_store.h"
#include "passes/asm_liveness.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

constexpr std::size_t kWindow = 8;
constexpr std::size_t kMaxPreds = 3;

bool is_straight(const ir::Type_instruction& ins) {
  return isa<ir::Mov, ir::Add, ir::Sub, ir::And, ir::Or, ir::Xor, ir::Compare, ir::BitTest,
             ir::ClearCarry, ir::SetCarry, ir::Inc, ir::Dec, ir::Push, ir::Pop,
             ir::ArithmeticShiftLeft, ir::LogicalShiftRight, ir::RotateLeft,
             ir::RotateRight>(ins);
}

bool is_terminator(const ir::Type_instruction& ins) {
  return isa<ir::Jump, ir::Return, ir::Ret, ir::JumpTable>(ins);
}

bool untouched(const ir::Type_instruction& ins, const ir::Type_operand& t,
               const ir::Type_operand& m, const AsmContext& ctx) {
  if (!is_straight(ins)) {
    return false;
  }
  if (const auto* mov = as<ir::Mov>(ins); mov != nullptr && mov->is_volatile) {
    return false;
  }
  for (const auto& op : instruction_operands(ins)) {
    if (isa<ir::Reg, ir::Imm, ir::ImmLabelLow, ir::ImmLabelHigh>(op)) {
      continue;
    }
    if (may_alias(op, t, ctx) || may_alias(op, m, ctx)) {
      return false;
    }
  }
  return true;
}

bool join_pair(const ir::Type_instruction& a, const ir::Type_instruction& b,
               const std::string& fn_name) {
  const auto* lda = as<ir::Mov>(a);
  const auto* sta = as<ir::Mov>(b);
  if (lda == nullptr || sta == nullptr || lda->is_volatile || sta->is_volatile) {
    return false;
  }
  const auto* t = as<ir::Data>(lda->src);
  return t != nullptr && t->name.starts_with("__local_" + fn_name + "__") &&
         is_reg_a(lda->dst) && is_reg_a(sta->src) && isa<ir::Data, ir::ZP>(sta->dst) &&
         sta->dst != lda->src;
}

const std::string* operand_name(const ir::Type_operand& op) {
  if (const auto* d = as<ir::Data>(op)) return &d->name;
  if (const auto* p = as<ir::Pseudo>(op)) return &p->name;
  if (const auto* lo = as<ir::ImmLabelLow>(op)) return &lo->name;
  if (const auto* hi = as<ir::ImmLabelHigh>(op)) return &hi->name;
  if (const auto* x = as<ir::IndexedData>(op)) return &x->name;
  return nullptr;
}

bool names_label(const ir::Type_instruction& ins, const std::set<std::string>& names) {
  if (const auto* b = as<ir::Branch>(ins)) {
    return names.count(b->target) > 0;
  }
  if (const auto* t = as<ir::JumpTable>(ins)) {
    for (const auto& target : t->targets) {
      if (names.count(target) > 0) {
        return true;
      }
    }
    return false;
  }
  if (isa<ir::Label>(ins)) {
    return false;
  }
  for (const auto& op : instruction_operands(ins)) {
    const std::string* name = operand_name(op);
    if (name != nullptr && names.count(*name) > 0) {
      return true;
    }
  }
  return false;
}

std::optional<std::size_t> last_store(const std::vector<ir::Type_instruction>& instrs,
                                      std::size_t end, const ir::Type_operand& t,
                                      const ir::Type_operand& m, const AsmContext& ctx) {
  for (std::size_t j = end; j-- > 0;) {
    const ir::Type_instruction& ins = instrs[j];
    if (const auto* mov = as<ir::Mov>(ins)) {
      if (mov->dst == t && !mov->is_volatile && isa<ir::Reg, ir::Imm>(mov->src)) {
        return j;
      }
    }
    if (!untouched(ins, t, m, ctx)) {
      return std::nullopt;
    }
  }
  return std::nullopt;
}

bool rewrite_at(std::vector<ir::Type_instruction>& instrs, std::size_t i,
                const std::string& fn_name, const AsmContext& ctx,
                const SymbolAddrs* zp_slot_symbols) {
  if (!isa<ir::Label>(instrs[i])) {
    return false;
  }
  if (i > 0 && isa<ir::Label>(instrs[i - 1])) {
    return false;
  }
  std::size_t end = i;
  while (end < instrs.size() && isa<ir::Label>(instrs[end])) {
    ++end;
  }
  std::set<std::string> names;
  for (std::size_t j = i; j < end; ++j) {
    names.insert(std::get<ir::Label>(instrs[j]).name);
  }
  std::size_t k = end;
  bool found = false;
  while (k + 1 < instrs.size() && k - end < kWindow) {
    if (join_pair(instrs[k], instrs[k + 1], fn_name)) {
      found = true;
      break;
    }
    ++k;
  }
  if (!found) {
    return false;
  }
  const ir::Type_operand t = std::get<ir::Mov>(instrs[k]).src;
  const ir::Type_operand m = std::get<ir::Mov>(instrs[k + 1]).dst;
  for (std::size_t j = end; j < k; ++j) {
    if (!untouched(instrs[j], t, m, ctx)) {
      return false;
    }
  }
  const LabelMap labels(instrs);
  if (!(a_dead_at(instrs, labels, k + 2) && flags_dead_at(instrs, labels, k + 2))) {
    return false;
  }
  if (!asm_dead_store::mem_dead_after(instrs, labels, k, t, ctx, fn_name, zp_slot_symbols)) {
    return false;
  }
  std::vector<std::size_t> preds;
  for (std::size_t j = 0; j < instrs.size(); ++j) {
    if (j >= i && j < end) {
      continue;
    }
    const ir::Type_instruction& ins = instrs[j];
    if (const auto* jump = as<ir::Jump>(ins); jump != nullptr && names.count(jump->target) > 0) {
      if (!jump->reg_args.empty()) {
        return false;
      }
      preds.push_back(j);
    } else if (names_label(ins, names)) {
      return false;
    }
  }
  if (i > 0 && !is_terminator(instrs[i - 1])) {
    preds.push_back(i);
  }
  if (preds.empty() || preds.size() > kMaxPreds) {
    return false;
  }
  std::vector<std::size_t> stores;
  for (const std::size_t p : preds) {
    const auto s = last_store(instrs, p, t, m, ctx);
    if (!s) {
      return false;
    }
    stores.push_back(*s);
  }
  for (const std::size_t s : stores) {
    auto& mov = std::get<ir::Mov>(instrs[s]);
    instrs[s] = ir::Mov{.src = mov.src, .dst = m};
  }
  instrs.erase(instrs.begin() + static_cast<std::ptrdiff_t>(k),
               instrs.begin() + static_cast<std::ptrdiff_t>(k + 2));
  return true;
}

void rewrite_function(ir::Function& fn, const AsmContext& ctx,
                      const SymbolAddrs* zp_slot_symbols) {
  auto& instrs = fn.instructions;
  std::size_t i = 0;
  while (i < instrs.size()) {
    if (!rewrite_at(instrs, i, fn.name, ctx, zp_slot_symbols)) {
      ++i;
    }
  }
}

}  // namespace

asm_ast::Program apply_join_store(asm_ast::Program prog, const AsmContext& ctx,
                                  const SymbolAddrs* zp_slot_symbols) {
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      rewrite_function(*fn, ctx, zp_slot_symbols);
    }
  }
  return prog;
}

}  // namespace c6502::passes
