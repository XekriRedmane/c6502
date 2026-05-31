"""TAC pass that recognizes the absolute,X-store pattern.

After the const-static-read fold, const-array-subscript fold, and
constant-Add reassociation, an expression like

    hires_page1[interlace_p1_offsets[2] + col] = value;

(with both `hires_page1` and `interlace_p1_offsets` const-qualified
statics) lowers to TAC

    ZeroExtend(col,         %ext)        # col uchar → 2-byte
    Binary(Add, C, %ext,    %addr)        # 16-bit ptr arithmetic
    Store(value,            %addr)        # write the byte

where C is a numeric Constant (the folded `hires_page1 +
interlace_p1_offsets[2]`). The 6502 has an addressing mode that
expresses exactly this: `STA $XXXX,X` (absolute,X), which adds
the X register to a compile-time-known 16-bit base before
storing — single instruction, 3 bytes.

This pass detects the three-instruction TAC pattern and rewrites
to the new `IndexedStore(address, index, src)` instruction, which
`tac_to_asm` lowers as

    Mov(uchar_var, A)               ; LDA val   (or via Reg(A))
    Mov(uchar_index, X)             ; LDX index
    Mov(A, IndexedAbs(C, X))        ; STA $C,X

— a 3-instruction / 7-byte sequence vs the original ~11
instructions / ~19 bytes for the same effect.

# Eligibility

The fusion fires when:

  * The Store's `dst_ptr` is a single-use Pseudo `%addr` defined
    by an `Add` of a Constant and another Pseudo `%ext`.
  * `%ext` is itself a single-use Pseudo defined by a
    `ZeroExtend(uchar_var, %ext)` (or `SignExtend` from a
    nonnegative-domain 1-byte source — but only ZeroExtend is
    handled here; signed 1-byte indices are rare in the
    addressing-mode role, and SignExtend would put a non-zero
    high byte for negative values which breaks the absolute,X
    invariant).
  * `uchar_var`'s c99 type is a 1-byte type (Char / SChar /
    UChar). The high byte of the index is zero, so `STA C,X`
    accesses `C + (X & 0xFF) = C + X`.
  * `C + 255 ≤ 0xFFFF`. The 6502's absolute,X addressing wraps
    modulo 0x10000, so a base above $FF00 with X near 255 would
    address into page zero, not what the C semantics want. The
    cap `C ≤ 0xFF00` keeps the access entirely within the
    16-bit address space.
  * The `Store.src` is a 1-byte typed Var (Char / SChar / UChar
    or a Pointer-typed Constant whose value fits in a byte). A
    multi-byte Store is currently not handled — it'd need
    multiple `STA $C+k,X` writes with carry-thread reasoning,
    deferred until a motivating case appears.

# Soundness

The single-use checks guarantee no other reader observes the
intermediate values, so removing the temps doesn't change
semantics. The C + 255 ≤ 0xFFFF check guarantees the absolute,X
addressing reaches the same byte the original
indirect-pointer write would have. The high byte of the
zero-extended index is provably zero (ZeroExtend produces
exactly that), so omitting the high-byte add is sound.
"""

from __future__ import annotations

import tac_ast
from passes.optimization.var_visit import count_uses
from passes.optimization.framework import (
    DefUsePass, DefUseEnv, Rewrite, PassContext, MatchResult,
    m_Store, m_Var, m_Any,
)
from passes.optimization.extend_add_chain import (
    recognize_extend_add_chain, is_1_byte_val, all_dsts,
)


def recognize_indexed_store(
    fn: tac_ast.Function, *, symbols=None,
) -> tac_ast.Function:
    """Fold each `ZeroExtend|SignExtend; Binary(Add, Const); Store` triple
    (1-byte index, 1-byte value, base in 0..0xFF00) into an `IndexedStore`
    (absolute,X), dropping the two intermediate defs. No-op without
    `symbols` (widths come from the symbol table)."""
    if symbols is None:
        return fn
    return _IMPL.run(fn, PassContext(ssa_dsts=all_dsts(fn), symbols=symbols))


class RecognizeIndexedStore(DefUsePass):
    """DefUsePass: matches Store(src, dst_ptr=Var). For each match,
    walks back twice through the SSA def-chain to find the
    ZeroExtend/SignExtend → Binary(Add, Const, %ext) → Store
    pattern. Uses the full def-idx so the Store and its producers
    need not be index-adjacent."""
    name = "recognize_indexed_store"
    pattern = m_Store(
        src=m_Any(capture='val'),
        dst_ptr=m_Var(capture='addr_var'),
        capture='store_instr',
    )

    def prepare_extra(self, fn, ctx):
        return count_uses(fn.instructions)

    def rewrite(self, m: MatchResult, env: DefUseEnv, ctx: PassContext) -> object | None:
        store_instr = m.bindings['store_instr']
        val = m.bindings['val']
        chain = recognize_extend_add_chain(m.bindings['addr_var'], env, ctx)
        if chain is None:
            return None
        # absolute,X needs a Constant base in 0..0xFF00 (so C+255 ≤ $FFFF).
        if not isinstance(chain.base, tac_ast.Constant):
            return None
        addr_value = chain.base.const.value
        if not (0 <= addr_value <= 0xFF00):
            return None
        # The stored value must be 1-byte (a single STA $C,X).
        if not is_1_byte_val(val, ctx.symbols):
            return None
        return Rewrite(
            replacement=tac_ast.IndexedStore(
                address=addr_value, index=chain.idx_var, src=val,
                is_volatile=store_instr.is_volatile,
            ),
            drop_defs=(chain.addr_var, chain.ext_var),
        )


_IMPL = RecognizeIndexedStore()
