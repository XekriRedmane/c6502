"""TAC pass: compute an operation of a Phi of constants in the Phi's
arguments.

`?:` with constant arms joins them in a Phi, and the arithmetic after
the join runs on whichever arrived:

    Phi(%c, [3 from .then, 2 from .else])
    Truncate(%c, %c.lo)
    Binary(+, %c.lo, 1uc, B)

C's `B = (flag ? 3 : 2) + 1;` lowered that way loads 3 or 2 and then
adds 1. Every argument is a constant, so the operation's result on
each path is a constant too, and the Phi can join those instead:

    Phi(%c.lo, [3uc from .then, 2uc from .else])
    Binary(+, %c.lo, 1uc, B)

and then

    Phi(%c.lo.p0, [4uc from .then, 3uc from .else])
    Copy(%c.lo.p0, B)

which `tac_to_asm` lowers as a load of 4 or 3 and a store.

# The rule

A `Phi` whose arguments are all integer constants, and whose
destination is an SSA name read by exactly one instruction `U`, takes
`U`'s operation when `U` is a `Unary`, a `Binary` whose other operand
is a constant, or a `Truncate`, and constant folding
(`constant_folding._fold`) folds `U` for every argument. The Phi's
destination becomes `U`'s when that is an SSA name, and `U` goes;
otherwise the Phi gets a new temporary of `U`'s destination's type and
`U` becomes a copy of it into the destination, which keeps the store
where it was.

`U` reads the Phi's destination, so the Phi's block dominates `U`,
and every reader of `U`'s destination comes after `U`, so the Phi
still defines that value before every read. The Phi computes the
value on every path into its block, where `U` may have run on fewer,
but the value is a constant on each path and computing it has no
effect, so that is harmless. An operation that doesn't fold for some
argument, such as a divide by zero, leaves the Phi alone.
"""
from __future__ import annotations

import tac_ast
from passes.optimization.framework import FixedpointPass, PassContext
from passes.optimization.var_visit import count_uses, rewrite_uses_in, uses_in

# An extend isn't among them. It would only widen the Phi, and a Phi
# of a byte's constants is what other rules recognize, such as the
# shift of a 0 or 1 through the carry (`tac_to_asm`'s `rotate_in`).
_FOLDABLE = (tac_ast.Unary, tac_ast.Binary, tac_ast.Truncate)


def _int_const(v) -> bool:
    from passes.optimization.constant_folding import _is_integer_const
    return isinstance(v, tac_ast.Constant) and _is_integer_const(v.const)


def fold_into_constant_phis(
    fn: tac_ast.Function, ctx: PassContext,
) -> tac_ast.Function:
    """Returns `fn` with the first operation of a Phi of constants that
    folds moved into the Phi's arguments (see the module docstring),
    or `fn` itself when none does."""
    from passes.optimization.constant_folding import _fold
    if ctx.symbols is None or ctx.ssa_dsts is None:
        return fn
    instrs = fn.instructions
    counts = count_uses(instrs)
    for i, phi in enumerate(instrs):
        if not (isinstance(phi, tac_ast.Phi) and phi.args
                and isinstance(phi.dst, tac_ast.Var)
                and phi.dst.name in ctx.ssa_dsts
                and counts.get(phi.dst.name, 0) == 1
                and all(_int_const(a.source) for a in phi.args)):
            continue
        j = next(
            (k for k, ins in enumerate(instrs)
             if any(u.name == phi.dst.name for u in uses_in(ins))),
            None,
        )
        if j is None:
            continue
        user = instrs[j]
        if not isinstance(user, _FOLDABLE) or not isinstance(
                user.dst, tac_ast.Var):
            continue
        if isinstance(user, tac_ast.Binary) and not any(
                _int_const(v) for v in (user.src1, user.src2)):
            continue
        args = []
        for a in phi.args:
            folded = _fold(
                rewrite_uses_in(
                    user, lambda v, s=a.source: s if v == phi.dst else v,
                ),
                ctx.symbols,
            )
            if not (isinstance(folded, tac_ast.Copy)
                    and _int_const(folded.src)):
                break
            args.append(tac_ast.PhiArg(pred_label=a.pred_label,
                                       source=folded.src))
        else:
            out = list(instrs)
            if user.dst.name in ctx.ssa_dsts:
                out[i] = tac_ast.Phi(dst=user.dst, args=args)
                del out[j]
            else:
                tmp = _temp(ctx, user.dst.name)
                out[i] = tac_ast.Phi(dst=tmp, args=args)
                out[j] = tac_ast.Copy(src=tmp, dst=user.dst)
            return tac_ast.Function(
                name=fn.name, is_global=fn.is_global,
                params=list(fn.params), instructions=out,
            )
    return fn


def _temp(ctx: PassContext, like: str) -> tac_ast.Var:
    """A new SSA local of `like`'s type."""
    from passes.type_checking import LocalAttr, Symbol
    k = 0
    while f"{like}.p{k}" in ctx.symbols:
        k += 1
    name = f"{like}.p{k}"
    import c99_ast
    t = ctx.symbols.get(like).type
    while isinstance(t, (c99_ast.Const, c99_ast.Volatile)):
        t = t.referenced_type
    ctx.symbols[name] = Symbol(type=t, attrs=LocalAttr())
    ctx.ssa_dsts.add(name)
    return tac_ast.Var(name=name)


class FoldIntoConstantPhi(FixedpointPass):
    name = "fold_into_constant_phi"

    def run(self, fn, ctx):
        return fold_into_constant_phis(fn, ctx)
