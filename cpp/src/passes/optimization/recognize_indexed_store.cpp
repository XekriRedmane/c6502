/* TAC pass that recognizes the absolute,X store.

After the fold of const static reads, the fold of subscripts of const
arrays, and the reassociation of constant Adds, an expression such as

    hires_page1[interlace_p1_offsets[2] + col] = value;

with `hires_page1` and `interlace_p1_offsets` both const-qualified
statics, lowers to the TAC

    ZeroExtend(col,         %ext)        # col uchar to 2 bytes
    Binary(Add, C, %ext,    %addr)       # 16-bit pointer arithmetic
    Store(value,            %addr)       # write the byte

where C is a numeric Constant, the folded `hires_page1 +
interlace_p1_offsets[2]`. The 6502 has an addressing mode for exactly
this, `STA $XXXX,X` (absolute,X), which adds X to a 16-bit base known
at compile time before storing, in one instruction of 3 bytes.

This pass finds the three instructions and rewrites them to
`IndexedStore(address, index, src)`, which `tac_to_asm` lowers as

    Mov(uchar_var, A)               ; LDA val
    Mov(uchar_index, X)             ; LDX index
    Mov(A, IndexedAbs(C, X))        ; STA $C,X

which is 3 instructions and 7 bytes, against about 11 instructions and
19 bytes.

# Eligibility

The fusion fires when these all hold.

  * The Store's `dst_ptr` is a temporary `%addr` read once and defined
    by an `Add` of a Constant and another temporary `%ext`.
  * `%ext` is defined by a `ZeroExtend(uchar_var, %ext)`. A `SignExtend`
    doesn't qualify, because X is unsigned, and `STA C,X` with a
    negative index writes `C + 256 - |i|` where C means `C - |i|`.
  * `uchar_var`'s c99 type is 1 byte (`char`, `signed char`, or
    `unsigned char`). The high byte of the index is zero, so `STA C,X`
    writes `C + (X & 0xFF) = C + X`.
  * `C + 255 <= 0xFFFF`. The 6502's absolute,X addressing wraps modulo
    0x10000, so a base above $FF00 with X near 255 would address page
    zero, which the C semantics don't mean. The cap `C <= 0xFF00` keeps
    the access inside the 16-bit address space.
  * The stored value is 1 byte: a Var of a 1-byte type or a 1-byte
    constant. A store of several bytes would need several `STA $C+k,X`
    writes, which no program has needed yet.

# Soundness

The single-use checks mean no other reader sees the intermediate
values, so removing the temporaries doesn't change what the program
computes. The check of C + 255 means the absolute,X addressing reaches
the byte the indirect write would have reached. The high byte of the
zero-extended index is zero by the ZeroExtend's definition, so leaving
out the add of the high byte is sound. */
#include "passes/optimization/recognize_indexed_store.h"

#include "bit_precise.h"
#include "errors.h"
#include "passes/optimization/extend_add_chain.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::tac_ast;
using framework::DefUseEnv;
using framework::DefUseRewrite;
using framework::PassContext;

// The rewrite of one `Store(src, dst_ptr=Var)`, which walks back
// through the SSA definitions to the ZeroExtend and the Add of a
// constant. The Store and its producers need not be adjacent.
std::optional<DefUseRewrite> rewrite_store(const ir::Type_instruction& instr,
                                           const DefUseEnv& env, const PassContext& ctx,
                                           const UseCounts& uses) {
  const auto* store = std::get_if<ir::Store>(&instr);
  if (store == nullptr) {
    return std::nullopt;
  }
  const auto* addr_var = std::get_if<ir::Var>(&store->dst_ptr);
  if (addr_var == nullptr) {
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
    throw InternalError("recognize_indexed_store: a floating constant as a base address");
  }
  if (!(0 <= *addr_value && *addr_value <= 0xFF00)) {
    return std::nullopt;
  }
  // The stored value must be 1 byte, for a single STA $C,X.
  if (!is_1_byte_val(store->src, ctx.symbols)) {
    return std::nullopt;
  }
  return DefUseRewrite::dropping(ir::IndexedStore{.address = *addr_value,
                                                  .index = chain->idx_var,
                                                  .src = store->src,
                                                  .is_volatile = store->is_volatile},
                                 chain->drop_defs());
}

}  // namespace

ir::Function recognize_indexed_store(ir::Function fn, const PassContext& ctx) {
  const UseCounts uses = count_uses(fn.instructions);
  return framework::run_def_use(std::move(fn), ctx,
                                [&](const ir::Type_instruction& instr, const DefUseEnv& env) {
                                  return rewrite_store(instr, env, ctx, uses);
                                });
}

}  // namespace c6502::passes
