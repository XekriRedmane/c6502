"""TAC pass that recognizes the indirect-(zp),Y access pattern.

For `ptr[i]` where `ptr` is a Pointer-typed local (or zp_abi parameter)
and `i` is a 1-byte typed integer, c99_to_tac lowers to:

    ZeroExtend(i,            %ext)        # i uchar → 2-byte
    Binary(Add, ptr, %ext,   %addr)       # 16-bit ptr arithmetic
    Load(%addr,              dst)         # read 1 byte

The 6502's indirect-indexed addressing mode `(zp),Y` does the same
`ptr + Y` arithmetic for free, with `Y = i`. So when `ptr` is (or
ends up at) a zero-page address, this collapses to:

    LDY i ; LDA (ptr_zp),Y ; STA dst        (~3 instructions / 5 bytes)

vs the original ~10 instructions / ~21 bytes (16-bit Add chain plus
DPTR setup plus indirect-Y read with Y=0). Even when ptr can't be
ZP-resident, lowering through DPTR with `Y=i` (instead of
`Y=0` and a precomputed sum) lets the asm-level forward copy
propagation hoist the loop-invariant DPTR setup out of an unrolled
body, which the current shape can't do because the DPTR contents
include the per-iteration index.

This pass detects the three-instruction TAC pattern and rewrites
to a single `IndirectIndexedLoad(ptr, index, dst)` (or the Store
mirror). The pattern recognizer doesn't need to know whether ptr
will end up in ZP — that decision happens in tac_to_asm and asm
regalloc.

# Eligibility

The fusion fires when:

  * The Load's `src_ptr` (or Store's `dst_ptr`) is a single-use
    Pseudo `%addr` defined by an `Add` of a Var `ptr` and another
    Pseudo `%ext`.
  * `ptr` is a Var (not a Constant — that pattern is the
    `recognize_indexed_load` / `_store` case).
  * `%ext` is itself a single-use Pseudo defined by
    `ZeroExtend(index_var, %ext)`.
  * `index_var`'s c99 type is a 1-byte type (Char / SChar / UChar).
    The high byte of the zero-extended index is provably zero, so
    setting `Y = index_var` is equivalent to adding `index_var` to
    the pointer's low byte and propagating any carry — i.e., the
    6502's `(zp),Y` semantics match exactly.
  * The Load's `dst` (or Store's `src`) is a 1-byte typed val. A
    multi-byte access would require multiple `LDA (zp),Y; INY` pairs
    with carry-safe iteration — deferred until a motivating case
    appears.

The c99 type of `ptr` is NOT checked here: the type checker
already verified the Binary is pointer arithmetic (because the
result feeds a Load/Store), and the only legal addends are a
pointer plus an integer. The Multiply that pointer arithmetic
would emit for `sizeof(pointee) > 1` would break the recognizer's
shape (it'd see `Binary(Multiply, ...)` instead of the ZeroExtend
result directly), so the by-1 scaling condition is implicit in
the structural match.

# Soundness

The single-use checks guarantee no other reader observes the
intermediate values, so removing the temps doesn't change
semantics. The high byte of `ZeroExtend(uchar, %ext)` is provably
zero, so `(ptr + ZeroExtend(i))` equals `(ptr + i)` as a 16-bit
sum, equals `*(ptr),Y` with `Y = i` on the 6502 (which adds Y to
the low byte and carries into the high byte). The 1-byte access
constraint matches the single LDA / STA the lowering emits.
"""

from __future__ import annotations

import tac_ast
from passes.optimization.var_visit import count_uses
from passes.optimization.framework import (
    DefUsePass, DefUseEnv, Rewrite, PostFixedpointPass, PassContext,
    MatchResult, m_Store, m_Load, m_Var, m_Any, m_OneOf,
)
from passes.optimization.extend_add_chain import (
    recognize_extend_add_chain, is_1_byte_var, is_1_byte_val, all_dsts,
)


def recognize_indirect_indexed(
    fn: tac_ast.Function, *, symbols=None,
) -> tac_ast.Function:
    """Walk `fn`'s instructions; for each foldable triple
    (`ZeroExtend; Binary(Add); Load|Store`), splice in an
    `IndirectIndexedLoad` / `IndirectIndexedStore` and drop the
    two intermediate def instructions. Without `symbols`, the pass
    is a no-op (we need symbol-table types to verify operand widths)."""
    if symbols is None:
        return fn
    return _IMPL.run(
        fn,
        PassContext(ssa_dsts=all_dsts(fn), symbols=symbols),
    )


def _resolves_to_constant(
    v: tac_ast.Var,
    def_idx: dict[str, int],
    all_instrs: list,
) -> bool:
    """True iff `v`'s SSA def chain ends in a Constant. Follows
    `Copy` chains of arbitrary depth; gives up on non-Copy defs.
    Used to defer to the IndexedLoad/Store recognizers when the
    pointer-side operand is just a Var-wrapped Constant."""
    seen: set[str] = set()
    cur = v
    while True:
        if cur.name in seen:
            return False
        seen.add(cur.name)
        idx = def_idx.get(cur.name)
        if idx is None:
            return False
        d = all_instrs[idx]
        if not isinstance(d, tac_ast.Copy):
            return False
        src = d.src
        if isinstance(src, tac_ast.Constant):
            return True
        if isinstance(src, tac_ast.Var):
            cur = src
            continue
        return False


class _RecognizeIndirectIndexedDefUse(DefUsePass):
    """DefUsePass: matches Load(src_ptr=Var) or Store(dst_ptr=Var) at
    the use site. For each match, walks back twice through the SSA
    def-chain to find the ZeroExtend/SignExtend → Binary(Add, ptr,
    %ext) → Load/Store pattern. Uses the full def-idx so the use and
    its producers need not be index-adjacent.

    Returns Rewrite(replacement, drop_defs=(addr_var, ext_var)) so the
    two intermediate defs are dropped atomically in the same rebuild
    pass — no subsequent DSE pass is needed to clean them up."""
    name = "recognize_indirect_indexed"
    pattern = m_OneOf(
        m_Load(
            src_ptr=m_Var(capture='addr_var'),
            dst=m_Var(capture='load_dst'),
            capture='load_instr',
        ),
        m_Store(
            src=m_Any(capture='store_src'),
            dst_ptr=m_Var(capture='addr_var'),
            capture='store_instr',
        ),
    )

    def prepare_extra(self, fn, ctx):
        return count_uses(fn.instructions)

    def rewrite(self, m: MatchResult, env: DefUseEnv, ctx: PassContext) -> object | None:
        chain = recognize_extend_add_chain(m.bindings['addr_var'], env, ctx)
        if chain is None:
            return None
        # (zp),Y needs a pointer Var base — a Constant base is the
        # absolute,X case handled by recognize_indexed_store/load.
        ptr_var = chain.base
        if not isinstance(ptr_var, tac_ast.Var):
            return None
        # Defer to recognize_indexed_* when the ptr resolves to a Constant.
        if _resolves_to_constant(ptr_var, env.def_idx, env.instructions):
            return None
        idx_var = chain.idx_var
        # Determine Load or Store and check the access value's width.
        if 'load_instr' in m.bindings:
            load_dst = m.bindings['load_dst']
            if not is_1_byte_var(load_dst, ctx.symbols):
                return None
            replacement: tac_ast.Type_instruction = tac_ast.IndirectIndexedLoad(
                ptr=ptr_var, index=idx_var, dst=load_dst,
                is_volatile=m.bindings['load_instr'].is_volatile,
            )
        else:
            store_src = m.bindings['store_src']
            if not is_1_byte_val(store_src, ctx.symbols):
                return None
            replacement = tac_ast.IndirectIndexedStore(
                ptr=ptr_var, index=idx_var, src=store_src,
                is_volatile=m.bindings['store_instr'].is_volatile,
            )
        # Drop the two intermediate defs (Binary(Add) and ZeroExtend)
        # atomically — no subsequent DSE pass needed.
        return Rewrite(
            replacement=replacement,
            drop_defs=(chain.addr_var, chain.ext_var),
        )


_IMPL = _RecognizeIndirectIndexedDefUse()


class RecognizeIndirectIndexed(PostFixedpointPass):
    """PostFixedpointPass wrapper. Delegates to _IMPL which uses
    Rewrite.drop_defs to atomically drop the two dead intermediate
    defs (Binary(Add) and ZeroExtend/SignExtend) in the same rebuild
    pass. This is sound even in PostFixedpointPass context — there
    is no need for a subsequent DSE pass."""
    name = "recognize_indirect_indexed"

    def run(self, fn, ctx):
        return _IMPL.run(fn, ctx)
