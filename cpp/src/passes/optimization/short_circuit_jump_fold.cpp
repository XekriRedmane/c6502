#include "passes/optimization/short_circuit_jump_fold.h"

#include <map>
#include <optional>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "bit_precise.h"
#include "passes/optimization/value_range.h"
#include "passes/optimization/var_visit.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::tac_ast;
namespace c99 = ::c6502::c99_ast;
using asdl::Int;
using framework::Instrs;
using IK = ir::Kind_instruction;
using CK = ir::Kind_const;

std::string* jump_target(ir::Type_instruction& instr) {
  switch (ir::kind(instr)) {
    case IK::Jump:
      return &std::get<ir::Jump>(instr).target;
    case IK::JumpIfTrue:
      return &std::get<ir::JumpIfTrue>(instr).target;
    case IK::JumpIfFalse:
      return &std::get<ir::JumpIfFalse>(instr).target;
    case IK::JumpIfCmp:
      return &std::get<ir::JumpIfCmp>(instr).target;
    case IK::JumpIfMasked:
      return &std::get<ir::JumpIfMasked>(instr).target;
    default:
      return nullptr;
  }
}

const std::string* jump_target(const ir::Type_instruction& instr) {
  return jump_target(const_cast<ir::Type_instruction&>(instr));
}

std::optional<Int> const_int_value(const ir::Type_val& val) {
  const auto* c = std::get_if<ir::Constant>(&val);
  if (c == nullptr) {
    return std::nullopt;
  }
  const auto* i = std::get_if<ir::ConstInt>(&c->const_);
  if (i == nullptr) {
    return std::nullopt;
  }
  return i->value;
}

bool is_var_named(const ir::Type_val& v, const std::string& name) {
  const auto* var = std::get_if<ir::Var>(&v);
  return var != nullptr && var->name == name;
}

struct Fold {
  bool natural = false;
  std::string branch_label;
  std::string target;
  Instrs copies;
  Instrs sc_copies;
};

std::optional<Fold> try_fold(const Instrs& instrs, std::size_t i,
                             const OrderedMap<std::string, std::size_t>& use_count,
                             const std::map<std::string, std::size_t, std::less<>>& jump_targets) {
  const std::size_t n = instrs.size();
  if (i + 5 >= n) {
    return std::nullopt;
  }
  std::size_t k = i + 3;
  while (k < n && std::holds_alternative<ir::Copy>(instrs[k])) {
    ++k;
  }
  if (k == i + 3 || k >= n) {
    return std::nullopt;
  }
  std::size_t j = k + 1;
  while (j < n && std::holds_alternative<ir::Copy>(instrs[j])) {
    ++j;
  }
  if (j >= n) {
    return std::nullopt;
  }
  const auto* cp_ft = std::get_if<ir::Copy>(&instrs[i]);
  const auto* jump_in_tail = std::get_if<ir::Jump>(&instrs[i + 1]);
  const auto* label_branch = std::get_if<ir::Label>(&instrs[i + 2]);
  const auto* cp_sc = std::get_if<ir::Copy>(&instrs[k - 1]);
  const auto* label_end = std::get_if<ir::Label>(&instrs[k]);
  const ir::Type_instruction& consumer = instrs[j];
  const IK consumer_kind = ir::kind(consumer);
  if (cp_ft == nullptr || jump_in_tail == nullptr || label_branch == nullptr ||
      cp_sc == nullptr || label_end == nullptr ||
      (consumer_kind != IK::JumpIfTrue && consumer_kind != IK::JumpIfFalse)) {
    return std::nullopt;
  }
  const auto* ft_dst = std::get_if<ir::Var>(&cp_ft->dst);
  const auto* sc_dst = std::get_if<ir::Var>(&cp_sc->dst);
  if (ft_dst == nullptr || sc_dst == nullptr || ft_dst->name != sc_dst->name) {
    return std::nullopt;
  }
  const std::string& t_name = ft_dst->name;
  const auto c_ft = const_int_value(cp_ft->src);
  const auto c_sc = const_int_value(cp_sc->src);
  if (!c_ft || !c_sc) {
    return std::nullopt;
  }
  if (!((*c_ft == 0 && *c_sc == 1) || (*c_ft == 1 && *c_sc == 0))) {
    return std::nullopt;
  }
  if (jump_in_tail->target != label_end->name) {
    return std::nullopt;
  }
  const ir::Type_val& condition = consumer_kind == IK::JumpIfTrue
                                      ? std::get<ir::JumpIfTrue>(consumer).condition
                                      : std::get<ir::JumpIfFalse>(consumer).condition;
  if (!is_var_named(condition, t_name)) {
    return std::nullopt;
  }
  Fold fold;
  fold.copies.assign(instrs.begin() + static_cast<std::ptrdiff_t>(k + 1),
                     instrs.begin() + static_cast<std::ptrdiff_t>(j));
  fold.sc_copies.assign(instrs.begin() + static_cast<std::ptrdiff_t>(i + 3),
                        instrs.begin() + static_cast<std::ptrdiff_t>(k - 1));
  for (const Instrs* group : {&fold.copies, &fold.sc_copies}) {
    for (const auto& cp : *group) {
      const auto& c = std::get<ir::Copy>(cp);
      if (is_var_named(c.src, t_name) || is_var_named(c.dst, t_name)) {
        return std::nullopt;
      }
    }
  }
  const std::size_t* uses = use_count.find(t_name);
  if (uses == nullptr || *uses != 1) {
    return std::nullopt;
  }
  const auto jt = jump_targets.find(label_end->name);
  if (jt == jump_targets.end() || jt->second != 1) {
    return std::nullopt;
  }
  const Int consumer_branch_value = consumer_kind == IK::JumpIfTrue ? 1 : 0;
  fold.natural = *c_sc == consumer_branch_value;
  fold.branch_label = label_branch->name;
  fold.target = *jump_target(consumer);
  return fold;
}

std::map<std::string, std::size_t, std::less<>> count_jump_targets(const Instrs& instrs) {
  std::map<std::string, std::size_t, std::less<>> out;
  for (const auto& instr : instrs) {
    if (const std::string* t = jump_target(instr)) {
      ++out[*t];
    }
  }
  return out;
}

std::optional<int> const_bits(CK k) {
  switch (k) {
    case CK::ConstChar:
    case CK::ConstUChar:
      return 8;
    case CK::ConstInt:
    case CK::ConstUInt:
      return 16;
    case CK::ConstLong:
    case CK::ConstULong:
      return 32;
    case CK::ConstLongLong:
    case CK::ConstULongLong:
      return 64;
    default:
      return std::nullopt;
  }
}

bool signed_class(CK k) {
  return k == CK::ConstChar || k == CK::ConstInt || k == CK::ConstLong || k == CK::ConstLongLong;
}

struct EqualJump {
  std::string name;
  std::optional<ir::Type_const> constant;
  std::string target;
};

std::optional<EqualJump> jump_if_equal(const ir::Type_instruction& instr) {
  if (const auto* jf = std::get_if<ir::JumpIfFalse>(&instr)) {
    if (const auto* v = std::get_if<ir::Var>(&jf->condition)) {
      return EqualJump{.name = v->name, .constant = std::nullopt, .target = jf->target};
    }
    return std::nullopt;
  }
  const auto* jc = std::get_if<ir::JumpIfCmp>(&instr);
  if (jc == nullptr || !std::holds_alternative<ir::Equal>(jc->op)) {
    return std::nullopt;
  }
  const std::pair<const ir::Type_val*, const ir::Type_val*> orders[] = {{&jc->src1, &jc->src2},
                                                                         {&jc->src2, &jc->src1}};
  for (const auto& [v, k] : orders) {
    const auto* var = std::get_if<ir::Var>(v);
    const auto* c = std::get_if<ir::Constant>(k);
    if (var != nullptr && c != nullptr && const_bits(ir::kind(c->const_))) {
      return EqualJump{.name = var->name, .constant = c->const_, .target = jc->target};
    }
  }
  return std::nullopt;
}

struct Merged {
  std::string name;
  ir::Type_const mask;
  ir::Type_const low;
  std::string target;
};

std::optional<Merged> merged_test(const EqualJump& first, const EqualJump& second,
                                  const SymbolTable& symbols) {
  const Symbol* sym = symbols.find(first.name);
  if (sym == nullptr) {
    return std::nullopt;
  }
  const c99::Type_data_type* t = &sym->type;
  while (const auto* c = std::get_if<c99::Const>(t)) {
    t = &*c->referenced_type;
  }
  if (std::holds_alternative<c99::Volatile>(*t)) {
    return std::nullopt;
  }
  const auto width = value_range::int_width(*t);
  if (!width) {
    return std::nullopt;
  }
  const ir::Type_const* k1 = first.constant ? &*first.constant : nullptr;
  const ir::Type_const* k2 = second.constant ? &*second.constant : nullptr;
  const ir::Type_const* proto = k1 != nullptr ? k1 : k2;
  if (proto == nullptr) {
    return std::nullopt;
  }
  const CK cls = ir::kind(*proto);
  if (*const_bits(cls) != 8 * *width) {
    return std::nullopt;
  }
  for (const ir::Type_const* k : {k1, k2}) {
    if (k != nullptr && ir::kind(*k) != cls) {
      return std::nullopt;
    }
  }
  const int bits = 8 * *width;
  const Int full = (Int(1) << bits) - 1;
  const Int a = k1 == nullptr ? Int(0) : (*bit_precise::raw_value(*k1) & full);
  const Int b = k2 == nullptr ? Int(0) : (*bit_precise::raw_value(*k2) & full);
  const Int p = a ^ b;
  if (p == 0 || (p & (p - 1)) != 0) {
    return std::nullopt;
  }
  const Int low = a < b ? a : b;
  const auto as_const = [&](Int v) {
    if (signed_class(cls) && v > (full >> 1)) {
      v -= full + 1;
    }
    return bit_precise::with_value(*proto, v);
  };
  return Merged{.name = first.name,
                .mask = as_const(full & ~p),
                .low = as_const(low),
                .target = first.target};
}

}  // namespace

ir::Function fold_short_circuit_jump(ir::Function fn) {
  const Instrs& instrs = fn.instructions;
  const auto use_count = count_uses(instrs);
  const auto jump_targets = count_jump_targets(instrs);
  std::set<std::string, std::less<>> existing_labels;
  for (const auto& ins : instrs) {
    if (const auto* l = std::get_if<ir::Label>(&ins)) {
      existing_labels.insert(l->name);
    }
  }
  int label_counter = 0;
  const auto fresh_label = [&]() {
    while (true) {
      std::string name = "." + fn.name + "@scfold@" + std::to_string(label_counter);
      ++label_counter;
      if (!existing_labels.contains(name)) {
        existing_labels.insert(name);
        return name;
      }
    }
  };
  const bool ends_in_terminator =
      !instrs.empty() &&
      (std::holds_alternative<ir::Ret>(instrs.back()) ||
       std::holds_alternative<ir::Jump>(instrs.back()));
  OrderedMap<std::string, std::string> retarget_map;
  Instrs new_instrs;
  Instrs stubs;
  std::size_t i = 0;
  while (i < instrs.size()) {
    std::optional<Fold> fold = try_fold(instrs, i, use_count, jump_targets);
    if (fold && (!fold->copies.empty() || !fold->sc_copies.empty()) && fold->natural &&
        !ends_in_terminator) {
      fold.reset();
    }
    if (!fold) {
      new_instrs.push_back(instrs[i]);
      ++i;
      continue;
    }
    if (fold->natural) {
      if (!fold->copies.empty() || !fold->sc_copies.empty()) {
        std::string stub = fresh_label();
        stubs.push_back(ir::Label{.name = stub});
        stubs.insert(stubs.end(), fold->sc_copies.begin(), fold->sc_copies.end());
        stubs.insert(stubs.end(), fold->copies.begin(), fold->copies.end());
        stubs.push_back(ir::Jump{.target = fold->target});
        retarget_map.insert_or_assign(fold->branch_label, stub);
        new_instrs.insert(new_instrs.end(), fold->copies.begin(), fold->copies.end());
      } else {
        retarget_map.insert_or_assign(fold->branch_label, fold->target);
      }
    } else {
      std::string new_label = fresh_label();
      retarget_map.insert_or_assign(fold->branch_label, new_label);
      new_instrs.insert(new_instrs.end(), fold->copies.begin(), fold->copies.end());
      new_instrs.push_back(ir::Jump{.target = fold->target});
      new_instrs.push_back(ir::Label{.name = new_label});
      new_instrs.insert(new_instrs.end(), fold->sc_copies.begin(), fold->sc_copies.end());
      new_instrs.insert(new_instrs.end(), fold->copies.begin(), fold->copies.end());
    }
    i += 6 + fold->copies.size() + fold->sc_copies.size();
  }
  new_instrs.insert(new_instrs.end(), stubs.begin(), stubs.end());
  if (retarget_map.empty()) {
    return fn;
  }
  std::map<std::string, std::string, std::less<>> resolved;
  for (const auto& [key, first] : retarget_map) {
    std::string target = first;
    std::set<std::string, std::less<>> seen{key};
    while (true) {
      const std::string* next = retarget_map.find(target);
      if (next == nullptr || seen.contains(target)) {
        break;
      }
      seen.insert(target);
      target = *next;
    }
    resolved.insert_or_assign(key, target);
  }
  for (auto& instr : new_instrs) {
    if (std::string* t = jump_target(instr)) {
      if (const auto it = resolved.find(*t); it != resolved.end()) {
        *t = it->second;
      }
    }
  }
  return framework::with_instructions(fn, std::move(new_instrs));
}

ir::Function merge_equality_jumps(ir::Function fn, SymbolTable* symbols) {
  if (symbols == nullptr) {
    return fn;
  }
  const Instrs& instrs = fn.instructions;
  Instrs out;
  bool changed = false;
  std::size_t i = 0;
  while (i < instrs.size()) {
    const auto first = jump_if_equal(instrs[i]);
    const auto second =
        i + 1 < instrs.size() ? jump_if_equal(instrs[i + 1]) : std::optional<EqualJump>{};
    std::optional<Merged> merged;
    if (first && second && first->name == second->name && first->target == second->target) {
      merged = merged_test(*first, *second, *symbols);
    }
    if (!merged) {
      out.push_back(instrs[i]);
      ++i;
      continue;
    }
    const Symbol* sym = symbols->find(merged->name);
    int k = 0;
    while (symbols->contains(merged->name + ".eqm" + std::to_string(k))) {
      ++k;
    }
    const ir::Var tmp{.name = merged->name + ".eqm" + std::to_string(k)};
    c99::Type_data_type type = sym->type;
    symbols->insert_or_assign(tmp.name, Symbol{.type = std::move(type), .attrs = LocalAttr{}});
    out.push_back(ir::Binary{.op = ir::BitwiseAnd{},
                             .src1 = ir::Var{.name = merged->name},
                             .src2 = ir::Constant{.const_ = merged->mask},
                             .dst = tmp});
    if (*bit_precise::raw_value(merged->low) == 0) {
      out.push_back(ir::JumpIfFalse{.condition = tmp, .target = merged->target});
    } else {
      out.push_back(ir::JumpIfCmp{.op = ir::Equal{},
                                  .src1 = tmp,
                                  .src2 = ir::Constant{.const_ = merged->low},
                                  .target = merged->target});
    }
    changed = true;
    i += 2;
  }
  if (!changed) {
    return fn;
  }
  return framework::with_instructions(fn, std::move(out));
}

ir::Function fold_short_circuit_jumps(ir::Function fn, const framework::PassContext& ctx) {
  return merge_equality_jumps(fold_short_circuit_jump(std::move(fn)), ctx.symbols);
}

}  // namespace c6502::passes
