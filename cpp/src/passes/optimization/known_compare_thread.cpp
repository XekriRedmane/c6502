#include "passes/optimization/known_compare_thread.h"

#include <algorithm>
#include <cstdint>
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

constexpr int kSteps = 16;

constexpr const char* kLabelPrefix = "cthread@";

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

bool is_jump(IK k) {
  return k == IK::Jump || k == IK::JumpIfTrue || k == IK::JumpIfFalse || k == IK::JumpIfCmp ||
         k == IK::JumpIfMasked;
}

bool is_conditional(IK k) {
  return k == IK::JumpIfTrue || k == IK::JumpIfFalse || k == IK::JumpIfCmp ||
         k == IK::JumpIfMasked;
}

bool writes_dst_only(IK k) {
  switch (k) {
    case IK::Copy:
    case IK::Unary:
    case IK::Binary:
    case IK::SignExtend:
    case IK::ZeroExtend:
    case IK::Truncate:
    case IK::IntToFloat:
    case IK::IntToDouble:
    case IK::FloatToInt:
    case IK::DoubleToInt:
    case IK::FloatToDouble:
    case IK::DoubleToFloat:
    case IK::GetAddress:
    case IK::Load:
    case IK::IndexedLoad:
    case IK::IndexedConstLoad:
    case IK::IndirectIndexedLoad:
      return true;
    default:
      return false;
  }
}

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

const std::string& target_of(const ir::Type_instruction& instr) {
  return *jump_target(const_cast<ir::Type_instruction&>(instr));
}

bool is_volatile(const c99::Type_data_type& type) {
  const c99::Type_data_type* t = &type;
  while (true) {
    if (std::holds_alternative<c99::Volatile>(*t)) {
      return true;
    }
    const auto* c = std::get_if<c99::Const>(t);
    if (c == nullptr) {
      return false;
    }
    t = &*c->referenced_type;
  }
}

std::optional<int> fact_width(const Symbol* sym) {
  if (sym == nullptr || is_volatile(sym->type)) {
    return std::nullopt;
  }
  return value_range::int_width(sym->type);
}

struct Test {
  std::string name;
  Int value = 0;
  bool equal_when_taken = false;
};

struct Known {
  std::optional<Int> eq;
  std::set<Int> ne;
};
using Facts = std::map<std::string, Known, std::less<>>;

std::optional<bool> decide(const Facts& facts, const Test& test) {
  const auto it = facts.find(test.name);
  if (it == facts.end()) {
    return std::nullopt;
  }
  bool equal = false;
  if (it->second.eq) {
    equal = *it->second.eq == test.value;
  } else if (it->second.ne.contains(test.value)) {
    equal = false;
  } else {
    return std::nullopt;
  }
  return equal == test.equal_when_taken;
}

void learn(Facts& facts, const Test& test, bool taken) {
  const bool equal = test.equal_when_taken == taken;
  Known& known = facts[test.name];
  if (equal) {
    known.eq = test.value;
  } else {
    known.ne.insert(test.value);
  }
}

class FunctionInfo {
 public:
  FunctionInfo(const Instrs& instrs, const SymbolTable& symbols)
      : instrs_(instrs), symbols_(symbols) {
    for (std::size_t k = 0; k < instrs.size(); ++k) {
      if (const auto* l = std::get_if<ir::Label>(&instrs[k])) {
        labels_.insert_or_assign(l->name, k);
      } else if (const auto* g = std::get_if<ir::GetAddress>(&instrs[k])) {
        if (const auto* v = std::get_if<ir::Var>(&g->operand)) {
          addressed_.insert(v->name);
        }
      }
    }
  }

  const Instrs& instrs() const { return instrs_; }

  std::optional<std::size_t> label(std::string_view name) const {
    const auto it = labels_.find(name);
    if (it == labels_.end()) {
      return std::nullopt;
    }
    return it->second;
  }

  std::optional<int> width_of(const std::string& name) {
    const auto it = widths_.find(name);
    if (it != widths_.end()) {
      return it->second;
    }
    const auto w = fact_width(symbols_.find(name));
    widths_.emplace(name, w);
    return w;
  }

  const std::string* skippable(const ir::Type_instruction& ins) const {
    const auto* cp = std::get_if<ir::Copy>(&ins);
    if (cp == nullptr) {
      return nullptr;
    }
    const auto* dst = std::get_if<ir::Var>(&cp->dst);
    if (dst == nullptr) {
      return nullptr;
    }
    const Symbol* sym = symbols_.find(dst->name);
    if (sym == nullptr || !std::holds_alternative<LocalAttr>(sym->attrs) ||
        addressed_.contains(dst->name)) {
      return nullptr;
    }
    if (const auto* src = std::get_if<ir::Var>(&cp->src)) {
      const Symbol* ssym = symbols_.find(src->name);
      if (ssym == nullptr || is_volatile(ssym->type)) {
        return nullptr;
      }
    }
    return &dst->name;
  }

  bool any_live_at(const std::set<std::string, std::less<>>& names, std::size_t i) {
    if (i >= instrs_.size()) {
      return false;
    }
    if (!live_) {
      compute_liveness();
    }
    const auto& live = (*live_)[i];
    for (const auto& name : names) {
      const auto it = ids_.find(name);
      if (it == ids_.end()) {
        continue;
      }
      const std::size_t id = it->second;
      if ((live[id / 64] >> (id % 64)) & 1) {
        return true;
      }
    }
    return false;
  }

 private:
  std::size_t id_of(std::string_view name) {
    const auto it = ids_.find(name);
    if (it != ids_.end()) {
      return it->second;
    }
    const std::size_t id = ids_.size();
    ids_.emplace(std::string(name), id);
    return id;
  }

  void compute_liveness() {
    const std::size_t n = instrs_.size();
    std::vector<std::vector<std::size_t>> succ(n);
    std::vector<std::vector<std::size_t>> uses(n);
    std::vector<std::vector<std::size_t>> defs(n);
    for (std::size_t k = 0; k < n; ++k) {
      const auto& ins = instrs_[k];
      const IK kind = ir::kind(ins);
      if (k + 1 < n && kind != IK::Jump && kind != IK::Ret) {
        succ[k].push_back(k + 1);
      }
      if (is_jump(kind)) {
        if (const auto j = label(target_of(ins))) {
          succ[k].push_back(*j);
        }
      }
      for (const auto name : uses_in(ins)) {
        uses[k].push_back(id_of(name));
      }
      for (const auto name : defs_in(ins)) {
        defs[k].push_back(id_of(name));
      }
    }
    const std::size_t words = (ids_.size() + 63) / 64;
    std::vector<std::vector<std::uint64_t>> live(n, std::vector<std::uint64_t>(words, 0));
    std::vector<std::uint64_t> next(words);
    bool changed = true;
    while (changed) {
      changed = false;
      for (std::size_t k = n; k-- > 0;) {
        std::fill(next.begin(), next.end(), 0);
        for (const std::size_t s : succ[k]) {
          for (std::size_t w = 0; w < words; ++w) {
            next[w] |= live[s][w];
          }
        }
        for (const std::size_t d : defs[k]) {
          next[d / 64] &= ~(std::uint64_t{1} << (d % 64));
        }
        for (const std::size_t u : uses[k]) {
          next[u / 64] |= std::uint64_t{1} << (u % 64);
        }
        if (next != live[k]) {
          live[k] = next;
          changed = true;
        }
      }
    }
    live_ = std::move(live);
  }

  const Instrs& instrs_;
  const SymbolTable& symbols_;
  std::map<std::string, std::size_t, std::less<>> labels_;
  std::set<std::string, std::less<>> addressed_;
  std::map<std::string, std::optional<int>, std::less<>> widths_;
  std::map<std::string, std::size_t, std::less<>> ids_;
  std::optional<std::vector<std::vector<std::uint64_t>>> live_;
};

std::optional<Test> equality_test(const ir::Type_instruction& ins, FunctionInfo& info) {
  const IK kind = ir::kind(ins);
  if (kind == IK::JumpIfTrue || kind == IK::JumpIfFalse) {
    const ir::Type_val& cond = kind == IK::JumpIfTrue ? std::get<ir::JumpIfTrue>(ins).condition
                                                      : std::get<ir::JumpIfFalse>(ins).condition;
    const auto* v = std::get_if<ir::Var>(&cond);
    if (v != nullptr && info.width_of(v->name)) {
      return Test{.name = v->name, .value = 0, .equal_when_taken = kind == IK::JumpIfFalse};
    }
    return std::nullopt;
  }
  if (kind != IK::JumpIfCmp) {
    return std::nullopt;
  }
  const auto& jc = std::get<ir::JumpIfCmp>(ins);
  const bool is_equal = std::holds_alternative<ir::Equal>(jc.op);
  if (!is_equal && !std::holds_alternative<ir::NotEqual>(jc.op)) {
    return std::nullopt;
  }
  const std::pair<const ir::Type_val*, const ir::Type_val*> orders[] = {{&jc.src1, &jc.src2},
                                                                         {&jc.src2, &jc.src1}};
  for (const auto& [v, c] : orders) {
    const auto* var = std::get_if<ir::Var>(v);
    const auto* k = std::get_if<ir::Constant>(c);
    if (var == nullptr || k == nullptr) {
      continue;
    }
    const auto w = info.width_of(var->name);
    const auto bits = const_bits(ir::kind(k->const_));
    if (!w || !bits || *bits != 8 * *w) {
      continue;
    }
    const Int value = *bit_precise::raw_value(k->const_) & ((Int(1) << *bits) - 1);
    return Test{.name = var->name, .value = value, .equal_when_taken = is_equal};
  }
  return std::nullopt;
}

std::optional<std::size_t> walk(FunctionInfo& info, Facts facts, const std::string& target) {
  const Instrs& instrs = info.instrs();
  const auto start = info.label(target);
  if (!start) {
    return std::nullopt;
  }
  std::size_t i = *start;
  std::optional<std::size_t> best;
  std::set<std::string, std::less<>> skipped;
  std::set<std::size_t> seen;
  int steps = 0;
  while (i < instrs.size() && steps < kSteps) {
    const auto& ins = instrs[i];
    if (std::holds_alternative<ir::Label>(ins)) {
      ++i;
      continue;
    }
    if (seen.contains(i)) {
      break;
    }
    seen.insert(i);
    if (const std::string* dst = info.skippable(ins)) {
      facts.erase(*dst);
      skipped.insert(*dst);
      ++i;
      continue;
    }
    const auto test = is_jump(ir::kind(ins)) ? equality_test(ins, info) : std::nullopt;
    if (!test) {
      break;
    }
    const auto taken = decide(facts, *test);
    if (!taken) {
      break;
    }
    ++steps;
    learn(facts, *test, *taken);
    if (*taken) {
      const auto j = info.label(target_of(ins));
      if (!j) {
        break;
      }
      i = *j;
    } else {
      ++i;
    }
    while (i < instrs.size() && std::holds_alternative<ir::Label>(instrs[i])) {
      ++i;
    }
    if (skipped.empty() || !info.any_live_at(skipped, i)) {
      best = i;
    }
  }
  return best;
}

long long next_label_number(const Instrs& instrs, const std::string& fn_name) {
  const std::string prefix = "." + fn_name + "@" + kLabelPrefix;
  long long next = 0;
  for (const auto& ins : instrs) {
    const auto* l = std::get_if<ir::Label>(&ins);
    if (l == nullptr || !l->name.starts_with(prefix)) {
      continue;
    }
    const std::string rest = l->name.substr(prefix.size());
    if (rest.empty() || rest.find_first_not_of("0123456789") != std::string::npos) {
      continue;
    }
    next = std::max(next, std::stoll(rest) + 1);
  }
  return next;
}

ir::Function thread(ir::Function fn, const SymbolTable& symbols) {
  const Instrs& instrs = fn.instructions;
  FunctionInfo info(instrs, symbols);
  std::map<std::size_t, std::string> retarget;
  std::map<std::size_t, std::optional<ir::Type_instruction>> replaced;
  std::map<std::size_t, std::string> added;
  long long counter = next_label_number(instrs, fn.name);
  Facts facts;
  for (std::size_t k = 0; k < instrs.size(); ++k) {
    const ir::Type_instruction* ins = &instrs[k];
    const IK kind = ir::kind(*ins);
    if (kind == IK::Label) {
      facts.clear();
      continue;
    }
    if (is_jump(kind)) {
      const auto test = equality_test(*ins, info);
      bool now_jump = kind == IK::Jump;
      if (test) {
        const auto decided = decide(facts, *test);
        if (decided && !*decided) {
          replaced[k] = std::nullopt;
          continue;
        }
        if (decided && *decided) {
          replaced[k] = ir::Type_instruction{ir::Jump{.target = target_of(*ins)}};
          ins = &*replaced[k];
          now_jump = true;
        }
      }
      Facts edge = facts;
      if (test && !now_jump) {
        learn(edge, *test, true);
      }
      const auto dest = walk(info, std::move(edge), target_of(*ins));
      if (dest) {
        if (*dest > 0 && std::holds_alternative<ir::Label>(instrs[*dest - 1])) {
          retarget[k] = std::get<ir::Label>(instrs[*dest - 1]).name;
        } else {
          if (!added.contains(*dest)) {
            added[*dest] = "." + fn.name + "@" + kLabelPrefix + std::to_string(counter);
            ++counter;
          }
          retarget[k] = added[*dest];
        }
      }
      if (now_jump) {
        facts.clear();
      } else if (test) {
        learn(facts, *test, false);
      }
      continue;
    }
    if (writes_dst_only(kind)) {
      if (const auto dst = framework::dst_name(*ins)) {
        if (const auto it = facts.find(*dst); it != facts.end()) {
          facts.erase(it);
        }
      }
      continue;
    }
    facts.clear();
  }
  if (retarget.empty() && replaced.empty()) {
    return fn;
  }
  Instrs out;
  for (std::size_t k = 0; k < instrs.size(); ++k) {
    if (const auto it = added.find(k); it != added.end()) {
      out.push_back(ir::Label{.name = it->second});
    }
    ir::Type_instruction ins = instrs[k];
    if (const auto it = replaced.find(k); it != replaced.end()) {
      if (!it->second) {
        continue;
      }
      ins = *it->second;
    }
    if (const auto it = retarget.find(k); it != retarget.end()) {
      *jump_target(ins) = it->second;
    }
    out.push_back(std::move(ins));
  }
  return framework::with_instructions(fn, std::move(out));
}

bool is_terminator(IK k) { return k == IK::Jump || k == IK::Ret; }

std::optional<Instrs> drop_unreached(const Instrs& start) {
  std::optional<Instrs> result;
  const Instrs* instrs = &start;
  while (true) {
    std::set<std::string, std::less<>> named;
    for (const auto& ins : *instrs) {
      if (is_jump(ir::kind(ins))) {
        named.insert(target_of(ins));
      }
    }
    Instrs out;
    bool dropping = false;
    for (const auto& ins : *instrs) {
      if (const auto* l = std::get_if<ir::Label>(&ins)) {
        const bool unreached = dropping || (!out.empty() && is_terminator(ir::kind(out.back())));
        if (unreached && !named.contains(l->name)) {
          dropping = true;
          continue;
        }
        dropping = false;
      } else if (dropping) {
        continue;
      }
      out.push_back(ins);
    }
    if (out.size() == instrs->size()) {
      return result;
    }
    result = std::move(out);
    instrs = &*result;
  }
}

std::pair<std::size_t, std::vector<std::string>> label_run(const Instrs& instrs, std::size_t i) {
  std::vector<std::string> names;
  while (i < instrs.size()) {
    const auto* l = std::get_if<ir::Label>(&instrs[i]);
    if (l == nullptr) {
      break;
    }
    names.push_back(l->name);
    ++i;
  }
  return {i, std::move(names)};
}

bool contains(const std::vector<std::string>& names, const std::string& name) {
  return std::find(names.begin(), names.end(), name) != names.end();
}

std::optional<std::pair<std::pair<std::size_t, std::size_t>, Instrs>> swap_at(const Instrs& instrs,
                                                                              std::size_t k) {
  const auto& cond = instrs[k];
  if (!is_conditional(ir::kind(cond))) {
    return std::nullopt;
  }
  const std::size_t n = instrs.size();
  const std::size_t j = label_run(instrs, k + 1).first;
  if (j >= n || !std::holds_alternative<ir::Jump>(instrs[j])) {
    return std::nullopt;
  }
  const std::size_t a_start = j + 1;
  const auto [a_body, a_labels] = label_run(instrs, a_start);
  if (a_labels.empty()) {
    return std::nullopt;
  }
  std::size_t a_end = a_body;
  while (a_end < n && !std::holds_alternative<ir::Label>(instrs[a_end])) {
    ++a_end;
  }
  if (a_end == a_body) {
    return std::nullopt;
  }
  const auto* last = std::get_if<ir::Jump>(&instrs[a_end - 1]);
  if (last == nullptr) {
    return std::nullopt;
  }
  for (std::size_t x = a_body; x + 1 < a_end; ++x) {
    if (is_terminator(ir::kind(instrs[x]))) {
      return std::nullopt;
    }
  }
  const auto [e_body, e_labels] = label_run(instrs, a_end);
  if (!contains(e_labels, target_of(cond))) {
    return std::nullopt;
  }
  std::size_t e_end = e_body;
  while (e_end < n && !std::holds_alternative<ir::Label>(instrs[e_end])) {
    const IK kind = ir::kind(instrs[e_end]);
    if (is_jump(kind) || kind == IK::Ret) {
      return std::nullopt;
    }
    ++e_end;
  }
  if (e_end == e_body) {
    return std::nullopt;
  }
  const auto c_labels = label_run(instrs, e_end).second;
  if (!contains(c_labels, last->target)) {
    return std::nullopt;
  }
  Instrs replacement(instrs.begin() + static_cast<std::ptrdiff_t>(a_end),
                     instrs.begin() + static_cast<std::ptrdiff_t>(e_end));
  replacement.push_back(ir::Jump{.target = last->target});
  replacement.insert(replacement.end(), instrs.begin() + static_cast<std::ptrdiff_t>(a_start),
                     instrs.begin() + static_cast<std::ptrdiff_t>(a_end - 1));
  return std::pair{std::pair{a_start, e_end}, std::move(replacement)};
}

std::optional<Instrs> place_branch_targets(const Instrs& instrs) {
  Instrs out = instrs;
  bool changed = false;
  for (std::size_t k = 0; k < out.size(); ++k) {
    auto swapped = swap_at(out, k);
    if (swapped) {
      std::move(swapped->second.begin(), swapped->second.end(),
                out.begin() + static_cast<std::ptrdiff_t>(swapped->first.first));
      changed = true;
    }
  }
  if (!changed) {
    return std::nullopt;
  }
  return out;
}

}  // namespace

ir::Function thread_known_compares(ir::Function fn, const framework::PassContext& ctx) {
  if (ctx.symbols == nullptr) {
    return fn;
  }
  ir::Function threaded = thread(std::move(fn), *ctx.symbols);
  std::optional<Instrs> dropped = drop_unreached(threaded.instructions);
  const Instrs& after_drop = dropped ? *dropped : threaded.instructions;
  std::optional<Instrs> placed = place_branch_targets(after_drop);
  if (!placed && !dropped) {
    return threaded;
  }
  return framework::with_instructions(threaded, placed ? std::move(*placed) : std::move(*dropped));
}

}  // namespace c6502::passes
