"""Reads a global just stored from a temporary in place of the temporary.

# What it does

`*++P` on a pointer `P` that isn't SSA-renamed (a global, here one in
zero page) is

    Binary(Add, P, 1, %0.1)
    Copy(%0.1, P)
    Load(%0.1, %1.1)

`%0.1` is read twice, so copy folding can't make the add write `P`
directly, and `tac_to_asm` adds into two zero-page bytes, copies them
to `P`, and loads through the copy, 14 bytes more than `P++; *P`. This
pass makes the load read `P`:

    Binary(Add, P, 1, %0.1)
    Copy(%0.1, P)
    Load(P, %1.1)

after which copy folding gives `Binary(Add, P, 1, P)`, which the INC
peephole turns into `INC P; BNE; INC P+1`, and the load is `LDA
(P),Y`.

# When it applies

- `Copy(T, G)` comes right after the `Binary` or `Unary` that defines
  `T`, an SSA-renamed name, and `G` is a variable of the same type (or
  both are pointers) that isn't SSA-renamed and isn't volatile. A `T`
  that is a `ZeroExtend` of a byte is left alone, since `tac_to_asm`
  lowers it with its high byte as 0 and has no such fact about `G`.
  Zork's `main` grew by 2 bytes when `disk_sector`, stored from such a
  `T`, was read in its place.
- `T` has exactly one read besides the `Copy`, in an instruction `U`
  later in the same block, with no label or jump between. `U` isn't a
  `Phi`, an extend, or a `Truncate`, and when it loads or stores
  through a pointer, `T` is that pointer and nothing else. A
  narrowing pass looks through an extend to its source, and a byte
  comparison is narrowed through a `ZeroExtend` of a global only when
  the global is unchanged up to the comparison. In Ali Baba's
  `handle_event_slot` a call came between the two.
- No instruction between the `Copy` and `U` writes `G` by name, stores
  through a pointer or at a fixed address, stores into `G` as an
  object (`IndexedSymbolStore`), or calls a function.

`G` then holds the value of `T` at `U`, so `U` reads the same value.
`T` is left with the `Copy` as its only reader, and copy folding,
later in the same sweep, removes it. A read of `G` in place of `T`
costs at most a byte more (an absolute address in place of a zero-page
one), and the store of `T` that goes with it saves two or more.
"""

from __future__ import annotations

from collections import Counter

import c99_ast
import tac_ast
from passes.optimization.framework import FixedpointPass, PassContext
from passes.optimization.var_visit import defs_in, rewrite_uses_in, uses_in


_ENDS_SCAN = (
    tac_ast.Label, tac_ast.Jump, tac_ast.JumpIfTrue, tac_ast.JumpIfFalse,
    tac_ast.JumpIfCmp, tac_ast.JumpIfMasked, tac_ast.Ret,
    tac_ast.FunctionCall, tac_ast.IndirectCall, tac_ast.Store,
    tac_ast.IndexedStore, tac_ast.IndirectIndexedStore,
)


def _plain_type(name: str, symbols):
    sym = symbols.get(name)
    return getattr(sym, "type", None)


def _is_volatile(t) -> bool:
    while isinstance(t, c99_ast.Const):
        t = t.referenced_type
    return isinstance(t, c99_ast.Volatile)


def _strip(t):
    while isinstance(t, (c99_ast.Const, c99_ast.Volatile)):
        t = t.referenced_type
    return t


def _through_pointer(ins, t: str) -> bool:
    """True iff `ins` reads `t` only as the pointer it loads or stores
    through."""
    if isinstance(ins, tac_ast.Load):
        ptr, others = ins.src_ptr, []
    elif isinstance(ins, tac_ast.Store):
        ptr, others = ins.dst_ptr, [ins.src]
    elif isinstance(ins, tac_ast.IndirectIndexedLoad):
        ptr, others = ins.ptr, [ins.index]
    elif isinstance(ins, tac_ast.IndirectIndexedStore):
        ptr, others = ins.ptr, [ins.index, ins.src]
    else:
        return not isinstance(ins, (
            tac_ast.Phi, tac_ast.ZeroExtend, tac_ast.SignExtend,
            tac_ast.Truncate))
    return (isinstance(ptr, tac_ast.Var) and ptr.name == t
            and not any(isinstance(v, tac_ast.Var) and v.name == t
                        for v in others))


def read_stored_globals(fn: tac_ast.Function, ctx: PassContext):
    """Applies the rewrite in the module docstring wherever it meets
    its conditions."""
    if ctx.symbols is None or ctx.ssa_dsts is None:
        return fn
    instrs = list(fn.instructions)
    reads: Counter = Counter()
    for ins in instrs:
        for u in uses_in(ins):
            reads[u.name] += 1
    changed = False
    for i in range(1, len(instrs)):
        copy = instrs[i]
        if not (isinstance(copy, tac_ast.Copy)
                and isinstance(copy.src, tac_ast.Var)
                and isinstance(copy.dst, tac_ast.Var)):
            continue
        t, g = copy.src.name, copy.dst.name
        if (t not in ctx.ssa_dsts or g in ctx.ssa_dsts or reads[t] != 2
                or not isinstance(instrs[i - 1], (tac_ast.Binary,
                                                  tac_ast.Unary))
                or not any(d.name == t for d in defs_in(instrs[i - 1]))):
            continue
        tt, gt = _plain_type(t, ctx.symbols), _plain_type(g, ctx.symbols)
        if tt is None or gt is None or _is_volatile(gt):
            continue
        tt, gt = _strip(tt), _strip(gt)
        if tt != gt and not (isinstance(tt, c99_ast.Pointer)
                             and isinstance(gt, c99_ast.Pointer)):
            continue
        for j in range(i + 1, len(instrs)):
            ins = instrs[j]
            if any(u.name == t for u in uses_in(ins)):
                if not _through_pointer(ins, t):
                    break
                instrs[j] = rewrite_uses_in(
                    ins, lambda v: copy.dst
                    if isinstance(v, tac_ast.Var) and v.name == t else v)
                reads[t] -= 1
                changed = True
                break
            if isinstance(ins, _ENDS_SCAN):
                break
            if isinstance(ins, tac_ast.IndexedSymbolStore) and ins.name == g:
                break
            if any(d.name == g for d in defs_in(ins)):
                break
    if not changed:
        return fn
    return tac_ast.Function(
        name=fn.name, is_global=fn.is_global,
        params=list(fn.params), instructions=instrs,
    )


class ReadStoredGlobals(FixedpointPass):
    name = "read_stored_globals"

    def run(self, fn, ctx):
        return read_stored_globals(fn, ctx)
