/* TAC peephole: fold `Truncate(SignExtend|ZeroExtend(x), u)` into
`Copy(x, u)`, or into `Truncate(x, u)` when it narrows past x's width.

Motivating shape. C99 §6.3.1.1 promotes 1-byte values to `int` whenever
they take part in an arithmetic, subscript, or bitwise context. For an
`int8_t i` array subscript like `arr[i]`, the promotion is
`SignExtend(i, %ext)`. The `recognize_indexed_*` passes collapse the Add
and the Load into an `IndexedLoad(arr, %narrow, _)` where `%narrow` is
`Truncate(%ext, %narrow)`, a 1-byte index recovered from the 2-byte
extension.

The result is a `SignExtend; ...; Truncate` round trip whose net effect
is the identity, modulo the narrowing of bytes. TAC's liveness of each
Var sees `%ext` as alive, since the Truncate reads it, so SSA dead-code
elimination can't drop the SignExtend. But byte 0 of `%ext` equals `x`
by the SignExtend's definition, and that's all the Truncate reads.

This pass recognizes the pattern and replaces the Truncate with a Copy,
or with a narrower Truncate when the destination is narrower than the
original source. After the rewrite `%ext` has no readers left, and SSA
dead-code elimination drops the SignExtend on the next iteration. The
TAC `IndexedLoad` chain then composes through copy folding to drop the
Copy too, leaving just `IndexedLoad(arr, i, _)`.

Soundness. `SignExtend(x_n, t_m)` produces an m-byte value whose low n
bytes equal `x_n` and whose high bytes copy its sign. `ZeroExtend(x_n,
t_m)` is the same with zero high bytes. In both, bytes 0 to n-1 of
`t_m` equal bytes 0 to n-1 of `x_n`, and bytes n to m-1 don't depend
on the rest of `x`. `Truncate(t_m, u_k)` reads bytes 0 to k-1 of `t_m`.

- When k <= n, every byte read comes from `x_n`'s low k bytes, so the
  Truncate is `Truncate(x_n, u_k)`, or a `Copy` when k == n.
- When k > n, some byte read comes from the extend's high bytes. A
  truncate to a width between x's and the extend's is the same extend
  of x to the narrower width.

When this fires. In the TAC fixed-point loop. After `recognize_indexed_*`
introduces the Truncate (to narrow the index for the absolute,X and
(zp),Y lowerings), this pass collapses the `SignExtend` and `Truncate`
round trip on the next iteration.

The gate on `x`. The rewrite moves the read of `x` from the extend to
the Truncate. That reads the same value only when nothing writes `x` in
between, which holds for an SSA-renamed name and not for a static or an
address-taken local (`long l2 = (int)g; g += 1; f((signed char)l2);`
read the incremented `g`).

The gate on `%ext`. In SSA form each name has one definition, so the
test that `Truncate.src`'s definition is a cast is enough. The cast's
destination doesn't have to be read only once: several Truncates can
each be rewritten, and SSA dead-code elimination collects the cast they
leave behind.

The second pass, `fold_extend_extend`, makes an extend of a widening
extend of `x` one extend of `x`. A `ZeroExtend` that widens leaves the
top bit of its result 0, so sign-extending that result further adds
zero bytes, the same bytes a zero extension adds.

- `ZeroExtend(ZeroExtend(x))` and `SignExtend(ZeroExtend(x))` are
  `ZeroExtend(x)`.
- `SignExtend(SignExtend(x))` is `SignExtend(x)`.
- `ZeroExtend(SignExtend(x))` stays, since its middle bytes copy x's
  sign and its top bytes are 0.

`(unsigned long)i` for a `uint8_t i` promotes `i` to `int` first, a
`ZeroExtend` and then a `SignExtend`, and the comparison narrowing in
`cmp_zero_jump_fold` follows only one `ZeroExtend`, so `i < sizeof a`
had compared four bytes. */
#include "passes/optimization/truncate_extend_fold.h"

#include <optional>

namespace c6502::passes {

namespace {

namespace ir = ::c6502::tac_ast;
namespace c99 = ::c6502::c99_ast;
using framework::DefUseEnv;
using framework::DefUseRewrite;
using framework::PassContext;
using K = ir::Kind_instruction;

// Returns the byte width of `v`'s declared type, or none when there's no
// symbol table, no symbol, or a type of no width this pass knows (a
// bit-precise type of an odd width among them).
std::optional<int> byte_width(const ir::Var& v, const SymbolTable* symbols) {
  if (symbols == nullptr) {
    return std::nullopt;
  }
  const Symbol* sym = symbols->find(v.name);
  if (sym == nullptr) {
    return std::nullopt;
  }
  const c99::Type_data_type* t = &sym->type;
  for (;;) {
    if (const auto* c = std::get_if<c99::Const>(t)) {
      t = &*c->referenced_type;
    } else if (const auto* vol = std::get_if<c99::Volatile>(t)) {
      t = &*vol->referenced_type;
    } else {
      break;
    }
  }
  switch (c99::kind(*t)) {
    case c99::Kind_data_type::Char:
    case c99::Kind_data_type::SChar:
    case c99::Kind_data_type::UChar:
      return 1;
    case c99::Kind_data_type::Int:
    case c99::Kind_data_type::UInt:
    case c99::Kind_data_type::Pointer:
      return 2;
    case c99::Kind_data_type::Long:
    case c99::Kind_data_type::ULong:
    case c99::Kind_data_type::Float:
      return 4;
    case c99::Kind_data_type::LongLong:
    case c99::Kind_data_type::ULongLong:
    case c99::Kind_data_type::Double:
      return 8;
    default:
      return std::nullopt;
  }
}

// The source and destination of a `SignExtend`, `ZeroExtend`, or
// `Truncate`, with its kind.
struct Cast {
  K kind;
  const ir::Type_val* src;
  const ir::Type_val* dst;
};

std::optional<Cast> as_cast(const ir::Type_instruction& instr) {
  switch (ir::kind(instr)) {
    case K::SignExtend: {
      const auto& i = std::get<ir::SignExtend>(instr);
      return Cast{K::SignExtend, &i.src, &i.dst};
    }
    case K::ZeroExtend: {
      const auto& i = std::get<ir::ZeroExtend>(instr);
      return Cast{K::ZeroExtend, &i.src, &i.dst};
    }
    case K::Truncate: {
      const auto& i = std::get<ir::Truncate>(instr);
      return Cast{K::Truncate, &i.src, &i.dst};
    }
    default:
      return std::nullopt;
  }
}

ir::Type_instruction make_cast(K kind, ir::Var src, ir::Var dst) {
  switch (kind) {
    case K::SignExtend:
      return ir::SignExtend{.src = std::move(src), .dst = std::move(dst)};
    case K::ZeroExtend:
      return ir::ZeroExtend{.src = std::move(src), .dst = std::move(dst)};
    default:
      return ir::Truncate{.src = std::move(src), .dst = std::move(dst)};
  }
}

// The rewrite of one `Truncate(src=Var, dst=Var)`, which walks back to
// the source's SSA definition and, when it is a cast of an SSA-renamed
// Var and the widths allow it, reads that Var instead.
std::optional<DefUseRewrite> rewrite_truncate(const ir::Type_instruction& instr,
                                              const DefUseEnv& env, const PassContext& ctx) {
  const auto* truncate = std::get_if<ir::Truncate>(&instr);
  if (truncate == nullptr) {
    return std::nullopt;
  }
  const auto* src_var = std::get_if<ir::Var>(&truncate->src);
  const auto* dst_var = std::get_if<ir::Var>(&truncate->dst);
  if (src_var == nullptr || dst_var == nullptr) {
    return std::nullopt;
  }
  if (!ctx.is_ssa(src_var->name)) {
    return std::nullopt;
  }
  const ir::Type_instruction* def = env.def_of(*src_var);
  const std::optional<Cast> cast = def != nullptr ? as_cast(*def) : std::nullopt;
  if (!cast) {
    return std::nullopt;
  }
  const auto* x = std::get_if<ir::Var>(cast->src);
  if (x == nullptr) {
    return std::nullopt;
  }
  if (!ctx.is_ssa(x->name)) {
    return std::nullopt;
  }
  const auto src_width = byte_width(*x, ctx.symbols);
  const auto dst_width = byte_width(*dst_var, ctx.symbols);
  if (!src_width || !dst_width) {
    return std::nullopt;
  }
  if (*dst_width > *src_width) {
    if (cast->kind == K::Truncate) {
      return std::nullopt;
    }
    return DefUseRewrite::one(make_cast(cast->kind, *x, *dst_var));
  }
  if (*dst_width == *src_width) {
    return DefUseRewrite::one(ir::Copy{.src = *x, .dst = *dst_var});
  }
  return DefUseRewrite::one(ir::Truncate{.src = *x, .dst = *dst_var});
}

// The rewrite of one extend of a Var into a Var, which walks back to
// the source's definition and makes an extend of a widening extend one
// extend.
std::optional<DefUseRewrite> rewrite_extend(const ir::Type_instruction& instr,
                                            const DefUseEnv& env, const PassContext& ctx) {
  const std::optional<Cast> outer = as_cast(instr);
  if (!outer || outer->kind == K::Truncate) {
    return std::nullopt;
  }
  const auto* src_var = std::get_if<ir::Var>(outer->src);
  const auto* dst_var = std::get_if<ir::Var>(outer->dst);
  if (src_var == nullptr || dst_var == nullptr) {
    return std::nullopt;
  }
  if (!ctx.is_ssa(src_var->name)) {
    return std::nullopt;
  }
  const ir::Type_instruction* def = env.def_of(*src_var);
  const std::optional<Cast> inner = def != nullptr ? as_cast(*def) : std::nullopt;
  if (!inner || inner->kind == K::Truncate) {
    return std::nullopt;
  }
  const auto* x = std::get_if<ir::Var>(inner->src);
  if (x == nullptr) {
    return std::nullopt;
  }
  if (!ctx.is_ssa(x->name)) {
    return std::nullopt;
  }
  const auto x_width = byte_width(*x, ctx.symbols);
  const auto mid_width = byte_width(*src_var, ctx.symbols);
  const auto dst_width = byte_width(*dst_var, ctx.symbols);
  if (!x_width || !mid_width || !dst_width) {
    return std::nullopt;
  }
  if (!(*x_width < *mid_width && *mid_width < *dst_width)) {
    return std::nullopt;
  }
  if (inner->kind == K::ZeroExtend) {
    return DefUseRewrite::one(ir::ZeroExtend{.src = *x, .dst = *dst_var});
  }
  if (outer->kind == K::SignExtend) {
    return DefUseRewrite::one(ir::SignExtend{.src = *x, .dst = *dst_var});
  }
  return std::nullopt;
}

}  // namespace

tac_ast::Function fold_truncate_extend(tac_ast::Function fn, const PassContext& ctx) {
  return framework::run_def_use(
      std::move(fn), ctx, [&](const ir::Type_instruction& instr, const DefUseEnv& env) {
        return rewrite_truncate(instr, env, ctx);
      });
}

tac_ast::Function fold_extend_extend(tac_ast::Function fn, const PassContext& ctx) {
  return framework::run_def_use(
      std::move(fn), ctx, [&](const ir::Type_instruction& instr, const DefUseEnv& env) {
        return rewrite_extend(instr, env, ctx);
      });
}

}  // namespace c6502::passes
