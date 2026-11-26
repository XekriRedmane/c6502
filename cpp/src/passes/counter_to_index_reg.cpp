#include "passes/counter_to_index_reg.h"

#include <algorithm>
#include <cstddef>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "passes/asm_aliasing.h"
#include "passes/asm_liveness.h"
#include "passes/decimal_arith.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

std::vector<ir::Type_operand> operands(const ir::Type_instruction& instr) {
  switch (ir::kind(instr)) {
    case ir::Kind_instruction::DecimalArith:
      return decimal_arith::operands(std::get<ir::DecimalArith>(instr));
    case ir::Kind_instruction::Mov: {
      const auto& i = std::get<ir::Mov>(instr);
      return {i.src, i.dst};
    }
    case ir::Kind_instruction::LoadAddress: {
      const auto& i = std::get<ir::LoadAddress>(instr);
      return {i.src, i.dst};
    }
    case ir::Kind_instruction::Add: {
      const auto& i = std::get<ir::Add>(instr);
      return {i.src, i.dst};
    }
    case ir::Kind_instruction::Sub: {
      const auto& i = std::get<ir::Sub>(instr);
      return {i.src, i.dst};
    }
    case ir::Kind_instruction::And: {
      const auto& i = std::get<ir::And>(instr);
      return {i.src, i.dst};
    }
    case ir::Kind_instruction::Or: {
      const auto& i = std::get<ir::Or>(instr);
      return {i.src, i.dst};
    }
    case ir::Kind_instruction::Xor: {
      const auto& i = std::get<ir::Xor>(instr);
      return {i.src1, i.src2, i.dst};
    }
    case ir::Kind_instruction::Compare: {
      const auto& i = std::get<ir::Compare>(instr);
      return {i.left, i.right};
    }
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
    case ir::Kind_instruction::Pop:
      return {std::get<ir::Pop>(instr).dst};
    case ir::Kind_instruction::Push:
      return {std::get<ir::Push>(instr).src};
    case ir::Kind_instruction::BitTest:
      return {std::get<ir::BitTest>(instr).src};
    default:
      return {};
  }
}

struct Key {
  bool is_zp = false;
  std::string name;
  asdl::Int addr = 0;

  bool operator==(const Key&) const = default;
  bool operator<(const Key& o) const {
    if (is_zp != o.is_zp) {
      return is_zp < o.is_zp;
    }
    if (name != o.name) {
      return name < o.name;
    }
    return addr < o.addr;
  }
};

std::optional<Key> key(const ir::Type_operand& op) {
  if (const auto* d = as<ir::Data>(op)) {
    return Key{.is_zp = false, .name = d->name, .addr = d->offset};
  }
  if (const auto* z = as<ir::ZP>(op)) {
    return Key{.is_zp = true, .name = {}, .addr = z->address + z->offset};
  }
  return std::nullopt;
}

enum class IndexReg { kX, kY };

bool is_index_reg(const ir::Type_reg& reg, IndexReg which) {
  return which == IndexReg::kX ? isa<ir::X>(reg) : isa<ir::Y>(reg);
}

bool touches_reg(const ir::Type_instruction& instr, IndexReg which) {
  if (const auto* call = as<ir::Call>(instr)) {
    const std::string reg = which == IndexReg::kX ? "X" : "Y";
    return !call_preserves(instr, reg) ||
           std::find(call->reg_args.begin(), call->reg_args.end(), reg) !=
               call->reg_args.end();
  }
  if (isa<ir::FunctionPrologue, ir::AllocateStack, ir::Ret, ir::Return,
          ir::LoadAddress, ir::DecimalArith>(instr)) {
    return true;
  }
  if (const auto* jump = as<ir::Jump>(instr); jump != nullptr && !jump->reg_args.empty()) {
    return true;
  }
  for (const auto& op : operands(instr)) {
    if (const auto* r = as<ir::Reg>(op); r != nullptr && is_index_reg(r->reg, which)) {
      return true;
    }
    if (const auto* ix = as<ir::IndexedData>(op);
        ix != nullptr && is_index_reg(ix->index, which)) {
      return true;
    }
    if (which == IndexReg::kY &&
        isa<ir::IndirectY, ir::IndirectZpY, ir::Frame, ir::Stack, ir::Indirect,
            ir::IndirectZp>(op)) {
      return true;
    }
  }
  return false;
}

const std::string* target(const ir::Type_instruction& instr) {
  if (const auto* j = as<ir::Jump>(instr)) {
    return &j->target;
  }
  if (const auto* b = as<ir::Branch>(instr)) {
    return &b->target;
  }
  return nullptr;
}

std::vector<std::size_t> successors(Instrs instrs, std::size_t i,
                                    const LabelMap& labels) {
  const auto& ins = instrs[i];
  if (isa<ir::Return, ir::Ret>(ins)) {
    return {};
  }
  const std::string* t = target(ins);
  if (isa<ir::Jump>(ins)) {
    const long k = labels.find(*t);
    if (k >= 0) {
      return {static_cast<std::size_t>(k)};
    }
    return {};
  }
  std::vector<std::size_t> out;
  if (i + 1 < instrs.size()) {
    out.push_back(i + 1);
  }
  if (t != nullptr) {
    const long k = labels.find(*t);
    if (k >= 0) {
      out.push_back(static_cast<std::size_t>(k));
    }
  }
  return out;
}

std::optional<std::vector<bool>> loop_region(Instrs instrs, std::size_t init_i,
                                             std::size_t dec_i,
                                             const LabelMap& labels) {
  const std::size_t n = instrs.size();
  std::vector<std::vector<std::size_t>> succ(n);
  for (std::size_t i = 0; i < n; ++i) {
    succ[i] = successors(instrs, i, labels);
  }
  std::vector<bool> forward(n, false);
  std::vector<std::size_t> stack{init_i + 1};
  while (!stack.empty()) {
    const std::size_t i = stack.back();
    stack.pop_back();
    if (i >= n || forward[i]) {
      continue;
    }
    forward[i] = true;
    stack.insert(stack.end(), succ[i].begin(), succ[i].end());
  }
  if (!forward[dec_i]) {
    return std::nullopt;
  }
  std::vector<std::vector<std::size_t>> preds(n);
  for (std::size_t i = 0; i < n; ++i) {
    for (std::size_t s : succ[i]) {
      preds[s].push_back(i);
    }
  }
  std::vector<bool> loop(n, false);
  stack.assign(1, dec_i);
  while (!stack.empty()) {
    const std::size_t i = stack.back();
    stack.pop_back();
    if (loop[i] || !forward[i] || i == init_i) {
      continue;
    }
    loop[i] = true;
    stack.insert(stack.end(), preds[i].begin(), preds[i].end());
  }
  for (std::size_t i = 0; i < n; ++i) {
    if (loop[i] || i == init_i) {
      continue;
    }
    for (std::size_t s : succ[i]) {
      if (loop[s]) {
        return std::nullopt;
      }
    }
  }
  return loop;
}

bool rewrite_one(std::vector<ir::Type_instruction>& instrs,
                 const SymbolAddrs* zp_symbol_addrs) {
  const Instrs view(instrs);
  const std::set<asdl::Int> pointer_addrs = pointer_byte_addresses(view);
  std::vector<std::pair<Key, std::vector<std::size_t>>> refs;
  std::map<Key, std::size_t> ref_index;
  std::set<Key> pointer_keys;
  for (std::size_t i = 0; i < instrs.size(); ++i) {
    for (const auto& op : operands(instrs[i])) {
      std::optional<Key> k = key(op);
      if (!k) {
        continue;
      }
      auto [it, inserted] = ref_index.try_emplace(*k, refs.size());
      if (inserted) {
        refs.emplace_back(*k, std::vector<std::size_t>{});
      }
      refs[it->second].second.push_back(i);
      if (is_pointer_byte(op, pointer_addrs, zp_symbol_addrs)) {
        pointer_keys.insert(*k);
      }
    }
  }
  const LabelMap labels(view);
  for (auto& [k, all_idxs] : refs) {
    std::vector<std::size_t> idxs = all_idxs;
    std::sort(idxs.begin(), idxs.end());
    idxs.erase(std::unique(idxs.begin(), idxs.end()), idxs.end());
    if (idxs.size() != 2 || pointer_keys.count(k) != 0) {
      continue;
    }
    const std::size_t init_i = idxs[0];
    const std::size_t dec_i = idxs[1];
    const auto* init = as<ir::Mov>(instrs[init_i]);
    const auto* dec = as<ir::Dec>(instrs[dec_i]);
    if (!(init != nullptr && !init->is_volatile && key(init->dst) == k &&
          isa<ir::Imm, ir::Data, ir::ZP, ir::Reg, ir::IndexedData>(init->src) &&
          dec != nullptr && key(dec->dst) == k && dec_i + 1 < instrs.size())) {
      continue;
    }
    const auto* br = as<ir::Branch>(instrs[dec_i + 1]);
    if (br == nullptr || !isa<ir::NE, ir::EQ>(br->cond)) {
      continue;
    }
    const long head = labels.find(br->target);
    if (!(head >= 0 && init_i < static_cast<std::size_t>(head) &&
          static_cast<std::size_t>(head) <= dec_i)) {
      continue;
    }
    std::optional<std::vector<bool>> loop = loop_region(view, init_i, dec_i, labels);
    if (!loop) {
      continue;
    }
    for (IndexReg which : {IndexReg::kX, IndexReg::kY}) {
      bool touched = false;
      for (std::size_t i = 0; i < instrs.size() && !touched; ++i) {
        touched = (*loop)[i] && i != dec_i && touches_reg(instrs[i], which);
      }
      if (touched) {
        continue;
      }
      // LDX has no abs,X mode and LDY no abs,Y mode, and the 6502 has no
      // TXY or TYX.
      const ir::Type_operand reg =
          which == IndexReg::kX ? ir::Reg{.reg = ir::X{}} : ir::Reg{.reg = ir::Y{}};
      auto dead_at = [&](std::size_t idx) {
        return which == IndexReg::kX ? x_dead_at(view, labels, idx)
                                     : y_dead_at(view, labels, idx);
      };
      const ir::Type_operand src = init->src;
      if (const auto* r = as<ir::Reg>(src); r != nullptr && is_index_reg(r->reg, which)) {
        if (!dead_at(init_i + 1)) {
          continue;
        }
        instrs[dec_i] = ir::Dec{.dst = reg};
        instrs.erase(instrs.begin() + static_cast<std::ptrdiff_t>(init_i));
        return true;
      }
      if (const auto* r = as<ir::Reg>(src); r != nullptr && isa<ir::X, ir::Y>(r->reg)) {
        continue;
      }
      if (const auto* ix = as<ir::IndexedData>(src);
          ix != nullptr && is_index_reg(ix->index, which)) {
        continue;
      }
      if (!dead_at(init_i)) {
        continue;
      }
      instrs[init_i] = ir::Mov{.src = src, .dst = reg, .is_volatile = false};
      instrs[dec_i] = ir::Dec{.dst = reg};
      return true;
    }
  }
  return false;
}

}  // namespace

asm_ast::Program apply_counter_to_index_reg(asm_ast::Program prog,
                                            const SymbolAddrs* zp_symbol_addrs) {
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      while (rewrite_one(fn->instructions, zp_symbol_addrs)) {
      }
    }
  }
  return prog;
}

}  // namespace c6502::passes
