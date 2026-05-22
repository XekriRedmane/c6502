"""TAC peephole: fold `Truncate(SignExtend|ZeroExtend(x), u)` →
`Copy(x, u)` (or `Truncate(x, u)` if narrowing past x's width).

# Motivating shape

C99 §6.3.1.1 integer-promotes 1-byte values to `int` whenever
they participate in an arithmetic / subscript / bitwise context.
For an `int8_t i` array subscript like `arr[i]`, the promotion is
`SignExtend(i, %ext)`. The downstream `recognize_indexed_*`
passes (now extended in this branch) collapse the Add + Load
into an `IndexedLoad(arr, %narrow, _)` where `%narrow` is
`Truncate(%ext, %narrow)` — a 1-byte index recovered from the
2-byte extension.

The result: a `SignExtend; ...; Truncate` round-trip whose net
effect is the identity (modulo byte-narrowing). TAC's per-Var
liveness sees `%ext` as alive (the Truncate reads it), so SSA-
DCE can't drop the SignExtend. But the byte-0 of `%ext` equals
`x` by the SignExtend's definition, and that's all the Truncate
reads.

This pass recognizes the pattern and replaces the Truncate with
a Copy (or narrower Truncate when the destination is narrower
than the original source). After the rewrite, `%ext` has no
remaining readers and SSA-DCE drops the SignExtend on the next
iteration. The TAC `IndexedLoad` chain composes through
copy-folding to drop the Copy too, leaving just
`IndexedLoad(arr, i, _)`.

# Soundness

`SignExtend(x_n, t_m)` produces an m-byte value whose low n
bytes equal `x_n` and whose high bytes are sign-replicated.
`ZeroExtend(x_n, t_m)` is the same but with zero-replicated
high bytes. In both cases:

  - byte_0..byte_(n-1) of `t_m` equal byte_0..byte_(n-1) of `x_n`.
  - byte_n..byte_(m-1) of `t_m` are derived (don't depend on `x`).

`Truncate(t_m, u_k)` reads byte_0..byte_(k-1) of `t_m`.

  - When k <= n: every byte read comes from `x_n`'s low k bytes.
    The Truncate is equivalent to `Truncate(x_n, u_k)` (or `Copy`
    when k == n).
  - When k > n: some byte read comes from the extend's
    sign-/zero-replicated high bytes. The rewrite would need to
    re-produce those, which isn't a savings. Skip.

# When this fires

The TAC fixed-point loop. After `recognize_indexed_*` introduces
the Truncate (which it does to narrow the index for the
absolute,X / (zp),Y lowering), this pass collapses the
`SignExtend → Truncate` round-trip on the next iteration.

# Soundness gate: `%ext` must be the cast's only def

In SSA each name has exactly one def — so checking that
`Truncate.src`'s def is a Cast is sufficient. (Non-SSA TAC
shapes shouldn't reach this pass; the optimization pipeline runs
post-`to_ssa`.) The Cast's dst doesn't need to be single-use:
multiple downstream Truncates can each rewrite independently and
SSA-DCE collects the now-orphaned Cast.
"""
from __future__ import annotations

import c99_ast
import tac_ast
from passes.optimization.framework import (
    DefUsePass, DefUseEnv, PassContext, MatchResult,
    m_Cast, m_Var,
)


def fold_truncate_extend(
    fn: tac_ast.Function, *,
    symbols=None, ssa_dsts: set[str] | None = None,
) -> tac_ast.Function:
    """Walk `fn`'s instructions; for each `Truncate(t, u)` whose
    src is defined by a `SignExtend(x, t)` or `ZeroExtend(x, t)`,
    rewrite the Truncate to read from `x` directly. The result is
    a Copy when `width(u) == width(x)` (the round-trip is the
    identity) or a narrower Truncate when `width(u) < width(x)`.
    Skip when `width(u) > width(x)` — the rewrite would discard
    information from the Cast's sign/zero-replicated high bytes.

    `ssa_dsts` is the set of names introduced by `to_ssa` (Vars
    that have exactly one def in SSA form). Without `ssa_dsts`,
    the pass is a no-op (safe default for non-SSA TAC)."""
    ctx = PassContext(symbols=symbols, ssa_dsts=ssa_dsts)
    return FoldTruncateExtend().run(fn, ctx)


def _byte_width(v: tac_ast.Var, symbols) -> int | None:
    """The byte width of `v`'s declared type, or None when the
    symbol table isn't available or the type is opaque."""
    if symbols is None:
        return None
    sym = symbols.get(v.name)
    if sym is None:
        return None
    t = sym.type
    while isinstance(t, (c99_ast.Const, c99_ast.Volatile)):
        t = t.referenced_type
    if isinstance(t, (c99_ast.Char, c99_ast.SChar, c99_ast.UChar)):
        return 1
    if isinstance(t, (c99_ast.Int, c99_ast.UInt, c99_ast.Pointer)):
        return 2
    if isinstance(t, (c99_ast.Long, c99_ast.ULong, c99_ast.Float)):
        return 4
    if isinstance(t, (
        c99_ast.LongLong, c99_ast.ULongLong, c99_ast.Double,
    )):
        return 8
    return None


class FoldTruncateExtend(DefUsePass):
    """DefUsePass: matches Truncate(src=Var, dst=Var). For each match,
    walks back to the src's SSA def to find a SignExtend or ZeroExtend;
    if found and widths align, rewrites to Copy or narrower Truncate."""
    name = "fold_truncate_extend"
    pattern = m_Cast(
        kind=tac_ast.Truncate,
        src=m_Var(capture='src'),
        dst=m_Var(capture='dst'),
        capture='truncate',
    )

    def rewrite(self, m: MatchResult, env: DefUseEnv, ctx: PassContext) -> object | None:
        src_var = m.bindings['src']
        dst_var = m.bindings['dst']
        truncate = m.bindings['truncate']

        # src_var must be SSA-renamed (single-def guarantee).
        if ctx.ssa_dsts is None or src_var.name not in ctx.ssa_dsts:
            return None

        # Walk back to find the defining SignExtend or ZeroExtend.
        cast = env.def_of(src_var)
        if not isinstance(cast, (tac_ast.SignExtend, tac_ast.ZeroExtend)):
            return None
        if not isinstance(cast.src, tac_ast.Var):
            return None

        # Width check: rewrite is only sound when dst is no wider than cast.src.
        src_width = _byte_width(cast.src, ctx.symbols)
        dst_width = _byte_width(dst_var, ctx.symbols)
        if src_width is None or dst_width is None:
            return None
        if dst_width > src_width:
            return None
        if dst_width == src_width:
            return tac_ast.Copy(src=cast.src, dst=dst_var)
        return tac_ast.Truncate(src=cast.src, dst=dst_var)
