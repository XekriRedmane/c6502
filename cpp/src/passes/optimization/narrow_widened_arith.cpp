/* TAC peephole: fold `Truncate(Binary(safe_op, Extend(a), Extend(b) |
Constant), u)` into `Binary(safe_op, a, b | narrowed_Constant, u)` at the
narrow width. The helpers that `recognize_indirect_indexed` and
`sink_increment` read are here too, and the header describes them.

The motivating shape. C99 §6.3.1.1 promotes 1-byte operands to `int`
before any arithmetic or bitwise op. For

    uint8_t a, b;
    uint8_t delta = (uint8_t)(a - b);

c99_to_tac writes this TAC.

    ZeroExtend(a   -> %ea:int)
    ZeroExtend(b   -> %eb:int)
    Binary(Subtract, %ea, %eb -> %r:int)    ; 16-bit subtract
    Truncate(%r -> %d:uchar)                ; cast back to 1 byte

`tac_to_asm` lowers the 16-bit subtract as a per-byte SBC chain. The high
byte's `LDA #$0; SBC #$0` is dead, since its result feeds only the byte
the Truncate drops, but the asm `dead_a_arith` can't always see through
the `LDA mem; BNE` shape after it, because `all_flags_dead_at` stops at
every Branch. Removing the work in the TAC removes it at the source.

Soundness. For an op whose result modulo 2^(8n) depends only on the
operands modulo 2^(8n) (the ops safe under wraparound), the low n bytes
of `op(extend_to_W(a), extend_to_W(b))` equal `op(a_low_n, b_low_n) mod
2^(8n)`. So

    Truncate(Binary(safe_op, Extend(a_n), Extend(b_n)), u_n)
        ≡ Binary(safe_op, a_n, b_n, u_n)

These ops are safe under wraparound.
  - Add and Subtract, whose carry goes strictly from low to high.
  - Multiply, since the low n bytes of a*b depend only on the low n
    bytes of a and b (2's complement and unsigned modular arithmetic
    coincide for a multiply).
  - BitwiseAnd, BitwiseOr, and BitwiseXor, which work on each byte.

In these ops the high bytes contribute to the low bytes of the result.
  - Divide and Modulo, since long division pulls from the high bytes.
  - RightShift, which shifts the high bytes down into the low ones. A
    shift of an extended byte is the exception, below.

LeftShift is safe in its first operand, since the low bytes of `x << k`
depend only on the low bytes of `x`. It's narrowed when the count is a
Constant below the narrow width in bits, or a variable. A constant shift
is inlined as ASL / ROL steps over every byte of the operand, so
`(uint8_t)(len << 4)` with `len` promoted to `int` would otherwise shift
a high byte that's thrown away. A variable count is a loop of one-bit
shifts (`tac_to_asm._shift_loop`), which shifts fewer bytes at the narrow
width, and a count that shifts every bit out gives 0 at either width.

RightShift is narrowed only when its first operand is a ZeroExtend of an
unsigned Var or a SignExtend of a signed one, and its count is a
Constant below the narrow width in bits. When the lowering inlines the
wide shift (`shift_cost.inline_shift_ok`) and not the narrow one, the
narrow one would be a loop, which is slower, and the shift is left wide.
A byte's arithmetic shift by 3 is a loop of three passes, and so is the
16-bit one, so it narrows. The extended bytes hold 0 or copies of the
sign bit, which are the bits a narrow logical or arithmetic shift of the
Var brings in, so `(uint8_t)(props[y] >> 5)` is a 1-byte shift. It had
shifted the extension's high byte, known to be 0, left by three and ORed
it into the result.

For a Constant operand, narrowing reduces the value modulo 2^(8n).
Modular arithmetic keeps the equivalence whatever the constant's
magnitude or sign. `Binary(Subtract, ZE(uc), Constant(0x1234:int))`
truncated to uchar is `Binary(Subtract, uc, Constant(0x34:uchar))`.

The rules for each Truncate.
  - Truncate.src is an SSA-renamed Var whose definition is a Binary safe
    under wraparound.
  - Each operand of the Binary is (a) an SSA-renamed Var whose
    definition is a `SignExtend` or `ZeroExtend` of a Var whose declared
    width is the Truncate's dst width, (b) a Constant that narrows to
    the dst width, or (c) any other integer Var wider than the dst
    width, such as a 16-bit global. Its low bytes come from a new
    `Truncate` just before the narrow Binary, so `(uint8_t)(w << 1)` on
    an `int w` becomes a 1-byte shift of `w`'s low byte. That happens
    only when the Truncate is the wide Binary's one reader, so that the
    wide Binary goes away. Otherwise the narrow one would repeat its
    work, as for `(uint8_t)(w + 3)` next to `(w + 3) >> 8`.
  - The Binary's dst is strictly wider than the Truncate's dst, since
    `constant_fold` and `fold_truncate_extend` take the trivial case.
  - The narrow Binary reads its operands where the Truncate was, so each
    Var it reads must still hold the value the wide Binary (or the
    Extend) read. An SSA-renamed Var always does. Any other Var has to
    be non-volatile, with the Extend or Binary in the same block as the
    Truncate and nothing in between that may write it (a definition of
    it, a call, or a store through a pointer or an index). Without this,
    `int t = g + 1; h(); (uint8_t)t` read `g` after `h` had changed it.

The output. The Truncate becomes a narrow Binary that writes the
Truncate's dst directly. The original Binary and Extends become dead,
single-use Extend dsts are dropped at once through the drops of a
`Rewrite`, and SSA dead-code elimination collects the rest on the next
round.

Comparisons. `Truncate(Binary(cmp, a, b))` for a comparison `cmp`
becomes `Binary(cmp, a, b)` into the Truncate's dst, since a comparison
is 0 or 1 at any width (`narrow_compare`). Its operands stay wide.

Iteration. The pass runs in the TAC fixed-point loop. It composes with
`fold_truncate_extend`, which takes the case with no Binary, with
constant folding, which sometimes resolves one Extend to a Constant, and
with SSA dead-code elimination, which collects the chain left behind.

Python finds the Truncate's index by identity (`_truncate_index`), which
for a name that several Truncates write is the matched one's position
where the index of definitions holds another's. The C++ pass takes the
index at which the walk calls the rewrite. */
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

// Returns whether `t`, without its `const` qualifiers, is `volatile`
// (Python's `_is_volatile`).
bool is_volatile_type(const c99::Type_data_type& t) {
  const c99::Type_data_type* p = &t;
  while (const auto* c = std::get_if<c99::Const>(p)) {
    p = &*c->referenced_type;
  }
  return std::holds_alternative<c99::Volatile>(*p);
}

// Returns `t` without its `const` and `volatile` qualifiers.
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

// Returns whether `instr` may end a block or write a variable it
// doesn't name (Python's `_ENDS_BLOCK + _MAY_WRITE_MEMORY`).
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
    if (!entry) {
      continue;
    }
    if (ends_block_or_writes_memory(*entry)) {
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

// Returns whether the low n bytes of `op` of two values depend only on
// their low n bytes (`_SAFE_OPS`), which the module comment explains.
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

// Returns whether `op` is a comparison, whose 0 or 1 fits any width
// (`_COMPARE_OPS`).
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

// The state of one run of the pass: the use counts that the rewrites
// decide their drops by, and the index of the instruction the walk is
// at.
struct Run {
  const PassContext& ctx;
  UseCounts counts;
  std::size_t at = 0;
};

// Adds the reads of a rewrite's new instructions to the counts that
// later rewrites in the same run decide their drops by (Python's
// `_count_new_uses`). The counts are taken once per run, so without this
// a later rewrite could drop the definition of an extend source that an
// earlier one now reads (the narrow Binary reads `p` where the wide one
// read `(int)p`).
void count_new_uses(UseCounts& counts, const Instrs& replacement) {
  for (const auto& instr : replacement) {
    for (const auto name : uses_in(instr)) {
      counts[std::string(name)] += 1;
    }
  }
}

// Returns the source Var of the extend `instr`, or null when it isn't a
// SignExtend or ZeroExtend of a Var.
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

// Returns whether `src`, the value a RightShift shifts, is a ZeroExtend
// of an unsigned Var or a SignExtend of a signed one, and that Var is
// `width` bytes, the width of the narrow result (Python's
// `_extended_shift_source`). A wider Var has bits above the result's
// that the shift brings down. `(uint8_t)((uint32_t)w >> 5)` on a 2-byte
// `w` reads bits 8 to 12 of `w`, so it can't be a shift of `w`'s low
// byte. The bytes above the extended Var's are copies of 0 or of its
// sign bit, which a narrow shift of the Var shifts in itself, since a
// logical shift fills with 0 and `tac_to_asm` picks the shift's kind
// from its first operand's type. So the low bytes of `(int)b >> k` are
// `b >> k` for a count below the Var's width in bits. Any other wide
// value shifts bits of its high bytes into the low ones, so
// `narrow_operand`'s Truncate of a wide Var doesn't apply.
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

// Returns `operand` narrowed to `width` bytes, signed when
// `is_unsigned_dst` is false, or none when narrowing isn't safe
// (Python's `_narrow_operand`). The narrow Binary sits where the
// Truncate is (`trunc_idx`), after the wide Binary (`bin_idx`). For a
// Var defined by a SignExtend or ZeroExtend of a Var of `width` bytes,
// returns the extend's source when it still holds the value the extend
// read. For any other integer Var wider than `width`, appends
// `Truncate(operand, tmp_name)` to `pre` and returns the temporary, when
// `operand` still holds the value the Binary read. For a Constant,
// returns a constant of the narrow class holding the value modulo
// 2^(8 * width).
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

// Returns `Truncate(a op b)` for a comparison `op` as `a op b` into the
// Truncate's dst (Python's `_narrow_compare`). A comparison is 0 or 1 at
// any width, so only its result narrows, and its operands stay as they
// are. `bool on = (b & 0x80) != 0;` is an `int` compare truncated to one
// byte, and a 1-byte compare with 0 lowers to `BEQ end; LDA #1; end:`
// (`tac_to_asm._translate_zero_test`) where an `int` one selects 0 or 1
// in each arm. Only when the Truncate is the compare's one reader, and
// each Var operand still holds at the Truncate the value the compare
// read.
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

// Python's `NarrowWidenedArith.rewrite`: a Truncate whose SSA source a
// wide Binary safe under wraparound defines becomes the narrow Binary
// into the Truncate's dst, by the rules of the module comment.
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
  // trunc_src must be SSA-renamed, so that it has one definition.
  if (!ctx.ssa_dsts->contains(trunc_src->name)) {
    return std::nullopt;
  }
  // Walk back to the definition of trunc_src, which must be a Binary
  // safe under wraparound.
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
    // No widening to undo. fold_truncate_extend or constant_fold takes
    // the trivial cases.
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
    // A left shift by a variable count narrows too. The low bytes of `x
    // << n` depend only on the low bytes of `x`, and a count that shifts
    // every bit out gives 0 at either width, since the loop of
    // `tac_to_asm._shift_loop` and the `asl*` helpers shift one bit per
    // count. `scaled <<= shift` on a byte had called `asl16`.
  } else if (is_shift) {
    const auto* count = std::get_if<ir::Constant>(&bin->src2);
    const std::optional<asdl::Int> value =
        count != nullptr ? bit_precise::raw_value(count->const_) : std::nullopt;
    if (!value || *value < 0 || *value >= 8 * *dst_width) {
      return std::nullopt;
    }
    // A narrow shift the lowering doesn't inline is a loop, which is
    // slower than a wide shift it inlines. `extended_shift_source` made
    // a right shift's first operand an extend of a Var.
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
  // Truncates of wide operands, placed just before the narrow Binary.
  // Their dsts are registered only once the rewrite is certain. They're
  // named after the wide result, which is an SSA name, so they're
  // unique, where the Truncate's dst may be a global that several
  // Truncates write.
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
  // The wide Binary stays when anything besides this Truncate reads its
  // result (`l0 ^= f(); l0 += (int)l0;` reads the wide XOR in the Add as
  // well as through the Truncate), and its Extend operands then stay
  // too.
  if (count_of(run.counts, bin_dst->name) != 1) {
    if (!pre.empty()) {
      // A wide operand isn't an extend that goes away, so the narrow
      // Binary would only repeat the wide one's work (`(uint8_t)(w + 3)`
      // next to `(w + 3) >> 8`).
      return std::nullopt;
    }
    count_new_uses(run.counts, r.replacement);
    return r;
  }
  // The Binary's dst goes with the rewrite, and so does each Extend dst
  // that only the Binary read, since nothing reads it once the Binary is
  // gone. The Truncate is the use being rewritten, which is replaced.
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
    // A new Truncate still reads the Extend's dst when the operand was
    // narrowed as a wide Var.
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
