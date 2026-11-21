// Computing a member of a static object in its own bytes.
//
// `c99_to_tac` reads a member of a static struct, or an element of a
// static array at a constant index, with `IndexedLoad` and writes it
// with `IndexedSymbolStore`, so `P.lo++` on a static `P` is
//
//     IndexedLoad(P, 0, t)
//     Binary(+, t, 1, u)
//     IndexedSymbolStore(P, 0, u)
//
// and `tac_to_asm` lowers it as written: copy the member's bytes into
// `t`, add into `u`, and copy `u` back. A plain static `w++` is
// `Binary(+, w, 1, w)`, which lowers to `LDA w; CLC; ADC #1; STA w` per
// byte, and the INC peephole (passes/inc_peephole.py) makes that
// `INC w; BNE; INC w+1`. The member's add never gets that shape, so it
// costs a copy in and a copy out, and the asm optimizer puts the bytes
// in registers or temporaries around the add. Likewise `P.lo = w << 1`
// shifts into a temporary and then copies it into the member.
//
// `plan_in_place_updates` finds the values that can be computed in the
// member's own bytes. `tac_to_asm` then names them by the member's bytes
// (`Data(P, k)`) and skips the store, so the instruction that computes
// the value writes the member. For `P.lo++` it skips the load too, so
// the add reads the member in place and the INC and DEC peepholes see
// their pattern.
//
// The rewrite moves the member's write from the store back to the
// instruction that computes `u`, and for an update it makes every read
// of `t` a read of the member. That's the same program when, in one
// function's instructions:
//
// - the store is the instruction right after the one that defines `u`
//   (a `Binary`, `Unary`, `Copy`, extend, or truncate), it isn't
//   volatile, and `u` is defined only there and read only by the store;
// - for an update, the definition is `t + c` or `t - c` with `c` a
//   constant, or a packed BCD add or subtract of `t` and a constant or a
//   local (`__c6502_bcd_add(t, x)`), `t` is loaded from the bytes the
//   store writes by a load that isn't volatile, `t` and `u` are the same
//   size, and `t` is defined only by the load and read only after it and
//   up to the add. Between the load and the add there's no label, jump,
//   call, store, or other write to the object (only instructions that
//   compute into locals and read memory), so the member still holds
//   `t`'s value wherever `t` is read;
// - otherwise, the definition reads nothing of the object, so writing
//   the member while computing `u` can't change what it reads;
// - `t` and `u` are locals of the function whose address isn't taken,
//   not parameters or statics.
//
// `tac_to_asm` also requires the object to have static storage and not
// to be renamed already, and `u` to need every byte.
#include "passes/in_place_member_update.h"

#include <map>
#include <set>
#include <utility>

#include "bit_precise.h"
#include "passes/asm_context.h"
#include "passes/optimization/var_visit.h"

namespace c6502::passes {

namespace tac = ::c6502::tac_ast;

namespace {

using Counter = std::map<std::string, int, std::less<>>;

int count_of(const Counter& c, std::string_view name) {
  const auto it = c.find(name);
  return it == c.end() ? 0 : it->second;
}

// Returns the destination of an instruction whose result can be
// computed in the member's bytes (Python's `_COMPUTES`), or null.
const tac::Type_val* computed_dst(const tac::Type_instruction& ins) {
  using K = tac::Kind_instruction;
  switch (tac::kind(ins)) {
    case K::Binary:
      return &std::get<tac::Binary>(ins).dst;
    case K::Unary:
      return &std::get<tac::Unary>(ins).dst;
    case K::Copy:
      return &std::get<tac::Copy>(ins).dst;
    case K::SignExtend:
      return &std::get<tac::SignExtend>(ins).dst;
    case K::ZeroExtend:
      return &std::get<tac::ZeroExtend>(ins).dst;
    case K::Truncate:
      return &std::get<tac::Truncate>(ins).dst;
    default:
      return nullptr;
  }
}

// Returns whether `ins` may sit between an update's load and its add: it
// computes into locals and reads memory, but doesn't write memory or
// change the flow of control (Python's `_PASSIVE`).
bool is_passive(const tac::Type_instruction& ins) {
  using K = tac::Kind_instruction;
  switch (tac::kind(ins)) {
    case K::SignExtend:
    case K::ZeroExtend:
    case K::Truncate:
    case K::Unary:
    case K::Binary:
    case K::Copy:
    case K::GetAddress:
    case K::Load:
    case K::IndexedLoad:
    case K::IndexedConstLoad:
    case K::IndirectIndexedLoad:
      return true;
    default:
      return false;
  }
}

// Returns the low byte of a constant index, as Python's `value & 0xFF`.
asdl::Int index_byte(const tac::Constant& c) {
  return asdl::floor_mod(*bit_precise::raw_value(c.const_), 256);
}

// Returns the index of the `IndexedLoad` of `obj` into `t` before
// instruction `j`, when only instructions that leave `obj`'s bytes alone
// come between them, or none.
std::optional<int> load_of(const std::vector<tac::Type_instruction>& instrs, int j,
                           std::string_view t, std::string_view obj) {
  for (int i = j - 1; i >= 0; --i) {
    const auto& ins = instrs[i];
    if (const auto* load = asdl::as<tac::IndexedLoad>(ins)) {
      const auto* dst = asdl::as<tac::Var>(load->dst);
      if (dst != nullptr && dst->name == t && load->name == obj) {
        return i;
      }
    }
    if (!is_passive(ins)) {
      return std::nullopt;
    }
    for (const auto d : defs_in(ins)) {
      if (d == t || d == obj) {
        return std::nullopt;
      }
    }
  }
  return std::nullopt;
}

// When instruction `j` adds a constant to, or subtracts one from, a
// local `t` loaded from byte `base` of `obj`, and the member can stand
// for `t`, returns the index of the load and `t`, else none. A packed
// BCD add or subtract may have `t` as either operand and a local as the
// other. Its chain writes byte k of the member after it reads byte k of
// each operand, so the member can stand for `t` in either place.
std::optional<std::pair<int, std::string>> update_of(
    const std::vector<tac::Type_instruction>& instrs, int j, asdl::Int base,
    const std::string& obj, const std::function<bool(std::string_view)>& renamable,
    const Counter& uses, const ValSize& size_of) {
  const auto* ins = asdl::as<tac::Binary>(instrs[j]);
  if (ins == nullptr) {
    return std::nullopt;
  }
  const tac::Var* t_val = nullptr;
  if (asdl::isa<tac::Add, tac::Subtract>(ins->op)) {
    if (!(asdl::isa<tac::Var>(ins->src1) && asdl::isa<tac::Constant>(ins->src2))) {
      return std::nullopt;
    }
    t_val = asdl::as<tac::Var>(ins->src1);
  } else if (asdl::isa<tac::BcdAdd, tac::BcdSubtract>(ins->op)) {
    std::vector<const tac::Type_val*> loaded;
    for (const tac::Type_val* v : {&ins->src1, &ins->src2}) {
      const auto* var = asdl::as<tac::Var>(*v);
      if (var != nullptr && load_of(instrs, j, var->name, obj).has_value()) {
        loaded.push_back(v);
      }
    }
    if (loaded.size() != 1) {
      return std::nullopt;
    }
    t_val = asdl::as<tac::Var>(*loaded[0]);
    const tac::Type_val& other = loaded[0] == &ins->src1 ? ins->src2 : ins->src1;
    if (const auto* o = asdl::as<tac::Var>(other)) {
      if (o->name == t_val->name || o->name == obj) {
        return std::nullopt;
      }
    }
  } else {
    return std::nullopt;
  }
  const std::string& t = t_val->name;
  const auto* dst = asdl::as<tac::Var>(ins->dst);
  if ((dst != nullptr && t == dst->name) || !renamable(t) ||
      size_of(tac::Type_val{*t_val}) != size_of(ins->dst)) {
    return std::nullopt;
  }
  const auto i = load_of(instrs, j, t, obj);
  if (!i) {
    return std::nullopt;
  }
  const auto& load = std::get<tac::IndexedLoad>(instrs[*i]);
  const auto* index = asdl::as<tac::Constant>(load.index);
  if (load.is_volatile || index == nullptr || index_byte(*index) + load.offset != base) {
    return std::nullopt;
  }
  // Every read of t is between the load and the add.
  int reads = 0;
  for (int k = *i + 1; k <= j; ++k) {
    for (const auto v : uses_in(instrs[k])) {
      reads += v == t;
    }
  }
  if (reads != count_of(uses, t)) {
    return std::nullopt;
  }
  return std::make_pair(*i, t);
}

}  // namespace

std::vector<InPlaceUpdate> plan_in_place_updates(
    const std::vector<tac::Type_instruction>& instrs, const std::vector<std::string>& params,
    const std::function<bool(std::string_view)>& is_local, const ValSize& size_of) {
  Counter defs;
  Counter uses;
  NameSet addressed;
  for (const auto& ins : instrs) {
    for (const auto v : defs_in(ins)) {
      ++defs[std::string(v)];
    }
    for (const auto v : uses_in(ins)) {
      ++uses[std::string(v)];
    }
    if (const auto* get = asdl::as<tac::GetAddress>(ins)) {
      if (const auto* var = asdl::as<tac::Var>(get->operand)) {
        addressed.insert(var->name);
      }
    }
  }
  const NameSet param_set(params.begin(), params.end());
  const auto renamable = [&](std::string_view name) {
    return !param_set.contains(name) && !addressed.contains(name) && count_of(defs, name) == 1 &&
           is_local(name);
  };

  std::vector<InPlaceUpdate> plan;
  NameSet taken;
  for (int j = 0; j + 1 < static_cast<int>(instrs.size()); ++j) {
    const auto& ins = instrs[j];
    const auto* store = asdl::as<tac::IndexedSymbolStore>(instrs[j + 1]);
    const tac::Type_val* dst = computed_dst(ins);
    if (dst == nullptr || !asdl::isa<tac::Var>(*dst) || store == nullptr || store->is_volatile ||
        !asdl::isa<tac::Constant>(store->index) || !(store->src == *dst)) {
      continue;
    }
    const std::string& u = std::get<tac::Var>(*dst).name;
    const std::string& obj = store->name;
    const asdl::Int base = index_byte(std::get<tac::Constant>(store->index)) + store->offset;
    if (!renamable(u) || count_of(uses, u) != 1 || taken.contains(u)) {
      continue;
    }
    const auto update = update_of(instrs, j, base, obj, renamable, uses, size_of);
    if (update && !taken.contains(update->second)) {
      plan.push_back(InPlaceUpdate{.store = j + 1,
                                   .u = u,
                                   .obj = obj,
                                   .base = base,
                                   .load = update->first,
                                   .t = update->second});
      taken.insert(update->second);
      taken.insert(u);
      continue;
    }
    bool reads_obj = false;
    for (const auto v : uses_in(ins)) {
      if (v == obj || taken.contains(v)) {
        reads_obj = true;
        break;
      }
    }
    if (reads_obj) {
      continue;
    }
    plan.push_back(InPlaceUpdate{.store = j + 1, .u = u, .obj = obj, .base = base});
    taken.insert(u);
  }
  return plan;
}

}  // namespace c6502::passes
