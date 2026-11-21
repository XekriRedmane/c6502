/* Copy folding, which fuses `<producer dst=%t>; Copy(%t, X)` pairs.

`tac_to_asm` lowers `x += 1`, where x is a name that wasn't promoted to
SSA (a static or an address-taken local), as

    Binary(Add, x, 1, %t)
    Copy(%t, x)

The temporary `%t` exists only because every TAC operation writes one
destination by convention, and nothing writes or reads `%t` again past
the Copy. In asm the Copy lowers to N more moves (`LDA %t.lo; STA x.lo;
LDA %t.hi; STA x.hi; ...`), typically 4 instructions, 13 bytes, and 14
cycles for a 16-bit value, and more when `%t` spilled to the frame.

This pass removes the temporary by making the producer write `X`
directly:

    Binary(Add, x, 1, x)

The behavior is the same, without the temporary. The fused instruction
reads, changes, and writes `x` in place, which the multi-byte INC
peephole (`passes/inc_peephole.py`) then turns from a chain of ADCs into
`INC + BNE` for a `Data` or `ZP` operand.

What is fused. Any TAC instruction with one destination field that is a
Var (`%t`) followed by `Copy(%t, X)`, where the Copy is the one read of
`%t` in the function. The producer's destination becomes `X`, and the
Copy goes.

The producers that fuse are a SignExtend, a ZeroExtend, a Truncate, the
six floating conversions, a Unary, a Binary, a Copy (which removes a
chain of copies), a GetAddress, a Load, an IndexedLoad, and a
FunctionCall or IndirectCall with a destination.

A Phi doesn't fuse. Its destination is always an SSA name in the IR
this pass sees (between to_ssa and from_ssa), and SSA construction's
invariant that the Phi is a Phi destination's one definition has to
stay until SSA destruction. Copy propagation and dead-store elimination
already remove `Phi(...) -> %t; Copy(%t, %r)` when `%r` is renamed.

When it runs. In the TAC optimizer's fixed point, after copy
propagation and dead-store elimination, with the other SSA passes, and
once after SSA destruction. These open new pairs:
  * strength reduction making a Multiply a LeftShift (the shift feeds
    the Copy the Multiply fed);
  * constant folding folding a cast (the Copy left over fuses);
  * copy propagation and dead-store elimination clearing the
    temporaries of a chain.

Soundness. The fusion is sound when

  * `%t`'s definition is the producer fused, which SSA gives for a
    renamed `%t`, and the count of reads covers for one that wasn't;
  * the Copy is the one read of `%t` in the whole function, so no other
    reader sees `%t`'s value change when the producer writes elsewhere;
  * the Copy comes right after the producer, so no instruction between
    them could write `X` (a write the Copy would overwrite without the
    fusion) or read `X` (and see a different value with it).

The count of reads makes this safe outside SSA too. When `%t` has
several definitions and one read (the Copy), the definition before the
Copy writes `X` instead, and the others write a name nothing reads,
which dead-store elimination drops in a later sweep.

`X` needn't be an SSA name, since the fusion handles both. Copy
propagation and dead-store elimination already handle an `X` that was
renamed, so what this pass alone does is the case of a name that
wasn't, the static or address-taken local that nothing else cleans up.

The fusion runs as a `WindowPass` of two instructions, which matches
any instruction followed by `Copy(Var, any)` and checks the rest in its
rewrite. Python's module also has `fold_copies`, an older single walk
without the statics among the names that can be read elsewhere, which
nothing in the pipeline calls, so it isn't ported. */
#include "passes/optimization/copy_folding.h"

#include <optional>
#include <string>
#include <utility>

namespace c6502::passes {

namespace {

namespace ir = ::c6502::tac_ast;
using framework::Instrs;
using framework::PassContext;
using K = ir::Kind_instruction;

// Returns the destination field of a producer whose destination can be
// redirected, or null for any other instruction. A Phi isn't one, since
// its destination is always an SSA name in this pipeline, and copy
// propagation and dead-store elimination clean up after it.
ir::Type_val* redirectable_dst(ir::Type_instruction& instr) {
  switch (ir::kind(instr)) {
    case K::SignExtend:
      return &std::get<ir::SignExtend>(instr).dst;
    case K::ZeroExtend:
      return &std::get<ir::ZeroExtend>(instr).dst;
    case K::Truncate:
      return &std::get<ir::Truncate>(instr).dst;
    case K::IntToFloat:
      return &std::get<ir::IntToFloat>(instr).dst;
    case K::IntToDouble:
      return &std::get<ir::IntToDouble>(instr).dst;
    case K::FloatToInt:
      return &std::get<ir::FloatToInt>(instr).dst;
    case K::DoubleToInt:
      return &std::get<ir::DoubleToInt>(instr).dst;
    case K::FloatToDouble:
      return &std::get<ir::FloatToDouble>(instr).dst;
    case K::DoubleToFloat:
      return &std::get<ir::DoubleToFloat>(instr).dst;
    case K::Unary:
      return &std::get<ir::Unary>(instr).dst;
    case K::Binary:
      return &std::get<ir::Binary>(instr).dst;
    case K::Copy:
      return &std::get<ir::Copy>(instr).dst;
    case K::GetAddress:
      return &std::get<ir::GetAddress>(instr).dst;
    case K::Load:
      return &std::get<ir::Load>(instr).dst;
    case K::IndexedLoad:
      return &std::get<ir::IndexedLoad>(instr).dst;
    case K::FunctionCall: {
      auto& d = std::get<ir::FunctionCall>(instr).dst;
      return d ? &*d : nullptr;
    }
    case K::IndirectCall: {
      auto& d = std::get<ir::IndirectCall>(instr).dst;
      return d ? &*d : nullptr;
    }
    default:
      return nullptr;
  }
}

// Returns `fn` with each pair of a producer and the Copy after it that
// fuses made one instruction, or `fn` itself. The names that can be
// read without a Var read
// in the instructions, which are the statics (other functions read
// them) and every local whose address is taken (a `Load` through the
// pointer reads it), don't fuse: the count of their reads says nothing
// about whether their write is dead, so the write has to stay. In
// `g = 7; h = g;` with a global `g`, fusing would store 7 into `h` and
// never write `g`.
tac_ast::Function fold_copies(tac_ast::Function fn, const PassContext& ctx) {
  framework::RuleEnv env = framework::make_rule_env(fn.instructions);
  const NameSet& observable = framework::observable_names(env, ctx);
  return framework::run_window(
      std::move(fn), 2, [&](const Instrs& instrs, std::size_t i) -> std::optional<Instrs> {
        const auto* copy = std::get_if<ir::Copy>(&instrs[i + 1]);
        if (copy == nullptr) {
          return std::nullopt;
        }
        const auto* src = std::get_if<ir::Var>(&copy->src);
        if (src == nullptr) {
          return std::nullopt;
        }
        const std::size_t* count = env.use_count.find(src->name);
        if (count == nullptr || *count != 1 || observable.contains(src->name)) {
          return std::nullopt;
        }
        ir::Type_instruction producer = instrs[i];
        ir::Type_val* dst = redirectable_dst(producer);
        const auto* dst_var = dst != nullptr ? std::get_if<ir::Var>(dst) : nullptr;
        if (dst_var == nullptr || dst_var->name != src->name) {
          return std::nullopt;
        }
        *dst = copy->dst;
        Instrs out;
        out.push_back(std::move(producer));
        return out;
      });
}

}  // namespace

tac_ast::Function fold_copies_in_fixedpoint(tac_ast::Function fn, const PassContext& ctx) {
  return fold_copies(std::move(fn), ctx);
}

tac_ast::Function fold_copies_post_destruction(tac_ast::Function fn, const PassContext& ctx) {
  return fold_copies(std::move(fn), ctx);
}

}  // namespace c6502::passes
