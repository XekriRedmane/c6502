#include "passes/optimization/cmp_zero_jump_fold.h"

#include <string>
#include <utility>

#include "bit_precise.h"
#include "passes/optimization/var_visit.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::tac_ast;
namespace c99 = ::c6502::c99_ast;
using framework::Instrs;
using framework::PassContext;
using framework::RuleEnv;
using K = ir::Kind_instruction;
using BK = ir::Kind_binary_operator;
using UseCount = OrderedMap<std::string, std::size_t>;
using DefIdx = std::map<std::string, std::size_t, std::less<>>;

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

bool is_pure_def(const ir::Type_instruction& instr) {
  switch (ir::kind(instr)) {
    case K::SignExtend:
    case K::ZeroExtend:
    case K::Truncate:
    case K::IntToFloat:
    case K::IntToDouble:
    case K::FloatToInt:
    case K::DoubleToInt:
    case K::FloatToDouble:
    case K::DoubleToFloat:
    case K::GetAddress:
    case K::Load:
    case K::IndexedLoad:
    case K::IndexedConstLoad:
    case K::IndirectIndexedLoad:
    case K::Unary:
    case K::Binary:
    case K::Copy:
      return true;
    default:
      return false;
  }
}

const ir::Type_instruction* entry(const Instrs& instrs, std::size_t k) { return &instrs[k]; }

const ir::Type_instruction* entry(const std::vector<std::optional<ir::Type_instruction>>& instrs,
                                  std::size_t k) {
  return instrs[k] ? &*instrs[k] : nullptr;
}

template <class List>
bool source_unchanged(const ir::Type_val& src, const ir::Type_val& dst, std::ptrdiff_t ext_idx,
                      std::ptrdiff_t use_idx, const List& instrs, const NameSet* ssa_dsts) {
  const auto* s = std::get_if<ir::Var>(&src);
  if (s == nullptr) {
    return false;
  }
  if (ssa_dsts != nullptr) {
    const auto* d = std::get_if<ir::Var>(&dst);
    if (d == nullptr || !ssa_dsts->contains(d->name)) {
      return false;
    }
    if (ssa_dsts->contains(s->name)) {
      return true;
    }
  }
  if (ext_idx >= use_idx) {
    return false;
  }
  for (std::ptrdiff_t k = ext_idx + 1; k < use_idx; ++k) {
    const ir::Type_instruction* ins = entry(instrs, static_cast<std::size_t>(k));
    if (ins == nullptr || !is_pure_def(*ins)) {
      return false;
    }
    for (const auto name : defs_in(*ins)) {
      if (name == s->name) {
        return false;
      }
    }
  }
  return true;
}

std::size_t count_of(const UseCount& use_count, std::string_view name) {
  const std::size_t* n = use_count.find(name);
  return n == nullptr ? 0 : *n;
}

std::optional<std::size_t> index_of(const DefIdx& def_idx, std::string_view name) {
  const auto it = def_idx.find(name);
  if (it == def_idx.end()) {
    return std::nullopt;
  }
  return it->second;
}

std::optional<asdl::Int> integer_value(const ir::Type_val& v) {
  const auto* c = std::get_if<ir::Constant>(&v);
  if (c == nullptr) {
    return std::nullopt;
  }
  return bit_precise::raw_value(c->const_);
}

bool narrowable_use(const ir::Var& val, const UseCount& use_count, const NameSet* ssa_dsts) {
  if (ssa_dsts != nullptr) {
    return ssa_dsts->contains(val.name);
  }
  return count_of(use_count, val.name) == 1;
}

struct Narrowed {
  ir::Var src;
  const c99::Type_data_type* type;
};

struct Scope {
  const Instrs& instrs;
  const DefIdx& def_idx;
  const UseCount& use_count;
  const SymbolTable* symbols;
  std::ptrdiff_t use_idx;
  const NameSet* ssa_dsts;
};

std::optional<Narrowed> narrow_through_zero_extend(const ir::Type_val& val, const Scope& s) {
  const auto* v = std::get_if<ir::Var>(&val);
  if (v == nullptr || !narrowable_use(*v, s.use_count, s.ssa_dsts)) {
    return std::nullopt;
  }
  std::optional<std::size_t> def_idx = index_of(s.def_idx, v->name);
  if (!def_idx) {
    return std::nullopt;
  }
  const ir::Type_instruction* defining = &s.instrs[*def_idx];
  int seen = 0;
  while (seen < 4) {
    const auto* copy = std::get_if<ir::Copy>(defining);
    if (copy == nullptr) {
      break;
    }
    const auto* src = std::get_if<ir::Var>(&copy->src);
    if (src == nullptr) {
      break;
    }
    ++seen;
    if (!narrowable_use(*src, s.use_count, s.ssa_dsts)) {
      return std::nullopt;
    }
    def_idx = index_of(s.def_idx, src->name);
    if (!def_idx) {
      return std::nullopt;
    }
    defining = &s.instrs[*def_idx];
  }
  const auto* ext = std::get_if<ir::ZeroExtend>(defining);
  if (ext == nullptr) {
    return std::nullopt;
  }
  const auto* src = std::get_if<ir::Var>(&ext->src);
  if (src == nullptr) {
    return std::nullopt;
  }
  if (!source_unchanged(ext->src, ext->dst, static_cast<std::ptrdiff_t>(*def_idx), s.use_idx,
                        s.instrs, s.ssa_dsts)) {
    return std::nullopt;
  }
  if (s.symbols == nullptr) {
    return std::nullopt;
  }
  const Symbol* sym = s.symbols->find(src->name);
  if (sym == nullptr) {
    return std::nullopt;
  }
  return Narrowed{*src, &sym->type};
}

std::optional<Narrowed> narrow_through_sign_extend(const ir::Type_val& val, const Scope& s) {
  const auto* v = std::get_if<ir::Var>(&val);
  if (v == nullptr || !narrowable_use(*v, s.use_count, s.ssa_dsts)) {
    return std::nullopt;
  }
  const std::optional<std::size_t> def_idx = index_of(s.def_idx, v->name);
  if (!def_idx) {
    return std::nullopt;
  }
  const auto* ext = std::get_if<ir::SignExtend>(&s.instrs[*def_idx]);
  if (ext == nullptr) {
    return std::nullopt;
  }
  const auto* src = std::get_if<ir::Var>(&ext->src);
  if (src == nullptr) {
    return std::nullopt;
  }
  if (!source_unchanged(ext->src, ext->dst, static_cast<std::ptrdiff_t>(*def_idx), s.use_idx,
                        s.instrs, s.ssa_dsts)) {
    return std::nullopt;
  }
  if (s.symbols == nullptr) {
    return std::nullopt;
  }
  const Symbol* sym = s.symbols->find(src->name);
  if (sym == nullptr) {
    return std::nullopt;
  }
  return Narrowed{*src, &sym->type};
}

std::optional<ir::Type_val> narrow_const_to_unsigned_byte(const ir::Type_val& val) {
  const std::optional<asdl::Int> c = integer_value(val);
  if (!c || *c < 0 || *c > 255) {
    return std::nullopt;
  }
  return ir::Constant{.const_ = ir::ConstUChar{.value = *c}};
}

std::optional<ir::Type_val> narrow_const_to_signed_byte(const ir::Type_val& val) {
  const std::optional<asdl::Int> c = integer_value(val);
  if (!c || *c < -128 || *c > 127) {
    return std::nullopt;
  }
  return ir::Constant{.const_ = ir::ConstChar{.value = *c}};
}

using Pair = std::pair<ir::Type_val, ir::Type_val>;

std::optional<Pair> try_narrow_compare(const ir::Type_val& src1, const ir::Type_val& src2,
                                       const Scope& s) {
  if (s.symbols == nullptr) {
    return std::nullopt;
  }
  const auto info1 = narrow_through_zero_extend(src1, s);
  const auto info2 = narrow_through_zero_extend(src2, s);
  const bool n1_ok = info1 && cmp_zero_jump_fold::is_narrow_unsigned_type(*info1->type);
  const bool n2_ok = info2 && cmp_zero_jump_fold::is_narrow_unsigned_type(*info2->type);
  if (n1_ok && n2_ok) {
    return Pair{info1->src, info2->src};
  }
  if (n1_ok) {
    if (auto nc = narrow_const_to_unsigned_byte(src2)) {
      return Pair{info1->src, std::move(*nc)};
    }
  }
  if (n2_ok) {
    if (auto nc = narrow_const_to_unsigned_byte(src1)) {
      return Pair{std::move(*nc), info2->src};
    }
  }
  return std::nullopt;
}

std::optional<Pair> try_narrow_signed_compare(const ir::Type_val& src1,
                                              const ir::Type_val& src2, const Scope& s) {
  if (s.symbols == nullptr) {
    return std::nullopt;
  }
  const auto info1 = narrow_through_sign_extend(src1, s);
  const auto info2 = narrow_through_sign_extend(src2, s);
  const bool n1_ok = info1 && std::holds_alternative<c99::SChar>(*info1->type);
  const bool n2_ok = info2 && std::holds_alternative<c99::SChar>(*info2->type);
  if (n1_ok && n2_ok) {
    return Pair{info1->src, info2->src};
  }
  if (n1_ok) {
    if (auto nc = narrow_const_to_signed_byte(src2)) {
      return Pair{info1->src, std::move(*nc)};
    }
  }
  if (n2_ok) {
    if (auto nc = narrow_const_to_signed_byte(src1)) {
      return Pair{std::move(*nc), info2->src};
    }
  }
  return std::nullopt;
}

bool is_constant_zero(const ir::Type_val& val) {
  const std::optional<asdl::Int> c = integer_value(val);
  return c && *c == 0;
}

const ir::Type_val* zero_compare_other_operand(const ir::Type_val& a, const ir::Type_val& b) {
  const bool a_zero = is_constant_zero(a);
  const bool b_zero = is_constant_zero(b);
  if (a_zero && !b_zero) {
    return &b;
  }
  if (b_zero && !a_zero) {
    return &a;
  }
  return nullptr;
}

ir::Type_val trace_through_zero_extend(ir::Type_val x, const Scope& s) {
  for (;;) {
    const auto* v = std::get_if<ir::Var>(&x);
    if (v == nullptr || !narrowable_use(*v, s.use_count, s.ssa_dsts)) {
      break;
    }
    const std::optional<std::size_t> def_idx = index_of(s.def_idx, v->name);
    if (!def_idx) {
      break;
    }
    const auto* ext = std::get_if<ir::ZeroExtend>(&s.instrs[*def_idx]);
    if (ext == nullptr) {
      break;
    }
    if (!source_unchanged(ext->src, ext->dst, static_cast<std::ptrdiff_t>(*def_idx), s.use_idx,
                          s.instrs, s.ssa_dsts)) {
      break;
    }
    ir::Type_val next = ext->src;
    x = std::move(next);
  }
  return x;
}

ir::Type_binary_operator adjusted_op_for_jumpif(const ir::Type_binary_operator& op,
                                                bool jump_if_true) {
  if (jump_if_true) {
    return op;
  }
  switch (ir::kind(op)) {
    case BK::Equal:
      return ir::NotEqual{};
    case BK::NotEqual:
      return ir::Equal{};
    case BK::LessThan:
      return ir::GreaterOrEqual{};
    case BK::GreaterOrEqual:
      return ir::LessThan{};
    case BK::GreaterThan:
      return ir::LessOrEqual{};
    case BK::LessOrEqual:
      return ir::GreaterThan{};
    default:
      throw std::logic_error("adjusted_op_for_jumpif: not a comparison");
  }
}

ir::Type_instruction replacement_jump(bool is_equal, bool jump_if_true, ir::Type_val x,
                                      const std::string& target) {
  const bool new_is_true = is_equal ? !jump_if_true : jump_if_true;
  if (new_is_true) {
    return ir::JumpIfTrue{.condition = std::move(x), .target = target};
  }
  return ir::JumpIfFalse{.condition = std::move(x), .target = target};
}

struct WindowJump {
  bool jump_if_true;
  const std::string* target;
};

std::optional<WindowJump> jump_on(const ir::Type_instruction& instr, const ir::Var& on) {
  if (const auto* t = std::get_if<ir::JumpIfTrue>(&instr)) {
    const auto* c = std::get_if<ir::Var>(&t->condition);
    if (c != nullptr && c->name == on.name) {
      return WindowJump{true, &t->target};
    }
    return std::nullopt;
  }
  if (const auto* f = std::get_if<ir::JumpIfFalse>(&instr)) {
    const auto* c = std::get_if<ir::Var>(&f->condition);
    if (c != nullptr && c->name == on.name) {
      return WindowJump{false, &f->target};
    }
  }
  return std::nullopt;
}

}  // namespace

bool extend_source_unchanged(const ir::Type_val& src, const ir::Type_val& dst,
                             std::ptrdiff_t ext_idx, std::ptrdiff_t use_idx,
                             const Instrs& instrs, const NameSet* ssa_dsts) {
  return source_unchanged(src, dst, ext_idx, use_idx, instrs, ssa_dsts);
}

bool extend_source_unchanged(const ir::Type_val& src, const ir::Type_val& dst,
                             std::ptrdiff_t ext_idx, std::ptrdiff_t use_idx,
                             const std::vector<std::optional<ir::Type_instruction>>& instrs,
                             const NameSet* ssa_dsts) {
  return source_unchanged(src, dst, ext_idx, use_idx, instrs, ssa_dsts);
}

namespace cmp_zero_jump_fold {

bool is_narrow_unsigned_type(const c99::Type_data_type& t) {
  return std::holds_alternative<c99::Char>(t) || std::holds_alternative<c99::UChar>(t);
}

std::optional<Instrs> fold_cmp_zero_jump_at(const Instrs& instrs, std::size_t i, RuleEnv& env,
                                            const PassContext& ctx) {
  const auto* binop = std::get_if<ir::Binary>(&instrs[i]);
  if (binop == nullptr || !is_compare_op(binop->op)) {
    return std::nullopt;
  }
  const auto* cond = std::get_if<ir::Var>(&binop->dst);
  if (cond == nullptr) {
    return std::nullopt;
  }
  const std::optional<WindowJump> jmp = jump_on(instrs[i + 1], *cond);
  if (!jmp || !framework::single_use(binop->dst, env, ctx)) {
    return std::nullopt;
  }
  std::ptrdiff_t use_idx = static_cast<std::ptrdiff_t>(env.def_idx.at(cond->name));
  if (use_idx != static_cast<std::ptrdiff_t>(i)) {
    use_idx = -1;
  }
  const Scope s{instrs, env.def_idx, env.use_count, ctx.symbols, use_idx, ctx.ssa_dsts};

  const BK op = ir::kind(binop->op);
  if (op == BK::Equal || op == BK::NotEqual) {
    if (const ir::Type_val* x = zero_compare_other_operand(binop->src1, binop->src2)) {
      return Instrs{replacement_jump(op == BK::Equal, jmp->jump_if_true,
                                     trace_through_zero_extend(*x, s), *jmp->target)};
    }
  }
  ir::Type_val src1 = binop->src1;
  ir::Type_val src2 = binop->src2;
  if (auto narrowed = try_narrow_compare(src1, src2, s)) {
    src1 = std::move(narrowed->first);
    src2 = std::move(narrowed->second);
  } else if (auto signed_pair = try_narrow_signed_compare(src1, src2, s)) {
    src1 = std::move(signed_pair->first);
    src2 = std::move(signed_pair->second);
  }
  return Instrs{ir::JumpIfCmp{.op = adjusted_op_for_jumpif(binop->op, jmp->jump_if_true),
                              .src1 = std::move(src1),
                              .src2 = std::move(src2),
                              .target = *jmp->target}};
}

std::optional<Instrs> narrow_value_compare_at(const Instrs& instrs, std::size_t i, RuleEnv& env,
                                              const PassContext& ctx) {
  const auto* binop = std::get_if<ir::Binary>(&instrs[i]);
  if (binop == nullptr || !is_compare_op(binop->op)) {
    return std::nullopt;
  }
  const auto* dst = std::get_if<ir::Var>(&binop->dst);
  if (dst == nullptr) {
    return std::nullopt;
  }
  const std::optional<std::size_t> at = index_of(env.def_idx, dst->name);
  const std::ptrdiff_t use_idx = at && *at == i ? static_cast<std::ptrdiff_t>(i) : -1;
  const Scope s{instrs, env.def_idx, env.use_count, ctx.symbols, use_idx, ctx.ssa_dsts};
  auto narrowed = try_narrow_compare(binop->src1, binop->src2, s);
  if (!narrowed) {
    return std::nullopt;
  }
  return Instrs{ir::Binary{.op = binop->op,
                           .src1 = std::move(narrowed->first),
                           .src2 = std::move(narrowed->second),
                           .dst = binop->dst}};
}

std::optional<Instrs> narrow_jump_compare_at(const Instrs& instrs, std::size_t i, RuleEnv& env,
                                             const PassContext& ctx) {
  const auto* jmp = std::get_if<ir::JumpIfCmp>(&instrs[i]);
  if (jmp == nullptr || !is_compare_op(jmp->op)) {
    return std::nullopt;
  }
  const Scope s{instrs,      env.def_idx, env.use_count, ctx.symbols, static_cast<std::ptrdiff_t>(i),
                ctx.ssa_dsts};
  auto narrowed = try_narrow_compare(jmp->src1, jmp->src2, s);
  if (!narrowed) {
    return std::nullopt;
  }
  return Instrs{ir::JumpIfCmp{.op = jmp->op,
                              .src1 = std::move(narrowed->first),
                              .src2 = std::move(narrowed->second),
                              .target = jmp->target}};
}

}  // namespace cmp_zero_jump_fold

}  // namespace c6502::passes
