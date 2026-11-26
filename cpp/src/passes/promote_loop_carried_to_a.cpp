// The pass rewrites a function when all of these hold.
//
//   * The function's first instruction that isn't a label, after any
//     zp_abi entry stubs, is `Mov(Reg(A), Data(__local_<fn>__*, 0))`,
//     the body-local save of an A-resident parameter.
//   * Somewhere later there is exactly one `LDY` from the same slot,
//     followed right away by an `Inc` of the slot.
//   * The label immediately before the `LDY` is the loop-start label,
//     with no instructions but labels between them.
//   * The slot has no other access sites in the function.
//   * Y is unused between the entry STA and the loop-start label, which
//     keeps the value parked by the TAY across the preamble.
//   * No instruction between the in-loop INC site and the back-edge
//     branch writes A. A may be read only by a `Compare(Reg(A), _)`
//     whose flags are overwritten before any later branch.
//   * The body contains a `Branch` whose target is the loop-start
//     label, the back edge that closes the carry path.
//
// When all of them pass, the entry STA becomes TAY, a TYA goes just
// before the loop-start label, the in-loop LDY becomes TAY, and the
// in-loop INC becomes `ClearCarry; Add(Imm(1), Reg(A))`.
#include "passes/promote_loop_carried_to_a.h"

#include <cstddef>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "passes/asm_liveness.h"
#include "passes/asm_operands.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

bool starts_with(const std::string& s, const std::string& prefix) {
  return s.compare(0, prefix.size(), prefix) == 0;
}

template <class Fn>
bool any_operand(const ir::Type_instruction& instr, Fn&& fn) {
  bool found = false;
  for_each_operand<true>(instr, [&](const ir::Type_operand& op) { found = found || fn(op); });
  return found;
}

std::optional<std::size_t> find_entry_local_save_idx(
    const std::vector<ir::Type_instruction>& instrs, const std::string& fn_name) {
  const std::string zpabi_prefix = "__zpabi_" + fn_name + "__";
  const std::string local_prefix = "__local_" + fn_name + "__";
  for (std::size_t i = 0; i < instrs.size(); ++i) {
    const auto& instr = instrs[i];
    if (isa<ir::Label>(instr)) {
      continue;
    }
    const auto* mov = as<ir::Mov>(instr);
    if (mov == nullptr || mov->is_volatile) {
      return std::nullopt;
    }
    const auto* src = as<ir::Reg>(mov->src);
    const auto* dst = as<ir::Data>(mov->dst);
    if (src == nullptr || dst == nullptr || dst->offset != 0) {
      return std::nullopt;
    }
    if (starts_with(dst->name, zpabi_prefix)) {
      continue;
    }
    if (starts_with(dst->name, local_prefix) && isa<ir::A>(src->reg)) {
      return i;
    }
    return std::nullopt;
  }
  return std::nullopt;
}

bool is_ldy_from_slot(const ir::Type_instruction& instr, const std::string& slot) {
  const auto* mov = as<ir::Mov>(instr);
  if (mov == nullptr) {
    return false;
  }
  const auto* src = as<ir::Data>(mov->src);
  if (src == nullptr || src->name != slot || src->offset != 0) {
    return false;
  }
  return is_reg_y(mov->dst);
}

bool is_inc_slot(const ir::Type_instruction& instr, const std::string& slot) {
  const auto* inc = as<ir::Inc>(instr);
  if (inc == nullptr) {
    return false;
  }
  const auto* dst = as<ir::Data>(inc->dst);
  return dst != nullptr && dst->name == slot && dst->offset == 0;
}

std::optional<std::size_t> find_ldy_inc_pair(
    const std::vector<ir::Type_instruction>& instrs, std::size_t start,
    const std::string& slot) {
  for (std::size_t i = start; i + 1 < instrs.size(); ++i) {
    if (is_ldy_from_slot(instrs[i], slot) && is_inc_slot(instrs[i + 1], slot)) {
      return i;
    }
  }
  return std::nullopt;
}

std::optional<std::size_t> find_loop_start_label(
    const std::vector<ir::Type_instruction>& instrs, std::size_t pair_idx,
    std::size_t lower_bound) {
  long j = static_cast<long>(pair_idx) - 1;
  std::optional<std::size_t> last_label;
  while (j > static_cast<long>(lower_bound)) {
    if (isa<ir::Label>(instrs[static_cast<std::size_t>(j)])) {
      last_label = static_cast<std::size_t>(j);
      --j;
      continue;
    }
    return last_label;
  }
  return last_label;
}

bool touches_slot(const ir::Type_instruction& instr, const std::string& slot) {
  return any_operand(instr, [&](const ir::Type_operand& op) {
    const auto* d = as<ir::Data>(op);
    return d != nullptr && d->name == slot;
  });
}

// Returns whether `instr` reads or writes Y, by an operand, by the
// `LDY #off` setup of an operand, or as a call that doesn't preserve Y.
bool touches_reg_y(const ir::Type_instruction& instr) {
  return any_operand(instr, [](const ir::Type_operand& op) { return is_reg_y(op); }) ||
         reads_y(instr) || kills_y(instr) || sets_up_y(instr);
}

std::optional<std::size_t> find_back_edge(
    const std::vector<ir::Type_instruction>& instrs, std::size_t start,
    const std::string& target) {
  for (std::size_t i = start; i < instrs.size(); ++i) {
    if (const auto* b = as<ir::Branch>(instrs[i]); b != nullptr && b->target == target) {
      return i;
    }
    if (const auto* j = as<ir::Jump>(instrs[i]); j != nullptr && j->target == target) {
      return i;
    }
  }
  return std::nullopt;
}

bool writes_reg_a(const ir::Type_instruction& instr) {
  switch (ir::kind(instr)) {
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
    case ir::Kind_instruction::Call:
    case ir::Kind_instruction::LoadAddress:
    case ir::Kind_instruction::DecimalArith:
      return true;
    default:
      return false;
  }
}

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
      return is_reg_a(i.src1) || is_reg_a(i.src2) || is_reg_a(i.dst);
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

// A store to memory leaves N and Z alone, so only a register
// destination counts.
bool overwrites_n_z(const ir::Type_instruction& instr) {
  if (isa<ir::Add, ir::Sub, ir::And, ir::Or, ir::Xor, ir::Inc, ir::Dec,
          ir::Compare, ir::ArithmeticShiftLeft, ir::LogicalShiftRight,
          ir::RotateLeft, ir::RotateRight, ir::BitTest>(instr)) {
    return true;
  }
  if (const auto* mov = as<ir::Mov>(instr)) {
    return isa<ir::Reg>(mov->dst);
  }
  if (const auto* pop = as<ir::Pop>(instr)) {
    return isa<ir::Reg>(pop->dst);
  }
  return false;
}

bool is_compare_with_dead_flags(const std::vector<ir::Type_instruction>& instrs,
                                std::size_t idx, std::size_t end) {
  const auto* cmp = as<ir::Compare>(instrs[idx]);
  if (cmp == nullptr || !is_reg_a(cmp->left)) {
    return false;
  }
  for (std::size_t j = idx + 1; j <= end; ++j) {
    if (j == instrs.size()) {
      return true;
    }
    const auto& nxt = instrs[j];
    if (isa<ir::Branch>(nxt)) {
      return false;
    }
    if (overwrites_n_z(nxt)) {
      return true;
    }
  }
  return true;
}

bool a_safe_across_carry(const std::vector<ir::Type_instruction>& instrs,
                         std::size_t start, std::size_t end) {
  for (std::size_t k = start; k < end; ++k) {
    const auto& instr = instrs[k];
    if (writes_reg_a(instr)) {
      return false;
    }
    if (reads_reg_a(instr) && !is_compare_with_dead_flags(instrs, k, end)) {
      return false;
    }
  }
  return true;
}

void rewrite(ir::Function& fn, std::size_t entry_sta_idx, std::size_t loop_start_idx,
             std::size_t pair_idx) {
  auto& instrs = fn.instructions;
  const ir::Type_operand reg_a = ir::Reg{.reg = ir::A{}};
  const ir::Type_operand reg_y = ir::Reg{.reg = ir::Y{}};
  const auto* pair_ldy = as<ir::Mov>(instrs[pair_idx]);
  const bool ldy_volatile = pair_ldy != nullptr && pair_ldy->is_volatile;
  std::vector<ir::Type_instruction> out;
  out.reserve(instrs.size() + 2);
  for (std::size_t k = 0; k < instrs.size(); ++k) {
    if (k == entry_sta_idx) {
      out.push_back(ir::Mov{.src = reg_a, .dst = reg_y, .is_volatile = false});
    } else if (k == loop_start_idx) {
      out.push_back(ir::Mov{.src = reg_y, .dst = reg_a, .is_volatile = false});
      out.push_back(std::move(instrs[k]));
    } else if (k == pair_idx) {
      out.push_back(ir::Mov{.src = reg_a, .dst = reg_y, .is_volatile = ldy_volatile});
    } else if (k == pair_idx + 1) {
      out.push_back(ir::ClearCarry{});
      out.push_back(ir::Add{.src = ir::Imm{.value = 1}, .dst = reg_a});
    } else {
      out.push_back(std::move(instrs[k]));
    }
  }
  instrs = std::move(out);
}

void promote_function(ir::Function& fn) {
  const auto& instrs = fn.instructions;
  const auto entry_sta_idx = find_entry_local_save_idx(instrs, fn.name);
  if (!entry_sta_idx) {
    return;
  }
  const std::string slot = std::get<ir::Data>(std::get<ir::Mov>(instrs[*entry_sta_idx]).dst).name;
  const auto pair_idx = find_ldy_inc_pair(instrs, *entry_sta_idx + 1, slot);
  if (!pair_idx) {
    return;
  }
  const auto loop_start_idx = find_loop_start_label(instrs, *pair_idx, *entry_sta_idx);
  if (!loop_start_idx) {
    return;
  }
  const std::string loop_label = std::get<ir::Label>(instrs[*loop_start_idx]).name;
  for (std::size_t k = 0; k < instrs.size(); ++k) {
    if (k == *entry_sta_idx || k == *pair_idx || k == *pair_idx + 1) {
      continue;
    }
    if (touches_slot(instrs[k], slot)) {
      return;
    }
  }
  for (std::size_t k = *entry_sta_idx + 1; k < *loop_start_idx; ++k) {
    if (touches_reg_y(instrs[k])) {
      return;
    }
  }
  const auto back_edge_idx = find_back_edge(instrs, *pair_idx + 2, loop_label);
  if (!back_edge_idx) {
    return;
  }
  if (!a_safe_across_carry(instrs, *pair_idx + 2, *back_edge_idx)) {
    return;
  }
  rewrite(fn, *entry_sta_idx, *loop_start_idx, *pair_idx);
}

}  // namespace

asm_ast::Program apply_promote_loop_carried_to_a(asm_ast::Program prog) {
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      promote_function(*fn);
    }
  }
  return prog;
}

}  // namespace c6502::passes
