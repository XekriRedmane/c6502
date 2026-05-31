"""TAC pass that recognizes the absolute,X-load pattern.

Mirror of `recognize_indexed_store.py`, but for the analogous Load
shape. After const-static fold + reassoc, an expression like

    pixels = hud_strip_src[y];   // hud_strip_src is `T * const`

(with `hud_strip_src` a const-pointer static folding to a numeric
address) lowers to

    ZeroExtend(y,           %ext)        # y uchar → 2-byte
    Binary(Add, C, %ext,    %addr)        # 16-bit ptr arithmetic
    Load(%addr,             pixels)       # read 1 byte

where C is the folded numeric base. The 6502 expresses this with
`LDA $XXXX,X` (absolute,X) — single instruction, 3 bytes.

This pass detects the three-instruction TAC pattern and rewrites
to the new `IndexedConstLoad(address, index, dst)` instruction,
which `tac_to_asm` lowers as

    Mov(uchar_idx, A)               ; LDA index   (or via Reg(A))
    Mov(A, X)                        ; TAX
    Mov(IndexedAbs(C, X), A)        ; LDA $C,X
    Mov(A, dst)                      ; STA dst

— a 4-instruction / 8-byte sequence vs the original ~10
instructions for the same effect (DPTR setup + indirect-Y load).

The eligibility checks are the same as `recognize_indexed_store`
modulo direction:
  * The Load's `src_ptr` is a single-use Pseudo `%addr` defined
    by an `Add` of a Constant and another Pseudo `%ext`.
  * `%ext` is itself a single-use Pseudo defined by
    `ZeroExtend(uchar_var, %ext)`.
  * `uchar_var`'s c99 type is 1 byte (Char / SChar / UChar).
  * `0 ≤ C ≤ 0xFF00` (so `C + 255` doesn't wrap past $FFFF).
  * The Load's `dst` is a 1-byte typed Var. Multi-byte loads
    would need multiple `LDA $C+k,X` reads — deferred until a
    motivating case appears.
"""
from __future__ import annotations

import tac_ast
from passes.optimization.var_visit import count_uses
from passes.optimization.framework import (
    DefUsePass, DefUseEnv, Rewrite, PassContext, MatchResult,
    m_Load, m_Var,
)
from passes.optimization.extend_add_chain import (
    recognize_extend_add_chain, is_1_byte_var, all_dsts,
)


def recognize_indexed_load(
    fn: tac_ast.Function, *, symbols=None,
) -> tac_ast.Function:
    """Fold each `ZeroExtend|SignExtend; Binary(Add, Const); Load` triple
    (1-byte index, 1-byte dst, base in 0..0xFF00) into an
    `IndexedConstLoad` (absolute,X), dropping the two intermediate defs.
    No-op without `symbols` (widths come from the symbol table)."""
    if symbols is None:
        return fn
    return _IMPL.run(fn, PassContext(ssa_dsts=all_dsts(fn), symbols=symbols))


class RecognizeIndexedLoad(DefUsePass):
    """DefUsePass: matches Load(src_ptr=Var, dst=Var). For each match,
    walks back twice through the SSA def-chain to find the
    ZeroExtend/SignExtend → Binary(Add, Const, %ext) → Load
    pattern. Uses the full def-idx so the Load and its producers
    need not be index-adjacent."""
    name = "recognize_indexed_load"
    pattern = m_Load(
        src_ptr=m_Var(capture='addr_var'),
        dst=m_Var(capture='load_dst'),
        capture='load_instr',
    )

    def prepare_extra(self, fn, ctx):
        return count_uses(fn.instructions)

    def rewrite(self, m: MatchResult, env: DefUseEnv, ctx: PassContext) -> object | None:
        load_instr = m.bindings['load_instr']
        load_dst = m.bindings['load_dst']
        # The loaded dst must be 1-byte (a single LDA $C,X).
        if not is_1_byte_var(load_dst, ctx.symbols):
            return None
        chain = recognize_extend_add_chain(m.bindings['addr_var'], env, ctx)
        if chain is None:
            return None
        # absolute,X needs a Constant base in 0..0xFF00 (so C+255 ≤ $FFFF).
        if not isinstance(chain.base, tac_ast.Constant):
            return None
        addr_value = chain.base.const.value
        if not (0 <= addr_value <= 0xFF00):
            return None
        return Rewrite(
            replacement=tac_ast.IndexedConstLoad(
                address=addr_value, index=chain.idx_var, dst=load_dst,
                is_volatile=load_instr.is_volatile,
            ),
            drop_defs=(chain.addr_var, chain.ext_var),
        )


_IMPL = RecognizeIndexedLoad()
