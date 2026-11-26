/* The largest value a TAC operand can hold, when it can't be negative.

`max_value` bounds an integer operand from its type and, for an
SSA-renamed Var, from the instruction that defines it. A bound means
the value lies between 0 and the bound. It returns none when the value
may be negative or nothing bounds it.

The rules for a definition are these, where `max(x)` is the bound of
the operand `x`.

- `ZeroExtend(x)`: `max(x)`, or the largest value of x's width.
- `SignExtend(x)`, `Copy(x)`, `Truncate(x)`: `max(x)`, when it fits
  the destination's type. A Truncate to an unsigned type is bounded by
  its type even when x isn't.
- `a & b`: the smaller bound, since the result has no bit the bounded
  operand lacks. One bound is enough.
- `a | b`, `a ^ b`: all ones up to the higher bit of the two bounds.
- `a + b`, `a * b`, `a << k`: the sum, the product, or the shifted
  bound, when that fits the destination's type, so that the result
  doesn't wrap.
- `a >> n`: `max(a)` shifted right by a constant `n`, or `max(a)` for a
  variable count. A nonnegative value shifts the same way whatever its
  type's signedness.
- `a / b`: `max(a)` divided by a positive constant `b`, or `max(a)` for
  any divisor whose bound is known, since a quotient of nonnegative
  values is no larger than its dividend.
- `a % b`: the smaller of `max(a)` and `b - 1` for a positive constant
  `b`, or `max(a)`.
- `Phi`: the largest bound of its arguments.

A Var's type bounds it too: an unsigned type by its largest value. A
signed type gives no bound, since its values may be negative.

The walk back through definitions stops at a depth of 12, and at a Var
it is already walking (a loop's Phi), where the type bound is all
that's left.

`narrow_byte_range` narrows a divide, a remainder, a right shift, and a
multiply whose operands are bounded by 255, and strength reduction
treats a signed divide or remainder by a power of two as an unsigned
one when the dividend is bounded.

A bound of an 8-byte type is below 2^64, so every sum and quotient
fits an `asdl::Int`. A product of two such bounds, or a bound shifted
left by up to 63 bits, may not, so those two compare the operands with
the destination's largest value first. */
#include "passes/optimization/value_range.h"

#include <algorithm>
#include <set>
#include <string>
#include <vector>

namespace c6502::passes::value_range {

namespace {

namespace ir = ::c6502::tac_ast;
namespace c99 = ::c6502::c99_ast;
using asdl::Int;
using Bound = std::optional<Int>;

constexpr int kMaxDepth = 12;

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

// Returns the value of an integer constant of the eight classes of the
// C types, or none for any other constant. The odd widths of the
// bit-precise types aren't among them.
std::optional<Int> int_const(const ir::Type_const& c) {
  switch (ir::kind(c)) {
    case ir::Kind_const::ConstChar:
      return std::get<ir::ConstChar>(c).value;
    case ir::Kind_const::ConstUChar:
      return std::get<ir::ConstUChar>(c).value;
    case ir::Kind_const::ConstInt:
      return std::get<ir::ConstInt>(c).value;
    case ir::Kind_const::ConstUInt:
      return std::get<ir::ConstUInt>(c).value;
    case ir::Kind_const::ConstLong:
      return std::get<ir::ConstLong>(c).value;
    case ir::Kind_const::ConstULong:
      return std::get<ir::ConstULong>(c).value;
    case ir::Kind_const::ConstLongLong:
      return std::get<ir::ConstLongLong>(c).value;
    case ir::Kind_const::ConstULongLong:
      return std::get<ir::ConstULongLong>(c).value;
    default:
      return std::nullopt;
  }
}

Bound const_value(const ir::Type_val& v) {
  if (const auto* c = std::get_if<ir::Constant>(&v)) {
    return int_const(c->const_);
  }
  return std::nullopt;
}

// Returns the number of bits of a nonnegative `x`.
int bit_length(Int x) {
  int n = 0;
  while (x > 0) {
    x >>= 1;
    ++n;
  }
  return n;
}

const c99::Type_data_type* var_type(const ir::Var& v, const SymbolTable* symbols) {
  if (symbols == nullptr) {
    return nullptr;
  }
  const Symbol* sym = symbols->find(v.name);
  return sym == nullptr ? nullptr : &sym->type;
}

// Returns `value` when the type `t` holds it, else none.
Bound fits(Bound value, const c99::Type_data_type& t) {
  if (!value) {
    return std::nullopt;
  }
  const Bound top = type_max(t);
  if (!top || *value > *top) {
    return std::nullopt;
  }
  return value;
}

struct Walk {
  const DefOf& def_of;
  const SymbolTable* symbols;
  const NameSet& ssa;

  Bound max(const ir::Type_val& val, int depth, const std::set<std::string>& visiting) const {
    if (const auto* c = std::get_if<ir::Constant>(&val)) {
      const auto v = int_const(c->const_);
      if (v && *v >= 0) {
        return v;
      }
      return std::nullopt;
    }
    const auto& var = std::get<ir::Var>(val);
    const c99::Type_data_type* t = var_type(var, symbols);
    if (t == nullptr || !int_width(*t)) {
      return std::nullopt;
    }
    const Bound by_type = is_unsigned_type(*t) ? type_max(*t) : std::nullopt;
    if (!ssa.contains(var.name) || depth >= kMaxDepth || visiting.contains(var.name)) {
      return by_type;
    }
    const ir::Type_instruction* ins = def_of(var);
    if (ins == nullptr) {
      return by_type;
    }
    std::set<std::string> inner = visiting;
    inner.insert(var.name);
    const Bound by_def = of_def(*ins, *t, depth + 1, inner);
    if (!by_def) {
      return by_type;
    }
    if (!by_type) {
      return by_def;
    }
    return std::min(*by_def, *by_type);
  }

  Bound of_def(const ir::Type_instruction& ins, const c99::Type_data_type& t, int depth,
               const std::set<std::string>& visiting) const {
    const auto m = [&](const ir::Type_val& v) { return max(v, depth, visiting); };
    switch (ir::kind(ins)) {
      case ir::Kind_instruction::ZeroExtend: {
        const auto& z = std::get<ir::ZeroExtend>(ins);
        Bound top;
        if (const auto* sv = std::get_if<ir::Var>(&z.src)) {
          if (const c99::Type_data_type* st = var_type(*sv, symbols)) {
            if (const auto w = int_width(*st)) {
              top = (Int(1) << (8 * *w)) - 1;
            }
          }
        }
        const Bound inner = m(z.src);
        if (!inner) {
          return top;
        }
        return top ? std::min(*inner, *top) : inner;
      }
      case ir::Kind_instruction::SignExtend:
        return fits(m(std::get<ir::SignExtend>(ins).src), t);
      case ir::Kind_instruction::Copy:
        return fits(m(std::get<ir::Copy>(ins).src), t);
      case ir::Kind_instruction::Truncate:
        return fits(m(std::get<ir::Truncate>(ins).src), t);
      case ir::Kind_instruction::Binary: {
        const auto& b = std::get<ir::Binary>(ins);
        return of_binary(b.op, b.src1, b.src2, t, m);
      }
      case ir::Kind_instruction::Phi: {
        const auto& phi = std::get<ir::Phi>(ins);
        if (phi.args.empty()) {
          return std::nullopt;
        }
        Int best = 0;
        for (const auto& a : phi.args) {
          const Bound x = m(std::get<ir::PhiArg>(a).source);
          if (!x) {
            return std::nullopt;
          }
          best = std::max(best, *x);
        }
        return fits(best, t);
      }
      default:
        return std::nullopt;
    }
  }

  template <class M>
  static Bound of_binary(const ir::Type_binary_operator& op, const ir::Type_val& a,
                         const ir::Type_val& b, const c99::Type_data_type& t, const M& m) {
    using Op = ir::Kind_binary_operator;
    switch (ir::kind(op)) {
      case Op::BitwiseAnd: {
        const Bound ma = m(a);
        const Bound mb = m(b);
        if (ma && mb) {
          return std::min(*ma, *mb);
        }
        return ma ? ma : mb;
      }
      case Op::BitwiseOr:
      case Op::BitwiseXor: {
        const Bound ma = m(a);
        const Bound mb = m(b);
        if (!ma || !mb) {
          return std::nullopt;
        }
        return fits((Int(1) << bit_length(std::max(*ma, *mb))) - 1, t);
      }
      case Op::Add: {
        const Bound ma = m(a);
        const Bound mb = m(b);
        if (!ma || !mb) {
          return std::nullopt;
        }
        return fits(*ma + *mb, t);
      }
      case Op::Multiply: {
        const Bound ma = m(a);
        const Bound mb = m(b);
        if (!ma || !mb) {
          return std::nullopt;
        }
        // The product fits `t` exactly when it is at most the type's
        // largest value, which is tested without forming a product
        // past 128 bits.
        const Bound top = type_max(t);
        if (!top || (*mb != 0 && *ma > *top / *mb)) {
          return std::nullopt;
        }
        return fits(*ma * *mb, t);
      }
      case Op::LeftShift: {
        const Bound ma = m(a);
        const Bound k = const_value(b);
        if (!ma || !k || *k < 0 || *k >= 64) {
          return std::nullopt;
        }
        const Bound top = type_max(t);
        if (!top || *ma > (*top >> static_cast<int>(*k))) {
          return std::nullopt;
        }
        return fits(*ma << static_cast<int>(*k), t);
      }
      case Op::RightShift: {
        const Bound ma = m(a);
        const Bound k = const_value(b);
        if (!ma) {
          return std::nullopt;
        }
        if (k && *k >= 0) {
          // Every bound is below 2^127, and C++ leaves a shift by the
          // width of `Int` or more undefined.
          return *k >= 127 ? Int(0) : *ma >> static_cast<int>(*k);
        }
        return ma;
      }
      case Op::Divide: {
        const Bound ma = m(a);
        const Bound k = const_value(b);
        if (!ma) {
          return std::nullopt;
        }
        if (k && *k > 0) {
          return *ma / *k;
        }
        if (m(b)) {
          return ma;
        }
        return std::nullopt;
      }
      case Op::Modulo: {
        const Bound ma = m(a);
        const Bound k = const_value(b);
        if (!ma) {
          return std::nullopt;
        }
        if (k && *k > 0) {
          return std::min(*ma, *k - 1);
        }
        if (m(b)) {
          return ma;
        }
        return std::nullopt;
      }
      default:
        return std::nullopt;
    }
  }
};

}  // namespace

std::optional<int> int_width(const c99::Type_data_type& t) {
  switch (c99::kind(strip(t))) {
    case c99::Kind_data_type::Char:
    case c99::Kind_data_type::SChar:
    case c99::Kind_data_type::UChar:
      return 1;
    case c99::Kind_data_type::Int:
    case c99::Kind_data_type::UInt:
      return 2;
    case c99::Kind_data_type::Long:
    case c99::Kind_data_type::ULong:
      return 4;
    case c99::Kind_data_type::LongLong:
    case c99::Kind_data_type::ULongLong:
      return 8;
    default:
      return std::nullopt;
  }
}

std::optional<Int> type_max(const c99::Type_data_type& t) {
  const auto w = int_width(t);
  if (!w) {
    return std::nullopt;
  }
  if (is_unsigned_type(t)) {
    return (Int(1) << (8 * *w)) - 1;
  }
  return (Int(1) << (8 * *w - 1)) - 1;
}

bool is_unsigned_type(const c99::Type_data_type& t) {
  switch (c99::kind(strip(t))) {
    case c99::Kind_data_type::Char:
    case c99::Kind_data_type::UChar:
    case c99::Kind_data_type::UInt:
    case c99::Kind_data_type::ULong:
    case c99::Kind_data_type::ULongLong:
      return true;
    default:
      return false;
  }
}

std::optional<Int> max_value(const ir::Type_val& val, const DefOf& def_of,
                             const SymbolTable* symbols, const NameSet* ssa_dsts) {
  static const NameSet kNone;
  const Walk walk{def_of, symbols, ssa_dsts != nullptr ? *ssa_dsts : kNone};
  return walk.max(val, 0, {});
}

}  // namespace c6502::passes::value_range
