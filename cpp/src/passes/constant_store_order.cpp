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

std::optional<std::string> slot_owner(std::string_view name) {
  for (auto prefix : kSlotPrefixes) {
    if (name.substr(0, prefix.size()) == prefix) {
      std::string_view rest = name.substr(prefix.size());
      return std::string(rest.substr(0, rest.find("__")));
    }
  }
  return std::nullopt;
}

struct Group {
  ir::Type_instruction load;
  std::vector<ir::Type_instruction> stores;
};

const ir::Type_operand& load_src(const Group& g) { return std::get<ir::Mov>(g.load).src; }

const ir::Data& store_dst(const ir::Type_instruction& s) {
  return std::get<ir::Data>(std::get<ir::Mov>(s).dst);
}

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

// C99 §6.5.6p8 keeps a subscript inside its object.
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
