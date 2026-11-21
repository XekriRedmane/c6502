/* Constant reassociation across nested Add chains.

The pass recognizes

    Binary(Add, C2, V, %inner)
    Binary(Add, C1, %inner, %outer)

(with the Constant on either side of either Add, since Add commutes)
and rewrites it to

    Binary(Add, (C1 + C2), V, %outer)

dropping the inner definition, which is then dead. The two Constants
are added at compile time, and the runtime work is one Add where it was
two.

The case that motivated it is pointer arithmetic on a static array with
a constant base offset. After the fold of `const` statics and of
constant array elements,

    hires_page1[interlace_p1_offsets[2] + col] = value;

lowers to

    Binary(Add, Constant(0x01D0), col_extended, %sum)
    Binary(Add, Constant(0x2000), %sum, %addr)
    Store(value, %addr)

and the two Constants become one 0x21D0, so the address is one 16-bit
Add of `Constant(0x21D0) + col`, which is half the runtime work and the
shape the IndexedStore recognizer matches for an absolute,X store.

Eligibility. The fusion is made when
  * the outer instruction is `Binary(Add, ..., dst=%outer)` with one
    Constant operand and one Var operand, and its destination is a Var;
  * that Var (`%inner`) is an SSA name whose definition is
    `Binary(Add, ...)` with one Constant operand and any other operand;
  * `%inner` is read exactly once in the function, by the outer Add.
    Otherwise the rewrite would repeat work the inner Add shared with
    its other readers;
  * the two Constants have one bit width. A signed and an unsigned class
    of one width may be mixed, since the bit pattern of an Add doesn't
    depend on the signedness, and the sum takes the outer Constant's
    class.

Width and wraparound. The two Constants are added modulo 2^bits for the
class's bit width, which is what the per-byte ADC chain of the 6502
computes for two-byte (pointer, `unsigned int`) or four-byte (`unsigned
long`) arithmetic. A signed overflow wraps the same way, since c6502
doesn't treat signed overflow as undefined in the lowering, which is
two's complement arithmetic.

A Subtract isn't handled. `K - (C - V)` is `K - C + V`, which isn't
`(K - C) - V`, and mixing Add and Subtract would take a record of the
signs through the chain, which value numbering or a polynomial form
would give.

Scope. One pair is fused per sweep, so a chain of three Adds or more is
reduced one pair at each sweep of the TAC optimizer's fixed point, which
is enough for the common `hires_page1 + arr[K] + col`.

The pass is a `DefUsePass`, and the def-use walk records each rewrite
in the view of the instructions it reads definitions from, so a chain
of fusions in one sweep (A into B, then B into C) reads the rewritten B
and not a definition already dropped. */
#include "passes/optimization/reassoc_const.h"

#include <string>
#include <utility>

#include "bit_precise.h"
#include "passes/optimization/var_visit.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::tac_ast;
using framework::DefUseEnv;
using framework::DefUseRewrite;
using framework::PassContext;
using BK = ir::Kind_binary_operator;

const ir::Constant* as_constant(const ir::Type_val& v) { return std::get_if<ir::Constant>(&v); }

}  // namespace

tac_ast::Function reassoc_constants(tac_ast::Function fn, const PassContext& ctx) {
  const auto uses = count_uses(fn.instructions);
  return framework::run_def_use(
      std::move(fn), ctx,
      [&](const ir::Type_instruction& instr,
          const DefUseEnv& env) -> std::optional<DefUseRewrite> {
        // The outer Add: a Constant and a Var, either way round, into a
        // Var.
        const auto* outer = std::get_if<ir::Binary>(&instr);
        if (outer == nullptr || ir::kind(outer->op) != BK::Add) {
          return std::nullopt;
        }
        const auto* outer_dst = std::get_if<ir::Var>(&outer->dst);
        if (outer_dst == nullptr) {
          return std::nullopt;
        }
        const ir::Constant* outer_const = as_constant(outer->src1);
        const ir::Var* outer_var = std::get_if<ir::Var>(&outer->src2);
        if (outer_const == nullptr || outer_var == nullptr) {
          outer_const = as_constant(outer->src2);
          outer_var = std::get_if<ir::Var>(&outer->src1);
        }
        if (outer_const == nullptr || outer_var == nullptr) {
          return std::nullopt;
        }
        // The inner Var must be an SSA name, which has one definition.
        if (!ctx.is_ssa(outer_var->name)) {
          return std::nullopt;
        }
        // Only a single-use inner Var is fused.
        const std::size_t* n = uses.find(outer_var->name);
        if (n == nullptr || *n != 1) {
          return std::nullopt;
        }
        const ir::Type_instruction* inner_instr = env.def_of(*outer_var);
        if (inner_instr == nullptr) {
          return std::nullopt;
        }
        // A non-SSA `x = x + C` reads its own destination, and fusing the
        // instruction with itself would erase the increment.
        if (outer_var->name == outer_dst->name) {
          return std::nullopt;
        }
        const auto* inner = std::get_if<ir::Binary>(inner_instr);
        if (inner == nullptr || ir::kind(inner->op) != BK::Add) {
          return std::nullopt;
        }
        const ir::Constant* inner_const = as_constant(inner->src1);
        const ir::Type_val* inner_other = &inner->src2;
        if (inner_const == nullptr) {
          inner_const = as_constant(inner->src2);
          inner_other = &inner->src1;
        }
        if (inner_const == nullptr) {
          return std::nullopt;
        }
        // The two Constants must have one bit width. A signed and an
        // unsigned class of it may be mixed.
        const auto outer_layout = bit_precise::integer_const_layout(outer_const->const_);
        const auto inner_layout = bit_precise::integer_const_layout(inner_const->const_);
        if (!outer_layout || !inner_layout || outer_layout->bytes != inner_layout->bytes) {
          return std::nullopt;
        }
        const asdl::Int sum = *bit_precise::raw_value(outer_const->const_) +
                              *bit_precise::raw_value(inner_const->const_);
        const asdl::Int combined =
            bit_precise::wrap(sum, 8 * outer_layout->bytes, outer_layout->is_signed);
        ir::Type_instruction replacement = ir::Binary{
            .op = ir::Add{},
            .src1 = ir::Constant{.const_ = bit_precise::with_value(outer_const->const_, combined)},
            .src2 = *inner_other,
            .dst = *outer_dst};
        return DefUseRewrite::dropping(std::move(replacement), {outer_var->name});
      });
}

}  // namespace c6502::passes
