#include "passes/optimization/narrow_byte_bitwise.h"

#include <cstddef>
#include <optional>
#include <string>
#include <utility>

#include "c99_ast.h"
#include "passes/optimization/cmp_zero_jump_fold.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::tac_ast;
namespace c99 = ::c6502::c99_ast;
using framework::DefUseEnv;
using framework::DefUseRewrite;
using framework::Instrs;
using framework::PassContext;
using BK = ir::Kind_binary_operator;
using CK = ir::Kind_const;
using DK = c99::Kind_data_type;

std::optional<int> width_of(const c99::Type_data_type& t) {
  const c99::Type_data_type* p = &t;
  for (;;) {
    if (const auto* c = std::get_if<c99::Const>(p)) {
      p = &*c->referenced_type;
    } else if (const auto* v = std::get_if<c99::Volatile>(p)) {
      p = &*v->referenced_type;
    } else {
      break;
    }
  }
  switch (c99::kind(*p)) {
    case DK::Char:
    case DK::SChar:
    case DK::UChar:
      return 1;
    case DK::Int:
    case DK::UInt:
    case DK::Pointer:
      return 2;
    case DK::Long:
    case DK::ULong:
      return 4;
    case DK::LongLong:
    case DK::ULongLong:
      return 8;
    default:
      return std::nullopt;
  }
}

bool is_volatile(const c99::Type_data_type& t) {
  const c99::Type_data_type* p = &t;
  while (const auto* c = std::get_if<c99::Const>(p)) {
    p = &*c->referenced_type;
  }
  return std::holds_alternative<c99::Volatile>(*p);
}

std::optional<asdl::Int> int_const_value(const ir::Type_const& c) {
  switch (ir::kind(c)) {
    case CK::ConstChar:
      return std::get<ir::ConstChar>(c).value;
    case CK::ConstUChar:
      return std::get<ir::ConstUChar>(c).value;
    case CK::ConstInt:
      return std::get<ir::ConstInt>(c).value;
    case CK::ConstUInt:
      return std::get<ir::ConstUInt>(c).value;
    case CK::ConstLong:
      return std::get<ir::ConstLong>(c).value;
    case CK::ConstULong:
      return std::get<ir::ConstULong>(c).value;
    case CK::ConstLongLong:
      return std::get<ir::ConstLongLong>(c).value;
    case CK::ConstULongLong:
      return std::get<ir::ConstULongLong>(c).value;
    default:
      return std::nullopt;
  }
}

struct Operand {
  std::optional<ir::Type_val> low;
  bool zero_high = false;
  bool extended = false;
};

std::optional<Operand> classify(const ir::Type_val& v, int width, const DefUseEnv& env,
                                const PassContext& ctx, std::ptrdiff_t use_idx) {
  if (const auto* c = std::get_if<ir::Constant>(&v)) {
    const std::optional<asdl::Int> value = int_const_value(c->const_);
    if (!value) {
      return std::nullopt;
    }
    return Operand{ir::Type_val{ir::Constant{.const_ = ir::ConstUChar{.value = *value & 0xFF}}},
                   *value >= 0 && *value <= 0xFF, false};
  }
  const auto* var = std::get_if<ir::Var>(&v);
  if (var == nullptr) {
    return std::nullopt;
  }
  const Symbol* sym = ctx.symbols->find(var->name);
  if (sym == nullptr || width_of(sym->type) != width || is_volatile(sym->type)) {
    return std::nullopt;
  }
  if (ctx.ssa_dsts->contains(var->name)) {
    const ir::Type_instruction* ext = env.def_of(*var);
    const ir::Type_val* src = nullptr;
    const ir::Type_val* dst = nullptr;
    bool zero = false;
    if (ext != nullptr) {
      if (const auto* z = std::get_if<ir::ZeroExtend>(ext)) {
        src = &z->src;
        dst = &z->dst;
        zero = true;
      } else if (const auto* s = std::get_if<ir::SignExtend>(ext)) {
        src = &s->src;
        dst = &s->dst;
      }
    }
    if (src != nullptr && std::holds_alternative<ir::Var>(*src)) {
      const Symbol* src_sym = ctx.symbols->find(std::get<ir::Var>(*src).name);
      if (src_sym != nullptr && width_of(src_sym->type) == 1 && !is_volatile(src_sym->type) &&
          extend_source_unchanged(*src, *dst,
                                  static_cast<std::ptrdiff_t>(env.def_idx.find(var->name)->second),
                                  use_idx, env.instructions, ctx.ssa_dsts)) {
        return Operand{*src, zero, true};
      }
    }
  }
  return Operand{std::nullopt, false, false};
}

ir::Var temp(const PassContext& ctx, const std::string& like, const std::string& tag) {
  int k = 0;
  while (ctx.symbols->contains(like + "." + tag + std::to_string(k))) {
    ++k;
  }
  const std::string name = like + "." + tag + std::to_string(k);
  ctx.symbols->insert_or_assign(name, Symbol{.type = c99::UChar{}, .attrs = LocalAttr{}});
  ctx.ssa_dsts->insert(name);
  return ir::Var{.name = name};
}

std::optional<DefUseRewrite> rewrite(const ir::Type_instruction& instr, const DefUseEnv& env,
                                     const PassContext& ctx, std::size_t at) {
  const auto* binary = std::get_if<ir::Binary>(&instr);
  if (binary == nullptr) {
    return std::nullopt;
  }
  const BK op = ir::kind(binary->op);
  if (op != BK::BitwiseAnd && op != BK::BitwiseOr && op != BK::BitwiseXor) {
    return std::nullopt;
  }
  const auto* dst = std::get_if<ir::Var>(&binary->dst);
  if (dst == nullptr) {
    return std::nullopt;
  }
  if (ctx.symbols == nullptr || ctx.ssa_dsts == nullptr) {
    return std::nullopt;
  }
  const Symbol* dst_sym = ctx.symbols->find(dst->name);
  if (dst_sym == nullptr) {
    return std::nullopt;
  }
  const std::optional<int> width = width_of(dst_sym->type);
  if (!width || *width == 1) {
    return std::nullopt;
  }
  const auto use_idx = static_cast<std::ptrdiff_t>(at);
  std::optional<Operand> ops[] = {classify(binary->src1, *width, env, ctx, use_idx),
                                  classify(binary->src2, *width, env, ctx, use_idx)};
  if (!ops[0] || !ops[1]) {
    return std::nullopt;
  }
  if (!ops[0]->extended && !ops[1]->extended) {
    return std::nullopt;
  }
  if (op == BK::BitwiseAnd) {
    if (!ops[0]->zero_high && !ops[1]->zero_high) {
      return std::nullopt;
    }
  } else if (!(ops[0]->zero_high && ops[1]->zero_high)) {
    return std::nullopt;
  }
  Instrs pre;
  std::vector<ir::Type_val> narrow;
  const ir::Type_val* vals[] = {&binary->src1, &binary->src2};
  for (int k = 0; k < 2; ++k) {
    if (ops[k]->low) {
      narrow.push_back(*ops[k]->low);
      continue;
    }
    const ir::Var t = temp(ctx, dst->name, "lo");
    pre.push_back(ir::Truncate{.src = *vals[k], .dst = t});
    narrow.push_back(t);
  }
  const ir::Var n = temp(ctx, dst->name, "n");
  DefUseRewrite r;
  r.is_rewrite = true;
  r.is_list = true;
  r.replacement = std::move(pre);
  r.replacement.push_back(ir::Binary{
      .op = binary->op, .src1 = std::move(narrow[0]), .src2 = std::move(narrow[1]), .dst = n});
  r.replacement.push_back(ir::ZeroExtend{.src = n, .dst = *dst});
  return r;
}

}  // namespace

ir::Function narrow_byte_bitwise(ir::Function fn, const PassContext& ctx) {
  std::size_t next = 0;
  return framework::run_def_use(std::move(fn), ctx,
                                [&](const ir::Type_instruction& instr, const DefUseEnv& env) {
                                  return rewrite(instr, env, ctx, next++);
                                });
}

}  // namespace c6502::passes
