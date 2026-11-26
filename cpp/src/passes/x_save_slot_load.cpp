#include "passes/x_save_slot_load.h"

#include <optional>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>

#include "passes/decimal_arith.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

bool is_reg_x(const ir::Type_operand& op) {
  const auto* r = as<ir::Reg>(op);
  return r != nullptr && isa<ir::X>(r->reg);
}

bool is_reg_a(const ir::Type_operand& op) {
  const auto* r = as<ir::Reg>(op);
  return r != nullptr && isa<ir::A>(r->reg);
}

bool is_reg_x_or_y(const ir::Type_operand& op) {
  const auto* r = as<ir::Reg>(op);
  return r != nullptr && isa<ir::X, ir::Y>(r->reg);
}

bool is_reg_y(const ir::Type_operand& op) {
  const auto* r = as<ir::Reg>(op);
  return r != nullptr && isa<ir::Y>(r->reg);
}

struct OpKey {
  bool zp = false;
  std::string name;
  asdl::Int address = 0;
  asdl::Int offset = 0;

  bool operator==(const OpKey&) const = default;
};

std::optional<OpKey> op_key(const ir::Type_operand& op) {
  if (const auto* d = as<ir::Data>(op)) {
    return OpKey{.zp = false, .name = d->name, .offset = d->offset};
  }
  if (const auto* z = as<ir::ZP>(op)) {
    return OpKey{.zp = true, .address = z->address, .offset = z->offset};
  }
  return std::nullopt;
}

bool has_key(const ir::Type_operand& op, const OpKey& key) {
  const auto k = op_key(op);
  return k.has_value() && *k == key;
}

using Instrs = std::vector<ir::Type_instruction>;

bool followed_by_tax(const Instrs& instrs, std::size_t i) {
  for (std::size_t j = i + 1; j < instrs.size(); ++j) {
    const auto& instr = instrs[j];
    if (isa<ir::Label>(instr)) {
      continue;
    }
    if (const auto* mov = as<ir::Mov>(instr)) {
      if (is_reg_a(mov->src) && is_reg_x(mov->dst)) {
        return true;
      }
      if (is_reg_a(mov->dst) || is_reg_x(mov->dst)) {
        return false;
      }
      continue;
    }
    return false;
  }
  return false;
}

bool writes_x(const ir::Type_instruction& instr) {
  if (const auto* mov = as<ir::Mov>(instr)) {
    return is_reg_x(mov->dst);
  }
  if (const auto* pop = as<ir::Pop>(instr)) {
    return is_reg_x(pop->dst);
  }
  if (const auto* inc = as<ir::Inc>(instr)) {
    return is_reg_x(inc->dst);
  }
  if (const auto* dec = as<ir::Dec>(instr)) {
    return is_reg_x(dec->dst);
  }
  return isa<ir::Call, ir::AllocateStack>(instr);
}

bool x_write_keeps_slot(const Instrs& instrs, std::size_t i, const OpKey& key) {
  const auto& instr = instrs[i];
  if (isa<ir::Inc, ir::Dec>(instr)) {
    return true;
  }
  if (const auto* mov = as<ir::Mov>(instr)) {
    if (has_key(mov->src, key)) {
      return true;
    }
    if (is_reg_a(mov->src) && i > 0) {
      const auto* prev = as<ir::Mov>(instrs[i - 1]);
      return prev != nullptr && has_key(prev->dst, key) &&
             !is_reg_x_or_y(prev->src);
    }
    return false;
  }
  if (isa<ir::Call>(instr) && i + 1 < instrs.size()) {
    const auto* nxt = as<ir::Mov>(instrs[i + 1]);
    return nxt != nullptr && is_reg_x(nxt->dst) && has_key(nxt->src, key);
  }
  return false;
}

std::vector<bool> x_holds_slot(const Instrs& instrs, const OpKey& key) {
  std::unordered_map<std::string, std::size_t> label_idx;
  for (std::size_t k = 0; k < instrs.size(); ++k) {
    if (const auto* l = as<ir::Label>(instrs[k])) {
      label_idx[l->name] = k;
    }
  }
  const std::size_t n = instrs.size();
  if (n == 0) {
    return {};
  }
  std::vector<int> state(n, -1);
  state[0] = 0;
  std::vector<std::size_t> work = {0};
  auto target_of = [&](const std::string& name) -> std::optional<std::size_t> {
    const auto it = label_idx.find(name);
    if (it == label_idx.end()) {
      return std::nullopt;
    }
    return it->second;
  };
  while (!work.empty()) {
    const std::size_t k = work.back();
    work.pop_back();
    const auto& instr = instrs[k];
    int out = state[k];
    if (const auto* mov = as<ir::Mov>(instr)) {
      if ((is_reg_x(mov->src) && has_key(mov->dst, key)) ||
          (is_reg_x(mov->dst) && has_key(mov->src, key)) ||
          (is_reg_a(mov->src) && is_reg_x(mov->dst) &&
           x_write_keeps_slot(instrs, k, key))) {
        out = 1;
      }
    }
    std::vector<std::size_t> succs;
    if (isa<ir::Ret, ir::Return>(instr)) {
    } else if (const auto* jump = as<ir::Jump>(instr)) {
      if (const auto t = target_of(jump->target)) {
        succs.push_back(*t);
      }
    } else {
      if (k + 1 < n) {
        succs.push_back(k + 1);
      }
      if (const auto* br = as<ir::Branch>(instr)) {
        if (const auto t = target_of(br->target)) {
          succs.push_back(*t);
        }
      }
    }
    for (const std::size_t succ : succs) {
      const int old = state[succ];
      const int now = old < 0 ? out : (old != 0 && out != 0 ? 1 : 0);
      if (now != old) {
        state[succ] = now;
        work.push_back(succ);
      }
    }
  }
  std::vector<bool> out(n);
  for (std::size_t k = 0; k < n; ++k) {
    out[k] = state[k] != 0;
  }
  return out;
}

void rewrite_function(ir::Function& fn) {
  const Instrs& instrs = fn.instructions;
  std::vector<OpKey> candidates;
  for (const auto& instr : instrs) {
    if (const auto* mov = as<ir::Mov>(instr); mov != nullptr && is_reg_x(mov->src)) {
      if (auto k = op_key(mov->dst)) {
        bool seen = false;
        for (const auto& c : candidates) {
          seen = seen || c == *k;
        }
        if (!seen) {
          candidates.push_back(std::move(*k));
        }
      }
    }
  }
  if (candidates.size() != 1) {
    return;
  }
  const OpKey key = candidates[0];

  bool disqualified = false;
  for (std::size_t i = 0; i < instrs.size() && !disqualified; ++i) {
    const auto& instr = instrs[i];
    if (const auto* da = as<ir::DecimalArith>(instr)) {
      for (const auto& w : decimal_arith::writes(*da)) {
        disqualified = disqualified || has_key(w, key);
      }
      continue;
    }
    if (const auto* mov = as<ir::Mov>(instr)) {
      if (!has_key(mov->dst, key) || is_reg_x(mov->src)) {
        continue;
      }
      if (is_reg_y(mov->src)) {
        disqualified = true;
        continue;
      }
      if (followed_by_tax(instrs, i)) {
        continue;
      }
      disqualified = true;
      continue;
    }
    std::visit(
        [&](const auto& ins) {
          using T = std::decay_t<decltype(ins)>;
          if constexpr (std::is_same_v<T, ir::Inc> || std::is_same_v<T, ir::Dec> ||
                        std::is_same_v<T, ir::ArithmeticShiftLeft> ||
                        std::is_same_v<T, ir::LogicalShiftRight> ||
                        std::is_same_v<T, ir::RotateLeft> ||
                        std::is_same_v<T, ir::RotateRight> ||
                        std::is_same_v<T, ir::Add> || std::is_same_v<T, ir::Sub> ||
                        std::is_same_v<T, ir::And> || std::is_same_v<T, ir::Or> ||
                        std::is_same_v<T, ir::Xor> || std::is_same_v<T, ir::Pop>) {
            if (has_key(ins.dst, key)) {
              disqualified = true;
            }
          }
        },
        instr);
  }
  if (disqualified) {
    return;
  }
  for (std::size_t i = 0; i < instrs.size(); ++i) {
    if (writes_x(instrs[i]) && !x_write_keeps_slot(instrs, i, key)) {
      return;
    }
  }

  // STX has no indirect-Y or absolute,X form.
  const std::vector<bool> holds = x_holds_slot(instrs, key);
  bool any = false;
  for (std::size_t k = 0; k < instrs.size(); ++k) {
    const auto* mov = as<ir::Mov>(instrs[k]);
    if (mov != nullptr && holds[k] && has_key(mov->src, key) &&
        (is_reg_a(mov->dst) || isa<ir::Data, ir::ZP>(mov->dst))) {
      any = true;
      break;
    }
  }
  if (!any) {
    return;
  }
  const ir::Type_operand reg_x = ir::Reg{.reg = ir::X{}};
  for (std::size_t k = 0; k < fn.instructions.size(); ++k) {
    auto* mov = as<ir::Mov>(fn.instructions[k]);
    if (mov != nullptr && holds[k] && has_key(mov->src, key) &&
        (is_reg_a(mov->dst) || isa<ir::Data, ir::ZP>(mov->dst))) {
      mov->src = reg_x;
    }
  }
}

}  // namespace

asm_ast::Program apply_x_save_slot_load(asm_ast::Program prog) {
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      rewrite_function(*fn);
    }
  }
  return prog;
}

}  // namespace c6502::passes
