#include "passes/optimization/ssa_construction.h"

#include <cstddef>
#include <functional>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <vector>

#include "errors.h"
#include "passes/optimization/cfg.h"
#include "passes/optimization/var_visit.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::tac_ast;
namespace c99 = ::c6502::c99_ast;
using K = ir::Kind_instruction;
using tac_cfg::BasicBlock;
using tac_cfg::Cfg;
using tac_cfg::kEntryId;
using tac_cfg::kExitId;
using Names = std::set<std::string, std::less<>>;

ir::Function maybe_prepend_preheader(ir::Function fn) {
  if (fn.instructions.empty() || !asdl::isa<ir::Label>(fn.instructions.front())) {
    return fn;
  }
  Names existing;
  for (const auto& i : fn.instructions) {
    if (const auto* l = asdl::as<ir::Label>(i)) {
      existing.insert(l->name);
    }
  }
  std::string name;
  for (int counter = 0;; ++counter) {
    name = "." + fn.name + "@ssa_preheader@" + std::to_string(counter);
    if (!existing.contains(name)) {
      break;
    }
  }
  fn.instructions.insert(fn.instructions.begin(), ir::Label{.name = name});
  return fn;
}

void ensure_block_labels(Cfg& cfg, const std::string& fn_name) {
  Names existing;
  for (const auto& [bid, blk] : cfg.blocks) {
    for (const auto& i : blk.instructions) {
      if (const auto* l = asdl::as<ir::Label>(i)) {
        existing.insert(l->name);
      }
    }
  }
  int counter = 0;
  for (const int bid : cfg.block_order) {
    BasicBlock& blk = *cfg.blocks.find(bid);
    if (!blk.instructions.empty() && asdl::isa<ir::Label>(blk.instructions.front())) {
      continue;
    }
    std::string name;
    while (true) {
      name = "." + fn_name + "@ssa_block@" + std::to_string(counter);
      ++counter;
      if (!existing.contains(name)) {
        existing.insert(name);
        break;
      }
    }
    blk.instructions.insert(blk.instructions.begin(), ir::Label{.name = name});
  }
}

const std::string& block_label(const BasicBlock& blk) {
  if (blk.instructions.empty() || !asdl::isa<ir::Label>(blk.instructions.front())) {
    throw InternalError("to_ssa: block " + std::to_string(blk.id) +
                        " has no leading label");
  }
  return std::get<ir::Label>(blk.instructions.front()).name;
}

bool is_volatile_qualified(const c99::Type_data_type* t) {
  while (const auto* c = asdl::as<c99::Const>(*t)) {
    t = &*c->referenced_type;
  }
  return asdl::isa<c99::Volatile>(*t);
}

bool is_scalar(const c99::Type_data_type& t) {
  switch (c99::kind(t)) {
    case c99::Kind_data_type::Int:
    case c99::Kind_data_type::Long:
    case c99::Kind_data_type::LongLong:
    case c99::Kind_data_type::UInt:
    case c99::Kind_data_type::ULong:
    case c99::Kind_data_type::ULongLong:
    case c99::Kind_data_type::Char:
    case c99::Kind_data_type::SChar:
    case c99::Kind_data_type::UChar:
    case c99::Kind_data_type::Float:
    case c99::Kind_data_type::Double:
    case c99::Kind_data_type::Pointer:
    case c99::Kind_data_type::BitInt:
      return true;
    default:
      return false;
  }
}

Names identify_promotable(const Cfg& cfg, const SymbolTable& symbols) {
  Names address_taken;
  Names candidates;
  for (const auto& [bid, blk] : cfg.blocks) {
    for (const auto& instr : blk.instructions) {
      for (const ir::Type_val* v : vals_in(instr)) {
        if (const auto* var = asdl::as<ir::Var>(*v)) {
          candidates.insert(var->name);
        }
      }
      if (const auto* ga = asdl::as<ir::GetAddress>(instr)) {
        if (const auto* var = asdl::as<ir::Var>(ga->operand)) {
          address_taken.insert(var->name);
        }
      }
    }
  }
  Names promotable;
  for (const auto& name : candidates) {
    if (address_taken.contains(name)) {
      continue;
    }
    const Symbol* sym = symbols.find(name);
    if (sym == nullptr || !std::holds_alternative<LocalAttr>(sym->attrs)) {
      continue;
    }
    if (is_volatile_qualified(&sym->type) || !is_scalar(sym->type)) {
      continue;
    }
    promotable.insert(name);
  }
  return promotable;
}

std::map<int, Names> compute_live_in(const Cfg& cfg, const Names& promotable) {
  std::map<int, Names> gen;
  std::map<int, Names> kill;
  for (const auto& [bid, blk] : cfg.blocks) {
    Names gen_b;
    Names kill_b;
    for (const auto& instr : blk.instructions) {
      for (const auto u : uses_in(instr)) {
        if (promotable.contains(u) && !kill_b.contains(u)) {
          gen_b.emplace(u);
        }
      }
      for (const auto d : defs_in(instr)) {
        if (promotable.contains(d)) {
          kill_b.emplace(d);
        }
      }
    }
    gen[bid] = std::move(gen_b);
    kill[bid] = std::move(kill_b);
  }
  std::map<int, Names> live_in;
  std::map<int, Names> live_out;
  for (const auto& [bid, blk] : cfg.blocks) {
    live_in[bid];
    live_out[bid];
  }
  bool changed = true;
  while (changed) {
    changed = false;
    for (const auto& [bid, blk] : cfg.blocks) {
      Names new_out;
      for (const int s : blk.successors) {
        const auto& in = live_in[s];
        new_out.insert(in.begin(), in.end());
      }
      Names new_in = gen[bid];
      for (const auto& v : new_out) {
        if (!kill[bid].contains(v)) {
          new_in.insert(v);
        }
      }
      if (new_out != live_out[bid] || new_in != live_in[bid]) {
        live_out[bid] = std::move(new_out);
        live_in[bid] = std::move(new_in);
        changed = true;
      }
    }
  }
  return live_in;
}

using PhisAt = std::map<int, std::vector<std::pair<std::string, std::size_t>>>;

std::size_t insert_phi(BasicBlock& blk, ir::Phi phi) {
  block_label(blk);
  std::size_t pos = 1;
  while (pos < blk.instructions.size() && asdl::isa<ir::Phi>(blk.instructions[pos])) {
    ++pos;
  }
  blk.instructions.insert(blk.instructions.begin() + static_cast<std::ptrdiff_t>(pos),
                          std::move(phi));
  return pos;
}

PhisAt place_phis(Cfg& cfg, const Names& promotable,
                  const OrderedMap<int, PyIntSet>& df, const std::map<int, Names>& live_in) {
  OrderedMap<std::string, std::set<std::int64_t>> defs;
  for (const auto& [bid, blk] : cfg.blocks) {
    for (const auto& instr : blk.instructions) {
      for (const auto d : defs_in(instr)) {
        if (promotable.contains(d)) {
          defs[std::string(d)].insert(bid);
        }
      }
    }
  }

  PhisAt phis_at;
  for (const auto& [v, def_blocks] : defs) {
    std::vector<std::int64_t> worklist(def_blocks.begin(), def_blocks.end());
    std::set<std::int64_t> already_placed;
    std::set<std::int64_t> already_visited(def_blocks);
    while (!worklist.empty()) {
      const std::int64_t x = worklist.back();
      worklist.pop_back();
      const PyIntSet* frontier = df.find(static_cast<int>(x));
      if (frontier == nullptr) {
        continue;
      }
      for (const std::int64_t y : frontier->sorted()) {
        if (already_placed.contains(y)) {
          continue;
        }
        BasicBlock& blk = *cfg.blocks.find(static_cast<int>(y));
        if (blk.predecessors.size() < 2) {
          continue;
        }
        const auto live = live_in.find(static_cast<int>(y));
        if (live == live_in.end() || !live->second.contains(v)) {
          continue;
        }
        const std::size_t idx = insert_phi(blk, ir::Phi{.dst = ir::Var{.name = v}, .args = {}});
        phis_at[static_cast<int>(y)].emplace_back(v, idx);
        already_placed.insert(y);
        if (!already_visited.contains(y)) {
          already_visited.insert(y);
          worklist.push_back(y);
        }
      }
    }
  }
  return phis_at;
}

struct Renamer {
  Cfg& cfg;
  const Names& promotable;
  const OrderedMap<int, std::vector<int>>& children;
  const std::map<int, std::string>& block_label_of;
  const PhisAt& phis_at;
  SymbolTable& symbols;
  std::map<std::string, int, std::less<>> counters;
  std::map<std::string, std::vector<std::string>, std::less<>> stacks;
  NameSet ssa_dsts;

  bool promotable_var(const ir::Type_val& v) const {
    const auto* var = asdl::as<ir::Var>(v);
    return var != nullptr && promotable.contains(var->name);
  }

  std::string fresh(const std::string& orig) {
    const int n = ++counters[orig];
    std::string new_name = orig + "." + std::to_string(n);
    if (const Symbol* sym = symbols.find(orig)) {
      Symbol copy{.type = sym->type, .attrs = LocalAttr{}};
      symbols.insert_or_assign(new_name, std::move(copy));
    }
    ssa_dsts.insert(new_name);
    return new_name;
  }

  void use(ir::Type_val& v) const {
    if (!promotable_var(v)) {
      return;
    }
    const auto& stack = stacks.find(std::get<ir::Var>(v).name)->second;
    if (!stack.empty()) {
      v = ir::Var{.name = stack.back()};
    }
  }

  void def(ir::Type_val& v, std::vector<std::string>& pushed) {
    if (!promotable_var(v)) {
      return;
    }
    const std::string orig = std::get<ir::Var>(v).name;
    std::string name = fresh(orig);
    stacks[orig].push_back(name);
    pushed.push_back(orig);
    v = ir::Var{.name = std::move(name)};
  }

  void rewrite_instruction(ir::Type_instruction& instr, std::vector<std::string>& pushed) {
    switch (ir::kind(instr)) {
      case K::Ret: {
        auto& r = std::get<ir::Ret>(instr);
        if (r.val) {
          use(*r.val);
        }
        break;
      }
#define C6502_SRC_DST(Name)                  \
  case K::Name: {                            \
    auto& x = std::get<ir::Name>(instr);     \
    use(x.src);                              \
    def(x.dst, pushed);                      \
    break;                                   \
  }
        C6502_SRC_DST(SignExtend)
        C6502_SRC_DST(ZeroExtend)
        C6502_SRC_DST(Truncate)
        C6502_SRC_DST(IntToFloat)
        C6502_SRC_DST(IntToDouble)
        C6502_SRC_DST(FloatToInt)
        C6502_SRC_DST(DoubleToInt)
        C6502_SRC_DST(FloatToDouble)
        C6502_SRC_DST(DoubleToFloat)
        C6502_SRC_DST(Unary)
        C6502_SRC_DST(Copy)
#undef C6502_SRC_DST
      case K::Binary: {
        auto& x = std::get<ir::Binary>(instr);
        use(x.src1);
        use(x.src2);
        def(x.dst, pushed);
        break;
      }
      case K::GetAddress:
        def(std::get<ir::GetAddress>(instr).dst, pushed);
        break;
      case K::Load: {
        auto& x = std::get<ir::Load>(instr);
        use(x.src_ptr);
        def(x.dst, pushed);
        break;
      }
      case K::Store: {
        auto& x = std::get<ir::Store>(instr);
        use(x.src);
        use(x.dst_ptr);
        break;
      }
      case K::IndexedLoad: {
        auto& x = std::get<ir::IndexedLoad>(instr);
        use(x.index);
        def(x.dst, pushed);
        break;
      }
      case K::IndexedStore: {
        auto& x = std::get<ir::IndexedStore>(instr);
        use(x.index);
        use(x.src);
        break;
      }
      case K::IndexedSymbolStore: {
        auto& x = std::get<ir::IndexedSymbolStore>(instr);
        use(x.index);
        use(x.src);
        break;
      }
      case K::IndexedConstLoad: {
        auto& x = std::get<ir::IndexedConstLoad>(instr);
        use(x.index);
        def(x.dst, pushed);
        break;
      }
      case K::IndirectIndexedLoad: {
        auto& x = std::get<ir::IndirectIndexedLoad>(instr);
        use(x.ptr);
        use(x.index);
        def(x.dst, pushed);
        break;
      }
      case K::IndirectIndexedStore: {
        auto& x = std::get<ir::IndirectIndexedStore>(instr);
        use(x.ptr);
        use(x.index);
        use(x.src);
        break;
      }
      case K::JumpIfCmp: {
        auto& x = std::get<ir::JumpIfCmp>(instr);
        use(x.src1);
        use(x.src2);
        break;
      }
      case K::JumpIfMasked:
        use(std::get<ir::JumpIfMasked>(instr).val);
        break;
      case K::JumpIfTrue:
        use(std::get<ir::JumpIfTrue>(instr).condition);
        break;
      case K::JumpIfFalse:
        use(std::get<ir::JumpIfFalse>(instr).condition);
        break;
      case K::FunctionCall: {
        auto& x = std::get<ir::FunctionCall>(instr);
        for (auto& a : x.args) {
          use(a);
        }
        if (x.dst) {
          def(*x.dst, pushed);
        }
        break;
      }
      case K::IndirectCall: {
        auto& x = std::get<ir::IndirectCall>(instr);
        use(x.ptr);
        for (auto& a : x.args) {
          use(a);
        }
        if (x.dst) {
          def(*x.dst, pushed);
        }
        break;
      }
      case K::Label:
      case K::Jump:
      case K::Phi:
        break;
    }
  }

  void visit(int bid) {
    BasicBlock* blk = cfg.blocks.find(bid);
    if (blk == nullptr) {
      return;
    }
    std::vector<std::string> pushed;

    for (auto& instr : blk->instructions) {
      auto* phi = asdl::as<ir::Phi>(instr);
      if (phi == nullptr || !promotable_var(phi->dst)) {
        continue;
      }
      const std::string orig = std::get<ir::Var>(phi->dst).name;
      std::string name = fresh(orig);
      stacks[orig].push_back(name);
      pushed.push_back(orig);
      phi->dst = ir::Var{.name = std::move(name)};
    }

    for (auto& instr : blk->instructions) {
      if (!asdl::isa<ir::Phi>(instr)) {
        rewrite_instruction(instr, pushed);
      }
    }

    std::string pred_label;
    if (bid != kEntryId && bid != kExitId) {
      pred_label = block_label(*blk);
    }
    if (const auto it = block_label_of.find(bid); it != block_label_of.end()) {
      pred_label = it->second;
    }
    const std::vector<int> successors = blk->successors;
    for (const int succ_id : successors) {
      const auto phis = phis_at.find(succ_id);
      if (phis == phis_at.end()) {
        continue;
      }
      BasicBlock& succ = *cfg.blocks.find(succ_id);
      for (const auto& [orig_var, idx] : phis->second) {
        const auto& stack = stacks.find(orig_var)->second;
        const std::string& src_name = stack.empty() ? orig_var : stack.back();
        std::get<ir::Phi>(succ.instructions[idx])
            .args.push_back(ir::PhiArg{.pred_label = pred_label,
                                       .source = ir::Var{.name = src_name}});
      }
    }

    if (const auto* kids = children.find(bid)) {
      for (const int child : *kids) {
        visit(child);
      }
    }

    for (const auto& orig : pushed) {
      stacks[orig].pop_back();
    }
  }
};

}  // namespace

std::pair<ir::Function, NameSet> to_ssa(ir::Function fn, SymbolTable& symbols) {
  fn = maybe_prepend_preheader(std::move(fn));
  Cfg cfg = tac_cfg::build_cfg(fn);
  ensure_block_labels(cfg, fn.name);

  const Names promotable = identify_promotable(cfg, symbols);
  if (promotable.empty()) {
    return {tac_cfg::cfg_to_function(fn, cfg), {}};
  }

  const OrderedMap<int, int> idom = tac_cfg::immediate_dominators(cfg);
  const OrderedMap<int, PyIntSet> df = tac_cfg::dominance_frontiers(cfg);
  const OrderedMap<int, std::vector<int>> children = tac_cfg::dominator_tree_children(idom);
  const std::map<int, Names> live_in = compute_live_in(cfg, promotable);
  std::map<int, std::string> block_label_of;
  for (const auto& [bid, dom] : idom) {
    if (bid == kEntryId || bid == kExitId) {
      continue;
    }
    const BasicBlock& blk = *cfg.blocks.find(bid);
    if (!blk.instructions.empty()) {
      block_label_of[bid] = block_label(blk);
    }
  }

  const PhisAt phis_at = place_phis(cfg, promotable, df, live_in);

  Renamer r{.cfg = cfg,
            .promotable = promotable,
            .children = children,
            .block_label_of = block_label_of,
            .phis_at = phis_at,
            .symbols = symbols};
  for (const auto& v : promotable) {
    r.counters[v] = 0;
    r.stacks[v];
  }
  for (const auto& p : fn.params) {
    if (promotable.contains(p)) {
      r.stacks[p].push_back(p);
      r.ssa_dsts.insert(p);
    }
  }
  r.visit(kEntryId);
  return {tac_cfg::cfg_to_function(fn, cfg), std::move(r.ssa_dsts)};
}

}  // namespace c6502::passes
