#include "passes/optimization/lnot_jump_fold.h"

#include <string>
#include <utility>

#include "c99_ast.h"
#include "passes/optimization/value_range.h"

namespace c6502::passes::lnot_jump_fold {

namespace {

namespace ir = ::c6502::tac_ast;
namespace c99 = ::c6502::c99_ast;
using framework::Instrs;
using framework::PassContext;
using framework::RuleEnv;
using BK = ir::Kind_binary_operator;

bool is_compare_op(const ir::Type_binary_operator& op) {
  switch (ir::kind(op)) {
    case BK::Equal:
    case BK::NotEqual:
    case BK::LessThan:
    case BK::GreaterThan:
    case BK::LessOrEqual:
    case BK::GreaterOrEqual:
      return true;
    default:
      return false;
  }
}

bool zero_or_one(const ir::Type_val& val, const RuleEnv& env, const PassContext& ctx,
                 int depth = 0) {
  const auto* v = std::get_if<ir::Var>(&val);
  if (v == nullptr || ctx.symbols == nullptr) {
    return false;
  }
  const Symbol* sym = ctx.symbols->find(v->name);
  if (sym == nullptr) {
    return false;
  }
  if (const auto* local = std::get_if<LocalAttr>(&sym->attrs); local != nullptr && local->was_bool) {
    return true;
  }
  if (const auto* st = std::get_if<StaticAttr>(&sym->attrs); st != nullptr && st->was_bool) {
    return true;
  }
  if (depth > 4 || !ctx.is_ssa(v->name)) {
    return false;
  }
  const ir::Type_instruction* ins = env.def_of(val);
  if (ins == nullptr) {
    return false;
  }
  if (const auto* b = std::get_if<ir::Binary>(ins)) {
    return is_compare_op(b->op);
  }
  if (const auto* u = std::get_if<ir::Unary>(ins)) {
    return std::holds_alternative<ir::LogicalNot>(u->op);
  }
  if (const auto* z = std::get_if<ir::ZeroExtend>(ins)) {
    return zero_or_one(z->src, env, ctx, depth + 1);
  }
  if (const auto* c = std::get_if<ir::Copy>(ins)) {
    return zero_or_one(c->src, env, ctx, depth + 1);
  }
  return false;
}

const c99::Type_data_type& byte_type(const c99::Type_data_type& t) {
  const c99::Type_data_type* p = &t;
  for (;;) {
    if (const auto* c = std::get_if<c99::Const>(p)) {
      p = &*c->referenced_type;
    } else if (const auto* v = std::get_if<c99::Volatile>(p)) {
      p = &*v->referenced_type;
    } else {
      return *p;
    }
  }
}

std::optional<ir::Type_val> one_for(const c99::Type_data_type& t) {
  switch (c99::kind(t)) {
    case c99::Kind_data_type::UChar:
    case c99::Kind_data_type::Char:
      return ir::Constant{.const_ = ir::ConstUChar{.value = 1}};
    case c99::Kind_data_type::SChar:
      return ir::Constant{.const_ = ir::ConstChar{.value = 1}};
    case c99::Kind_data_type::Int:
      return ir::Constant{.const_ = ir::ConstInt{.value = 1}};
    case c99::Kind_data_type::UInt:
      return ir::Constant{.const_ = ir::ConstUInt{.value = 1}};
    default:
      return std::nullopt;
  }
}

}  // namespace

std::optional<Instrs> fold_lnot_jump_at(const Instrs& instrs, std::size_t i, RuleEnv& env,
                                        const PassContext& ctx) {
  const auto* unary = std::get_if<ir::Unary>(&instrs[i]);
  if (unary == nullptr || !std::holds_alternative<ir::LogicalNot>(unary->op)) {
    return std::nullopt;
  }
  const auto* not_dst = std::get_if<ir::Var>(&unary->dst);
  if (not_dst == nullptr) {
    return std::nullopt;
  }
  const ir::Type_val dst_val = *not_dst;
  if (const auto* t = std::get_if<ir::JumpIfTrue>(&instrs[i + 1])) {
    if (t->condition == dst_val && framework::single_use(unary->dst, env, ctx)) {
      return Instrs{ir::JumpIfFalse{.condition = unary->src, .target = t->target}};
    }
  } else if (const auto* f = std::get_if<ir::JumpIfFalse>(&instrs[i + 1])) {
    if (f->condition == dst_val && framework::single_use(unary->dst, env, ctx)) {
      return Instrs{ir::JumpIfTrue{.condition = unary->src, .target = f->target}};
    }
  }
  return std::nullopt;
}

std::optional<Instrs> lnot_of_zero_or_one_at(const Instrs& instrs, std::size_t i, RuleEnv& env,
                                             const PassContext& ctx) {
  const auto* unary = std::get_if<ir::Unary>(&instrs[i]);
  if (unary == nullptr || !std::holds_alternative<ir::LogicalNot>(unary->op)) {
    return std::nullopt;
  }
  const auto* dst = std::get_if<ir::Var>(&unary->dst);
  if (dst == nullptr || ctx.symbols == nullptr) {
    return std::nullopt;
  }
  if (!zero_or_one(unary->src, env, ctx)) {
    return std::nullopt;
  }
  const std::size_t* uses = env.use_count.find(dst->name);
  if (uses != nullptr && *uses == 1) {
    const ir::Type_val dst_val = *dst;
    for (const auto& ins : *env.instructions) {
      if (const auto* t = std::get_if<ir::JumpIfTrue>(&ins); t != nullptr && t->condition == dst_val) {
        return std::nullopt;
      }
      if (const auto* f = std::get_if<ir::JumpIfFalse>(&ins); f != nullptr && f->condition == dst_val) {
        return std::nullopt;
      }
    }
  }
  const ir::Var& src = std::get<ir::Var>(unary->src);
  const c99::Type_data_type s_t = byte_type(ctx.symbols->find(src.name)->type);
  const Symbol* d_sym = ctx.symbols->find(dst->name);
  if (d_sym == nullptr) {
    return std::nullopt;
  }
  const c99::Type_data_type d_type = d_sym->type;
  const std::optional<int> s_w = value_range::int_width(s_t);
  const std::optional<int> d_w = value_range::int_width(d_type);
  if (!s_w || !d_w) {
    return std::nullopt;
  }
  if (*s_w == *d_w) {
    std::optional<ir::Type_val> one = one_for(byte_type(d_type));
    if (!one) {
      return std::nullopt;
    }
    return Instrs{ir::Binary{
        .op = ir::BitwiseXor{}, .src1 = unary->src, .src2 = std::move(*one), .dst = unary->dst}};
  }
  const bool extend = *s_w < *d_w;
  const c99::Type_data_type t_type = extend ? s_t : byte_type(d_type);
  std::optional<ir::Type_val> one = one_for(t_type);
  if (!one) {
    return std::nullopt;
  }
  int k = 0;
  while (ctx.symbols->contains(dst->name + ".not" + std::to_string(k))) {
    ++k;
  }
  const ir::Var tmp{.name = dst->name + ".not" + std::to_string(k)};
  ctx.symbols->insert_or_assign(tmp.name, Symbol{.type = t_type, .attrs = LocalAttr{}});
  if (ctx.ssa_dsts != nullptr) {
    ctx.ssa_dsts->insert(tmp.name);
  }
  if (extend) {
    return Instrs{
        ir::Binary{.op = ir::BitwiseXor{}, .src1 = unary->src, .src2 = std::move(*one), .dst = tmp},
        ir::ZeroExtend{.src = tmp, .dst = unary->dst}};
  }
  return Instrs{
      ir::Truncate{.src = unary->src, .dst = tmp},
      ir::Binary{.op = ir::BitwiseXor{}, .src1 = tmp, .src2 = std::move(*one), .dst = unary->dst}};
}

}  // namespace c6502::passes::lnot_jump_fold
