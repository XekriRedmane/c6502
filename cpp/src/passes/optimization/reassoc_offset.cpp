#include "passes/optimization/reassoc_offset.h"

#include <map>
#include <set>
#include <string>
#include <utility>

#include "passes/optimization/strength_reduction.h"
#include "passes/optimization/var_visit.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::tac_ast;
namespace c99 = ::c6502::c99_ast;
using framework::PassContext;
using BK = ir::Kind_binary_operator;

std::optional<int> width(const PassContext& ctx, const ir::Type_val& v) {
  const auto* var = std::get_if<ir::Var>(&v);
  if (var == nullptr) {
    return std::nullopt;
  }
  const Symbol* sym = ctx.symbols->find(var->name);
  if (sym == nullptr) {
    return std::nullopt;
  }
  if (asdl::isa<c99::Pointer>(sym->type)) {
    return 2;
  }
  return strength_reduction::scalar_int_size(sym->type);
}

std::optional<std::pair<const ir::Type_val*, const ir::Type_val*>> split_const(
    const ir::Type_val& a, const ir::Type_val& b) {
  if (asdl::isa<ir::Constant>(a) && asdl::isa<ir::Var>(b)) {
    return std::make_pair(&a, &b);
  }
  if (asdl::isa<ir::Constant>(b) && asdl::isa<ir::Var>(a)) {
    return std::make_pair(&b, &a);
  }
  return std::nullopt;
}

void register_temp(const PassContext& ctx, const std::string& like, const std::string& name) {
  if (!ctx.symbols->contains(name)) {
    ctx.symbols->insert_or_assign(name,
                                  Symbol{.type = ctx.symbols->at(like).type, .attrs = LocalAttr{}});
  }
  ctx.ssa_dsts->insert(name);
}

}  // namespace

tac_ast::Function reassoc_offset_constant(tac_ast::Function fn, const PassContext& ctx) {
  if (ctx.ssa_dsts == nullptr || ctx.ssa_dsts->empty() || ctx.symbols == nullptr) {
    return fn;
  }
  const framework::Instrs& instrs = fn.instructions;
  std::map<std::string, std::size_t, std::less<>> defs;
  for (std::size_t i = 0; i < instrs.size(); ++i) {
    const auto d = framework::dst_name(instrs[i]);
    if (d && ctx.is_ssa(*d)) {
      defs.insert_or_assign(std::string(*d), i);
    }
  }
  const auto uses = count_uses(instrs);
  std::map<std::size_t, framework::Instrs> rewrites;
  std::set<std::size_t> dropped;
  for (std::size_t i = 0; i < instrs.size(); ++i) {
    const auto* instr = std::get_if<ir::Binary>(&instrs[i]);
    if (instr == nullptr || ir::kind(instr->op) != BK::Add) {
      continue;
    }
    const auto* dst = std::get_if<ir::Var>(&instr->dst);
    if (dst == nullptr || !ctx.is_ssa(dst->name)) {
      continue;
    }
    const std::pair<const ir::Type_val*, const ir::Type_val*> orders[] = {
        {&instr->src1, &instr->src2}, {&instr->src2, &instr->src1}};
    for (const auto& [p, off] : orders) {
      const auto* off_var = std::get_if<ir::Var>(off);
      if (!asdl::isa<ir::Var>(*p) || off_var == nullptr || !ctx.is_ssa(off_var->name)) {
        continue;
      }
      const std::size_t* n = uses.find(off_var->name);
      if (n == nullptr || *n != 1) {
        continue;
      }
      const auto j = defs.find(off_var->name);
      if (j == defs.end() || dropped.contains(j->second)) {
        continue;
      }
      const auto* inner = std::get_if<ir::Binary>(&instrs[j->second]);
      if (inner == nullptr || ir::kind(inner->op) != BK::Add) {
        continue;
      }
      const auto split = split_const(inner->src1, inner->src2);
      if (!split) {
        continue;
      }
      const auto& [k, x] = *split;
      const std::optional<int> w = width(ctx, instr->dst);
      if (!w || w != width(ctx, *p) || w != width(ctx, *off)) {
        continue;
      }
      if (width(ctx, *x) != w) {
        continue;
      }
      const std::string tmp = dst->name + ".ra";
      register_temp(ctx, dst->name, tmp);
      framework::Instrs rep;
      rep.push_back(ir::Binary{.op = ir::Add{}, .src1 = *p, .src2 = *x, .dst = ir::Var{tmp}});
      rep.push_back(ir::Binary{.op = ir::Add{}, .src1 = ir::Var{tmp}, .src2 = *k, .dst = *dst});
      rewrites.insert_or_assign(i, std::move(rep));
      dropped.insert(j->second);
      break;
    }
  }
  if (rewrites.empty()) {
    return fn;
  }
  framework::Instrs out;
  for (std::size_t i = 0; i < instrs.size(); ++i) {
    if (dropped.contains(i)) {
      continue;
    }
    const auto it = rewrites.find(i);
    if (it == rewrites.end()) {
      out.push_back(instrs[i]);
    } else {
      for (auto& r : it->second) {
        out.push_back(std::move(r));
      }
    }
  }
  return framework::with_instructions(fn, std::move(out));
}

}  // namespace c6502::passes
