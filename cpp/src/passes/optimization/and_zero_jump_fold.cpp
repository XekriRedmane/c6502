#include "passes/optimization/and_zero_jump_fold.h"

#include <string>
#include <utility>

#include "passes/optimization/cmp_zero_jump_fold.h"

namespace c6502::passes::and_zero_jump_fold {

namespace {

namespace ir = ::c6502::tac_ast;
namespace c99 = ::c6502::c99_ast;
using framework::Instrs;
using framework::PassContext;
using framework::RuleEnv;
using CK = ir::Kind_const;

std::optional<asdl::Int> wide_int_value(const ir::Type_val& v) {
  const auto* c = std::get_if<ir::Constant>(&v);
  if (c == nullptr) {
    return std::nullopt;
  }
  switch (ir::kind(c->const_)) {
    case CK::ConstInt:
      return std::get<ir::ConstInt>(c->const_).value;
    case CK::ConstUInt:
      return std::get<ir::ConstUInt>(c->const_).value;
    case CK::ConstLong:
      return std::get<ir::ConstLong>(c->const_).value;
    case CK::ConstULong:
      return std::get<ir::ConstULong>(c->const_).value;
    case CK::ConstLongLong:
      return std::get<ir::ConstLongLong>(c->const_).value;
    case CK::ConstULongLong:
      return std::get<ir::ConstULongLong>(c->const_).value;
    default:
      return std::nullopt;
  }
}

std::optional<asdl::Int> mask_value(const ir::Type_val& v) {
  const auto* c = std::get_if<ir::Constant>(&v);
  if (c == nullptr) {
    return std::nullopt;
  }
  switch (ir::kind(c->const_)) {
    case CK::ConstChar:
      return std::get<ir::ConstChar>(c->const_).value;
    case CK::ConstUChar:
      return std::get<ir::ConstUChar>(c->const_).value;
    default:
      return wide_int_value(v);
  }
}

int bit_length(asdl::Int v) {
  int n = 0;
  while (v > 0) {
    v >>= 1;
    ++n;
  }
  return n;
}

struct OneByteMask {
  const ir::Var* val;
  int k;
  asdl::Int mask;
};

std::optional<OneByteMask> one_byte_mask(const ir::Binary& binop) {
  const std::pair<const ir::Type_val*, const ir::Type_val*> orders[] = {
      {&binop.src1, &binop.src2}, {&binop.src2, &binop.src1}};
  for (const auto& [val, cst] : orders) {
    const auto* var = std::get_if<ir::Var>(val);
    const std::optional<asdl::Int> c = wide_int_value(*cst);
    if (var == nullptr || !c) {
      continue;
    }
    if (*c <= 0) {
      continue;
    }
    const int k = (bit_length(*c) - 1) / 8;
    const asdl::Int low_mask = (asdl::Int(1) << (8 * k)) - 1;
    if ((*c >> (8 * k)) > 0xFF || (*c & low_mask) != 0) {
      continue;
    }
    return OneByteMask{var, k, *c >> (8 * k)};
  }
  return std::nullopt;
}

std::optional<Instrs> try_wide_byte(const ir::Binary& binop, const PassContext& ctx) {
  const std::optional<OneByteMask> found = one_byte_mask(binop);
  if (!found || ctx.symbols == nullptr) {
    return std::nullopt;
  }
  const Symbol* sym = ctx.symbols->find(found->val->name);
  if (sym == nullptr) {
    return std::nullopt;
  }
  int size = 0;
  switch (c99::kind(sym->type)) {
    case c99::Kind_data_type::Int:
    case c99::Kind_data_type::UInt:
      size = 2;
      break;
    case c99::Kind_data_type::Long:
    case c99::Kind_data_type::ULong:
      size = 4;
      break;
    case c99::Kind_data_type::LongLong:
    case c99::Kind_data_type::ULongLong:
      size = 8;
      break;
    default:
      return std::nullopt;
  }
  if (found->k >= size) {
    return std::nullopt;
  }
  const c99::Type_data_type val_type = sym->type;
  const std::string base = std::get<ir::Var>(binop.dst).name;
  Instrs out;
  ir::Type_val src = *found->val;
  if (found->k != 0) {
    ir::Var shifted{.name = base + ".byte"};
    ctx.symbols->insert_or_assign(shifted.name, Symbol{.type = val_type, .attrs = LocalAttr{}});
    out.push_back(ir::Binary{
        .op = ir::RightShift{},
        .src1 = *found->val,
        .src2 = ir::Constant{.const_ = ir::ConstInt{.value = asdl::Int(8 * found->k)}},
        .dst = shifted});
    src = shifted;
  }
  ir::Var byte{.name = base + ".b"};
  ctx.symbols->insert_or_assign(byte.name, Symbol{.type = c99::UChar{}, .attrs = LocalAttr{}});
  out.push_back(ir::Truncate{.src = std::move(src), .dst = byte});
  if (ctx.ssa_dsts != nullptr) {
    if (found->k != 0) {
      ctx.ssa_dsts->insert(base + ".byte");
    }
    ctx.ssa_dsts->insert(byte.name);
  }
  return out;
}

std::optional<std::pair<ir::Var, asdl::Int>> try_narrow_pair(const ir::Type_val& maybe_narrow,
                                                            const ir::Type_val& maybe_const,
                                                            const RuleEnv& env,
                                                            const PassContext& ctx,
                                                            std::ptrdiff_t use_idx) {
  const std::optional<asdl::Int> c = mask_value(maybe_const);
  if (!c || *c < 0 || *c > 0xFF) {
    return std::nullopt;
  }
  const auto* v = std::get_if<ir::Var>(&maybe_narrow);
  if (v == nullptr) {
    return std::nullopt;
  }
  const std::size_t* uses = env.use_count.find(v->name);
  if (uses == nullptr || *uses != 1) {
    return std::nullopt;
  }
  const auto at = env.def_idx.find(v->name);
  if (at == env.def_idx.end()) {
    return std::nullopt;
  }
  const auto* ext = std::get_if<ir::ZeroExtend>(&(*env.instructions)[at->second]);
  if (ext == nullptr) {
    return std::nullopt;
  }
  const auto* src = std::get_if<ir::Var>(&ext->src);
  if (src == nullptr) {
    return std::nullopt;
  }
  const Symbol* sym = ctx.symbols->find(src->name);
  if (sym == nullptr || !cmp_zero_jump_fold::is_narrow_unsigned_type(sym->type)) {
    return std::nullopt;
  }
  if (!extend_source_unchanged(ext->src, ext->dst, static_cast<std::ptrdiff_t>(at->second),
                               use_idx, *env.instructions, ctx.ssa_dsts)) {
    return std::nullopt;
  }
  return std::make_pair(*src, *c);
}

}  // namespace

std::optional<Instrs> fold_narrow_and_jump_at(const Instrs& instrs, std::size_t i, RuleEnv& env,
                                              const PassContext& ctx) {
  const auto* binop = std::get_if<ir::Binary>(&instrs[i]);
  if (binop == nullptr || !std::holds_alternative<ir::BitwiseAnd>(binop->op)) {
    return std::nullopt;
  }
  const auto* and_dst = std::get_if<ir::Var>(&binop->dst);
  if (and_dst == nullptr) {
    return std::nullopt;
  }
  bool jump_if_true = false;
  const std::string* target = nullptr;
  if (const auto* t = std::get_if<ir::JumpIfTrue>(&instrs[i + 1])) {
    if (t->condition == ir::Type_val{*and_dst}) {
      jump_if_true = true;
      target = &t->target;
    }
  } else if (const auto* f = std::get_if<ir::JumpIfFalse>(&instrs[i + 1])) {
    if (f->condition == ir::Type_val{*and_dst}) {
      target = &f->target;
    }
  }
  if (target == nullptr || !framework::single_use(binop->dst, env, ctx) ||
      ctx.symbols == nullptr) {
    return std::nullopt;
  }
  std::ptrdiff_t use_idx = static_cast<std::ptrdiff_t>(env.def_idx.at(and_dst->name));
  if (use_idx != static_cast<std::ptrdiff_t>(i)) {
    use_idx = -1;
  }
  auto narrow = try_narrow_pair(binop->src1, binop->src2, env, ctx, use_idx);
  if (!narrow) {
    narrow = try_narrow_pair(binop->src2, binop->src1, env, ctx, use_idx);
  }
  if (!narrow) {
    std::optional<Instrs> wide = try_wide_byte(*binop, ctx);
    if (!wide) {
      return std::nullopt;
    }
    const ir::Type_val val = std::get<ir::Truncate>(wide->back()).dst;
    wide->push_back(ir::JumpIfMasked{.val = val,
                                     .mask = one_byte_mask(*binop)->mask,
                                     .jump_when_nonzero = jump_if_true,
                                     .target = *target});
    return wide;
  }
  return Instrs{ir::JumpIfMasked{.val = std::move(narrow->first),
                                 .mask = narrow->second,
                                 .jump_when_nonzero = jump_if_true,
                                 .target = *target}};
}

}  // namespace c6502::passes::and_zero_jump_fold
