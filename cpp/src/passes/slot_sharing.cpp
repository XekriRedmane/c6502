// The port of passes/slot_sharing.py: keeping a value in a callee's
// parameter bytes across calls.
//
// Background
//
// `zp_slot_allocation` gives each zp_abi function's parameters bytes no
// function on a common call path uses, and `zp_local_allocation` does
// the same for body locals. So a caller that passes a value it still
// needs afterwards keeps it in its own pool and copies it into the
// callee's slot before every call.
//
//     void instr_save(void) {
//         uint8_t *page = Z_HEADER_ADDR;
//         do {
//             if (inc_sector_and_write(page)) goto fail;
//             page += 0x100;
//         } while (--pages != 0);
//         ...
//
// That copies `page` into `inc_sector_and_write`'s slot each time round.
// Zork's own code keeps the pointer in SCRATCH2, which the routines it
// calls leave alone, and copies nothing. `find_shares` finds the values
// that can live in the callee's bytes the same way, and the allocators
// then keep those bytes apart from everything that could change them.
//
// Two kinds of value qualify.
//
// - A local `v` of a function `E` passed as parameter `q` of `G`, and
//   still needed after one of those calls. `E` then keeps `v` in `G`'s
//   slot for `q`, and is a *holder* of the bytes. A local that a copy
//   links to `v` (out of SSA, `page += 0x100` in a loop is `t = page +
//   256; ...; page = t`) goes there too when the two are never live at
//   once, so the copy goes. So does a local that a step links to it,
//   `t = s + k` or `t = s - k` with `s` dead after the step, so each
//   `++sector` of `write(++sector)` is an add in place, and a value a
//   call returns into one of them, which isn't live across that call.
// - A parameter `p` of a zp_abi function `F` passed on as parameter `q`
//   of `G`. `F`'s slot for `p` gets the same address as `G`'s slot for
//   `q`, so `F`'s callers write the value where `G` reads it and `F`
//   copies nothing. `F`'s slot is then a *member* of the bytes, as `G`'s
//   is. A parameter passed on to several callees joins each one's bytes,
//   so all their slots are members of one share.
//
// A set of such slots and holders is a `SlotShare`. The value goes in
// one set of bytes for every function in it. Members join before any
// holder is placed, so that a caller holding its value in two members'
// bytes doesn't keep the members apart. `slot_aliases` gives
// `tac_to_asm` one spelling for each byte of a share, so it doesn't copy
// an argument that's already in the callee's bytes under another
// member's symbol. `tac_to_asm` writes each bound variable as the slot's
// `Data` operand (`bindings` says which), so the copy before the call is
// a store of a byte to itself, which goes.
//
// The conditions
//
// Each function involved has a private pool (`pool_eligible`), so
// everything it calls is bounded, and isn't on a cycle. Each member's
// owner never assigns the parameter or takes its address, and isn't a
// routine with a fixed address. The parameter isn't passed in a register
// or at a fixed address, and is an integer or a pointer. The variable
// has the parameter's size, isn't `volatile` or `register`, and isn't an
// object whose address is taken or that's indexed by name.
//
// A function *writes* the bytes when it calls a member's owner and
// passes, for that member's parameter, anything but its own value in the
// bytes, or when it's a holder and assigns its variable. Then, for each
// bound variable `v` of a function `F`,
//
// 1. a call in `F` to a member's owner passes `v` (or a local sharing
//    its home) for that parameter, or `v` is dead after the call and
//    isn't one of the call's arguments, since the call's argument writes
//    would otherwise overwrite `v`, or read it after overwriting it;
// 2. a call that `v` is live across reaches no function that writes the
//    bytes, other than `F`;
// 3. when any member's owner has external linkage, a call that `v` is
//    live across reaches no function outside this unit (other than a
//    routine with a fixed address, or an owner from another unit,
//    below), since one in another unit might call the owner.
//
// A binding that breaks a condition is dropped, and the check is
// repeated, since dropping an alias can make its function a writer. When
// some of the bindings that break one are parameters of functions with
// external linkage, only those go in that round, since they may be what
// makes rule 3 apply to the others. Each function appears at most once
// in a share, as a member or as a holder, since two of its values can't
// have one home.
//
// Across units
//
// A member's owner may be a function this unit only declares, when the
// declaration makes the parameter `const`, as in `bool
// inc_sector_and_write(uint8_t *const buffer);`. The compiler takes that
// as a promise that the function leaves its parameter's bytes alone, and
// so does everything it calls, and rule 2 can't be checked for what it
// calls here. So each share records what the linker needs to check it
// for the whole program: for each function in it, the callees its value
// is live across (`live`), the callers whose every call to an owner
// passes their own value in the bytes (`forwards`), and the holders that
// assign their variable (`writers`). The linker merges the shares of all
// units and repeats rule 2 with the program's call graph and each
// definition's record of the parameters it assigns.
//
// The allocators (`allocate_zp_slots`, `allocate_function_locals`) keep
// a share's bytes off every other byte of every function that can be on
// the call stack with any function in it, which rule 2 relies on.
//
// `check_shares` reads the finished asm. A pass after `find_shares` that
// made an owner write its shared bytes would make the caller read a
// value that changed, so the finished program is checked once for such a
// write.
//
// Python keys the bindings by `id()`. Here they are held in a vector and
// keyed by their index in it, which is the order Python's lists keep
// them in.
#include "passes/slot_sharing.h"

#include <algorithm>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <tuple>
#include <utility>

#include "errors.h"
#include "passes/asm_context.h"
#include "passes/call_edges.h"
#include "passes/recursion_saves.h"
#include "passes/replace_pseudoregisters.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;

// Calls `fn` on each operand that `instr` writes in memory.
template <class Fn>
void memory_writes(const ir::Type_instruction& instr, Fn&& fn) {
  switch (ir::kind(instr)) {
    case ir::Kind_instruction::DecimalArith:
      for (const auto& op : std::get<ir::DecimalArith>(instr).dst) {
        fn(op);
      }
      return;
    case ir::Kind_instruction::Mov:
      fn(std::get<ir::Mov>(instr).dst);
      return;
    case ir::Kind_instruction::Inc:
      fn(std::get<ir::Inc>(instr).dst);
      return;
    case ir::Kind_instruction::Dec:
      fn(std::get<ir::Dec>(instr).dst);
      return;
    case ir::Kind_instruction::ArithmeticShiftLeft:
      fn(std::get<ir::ArithmeticShiftLeft>(instr).dst);
      return;
    case ir::Kind_instruction::LogicalShiftRight:
      fn(std::get<ir::LogicalShiftRight>(instr).dst);
      return;
    case ir::Kind_instruction::RotateLeft:
      fn(std::get<ir::RotateLeft>(instr).dst);
      return;
    case ir::Kind_instruction::RotateRight:
      fn(std::get<ir::RotateRight>(instr).dst);
      return;
    case ir::Kind_instruction::Pop:
      fn(std::get<ir::Pop>(instr).dst);
      return;
    case ir::Kind_instruction::LoadAddress:
      fn(std::get<ir::LoadAddress>(instr).dst);
      return;
    default:
      return;
  }
}

}  // namespace

ReachSets reach_sets(const tac_ast::Program& prog) {
  OrderedMap<std::string, NameSet> direct;
  for (const auto& tl : prog.top_level) {
    if (const auto* fn = std::get_if<tac_ast::Function>(&tl)) {
      NameSet callees;
      for (const auto* call : graph_calls(*fn)) {
        callees.insert(call->name);
      }
      direct.insert_or_assign(fn->name, std::move(callees));
    }
  }
  // The closure, by a walk from each function. A set's order doesn't
  // reach the result, which is a set too.
  ReachSets out;
  for (const auto& [name, callees] : direct) {
    NameSet seen;
    std::vector<std::string> stack(callees.begin(), callees.end());
    while (!stack.empty()) {
      std::string c = std::move(stack.back());
      stack.pop_back();
      if (!seen.insert(c).second) {
        continue;
      }
      if (const NameSet* next = direct.find(c)) {
        stack.insert(stack.end(), next->begin(), next->end());
      }
    }
    out.insert_or_assign(name, std::move(seen));
  }
  return out;
}

bool coexist(std::string_view a, std::string_view b, const ReachSets& reach) {
  if (a == b) {
    return true;
  }
  const NameSet* from_a = reach.find(a);
  if (from_a != nullptr && from_a->find(b) != from_a->end()) {
    return true;
  }
  const NameSet* from_b = reach.find(b);
  return from_b != nullptr && from_b->find(a) != from_b->end();
}

std::vector<std::size_t> share_conflicts(std::string_view fn, const std::vector<SlotShare>& shares,
                                         const ReachSets& reach) {
  std::vector<std::size_t> out;
  for (std::size_t i = 0; i < shares.size(); ++i) {
    for (const auto& f : shares[i].functions()) {
      if (coexist(fn, f, reach)) {
        out.push_back(i);
        break;
      }
    }
  }
  return out;
}

std::size_t SlotShare::width() const { return members.at(0).second.size(); }

NameSet SlotShare::functions() const {
  NameSet out = owners();
  out.insert(holders.begin(), holders.end());
  return out;
}

NameSet SlotShare::owners() const {
  NameSet out;
  for (const auto& [fn, syms] : members) {
    out.insert(fn);
  }
  return out;
}

NameSet SlotShare::slots() const {
  NameSet out;
  for (const auto& [fn, syms] : members) {
    out.insert(syms.begin(), syms.end());
  }
  return out;
}

void check_shares(const asm_ast::Program& prog, const std::vector<SlotShare>& shares) {
  std::map<std::string, NameSet, std::less<>> own;
  for (const auto& share : shares) {
    for (const auto& [f, syms] : share.members) {
      own[f].insert(syms.begin(), syms.end());
    }
  }
  for (const auto& tl : prog.top_level) {
    const auto* fn = asdl::as<ir::Function>(tl);
    if (fn == nullptr) {
      continue;
    }
    const auto it = own.find(fn->name);
    if (it == own.end()) {
      continue;
    }
    for (const auto& instr : fn->instructions) {
      memory_writes(instr, [&](const ir::Type_operand& op) {
        const auto* d = asdl::as<ir::Data>(op);
        if (d != nullptr && it->second.contains(d->name)) {
          throw InternalError("internal error: `" + fn->name + "` writes `" + d->name +
                              "`, a parameter slot that holds a value its callers keep there "
                              "(passes/slot_sharing.py)");
        }
      });
    }
  }
}


// --- Finding the shares ---

namespace {

namespace tac = ::c6502::tac_ast;
namespace c99 = ::c6502::c99_ast;

// A function's slot symbols, one entry per parameter, low byte first,
// or none for a parameter that can't be in a share.
using ParamSlots = std::vector<std::optional<std::vector<std::string>>>;
using SlotsByFn = std::map<std::string, ParamSlots, std::less<>>;
using FnPair = std::pair<std::string, std::string>;

// One variable kept in a callee's parameter slots (Python's `_Binding`).
struct Binding {
  // The function the variable belongs to.
  std::string fn;
  // The variable.
  std::string var;
  // The callee whose parameter slots it lives in.
  std::string owner;
  // The callee's parameter's position.
  std::size_t index = 0;
  // Whether `var` is a parameter of `fn`, else a local.
  bool alias = false;
  // For a local copied to or from a bound local, and never live at the
  // same time, the bound local whose home it shares.
  std::optional<std::string> primary;
};

// Returns `xs[lo:hi]`, as Python slices a list, clamped to its length.
template <class T>
std::vector<T> slice(const std::vector<T>& xs, asdl::Int lo, asdl::Int hi) {
  const auto n = static_cast<asdl::Int>(xs.size());
  lo = std::min(std::max<asdl::Int>(lo, 0), n);
  hi = std::min(std::max(hi, lo), n);
  return {xs.begin() + static_cast<std::ptrdiff_t>(lo), xs.begin() + static_cast<std::ptrdiff_t>(hi)};
}

const ZpLayout* zp_layout(const Abi& abi, std::string_view name) {
  const ParamLayout* layout = abi.find(name);
  return layout == nullptr ? nullptr : std::get_if<ZpLayout>(layout);
}

bool is_volatile(const c99::Type_data_type& type) {
  const c99::Type_data_type* t = &type;
  while (true) {
    if (asdl::isa<c99::Volatile>(*t)) {
      return true;
    }
    const auto* c = asdl::as<c99::Const>(*t);
    if (c == nullptr) {
      return false;
    }
    t = &*c->referenced_type;
  }
}

bool is_scalar_type(const c99::Type_data_type& type) {
  const c99::Type_data_type* t = &type;
  while (true) {
    if (const auto* c = asdl::as<c99::Const>(*t)) {
      t = &*c->referenced_type;
    } else if (const auto* v = asdl::as<c99::Volatile>(*t)) {
      t = &*v->referenced_type;
    } else {
      break;
    }
  }
  return asdl::isa<c99::Pointer, c99::Char, c99::SChar, c99::UChar, c99::Int, c99::UInt, c99::Long,
                   c99::ULong>(*t);
}

// Returns the names whose address `fn` takes or that it indexes by name,
// which live in memory rather than in a value's home.
NameSet in_memory(const tac::Function& fn) {
  NameSet out;
  for (const auto& instr : fn.instructions) {
    switch (tac::kind(instr)) {
      case tac::Kind_instruction::GetAddress:
        if (const auto* v = std::get_if<tac::Var>(&std::get<tac::GetAddress>(instr).operand)) {
          out.insert(v->name);
        }
        break;
      case tac::Kind_instruction::IndexedLoad:
        out.insert(std::get<tac::IndexedLoad>(instr).name);
        break;
      case tac::Kind_instruction::IndexedSymbolStore:
        out.insert(std::get<tac::IndexedSymbolStore>(instr).name);
        break;
      default:
        break;
    }
  }
  return out;
}

// Returns the names `fn` assigns, takes the address of, or indexes by
// name.
NameSet changed_names(const tac::Function& fn) {
  NameSet out = in_memory(fn);
  for (const auto& instr : fn.instructions) {
    for (const auto d : recursion_saves::defs_in(instr)) {
      out.emplace(d);
    }
  }
  return out;
}

bool plain_local(std::string_view v, const SymbolTable& symbols, const NameSet& memory) {
  const Symbol* sym = symbols.find(v);
  if (sym == nullptr) {
    return false;
  }
  const auto* attrs = std::get_if<LocalAttr>(&sym->attrs);
  if (attrs == nullptr) {
    return false;
  }
  if (attrs->register_ || memory.contains(v) || is_volatile(sym->type)) {
    return false;
  }
  return is_scalar_type(sym->type);
}

// Returns `name`'s slot symbols, one entry per parameter, or none for a
// parameter that can't be in a share: one passed in a register or at a
// fixed address, one that isn't an integer or a pointer, one the
// function assigns or takes the address of, or, for a function defined
// in another unit, one its declaration doesn't make `const`.
ParamSlots param_slots(const std::string& name, const Abi& abi, const SymbolTable& symbols,
                       const TypeTable* types,
                       const OrderedMap<std::string, const tac::Function*>& fns,
                       const std::map<std::string, NameSet, std::less<>>& changed) {
  const Symbol* sym = symbols.find(name);
  const auto* ftype = sym == nullptr ? nullptr : asdl::as<c99::FunType>(sym->type);
  const tac::Function* const* defined = fns.find(name);
  if (ftype == nullptr) {
    return ParamSlots(defined == nullptr ? 0 : (*defined)->params.size());
  }
  const ZpLayout* layout = zp_layout(abi, name);
  if (layout == nullptr) {
    return ParamSlots(ftype->params.size());
  }
  ParamSlots out;
  asdl::Int flat = 0;
  const std::size_t n_slots = layout->slot_symbols.size();
  std::vector<std::optional<asdl::Int>> pinned = layout->pinned;
  if (pinned.empty()) {
    pinned.resize(n_slots);
  }
  std::vector<std::optional<std::string>> regs = layout->param_registers;
  if (regs.empty()) {
    regs.resize(n_slots);
  }
  for (std::size_t j = 0; j < ftype->params.size(); ++j) {
    const c99::Type_data_type& t = ftype->params[j];
    const asdl::Int n = sizeof_type(t, types);
    std::vector<std::string> syms = slice(layout->slot_symbols, flat, flat + n);
    const auto reg_slice = slice(regs, flat, flat + n);
    const auto pin_slice = slice(pinned, flat, flat + n);
    bool ok = static_cast<asdl::Int>(syms.size()) == n &&
              std::none_of(reg_slice.begin(), reg_slice.end(),
                           [](const auto& r) { return r.has_value(); }) &&
              std::none_of(pin_slice.begin(), pin_slice.end(),
                           [](const auto& a) { return a.has_value(); }) &&
              is_scalar_type(t);
    flat += n;
    if (ok && defined != nullptr) {
      const auto& params = (*defined)->params;
      ok = j < params.size() && !changed.at(name).contains(params[j]);
    } else if (ok) {
      ok = asdl::isa<c99::Const>(t) && !is_volatile(t);
    }
    out.push_back(ok ? std::optional<std::vector<std::string>>(std::move(syms)) : std::nullopt);
  }
  return out;
}

// Returns the variable `instr` adds a constant to or subtracts one from,
// when it's `t = s + k`, `t = k + s`, or `t = s - k` with an integer
// constant `k`.
const std::string* step_source(const tac::Type_instruction& instr) {
  const auto* bin = asdl::as<tac::Binary>(instr);
  if (bin == nullptr) {
    return nullptr;
  }
  const tac::Type_val* a = &bin->src1;
  const tac::Type_val* b = &bin->src2;
  if (asdl::isa<tac::Add>(bin->op)) {
    if (asdl::isa<tac::Constant>(*a)) {
      std::swap(a, b);
    }
  } else if (!asdl::isa<tac::Subtract>(bin->op)) {
    return nullptr;
  }
  const auto* var = std::get_if<tac::Var>(a);
  if (var != nullptr && asdl::isa<tac::Constant>(*b)) {
    return &var->name;
  }
  return nullptr;
}

// Returns the name of the `Var` `v`, or null for a constant.
const std::string* var_name(const tac::Type_val& v) {
  const auto* var = std::get_if<tac::Var>(&v);
  return var == nullptr ? nullptr : &var->name;
}

const std::string* var_name(const std::optional<tac::Type_val>& v) {
  return v ? var_name(*v) : nullptr;
}

bool intersects(const NameSet& a, const NameSet& b) {
  const NameSet& small = a.size() <= b.size() ? a : b;
  const NameSet& large = a.size() <= b.size() ? b : a;
  return std::any_of(small.begin(), small.end(),
                     [&](const std::string& x) { return large.contains(x); });
}

// Returns `var` and the locals of `fn` linked to it by copies (`a = b`)
// and by steps (`a = b + k`, `a = b - k`) that can share its home: of
// its size, plain (see `plain_local`), not bound elsewhere, and never
// live where another of them is defined, other than by a copy between
// them or by a step after which the stepped variable is dead. A sector
// number stepped before each call that takes it, `write(++sector)`, is a
// chain of such steps, and in one home each step is an add in place.
NameSet copy_group(const tac::Function& fn, const std::string& var,
                   const std::vector<NameSet>& live, const SymbolTable& symbols,
                   const TypeTable* types, const std::set<FnPair>& bound) {
  const NameSet memory = in_memory(fn);
  const asdl::Int size = size_of_name(var, &symbols, types);
  std::vector<std::pair<std::string, std::string>> copies;
  for (const auto& instr : fn.instructions) {
    if (const auto* c = asdl::as<tac::Copy>(instr)) {
      const std::string* src = var_name(c->src);
      const std::string* dst = var_name(c->dst);
      if (src != nullptr && dst != nullptr) {
        copies.emplace_back(*src, *dst);
      }
    }
  }
  for (const auto& instr : fn.instructions) {
    if (const std::string* src = step_source(instr)) {
      // A step is a `Binary`, whose destination is a variable.
      copies.emplace_back(*src, *var_name(std::get<tac::Binary>(instr).dst));
    }
  }

  const auto interferes = [&](const std::string& t, const NameSet& group) {
    NameSet names = group;
    names.insert(t);
    for (std::size_t k = 0; k < fn.instructions.size(); ++k) {
      const auto& instr = fn.instructions[k];
      if (const auto* c = asdl::as<tac::Copy>(instr)) {
        const std::string* src = var_name(c->src);
        const std::string* dst = var_name(c->dst);
        if (src != nullptr && names.contains(*src) && dst != nullptr && names.contains(*dst)) {
          continue;
        }
      }
      if (const std::string* src = step_source(instr)) {
        const std::string* dst = var_name(std::get<tac::Binary>(instr).dst);
        if (names.contains(*src) && dst != nullptr && names.contains(*dst) &&
            !live[k].contains(*src)) {
          continue;
        }
      }
      NameSet defined;
      for (const auto d : recursion_saves::defs_in(instr)) {
        defined.emplace(d);
      }
      if (defined.contains(t) && intersects(live[k], group)) {
        return true;
      }
      if (intersects(defined, group) && live[k].contains(t)) {
        return true;
      }
    }
    return false;
  };

  NameSet group{var};
  bool changed = true;
  while (changed) {
    changed = false;
    for (const auto& [a, c] : copies) {
      for (const auto& [t, m] : {std::pair<const std::string&, const std::string&>(c, a),
                                 std::pair<const std::string&, const std::string&>(a, c)}) {
        if (group.contains(m) && !group.contains(t) && !bound.contains(FnPair(fn.name, t)) &&
            std::find(fn.params.begin(), fn.params.end(), t) == fn.params.end() &&
            plain_local(t, symbols, memory) && size_of_name(t, &symbols, types) == size &&
            !interferes(t, group)) {
          group.insert(t);
          changed = true;
        }
      }
    }
  }
  return group;
}

// Returns the position of `syms` among `slots`, as `list.index` does.
std::size_t slot_index(const ParamSlots& slots, const std::vector<std::string>& syms) {
  for (std::size_t i = 0; i < slots.size(); ++i) {
    if (slots[i] && *slots[i] == syms) {
      return i;
    }
  }
  throw InternalError("ValueError: a share's slots aren't its owner's");
}

std::size_t param_index(const std::vector<std::string>& params, const std::string& name) {
  const auto it = std::find(params.begin(), params.end(), name);
  if (it == params.end()) {
    throw InternalError("ValueError: a bound parameter isn't its function's");
  }
  return static_cast<std::size_t>(it - params.begin());
}

// Returns each member's owner mapped to the positions of its parameters
// in `share`.
std::map<std::string, std::vector<std::size_t>, std::less<>> member_indexes(
    const SlotShare& share, const SlotsByFn& slots) {
  std::map<std::string, std::vector<std::size_t>, std::less<>> out;
  for (const auto& [f, syms] : share.members) {
    out[f].push_back(slot_index(slots.at(f), syms));
  }
  return out;
}

using SlotKey = std::pair<std::string, std::size_t>;

// Groups the bindings' slots into shares. Returns the shares and each
// binding's share index, or none for a binding that would put a
// function in its share twice and is left out.
std::pair<std::vector<SlotShare>, std::vector<std::optional<std::size_t>>> classes(
    const std::vector<Binding>& cands, const SlotsByFn& slots,
    const std::map<std::string, std::vector<std::string>, std::less<>>& params) {
  std::map<SlotKey, SlotKey> parent;
  const auto find = [&](SlotKey k) {
    while (true) {
      const auto it = parent.find(k);
      if (it == parent.end() || it->second == k) {
        return k;
      }
      k = it->second;
    }
  };
  std::map<SlotKey, std::set<SlotKey>> members;
  std::map<SlotKey, NameSet> holders;
  const auto fns_of = [&](const SlotKey& root) {
    std::vector<std::string> out;
    if (const auto it = members.find(root); it != members.end()) {
      for (const auto& [f, k] : it->second) {
        out.push_back(f);
      }
    } else {
      out.push_back(root.first);
    }
    if (const auto it = holders.find(root); it != holders.end()) {
      out.insert(out.end(), it->second.begin(), it->second.end());
    }
    return out;
  };
  const auto has_duplicates = [](std::vector<std::string> xs) {
    std::sort(xs.begin(), xs.end());
    return std::adjacent_find(xs.begin(), xs.end()) != xs.end();
  };

  // Members first, so that a holder of one member's bytes doesn't keep
  // two members from joining: a function's parameter passed on
  // unchanged joins its callee's bytes whatever its callers keep there.
  std::vector<std::size_t> order;
  for (std::size_t i = 0; i < cands.size(); ++i) {
    if (cands[i].alias) {
      order.push_back(i);
    }
  }
  for (std::size_t i = 0; i < cands.size(); ++i) {
    if (!cands[i].alias) {
      order.push_back(i);
    }
  }
  std::vector<std::size_t> kept;
  for (const std::size_t i : order) {
    const Binding& b = cands[i];
    if (b.primary) {
      continue;
    }
    const SlotKey target = find(SlotKey(b.owner, b.index));
    if (b.alias) {
      const SlotKey src = find(SlotKey(b.fn, param_index(params.at(b.fn), b.var)));
      if (src == target) {
        continue;
      }
      std::vector<std::string> both = fns_of(src);
      const std::vector<std::string> theirs = fns_of(target);
      both.insert(both.end(), theirs.begin(), theirs.end());
      if (has_duplicates(both)) {
        continue;
      }
      parent[src] = target;
      std::set<SlotKey> joined;
      if (const auto it = members.find(target); it != members.end()) {
        joined = it->second;
      } else {
        joined = {target};
      }
      if (const auto it = members.find(src); it != members.end()) {
        joined.insert(it->second.begin(), it->second.end());
        members.erase(it);
      } else {
        joined.insert(src);
      }
      members[target] = std::move(joined);
      NameSet hs;
      if (const auto it = holders.find(target); it != holders.end()) {
        hs = it->second;
      }
      if (const auto it = holders.find(src); it != holders.end()) {
        hs.insert(it->second.begin(), it->second.end());
        holders.erase(it);
      }
      holders[target] = std::move(hs);
    } else {
      const std::vector<std::string> fs = fns_of(target);
      if (std::find(fs.begin(), fs.end(), b.fn) != fs.end()) {
        continue;
      }
      members.try_emplace(target, std::set<SlotKey>{target});
      holders[target].insert(b.fn);
    }
    kept.push_back(i);
  }
  std::set<FnPair> primaries;
  for (const std::size_t i : kept) {
    primaries.emplace(cands[i].fn, cands[i].var);
  }
  for (std::size_t i = 0; i < cands.size(); ++i) {
    if (cands[i].primary && primaries.contains(FnPair(cands[i].fn, *cands[i].primary))) {
      kept.push_back(i);
    }
  }
  std::set<SlotKey> roots;
  for (const std::size_t i : kept) {
    roots.insert(find(SlotKey(cands[i].owner, cands[i].index)));
  }
  std::vector<SlotShare> shares;
  std::map<SlotKey, std::size_t> index;
  for (const SlotKey& r : roots) {
    index[r] = shares.size();
    SlotShare share;
    std::set<SlotKey> ms;
    if (const auto it = members.find(r); it != members.end()) {
      ms = it->second;
    } else {
      ms = {r};
    }
    for (const auto& [f, k] : ms) {
      share.members.emplace_back(f, slots.at(f).at(k).value());
    }
    std::sort(share.members.begin(), share.members.end());
    if (const auto it = holders.find(r); it != holders.end()) {
      share.holders.assign(it->second.begin(), it->second.end());
    }
    shares.push_back(std::move(share));
  }
  std::vector<std::optional<std::size_t>> by_binding(cands.size());
  for (const std::size_t i : kept) {
    by_binding[i] = index.at(find(SlotKey(cands[i].owner, cands[i].index)));
  }
  return {std::move(shares), std::move(by_binding)};
}

// Returns, for each share, the functions here that write its bytes (see
// the comment at the top of the file).
std::vector<NameSet> writers_of(const OrderedMap<std::string, const tac::Function*>& fns,
                                const std::vector<Binding>& cands,
                                const std::vector<std::size_t>& by_binding,
                                const std::vector<SlotShare>& shares, const SlotsByFn& slots) {
  std::map<FnPair, std::size_t> self_bound;
  for (std::size_t i = 0; i < cands.size(); ++i) {
    self_bound.insert_or_assign(FnPair(cands[i].fn, cands[i].var), by_binding[i]);
  }
  std::map<std::string, std::vector<std::pair<std::size_t, std::size_t>>, std::less<>> owned;
  for (std::size_t k = 0; k < shares.size(); ++k) {
    for (const auto& [f, syms] : shares[k].members) {
      owned[f].emplace_back(slot_index(slots.at(f), syms), k);
    }
  }
  std::vector<NameSet> out(shares.size());
  for (const auto& [name, fn] : fns) {
    for (const auto& instr : fn->instructions) {
      const auto* call = asdl::as<tac::FunctionCall>(instr);
      if (call == nullptr) {
        continue;
      }
      const auto it = owned.find(call->name);
      if (it == owned.end()) {
        continue;
      }
      for (const auto& [j, k] : it->second) {
        const std::string* arg = j < call->args.size() ? var_name(call->args[j]) : nullptr;
        bool mine = false;
        if (arg != nullptr) {
          const auto bound = self_bound.find(FnPair(name, *arg));
          mine = bound != self_bound.end() && bound->second == k;
        }
        if (!mine) {
          out[k].insert(name);
        }
      }
    }
  }
  for (std::size_t i = 0; i < cands.size(); ++i) {
    const Binding& b = cands[i];
    if (b.alias) {
      continue;
    }
    const tac::Function& fn = *fns.at(b.fn);
    const bool assigns = std::any_of(
        fn.instructions.begin(), fn.instructions.end(), [&](const tac::Type_instruction& instr) {
          const auto defs = recursion_saves::defs_in(instr);
          return std::find(defs.begin(), defs.end(), b.var) != defs.end();
        });
    if (assigns) {
      out[by_binding[i]].insert(b.fn);
    }
  }
  return out;
}

// Returns whether call `call` of a function stores its result into
// `var`, which then isn't live across the call: the callee has returned
// before the value is stored.
bool defines(const tac::FunctionCall& call, const std::string& var) {
  const std::string* dst = var_name(call.dst);
  return dst != nullptr && *dst == var;
}

// Returns `share` with the records the linker checks it by (see "Across
// units"). `mine` are the indices of its bindings in `cands`.
SlotShare with_link_records(SlotShare share, const std::vector<Binding>& cands,
                            const std::vector<std::size_t>& mine,
                            const OrderedMap<std::string, const tac::Function*>& fns,
                            const std::map<std::string, std::vector<NameSet>, std::less<>>& live,
                            const SlotsByFn& slots, const Bindings& bindings,
                            const NameSet& writers) {
  std::map<std::string, NameSet, std::less<>> across;
  for (const std::size_t i : mine) {
    const Binding& b = cands[i];
    const tac::Function& fn = *fns.at(b.fn);
    const std::vector<NameSet>& lv = live.at(b.fn);
    for (std::size_t k = 0; k < fn.instructions.size(); ++k) {
      const auto* call = asdl::as<tac::FunctionCall>(fn.instructions[k]);
      if (call != nullptr && lv[k].contains(b.var) && !defines(*call, b.var)) {
        across[b.fn].insert(call->name);
      }
    }
  }
  const auto indexes = member_indexes(share, slots);
  const NameSet share_slots = share.slots();
  std::set<FnPair> forwards;
  for (const auto& [name, fn] : fns) {
    NameSet own;
    if (const auto* vs = bindings.find(name)) {
      for (const auto& [v, s] : *vs) {
        if (std::all_of(s.begin(), s.end(),
                        [&](const std::string& x) { return share_slots.contains(x); })) {
          own.insert(v);
        }
      }
    }
    for (const auto& [g, js] : indexes) {
      bool any_call = false;
      bool all_own = true;
      for (const auto& instr : fn->instructions) {
        const auto* call = asdl::as<tac::FunctionCall>(instr);
        if (call == nullptr || call->name != g) {
          continue;
        }
        any_call = true;
        for (const std::size_t j : js) {
          if (j >= call->args.size()) {
            continue;
          }
          const std::string* arg = var_name(call->args[j]);
          if (arg == nullptr || !own.contains(*arg)) {
            all_own = false;
          }
        }
      }
      if (any_call && all_own) {
        forwards.emplace(name, g);
      }
    }
  }
  share.live.clear();
  for (const auto& [f, cs] : across) {
    share.live.emplace_back(f, std::vector<std::string>(cs.begin(), cs.end()));
  }
  share.forwards.assign(forwards.begin(), forwards.end());
  share.writers.clear();
  for (const auto& w : writers) {
    if (std::find(share.holders.begin(), share.holders.end(), w) != share.holders.end()) {
      share.writers.push_back(w);
    }
  }
  return share;
}

}  // namespace

std::pair<std::vector<SlotShare>, Bindings> find_shares(const tac::Program& prog, const Abi& abi,
                                                        const SymbolTable& symbols,
                                                        const TypeTable* types,
                                                        const NameSet& eligible,
                                                        const NameSet& fixed,
                                                        const NameSet& recursion) {
  OrderedMap<std::string, const tac::Function*> fns;
  for (const auto& tl : prog.top_level) {
    if (const auto* fn = std::get_if<tac::Function>(&tl)) {
      fns.insert_or_assign(fn->name, fn);
    }
  }
  const ReachSets reach = reach_sets(prog);
  NameSet ok;
  for (const auto& [n, fn] : fns) {
    if (eligible.contains(n) && !recursion.contains(n)) {
      ok.insert(n);
    }
  }
  std::map<std::string, NameSet, std::less<>> changed;
  for (const auto& [n, fn] : fns) {
    changed[n] = changed_names(*fn);
  }
  SlotsByFn slots;
  for (const auto& [n, fn] : fns) {
    slots[n] = param_slots(n, abi, symbols, types, fns, changed);
  }
  for (const auto& [n, layout] : abi) {
    if (std::holds_alternative<ZpLayout>(layout) && !slots.contains(n)) {
      slots[n] = param_slots(n, abi, symbols, types, fns, changed);
    }
  }
  std::map<std::string, std::vector<NameSet>, std::less<>> live;
  for (const auto& n : ok) {
    live[n] = recursion_saves::live_out(*fns.at(n));
  }

  const auto owner_ok = [&](const std::string& g) {
    if (fixed.contains(g)) {
      return false;
    }
    if (fns.contains(g)) {
      return ok.contains(g);
    }
    return zp_layout(abi, g) != nullptr;
  };

  std::vector<Binding> cands;
  std::set<FnPair> bound;
  // A parameter passed on to several callees joins each one's bytes, so
  // `is_separator(c)`, which passes `c` to two functions, copies it for
  // neither. A local has one home, the first callee's.
  std::set<std::tuple<std::string, std::string, std::string, std::size_t>> passed_on;
  for (const auto& fname : ok) {
    const tac::Function& fn = *fns.at(fname);
    const NameSet memory = in_memory(fn);
    for (std::size_t i = 0; i < fn.instructions.size(); ++i) {
      const auto* call = asdl::as<tac::FunctionCall>(fn.instructions[i]);
      if (call == nullptr) {
        continue;
      }
      const std::string& g = call->name;
      if (g == fname || !owner_ok(g)) {
        continue;
      }
      const ParamSlots& gslots = slots.at(g);
      for (std::size_t j = 0; j < call->args.size(); ++j) {
        const std::string* arg = var_name(call->args[j]);
        if (arg == nullptr) {
          continue;
        }
        const std::string& v = *arg;
        if (j >= gslots.size()) {
          continue;
        }
        const auto p = std::find(fn.params.begin(), fn.params.end(), v);
        const bool is_param = p != fn.params.end();
        if (is_param) {
          if (passed_on.contains({fname, v, g, j})) {
            continue;
          }
        } else if (bound.contains(FnPair(fname, v))) {
          continue;
        }
        const auto& target = gslots[j];
        if (!target) {
          continue;
        }
        bool alias = false;
        if (is_param) {
          const auto k = static_cast<std::size_t>(p - fn.params.begin());
          const ParamSlots& mine = slots.at(fname);
          if (k >= mine.size() || !mine[k] || mine[k]->size() != target->size()) {
            continue;
          }
          alias = true;
        } else {
          if (!plain_local(v, symbols, memory)) {
            continue;
          }
          if (size_of_name(v, &symbols, types) != static_cast<asdl::Int>(target->size())) {
            continue;
          }
          if (!live.at(fname)[i].contains(v)) {
            continue;
          }
        }
        cands.push_back(Binding{fname, v, g, j, alias, std::nullopt});
        bound.emplace(fname, v);
        if (alias) {
          passed_on.insert({fname, v, g, j});
        }
      }
    }
  }
  // Each local candidate's copy group shares its home. A later candidate
  // of the same function in that group joins it rather than taking a
  // home of its own, as each value of `write(++sector)` does.
  std::vector<Binding> locals;
  for (const auto& b : cands) {
    if (!b.alias) {
      locals.push_back(b);
    }
  }
  std::set<FnPair> grouped;
  for (const auto& b : locals) {
    if (!grouped.insert(FnPair(b.fn, b.var)).second) {
      continue;
    }
    std::set<FnPair> joinable;
    for (const auto& c : locals) {
      if (c.fn == b.fn && !grouped.contains(FnPair(c.fn, c.var))) {
        joinable.emplace(c.fn, c.var);
      }
    }
    std::set<FnPair> others;
    for (const auto& x : bound) {
      if (!joinable.contains(x)) {
        others.insert(x);
      }
    }
    NameSet group = copy_group(*fns.at(b.fn), b.var, live.at(b.fn), symbols, types, others);
    group.erase(b.var);
    for (const auto& t : group) {
      std::erase_if(cands, [&](const Binding& c) { return !c.alias && c.fn == b.fn && c.var == t; });
      cands.push_back(Binding{b.fn, t, b.owner, b.index, false, b.var});
      bound.emplace(b.fn, t);
      grouped.emplace(b.fn, t);
    }
  }

  const auto group_of = [&](const Binding& b) {
    const std::string& primary = b.primary ? *b.primary : b.var;
    NameSet out;
    for (const auto& c : cands) {
      if (c.fn == b.fn && (c.primary ? *c.primary : c.var) == primary) {
        out.insert(c.var);
      }
    }
    return out;
  };

  std::map<std::string, std::vector<std::string>, std::less<>> params;
  for (const auto& [n, fn] : fns) {
    params[n] = fn->params;
  }
  std::vector<SlotShare> shares;
  std::vector<std::size_t> by_binding;
  std::vector<NameSet> writers;
  while (true) {
    auto [found, index] = classes(cands, slots, params);
    shares = std::move(found);
    std::vector<Binding> kept;
    by_binding.clear();
    for (std::size_t i = 0; i < cands.size(); ++i) {
      if (index[i]) {
        kept.push_back(std::move(cands[i]));
        by_binding.push_back(*index[i]);
      }
    }
    cands = std::move(kept);
    writers = writers_of(fns, cands, by_binding, shares, slots);
    std::set<std::size_t> bad;
    for (std::size_t bi = 0; bi < cands.size(); ++bi) {
      const Binding& b = cands[bi];
      const std::size_t k = by_binding[bi];
      const NameSet group = group_of(b);
      const SlotShare& share = shares[k];
      const NameSet owners = share.owners();
      NameSet extern_owners;
      bool global_owner = false;
      for (const auto& f : owners) {
        const tac::Function* const* def = fns.find(f);
        if (def == nullptr) {
          extern_owners.insert(f);
          global_owner = true;
        } else if ((*def)->is_global) {
          global_owner = true;
        }
      }
      const auto indexes = member_indexes(share, slots);
      const tac::Function& fn = *fns.at(b.fn);
      const std::vector<NameSet>& lv = live.at(b.fn);
      for (std::size_t i = 0; i < fn.instructions.size(); ++i) {
        const auto* call = asdl::as<tac::FunctionCall>(fn.instructions[i]);
        if (call == nullptr) {
          continue;
        }
        // A value the call returns isn't live across it: the callee has
        // returned before the value is stored.
        const bool v_live = lv[i].contains(b.var) && !defines(*call, b.var);
        const bool passed = std::any_of(call->args.begin(), call->args.end(),
                                        [&](const tac::Type_val& a) {
                                          const std::string* n = var_name(a);
                                          return n != nullptr && *n == b.var;
                                        });
        if (const auto it = indexes.find(call->name); it != indexes.end()) {
          for (const std::size_t j : it->second) {
            const std::string* arg = j < call->args.size() ? var_name(call->args[j]) : nullptr;
            const bool mine = arg != nullptr && group.contains(*arg);
            if (!mine && (v_live || passed)) {
              bad.insert(bi);
            }
          }
        }
        if (!v_live) {
          continue;
        }
        NameSet called{call->name};
        if (const NameSet* r = reach.find(call->name)) {
          called.insert(r->begin(), r->end());
        }
        for (const auto& c : called) {
          if (c != b.fn && writers[k].contains(c)) {
            bad.insert(bi);
            break;
          }
        }
        if (global_owner) {
          for (const auto& c : called) {
            if (!fns.contains(c) && !fixed.contains(c) && !extern_owners.contains(c)) {
              bad.insert(bi);
              break;
            }
          }
        }
      }
    }
    if (bad.empty()) {
      break;
    }
    // A failing parameter of a function with external linkage is what
    // makes its share's owners global (rule 3), and so may be what fails
    // the share's other bindings. Those go first, and the rest are
    // checked again without them. `store_result(v)` passing `v` on to
    // `push` fails rule 3, and while it's in the share `var_get`, holding
    // the value it pops in `push`'s bytes across `push(value)`, failed
    // with it.
    std::set<std::size_t> global_aliases;
    for (const std::size_t bi : bad) {
      if (cands[bi].alias && fns.at(cands[bi].fn)->is_global) {
        global_aliases.insert(bi);
      }
    }
    if (!global_aliases.empty() && global_aliases != bad) {
      bad = std::move(global_aliases);
    }
    // A binding that goes takes the locals sharing its home with it.
    std::set<FnPair> gone;
    for (const std::size_t bi : bad) {
      gone.emplace(cands[bi].fn, cands[bi].var);
    }
    std::vector<Binding> rest;
    for (std::size_t bi = 0; bi < cands.size(); ++bi) {
      const Binding& b = cands[bi];
      if (bad.contains(bi) || (b.primary && gone.contains(FnPair(b.fn, *b.primary)))) {
        continue;
      }
      rest.push_back(b);
    }
    cands = std::move(rest);
  }

  Bindings bindings;
  for (const auto& b : cands) {
    bindings[b.fn].insert_or_assign(b.var, slots.at(b.owner).at(b.index).value());
  }
  std::vector<SlotShare> out;
  for (std::size_t k = 0; k < shares.size(); ++k) {
    std::vector<std::size_t> mine;
    for (std::size_t bi = 0; bi < cands.size(); ++bi) {
      if (by_binding[bi] == k) {
        mine.push_back(bi);
      }
    }
    if (mine.empty()) {
      continue;
    }
    out.push_back(with_link_records(shares[k], cands, mine, fns, live, slots, bindings,
                                    writers[k]));
  }
  return {std::move(out), std::move(bindings)};
}

OrderedMap<std::string, std::string> slot_aliases(const std::vector<SlotShare>& shares) {
  OrderedMap<std::string, std::string> out;
  for (const auto& share : shares) {
    const auto& canon = share.members.at(0).second;
    for (const auto& [f, syms] : share.members) {
      for (std::size_t k = 0; k < syms.size(); ++k) {
        out.insert_or_assign(syms[k], canon.at(k));
      }
    }
  }
  return out;
}

OrderedMap<std::string, std::vector<std::string>> changed_param_slots(const tac::Program& prog,
                                                                      const Abi& abi,
                                                                      const SymbolTable& symbols,
                                                                      const TypeTable* types) {
  OrderedMap<std::string, std::vector<std::string>> out;
  for (const auto& tl : prog.top_level) {
    const auto* fn = std::get_if<tac::Function>(&tl);
    if (fn == nullptr) {
      continue;
    }
    const ZpLayout* layout = zp_layout(abi, fn->name);
    if (layout == nullptr) {
      continue;
    }
    const NameSet changed = changed_names(*fn);
    asdl::Int flat = 0;
    std::vector<std::string> syms;
    for (const auto& p : fn->params) {
      const asdl::Int n = size_of_name(p, &symbols, types);
      if (changed.contains(p)) {
        const auto part = slice(layout->slot_symbols, flat, flat + n);
        syms.insert(syms.end(), part.begin(), part.end());
      }
      flat += n;
    }
    if (!syms.empty()) {
      out.insert_or_assign(fn->name, std::move(syms));
    }
  }
  return out;
}

OrderedMap<std::string, NameSet> pinned_param_copies(const tac::Program& prog, const Abi& abi,
                                                     const SymbolTable& symbols,
                                                     const TypeTable* types,
                                                     const NameSet& addressed) {
  const ReachSets reach = reach_sets(prog);
  std::map<std::string, std::set<asdl::Int>, std::less<>> bytes_of;
  for (const auto& [name, layout] : abi) {
    if (const auto* zp = std::get_if<ZpLayout>(&layout)) {
      auto& bytes = bytes_of[name];
      for (const auto& a : zp->pinned) {
        if (a) {
          bytes.insert(*a);
        }
      }
    }
  }
  OrderedMap<std::string, NameSet> out;
  for (const auto& tl : prog.top_level) {
    const auto* fn = std::get_if<tac::Function>(&tl);
    if (fn == nullptr) {
      continue;
    }
    const ZpLayout* layout = zp_layout(abi, fn->name);
    if (layout == nullptr || bytes_of.at(fn->name).empty()) {
      continue;
    }
    NameSet called;
    if (const NameSet* r = reach.find(fn->name)) {
      called = *r;
    }
    called.erase(fn->name);
    const bool unknown = std::any_of(called.begin(), called.end(), [&](const std::string& c) {
      return !reach.contains(c) && !addressed.contains(c);
    });
    asdl::Int flat = 0;
    for (const auto& p : fn->params) {
      const asdl::Int n = size_of_name(p, &symbols, types);
      std::set<asdl::Int> mine;
      for (const auto& a : slice(layout->pinned, flat, flat + n)) {
        if (a) {
          mine.insert(*a);
        }
      }
      flat += n;
      if (mine.empty()) {
        continue;
      }
      bool hit = unknown;
      for (const auto& c : called) {
        if (hit) {
          break;
        }
        if (const auto it = bytes_of.find(c); it != bytes_of.end()) {
          hit = std::any_of(mine.begin(), mine.end(),
                            [&](asdl::Int a) { return it->second.contains(a); });
        }
      }
      if (hit) {
        out[fn->name].insert(p);
      }
    }
  }
  return out;
}

OrderedMap<std::string, NameSet> preferred_symbols(const Bindings& bindings) {
  OrderedMap<std::string, NameSet> out;
  for (const auto& [fn, vs] : bindings) {
    NameSet syms;
    for (const auto& [v, slots] : vs) {
      syms.insert(slots.begin(), slots.end());
    }
    out.insert_or_assign(fn, std::move(syms));
  }
  return out;
}

// --- Linking ---

std::vector<SlotShare> merge_shares(const std::vector<SlotShare>& shares,
                                    const OrderedMap<std::string, std::string>& canonical) {
  std::vector<std::size_t> parent(shares.size());
  for (std::size_t k = 0; k < parent.size(); ++k) {
    parent[k] = k;
  }
  const auto find = [&](std::size_t k) {
    while (parent[k] != k) {
      k = parent[k];
    }
    return k;
  };
  std::vector<SlotShare> renamed = shares;
  for (SlotShare& s : renamed) {
    for (auto& member : s.members) {
      for (std::string& x : member.second) {
        if (const std::string* to = canonical.find(x)) {
          x = *to;
        }
      }
    }
  }
  // Python walks each share's slots in a set's order. That order decides
  // which share becomes the root of a group, and nothing after reads the
  // roots, only which shares each group holds and the order in which the
  // groups first appear.
  std::map<std::string, std::size_t, std::less<>> first;
  for (std::size_t k = 0; k < renamed.size(); ++k) {
    for (const std::string& sym : renamed[k].slots()) {
      if (const auto it = first.find(sym); it != first.end()) {
        parent[find(k)] = find(it->second);
      } else {
        first.emplace(sym, k);
      }
    }
  }
  OrderedMap<std::size_t, std::vector<const SlotShare*>> groups;
  for (std::size_t k = 0; k < renamed.size(); ++k) {
    groups[find(k)].push_back(&renamed[k]);
  }
  std::vector<SlotShare> out;
  for (const auto& [root, group] : groups) {
    // Python sorts sets of strings and of tuples, which compare their
    // strings by code point, the order in which `std::string` compares
    // UTF-8 bytes.
    std::set<std::pair<std::string, std::vector<std::string>>> member_set;
    std::set<std::string> holder_set;
    std::set<std::pair<std::string, std::string>> forward_set;
    std::set<std::string> writer_set;
    OrderedMap<std::string, std::set<std::string>> live;
    for (const SlotShare* s : group) {
      member_set.insert(s->members.begin(), s->members.end());
      holder_set.insert(s->holders.begin(), s->holders.end());
      forward_set.insert(s->forwards.begin(), s->forwards.end());
      writer_set.insert(s->writers.begin(), s->writers.end());
      for (const auto& [f, cs] : s->live) {
        live[f].insert(cs.begin(), cs.end());
      }
    }
    SlotShare merged;
    merged.members.assign(member_set.begin(), member_set.end());
    merged.holders.assign(holder_set.begin(), holder_set.end());
    std::map<std::string, std::size_t> uses;
    for (const auto& member : merged.members) {
      ++uses[member.first];
    }
    for (const std::string& h : merged.holders) {
      ++uses[h];
    }
    for (const auto& [f, n] : uses) {
      if (n > 1) {
        // `uses` is sorted, so `f` is the first of Python's sorted list
        // of the functions that appear twice.
        throw CompileError("ValueError", "`" + f +
                                             "` would keep two values in the parameter bytes `" +
                                             merged.members.at(0).second.at(0) + "` share");
      }
    }
    std::set<std::pair<std::string, std::vector<std::string>>> live_set;
    for (const auto& [f, cs] : live) {
      live_set.emplace(f, std::vector<std::string>(cs.begin(), cs.end()));
    }
    merged.live.assign(live_set.begin(), live_set.end());
    merged.forwards.assign(forward_set.begin(), forward_set.end());
    merged.writers.assign(writer_set.begin(), writer_set.end());
    out.push_back(std::move(merged));
  }
  return out;
}

void check_linked_shares(const std::vector<SlotShare>& shares,
                         const std::vector<std::pair<std::string, std::string>>& calls,
                         const NameSet& defined, const OrderedMap<std::string, NameSet>& changed) {
  std::map<std::string, NameSet, std::less<>> direct;
  for (const std::string& f : defined) {
    direct[f];
  }
  for (const auto& [a, b] : calls) {
    direct[a].insert(b);
  }
  // The closure, by a walk from each function, as Python's `_closure`.
  // The order of the walk doesn't reach the result, which is a set.
  std::map<std::string, NameSet, std::less<>> reach;
  for (const auto& [name, callees] : direct) {
    NameSet seen;
    std::vector<std::string> stack(callees.begin(), callees.end());
    while (!stack.empty()) {
      std::string c = std::move(stack.back());
      stack.pop_back();
      if (!seen.insert(c).second) {
        continue;
      }
      if (const auto it = direct.find(c); it != direct.end()) {
        stack.insert(stack.end(), it->second.begin(), it->second.end());
      }
    }
    reach.emplace(name, std::move(seen));
  }
  for (const SlotShare& share : shares) {
    const std::string what = "the parameter bytes `" + share.members.at(0).second.at(0) + "`";
    for (const auto& [f, syms] : share.members) {
      if (!defined.contains(f)) {
        throw CompileError("ValueError", what + " hold a value across calls to `" + f +
                                             "`, which no unit defines, so nothing checks that "
                                             "it leaves its `const` parameter alone");
      }
      const NameSet* assigned = changed.find(f);
      if (assigned != nullptr &&
          std::any_of(syms.begin(), syms.end(),
                      [&](const std::string& s) { return assigned->contains(s); })) {
        throw CompileError("ValueError", "a unit keeps a value in " + what +
                                             " across a call to `" + f +
                                             "` because its declaration makes the parameter "
                                             "`const`, but `" +
                                             f + "` changes it");
      }
    }
    const NameSet owners = share.owners();
    const std::set<std::pair<std::string, std::string>> forwards(share.forwards.begin(),
                                                                 share.forwards.end());
    NameSet writers(share.writers.begin(), share.writers.end());
    for (const auto& edge : calls) {
      if (owners.contains(edge.second) && !forwards.contains(edge)) {
        writers.insert(edge.first);
      }
    }
    for (const auto& [f, callees] : share.live) {
      for (const std::string& c : callees) {
        NameSet reached{c};
        if (const auto it = reach.find(c); it != reach.end()) {
          reached.insert(it->second.begin(), it->second.end());
        }
        // The first of Python's sorted `({c} | reach[c]) & (writers - {f})`.
        const auto hit = std::find_if(reached.begin(), reached.end(), [&](const std::string& n) {
          return n != f && writers.contains(n);
        });
        if (hit != reached.end()) {
          throw CompileError("ValueError", "`" + f + "` keeps a value in " + what +
                                               " across its call to `" + c +
                                               "`, because a declaration makes the parameter "
                                               "`const`, but `" +
                                               *hit + "`, which `" + c +
                                               "` reaches, writes those bytes; declare the "
                                               "parameter without `const`");
        }
      }
    }
  }
}

}  // namespace c6502::passes
