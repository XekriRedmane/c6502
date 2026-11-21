/* TAC pass that recognizes the absolute,X load.

It mirrors `recognize_indexed_store` for the Load. After the fold of
const statics and the reassociation of constants, an expression such as

    pixels = hud_strip_src[y];   // hud_strip_src is `T * const`

with `hud_strip_src` a const pointer static that folds to a numeric
address, lowers to

    ZeroExtend(y,           %ext)        # y uchar to 2 bytes
    Binary(Add, C, %ext,    %addr)       # 16-bit pointer arithmetic
    Load(%addr,             pixels)      # read 1 byte

where C is the folded numeric base. The 6502 expresses this with
`LDA $XXXX,X` (absolute,X), one instruction of 3 bytes.

This pass finds the three instructions and rewrites them to
`IndexedConstLoad(address, index, dst)`, which `tac_to_asm` lowers as

    Mov(uchar_idx, A)               ; LDA index
    Mov(A, X)                       ; TAX
    Mov(IndexedAbs(C, X), A)        ; LDA $C,X
    Mov(A, dst)                     ; STA dst

which is 4 instructions and 8 bytes, against about 10 instructions for
the setup of DPTR and the indirect-Y load.

The conditions are those of `recognize_indexed_store`, for the other
direction.

  * The Load's `src_ptr` is a temporary `%addr` read once and defined by
    an `Add` of a Constant and another temporary `%ext`.
  * `%ext` is defined by `ZeroExtend(uchar_var, %ext)`.
  * `uchar_var`'s c99 type is 1 byte (`char`, `signed char`, or
    `unsigned char`).
  * `0 <= C <= 0xFF00`, so that `C + 255` doesn't wrap past $FFFF.
  * The Load's `dst` is a 1-byte Var. A load of several bytes would need
    several `LDA $C+k,X` reads, which no program has needed yet. */
#include "passes/optimization/recognize_indexed_load.h"

#include "bit_precise.h"
#include "errors.h"
#include "passes/optimization/extend_add_chain.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::tac_ast;
using framework::DefUseEnv;
using framework::DefUseRewrite;
using framework::PassContext;

// The rewrite of one `Load(src_ptr=Var, dst=Var)`, which walks back
// through the SSA definitions to the ZeroExtend and the Add of a
// constant. The Load and its producers need not be adjacent.
std::optional<DefUseRewrite> rewrite_load(const ir::Type_instruction& instr, const DefUseEnv& env,
                                          const PassContext& ctx, const UseCounts& uses) {
  const auto* load = std::get_if<ir::Load>(&instr);
  if (load == nullptr) {
    return std::nullopt;
  }
  const auto* addr_var = std::get_if<ir::Var>(&load->src_ptr);
  const auto* load_dst = std::get_if<ir::Var>(&load->dst);
  if (addr_var == nullptr || load_dst == nullptr) {
    return std::nullopt;
  }
  // The loaded value must be 1 byte, for a single LDA $C,X.
  if (!is_1_byte_var(*load_dst, ctx.symbols)) {
    return std::nullopt;
  }
  const auto chain = recognize_extend_add_chain(*addr_var, env, ctx, uses);
  if (!chain) {
    return std::nullopt;
  }
  // absolute,X takes a Constant base from 0 to 0xFF00, so that C + 255
  // is at most $FFFF.
  const auto* base = std::get_if<ir::Constant>(&chain->base);
  if (base == nullptr) {
    return std::nullopt;
  }
  const auto addr_value = bit_precise::raw_value(base->const_);
  if (!addr_value) {
    throw InternalError("recognize_indexed_load: a floating constant as a base address");
  }
  if (!(0 <= *addr_value && *addr_value <= 0xFF00)) {
    return std::nullopt;
  }
  return DefUseRewrite::dropping(ir::IndexedConstLoad{.address = *addr_value,
                                                      .index = chain->idx_var,
                                                      .dst = *load_dst,
                                                      .is_volatile = load->is_volatile},
                                 chain->drop_defs());
}

}  // namespace

ir::Function recognize_indexed_load(ir::Function fn, const PassContext& ctx) {
  const UseCounts uses = count_uses(fn.instructions);
  return framework::run_def_use(std::move(fn), ctx,
                                [&](const ir::Type_instruction& instr, const DefUseEnv& env) {
                                  return rewrite_load(instr, env, ctx, uses);
                                });
}

}  // namespace c6502::passes
