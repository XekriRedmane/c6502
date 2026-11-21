/* TAC pass: a wide divide, remainder, or right shift of a value from 0 to
255 is done on one byte, and so is a left shift whose result stays below
256, and a wide multiply of two such values reads them as zero-extended
bytes.

The motivating shapes. C99 §6.3.1.1 promotes a byte to `int` before `/`,
`%`, and `>>`, so

    uint8_t pos;
    uint8_t row = pos / 20;

divides two `int`s,

    ZeroExtend(pos -> %e:int)
    Binary(Divide, %e, ConstInt(20) -> %q:int)
    Truncate(%q -> row)

and `tac_to_asm` called the signed 16-bit helper `sdivmod16`. The
dividend is from 0 to 255 and the divisor from 1 to 255, so the quotient
is that of an unsigned byte divide, and so is the remainder. This pass
rewrites the divide as

    Binary(Divide, pos, ConstUChar(20) -> %n:uchar)
    ZeroExtend(%n -> %q:int)

after which `fold_truncate_extend` makes `row` the byte quotient,
strength reduction makes a power of two a shift or a mask, and
`tac_to_asm` lowers a byte divide by any other constant as a loop of
subtractions (`_inline_byte_divide`).

A right shift of such a value, `(style >> 5) & 3` with a `uint8_t style`,
is a shift of the byte for the same reason, whatever the count, and the
high byte shifted out is zero. As an `int` it was a signed 16-bit shift
by 5, which called `asr16`.

A multiply of two such values, `price * (tier + 1)` with a `uint8_t
price` and a `tier` from 0 to 3, has a 16-bit product. `tac_to_asm`
lowers a 16-bit multiply whose operands each have one significant byte
(`passes/significant_bytes.py`) as an 8-by-8 shift-and-add loop in place
of a `mul16` call (`_inline_byte_product`). Here `tier + 1` is an `int`
add, whose high byte isn't known to be zero, so this pass reads the
operand as `ZeroExtend(Truncate(tier + 1))`, and `narrow_widened_arith`
then makes the add a byte add.

The rule. `value_range::max_value` bounds each operand. An operand
qualifies when its bound is at most 255, which also means it isn't
negative. A signed and an unsigned divide, remainder, or right shift then
give the same result, which is the unsigned byte one.

  - `Divide` and `Modulo`: the dividend qualifies, and the divisor is a
    constant from 1 to 255 or a Var that qualifies (a zero divisor has
    no defined result at either width).
  - `RightShift`: the shifted value qualifies. The lowering reads only
    the count's low byte, so a count that's a ZeroExtend of a byte is
    read as the byte, and any other count stays. A constant count of 8
    or more gives 0 at either width, and the shift becomes a copy of 0.
    A variable count of 8 to 15 gives 0 at both widths, since the byte
    shift's loop shifts every bit out.
  - `LeftShift`: the count is a constant `k` from 0 to 7 and the shifted
    value's bound shifted left by `k` is at most 255, so the bits shifted
    out of the low byte are zeros and the high bytes of the result are
    0. `0x28 * (row / 8)` on a byte `row`, which strength reduction makes
    `((row / 8) << 3) + ((row / 8) << 5)`, shifts `row / 8`, at most 31,
    by 3 as a byte.
  - `Multiply`: the destination is 2 bytes and both operands qualify,
    and at least one isn't already a byte constant or a ZeroExtend of a
    byte.

Each operand becomes a byte. A constant becomes a `ConstUChar`. An
SSA-renamed Var defined by a ZeroExtend of an unsigned byte becomes that
byte, when it holds the same value at the op (`extend_source_unchanged`).
Any other Var gets a new `Truncate` just before the op, which reads it
where the op did.

Python finds the op's position by identity in the instructions as
rewritten so far, and the C++ pass takes the index at which the walk
calls the rewrite. */
#include "passes/optimization/narrow_byte_range.h"

#include <cstddef>
#include <optional>
#include <string>
#include <utility>

#include "bit_precise.h"
#include "c99_ast.h"
#include "errors.h"
#include "passes/optimization/cmp_zero_jump_fold.h"
#include "passes/optimization/strength_reduction.h"
#include "passes/optimization/value_range.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::tac_ast;
namespace c99 = ::c6502::c99_ast;
using framework::DefUseEnv;
using framework::DefUseRewrite;
using framework::Instrs;
using framework::PassContext;
using BK = ir::Kind_binary_operator;

// Returns whether `t`, without its `const` qualifiers, is `volatile`.
bool is_volatile(const c99::Type_data_type& t) {
  const c99::Type_data_type* p = &t;
  while (const auto* c = std::get_if<c99::Const>(p)) {
    p = &*c->referenced_type;
  }
  return std::holds_alternative<c99::Volatile>(*p);
}

// Returns the integer value of a constant operand, or none for a Var or
// a floating constant (`getattr(v.const, "value", None)`).
std::optional<asdl::Int> constant_value(const ir::Type_val& v) {
  const auto* c = std::get_if<ir::Constant>(&v);
  return c != nullptr ? bit_precise::raw_value(c->const_) : std::nullopt;
}

// What one rewrite reads: the context, the view of the walk, and the
// op's position.
struct Site {
  const PassContext& ctx;
  const DefUseEnv& env;
  std::ptrdiff_t use_idx;
};

// Returns whether `v` is a constant, or a non-volatile Var of the op's
// width (Python's `_operand_ok`).
bool operand_ok(const ir::Type_val& v, int width, const PassContext& ctx) {
  if (std::holds_alternative<ir::Constant>(v)) {
    return true;
  }
  const auto* var = std::get_if<ir::Var>(&v);
  if (var == nullptr) {
    return false;
  }
  const Symbol* sym = ctx.symbols->find(var->name);
  return sym != nullptr && value_range::int_width(sym->type) == width && !is_volatile(sym->type);
}

// Returns the bound `value_range::max_value` gives `v`.
std::optional<asdl::Int> bound(const ir::Type_val& v, const Site& s) {
  const value_range::DefOf def_of = [&](const ir::Var& var) { return s.env.def_of(var); };
  return value_range::max_value(v, def_of, s.ctx.symbols, s.ctx.ssa_dsts);
}

// Returns the unsigned byte a Var is a ZeroExtend of, when it holds the
// same value at the op (Python's `_extend_source`), or none.
std::optional<ir::Var> extend_source(const ir::Type_val& v, const Site& s) {
  const auto* var = std::get_if<ir::Var>(&v);
  if (var == nullptr || !s.ctx.ssa_dsts->contains(var->name)) {
    return std::nullopt;
  }
  const ir::Type_instruction* def = s.env.def_of(*var);
  const auto* ext = def != nullptr ? std::get_if<ir::ZeroExtend>(def) : nullptr;
  if (ext == nullptr) {
    return std::nullopt;
  }
  const auto* src = std::get_if<ir::Var>(&ext->src);
  if (src == nullptr) {
    return std::nullopt;
  }
  const Symbol* src_sym = s.ctx.symbols->find(src->name);
  if (src_sym == nullptr || value_range::int_width(src_sym->type) != 1 ||
      !value_range::is_unsigned_type(src_sym->type) || is_volatile(src_sym->type)) {
    return std::nullopt;
  }
  const std::size_t def_idx = s.env.def_idx.find(var->name)->second;
  if (!extend_source_unchanged(ext->src, ext->dst, static_cast<std::ptrdiff_t>(def_idx),
                               s.use_idx, s.env.instructions, s.ctx.ssa_dsts)) {
    return std::nullopt;
  }
  return *src;
}

// Returns whether `v` is a byte already, a constant from 0 to 255 or a
// ZeroExtend of a byte (Python's `_byte_form`).
bool byte_form(const ir::Type_val& v, const Site& s) {
  if (const auto* c = std::get_if<ir::Constant>(&v)) {
    const asdl::Int value = bit_precise::raw_value(c->const_).value_or(-1);
    return value >= 0 && value <= 0xFF;
  }
  return extend_source(v, s).has_value();
}

// Returns a new SSA local named after `base`, an `unsigned char` or of
// `like`'s type (Python's `_temp`).
ir::Var temp(const PassContext& ctx, const std::string& base, const std::string& tag,
             const std::string* like = nullptr) {
  int k = 0;
  while (ctx.symbols->contains(base + "." + tag + std::to_string(k))) {
    ++k;
  }
  const std::string name = base + "." + tag + std::to_string(k);
  c99::Type_data_type t = c99::UChar{};
  if (like != nullptr) {
    const Symbol* sym = ctx.symbols->find(*like);
    if (sym == nullptr) {
      throw InternalError("narrow_byte_range: no symbol for " + *like);
    }
    t = sym->type;
  }
  ctx.symbols->insert_or_assign(name, Symbol{.type = std::move(t), .attrs = LocalAttr{}});
  ctx.ssa_dsts->insert(name);
  return ir::Var{.name = name};
}

// Returns `v` as an unsigned byte, from 0 to 255 by its bound (Python's
// `_byte`). A Var that isn't an extend of a byte is read through a new
// Truncate appended to `pre`.
ir::Type_val byte_of(const ir::Type_val& v, const Site& s, const std::string& base, Instrs& pre) {
  if (const auto* c = std::get_if<ir::Constant>(&v)) {
    const std::optional<asdl::Int> value = bit_precise::raw_value(c->const_);
    if (!value) {
      throw InternalError("narrow_byte_range: a floating constant as a byte");
    }
    return ir::Constant{.const_ = ir::ConstUChar{.value = *value}};
  }
  if (std::optional<ir::Var> src = extend_source(v, s)) {
    return *src;
  }
  ir::Var t = temp(s.ctx, base, "lo");
  pre.push_back(ir::Truncate{.src = v, .dst = t});
  return t;
}

// Returns the op on the bytes `a` and `b` into a new `unsigned char`,
// zero-extended into `dst`, after `pre` (Python's `_narrowed`).
DefUseRewrite narrowed(const ir::Type_binary_operator& op, ir::Type_val a, ir::Type_val b,
                       const ir::Var& dst, Instrs pre, const PassContext& ctx) {
  const ir::Var n = temp(ctx, dst.name, "nb");
  DefUseRewrite r;
  r.is_rewrite = true;
  r.is_list = true;
  r.replacement = std::move(pre);
  r.replacement.push_back(ir::Binary{.op = op, .src1 = std::move(a), .src2 = std::move(b), .dst = n});
  r.replacement.push_back(ir::ZeroExtend{.src = n, .dst = dst});
  return r;
}

// Reads each operand of a 2-byte multiply of two bytes as a ZeroExtend
// of a byte, when one of them isn't already one (Python's `_multiply`).
std::optional<DefUseRewrite> multiply(const ir::Binary& binary, const ir::Var& dst, int width,
                                      const Site& s) {
  if (width != 2) {
    return std::nullopt;
  }
  if (!operand_ok(binary.src2, width, s.ctx)) {
    return std::nullopt;
  }
  const std::optional<asdl::Int> mb = bound(binary.src2, s);
  if (!mb || *mb > 0xFF) {
    return std::nullopt;
  }
  const bool forms[] = {byte_form(binary.src1, s), byte_form(binary.src2, s)};
  if (forms[0] && forms[1]) {
    return std::nullopt;
  }
  Instrs pre;
  std::vector<ir::Type_val> operands;
  const ir::Type_val* vals[] = {&binary.src1, &binary.src2};
  for (int k = 0; k < 2; ++k) {
    const ir::Type_val& v = *vals[k];
    if (forms[k]) {
      operands.push_back(v);
      continue;
    }
    ir::Type_val low = byte_of(v, s, dst.name, pre);
    const auto* var = std::get_if<ir::Var>(&v);
    if (var == nullptr) {
      throw InternalError("narrow_byte_range: a constant operand of a multiply as a Var");
    }
    const ir::Var wide = temp(s.ctx, dst.name, "nw", &var->name);
    pre.push_back(ir::ZeroExtend{.src = std::move(low), .dst = wide});
    operands.push_back(wide);
  }
  DefUseRewrite r;
  r.is_rewrite = true;
  r.is_list = true;
  r.replacement = std::move(pre);
  r.replacement.push_back(ir::Binary{
      .op = binary.op, .src1 = std::move(operands[0]), .src2 = std::move(operands[1]), .dst = dst});
  return r;
}

// Python's `NarrowByteRange.rewrite`: a wide `Binary(/, %, >>, <<, *)`
// whose operands are bounded by 255 is done on bytes, by the rules of
// the module comment.
std::optional<DefUseRewrite> rewrite(const ir::Type_instruction& instr, const DefUseEnv& env,
                                     const PassContext& ctx, std::size_t at) {
  const auto* binary = std::get_if<ir::Binary>(&instr);
  if (binary == nullptr) {
    return std::nullopt;
  }
  const BK op = ir::kind(binary->op);
  if (op != BK::Divide && op != BK::Modulo && op != BK::RightShift && op != BK::Multiply &&
      op != BK::LeftShift) {
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
  const c99::Type_data_type dst_type = dst_sym->type;
  const std::optional<int> width = value_range::int_width(dst_type);
  if (!width || *width == 1) {
    return std::nullopt;
  }
  const Site s{ctx, env, static_cast<std::ptrdiff_t>(at)};
  const ir::Type_val& a = binary->src1;
  const ir::Type_val& b = binary->src2;
  if (!operand_ok(a, *width, ctx)) {
    return std::nullopt;
  }
  const std::optional<asdl::Int> ma = bound(a, s);
  if (!ma || *ma > 0xFF) {
    return std::nullopt;
  }
  if (op == BK::Multiply) {
    return multiply(*binary, *dst, *width, s);
  }
  if (op == BK::LeftShift) {
    // `(row / 8) << 3` is at most 248, so its high byte is 0.
    const std::optional<asdl::Int> count = constant_value(b);
    if (!count || *count < 0 || *count > 7 || (*ma << static_cast<int>(*count)) > 0xFF) {
      return std::nullopt;
    }
    Instrs pre;
    ir::Type_val low = byte_of(a, s, dst->name, pre);
    return narrowed(binary->op, std::move(low), ir::Constant{.const_ = ir::ConstUChar{.value = *count}},
                    *dst, std::move(pre), ctx);
  }
  if (op == BK::RightShift) {
    const std::optional<asdl::Int> count = constant_value(b);
    if (count && *count >= 8) {
      const std::optional<ir::Type_const> zero = strength_reduction::zero_for_c99_type(dst_type);
      if (!zero) {
        throw InternalError("narrow_byte_range: no zero constant for the shift's type");
      }
      return DefUseRewrite::one(ir::Copy{.src = ir::Constant{.const_ = *zero}, .dst = *dst});
    }
    if (count && *count < 0) {
      return std::nullopt;
    }
    Instrs pre;
    ir::Type_val low = byte_of(a, s, dst->name, pre);
    // The lowering reads only the count's low byte, so a count that's a
    // widened byte is read as the byte.
    ir::Type_val count_val = b;
    if (std::optional<ir::Var> src = extend_source(b, s)) {
      count_val = *src;
    }
    return narrowed(binary->op, std::move(low), std::move(count_val), *dst, std::move(pre), ctx);
  }
  // Divide and Modulo.
  if (std::holds_alternative<ir::Constant>(b)) {
    const std::optional<asdl::Int> c = constant_value(b);
    if (!c || *c < 1 || *c > 0xFF) {
      return std::nullopt;
    }
  } else {
    if (!operand_ok(b, *width, ctx)) {
      return std::nullopt;
    }
    const std::optional<asdl::Int> mb = bound(b, s);
    if (!mb || *mb > 0xFF) {
      return std::nullopt;
    }
  }
  Instrs pre;
  ir::Type_val low_a = byte_of(a, s, dst->name, pre);
  ir::Type_val low_b = byte_of(b, s, dst->name, pre);
  return narrowed(binary->op, std::move(low_a), std::move(low_b), *dst, std::move(pre), ctx);
}

}  // namespace

ir::Function narrow_byte_range(ir::Function fn, const PassContext& ctx) {
  // The walk calls the rewrite once for each instruction in order, so
  // the count of calls is the instruction's index.
  std::size_t next = 0;
  return framework::run_def_use(std::move(fn), ctx,
                                [&](const ir::Type_instruction& instr, const DefUseEnv& env) {
                                  return rewrite(instr, env, ctx, next++);
                                });
}

}  // namespace c6502::passes
