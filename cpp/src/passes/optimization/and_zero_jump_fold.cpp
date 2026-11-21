/* A TAC rule that folds `Binary(BitwiseAnd, x, ConstInt(C), %res);
JumpIfTrue/False(%res, t)`, with a single-use `%res`, into one
`JumpIfMasked` when the operand narrows to 1 byte.

The motivating idiom. C99 `if (uchar_var & 0x80)` lowers through
c99_to_tac as this TAC.

    ZeroExtend(uchar_var, %ext)               # uchar promoted to int
    Binary(BitwiseAnd, %ext, ConstInt(0x80), %res)
    JumpIfFalse(%res, .if_end)

Without the rule, `tac_to_asm` lowers the BitwiseAnd as a 2-byte AND
chain that writes both bytes of `%res`, followed by a JumpIfFalse that
ORs the bytes and branches on Z. After the constant propagation,
dead-store, and redundant-load peepholes do what they can, this is
typically what is left.

    LDA uchar      ; AND #$80
    STA %res.lo     ; LDA #$00 ; STA %res.hi
    LDA %res.lo     ; BEQ .if_end

The `STA %res.hi` writes a constant zero into a slot nothing reads,
since the test only reads the low byte, and the `LDA %res.lo` after
`STA %res.lo` reloads the byte just stored. Both could be removed in
principle, but the asm passes don't catch them in every shape.

What the rule does. When the operand of the AND traces back to a 1-byte
unsigned value through `ZeroExtend(narrow, %ext)`, and the other operand
is an integer constant from 0 to 255, with a single-use AND result, the
pair collapses to

    JumpIfMasked(narrow, mask=C, jump_when_nonzero=..., target)

which `tac_to_asm` lowers as `LDA narrow ; AND #C ; B(EQ|NE) target`,
three instructions with no stores and no zero high byte. With `C ==
0x80` the asm peephole `and_sign_bit_branch` then folds `LDA; AND #$80;
B(EQ|NE)` into `LDA; B(PL|MI)`, which leaves two.

The sense. `JumpIfMasked` keeps its sense in `jump_when_nonzero`. A
`JumpIfTrue(%res, t)` gives `jump_when_nonzero=True`, a jump when
`(narrow & C) != 0`, and a `JumpIfFalse(%res, t)` gives
`jump_when_nonzero=False`, a jump when `(narrow & C) == 0`.

Soundness. `(uchar)x & 0x80` is always 0 or 0x80, and so is the same
value after a ZeroExtend, whose upper byte is 0. The widened AND
`(int)x_ext & 0x80` gives the same value. JumpIfFalse jumps when it is
zero and JumpIfTrue when it isn't. A 1-byte AND `narrow & 0x80` gives
the same 0 or 0x80, and a 1-byte branch on it tests the same predicate,
so the fold narrows an operation whose meaning doesn't depend on the
extra width. This holds for any mask from 0 to 255, since every bit of
the source above bit 7 is zero, so a mask bit above bit 7 contributes
nothing. The rule requires `0 <= C <= 0xFF` so that the mask fits the
byte of the narrowed AND.

A wide value with a mask in one byte. `if (zchars & 0x8000)` on a 16-bit
`zchars` has no extend to trace. When the mask's set bits all fall in
one byte `k` of the value (here `0x80 << 8`), only that byte of the AND
can be nonzero, so the test is of that byte.

    Binary(RightShift, zchars, 8 * k, %s)   # the byte moved down
    Truncate(%s, %b)                         # and taken alone
    JumpIfMasked(%b, mask=0x80, ...)

`tac_to_asm` lowers the shift by a whole byte as a byte move, and the
asm peepholes leave `LDA zchars+1; BPL` (or `BIT zchars+1; BPL`) where
the 16-bit AND stored both bytes and ORed them together. The value may
be any integer variable of 2 or more bytes, signed or unsigned, since
byte `k` is the same either way. `%s` and `%b` are new temporaries,
named after the AND's result.

Where it runs. In the fixed-point loop in SSA form, in the rule set
with `fold_cmp_zero_jump`. The two complement each other. That one
takes `(uchar < N)` and `(uchar == 0)`, and this one takes `(uchar &
mask)`. Both rely on the single-use gate and on the same test that the
extend's source is unchanged.

Only a Binary immediately followed by the jump is taken. The shapes
c99_to_tac writes for `if`, `while`, and `?:` are adjacent, and other
cases wait for copy propagation and dead-store elimination to close the
gap in later rounds of the fixed point. */
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

// Returns the value of `v` when it is a constant of one of the classes
// of `_INT_CONSTS` (the integer classes of 2 bytes or more, with no odd
// width), or none.
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

// Returns the value of `v` when it is a constant of a mask's classes in
// `_try_narrow_pair`, those of `wide_int_value` and the two byte
// classes, or none.
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

// A wide operand of the AND whose mask's set bits fall in one byte: the
// operand, the byte's index `k`, and the mask moved down to that byte.
struct OneByteMask {
  const ir::Var* val;
  int k;
  asdl::Int mask;
};

// Returns the operand and byte when one operand of the AND is a Var and
// the other an integer constant whose set bits all fall in byte `k`
// (Python's `_one_byte_mask`), or none.
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

// Returns the instructions that put byte `k` of the AND's wide operand
// in a 1-byte temporary, for the wide form of the module comment, or
// none (Python's `_try_wide_byte`). The last one's dst is the temporary.
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

// Returns the extend's source and the mask when `maybe_narrow` traces
// through a ZeroExtend to a `char` or `unsigned char` Var and
// `maybe_const` is an integer constant from 0 to 255, with the source
// holding at `use_idx` the value it held at the extend (Python's
// `_try_narrow_pair`), or none.
std::optional<std::pair<ir::Var, asdl::Int>> try_narrow_pair(const ir::Type_val& maybe_narrow,
                                                            const ir::Type_val& maybe_const,
                                                            const RuleEnv& env,
                                                            const PassContext& ctx,
                                                            std::ptrdiff_t use_idx) {
  // Only integer constants, since a floating one makes no sense as a
  // bitwise mask.
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

// Python's `_build_narrow_and_jump`, behind the guards of
// `AND_ZERO_RULE`, `single_use('and_dst')` and `have_symbols()`. AND is
// commutative, so both orders of the operands are tried.
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
  // The AND's own position, where the narrowed source is read in place
  // of the extend's dst. A non-SSA AND dst written again later has its
  // last definition elsewhere, and then -1 lets only an SSA-renamed
  // extend source qualify.
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
