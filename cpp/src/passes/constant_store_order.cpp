// Groups a run of stores by the value stored, so each value is loaded
// once.
//
// The pattern
//
// Code that sets up several variables stores constants one after
// another, and the order of the C statements decides the order of the
// values loaded into A. Zork's `main` starts
//
//     LDA   #$00
//     STA   PRINTER_CSW
//     LDA   #$C1
//     STA   PRINTER_CSW+1
//     LDA   #$00
//     STA   ZCODE_PAGE_VALID
//     ...
//
// and loads 0 twice. The pass moves the stores of each value next to the
// first load of it,
//
//     LDA   #$00
//     STA   PRINTER_CSW
//     STA   ZCODE_PAGE_VALID
//     ...
//     LDA   #$C1
//     STA   PRINTER_CSW+1
//
// which drops each load after the first, 2 bytes and 2 cycles each.
//
// Values loaded from memory
//
// The same applies to a value loaded from memory that the run doesn't
// change. Lode Runner's `fill_hole` copies a brick's row and column into
// locals and then into `GAME_ROWNUM` and `GAME_COLNUM`, and
// rematerialization (`asm_remat`, `memory_value_propagation`) can turn
// each copy of a local into a second load of the table,
//
//     LDA   BRICK_DIG_ROWS,X
//     STA   __local_handle_timers__2
//     LDA   BRICK_DIG_COLS,X
//     STA   __local_handle_timers__saved_guard_num
//     LDA   BRICK_DIG_ROWS,X
//     STA   GAME_ROWNUM
//     LDA   BRICK_DIG_COLS,X
//     STA   GAME_COLNUM
//
// which the pass writes with one load of each table byte,
//
//     LDA   BRICK_DIG_ROWS,X
//     STA   __local_handle_timers__2
//     STA   GAME_ROWNUM
//     LDA   BRICK_DIG_COLS,X
//     STA   __local_handle_timers__saved_guard_num
//     STA   GAME_COLNUM
//
// Copies within the run
//
// A load of a `Data` byte that an earlier group of the run stored gives
// A the value that group loaded, since nothing else in the run writes
// the byte, so it joins that group. The rematerializations above leave a
// copy of a local in place when the local is read again later, because a
// load of `BRICK_DIG_ROWS,X` takes a byte more than a load of the local
// (`asm_remat._costs_more`), and
//
//     LDA   BRICK_DIG_ROWS,X
//     STA   __local_handle_timers__2
//     LDA   BRICK_DIG_COLS,X
//     STA   __local_handle_timers__saved_guard_num
//     STA   GAME_COLNUM
//     LDA   __local_handle_timers__2
//     STA   GAME_ROWNUM
//
// becomes the same code as above. Such a load reads nothing once the run
// is rewritten, so the condition below on the bytes the loads read
// leaves it out.
//
// Conditions
//
// A run is a straight sequence of loads into A, each followed by one or
// more `STA m`, with nothing else between them. A load is `LDA #k` (an
// immediate or a label's half), `LDA n` of a `Data` byte, or
// `LDA n+k,X` / `LDA n+k,Y` (`IndexedData`), and none is volatile. The
// asm IR's `Mov(#k, m)`, which `asm_emit` writes as `LDA #k; STA m`,
// counts as a load and a store, and the pass writes the run back as
// separate loads and stores. Two loads load the same value when their
// operands are equal, which for an indexed load includes the index
// register. Nothing in the run writes X or Y, so `n,X` names the same
// byte at each load in it.
//
// Every `m` is a `Data` byte that no other store in the run names, and
// no store is volatile. The pass moves stores earlier, past loads, and
// moves the load of the last value later, past stores, so no store in
// the run may write a byte that a load in it reads. A store to `m` can't
// write the byte of a load of `Data` byte `n` unless `m` and `n` are the
// same name and offset. A load of `n+k,X` is taken to stay inside the
// object `n`, which C requires of a subscript (C99 §6.5.6p8), so no
// store in the run may name `n`. The rematerializations that produced
// the second loads are sound only on the same premise. The premise isn't
// applied to an indexed load of a compiler slot (`__zpabi_*`,
// `__local_*`) or a runtime symbol (`HARGS` and the rest), whose bytes
// other names may cover, so a run with such a load is left alone. With
// those conditions met, the order of the stores can't be seen, and each
// load reads the same value wherever it goes.
//
// A run that stores into or loads from the slots of two different
// functions is left alone, since the linker gives two functions that
// can't be on the call stack together the same bytes. The value loaded
// last stays last, so A and N/Z end the run as they did, and the loads
// and stores leave X, Y, C, and V alone. Every byte the run stores holds
// the value it held before at the end of the run. The pass changes a run
// only when it drops a load, so the peephole fixed point it runs in
// still only shrinks the code.
#include "passes/constant_store_order.h"

#include <algorithm>
#include <cstddef>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

const ir::Type_operand kRegA = ir::Reg{.reg = ir::A{}};

constexpr std::string_view kSlotPrefixes[] = {"__zpabi_", "__local_"};

bool is_a(const ir::Type_operand& op) {
  const auto* r = as<ir::Reg>(op);
  return r != nullptr && isa<ir::A>(r->reg);
}

bool is_constant(const ir::Type_operand& op) {
  return isa<ir::Imm, ir::ImmLabelLow, ir::ImmLabelHigh>(op);
}

// Returns whether `instr` is a load into A, not volatile, of a constant,
// a `Data` byte, or an `IndexedData` byte.
bool is_load(const ir::Type_instruction& instr) {
  const auto* m = as<ir::Mov>(instr);
  return m != nullptr && is_a(m->dst) &&
         (is_constant(m->src) || isa<ir::Data, ir::IndexedData>(m->src)) &&
         !m->is_volatile;
}

bool is_store(const ir::Type_instruction& instr) {
  const auto* m = as<ir::Mov>(instr);
  return m != nullptr && is_a(m->src) && isa<ir::Data>(m->dst) && !m->is_volatile;
}

// Returns whether `instr` is a store of a constant to a byte,
// `Mov(Imm, Data)`, which `asm_emit` writes as `LDA #k; STA m`.
bool is_constant_store(const ir::Type_instruction& instr) {
  const auto* m = as<ir::Mov>(instr);
  return m != nullptr && is_constant(m->src) && isa<ir::Data>(m->dst) &&
         !m->is_volatile;
}

bool starts_with_slot_prefix(std::string_view name) {
  for (auto prefix : kSlotPrefixes) {
    if (name.substr(0, prefix.size()) == prefix) {
      return true;
    }
  }
  return false;
}

// Returns the function a `__zpabi_f__x` or `__local_f__x` slot belongs
// to, or nothing for any other name.
std::optional<std::string> slot_owner(std::string_view name) {
  for (auto prefix : kSlotPrefixes) {
    if (name.substr(0, prefix.size()) == prefix) {
      std::string_view rest = name.substr(prefix.size());
      return std::string(rest.substr(0, rest.find("__")));
    }
  }
  return std::nullopt;
}

// A load and the stores after it.
struct Group {
  ir::Type_instruction load;
  std::vector<ir::Type_instruction> stores;
};

const ir::Type_operand& load_src(const Group& g) { return std::get<ir::Mov>(g.load).src; }

const ir::Data& store_dst(const ir::Type_instruction& s) {
  return std::get<ir::Data>(std::get<ir::Mov>(s).dst);
}

// Returns the groups of the run starting at `instrs[i]` and the index
// after it, or no groups and `i` when no run starts there. A
// `Mov(Imm, Data)` counts as its load and its store.
std::pair<std::vector<Group>, std::size_t> run(const std::vector<ir::Type_instruction>& instrs,
                                               std::size_t i) {
  std::vector<Group> groups;
  std::size_t j = i;
  while (j < instrs.size()) {
    const ir::Type_instruction& instr = instrs[j];
    Group g;
    std::size_t k = j + 1;
    if (is_constant_store(instr)) {
      const auto& m = std::get<ir::Mov>(instr);
      g.load = ir::Mov{.src = m.src, .dst = kRegA};
      g.stores.push_back(ir::Mov{.src = kRegA, .dst = m.dst});
    } else if (is_load(instr)) {
      g.load = instr;
    } else {
      break;
    }
    while (k < instrs.size() && is_store(instrs[k])) {
      g.stores.push_back(instrs[k]);
      ++k;
    }
    if (g.stores.empty()) {
      break;
    }
    groups.push_back(std::move(g));
    j = k;
  }
  return {std::move(groups), j};
}

// Returns whether a store to `Data(name, offset)` may write the byte
// that the load operand `src` reads. An `IndexedData` load stays inside
// its object (C99 §6.5.6p8), except one of a compiler slot or a runtime
// symbol, which counts as reaching every byte.
bool store_may_write(const ir::Type_operand& src, const std::string& name,
                     const asdl::Int& offset) {
  if (const auto* d = as<ir::Data>(src)) {
    return d->name == name && d->offset == offset;
  }
  if (const auto* x = as<ir::IndexedData>(src)) {
    return x->name == name || x->name == "SSP" || x->name == "FP" ||
           x->name == "HARGS" || x->name == "DPTR" || starts_with_slot_prefix(x->name);
  }
  return false;
}

// Returns `groups` with each value's stores after its first load and the
// last value last, or nothing when that drops no load or the run breaks
// a condition.
//
// Each group's value is a key: the index of its load's operand in a
// list of the distinct operands, or, for a load of a byte an earlier
// group of the run stored, the key of the value that group loaded.
std::optional<std::vector<Group>> merged(std::vector<Group> groups) {
  std::vector<ir::Type_operand> distinct_srcs;
  std::vector<std::size_t> keys;
  std::vector<bool> copies;
  std::map<std::pair<std::string, asdl::Int>, std::size_t> stored;
  for (const auto& g : groups) {
    const ir::Type_operand& src = load_src(g);
    const auto* d = as<ir::Data>(src);
    std::optional<std::size_t> from_store;
    if (d != nullptr) {
      const auto it = stored.find({d->name, d->offset});
      if (it != stored.end()) {
        from_store = it->second;
      }
    }
    if (from_store) {
      keys.push_back(*from_store);
      copies.push_back(true);
    } else {
      const auto it = std::find(distinct_srcs.begin(), distinct_srcs.end(), src);
      if (it == distinct_srcs.end()) {
        keys.push_back(distinct_srcs.size());
        distinct_srcs.push_back(src);
      } else {
        keys.push_back(static_cast<std::size_t>(it - distinct_srcs.begin()));
      }
      copies.push_back(false);
    }
    for (const auto& s : g.stores) {
      const ir::Data& dst = store_dst(s);
      stored[{dst.name, dst.offset}] = keys.back();
    }
  }
  if (std::set<std::size_t>(keys.begin(), keys.end()).size() == keys.size()) {
    return std::nullopt;
  }
  std::vector<std::pair<std::string, asdl::Int>> targets;
  for (const auto& g : groups) {
    for (const auto& s : g.stores) {
      const ir::Data& dst = store_dst(s);
      targets.emplace_back(dst.name, dst.offset);
    }
  }
  if (std::set<std::pair<std::string, asdl::Int>>(targets.begin(), targets.end()).size() !=
      targets.size()) {
    return std::nullopt;
  }
  std::set<std::string> owners;
  for (const auto& t : targets) {
    if (auto owner = slot_owner(t.first)) {
      owners.insert(*owner);
    }
  }
  for (const auto& g : groups) {
    if (const auto* d = as<ir::Data>(load_src(g))) {
      if (auto owner = slot_owner(d->name)) {
        owners.insert(*owner);
      }
    }
  }
  if (owners.size() > 1) {
    return std::nullopt;
  }
  // A load of a byte the run stored earlier goes with the load whose
  // value was stored there, so it reads nothing once the run is
  // rewritten, and only the other loads' bytes must stay unwritten.
  for (std::size_t g = 0; g < groups.size(); ++g) {
    if (copies[g]) {
      continue;
    }
    for (const auto& t : targets) {
      if (store_may_write(load_src(groups[g]), t.first, t.second)) {
        return std::nullopt;
      }
    }
  }
  std::vector<std::size_t> order;
  std::map<std::size_t, Group> by_key;
  for (std::size_t g = 0; g < groups.size(); ++g) {
    auto it = by_key.find(keys[g]);
    if (it == by_key.end()) {
      it = by_key.emplace(keys[g], Group{.load = groups[g].load}).first;
      order.push_back(keys[g]);
    }
    auto& stores = it->second.stores;
    stores.insert(stores.end(), groups[g].stores.begin(), groups[g].stores.end());
  }
  const std::size_t last = keys.back();
  order.erase(std::find(order.begin(), order.end(), last));
  order.push_back(last);
  std::vector<Group> out;
  for (std::size_t k : order) {
    out.push_back(std::move(by_key.at(k)));
  }
  return out;
}

void rewrite(std::vector<ir::Type_instruction>& instrs) {
  std::vector<ir::Type_instruction> out;
  bool changed = false;
  std::size_t i = 0;
  while (i < instrs.size()) {
    auto [groups, end] = run(instrs, i);
    std::optional<std::vector<Group>> result;
    if (!groups.empty()) {
      result = merged(std::move(groups));
    }
    if (!result) {
      out.push_back(instrs[i]);
      ++i;
      continue;
    }
    for (auto& g : *result) {
      out.push_back(std::move(g.load));
      for (auto& s : g.stores) {
        out.push_back(std::move(s));
      }
    }
    changed = true;
    i = end;
  }
  if (changed) {
    instrs = std::move(out);
  }
}

}  // namespace

asm_ast::Program apply_constant_store_order(asm_ast::Program prog) {
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      rewrite(fn->instructions);
    }
  }
  return prog;
}

}  // namespace c6502::passes
