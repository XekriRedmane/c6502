"""Chooses a constant offset from one pointer, where a join chooses
between that pointer plus one constant or another.

    uint8_t *w = n < 3 ? &E->sword : &E->dagger;
    *w = v;

In SSA form the two arms compute `E + 7` and `E + 9`, and the join
takes one of them:

    .then:  Binary(Add, E, 7, %t1)
            Jump(.end)
    .else:  Binary(Add, E, 9, %t2)
    .end:   %w = Phi(.then: %t1, .else: %t2)
            Store(v, %w)

The pass makes the arms choose the offset and the join add it:

    .end:   %w.o = Phi(.then: 7, .else: 9)      # unsigned char
            %w.e = ZeroExtend(%w.o)
            %w = Binary(Add, E, %w.e)
            Store(v, %w)

`recognize_indirect_indexed` then makes the store `(E),Y` with
`%w.o` in Y, so each arm loads a constant into Y where it had added
a constant to both bytes of the pointer. `calc_combat_strength` and
`apply_shop_effect` in examples/ali_baba choose a member this way.

# Conditions

- Every argument of the Phi is an SSA temporary read only by the Phi
  (for one or more of its edges), defined before it by `p + k` (or `k
  + p`) with the same Var `p` in each and an integer constant `k` from
  0 to 255.
- `p` has the same value at the join as at each add. An SSA-renamed
  `p` has one value. For any other `p`, such as a zero-page pointer
  global, the instructions from the first add to the Phi write no
  memory, call nothing, and don't write `p`, and each jump among them
  goes to a label among them (`_unchanged_between`), so no path from
  an add to the join leaves that range. A `p` whose type is `volatile`
  is left alone, since its read would move.
- The Phi's result and `p` are 2 bytes wide, which a pointer is, and
  the result's one reader is a load or store through it, which
  becomes `(p),Y`. A pointer read more than once, as
  `calc_combat_strength` in examples/ali_baba/combat.c reads
  `*weapon` on either side of a call that may move `ENTITY_PTR`, is
  still computed in 16 bits, and the choice of an offset would only
  add a step.

An add of 2 bytes wraps at 16 bits whichever order its operands are
added in, so `p + ZeroExtend(k)` is `p + k`.

# Where to run

After the fixed point and before `static_indexed_access` and
`recognize_indirect_indexed`, which take `p + ZeroExtend(o)` as an
indexed access.
"""
from __future__ import annotations

import c99_ast
import tac_ast
from passes.optimization.framework import PostFixedpointPass
from passes.optimization.var_visit import count_uses


class PhiOffset(PostFixedpointPass):
    name = "phi_offset"

    def run(self, fn, ctx):
        return phi_offset(fn, ctx)


_JUMPS = (
    tac_ast.Jump, tac_ast.JumpIfTrue, tac_ast.JumpIfFalse,
    tac_ast.JumpIfCmp, tac_ast.JumpIfMasked,
)


def _two_bytes(t) -> bool:
    while isinstance(t, c99_ast.Const):
        t = t.referenced_type
    return isinstance(t, (c99_ast.Pointer, c99_ast.Int, c99_ast.UInt))


def _volatile(t) -> bool:
    while isinstance(t, c99_ast.Const):
        t = t.referenced_type
    return isinstance(t, c99_ast.Volatile)


def _byte_constant(v) -> int | None:
    if not isinstance(v, tac_ast.Constant):
        return None
    value = getattr(v.const, "value", None)
    if isinstance(value, int) and 0 <= value <= 255:
        return value
    return None


def _split(instr):
    """`(p, k)` for `Binary(Add, p, k)` or `Binary(Add, k, p)` with a
    Var `p` and a constant `k` from 0 to 255, else None."""
    if not (isinstance(instr, tac_ast.Binary)
            and isinstance(instr.op, tac_ast.Add)):
        return None
    for p, k in ((instr.src1, instr.src2), (instr.src2, instr.src1)):
        value = _byte_constant(k)
        if isinstance(p, tac_ast.Var) and value is not None:
            return p, value
    return None


# Instructions that write no memory and call nothing. Each writes at
# most its `dst`.
_QUIET = (
    tac_ast.Label, *_JUMPS, tac_ast.Ret, tac_ast.Copy, tac_ast.Binary,
    tac_ast.Unary, tac_ast.ZeroExtend, tac_ast.SignExtend,
    tac_ast.Truncate, tac_ast.Load, tac_ast.IndexedLoad,
    tac_ast.IndirectIndexedLoad, tac_ast.GetAddress, tac_ast.Phi,
)


def _unchanged_between(instrs, lo: int, hi: int, p, ssa) -> bool:
    """Whether every path from `instrs[lo]` to `instrs[hi]` leaves `p`
    unchanged. An SSA name has one value. For any other `p`, the
    instructions from `lo` to `hi` must write no memory, call nothing,
    and not write `p`, and every jump among them must go to a label
    among them, so that no path leaves the range and comes back."""
    if p.name in ssa:
        return True
    labels = {ins.name for ins in instrs[lo:hi + 1]
              if isinstance(ins, tac_ast.Label)}
    for ins in instrs[lo:hi]:
        if not isinstance(ins, _QUIET):
            return False
        if isinstance(ins, (tac_ast.Load, tac_ast.IndexedLoad,
                            tac_ast.IndirectIndexedLoad)) and getattr(
                ins, "is_volatile", False):
            return False
        dst = getattr(ins, "dst", None)
        if isinstance(dst, tac_ast.Var) and dst.name == p.name:
            return False
        target = getattr(ins, "target", None)
        if isinstance(ins, _JUMPS) and target not in labels:
            return False
    return True


def _one_access(instrs, d, uses) -> bool:
    """Whether the only reader of `d` is one load or store through it,
    which the indexed-access recognizers can make `(p),Y`. A pointer
    with other readers is still computed in 16 bits, and the choice of
    an offset only adds a step."""
    if uses[d.name] != 1:
        return False
    for ins in instrs:
        if isinstance(ins, tac_ast.Load) and ins.src_ptr == d:
            return True
        if (isinstance(ins, tac_ast.Store) and ins.dst_ptr == d
                and ins.src != d):
            return True
    return False


def phi_offset(fn: tac_ast.Function, ctx) -> tac_ast.Function:
    """Returns `fn` with each eligible Phi of `p + k` values rewritten
    as a Phi of the constants `k` added to `p` at the join. A no-op
    outside SSA form."""
    ssa = ctx.ssa_dsts
    symbols = ctx.symbols
    if not ssa or symbols is None:
        return fn
    instrs = fn.instructions
    defs = {}
    for i, ins in enumerate(instrs):
        dst = getattr(ins, "dst", None)
        if isinstance(dst, tac_ast.Var) and dst.name in ssa:
            defs[dst.name] = i
    uses = count_uses(instrs)
    drop: set[int] = set()
    replace: dict[int, list] = {}
    for i, ins in enumerate(instrs):
        if not (isinstance(ins, tac_ast.Phi) and ins.args
                and isinstance(ins.dst, tac_ast.Var)):
            continue
        sym = symbols.get(ins.dst.name)
        if (sym is None or not _two_bytes(sym.type)
                or not _one_access(instrs, ins.dst, uses)):
            continue
        base = None
        offsets = []
        sites = []
        readers = {}
        for arg in ins.args:
            if isinstance(arg.source, tac_ast.Var):
                readers[arg.source.name] = readers.get(arg.source.name, 0) + 1
        for arg in ins.args:
            src = arg.source
            if not (isinstance(src, tac_ast.Var) and src.name in ssa
                    and uses[src.name] == readers[src.name]
                    and src.name in defs):
                break
            j = defs[src.name]
            split = _split(instrs[j])
            if split is None or j in drop or j > i:
                break
            p, k = split
            if base is None:
                base = p
            elif p.name != base.name:
                break
            offsets.append(k)
            sites.append(j)
        else:
            psym = symbols.get(base.name)
            if (psym is None or not _two_bytes(psym.type)
                    or _volatile(psym.type)
                    or not _unchanged_between(
                        instrs, min(sites), i, base, ssa)):
                continue
            o = _new_temp(ctx, f"{ins.dst.name}.o", c99_ast.UChar())
            e = _new_temp(ctx, f"{ins.dst.name}.e", c99_ast.UInt())
            replace[i] = [
                tac_ast.Phi(dst=o, args=[
                    tac_ast.PhiArg(
                        pred_label=arg.pred_label,
                        source=tac_ast.Constant(
                            const=tac_ast.ConstUChar(value=k)),
                    )
                    for arg, k in zip(ins.args, offsets)
                ]),
            ]
            # The add follows the block's last Phi.
            last = i
            while (last + 1 < len(instrs)
                   and isinstance(instrs[last + 1], tac_ast.Phi)):
                last += 1
            tail = replace.setdefault(-1 - last, [])
            tail.extend([
                tac_ast.ZeroExtend(src=o, dst=e),
                tac_ast.Binary(op=tac_ast.Add(), src1=base, src2=e,
                               dst=ins.dst),
            ])
            drop.update(sites)
    if not replace:
        return fn
    out = []
    for i, ins in enumerate(instrs):
        if i in drop:
            continue
        out.extend(replace.get(i, [ins]))
        out.extend(replace.get(-1 - i, []))
    return tac_ast.Function(
        name=fn.name, is_global=fn.is_global,
        params=list(fn.params), instructions=out,
    )


def _new_temp(ctx, name: str, t) -> tac_ast.Var:
    """Registers the local `name` of type `t` as an SSA name."""
    from passes.type_checking import LocalAttr, Symbol
    ctx.symbols[name] = Symbol(type=t, attrs=LocalAttr())
    ctx.ssa_dsts.add(name)
    return tac_ast.Var(name=name)
