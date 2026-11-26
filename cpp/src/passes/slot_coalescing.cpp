// Gives a zero-page local that holds a copy of a parameter the
// parameter's own slot.
//
// When it applies
//
//   * The function is zp_abi, and the slot is one of its own parameter
//     bytes. It isn't a register parameter's byte or a `zeropage(addr)`
//     one, and it isn't in a share (`slot_sharing`), where a caller
//     keeps a value of its own across the call.
//   * The local is one of the function's `__local_F__*` bytes. Neither
//     byte's address is taken in the unit
//     (`asm_aliasing.unreachable_by_pointer`), so only the function's
//     own instructions read or write them: a callee's zero page is
//     disjoint from its caller's, and a store through a pointer can't
//     reach them. Neither is reached by address, by a `ZP` operand or a
//     `(zp),Y` pointer base, which is how a local used as a pointer is
//     spelled. Each is named only at offset 0, never by an
//     `IndexedData`, a `LoadAddress`, or a volatile access (a `volatile`
//     local keeps bytes of its own), and only by instructions whose
//     reads and writes the pass models.
//   * The two are copy-related: one is written with the other's value,
//     by `LDA P; STA L` (through any register) or a memory-to-memory
//     `Mov`.
//   * They don't interfere: no instruction writes one while the other
//     is live after it, except a write of one with the other's value.
//     Liveness is computed over the function's instructions, with
//     nothing live at a return or a tail call, since the caller doesn't
//     read its arguments back. Which register holds which byte's value
//     is followed forward through the function, keeping at a label what
//     is the same on every path into it, and forgetting everything at
//     any instruction other than a load, a transfer, a store, a
//     compare, a branch, or a carry flag change.
//
// Several locals can join one slot, one at a time, each checked against
// everything already in it. A function's locals go into the slots of
// one callee at most, the one whose slots take the most, and only along
// a copy into the slot, which is the copy that goes.
//
// The link metadata records each slot this pass writes as one its
// function assigns, so a unit that declares the function with a `const`
// parameter, and so would keep a value of its own in the slot, is
// rejected by the linker (`slot_sharing.check_linked_shares`).
//
// A callee's parameter slot
//
// A local whose value is copied into a callee's parameter slot for the
// call can be that slot from where it's computed.
//
// A call reads its callee's slots, and a tail call's `JMP` does too. A
// call may also write any callee's slots, since functions that are
// never active together can share bytes, so the local can't be live
// across any call. For the same reason, a store to a slot of another
// function may write the same byte: the linker allocates the slots
// again for the whole program, so two that are apart in this unit can
// share a byte there. Only the same function's slots are certainly
// apart. So the local can't be live across a store into another
// function's slot either.
//
// The slot is a parameter byte of a function this one calls, other
// than a register or `zeropage(addr)` byte, outside every share, and
// out of reach of any pointer. The callee may change its own slot,
// since the local is dead once the callee has it. The link metadata
// gets no record of these, since the function writes the callee's slot
// as it writes any argument.
#include "passes/slot_coalescing.h"

#include <algorithm>
#include <array>
#include <concepts>
#include <cstdint>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "passes/asm_aliasing.h"
#include "passes/decimal_arith.h"
#include "passes/operand_rewrite.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

bool is_rmw(const ir::Type_instruction& ins) {
  return isa<ir::Inc, ir::Dec, ir::ArithmeticShiftLeft, ir::LogicalShiftRight,
             ir::RotateLeft, ir::RotateRight>(ins);
}

bool is_alu(const ir::Type_instruction& ins) {
  return isa<ir::Add, ir::Sub, ir::And, ir::Or, ir::Xor>(ins);
}

bool is_read_only(const ir::Type_instruction& ins) {
  return isa<ir::Compare, ir::BitTest, ir::Push>(ins);
}

bool has_no_operands(const ir::Type_instruction& ins) {
  return isa<ir::Branch, ir::Jump, ir::Label, ir::Return, ir::Ret, ir::Call,
             ir::SetCarry, ir::ClearCarry, ir::FunctionPrologue,
             ir::AllocateStack>(ins);
}

bool keeps_registers(const ir::Type_instruction& ins) {
  return isa<ir::Compare, ir::BitTest, ir::Push, ir::Branch, ir::SetCarry,
             ir::ClearCarry>(ins);
}

const ir::Type_operand* dst_of(const ir::Type_instruction& ins) {
  return std::visit(
      [](const auto& x) -> const ir::Type_operand* {
        if constexpr (requires {
                        { x.dst } -> std::same_as<const ir::Type_operand&>;
                      }) {
          return &x.dst;
        } else {
          return nullptr;
        }
      },
      ins);
}

class Bits {
 public:
  Bits() = default;
  explicit Bits(std::size_t n) : w_((n + 63) / 64, 0) {}
  void set(std::size_t i) { w_[i / 64] |= std::uint64_t{1} << (i % 64); }
  bool test(std::size_t i) const { return (w_[i / 64] >> (i % 64)) & 1; }
  Bits& operator|=(const Bits& o) {
    for (std::size_t k = 0; k < w_.size(); ++k) {
      w_[k] |= o.w_[k];
    }
    return *this;
  }
  Bits minus(const Bits& o) const {
    Bits out = *this;
    for (std::size_t k = 0; k < w_.size(); ++k) {
      out.w_[k] &= ~o.w_[k];
    }
    return out;
  }
  bool operator==(const Bits&) const = default;

 private:
  std::vector<std::uint64_t> w_;
};

class NameIndex {
 public:
  explicit NameIndex(const NameSet& names) : names_(names.begin(), names.end()) {
    for (std::size_t i = 0; i < names_.size(); ++i) {
      index_.emplace(names_[i], static_cast<int>(i));
    }
  }
  int find(const std::string& name) const {
    const auto it = index_.find(name);
    return it == index_.end() ? -1 : it->second;
  }
  int data(const ir::Type_operand& op) const {
    const auto* d = as<ir::Data>(op);
    return d == nullptr ? -1 : find(d->name);
  }
  std::size_t size() const { return names_.size(); }
  const std::string& name(std::size_t i) const { return names_[i]; }

 private:
  std::vector<std::string> names_;
  std::unordered_map<std::string, int> index_;
};

struct ReadsWrites {
  Bits reads;
  Bits writes;
};

ReadsWrites direct_reads_writes(const ir::Type_instruction& ins,
                                const NameIndex& names) {
  ReadsWrites rw{Bits(names.size()), Bits(names.size())};
  const auto add = [&](Bits& bits, const ir::Type_operand& op) {
    if (const int i = names.data(op); i >= 0) {
      bits.set(static_cast<std::size_t>(i));
    }
  };
  if (const auto* dec = as<ir::DecimalArith>(ins)) {
    for (const auto& op : decimal_arith::reads(*dec)) {
      add(rw.reads, op);
    }
    for (const auto& op : decimal_arith::writes(*dec)) {
      add(rw.writes, op);
    }
    return rw;
  }
  if (const auto* mov = as<ir::Mov>(ins)) {
    add(rw.reads, mov->src);
    add(rw.writes, mov->dst);
  } else if (const auto* pop = as<ir::Pop>(ins)) {
    add(rw.writes, pop->dst);
  } else if (is_rmw(ins)) {
    const ir::Type_operand& dst = *dst_of(ins);
    add(rw.reads, dst);
    add(rw.writes, dst);
  } else {
    for (const auto& op : instruction_operands(ins)) {
      add(rw.reads, op);
    }
    if (is_alu(ins)) {
      add(rw.writes, *dst_of(ins));
    }
  }
  return rw;
}

struct Ctx {
  std::unordered_map<std::string, std::string> owners;
  std::unordered_set<std::string> labels;
  const std::unordered_map<std::string, std::string>* all_owners = nullptr;

  const std::string* slot_owner(const std::string& name) const {
    const auto& table =
        all_owners != nullptr && !all_owners->empty() ? *all_owners : owners;
    const auto it = table.find(name);
    return it == table.end() ? nullptr : &it->second;
  }
};

ReadsWrites reads_writes(const ir::Type_instruction& ins, const NameIndex& names,
                         const Ctx& ctx) {
  const auto owner_of = [&](std::size_t i) -> const std::string* {
    const auto it = ctx.owners.find(names.name(i));
    return it == ctx.owners.end() ? nullptr : &it->second;
  };
  if (const auto* call = as<ir::Call>(ins)) {
    ReadsWrites rw{Bits(names.size()), Bits(names.size())};
    for (std::size_t i = 0; i < names.size(); ++i) {
      if (const std::string* owner = owner_of(i)) {
        rw.writes.set(i);
        if (*owner == call->name) {
          rw.reads.set(i);
        }
      }
    }
    return rw;
  }
  if (const auto* jump = as<ir::Jump>(ins);
      jump != nullptr && !ctx.labels.contains(jump->target)) {
    ReadsWrites rw{Bits(names.size()), Bits(names.size())};
    for (std::size_t i = 0; i < names.size(); ++i) {
      if (const std::string* owner = owner_of(i); owner && *owner == jump->target) {
        rw.reads.set(i);
      }
    }
    return rw;
  }
  ReadsWrites rw = direct_reads_writes(ins, names);
  std::vector<ir::Type_operand> written;
  if (const auto* dec = as<ir::DecimalArith>(ins)) {
    written = decimal_arith::writes(*dec);
  } else if (const ir::Type_operand* dst = dst_of(ins)) {
    written.push_back(*dst);
  }
  for (const auto& w : written) {
    const auto* d = as<ir::Data>(w);
    if (d == nullptr || !d->name.starts_with("__zpabi_")) {
      continue;
    }
    const std::string* owner = ctx.slot_owner(d->name);
    for (std::size_t i = 0; i < names.size(); ++i) {
      const std::string* other = owner_of(i);
      if (other != nullptr && (owner == nullptr || *other != *owner)) {
        rw.writes.set(i);
      }
    }
  }
  return rw;
}

std::vector<Bits> live_out(Instrs instrs, const std::vector<ReadsWrites>& rw,
                           std::size_t n_names) {
  const LabelMap label_at(instrs);
  std::vector<std::vector<std::size_t>> succ(instrs.size());
  for (std::size_t k = 0; k < instrs.size(); ++k) {
    succ[k] = slot_coalescing::successors(instrs, k, label_at);
  }
  std::vector<Bits> live_in(instrs.size(), Bits(n_names));
  std::vector<Bits> live_out(instrs.size(), Bits(n_names));
  bool changed = true;
  while (changed) {
    changed = false;
    for (std::size_t k = instrs.size(); k-- > 0;) {
      Bits out(n_names);
      for (const std::size_t s : succ[k]) {
        out |= live_in[s];
      }
      Bits new_in = out.minus(rw[k].writes);
      new_in |= rw[k].reads;
      if (!(out == live_out[k]) || !(new_in == live_in[k])) {
        live_out[k] = std::move(out);
        live_in[k] = std::move(new_in);
        changed = true;
      }
    }
  }
  return live_out;
}

using Holds = std::array<int, 3>;
constexpr Holds kNothing = {-1, -1, -1};
constexpr std::size_t kY = 2;

std::size_t reg_index(const ir::Type_reg& reg) { return reg.index(); }

Holds meet(const std::vector<std::size_t>& preds,
           const std::vector<std::optional<Holds>>& state_out, bool entry) {
  if (entry) {
    return kNothing;
  }
  std::optional<Holds> out;
  for (const std::size_t p : preds) {
    if (!state_out[p]) {
      continue;
    }
    if (!out) {
      out = *state_out[p];
      continue;
    }
    for (std::size_t r = 0; r < 3; ++r) {
      if ((*state_out[p])[r] != (*out)[r]) {
        (*out)[r] = -1;
      }
    }
  }
  return out ? *out : kNothing;
}

std::pair<Holds, int> step(const Holds& holds, const ir::Type_instruction& ins,
                           const NameIndex& names) {
  if (isa<ir::Label>(ins)) {
    return {holds, -1};
  }
  if (const auto* mov = as<ir::Mov>(ins)) {
    Holds next = holds;
    const auto* src_reg = as<ir::Reg>(mov->src);
    if (const auto* dst_reg = as<ir::Reg>(mov->dst)) {
      const int value = src_reg != nullptr ? holds[reg_index(src_reg->reg)]
                                           : names.data(mov->src);
      if (sets_up_y(ins)) {
        next[kY] = -1;
      }
      next[reg_index(dst_reg->reg)] = value;
      return {next, -1};
    }
    const int target = names.data(mov->dst);
    std::size_t reg = 0;
    int value = -1;
    if (src_reg != nullptr) {
      reg = reg_index(src_reg->reg);
      value = holds[reg];
    } else {
      reg = 0;
      value = names.data(mov->src);
    }
    int copy = -1;
    if (target >= 0) {
      if (value >= 0 && value != target) {
        copy = value;
      }
      for (auto& r : next) {
        if (r == target) {
          r = -1;
        }
      }
      next[reg] = target;
    } else if (src_reg == nullptr) {
      next[reg] = value;
    }
    if (sets_up_y(ins)) {
      next[kY] = -1;
    }
    return {next, copy};
  }
  if (keeps_registers(ins)) {
    Holds next = holds;
    if (sets_up_y(ins)) {
      next[kY] = -1;
    }
    return {next, -1};
  }
  return {kNothing, -1};
}

std::vector<std::optional<Holds>> register_states(
    Instrs instrs, const NameIndex& names,
    std::vector<std::vector<std::size_t>>& preds) {
  const LabelMap label_at(instrs);
  preds.assign(instrs.size(), {});
  for (std::size_t k = 0; k < instrs.size(); ++k) {
    for (const std::size_t s : slot_coalescing::successors(instrs, k, label_at)) {
      preds[s].push_back(k);
    }
  }
  std::vector<std::optional<Holds>> state_out(instrs.size());
  bool changed = true;
  while (changed) {
    changed = false;
    for (std::size_t k = 0; k < instrs.size(); ++k) {
      const Holds in = meet(preds[k], state_out, k == 0);
      const Holds out = step(in, instrs[k], names).first;
      if (!state_out[k] || *state_out[k] != out) {
        state_out[k] = out;
        changed = true;
      }
    }
  }
  return state_out;
}

std::set<std::size_t> self_stores(Instrs instrs, const NameSet& names_set) {
  const NameIndex names(names_set);
  std::vector<std::vector<std::size_t>> preds;
  const auto state_out = register_states(instrs, names, preds);
  std::set<std::size_t> drop;
  for (std::size_t k = 0; k < instrs.size(); ++k) {
    const auto* mov = as<ir::Mov>(instrs[k]);
    if (mov == nullptr || mov->is_volatile) {
      continue;
    }
    const auto* src_reg = as<ir::Reg>(mov->src);
    const int dst = names.data(mov->dst);
    if (src_reg == nullptr || dst < 0) {
      continue;
    }
    const Holds holds = meet(preds[k], state_out, k == 0);
    if (holds[reg_index(src_reg->reg)] == dst) {
      drop.insert(k);
    }
  }
  return drop;
}

NameSet by_address(Instrs instrs, const NameSet& names,
                   const SymbolAddrs& slot_symbols) {
  std::set<asdl::Int> reached;
  for (const auto& ins : instrs) {
    for (const auto& op : instruction_operands(ins)) {
      if (const auto* zp = as<ir::ZP>(op)) {
        reached.insert(zp->address + zp->offset);
      } else if (const auto* zi = as<ir::IndirectZp>(op)) {
        reached.insert(zi->address);
        reached.insert(zi->address + 1);
      } else if (const auto* zy = as<ir::IndirectZpY>(op)) {
        reached.insert(zy->address);
        reached.insert(zy->address + 1);
      }
    }
  }
  NameSet out;
  for (const auto& n : names) {
    const auto it = slot_symbols.find(n);
    if (it == slot_symbols.end() || reached.contains(it->second)) {
      out.insert(n);
    }
  }
  return out;
}

std::size_t first_use(Instrs instrs, const std::string& name) {
  for (std::size_t k = 0; k < instrs.size(); ++k) {
    for (const auto& op : instruction_operands(instrs[k])) {
      const auto* d = as<ir::Data>(op);
      if (d != nullptr && d->name == name) {
        return k;
      }
    }
  }
  return instrs.size();
}

using CalleeSlots = std::vector<std::pair<std::string, std::string>>;

const ZpLayout* find_layout(const AbiLayouts& abi, const std::string& name) {
  for (const auto& [f, layout] : abi) {
    if (f == name) {
      return layout ? &*layout : nullptr;
    }
  }
  return nullptr;
}

bool plain_slot(const ZpLayout& layout, std::size_t k) {
  const bool pinned = k < layout.pinned.size() && layout.pinned[k].has_value();
  const bool reg =
      k < layout.param_registers.size() && layout.param_registers[k].has_value();
  return !pinned && !reg;
}

CalleeSlots callee_slots(const ir::Function& fn, const AbiLayouts& abi,
                         const NameSet& shared, const NameSet& unreachable) {
  std::unordered_set<std::string> labels;
  for (const auto& ins : fn.instructions) {
    if (const auto* label = as<ir::Label>(ins)) {
      labels.insert(label->name);
    }
  }
  std::set<std::string> called;
  for (const auto& ins : fn.instructions) {
    if (const auto* call = as<ir::Call>(ins)) {
      called.insert(call->name);
    } else if (const auto* jump = as<ir::Jump>(ins);
               jump != nullptr && !labels.contains(jump->target)) {
      called.insert(jump->target);
    }
  }
  called.erase(fn.name);
  CalleeSlots out;
  for (const auto& g : called) {
    const ZpLayout* layout = find_layout(abi, g);
    if (layout == nullptr) {
      continue;
    }
    for (std::size_t k = 0; k < layout->slot_symbols.size(); ++k) {
      const std::string& sym = layout->slot_symbols[k];
      if (plain_slot(*layout, k) && !shared.contains(sym) &&
          unreachable.contains(sym)) {
        out.emplace_back(sym, g);
      }
    }
  }
  return out;
}

void rename(std::vector<ir::Type_instruction>& instrs,
            const std::unordered_map<std::string, std::string>& renames) {
  for (auto& ins : instrs) {
    for_each_operand_mut(ins, [&](ir::Type_operand& op) {
      if (auto* d = as<ir::Data>(op)) {
        if (const auto it = renames.find(d->name); it != renames.end()) {
          d->name = it->second;
        }
      }
    });
  }
}

std::vector<std::string> coalesce(
    ir::Function& fn, const std::vector<std::string>& slots,
    const CalleeSlots& callee, const NameSet& unreachable,
    const SymbolAddrs& slot_symbols,
    const std::unordered_map<std::string, std::string>& all_owners) {
  const std::vector<ir::Type_instruction>& instrs = fn.instructions;
  const std::string prefix = "__local_" + fn.name + "__";
  const std::unordered_set<std::string> slot_set(slots.begin(), slots.end());
  std::unordered_set<std::string> callee_set;
  for (const auto& [sym, g] : callee) {
    callee_set.insert(sym);
  }
  NameSet names_set;
  for (const auto& ins : instrs) {
    for (const auto& op : instruction_operands(ins)) {
      const auto* d = as<ir::Data>(op);
      if (d != nullptr &&
          (slot_set.contains(d->name) || callee_set.contains(d->name) ||
           d->name.starts_with(prefix)) &&
          unreachable.contains(d->name)) {
        names_set.insert(d->name);
      }
    }
  }
  for (const auto& n : slot_coalescing::excluded(instrs, names_set)) {
    names_set.erase(n);
  }
  for (const auto& n : by_address(instrs, names_set, slot_symbols)) {
    names_set.erase(n);
  }
  Ctx ctx;
  ctx.all_owners = &all_owners;
  CalleeSlots owners;
  for (const auto& [s, g] : callee) {
    if (names_set.contains(s)) {
      owners.emplace_back(s, g);
      ctx.owners.emplace(s, g);
    }
  }
  std::vector<std::string> params;
  for (const auto& s : slots) {
    if (names_set.contains(s)) {
      params.push_back(s);
    }
  }
  std::vector<std::string> targets = params;
  {
    std::vector<std::pair<std::size_t, std::string>> by_use;
    for (const auto& [s, g] : owners) {
      by_use.emplace_back(first_use(instrs, s), s);
    }
    std::stable_sort(by_use.begin(), by_use.end(),
                     [](const auto& x, const auto& y) { return x.first < y.first; });
    for (auto& [use, s] : by_use) {
      targets.push_back(std::move(s));
    }
  }
  if (targets.empty()) {
    return {};
  }
  for (const auto& ins : instrs) {
    if (const auto* label = as<ir::Label>(ins)) {
      ctx.labels.insert(label->name);
    }
  }
  const NameIndex names(names_set);
  std::vector<ReadsWrites> rw;
  rw.reserve(instrs.size());
  for (const auto& ins : instrs) {
    rw.push_back(reads_writes(ins, names, ctx));
  }
  const auto live = live_out(instrs, rw, names.size());
  std::vector<std::optional<int>> copies(instrs.size());
  std::set<std::pair<int, int>> pairs;
  std::set<std::pair<int, int>> into;
  for (const auto& [k, src_name] : slot_coalescing::copy_sources(instrs, names_set)) {
    const int src = names.find(src_name);
    copies[k] = src;
    const Bits writes = direct_reads_writes(instrs[k], names).writes;
    for (std::size_t dst = 0; dst < names.size(); ++dst) {
      if (writes.test(dst)) {
        const int d = static_cast<int>(dst);
        pairs.emplace(std::min(d, src), std::max(d, src));
        into.emplace(src, d);
      }
    }
  }
  std::vector<int> locals;
  for (std::size_t i = 0; i < names.size(); ++i) {
    if (names.name(i).starts_with(prefix)) {
      locals.push_back(static_cast<int>(i));
    }
  }
  const auto interferes = [&](int a, int b) {
    for (std::size_t k = 0; k < instrs.size(); ++k) {
      for (const int w : {a, b}) {
        if (!rw[k].writes.test(static_cast<std::size_t>(w))) {
          continue;
        }
        const int other = w == a ? b : a;
        if (live[k].test(static_cast<std::size_t>(other)) && copies[k] != other) {
          return true;
        }
      }
    }
    return false;
  };
  using Renames = std::unordered_map<std::string, std::string>;
  const auto merge = [&](int slot, std::set<int>& taken, Renames& renames) {
    std::vector<int> members{slot};
    const bool callee_slot = ctx.owners.contains(names.name(slot));
    bool grew = true;
    while (grew) {
      grew = false;
      for (const int local : locals) {
        if (taken.contains(local) ||
            std::find(members.begin(), members.end(), local) != members.end()) {
          continue;
        }
        bool related = false;
        if (callee_slot) {
          related = into.contains({local, slot});
        } else {
          for (const int m : members) {
            related = related ||
                      pairs.contains({std::min(local, m), std::max(local, m)});
          }
        }
        if (!related) {
          continue;
        }
        bool clash = false;
        for (const int m : members) {
          if (interferes(local, m)) {
            clash = true;
            break;
          }
        }
        if (clash) {
          continue;
        }
        members.push_back(local);
        taken.insert(local);
        renames[names.name(local)] = names.name(slot);
        grew = true;
      }
    }
    return static_cast<long>(members.size()) - 1;
  };
  Renames renames;
  std::set<int> taken;
  std::vector<std::string> merged;
  for (const auto& p : params) {
    if (merge(names.find(p), taken, renames) != 0) {
      merged.push_back(p);
    }
  }
  std::set<std::string> callees;
  for (const auto& [s, g] : owners) {
    callees.insert(g);
  }
  std::optional<std::pair<long, Renames>> best;
  for (const auto& g : callees) {
    Renames trial_renames = renames;
    std::set<int> trial_taken = taken;
    long count = 0;
    for (const auto& slot : targets) {
      const auto it = ctx.owners.find(slot);
      if (it != ctx.owners.end() && it->second == g) {
        count += merge(names.find(slot), trial_taken, trial_renames);
      }
    }
    if (count != 0 && (!best || count > best->first)) {
      best.emplace(count, std::move(trial_renames));
    }
  }
  if (best) {
    renames = std::move(best->second);
  }
  if (renames.empty()) {
    return {};
  }
  std::vector<ir::Type_instruction> renamed = instrs;
  rename(renamed, renames);
  NameSet keep;
  for (const auto& n : names_set) {
    if (!renames.contains(n)) {
      keep.insert(n);
    }
  }
  const auto drop = self_stores(renamed, keep);
  std::vector<ir::Type_instruction> out;
  out.reserve(renamed.size());
  for (std::size_t k = 0; k < renamed.size(); ++k) {
    if (!drop.contains(k)) {
      out.push_back(std::move(renamed[k]));
    }
  }
  fn.instructions = std::move(out);
  return merged;
}

}  // namespace

namespace slot_coalescing {

NameSet excluded(Instrs instrs, const NameSet& names) {
  NameSet out;
  for (const auto& ins : instrs) {
    const auto ops = instruction_operands(ins);
    const auto* mov = as<ir::Mov>(ins);
    if (mov != nullptr && mov->is_volatile) {
      for (const auto& op : ops) {
        const auto* d = as<ir::Data>(op);
        if (d != nullptr && names.contains(d->name)) {
          out.insert(d->name);
        }
      }
    }
    for (const auto& op : ops) {
      if (const auto* d = as<ir::Data>(op);
          d != nullptr && names.contains(d->name) && d->offset != 0) {
        out.insert(d->name);
      }
      if (const auto* x = as<ir::IndexedData>(op);
          x != nullptr && names.contains(x->name)) {
        out.insert(x->name);
      }
    }
    const bool known = isa<ir::Mov, ir::Pop>(ins) || is_rmw(ins) || is_alu(ins) ||
                       is_read_only(ins) || has_no_operands(ins);
    if (!known) {
      for (const auto& op : ops) {
        if (const auto* d = as<ir::Data>(op); d != nullptr && names.contains(d->name)) {
          out.insert(d->name);
        }
        if (const auto* x = as<ir::IndexedData>(op);
            x != nullptr && names.contains(x->name)) {
          out.insert(x->name);
        }
      }
    }
  }
  return out;
}

std::vector<std::size_t> successors(Instrs instrs, std::size_t k,
                                    const LabelMap& label_at) {
  const auto& ins = instrs[k];
  if (isa<ir::Return, ir::Ret>(ins)) {
    return {};
  }
  if (const auto* jump = as<ir::Jump>(ins)) {
    const long t = label_at.find(jump->target);
    if (t < 0 || !jump->reg_args.empty()) {
      return {};
    }
    return {static_cast<std::size_t>(t)};
  }
  std::vector<std::size_t> out;
  if (k + 1 < instrs.size()) {
    out.push_back(k + 1);
  }
  if (const auto* br = as<ir::Branch>(ins)) {
    const long t = label_at.find(br->target);
    if (t >= 0) {
      out.push_back(static_cast<std::size_t>(t));
    }
  }
  return out;
}

std::map<std::size_t, std::string> copy_sources(Instrs instrs,
                                                const NameSet& names_set) {
  const NameIndex names(names_set);
  std::vector<std::vector<std::size_t>> preds;
  const auto state_out = register_states(instrs, names, preds);
  std::map<std::size_t, std::string> copies;
  for (std::size_t k = 0; k < instrs.size(); ++k) {
    const int copy =
        step(meet(preds[k], state_out, k == 0), instrs[k], names).second;
    if (copy >= 0) {
      copies[k] = names.name(static_cast<std::size_t>(copy));
    }
  }
  return copies;
}

}  // namespace slot_coalescing

std::pair<asm_ast::Program, WrittenSlots> apply_slot_coalescing(
    asm_ast::Program prog, const AbiLayouts& abi,
    const std::vector<SlotShare>& shares, const SymbolAddrs& slot_symbols) {
  const NameSet unreachable = unreachable_by_pointer(prog);
  NameSet shared;
  for (const auto& share : shares) {
    for (const auto& [f, syms] : share.members) {
      shared.insert(syms.begin(), syms.end());
    }
  }
  std::unordered_map<std::string, std::string> all_owners;
  for (const auto& [f, layout] : abi) {
    if (layout) {
      for (const auto& sym : layout->slot_symbols) {
        all_owners[sym] = f;
      }
    }
  }
  WrittenSlots written;
  for (auto& tl : prog.top_level) {
    auto* fn = as<ir::Function>(tl);
    if (fn == nullptr) {
      continue;
    }
    const ZpLayout* layout = find_layout(abi, fn->name);
    if (layout == nullptr) {
      continue;
    }
    std::vector<std::string> slots;
    for (std::size_t k = 0; k < layout->slot_symbols.size(); ++k) {
      const std::string& s = layout->slot_symbols[k];
      if (plain_slot(*layout, k) && !shared.contains(s) && unreachable.contains(s)) {
        slots.push_back(s);
      }
    }
    const CalleeSlots callee = callee_slots(*fn, abi, shared, unreachable);
    auto merged = coalesce(*fn, slots, callee, unreachable, slot_symbols, all_owners);
    if (!merged.empty()) {
      written.emplace_back(fn->name, std::move(merged));
    }
  }
  return {std::move(prog), std::move(written)};
}

}  // namespace c6502::passes
