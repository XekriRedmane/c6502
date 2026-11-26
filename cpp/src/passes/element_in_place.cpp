#include "passes/element_in_place.h"

#include <bit>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "passes/asm_aliasing.h"
#include "passes/asm_dead_store.h"
#include "passes/asm_liveness.h"
#include "passes/asm_operands.h"
#include "passes/inc_peephole.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

const ir::Type_operand kA = ir::Reg{.reg = ir::A{}};

enum Cell { kCellA, kCellT, kCellE };

using Facts = std::uint8_t;
constexpr Facts kAT = 1;
constexpr Facts kAE = 2;
constexpr Facts kTE = 4;
constexpr Facts kAll = kAT | kAE | kTE;

Facts pair_of(Cell a, Cell b) {
  if ((a == kCellA && b == kCellT) || (a == kCellT && b == kCellA)) {
    return kAT;
  }
  if ((a == kCellA && b == kCellE) || (a == kCellE && b == kCellA)) {
    return kAE;
  }
  return kTE;
}

Facts kill(Facts s, Cell cell) {
  switch (cell) {
    case kCellA:
      return s & static_cast<Facts>(~(kAT | kAE));
    case kCellT:
      return s & static_cast<Facts>(~(kAT | kTE));
    case kCellE:
      return s & static_cast<Facts>(~(kAE | kTE));
  }
  return s;
}

Facts assign(Facts s, Cell cell, std::optional<Cell> value) {
  s = kill(s, cell);
  if (!value || *value == cell) {
    return s;
  }
  s |= pair_of(cell, *value);
  return std::popcount(static_cast<unsigned>(s)) >= 2 ? kAll : s;
}

bool starts_with(const std::string& s, const std::string& prefix) {
  return s.compare(0, prefix.size(), prefix) == 0;
}

bool is_rmw(const ir::Type_instruction& ins) {
  return isa<ir::Inc, ir::Dec, ir::ArithmeticShiftLeft, ir::LogicalShiftRight,
             ir::RotateLeft, ir::RotateRight>(ins);
}

bool is_alu(const ir::Type_instruction& ins) {
  return isa<ir::Add, ir::Sub, ir::And, ir::Or>(ins);
}

const ir::Type_operand* dst_of(const ir::Type_instruction& ins) {
  switch (ir::kind(ins)) {
    case ir::Kind_instruction::Inc:
      return &std::get<ir::Inc>(ins).dst;
    case ir::Kind_instruction::Dec:
      return &std::get<ir::Dec>(ins).dst;
    case ir::Kind_instruction::ArithmeticShiftLeft:
      return &std::get<ir::ArithmeticShiftLeft>(ins).dst;
    case ir::Kind_instruction::LogicalShiftRight:
      return &std::get<ir::LogicalShiftRight>(ins).dst;
    case ir::Kind_instruction::RotateLeft:
      return &std::get<ir::RotateLeft>(ins).dst;
    case ir::Kind_instruction::RotateRight:
      return &std::get<ir::RotateRight>(ins).dst;
    case ir::Kind_instruction::Add:
      return &std::get<ir::Add>(ins).dst;
    case ir::Kind_instruction::Sub:
      return &std::get<ir::Sub>(ins).dst;
    case ir::Kind_instruction::And:
      return &std::get<ir::And>(ins).dst;
    case ir::Kind_instruction::Or:
      return &std::get<ir::Or>(ins).dst;
    case ir::Kind_instruction::Xor:
      return &std::get<ir::Xor>(ins).dst;
    case ir::Kind_instruction::Pop:
      return &std::get<ir::Pop>(ins).dst;
    default:
      return nullptr;
  }
}

const ir::Type_operand& rmw_dst(const ir::Type_instruction& ins) { return *dst_of(ins); }

ir::Type_instruction with_rmw_dst(const ir::Type_instruction& ins,
                                  const ir::Type_operand& dst) {
  switch (ir::kind(ins)) {
    case ir::Kind_instruction::Inc:
      return ir::Inc{.dst = dst};
    case ir::Kind_instruction::Dec:
      return ir::Dec{.dst = dst};
    case ir::Kind_instruction::ArithmeticShiftLeft:
      return ir::ArithmeticShiftLeft{.dst = dst};
    case ir::Kind_instruction::LogicalShiftRight:
      return ir::LogicalShiftRight{.dst = dst};
    case ir::Kind_instruction::RotateLeft:
      return ir::RotateLeft{.dst = dst};
    default:
      return ir::RotateRight{.dst = dst};
  }
}

bool is_direct(const ir::Type_operand& op) {
  return isa<ir::Imm, ir::ImmLabelLow, ir::ImmLabelHigh, ir::Data, ir::ZP,
             ir::IndexedData, ir::Reg>(op);
}

NameSet named_labels(const std::vector<ir::Type_instruction>& instrs) {
  NameSet out;
  for (const auto& ins : instrs) {
    for_each_operand<true>(ins, [&](const ir::Type_operand& op) {
      if (const auto* lo = as<ir::ImmLabelLow>(op)) {
        out.insert(lo->name);
      } else if (const auto* hi = as<ir::ImmLabelHigh>(op)) {
        out.insert(hi->name);
      }
    });
  }
  return out;
}

struct Pair {
  const ir::Type_operand& t;
  const ir::Type_operand& e;
  const AsmContext& ctx;
  const SymbolAddrs& slots;

  std::optional<Cell> key(const ir::Type_operand& op) const {
    if (op == t) {
      return kCellT;
    }
    if (op == e) {
      return kCellE;
    }
    return std::nullopt;
  }

  bool in_zero_page(const std::string& name) const {
    if (ctx.is_user_zp_global(name) || is_zp_symbol(name)) {
      return true;
    }
    return ctx.layout.addr(name) != nullptr;
  }

  bool may_reach(const ir::Type_operand& op) const {
    if (!may_alias(op, e, ctx, {.zp_slot_symbols = &slots})) {
      return false;
    }
    const auto& el = std::get<ir::IndexedData>(e);
    const auto* d = as<ir::Data>(op);
    return !(d != nullptr && d->name != el.name && !el.name.empty() && el.offset >= 0 &&
             in_zero_page(d->name) && !in_zero_page(el.name));
  }

  Facts write(Facts s, const ir::Type_operand& dst, std::optional<Cell> value,
              bool is_volatile) const {
    if (dst == t) {
      return assign(s, kCellT, value);
    }
    if (dst == e && !is_volatile) {
      return assign(s, kCellE, value);
    }
    if (may_alias(dst, t, ctx, {.zp_slot_symbols = &slots})) {
      s = kill(s, kCellT);
    }
    if (may_reach(dst)) {
      s = kill(s, kCellE);
    }
    return s;
  }

  Facts transfer_mov(const ir::Mov& ins, Facts s) const {
    const auto& src = ins.src;
    const auto& dst = ins.dst;
    const bool vol = ins.is_volatile;
    if (const auto* reg = as<ir::Reg>(dst)) {
      if (isa<ir::A>(reg->reg)) {
        return assign(s, kCellA, vol ? std::nullopt : key(src));
      }
      if (kills_a(ins) || !is_direct(src)) {
        s = kill(s, kCellA);
      }
      if (isa<ir::X>(reg->reg)) {
        s = kill(s, kCellE);
      }
      return s;
    }
    std::optional<Cell> value;
    if (src == kA) {
      value = kCellA;
    } else if (isa<ir::Reg>(src)) {
      if (kills_a(ins)) {
        s = kill(s, kCellA);
      }
    } else {
      s = assign(s, kCellA, vol ? std::nullopt : key(src));
      value = kCellA;
    }
    return write(s, dst, value, vol);
  }

  Facts transfer(const ir::Type_instruction& ins, Facts s) const {
    if (isa<ir::Label, ir::Branch, ir::Compare, ir::BitTest, ir::SetCarry,
            ir::ClearCarry, ir::Push>(ins)) {
      return s;
    }
    if (const auto* jump = as<ir::Jump>(ins)) {
      return starts_with(jump->target, ".") ? s : Facts{0};
    }
    if (const auto* mov = as<ir::Mov>(ins)) {
      return transfer_mov(*mov, s);
    }
    if (is_rmw(ins) || isa<ir::Pop>(ins)) {
      const ir::Type_operand& d = *dst_of(ins);
      if (const auto* reg = as<ir::Reg>(d)) {
        if (isa<ir::A>(reg->reg)) {
          return kill(s, kCellA);
        }
        if (isa<ir::X>(reg->reg)) {
          return kill(s, kCellE);
        }
        return s;
      }
      return write(s, d, std::nullopt, true);
    }
    if (is_alu(ins) || isa<ir::Xor>(ins)) {
      const ir::Type_operand& d = *dst_of(ins);
      if (d == kA) {
        return kill(s, kCellA);
      }
      return 0;
    }
    return 0;
  }

  std::vector<std::optional<Facts>> facts(const std::vector<ir::Type_instruction>& instrs,
                                          const LabelMap& labels,
                                          const NameSet& named) const {
    const std::size_t n = instrs.size();
    std::vector<std::optional<Facts>> before(n);
    if (n == 0) {
      return before;
    }
    before[0] = Facts{0};
    std::vector<std::size_t> work = {0};
    while (!work.empty()) {
      const std::size_t j = work.back();
      work.pop_back();
      const auto& ins = instrs[j];
      const Facts after = transfer(ins, *before[j]);
      std::vector<std::size_t> succ;
      if (const auto* table = as<ir::JumpTable>(ins)) {
        for (const auto& x : table->targets) {
          if (const long k = labels.find(x); k >= 0) {
            succ.push_back(static_cast<std::size_t>(k));
          }
        }
      } else {
        succ = asm_dead_store::successors(instrs, j, labels);
      }
      for (const std::size_t s : succ) {
        Facts state = after;
        if (const auto* label = as<ir::Label>(instrs[s]);
            label != nullptr && named.contains(label->name)) {
          state = 0;
        }
        const std::optional<Facts> old = before[s];
        const Facts updated = old ? static_cast<Facts>(*old & state) : state;
        if (!old || updated != *old) {
          before[s] = updated;
          work.push_back(s);
        }
      }
    }
    return before;
  }

  // The 6502 has no `LDX abs,X` or `CPX abs,X`.
  std::optional<ir::Type_instruction> rewritten(const ir::Type_instruction& ins) const {
    if (const auto* mov = as<ir::Mov>(ins); mov != nullptr && mov->src == t && mov->dst != t) {
      if (const auto* reg = as<ir::Reg>(mov->dst); reg != nullptr && isa<ir::X>(reg->reg)) {
        return std::nullopt;
      }
      return ir::Mov{.src = e, .dst = mov->dst, .is_volatile = mov->is_volatile};
    }
    if (const auto* cmp = as<ir::Compare>(ins);
        cmp != nullptr && cmp->left == kA && cmp->right == t) {
      return ir::Compare{.left = kA, .right = e};
    }
    if (const auto* i = as<ir::Add>(ins); i != nullptr && i->dst == kA && i->src == t) {
      return ir::Add{.src = e, .dst = kA};
    }
    if (const auto* i = as<ir::Sub>(ins); i != nullptr && i->dst == kA && i->src == t) {
      return ir::Sub{.src = e, .dst = kA};
    }
    if (const auto* i = as<ir::And>(ins); i != nullptr && i->dst == kA && i->src == t) {
      return ir::And{.src = e, .dst = kA};
    }
    if (const auto* i = as<ir::Or>(ins); i != nullptr && i->dst == kA && i->src == t) {
      return ir::Or{.src = e, .dst = kA};
    }
    if (const auto* x = as<ir::Xor>(ins);
        x != nullptr && x->dst == kA && ((x->src1 == t) != (x->src2 == t))) {
      return ir::Xor{.src1 = x->src1 == t ? e : x->src1,
                     .src2 = x->src2 == t ? e : x->src2,
                     .dst = kA};
    }
    return std::nullopt;
  }
};

bool writes_whole(const ir::Type_instruction& ins, const ir::Type_operand& t) {
  if (const auto* mov = as<ir::Mov>(ins)) {
    return mov->dst == t;
  }
  if (const auto* pop = as<ir::Pop>(ins)) {
    return pop->dst == t;
  }
  return false;
}

// INC, DEC, and the shifts set N and Z from the value they write (a
// shift sets C too), LDA sets N and Z, and STA sets no flag.
std::vector<std::size_t> sites(const ir::Function& fn, const SymbolAddrs& slots) {
  const auto& instrs = fn.instructions;
  const std::string prefix = "__local_" + fn.name + "__";
  const NameSet taken = named_labels(instrs);
  std::vector<std::size_t> out;
  for (std::size_t i = 0; i + 2 < instrs.size(); ++i) {
    const auto& step = instrs[i];
    const auto& load = instrs[i + 1];
    const auto& store = instrs[i + 2];
    if (!is_rmw(step)) {
      continue;
    }
    const auto* t = as<ir::Data>(rmw_dst(step));
    if (!(t != nullptr && starts_with(t->name, prefix) && !taken.contains(t->name) &&
          slots.contains(t->name))) {
      continue;
    }
    const auto* ld = as<ir::Mov>(load);
    if (!(ld != nullptr && ld->src == rmw_dst(step) && ld->dst == kA && !ld->is_volatile)) {
      continue;
    }
    const auto* st = as<ir::Mov>(store);
    if (st != nullptr && st->src == kA && is_x_indexed(st->dst) && !st->is_volatile) {
      out.push_back(i);
    }
  }
  return out;
}

std::optional<std::vector<ir::Type_instruction>> rewrite_pair(
    const std::vector<ir::Type_instruction>& instrs, const std::vector<std::size_t>& indices,
    const AsmContext& ctx, const SymbolAddrs& slots) {
  const ir::Type_operand t = rmw_dst(instrs[indices[0]]);
  const ir::Type_operand e = std::get<ir::Mov>(instrs[indices[0] + 2]).dst;
  const Pair pair{.t = t, .e = e, .ctx = ctx, .slots = slots};
  const LabelMap labels(instrs);
  const NameSet named = named_labels(instrs);
  const auto facts = pair.facts(instrs, labels, named);
  auto holds_te = [&](std::size_t j) { return facts[j] && (*facts[j] & kTE) != 0; };
  std::vector<std::size_t> good;
  for (const std::size_t i : indices) {
    if (holds_te(i)) {
      good.push_back(i);
    }
  }
  if (good.empty()) {
    return std::nullopt;
  }
  const std::set<std::size_t> starts(good.begin(), good.end());
  std::set<std::size_t> reads;
  std::set<std::size_t> seen;
  std::vector<std::size_t> stack;
  for (const std::size_t i : good) {
    stack.push_back(i + 3);
  }
  while (!stack.empty()) {
    const std::size_t j = stack.back();
    stack.pop_back();
    if (j >= instrs.size() || seen.contains(j)) {
      continue;
    }
    seen.insert(j);
    if (starts.contains(j)) {
      stack.push_back(j + 3);
      continue;
    }
    const auto& ins = instrs[j];
    if (isa<ir::Phi, ir::FunctionPrologue, ir::AllocateStack, ir::DecimalArith>(ins)) {
      return std::nullopt;
    }
    if (asm_dead_store::reads(ins, t, ctx, &slots, true)) {
      if (!holds_te(j) || !pair.rewritten(ins)) {
        return std::nullopt;
      }
      reads.insert(j);
    } else if (writes_whole(ins, t)) {
      continue;
    }
    if (const auto* table = as<ir::JumpTable>(ins)) {
      for (const auto& n : table->targets) {
        if (const long k = labels.find(n); k >= 0) {
          stack.push_back(static_cast<std::size_t>(k));
        }
      }
      continue;
    }
    for (const std::size_t s : asm_dead_store::successors(instrs, j, labels)) {
      stack.push_back(s);
    }
  }
  long gain = 0;
  for (const std::size_t i : good) {
    gain += 1 + (a_dead_at(instrs, labels, i + 3) ? 3 : 0);
  }
  if (gain - static_cast<long>(reads.size()) <= 0) {
    return std::nullopt;
  }
  std::vector<ir::Type_instruction> out;
  out.reserve(instrs.size());
  for (std::size_t k = 0; k < instrs.size(); ++k) {
    const auto& ins = instrs[k];
    if (starts.contains(k)) {
      out.push_back(with_rmw_dst(ins, e));
      out.push_back(ir::Mov{.src = e, .dst = kA, .is_volatile = false});
      continue;
    }
    bool inside = false;
    for (const std::size_t s : starts) {
      if (s < k && k <= s + 2) {
        inside = true;
        break;
      }
    }
    if (inside) {
      continue;
    }
    if (reads.contains(k)) {
      out.push_back(*pair.rewritten(ins));
    } else {
      out.push_back(ins);
    }
  }
  return out;
}

void rewrite_function(ir::Function& fn, const AsmContext& ctx, const SymbolAddrs& slots) {
  const auto& instrs = fn.instructions;
  const auto found = sites(fn, slots);
  if (found.empty()) {
    return;
  }
  std::vector<std::pair<std::pair<ir::Type_operand, ir::Type_operand>,
                        std::vector<std::size_t>>>
      by_pair;
  for (const std::size_t i : found) {
    std::pair<ir::Type_operand, ir::Type_operand> k{rmw_dst(instrs[i]),
                                                    std::get<ir::Mov>(instrs[i + 2]).dst};
    bool placed = false;
    for (auto& [key, indices] : by_pair) {
      if (key == k) {
        indices.push_back(i);
        placed = true;
        break;
      }
    }
    if (!placed) {
      by_pair.push_back({std::move(k), {i}});
    }
  }
  for (const auto& [key, indices] : by_pair) {
    if (auto out = rewrite_pair(instrs, indices, ctx, slots)) {
      fn.instructions = std::move(*out);
      return;
    }
  }
}

}  // namespace

asm_ast::Program apply_element_in_place(asm_ast::Program prog, const AsmContext& ctx,
                                        const SymbolAddrs* zp_slot_symbols) {
  if (zp_slot_symbols == nullptr || zp_slot_symbols->empty()) {
    return prog;
  }
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      rewrite_function(*fn, ctx, *zp_slot_symbols);
    }
  }
  return prog;
}

}  // namespace c6502::passes
