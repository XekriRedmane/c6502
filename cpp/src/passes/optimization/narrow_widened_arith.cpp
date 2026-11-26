#include "passes/optimization/narrow_widened_arith.h"

#include <string>
#include <utility>

#include "bit_precise.h"
#include "errors.h"
#include "passes/optimization/var_visit.h"
#include "passes/shift_cost.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::tac_ast;
namespace c99 = ::c6502::c99_ast;
using K = ir::Kind_instruction;

bool is_volatile_type(const c99::Type_data_type& t) {
  const c99::Type_data_type* p = &t;
  while (const auto* c = std::get_if<c99::Const>(p)) {
    p = &*c->referenced_type;
  }
  return std::holds_alternative<c99::Volatile>(*p);
}

const c99::Type_data_type& strip(const c99::Type_data_type& t) {
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

bool ends_block_or_writes_memory(const ir::Type_instruction& instr) {
  switch (ir::kind(instr)) {
    case K::Label:
    case K::Ret:
    case K::Jump:
    case K::JumpIfTrue:
    case K::JumpIfFalse:
    case K::JumpIfCmp:
    case K::JumpIfMasked:
    case K::FunctionCall:
    case K::IndirectCall:
    case K::Store:
    case K::IndexedStore:
    case K::IndexedSymbolStore:
    case K::IndirectIndexedStore:
      return true;
    default:
      return false;
  }
}

}  // namespace

bool var_unchanged(const ir::Var& var, std::size_t start, std::size_t end,
                   const framework::DefUseEnv& env, const framework::PassContext& ctx) {
  if (ctx.ssa_dsts->contains(var.name)) {
    return true;
  }
  const Symbol* sym = ctx.symbols->find(var.name);
  if (sym == nullptr || is_volatile_type(sym->type) || start >= end) {
    return false;
  }
  for (std::size_t k = start + 1; k < end; ++k) {
    const auto& entry = env.instructions[k];
    if (!entry || ends_block_or_writes_memory(*entry)) {
      return false;
    }
    for (const auto name : defs_in(*entry)) {
      if (name == var.name) {
        return false;
      }
    }
  }
  return true;
}

}  // namespace c6502::passes

namespace c6502::passes::narrow_widened_arith {

namespace {
namespace ir = ::c6502::tac_ast;
namespace c99 = ::c6502::c99_ast;
using DK = c99::Kind_data_type;
}  // namespace

void register_temp(const framework::PassContext& ctx, std::string_view like,
                   const std::string& name) {
  if (!ctx.symbols->contains(name)) {
    const Symbol* src = ctx.symbols->find(like);
    if (src == nullptr) {
      throw InternalError("register_temp: no symbol for " + std::string(like));
    }
    c99::Type_data_type type = src->type;
    ctx.symbols->insert_or_assign(name, Symbol{.type = std::move(type), .attrs = LocalAttr{}});
  }
  ctx.ssa_dsts->insert(name);
}

std::optional<ir::Constant> narrow_constant(const ir::Constant& c, int width, bool is_unsigned) {
  const std::optional<asdl::Int> raw = bit_precise::raw_value(c.const_);
  if (!raw) {
    return std::nullopt;
  }
  if (width != 1 && width != 2 && width != 4 && width != 8) {
    return std::nullopt;
  }
  const asdl::Int modulus = asdl::Int(1) << (8 * width);
  const asdl::Int masked = asdl::floor_mod(*raw, modulus);
  const asdl::Int half = modulus >> 1;
  const asdl::Int as_signed = masked < half ? masked : masked - modulus;
  switch (width) {
    case 1:
      if (is_unsigned) {
        return ir::Constant{.const_ = ir::ConstUChar{.value = masked}};
      }
      return ir::Constant{.const_ = ir::ConstChar{.value = as_signed}};
    case 2:
      if (is_unsigned) {
        return ir::Constant{.const_ = ir::ConstUInt{.value = masked}};
      }
      return ir::Constant{.const_ = ir::ConstInt{.value = as_signed}};
    case 4:
      if (is_unsigned) {
        return ir::Constant{.const_ = ir::ConstULong{.value = masked}};
      }
      return ir::Constant{.const_ = ir::ConstLong{.value = as_signed}};
    default:
      if (is_unsigned) {
        return ir::Constant{.const_ = ir::ConstULongLong{.value = masked}};
      }
      return ir::Constant{.const_ = ir::ConstLongLong{.value = as_signed}};
  }
}

std::optional<int> byte_width(const ir::Var& v, const SymbolTable* symbols) {
  if (symbols == nullptr) {
    return std::nullopt;
  }
  const Symbol* sym = symbols->find(v.name);
  if (sym == nullptr) {
    return std::nullopt;
  }
  switch (c99::kind(strip(sym->type))) {
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
    case DK::Float:
      return 4;
    case DK::LongLong:
    case DK::ULongLong:
    case DK::Double:
      return 8;
    default:
      return std::nullopt;
  }
}

std::optional<bool> is_unsigned(const ir::Var& v, const SymbolTable* symbols) {
  if (symbols == nullptr) {
    return std::nullopt;
  }
  const Symbol* sym = symbols->find(v.name);
  if (sym == nullptr) {
    return std::nullopt;
  }
  switch (c99::kind(strip(sym->type))) {
    case DK::Char:
    case DK::UChar:
    case DK::UInt:
    case DK::Pointer:
    case DK::ULong:
    case DK::ULongLong:
      return true;
    case DK::SChar:
    case DK::Int:
    case DK::Long:
    case DK::LongLong:
      return false;
    default:
      return std::nullopt;
  }
}

}  // namespace c6502::passes::narrow_widened_arith

namespace c6502::passes {

namespace {

namespace ir = ::c6502::tac_ast;
using framework::DefUseEnv;
using framework::DefUseRewrite;
using framework::Instrs;
using framework::PassContext;
using BK = ir::Kind_binary_operator;
using UseCounts = OrderedMap<std::string, std::size_t>;
using narrow_widened_arith::byte_width;
using narrow_widened_arith::is_unsigned;

std::size_t count_of(const UseCounts& counts, std::string_view name) {
  const std::size_t* n = counts.find(name);
  return n == nullptr ? 0 : *n;
}

bool is_safe_op(const ir::Type_binary_operator& op) {
  switch (ir::kind(op)) {
    case BK::Add:
    case BK::Subtract:
    case BK::Multiply:
    case BK::BitwiseAnd:
    case BK::BitwiseOr:
    case BK::BitwiseXor:
      return true;
    default:
      return false;
  }
}

bool is_compare_op(const ir::Type_binary_operator& op) {
  switch (ir::kind(op)) {
    case BK::Equal:
    case BK::NotEqual:
    case BK::LessThan:
    case BK::LessOrEqual:
    case BK::GreaterThan:
    case BK::GreaterOrEqual:
      return true;
    default:
      return false;
  }
}

struct Run {
  const PassContext& ctx;
  UseCounts counts;
  std::size_t at = 0;
};

void count_new_uses(UseCounts& counts, const Instrs& replacement) {
  for (const auto& instr : replacement) {
    for (const auto name : uses_in(instr)) {
      counts[std::string(name)] += 1;
    }
  }
}

const ir::Var* extend_var_source(const ir::Type_instruction* instr) {
  if (instr == nullptr) {
    return nullptr;
  }
  if (const auto* z = std::get_if<ir::ZeroExtend>(instr)) {
    return std::get_if<ir::Var>(&z->src);
  }
  if (const auto* s = std::get_if<ir::SignExtend>(instr)) {
    return std::get_if<ir::Var>(&s->src);
  }
  return nullptr;
}

bool extended_shift_source(const ir::Type_val& src, int width, const DefUseEnv& env,
                           const PassContext& ctx) {
  const auto* v = std::get_if<ir::Var>(&src);
  if (v == nullptr || !ctx.ssa_dsts->contains(v->name)) {
    return false;
  }
  const ir::Type_instruction* cast = env.def_of(*v);
  const ir::Var* var = extend_var_source(cast);
  if (var == nullptr || byte_width(*var, ctx.symbols) != width) {
    return false;
  }
  const std::optional<bool> unsigned_src = is_unsigned(*var, ctx.symbols);
  if (!unsigned_src) {
    return false;
  }
  return *unsigned_src == std::holds_alternative<ir::ZeroExtend>(*cast);
}

std::optional<ir::Type_val> narrow_operand(const ir::Type_val& operand, int width,
                                           bool is_unsigned_dst, const DefUseEnv& env,
                                           std::size_t bin_idx, std::size_t trunc_idx,
                                           const std::string& tmp_name, const PassContext& ctx,
                                           Instrs& pre) {
  if (const auto* c = std::get_if<ir::Constant>(&operand)) {
    if (auto n = narrow_widened_arith::narrow_constant(*c, width, is_unsigned_dst)) {
      return ir::Type_val{std::move(*n)};
    }
    return std::nullopt;
  }
  const auto* v = std::get_if<ir::Var>(&operand);
  if (v == nullptr) {
    return std::nullopt;
  }
  if (ctx.ssa_dsts->contains(v->name)) {
    const auto it = env.def_idx.find(v->name);
    if (it != env.def_idx.end()) {
      const auto& entry = env.instructions[it->second];
      const ir::Var* src = extend_var_source(entry ? &*entry : nullptr);
      if (src != nullptr && byte_width(*src, ctx.symbols) == width) {
        if (!var_unchanged(*src, it->second, trunc_idx, env, ctx)) {
          return std::nullopt;
        }
        return ir::Type_val{*src};
      }
    }
  }
  const std::optional<int> w = byte_width(*v, ctx.symbols);
  if (!w || *w <= width || !is_unsigned(*v, ctx.symbols) ||
      !var_unchanged(*v, bin_idx, trunc_idx, env, ctx)) {
    return std::nullopt;
  }
  const ir::Var tmp{.name = tmp_name};
  pre.push_back(ir::Truncate{.src = operand, .dst = tmp});
  return ir::Type_val{tmp};
}

std::optional<DefUseRewrite> narrow_compare(const ir::Binary& bin, const ir::Var& trunc_src,
                                            const ir::Var& dst, std::size_t trunc_idx,
                                            const DefUseEnv& env, Run& run) {
  const PassContext& ctx = run.ctx;
  const auto* bin_dst = std::get_if<ir::Var>(&bin.dst);
  if (bin_dst == nullptr) {
    return std::nullopt;
  }
  const std::optional<int> dst_width = byte_width(dst, ctx.symbols);
  const std::optional<int> bin_width = byte_width(*bin_dst, ctx.symbols);
  if (!dst_width || !bin_width || *dst_width >= *bin_width) {
    return std::nullopt;
  }
  if (count_of(run.counts, bin_dst->name) != 1) {
    return std::nullopt;
  }
  const auto bin_at = env.def_idx.find(trunc_src.name);
  if (bin_at == env.def_idx.end()) {
    return std::nullopt;
  }
  for (const ir::Type_val* operand : {&bin.src1, &bin.src2}) {
    const auto* v = std::get_if<ir::Var>(operand);
    if (v != nullptr && !var_unchanged(*v, bin_at->second, trunc_idx, env, ctx)) {
      return std::nullopt;
    }
  }
  DefUseRewrite r = DefUseRewrite::dropping(
      ir::Binary{.op = bin.op, .src1 = bin.src1, .src2 = bin.src2, .dst = dst}, {bin_dst->name});
  count_new_uses(run.counts, r.replacement);
  return r;
}

std::optional<DefUseRewrite> rewrite(const ir::Type_instruction& instr, const DefUseEnv& env,
                                     Run& run) {
  const std::size_t trunc_idx = run.at++;
  const PassContext& ctx = run.ctx;
  const auto* trunc = std::get_if<ir::Truncate>(&instr);
  if (trunc == nullptr) {
    return std::nullopt;
  }
  const auto* trunc_src = std::get_if<ir::Var>(&trunc->src);
  const auto* dst = std::get_if<ir::Var>(&trunc->dst);
  if (trunc_src == nullptr || dst == nullptr) {
    return std::nullopt;
  }
  if (ctx.ssa_dsts == nullptr || ctx.symbols == nullptr) {
    return std::nullopt;
  }
  if (!ctx.ssa_dsts->contains(trunc_src->name)) {
    return std::nullopt;
  }
  const ir::Type_instruction* def = env.def_of(*trunc_src);
  const auto* bin = def ? std::get_if<ir::Binary>(def) : nullptr;
  if (bin == nullptr) {
    return std::nullopt;
  }
  if (is_compare_op(bin->op)) {
    return narrow_compare(*bin, *trunc_src, *dst, trunc_idx, env, run);
  }
  const BK op = ir::kind(bin->op);
  const bool is_shift = op == BK::LeftShift || op == BK::RightShift;
  if (!is_safe_op(bin->op) && !is_shift) {
    return std::nullopt;
  }
  const auto* bin_dst = std::get_if<ir::Var>(&bin->dst);
  if (bin_dst == nullptr) {
    return std::nullopt;
  }
  const std::optional<int> dst_width = byte_width(*dst, ctx.symbols);
  const std::optional<int> bin_width = byte_width(*bin_dst, ctx.symbols);
  if (!dst_width || !bin_width) {
    return std::nullopt;
  }
  if (*dst_width >= *bin_width) {
    return std::nullopt;
  }
  if (op == BK::RightShift && !extended_shift_source(bin->src1, *dst_width, env, ctx)) {
    return std::nullopt;
  }
  const std::optional<bool> dst_unsigned = is_unsigned(*dst, ctx.symbols);
  if (!dst_unsigned) {
    return std::nullopt;
  }
  if (op == BK::LeftShift && std::holds_alternative<ir::Var>(bin->src2)) {
  } else if (is_shift) {
    const auto* count = std::get_if<ir::Constant>(&bin->src2);
    const std::optional<asdl::Int> value =
        count != nullptr ? bit_precise::raw_value(count->const_) : std::nullopt;
    if (!value || *value < 0 || *value >= 8 * *dst_width) {
      return std::nullopt;
    }
    if (op == BK::RightShift && *value != 0) {
      const ir::Var* shifted = extend_var_source(env.def_of(std::get<ir::Var>(bin->src1)));
      const bool signed_right = is_unsigned(*shifted, ctx.symbols) != std::optional<bool>(true);
      const int n = static_cast<int>(*value);
      if (!inline_shift_ok(*dst_width, n, signed_right) &&
          inline_shift_ok(*bin_width, n, signed_right)) {
        return std::nullopt;
      }
    }
  }
  const auto bin_at = env.def_idx.find(trunc_src->name);
  if (bin_at == env.def_idx.end()) {
    return std::nullopt;
  }
  const std::size_t bin_idx = bin_at->second;
  Instrs pre;
  const std::optional<ir::Type_val> narrow_s1 =
      narrow_operand(bin->src1, *dst_width, *dst_unsigned, env, bin_idx, trunc_idx,
                     trunc_src->name + ".lo1", ctx, pre);
  if (!narrow_s1) {
    return std::nullopt;
  }
  const std::optional<ir::Type_val> narrow_s2 =
      narrow_operand(bin->src2, *dst_width, *dst_unsigned, env, bin_idx, trunc_idx,
                     trunc_src->name + ".lo2", ctx, pre);
  if (!narrow_s2) {
    return std::nullopt;
  }
  const bool wide_stays = count_of(run.counts, bin_dst->name) != 1;
  if (wide_stays && !pre.empty()) {
    return std::nullopt;
  }
  for (const auto& t : pre) {
    narrow_widened_arith::register_temp(ctx, dst->name,
                                        std::get<ir::Var>(std::get<ir::Truncate>(t).dst).name);
  }
  DefUseRewrite r;
  r.is_rewrite = true;
  r.is_list = !pre.empty();
  r.replacement = pre;
  r.replacement.push_back(
      ir::Binary{.op = bin->op, .src1 = *narrow_s1, .src2 = *narrow_s2, .dst = *dst});
  if (wide_stays) {
    count_new_uses(run.counts, r.replacement);
    return r;
  }
  r.drop_defs.push_back(bin_dst->name);
  for (const ir::Type_val* operand : {&bin->src1, &bin->src2}) {
    const auto* v = std::get_if<ir::Var>(operand);
    if (v == nullptr || !ctx.ssa_dsts->contains(v->name)) {
      continue;
    }
    const ir::Type_instruction* ext = env.def_of(*v);
    if (ext == nullptr || !(std::holds_alternative<ir::SignExtend>(*ext) ||
                            std::holds_alternative<ir::ZeroExtend>(*ext))) {
      continue;
    }
    bool read_by_pre = false;
    for (const auto& t : pre) {
      if (std::get<ir::Truncate>(t).src == *operand) {
        read_by_pre = true;
      }
    }
    if (read_by_pre) {
      continue;
    }
    if (count_of(run.counts, v->name) == 1) {
      r.drop_defs.push_back(v->name);
    }
  }
  count_new_uses(run.counts, r.replacement);
  return r;
}

}  // namespace

ir::Function apply_narrow_widened_arith(ir::Function fn, const PassContext& ctx) {
  Run run{ctx, count_uses(fn.instructions)};
  return framework::run_def_use(std::move(fn), ctx,
                                [&](const ir::Type_instruction& instr, const DefUseEnv& env) {
                                  return rewrite(instr, env, run);
                                });
}

}  // namespace c6502::passes
