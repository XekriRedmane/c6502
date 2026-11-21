/* TAC strength reduction.

The pass rewrites a multiply, divide, or remainder by a power of two,
and a multiply by a few powers of two, into shifts and simpler
operations.

  * `Multiply(x, 2^k)` and `Multiply(2^k, x)` become `LeftShift(x, k)`,
    whatever the signedness, since a left shift by k multiplies by 2^k
    with the same wrap-around for signed and unsigned values.
  * `Multiply(x, 1)` and `Multiply(1, x)` become `Copy(x, dst)`.
  * `Multiply(x, 2^a ± 2^b)` becomes two shifts and an Add or a
    Subtract, when `shift_cost` says both shifts are inlined
    (`two_term_multiply`).
  * A one-byte `Multiply(x, C)` whose constant is three signed powers of
    two, such as 69 = 64 + 4 + 1, becomes three shifts of A and two Adds
    or Subtracts (`three_term_byte_multiply`).
  * `Multiply(x, 0)` isn't handled here. Constant folding takes the case
    of two constants, and `var * 0` is left, since copy propagation and
    constant folding usually take it later.
  * An unsigned `Divide(x, 2^k)` becomes `RightShift(x, k)`. A signed one
    is rewritten only when `x` can't be negative (`nonnegative`), since
    C99 §6.5.5.6 truncates toward zero and an arithmetic right shift
    rounds toward negative infinity, which give different quotients
    for a negative dividend: `-3 / 2 == -1` and `-3 >> 1 == -2`.
  * `Divide(x, 1)` becomes `Copy(x, dst)` for either signedness.
  * An unsigned `Modulo(x, 2^k)` becomes `BitwiseAnd(x, 2^k - 1)`. A
    signed one is rewritten only for an `x` that can't be negative, for
    the same reason, since the sign of the result is the dividend's and
    an AND can't be negative.
  * `Modulo(x, 1)` becomes a `Copy` of the zero of x's class, which for
    a Var comes from the symbol table.

This matters on the 6502 because `Multiply` lowers to a call of a
runtime helper (`mul8`, `mul16`, `mul32`), and every value live across
the call takes a callee-saved zero-page color, which the prologue and
epilogue save and restore. With the asm-level inline shift by one in
`tac_to_asm`, a multiply by a power of two calls nothing, so lifetimes
across calls shrink and the prologue can collapse further.

The rewrite is structural, with no evaluation beyond the test of the
constant, so it is safe before or after constant folding. The
three rules of Python's `RuleSet`, for `Multiply`, `Divide`, and
`Modulo`, match disjoint operators, so the port tries the one that
matches. */
#include "passes/optimization/strength_reduction.h"

#include <string>
#include <utility>
#include <vector>

#include "bit_precise.h"
#include "passes/optimization/value_range.h"
#include "passes/shift_cost.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::tac_ast;
namespace c99 = ::c6502::c99_ast;
using framework::PassContext;
using framework::RuleEnv;
using BK = ir::Kind_binary_operator;
using CK = ir::Kind_const;
using TK = c99::Kind_data_type;

// Returns the constant of `v` when it is a Constant of one of the eight
// integer classes of 1, 2, 4, and 8 bytes (Python's `_INT_CONST_TYPES`),
// or null.
const ir::Type_const* int_const(const ir::Type_val& v) {
  const auto* c = std::get_if<ir::Constant>(&v);
  if (c == nullptr) {
    return nullptr;
  }
  switch (ir::kind(c->const_)) {
    case CK::ConstChar:
    case CK::ConstUChar:
    case CK::ConstInt:
    case CK::ConstLong:
    case CK::ConstLongLong:
    case CK::ConstUInt:
    case CK::ConstULong:
    case CK::ConstULongLong:
      return &c->const_;
    default:
      return nullptr;
  }
}

asdl::Int raw(const ir::Type_const& c) { return *bit_precise::raw_value(c); }

int bit_length(asdl::Int v) {
  int n = 0;
  for (; v > 0; v >>= 1) {
    ++n;
  }
  return n;
}

// Returns `(log2(value), value)` when `val` is a positive integer
// Constant whose value is a power of two (1, 2, 4, 8, ...), or none
// (Python's `_power_of_two_const`). 1 is included, so that a multiply
// or a divide by 1 becomes a Copy. A signed class may hold a negative
// value, and a multiplier of -2^k isn't taken.
std::optional<std::pair<int, asdl::Int>> power_of_two_const(const ir::Type_val& val) {
  const ir::Type_const* c = int_const(val);
  if (c == nullptr) {
    return std::nullopt;
  }
  const asdl::Int v = raw(*c);
  if (v <= 0 || (v & (v - 1)) != 0) {
    return std::nullopt;
  }
  return std::make_pair(bit_length(v) - 1, v);
}

// Returns the count of a `LeftShift` or `RightShift` the rules write.
// The count's type is its own promoted type (C99 §6.5.7.3), whatever the
// value's, and tac_to_asm reads only its low byte, so it is a `ConstInt`.
ir::Type_val shift_count(int k) { return ir::Constant{.const_ = ir::ConstInt{k}}; }

// Returns `<base>.sr<k>` for the first k no symbol has (Python's
// `_fresh_name`). A global keeps its name in SSA form, so two multiplies
// into it would otherwise make the same temporary.
std::string fresh_name(const PassContext& ctx, const std::string& base) {
  int k = 0;
  while (ctx.symbols->contains(base + ".sr" + std::to_string(k))) {
    ++k;
  }
  return base + ".sr" + std::to_string(k);
}

// Registers `name` as a local with `like`'s type, unless it has a
// symbol, and as an SSA name in SSA form.
void register_temp(const PassContext& ctx, const std::string& like, const std::string& name) {
  if (!ctx.symbols->contains(name)) {
    ctx.symbols->insert_or_assign(name,
                                  Symbol{.type = ctx.symbols->at(like).type, .attrs = LocalAttr{}});
  }
  if (ctx.ssa_dsts != nullptr) {
    ctx.ssa_dsts->insert(name);
  }
}

// Returns a new temporary named after `dst`, registered.
ir::Var temp(const PassContext& ctx, const std::string& dst) {
  ir::Var t{.name = fresh_name(ctx, dst)};
  register_temp(ctx, dst, t.name);
  return t;
}

ir::Type_instruction binary(ir::Type_binary_operator op, ir::Type_val a, ir::Type_val b,
                            ir::Type_val dst) {
  return ir::Binary{.op = std::move(op), .src1 = std::move(a), .src2 = std::move(b),
                    .dst = std::move(dst)};
}

ir::Type_binary_operator add_or_subtract(bool subtract) {
  return subtract ? ir::Type_binary_operator{ir::Subtract{}} : ir::Type_binary_operator{ir::Add{}};
}

// Returns the nonzero digits `(position, ±1)` of `value`'s non-adjacent
// form, lowest first. 69 is [(0, 1), (2, 1), (6, 1)], and 7 is [(0, -1),
// (3, 1)].
std::vector<std::pair<int, int>> non_adjacent_form(asdl::Int value) {
  std::vector<std::pair<int, int>> out;
  int k = 0;
  while (value != 0) {
    if ((value & 1) != 0) {
      const int d = 2 - static_cast<int>(value & 3);
      out.emplace_back(k, d);
      value -= d;
    }
    value >>= 1;
    ++k;
  }
  return out;
}

// Rewrites a one-byte `Multiply(x, C)` whose constant modulo 256 is three
// signed powers of two, `2^a ± 2^b ± 2^c` with a > b > c, as
//
//     t = x << c;  u = t << (b - c);  v = u << (a - b)
//     w = v ± u;   dst = w ± t
//
// Ali Baba's PRNG step, `69 * s + $53`, had called `mul8`, which takes
// the two operands in HARGS and loops over eight bits. The shifts and
// adds, `ASL; ASL; STA t; ASL` four times and two `CLC; ADC`, are 4 or 5
// bytes longer than the call's 11, and take about 30 cycles against the
// helper's few hundred. The original game computes its PRNG step with
// shifts for the same reason. The digits are the constant's non-adjacent
// form, which has the fewest nonzero ones. A digit at bit 8 or above
// adds a multiple of 256, which is 0 in a byte, so it's dropped, and the
// top digit left must be +1.
std::optional<framework::Instrs> three_term_byte_multiply(const ir::Var& x,
                                                          const ir::Type_val& const_side,
                                                          const ir::Type_val& dst,
                                                          const PassContext& ctx) {
  const ir::Type_const* c = int_const(const_side);
  if (c == nullptr) {
    return std::nullopt;
  }
  const auto* dst_var = std::get_if<ir::Var>(&dst);
  if (ctx.symbols == nullptr || dst_var == nullptr) {
    return std::nullopt;
  }
  const Symbol* dst_sym = ctx.symbols->find(dst_var->name);
  if (dst_sym == nullptr || strength_reduction::scalar_int_size(dst_sym->type) != 1) {
    return std::nullopt;
  }
  std::vector<std::pair<int, int>> digits;
  for (const auto& d : non_adjacent_form(raw(*c) & 0xFF)) {
    if (d.first < 8) {
      digits.push_back(d);
    }
  }
  if (digits.size() != 3 || digits.back().second != 1) {
    return std::nullopt;
  }
  const int s0 = digits[0].second;
  const int s1 = digits[1].second;
  framework::Instrs out;
  std::vector<ir::Var> terms;
  ir::Var prev = x;
  int prev_k = 0;
  for (const auto& [k, sign] : digits) {
    if (k == prev_k) {
      terms.push_back(prev);
      continue;
    }
    ir::Var t = temp(ctx, dst_var->name);
    out.push_back(binary(ir::LeftShift{}, prev, shift_count(k - prev_k), t));
    terms.push_back(t);
    prev = std::move(t);
    prev_k = k;
  }
  const ir::Var w = temp(ctx, dst_var->name);
  out.push_back(binary(add_or_subtract(s1 < 0), terms[2], terms[1], w));
  out.push_back(binary(add_or_subtract(s0 < 0), w, terms[0], dst));
  return out;
}

// Rewrites `Multiply(x, C)` with `C = 2^a + 2^b` or `C = 2^a - 2^b` (a >
// b >= 0) into shifts and one Add or Subtract:
//
//     C = 2^a ± 1:     t = x << a;  dst = t ± x
//     C = 2^a ± 2^b:   t = x << b;  u = t << (a-b);  dst = u ± t
//
// Both forms wrap as the multiply does, modulo 2^(8n) for an n-byte
// destination, for signed and unsigned alike. The rewrite is made only
// when `inline_shift_ok` takes every shift at the destination's width,
// so the multiply helper is never traded for a shift helper. `x * 9` on
// a 16-bit int becomes `(x << 3) + x`, which is 6 shift instructions
// and a 16-bit add where it was a `JSR mul16`. The temporaries are named
// after `dst` and registered with its type, and are SSA names, so later
// passes treat them like any other.
std::optional<framework::Instrs> two_term_multiply(const ir::Var& x,
                                                   const ir::Type_val& const_side,
                                                   const ir::Type_val& dst,
                                                   const PassContext& ctx) {
  const ir::Type_const* c = int_const(const_side);
  if (c == nullptr || raw(*c) <= 0) {
    return std::nullopt;
  }
  const auto* dst_var = std::get_if<ir::Var>(&dst);
  if (ctx.symbols == nullptr || dst_var == nullptr) {
    return std::nullopt;
  }
  const Symbol* dst_sym = ctx.symbols->find(dst_var->name);
  if (dst_sym == nullptr) {
    return std::nullopt;
  }
  const std::optional<int> size = strength_reduction::scalar_int_size(dst_sym->type);
  if (!size) {
    return std::nullopt;
  }
  const auto terms = strength_reduction::two_power_terms(raw(*c));
  if (!terms) {
    return std::nullopt;
  }
  std::vector<int> shifts;
  if (terms->b == 0) {
    shifts = {terms->a};
  } else {
    shifts = {terms->b, terms->a - terms->b};
  }
  for (const int k : shifts) {
    if (!inline_shift_ok(*size, k)) {
      return std::nullopt;
    }
  }
  framework::Instrs out;
  const ir::Var t = temp(ctx, dst_var->name);
  out.push_back(binary(ir::LeftShift{}, x, shift_count(shifts[0]), t));
  if (terms->b == 0) {
    out.push_back(binary(add_or_subtract(terms->subtract), t, x, dst));
    return out;
  }
  const ir::Var u = temp(ctx, dst_var->name);
  out.push_back(binary(ir::LeftShift{}, t, shift_count(shifts[1]), u));
  out.push_back(binary(add_or_subtract(terms->subtract), u, t, dst));
  return out;
}

// Python's `_build_multiply`: `Multiply(x, 2^k)` becomes `LeftShift(x,
// k)`, and `Multiply(x, 1)` a Copy, with the constant on either side.
// Two constants are left to constant folding.
std::optional<framework::Instrs> build_multiply(const ir::Binary& b, const PassContext& ctx) {
  if (asdl::isa<ir::Constant>(b.src1) && asdl::isa<ir::Constant>(b.src2)) {
    return std::nullopt;
  }
  const std::pair<const ir::Type_val*, const ir::Type_val*> orders[] = {{&b.src1, &b.src2},
                                                                       {&b.src2, &b.src1}};
  for (const auto& [var_side, const_side] : orders) {
    const auto c = power_of_two_const(*const_side);
    if (!c) {
      continue;
    }
    framework::Instrs out;
    if (c->first == 0) {
      out.push_back(ir::Copy{.src = *var_side, .dst = b.dst});
    } else {
      out.push_back(binary(ir::LeftShift{}, *var_side, shift_count(c->first), b.dst));
    }
    return out;
  }
  for (const auto& [var_side, const_side] : orders) {
    if (const auto* x = std::get_if<ir::Var>(var_side)) {
      auto rewrite = two_term_multiply(*x, *const_side, b.dst, ctx);
      if (!rewrite) {
        rewrite = three_term_byte_multiply(*x, *const_side, b.dst, ctx);
      }
      if (rewrite) {
        return rewrite;
      }
    }
  }
  return std::nullopt;
}

// Returns whether `val` is unsigned (Python's `_is_unsigned`): a
// Constant by its class, and a Var by its type in the symbol table, by
// the rule of `tac_to_asm._is_unsigned_val`, so that both passes treat
// a value alike.
bool is_unsigned(const ir::Type_val& val, const SymbolTable* symbols) {
  if (const auto* c = std::get_if<ir::Constant>(&val)) {
    return asdl::isa<ir::ConstUChar, ir::ConstUInt, ir::ConstULong, ir::ConstULongLong>(c->const_);
  }
  if (const auto* v = std::get_if<ir::Var>(&val)) {
    if (symbols == nullptr) {
      return false;
    }
    const Symbol* sym = symbols->find(v->name);
    if (sym == nullptr) {
      return false;
    }
    return asdl::isa<c99::UInt, c99::ULong, c99::ULongLong, c99::Char, c99::UChar, c99::Pointer>(
        sym->type);
  }
  return false;
}

// Returns whether `val` can't be negative (`value_range::max_value`). A
// signed divide or remainder of such a value by a power of two is then
// the unsigned one, since truncating toward zero and rounding down give
// the same nonnegative quotient. `(space + 1) / 2` on a `uint8_t space` is
// `ZeroExtend(space) + 1`, at most 256, so it's a shift where it had
// called `sdivmod16`.
bool nonnegative(const ir::Type_val& val, const RuleEnv& env, const PassContext& ctx) {
  const value_range::DefOf def_of = [&](const ir::Var& v) -> const ir::Type_instruction* {
    return env.def_of(ir::Type_val{v});
  };
  return value_range::max_value(val, def_of, ctx.symbols, ctx.ssa_dsts).has_value();
}

// Python's `_build_divide`: an unsigned `Divide(x, 2^k)` becomes
// `RightShift(x, k)`, and `Divide(x, 1)` a Copy. Only `x / const` is
// taken, since a divide doesn't commute.
std::optional<framework::Instrs> build_divide(const ir::Binary& b, const RuleEnv& env,
                                              const PassContext& ctx) {
  const auto c = power_of_two_const(b.src2);
  if (!c) {
    return std::nullopt;
  }
  framework::Instrs out;
  if (c->first == 0) {
    if (asdl::isa<ir::Constant>(b.src1)) {
      return std::nullopt;
    }
    out.push_back(ir::Copy{.src = b.src1, .dst = b.dst});
    return out;
  }
  if (!is_unsigned(b.src1, ctx.symbols) && !nonnegative(b.src1, env, ctx)) {
    return std::nullopt;
  }
  out.push_back(binary(ir::RightShift{}, b.src1, shift_count(c->first), b.dst));
  return out;
}

// Returns a zero of the class of `val`: a Constant's own class, or for a
// Var the class of its type in the symbol table (Python's `_zero_const`).
std::optional<ir::Type_const> zero_const(const ir::Type_val& val, const SymbolTable* symbols) {
  if (const ir::Type_const* c = int_const(val)) {
    return bit_precise::with_value(*c, 0);
  }
  if (asdl::isa<ir::Constant>(val)) {
    return std::nullopt;
  }
  const auto* v = std::get_if<ir::Var>(&val);
  if (v == nullptr || symbols == nullptr) {
    return std::nullopt;
  }
  const Symbol* sym = symbols->find(v->name);
  if (sym == nullptr) {
    return std::nullopt;
  }
  return strength_reduction::zero_for_c99_type(sym->type);
}

// Python's `_build_modulo`: an unsigned `Modulo(x, 2^k)` becomes
// `BitwiseAnd(x, 2^k - 1)`, and `Modulo(x, 1)` a Copy of a zero.
std::optional<framework::Instrs> build_modulo(const ir::Binary& b, const RuleEnv& env,
                                              const PassContext& ctx) {
  const auto c = power_of_two_const(b.src2);
  if (!c) {
    return std::nullopt;
  }
  framework::Instrs out;
  if (c->first == 0) {
    auto zero = zero_const(b.src1, ctx.symbols);
    if (!zero) {
      return std::nullopt;
    }
    out.push_back(ir::Copy{.src = ir::Constant{.const_ = std::move(*zero)}, .dst = b.dst});
    return out;
  }
  if (!is_unsigned(b.src1, ctx.symbols) && !nonnegative(b.src1, env, ctx)) {
    return std::nullopt;
  }
  // The mask is a constant of src2's class, which the type checker made
  // src1's promoted type.
  const ir::Type_const& divisor = std::get<ir::Constant>(b.src2).const_;
  out.push_back(binary(ir::BitwiseAnd{}, b.src1,
                       ir::Constant{.const_ = bit_precise::with_value(divisor, c->second - 1)},
                       b.dst));
  return out;
}

}  // namespace

namespace strength_reduction {

std::optional<int> scalar_int_size(const c99::Type_data_type& t) {
  switch (c99::kind(t)) {
    case TK::Char:
    case TK::SChar:
    case TK::UChar:
      return 1;
    case TK::Int:
    case TK::UInt:
      return 2;
    case TK::Long:
    case TK::ULong:
      return 4;
    case TK::LongLong:
    case TK::ULongLong:
      return 8;
    default:
      return std::nullopt;
  }
}

std::optional<ir::Type_const> zero_for_c99_type(const c99::Type_data_type& t) {
  switch (c99::kind(t)) {
    case TK::Int:
      return ir::ConstInt{0};
    case TK::Long:
      return ir::ConstLong{0};
    case TK::LongLong:
      return ir::ConstLongLong{0};
    case TK::UInt:
      return ir::ConstUInt{0};
    case TK::ULong:
      return ir::ConstULong{0};
    case TK::ULongLong:
      return ir::ConstULongLong{0};
    case TK::Char:
      // `char` promotes to int.
      return ir::ConstUInt{0};
    case TK::SChar:
      return ir::ConstInt{0};
    case TK::UChar:
      return ir::ConstUInt{0};
    default:
      return std::nullopt;
  }
}

std::optional<TwoPowerTerms> two_power_terms(asdl::Int value) {
  const asdl::Int low = value & -value;
  const asdl::Int rest = value - low;
  if (rest != 0 && (rest & (rest - 1)) == 0) {
    return TwoPowerTerms{bit_length(rest) - 1, bit_length(low) - 1, false};
  }
  const asdl::Int top = value + low;
  if ((top & (top - 1)) == 0) {
    return TwoPowerTerms{bit_length(top) - 1, bit_length(low) - 1, true};
  }
  return std::nullopt;
}

}  // namespace strength_reduction

tac_ast::Function reduce_strength(tac_ast::Function fn, const PassContext& ctx) {
  return framework::run_rules(
      std::move(fn),
      [&](const framework::Instrs& instrs, std::size_t i,
          RuleEnv& env) -> std::optional<std::pair<framework::Instrs, std::size_t>> {
        const auto* b = std::get_if<ir::Binary>(&instrs[i]);
        if (b == nullptr) {
          return std::nullopt;
        }
        std::optional<framework::Instrs> rep;
        switch (ir::kind(b->op)) {
          case BK::Multiply:
            rep = build_multiply(*b, ctx);
            break;
          case BK::Divide:
            rep = build_divide(*b, env, ctx);
            break;
          case BK::Modulo:
            rep = build_modulo(*b, env, ctx);
            break;
          default:
            break;
        }
        if (!rep) {
          return std::nullopt;
        }
        return std::make_pair(std::move(*rep), std::size_t{1});
      });
}

}  // namespace c6502::passes
