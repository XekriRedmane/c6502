"""TAC pass: a byte-indexed access to a static object, with a constant
offset, as `IndexedLoad` / `IndexedSymbolStore`.

# What it does

`prompts[which].answer` on `struct prompt prompts[3]` (16-byte
records, `answer` at byte 15) lowers to

    GetAddress(prompts, %g)
    Binary(LeftShift, which, 4, %b)       ; byte-scaled index
    ZeroExtend(%b, %z)
    Binary(Add, %g, %z, %a)
    Binary(Add, %a, 15, %p)
    Load(%p, %key)

which, through a pointer, becomes `LDY #15; LDA (ptr),Y` with the
pointer built in two zero-page bytes first. This pass turns the access
into

    IndexedLoad(prompts, %b, %key, offset=15)

which `tac_to_asm` lowers as `LDX b; LDA prompts+15,X`. The same goes
for a `Store` (as `IndexedSymbolStore`), and for an
`IndirectIndexedLoad` / `IndirectIndexedStore` whose index is a
constant.

The rewrite is exact: `abs,X` adds the index to the full 16-bit
address `prompts+15`, so it reads `prompts + 15 + b` as the `Load`
did, with no bound on the object's size.

A pointer that's passed on rather than read through,
`cout_string(&prompts[which].answer, 1)`, is `(prompts + z) + 15`.
The pass reassociates that to `(prompts + 15) + z`, so the constant
part is one link-time address, `#<(prompts+15)`
(`passes/label_offset_fold.py` folds the add of a label's halves and
a constant), and the value takes one 16-bit add.

# Pattern

  * The address is `GetAddress(o) + ZeroExtend(b)`, optionally plus a
    constant, with `o` a static object and `b` one byte. Each
    intermediate is an SSA value.
  * Any `Load` / `Store` / `IndirectIndexedLoad` / `IndirectIndexedStore`
    through it is rewritten; the address computation is left for
    dead-code elimination when nothing else uses it.

# Where it runs

Once, after the fixed-point loop and before `recognize_indirect_indexed`.
"""

from __future__ import annotations

import tac_ast
from passes.optimization.framework import PostFixedpointPass
from passes.type_checking import StaticAttr, Symbol


class StaticIndexedAccess(PostFixedpointPass):
    name = "static_indexed_access"

    def run(self, fn, ctx):
        return static_indexed_access(fn, ctx)


def static_indexed_access(fn: tac_ast.Function, ctx) -> tac_ast.Function:
    ssa = ctx.ssa_dsts
    symbols = ctx.symbols
    if not ssa or symbols is None:
        return fn
    defs: dict[str, tac_ast.Type_instruction] = {}
    for ins in fn.instructions:
        dst = getattr(ins, "dst", None)
        if isinstance(dst, tac_ast.Var) and dst.name in ssa:
            defs[dst.name] = ins

    def def_of(v):
        if isinstance(v, tac_ast.Var) and v.name in ssa:
            return defs.get(v.name)
        return None

    def one_byte(v) -> bool:
        if isinstance(v, tac_ast.Constant):
            return isinstance(v.const, (tac_ast.ConstChar, tac_ast.ConstUChar))
        sym = symbols.get(getattr(v, "name", None))
        return sym is not None and _byte_type(sym.type)

    def static_base(v):
        """`(object, byte index)` when `v` is `GetAddress(o) +
        ZeroExtend(b)` for a static `o` and a byte `b`."""
        add = def_of(v)
        if not (isinstance(add, tac_ast.Binary)
                and isinstance(add.op, tac_ast.Add)):
            return None
        for g, z in ((add.src1, add.src2), (add.src2, add.src1)):
            ga, ze = def_of(g), def_of(z)
            if not (isinstance(ga, tac_ast.GetAddress)
                    and isinstance(ga.operand, tac_ast.Var)
                    and isinstance(ze, tac_ast.ZeroExtend)
                    and one_byte(ze.src)):
                continue
            sym = symbols.get(ga.operand.name)
            if sym is not None and isinstance(sym.attrs, StaticAttr):
                return ga.operand.name, ze.src, g, z
        return None

    def address(v):
        """`(object, byte index, offset)` for the address `v`."""
        base = static_base(v)
        if base is not None:
            return base[0], base[1], 0
        add = def_of(v)
        if isinstance(add, tac_ast.Binary) and isinstance(add.op, tac_ast.Add):
            for a, k in ((add.src1, add.src2), (add.src2, add.src1)):
                off = _const(k)
                base = static_base(a)
                if off is not None and base is not None:
                    return base[0], base[1], off
        return None

    out: list[tac_ast.Type_instruction] = []
    changed = False
    for ins in fn.instructions:
        new = None
        match ins:
            case tac_ast.Load(src_ptr=p, dst=d, is_volatile=vol):
                hit = address(p)
                if hit is not None:
                    new = tac_ast.IndexedLoad(
                        name=hit[0], index=hit[1], dst=d,
                        is_volatile=vol, offset=hit[2],
                    )
            case tac_ast.Store(src=s, dst_ptr=p, is_volatile=vol):
                hit = address(p)
                if hit is not None:
                    new = tac_ast.IndexedSymbolStore(
                        name=hit[0], index=hit[1], src=s,
                        is_volatile=vol, offset=hit[2],
                    )
            case tac_ast.IndirectIndexedLoad(
                ptr=p, index=j, dst=d, is_volatile=vol,
            ) if _const(j) is not None:
                hit = address(p)
                if hit is not None:
                    new = tac_ast.IndexedLoad(
                        name=hit[0], index=hit[1], dst=d,
                        is_volatile=vol, offset=hit[2] + _const(j),
                    )
            case tac_ast.IndirectIndexedStore(
                ptr=p, index=j, src=s, is_volatile=vol,
            ) if _const(j) is not None:
                hit = address(p)
                if hit is not None:
                    new = tac_ast.IndexedSymbolStore(
                        name=hit[0], index=hit[1], src=s,
                        is_volatile=vol, offset=hit[2] + _const(j),
                    )
            case tac_ast.Binary(op=tac_ast.Add(), src1=s1, src2=s2, dst=d):
                new = _reassociate(ins, s1, s2, d, static_base, ctx)
                if new is not None:
                    out.extend(new)
                    changed = True
                    continue
        if new is not None:
            out.append(new)
            changed = True
        else:
            out.append(ins)
    if not changed:
        return fn
    return tac_ast.Function(
        name=fn.name, is_global=fn.is_global,
        params=list(fn.params), instructions=out,
    )


def _reassociate(ins, s1, s2, d, static_base, ctx):
    """`(g + z) + k` as `(g + k) + z` for `g` the address of a static
    object, so `g + k` is one link-time address. Returns the two
    instructions, or None."""
    for a, k in ((s1, s2), (s2, s1)):
        off = _const(k)
        if off is None or off == 0:
            continue
        base = static_base(a)
        if base is None:
            continue
        _, _, g, z = base
        sym = ctx.symbols.get(d.name) if isinstance(d, tac_ast.Var) else None
        if sym is None:
            return None
        name = f"{d.name}.base"
        n = 0
        while name in ctx.symbols:
            n += 1
            name = f"{d.name}.base{n}"
        ctx.symbols[name] = Symbol(type=sym.type, attrs=sym.attrs)
        ctx.ssa_dsts.add(name)
        t = tac_ast.Var(name=name)
        return [
            tac_ast.Binary(op=tac_ast.Add(), src1=g, src2=k, dst=t),
            tac_ast.Binary(op=tac_ast.Add(), src1=t, src2=z, dst=d),
        ]
    return None


def _const(v) -> int | None:
    if isinstance(v, tac_ast.Constant) and isinstance(v.const, (
        tac_ast.ConstChar, tac_ast.ConstUChar, tac_ast.ConstInt,
        tac_ast.ConstUInt, tac_ast.ConstLong, tac_ast.ConstULong,
    )):
        value = v.const.value & 0xFFFF
        return value
    return None


def _byte_type(t) -> bool:
    import c99_ast
    while isinstance(t, (c99_ast.Const, c99_ast.Volatile)):
        t = t.referenced_type
    return isinstance(t, (c99_ast.Char, c99_ast.SChar, c99_ast.UChar))
