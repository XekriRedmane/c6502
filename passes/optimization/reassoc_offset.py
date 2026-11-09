"""Moves a constant out of an address offset, so the base and the
variable part of the offset are added first.

    Binary(Add, k, x, %off)        # or x + k
    Binary(Add, p, %off, %addr)    # or %off + p

becomes

    Binary(Add, p, x, %addr.ra)
    Binary(Add, %addr.ra, k, %addr)

when `%off` has no other use, `p` isn't a Constant, and all of `p`,
`%off`, and `%addr` have the same width. Addition at a fixed width
wraps, so `p + (x + k)` and `(p + x) + k` are the same value, and the
rewrite takes the same two adds.

It's worth doing for subscripts like `dict[n + 1]`, `dict[n + 2]`,
`dict[n + 3]`. Each one's `p + x` is then the same computation, which
common-subexpression elimination shares, and each `+ k` feeding a
Load or Store becomes an indirect-indexed access with `LDY #k`
(`recognize_indirect_indexed`).

A Constant `p` is left alone, because `reassoc_constants` folds the
two constants of `C + (x + k)` into one instead.
"""

from __future__ import annotations

import tac_ast
from passes.optimization.framework import FixedpointPass, PassContext
from passes.optimization.var_visit import count_uses


class ReassocOffsetConstant(FixedpointPass):
    name = "reassoc_offset_constant"

    def run(self, fn, ctx):
        return reassoc_offset_constant(fn, ctx)


def _width(ctx: PassContext, v: tac_ast.Type_val) -> int | None:
    """Byte width of an integer or pointer Var, or None for anything
    else."""
    import c99_ast
    from passes.optimization.strength_reduction import _scalar_int_size
    if not isinstance(v, tac_ast.Var):
        return None
    sym = ctx.symbols.get(v.name)
    if sym is None:
        return None
    if isinstance(sym.type, c99_ast.Pointer):
        return 2
    return _scalar_int_size(sym.type)


def _split_const(
    a: tac_ast.Type_val, b: tac_ast.Type_val,
) -> tuple[tac_ast.Constant, tac_ast.Var] | None:
    if isinstance(a, tac_ast.Constant) and isinstance(b, tac_ast.Var):
        return a, b
    if isinstance(b, tac_ast.Constant) and isinstance(a, tac_ast.Var):
        return b, a
    return None


def reassoc_offset_constant(
    fn: tac_ast.Function, ctx: PassContext,
) -> tac_ast.Function:
    """Returns `fn` with each eligible `p + (x + k)` rewritten to
    `(p + x) + k`. A no-op outside SSA form."""
    ssa = ctx.ssa_dsts
    if not ssa or ctx.symbols is None:
        return fn
    defs: dict[str, int] = {}
    for i, instr in enumerate(fn.instructions):
        dst = getattr(instr, "dst", None)
        if isinstance(dst, tac_ast.Var) and dst.name in ssa:
            defs[dst.name] = i
    uses = count_uses(fn.instructions)
    rewrites: dict[int, list[tac_ast.Type_instruction]] = {}
    dropped: set[int] = set()
    for i, instr in enumerate(fn.instructions):
        if not (isinstance(instr, tac_ast.Binary)
                and isinstance(instr.op, tac_ast.Add)
                and isinstance(instr.dst, tac_ast.Var)
                and instr.dst.name in ssa):
            continue
        for p, off in ((instr.src1, instr.src2), (instr.src2, instr.src1)):
            if not (isinstance(p, tac_ast.Var)
                    and isinstance(off, tac_ast.Var)
                    and off.name in ssa
                    and uses[off.name] == 1):
                continue
            j = defs.get(off.name)
            if j is None or j in dropped:
                continue
            inner = fn.instructions[j]
            if not (isinstance(inner, tac_ast.Binary)
                    and isinstance(inner.op, tac_ast.Add)):
                continue
            split = _split_const(inner.src1, inner.src2)
            if split is None:
                continue
            k, x = split
            w = _width(ctx, instr.dst)
            if w is None or w != _width(ctx, p) or w != _width(ctx, off):
                continue
            if _width(ctx, x) != w:
                continue
            tmp = f"{instr.dst.name}.ra"
            _register_temp(ctx, like=instr.dst.name, name=tmp)
            rewrites[i] = [
                tac_ast.Binary(
                    op=tac_ast.Add(), src1=p, src2=x,
                    dst=tac_ast.Var(name=tmp),
                ),
                tac_ast.Binary(
                    op=tac_ast.Add(), src1=tac_ast.Var(name=tmp), src2=k,
                    dst=instr.dst,
                ),
            ]
            dropped.add(j)
            break
    if not rewrites:
        return fn
    out: list[tac_ast.Type_instruction] = []
    for i, instr in enumerate(fn.instructions):
        if i in dropped:
            continue
        out.extend(rewrites.get(i, [instr]))
    return tac_ast.Function(
        name=fn.name, is_global=fn.is_global,
        params=list(fn.params), instructions=out,
    )


def _register_temp(ctx: PassContext, like: str, name: str) -> None:
    """Registers `name` as a local with `like`'s type, and as an SSA
    name."""
    from passes.type_checking import LocalAttr, Symbol
    if name not in ctx.symbols:
        ctx.symbols[name] = Symbol(
            type=ctx.symbols.get(like).type, attrs=LocalAttr(),
        )
    ctx.ssa_dsts.add(name)
