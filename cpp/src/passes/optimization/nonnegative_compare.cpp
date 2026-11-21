/* A TAC rule that does a signed ordering compare of two values that can't
be negative as an unsigned one.

`row + 1 < WNDBTM`, with `row` and `WNDBTM` bytes, compares two `int`s,
and a signed compare of 2-byte values is a subtract and a test of the
overflow flag before the sign (`BVC; EOR #$80; BMI`). The values are
from 0 to 256 and from 0 to 255, and two values from 0 to a type's
largest value compare the same way as signed and as unsigned numbers,
so an unsigned compare, which branches on the carry, gives the same
result in 4 bytes less.

`value_range::max_value` bounds each operand. The rule runs last in the
jump-fold rule set (`fold_producer_jumps`), after the rules that narrow
a compare of extended bytes to a byte compare, and applies to a
`JumpIfCmp` and to a comparison kept as a value. */
#include "passes/optimization/nonnegative_compare.h"

#include <string>
#include <utility>

#include "bit_precise.h"
#include "c99_ast.h"
#include "passes/optimization/value_range.h"

namespace c6502::passes::nonnegative_compare {

namespace {

namespace ir = ::c6502::tac_ast;
namespace c99 = ::c6502::c99_ast;
using framework::Instrs;
using framework::PassContext;
using framework::RuleEnv;
using BK = ir::Kind_binary_operator;
using CK = ir::Kind_const;
using DK = c99::Kind_data_type;

// Returns whether `op` is `<`, `>`, `<=`, or `>=` (`_ORDERING_OPS`).
bool is_ordering_op(const ir::Type_binary_operator& op) {
  switch (ir::kind(op)) {
    case BK::LessThan:
    case BK::GreaterThan:
    case BK::LessOrEqual:
    case BK::GreaterOrEqual:
      return true;
    default:
      return false;
  }
}

// Returns the unsigned constant of the width of the signed integer
// constant `c` holding `c`'s value, or none for any other class
// (`_UNSIGNED_CONST`).
std::optional<ir::Type_const> unsigned_const(const ir::Type_const& c) {
  switch (ir::kind(c)) {
    case CK::ConstChar:
      return ir::ConstUChar{.value = std::get<ir::ConstChar>(c).value};
    case CK::ConstInt:
      return ir::ConstUInt{.value = std::get<ir::ConstInt>(c).value};
    case CK::ConstLong:
      return ir::ConstULong{.value = std::get<ir::ConstLong>(c).value};
    case CK::ConstLongLong:
      return ir::ConstULongLong{.value = std::get<ir::ConstLongLong>(c).value};
    default:
      return std::nullopt;
  }
}

// Returns the unsigned c99 type of the width of the signed integer type
// `t` as written, or none for any other type (`_UNSIGNED_TYPE`).
std::optional<c99::Type_data_type> unsigned_type(const c99::Type_data_type& t) {
  switch (c99::kind(t)) {
    case DK::SChar:
      return c99::UChar{};
    case DK::Int:
      return c99::UInt{};
    case DK::Long:
      return c99::ULong{};
    case DK::LongLong:
      return c99::ULongLong{};
    default:
      return std::nullopt;
  }
}

// Returns whether `v` is a constant of a signed integer class of
// `_UNSIGNED_CONST`, or a Var whose type is `int`, `long`, or `long
// long` as written (`signed_type`).
bool signed_type(const ir::Type_val& v, const PassContext& ctx) {
  if (const auto* c = std::get_if<ir::Constant>(&v)) {
    return unsigned_const(c->const_).has_value();
  }
  const auto* var = std::get_if<ir::Var>(&v);
  if (var == nullptr) {
    return false;
  }
  const Symbol* sym = ctx.symbols->find(var->name);
  if (sym == nullptr) {
    return false;
  }
  return unsigned_type(sym->type).has_value() && !std::holds_alternative<c99::SChar>(sym->type);
}

// The operands of a compare done unsigned, and the copies that make
// them.
struct Unsigned {
  Instrs pre;
  ir::Type_val a;
  ir::Type_val b;
};

// Returns the operands as unsigned values, with the copies that make
// them, for a signed ordering compare of `src1` and `src2` whose values
// `value_range::max_value` shows can't be negative, or none (Python's
// `_unsigned_operands`).
//
// Two values from 0 to the type's largest compare the same way as signed
// and as unsigned numbers, and an unsigned compare has no test of the
// overflow flag (`BVC; EOR #$80`). A constant becomes the unsigned
// constant of its width, which makes the compare unsigned by itself
// (`tac_to_asm._is_unsigned_val`), and when both are Vars the first is
// copied into a new unsigned temporary of its width, a copy that copy
// propagation keeps since it changes the signedness and that the asm
// optimizer's copy propagation removes. `row + 1 < WNDBTM` with byte
// operands is the motivating case.
//
// A compare with the constant 0 is left alone, for the sign-bit lowering
// of a signed compare with 0, and so is any operand of a type other than
// a signed integer of 2 bytes or more, since other rules narrow a byte
// compare to an unsigned one when they can.
std::optional<Unsigned> unsigned_operands(const ir::Type_val& src1, const ir::Type_val& src2,
                                          const RuleEnv& env, const PassContext& ctx) {
  if (ctx.symbols == nullptr || ctx.ssa_dsts == nullptr) {
    return std::nullopt;
  }
  const value_range::DefOf def_of = [&](const ir::Var& v) -> const ir::Type_instruction* {
    return env.def_of(ir::Type_val{v});
  };
  for (const ir::Type_val* v : {&src1, &src2}) {
    if (!signed_type(*v, ctx)) {
      return std::nullopt;
    }
    if (const auto* c = std::get_if<ir::Constant>(v)) {
      if (std::holds_alternative<ir::ConstChar>(c->const_)) {
        return std::nullopt;
      }
      if (*bit_precise::raw_value(c->const_) <= 0) {
        return std::nullopt;
      }
    } else if (!value_range::max_value(*v, def_of, ctx.symbols, ctx.ssa_dsts)) {
      return std::nullopt;
    }
  }
  const bool any_constant =
      std::holds_alternative<ir::Constant>(src1) || std::holds_alternative<ir::Constant>(src2);
  Unsigned out{.pre = {}, .a = src1, .b = src2};
  ir::Type_val* slots[] = {&out.a, &out.b};
  for (ir::Type_val* slot : slots) {
    if (const auto* c = std::get_if<ir::Constant>(slot)) {
      *slot = ir::Constant{.const_ = *unsigned_const(c->const_)};
    } else if (out.pre.empty() && !any_constant) {
      const std::string name = std::get<ir::Var>(*slot).name;
      int k = 0;
      while (ctx.symbols->contains(name + ".u" + std::to_string(k))) {
        ++k;
      }
      const std::string tmp = name + ".u" + std::to_string(k);
      c99::Type_data_type t = *unsigned_type(ctx.symbols->find(name)->type);
      ctx.symbols->insert_or_assign(tmp, Symbol{.type = std::move(t), .attrs = LocalAttr{}});
      ctx.ssa_dsts->insert(tmp);
      out.pre.push_back(ir::Copy{.src = *slot, .dst = ir::Var{.name = tmp}});
      *slot = ir::Var{.name = tmp};
    }
  }
  return out;
}

}  // namespace

// Python's `_build_nonnegative_compare`: a signed ordering compare of
// two values that can't be negative is done unsigned
// (`unsigned_operands`), as a `JumpIfCmp` or as a comparison kept as a
// value.
std::optional<Instrs> nonnegative_compare_at(const Instrs& instrs, std::size_t i, RuleEnv& env,
                                             const PassContext& ctx) {
  if (const auto* jmp = std::get_if<ir::JumpIfCmp>(&instrs[i])) {
    if (!is_ordering_op(jmp->op)) {
      return std::nullopt;
    }
    std::optional<Unsigned> got = unsigned_operands(jmp->src1, jmp->src2, env, ctx);
    if (!got) {
      return std::nullopt;
    }
    Instrs out = std::move(got->pre);
    out.push_back(ir::JumpIfCmp{
        .op = jmp->op, .src1 = std::move(got->a), .src2 = std::move(got->b), .target = jmp->target});
    return out;
  }
  const auto* bin = std::get_if<ir::Binary>(&instrs[i]);
  if (bin == nullptr || !is_ordering_op(bin->op)) {
    return std::nullopt;
  }
  std::optional<Unsigned> got = unsigned_operands(bin->src1, bin->src2, env, ctx);
  if (!got) {
    return std::nullopt;
  }
  Instrs out = std::move(got->pre);
  out.push_back(ir::Binary{
      .op = bin->op, .src1 = std::move(got->a), .src2 = std::move(got->b), .dst = bin->dst});
  return out;
}

}  // namespace c6502::passes::nonnegative_compare
