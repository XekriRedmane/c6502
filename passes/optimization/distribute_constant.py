"""Multiplies a widened byte before adding a constant to it.

    ZeroExtend(n, %x)              # n is one byte
    Binary(Add | Subtract, %x, c, %t)
    Binary(Multiply, %t, k, %m)    # or k * %t

becomes

    ZeroExtend(n, %x)
    Binary(Multiply, %x, k, %m.dc)
    Binary(Add, %m.dc, c * k, %m)  # Subtract for a Subtract

when `%t` has no other use and `c` and `k` are integer constants.
Arithmetic at a fixed width wraps, so `(x ± c) * k` and `x * k ± c *
k` are the same value. The product is then of a value with one
significant byte, which `strength_reduction` and `tac_to_asm` lower
in fewer instructions: its shifts are byte shifts in A, and a
product they can't make from shifts is an 8-by-8 loop where the
other is a call of `mul16`. The add is one add, as the subtract
was.

`&MENU_DESC_PTR->options[n - 1]` with 9-byte options, in
examples/ali_baba/input.c, was `n - 1` in 16 bits and then three
16-bit shifts and an add. After the rewrite `n * 9` is computed from
the byte `n`, and `reassoc_offset_constant` adds the `-9` after the
pointer.

Runs in the fixed point before strength reduction, which would
otherwise turn the multiply into shifts first.
"""
from __future__ import annotations

import tac_ast
from passes.optimization.framework import FixedpointPass
from passes.optimization.var_visit import count_uses


class DistributeConstant(FixedpointPass):
    name = "distribute_constant"

    def run(self, fn, ctx):
        return distribute_constant(fn, ctx)


_BITS = {
    tac_ast.ConstInt: (16, True), tac_ast.ConstUInt: (16, False),
    tac_ast.ConstLong: (32, True), tac_ast.ConstULong: (32, False),
    tac_ast.ConstLongLong: (64, True), tac_ast.ConstULongLong: (64, False),
}


def _wrap(value: int, cls) -> int:
    bits, signed = _BITS[cls]
    value &= (1 << bits) - 1
    if signed and value >= 1 << (bits - 1):
        value -= 1 << bits
    return value


def _constant(v):
    if isinstance(v, tac_ast.Constant) and type(v.const) in _BITS:
        return v.const
    return None


def distribute_constant(fn: tac_ast.Function, ctx) -> tac_ast.Function:
    """Returns `fn` with each eligible `(x ± c) * k` rewritten to `x * k
    + (±c * k)`. A no-op outside SSA form."""
    ssa = ctx.ssa_dsts
    if not ssa or ctx.symbols is None:
        return fn
    instrs = fn.instructions
    defs = {}
    for i, ins in enumerate(instrs):
        dst = getattr(ins, "dst", None)
        if isinstance(dst, tac_ast.Var) and dst.name in ssa:
            defs[dst.name] = i
    uses = count_uses(instrs)
    rewrites: dict[int, list] = {}
    for i, ins in enumerate(instrs):
        if not (isinstance(ins, tac_ast.Binary)
                and isinstance(ins.op, tac_ast.Multiply)
                and isinstance(ins.dst, tac_ast.Var)):
            continue
        for t, kv in ((ins.src1, ins.src2), (ins.src2, ins.src1)):
            k = _constant(kv)
            if not (k is not None and isinstance(t, tac_ast.Var)
                    and t.name in ssa and uses[t.name] == 1
                    and t.name in defs):
                continue
            inner = instrs[defs[t.name]]
            if not (isinstance(inner, tac_ast.Binary)
                    and isinstance(inner.op, (tac_ast.Add, tac_ast.Subtract))):
                continue
            c = _constant(inner.src2)
            x = inner.src1
            if c is None and isinstance(inner.op, tac_ast.Add):
                c, x = _constant(inner.src1), inner.src2
            if (c is None or type(c) is not type(k)
                    or not _widened_byte(x, instrs, defs, ctx)):
                continue
            # `x * k - c * k` keeps the subtract, whose top byte
            # `carry_inc` makes `BCS skip; DEC`, where an add of the
            # negated constant would add `#$FF` to it.
            op = inner.op
            product = c.value * k.value
            if product < 0:
                op = (tac_ast.Add() if isinstance(op, tac_ast.Subtract)
                      else tac_ast.Subtract())
                product = -product
            name = _fresh(ctx, ins.dst.name)
            m = tac_ast.Var(name=name)
            rewrites[i] = [
                tac_ast.Binary(op=tac_ast.Multiply(), src1=x, src2=kv, dst=m),
                tac_ast.Binary(
                    op=op, src1=m,
                    src2=tac_ast.Constant(const=type(k)(
                        value=_wrap(product, type(k)))),
                    dst=ins.dst,
                ),
            ]
            break
    if not rewrites:
        return fn
    out = []
    for i, ins in enumerate(instrs):
        out.extend(rewrites.get(i, [ins]))
    return tac_ast.Function(
        name=fn.name, is_global=fn.is_global,
        params=list(fn.params), instructions=out,
    )


def _widened_byte(x, instrs, defs, ctx) -> bool:
    """Whether `x` is an SSA name defined by a `ZeroExtend` of a
    one-byte value."""
    from passes.optimization.extend_add_chain import is_1_byte_val
    if not (isinstance(x, tac_ast.Var) and x.name in defs):
        return False
    d = instrs[defs[x.name]]
    return isinstance(d, tac_ast.ZeroExtend) and is_1_byte_val(d.src, ctx.symbols)


def _fresh(ctx, base: str) -> str:
    """Registers `<base>.dc<k>`, for the first k no symbol has, with
    `base`'s type, as an SSA name."""
    from passes.type_checking import LocalAttr, Symbol
    k = 0
    while f"{base}.dc{k}" in ctx.symbols:
        k += 1
    name = f"{base}.dc{k}"
    ctx.symbols[name] = Symbol(type=ctx.symbols.get(base).type,
                               attrs=LocalAttr())
    ctx.ssa_dsts.add(name)
    return name
