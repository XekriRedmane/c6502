#include "passes/optimization/compare_subtract.h"

#include <cstddef>
#include <optional>
#include <string>
#include <string_view>
#include <tuple>
#include <vector>

#include "asdl/runtime.h"
#include "passes/optimization/cfg.h"
#include "passes/optimization/var_visit.h"

namespace c6502::passes {

namespace compare_subtract {

namespace tac = ::c6502::tac_ast;

std::pair<const tac::Type_val*, const tac::Type_val*> compared_difference(
    const tac::Type_binary_operator& op, const tac::Type_val& src1, const tac::Type_val& src2) {
  if (asdl::isa<tac::LessThan, tac::GreaterOrEqual>(op)) {
    return {&src1, &src2};
  }
  return {&src2, &src1};
}

}  // namespace compare_subtract

namespace {

namespace ir = ::c6502::tac_ast;
namespace c99 = ::c6502::c99_ast;
using framework::Instrs;
using framework::PassContext;
using IK = ir::Kind_instruction;
using tac_cfg::Cfg;

const c99::Type_data_type& strip(const c99::Type_data_type& t) {
  const c99::Type_data_type* p = &t;
  while (true) {
    if (const auto* c = std::get_if<c99::Const>(p)) {
      p = &*c->referenced_type;
    } else if (const auto* v = std::get_if<c99::Volatile>(p)) {
      p = &*v->referenced_type;
    } else {
      return *p;
    }
  }
}

bool volatile_symbol(const Symbol* sym) {
  if (sym == nullptr) {
    return true;
  }
  const c99::Type_data_type* t = &sym->type;
  while (true) {
    if (std::holds_alternative<c99::Volatile>(*t)) {
      return true;
    }
    if (const auto* c = std::get_if<c99::Const>(t)) {
      t = &*c->referenced_type;
    } else {
      return false;
    }
  }
}

bool one_byte(const c99::Type_data_type& t) {
  return asdl::isa<c99::Char, c99::SChar, c99::UChar>(t);
}

bool integer(const c99::Type_data_type& t) {
  return asdl::isa<c99::Char, c99::SChar, c99::UChar, c99::Int, c99::UInt, c99::Long, c99::ULong,
                   c99::LongLong, c99::ULongLong, c99::Pointer>(t);
}

bool ordering(const ir::Type_binary_operator& op) {
  return asdl::isa<ir::LessThan, ir::GreaterOrEqual, ir::GreaterThan, ir::LessOrEqual>(op);
}

struct Read {
  IK kind;
  const ir::Type_val* src;
  const ir::Type_val* dst;
};

std::optional<Read> as_read(const ir::Type_instruction& ins) {
  switch (ir::kind(ins)) {
    case IK::ZeroExtend: {
      const auto& i = std::get<ir::ZeroExtend>(ins);
      return Read{IK::ZeroExtend, &i.src, &i.dst};
    }
    case IK::SignExtend: {
      const auto& i = std::get<ir::SignExtend>(ins);
      return Read{IK::SignExtend, &i.src, &i.dst};
    }
    case IK::Truncate: {
      const auto& i = std::get<ir::Truncate>(ins);
      return Read{IK::Truncate, &i.src, &i.dst};
    }
    case IK::Copy: {
      const auto& i = std::get<ir::Copy>(ins);
      return Read{IK::Copy, &i.src, &i.dst};
    }
    default:
      return std::nullopt;
  }
}

ir::Type_instruction make_read(IK kind, ir::Type_val src, ir::Type_val dst) {
  switch (kind) {
    case IK::ZeroExtend:
      return ir::ZeroExtend{.src = std::move(src), .dst = std::move(dst)};
    case IK::SignExtend:
      return ir::SignExtend{.src = std::move(src), .dst = std::move(dst)};
    case IK::Truncate:
      return ir::Truncate{.src = std::move(src), .dst = std::move(dst)};
    default:
      return ir::Copy{.src = std::move(src), .dst = std::move(dst)};
  }
}

tac_cfg::BasicBlock& block_of(Cfg& cfg, int id) {
  return *cfg.blocks.find(id);
}

bool is_ssa_var(const ir::Type_val& v, const PassContext& ctx) {
  const auto* var = std::get_if<ir::Var>(&v);
  return var != nullptr && ctx.is_ssa(var->name);
}

bool writes_memory(const ir::Type_instruction& instr, const PassContext& ctx) {
  switch (ir::kind(instr)) {
    case IK::ZeroExtend:
    case IK::SignExtend:
    case IK::Truncate:
    case IK::Copy:
    case IK::Binary:
    case IK::Unary:
    case IK::Label:
    case IK::Phi:
    case IK::GetAddress:
    case IK::JumpIfCmp:
      break;
    case IK::Load:
      if (std::get<ir::Load>(instr).is_volatile) {
        return true;
      }
      break;
    case IK::IndexedLoad:
      if (std::get<ir::IndexedLoad>(instr).is_volatile) {
        return true;
      }
      break;
    case IK::IndexedConstLoad:
      if (std::get<ir::IndexedConstLoad>(instr).is_volatile) {
        return true;
      }
      break;
    case IK::IndirectIndexedLoad:
      if (std::get<ir::IndirectIndexedLoad>(instr).is_volatile) {
        return true;
      }
      break;
    default:
      return true;
  }
  const auto dst = framework::dst_name(instr);
  return dst && !ctx.is_ssa(*dst);
}

bool any_writes_memory(const Instrs& instrs, std::size_t from, std::size_t to,
                       const PassContext& ctx) {
  for (std::size_t i = from; i < to; ++i) {
    if (writes_memory(instrs[i], ctx)) {
      return true;
    }
  }
  return false;
}

const c99::Type_data_type* type_of(const ir::Type_val& v, const PassContext& ctx) {
  const auto* var = std::get_if<ir::Var>(&v);
  if (var == nullptr) {
    return nullptr;
  }
  const Symbol* sym = ctx.symbols->find(var->name);
  return sym == nullptr ? nullptr : &strip(sym->type);
}

bool stable(const ir::Type_val& v, const PassContext& ctx) {
  if (const auto* c = std::get_if<ir::Constant>(&v)) {
    return !asdl::isa<ir::ConstFloat, ir::ConstDouble>(c->const_);
  }
  return is_ssa_var(v, ctx);
}

struct MemoryRead {
  IK kind;
  std::string variable;
  const c99::Type_data_type* type;

  bool operator==(const MemoryRead& o) const {
    if (kind != o.kind || variable != o.variable) {
      return false;
    }
    if (type == nullptr || o.type == nullptr) {
      return type == o.type;
    }
    return *type == *o.type;
  }
};

std::optional<MemoryRead> memory_read(const ir::Type_val& v, const Instrs& instrs, std::size_t end,
                                      const PassContext& ctx) {
  if (!is_ssa_var(v, ctx)) {
    return std::nullopt;
  }
  for (std::size_t k = 0; k < end; ++k) {
    const auto read = as_read(instrs[k]);
    if (!read || !(*read->dst == v)) {
      continue;
    }
    const auto* src = std::get_if<ir::Var>(read->src);
    if (src == nullptr || ctx.is_ssa(src->name) || volatile_symbol(ctx.symbols->find(src->name)) ||
        any_writes_memory(instrs, k + 1, end, ctx)) {
      return std::nullopt;
    }
    return MemoryRead{read->kind, src->name, type_of(v, ctx)};
  }
  return std::nullopt;
}

const ir::Type_val* matching(const ir::Type_val& want, const ir::Type_val& have, const Instrs& head,
                             std::size_t head_end, const Instrs& prefix, std::size_t prefix_end,
                             const PassContext& ctx) {
  if (want == have) {
    return &want;
  }
  const auto key = memory_read(want, head, head_end, ctx);
  if (key && key == memory_read(have, prefix, prefix_end, ctx)) {
    return &want;
  }
  return nullptr;
}

bool hoist_from(Instrs& body, Instrs& instrs, const ir::Type_val& a, const ir::Type_val& b,
                const c99::Type_data_type& t, const PassContext& ctx) {
  const Instrs head(instrs.begin(), instrs.end() - 1);
  for (std::size_t k = 0; k < body.size(); ++k) {
    const auto* ins = std::get_if<ir::Binary>(&body[k]);
    if (ins == nullptr || !std::holds_alternative<ir::Subtract>(ins->op) ||
        !is_ssa_var(ins->dst, ctx)) {
      continue;
    }
    const auto* dst_type = type_of(ins->dst, ctx);
    if (dst_type == nullptr || !(*dst_type == t)) {
      continue;
    }
    const ir::Type_val* src1 = matching(a, ins->src1, head, head.size(), body, k, ctx);
    const ir::Type_val* src2 = matching(b, ins->src2, head, head.size(), body, k, ctx);
    if (src1 == nullptr || src2 == nullptr) {
      continue;
    }
    ir::Binary moved{.op = ins->op, .src1 = *src1, .src2 = *src2, .dst = ins->dst};
    body.erase(body.begin() + static_cast<std::ptrdiff_t>(k));
    instrs.insert(instrs.end() - 1, std::move(moved));
    return true;
  }
  return false;
}

std::string fresh_exit_name(const std::string& base, const PassContext& ctx) {
  std::string name = base + ".exit";
  int n = 0;
  while (ctx.symbols->contains(name)) {
    ++n;
    name = base + ".exit" + std::to_string(n);
  }
  return name;
}

std::optional<ir::Type_instruction> reread(const ir::Type_val& b, const Instrs& head,
                                           const PassContext& ctx) {
  const auto* bv = std::get_if<ir::Var>(&b);
  if (bv == nullptr || !ctx.is_ssa(bv->name)) {
    return std::nullopt;
  }
  for (std::size_t k = 0; k < head.size(); ++k) {
    const auto read = as_read(head[k]);
    if (!read || !(*read->dst == b)) {
      continue;
    }
    const auto* src = std::get_if<ir::Var>(read->src);
    if (src == nullptr || ctx.is_ssa(src->name) || volatile_symbol(ctx.symbols->find(src->name)) ||
        any_writes_memory(head, k + 1, head.size(), ctx)) {
      return std::nullopt;
    }
    const std::string name = fresh_exit_name(bv->name, ctx);
    const Symbol sym = ctx.symbols->at(bv->name);
    ctx.symbols->insert_or_assign(name, sym);
    ctx.ssa_dsts->insert(name);
    return make_read(read->kind, *read->src, ir::Var{.name = name});
  }
  return std::nullopt;
}

bool recompute_on_exit(Cfg& cfg, int bid, int succ, const ir::Type_val& a, const ir::Binary& sub,
                       const PassContext& ctx) {
  const auto* av = std::get_if<ir::Var>(&a);
  if (av == nullptr || !ctx.is_ssa(av->name)) {
    return false;
  }
  {
    const auto& instrs = block_of(cfg, bid).instructions;
    bool phi = false;
    for (const auto& i : instrs) {
      if (const auto* p = std::get_if<ir::Phi>(&i); p != nullptr && p->dst == a) {
        phi = true;
        break;
      }
    }
    if (!phi) {
      return false;
    }
  }
  std::vector<int> others;
  for (const int s : block_of(cfg, bid).successors) {
    if (s != succ) {
      others.push_back(s);
    }
  }
  if (others.size() != 1 || others[0] == tac_cfg::kEntryId || others[0] == tac_cfg::kExitId) {
    return false;
  }
  const int exit_id = others[0];
  if (block_of(cfg, exit_id).predecessors != std::vector<int>{bid}) {
    return false;
  }
  const auto idom = tac_cfg::immediate_dominators(cfg);

  auto under_exit = [&](int x) {
    while (true) {
      if (x == exit_id) {
        return true;
      }
      const int* up = idom.find(x);
      if (up == nullptr || *up == x) {
        return false;
      }
      x = *up;
    }
  };

  OrderedMap<std::string, int> label_block;
  for (const int k : cfg.block_order) {
    const auto& instrs = block_of(cfg, k).instructions;
    if (!instrs.empty()) {
      if (const auto* l = std::get_if<ir::Label>(&instrs[0])) {
        label_block.insert_or_assign(l->name, k);
      }
    }
  }

  auto phi_arg_side = [&](const std::string& arg_pred) {
    const int* pred = label_block.find(arg_pred);
    return pred != nullptr && under_exit(*pred);
  };

  bool exit_reads = false;
  for (const int k : cfg.block_order) {
    for (const auto& ins : block_of(cfg, k).instructions) {
      if (const auto* phi = std::get_if<ir::Phi>(&ins)) {
        for (const auto& phi_arg : phi->args) {
          const auto& arg = std::get<ir::PhiArg>(phi_arg);
          if (arg.source == a) {
            if (!phi_arg_side(arg.pred_label)) {
              return false;
            }
            exit_reads = true;
          }
        }
        continue;
      }
      bool reads = false;
      for (const auto name : uses_in(ins)) {
        if (name == av->name) {
          reads = true;
          break;
        }
      }
      if (reads) {
        if (under_exit(k)) {
          exit_reads = true;
        } else if (k != bid) {
          return false;
        }
      }
    }
  }
  if (!exit_reads) {
    return false;
  }
  const Symbol* sym = ctx.symbols->find(av->name);
  if (sym == nullptr) {
    return false;
  }
  const std::string name = fresh_exit_name(av->name, ctx);
  const ir::Type_val old = ir::Var{.name = name};
  const Symbol copy = *sym;
  ctx.symbols->insert_or_assign(name, copy);
  ctx.ssa_dsts->insert(name);

  const ValMapper mapper = [&](const ir::Type_val& v) -> std::optional<ir::Type_val> {
    if (v == a) {
      return old;
    }
    return std::nullopt;
  };

  for (const int k : cfg.block_order) {
    auto& instrs = block_of(cfg, k).instructions;
    for (auto& ins : instrs) {
      if (auto* phi = std::get_if<ir::Phi>(&ins)) {
        for (auto& phi_arg : phi->args) {
          auto& arg = std::get<ir::PhiArg>(phi_arg);
          if (arg.source == a && phi_arg_side(arg.pred_label)) {
            arg.source = old;
          }
        }
      } else if (under_exit(k)) {
        if (auto rewritten = rewrite_uses_in(ins, mapper)) {
          ins = std::move(*rewritten);
        }
      }
    }
  }
  auto& instrs = block_of(cfg, exit_id).instructions;
  std::size_t at = 0;
  while (at < instrs.size() && asdl::isa<ir::Label, ir::Phi>(instrs[at])) {
    ++at;
  }
  ir::Type_val b = sub.src2;
  if (auto again = reread(b, block_of(cfg, bid).instructions, ctx)) {
    b = ir::Var{.name = std::string(*framework::dst_name(*again))};
    instrs.insert(instrs.begin() + static_cast<std::ptrdiff_t>(at), std::move(*again));
    ++at;
  }
  instrs.insert(instrs.begin() + static_cast<std::ptrdiff_t>(at),
                ir::Binary{.op = ir::Add{}, .src1 = sub.dst, .src2 = std::move(b), .dst = old});
  return true;
}

}  // namespace

ir::Function hoist_subtract_to_compare(ir::Function fn, const PassContext& ctx) {
  if (ctx.ssa_dsts == nullptr || ctx.ssa_dsts->empty() || ctx.symbols == nullptr) {
    return fn;
  }
  Cfg cfg = tac_cfg::build_cfg(fn);
  bool changed = false;
  std::vector<std::tuple<int, int, ir::Type_val, ir::Binary>> hoisted;
  for (const int bid : cfg.block_order) {
    auto& block = block_of(cfg, bid);
    if (block.instructions.empty()) {
      continue;
    }
    const auto* last = std::get_if<ir::JumpIfCmp>(&block.instructions.back());
    if (last == nullptr || !ordering(last->op)) {
      continue;
    }
    const auto [pa, pb] = compare_subtract::compared_difference(last->op, last->src1, last->src2);
    const ir::Type_val a = *pa;
    const ir::Type_val b = *pb;
    if (!stable(a, ctx) || !stable(b, ctx)) {
      continue;
    }
    const c99::Type_data_type* t = type_of(a, ctx);
    if (t == nullptr) {
      t = type_of(b, ctx);
    }
    if (t == nullptr || !integer(*t) || one_byte(*t)) {
      continue;
    }
    const c99::Type_data_type type = *t;
    const std::vector<int> successors = block.successors;
    for (const int s : successors) {
      if (s == tac_cfg::kEntryId || s == tac_cfg::kExitId) {
        continue;
      }
      auto& target = block_of(cfg, s);
      if (target.predecessors != std::vector<int>{bid}) {
        continue;
      }
      auto& instrs = block_of(cfg, bid).instructions;
      if (hoist_from(target.instructions, instrs, a, b, type, ctx)) {
        changed = true;
        hoisted.emplace_back(bid, s, a, std::get<ir::Binary>(instrs[instrs.size() - 2]));
        break;
      }
    }
  }
  if (!changed) {
    return fn;
  }
  for (const auto& [bid, succ, a, sub] : hoisted) {
    recompute_on_exit(cfg, bid, succ, a, sub, ctx);
  }
  return tac_cfg::cfg_to_function(fn, cfg);
}

}  // namespace c6502::passes
