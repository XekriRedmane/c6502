"""Puts a divide and a remainder of the same operands next to each other.

# What it does

`tac_to_asm` lowers a `/` and a `%` of the same operands with one loop
of subtractions, or one call of a `divmod` helper, when the two come
close together in one straight run with no store or call between them
(`_plan_divmod_pairs`). A store, a call, or a branch between them
gives two loops. Ali Baba's `load_scene_from_disk` stores
`sector % 13` through a pointer before it computes `sector / 13`, and
`render_font_char_direct` divides inside an `if` and takes the
remainder after it:

    JumpIfCmp(LessThan, V, 24uc, .if_end@0)
    Binary(Divide, V, 24uc, Q)
  .if_end@0:
    Binary(Modulo, V, 24uc, R)

This pass computes both into temporaries at one place and leaves a
`Copy` of each where its instruction was:

    Binary(Divide, V, 24uc, %q)
    Binary(Modulo, V, 24uc, %r)
    JumpIfCmp(LessThan, V, 24uc, .if_end@0)
    Copy(%q, Q)
  .if_end@0:
    Copy(%r, R)

A result whose destination is an SSA-renamed name is computed straight
into that name, and nothing is left at its old place.

# Where the pair goes

Let `A` and `B` be the blocks of the two instructions and `D` their
nearest common dominator. When `D` is the block of one of them, the
pair goes at that one, the first of the two in `D`. Otherwise it goes
at the end of `D`, before its jump, and then every path from there to
the function's exit has to pass through `A` or `B`, so the pair never
runs on a path that computed neither.

# When it applies

- The two are `Binary(Divide)` and `Binary(Modulo)` with the same
  `src1`, a variable of 1, 2, 4, or 8 bytes that isn't floating, and
  the same `src2`. A `src2` that is a variable is taken only when both
  are in one block, and a constant one must not be 0, so that the
  pair never divides by zero on a path that didn't.
- Neither destination is an operand, and no operand is volatile, since
  the pair reads each operand once.
- From the pair's new place to each instruction, no instruction writes
  an operand. Every instruction in a block that lies on a path from
  `D` to `A` or `B` counts, and so does every instruction after the
  new place in `D`. An operand that isn't SSA-renamed (a static, an
  object at a fixed address, an address-taken local) can be written
  through a pointer or by a call, so then no store through a pointer,
  `IndexedStore`, store into the operand by name, or call may come
  there either.

The operands then hold the same values at the new place as at each
instruction, and the division has no side effects, so each result is
the value the instruction computed. Each destination is still written
where it was. Pairs whose two instructions are already adjacent are
left alone.
"""

from __future__ import annotations

import c99_ast
import tac_ast
from passes.optimization.cfg import (
    ENTRY_ID, EXIT_ID, build_cfg, immediate_dominators,
)
from passes.optimization.framework import PassContext, PostFixedpointPass
from passes.optimization.var_visit import defs_in


_OPS = (tac_ast.Divide, tac_ast.Modulo)

_MEMORY_WRITES = (
    tac_ast.Store, tac_ast.IndexedStore, tac_ast.IndirectIndexedStore,
    tac_ast.FunctionCall, tac_ast.IndirectCall,
)


def _is_volatile(name: str, symbols) -> bool:
    sym = symbols.get(name)
    t = getattr(sym, "type", None)
    while isinstance(t, c99_ast.Const):
        t = t.referenced_type
    return isinstance(t, c99_ast.Volatile)


def _width(name: str, symbols) -> int | None:
    sym = symbols.get(name)
    t = getattr(sym, "type", None)
    while isinstance(t, (c99_ast.Const, c99_ast.Volatile)):
        t = t.referenced_type
    if isinstance(t, (c99_ast.Char, c99_ast.SChar, c99_ast.UChar)):
        return 1
    if isinstance(t, (c99_ast.Int, c99_ast.UInt)):
        return 2
    if isinstance(t, (c99_ast.Long, c99_ast.ULong)):
        return 4
    if isinstance(t, (c99_ast.LongLong, c99_ast.ULongLong)):
        return 8
    return None


def _candidate(ins) -> bool:
    return (isinstance(ins, tac_ast.Binary) and isinstance(ins.op, _OPS)
            and isinstance(ins.src1, tac_ast.Var)
            and isinstance(ins.src2, (tac_ast.Var, tac_ast.Constant))
            and isinstance(ins.dst, tac_ast.Var))


def _new_name(base: str, symbols) -> str:
    k = 0
    while f"{base}.dm{k}" in symbols:
        k += 1
    return f"{base}.dm{k}"


def _between(cfg, start: int, stops: set[int]) -> set[int]:
    """The blocks reachable from `start`'s successors without passing
    through `start` or a block in `stops`, with the `stops` reached."""
    seen: set[int] = set()
    work = list(cfg.blocks[start].successors)
    while work:
        b = work.pop()
        if b in seen or b == start:
            continue
        seen.add(b)
        if b in stops:
            continue
        work.extend(cfg.blocks[b].successors)
    return seen


def _on_cycle(cfg, block: int, avoid: int) -> bool:
    """True iff `block` can be reached again from its successors
    without passing through `avoid`."""
    seen: set[int] = set()
    work = list(cfg.blocks[block].successors)
    while work:
        b = work.pop()
        if b == block:
            return True
        if b in seen or b == avoid:
            continue
        seen.add(b)
        work.extend(cfg.blocks[b].successors)
    return False


def _reaching(cfg, blocks: set[int], targets: set[int]) -> set[int]:
    """The blocks of `blocks` from which a block of `targets` can be
    reached inside `blocks`."""
    out = set(targets & blocks)
    changed = True
    while changed:
        changed = False
        for b in blocks - out:
            if any(s in out for s in cfg.blocks[b].successors):
                out.add(b)
                changed = True
    return out


def hoist_divmod_pairs(fn: tac_ast.Function, ctx: PassContext):
    """Applies the rewrite in the module docstring to the first pair
    of `fn` that meets its conditions, and repeats until none does."""
    if ctx.symbols is None or ctx.ssa_dsts is None:
        return fn
    for _ in range(64):
        new = _hoist_one(fn, ctx)
        if new is None:
            return fn
        fn = new
    return fn


def _hoist_one(fn: tac_ast.Function, ctx: PassContext):
    symbols, ssa = ctx.symbols, ctx.ssa_dsts
    cfg = build_cfg(fn)
    idom = immediate_dominators(cfg)
    where: list[tuple[int, int]] = []
    for bid in cfg.block_order:
        for k in range(len(cfg.blocks[bid].instructions)):
            where.append((bid, k))
    instrs = list(fn.instructions)

    def dominators(b: int) -> list[int]:
        out = [b]
        while b != ENTRY_ID:
            b = idom[b]
            out.append(b)
        return out

    for i, a in enumerate(instrs):
        if not _candidate(a) or where[i][0] not in idom:
            continue
        operands = {a.src1.name}
        if isinstance(a.src2, tac_ast.Var):
            operands.add(a.src2.name)
        if isinstance(a.src2, tac_ast.Constant):
            if getattr(a.src2.const, "value", None) in (0, None):
                continue
        if (_width(a.src1.name, symbols) is None
                or any(_is_volatile(n, symbols) for n in operands)):
            continue
        for j in range(i + 1, len(instrs)):
            b = instrs[j]
            if not (_candidate(b) and type(b.op) is not type(a.op)
                    and b.src1 == a.src1 and b.src2 == a.src2
                    and where[j][0] in idom):
                continue
            if where[j][0] == where[i][0] and j - i <= 8 and not any(
                    isinstance(instrs[k], _MEMORY_WRITES
                               + (tac_ast.IndexedSymbolStore,))
                    for k in range(i + 1, j)):
                # tac_to_asm's `_plan_divmod_pairs` takes these.
                break
            if {a.dst.name, b.dst.name} & operands:
                break
            new = _try_pair(fn, cfg, idom, dominators, where, i, j,
                            operands, ctx)
            if new is not None:
                return new
            break
    return None


def _try_pair(fn, cfg, idom, dominators, where, i, j, operands, ctx):
    symbols, ssa = ctx.symbols, ctx.ssa_dsts
    instrs = fn.instructions
    a, b = instrs[i], instrs[j]
    ba, bb = where[i][0], where[j][0]
    doms_a = dominators(ba)
    common = next(d for d in dominators(bb) if d in doms_a)
    if common == ENTRY_ID:
        return None
    if isinstance(a.src2, tac_ast.Var) and ba != bb:
        return None
    # The pair's new place, as an index into `instrs`.
    if common == ba:
        at = i
    elif common == bb:
        at = j
    else:
        start = next(k for k, w in enumerate(where) if w[0] == common)
        at = start + len(cfg.blocks[common].instructions) - 1
        if not isinstance(instrs[at], (
                tac_ast.Jump, tac_ast.JumpIfTrue, tac_ast.JumpIfFalse,
                tac_ast.JumpIfCmp, tac_ast.JumpIfMasked)):
            at += 1
        if EXIT_ID in _between(cfg, common, {ba, bb}):
            return None
    # The instructions on the way from the new place to each of the two.
    if ba == bb == common:
        lo, hi = min(i, j, at), max(i, j)
        path = list(range(lo + 1, hi))
    else:
        region = _reaching(cfg, _between(cfg, common, {ba, bb}), {ba, bb})
        # In the block of `a` or `b`, only the instructions before it
        # are on the way to it, unless a loop leads back into the block
        # or the block leads on to the other one.
        ends = {}
        for w, e, other in ((ba, i, bb), (bb, j, ba)):
            if (w != common and not _on_cycle(cfg, w, common)
                    and other not in _between(cfg, w, {common})):
                ends[w] = e
        path = [k for k, w in enumerate(where)
                if (w[0] in region and k < ends.get(w[0], len(where)))
                or (w[0] == common and k >= at)]
    needs_memory = any(n not in ssa for n in operands)
    for k in path:
        ins = instrs[k]
        if k in (i, j):
            continue
        if any(d.name in operands for d in defs_in(ins)):
            return None
        if needs_memory and (
                isinstance(ins, _MEMORY_WRITES)
                or (isinstance(ins, tac_ast.IndexedSymbolStore)
                    and ins.name in operands)):
            return None
    pair = []
    copies: dict[int, tac_ast.Type_instruction | None] = {}
    for k, ins in ((i, a), (j, b)):
        if ins.dst.name in ssa:
            dst = ins.dst
            copies[k] = None
        else:
            name = _new_name(ins.dst.name, symbols)
            from passes.type_checking import LocalAttr, Symbol
            symbols[name] = Symbol(
                type=symbols.get(ins.dst.name).type, attrs=LocalAttr(),
            )
            ssa.add(name)
            dst = tac_ast.Var(name=name)
            copies[k] = tac_ast.Copy(src=dst, dst=ins.dst)
        pair.append(tac_ast.Binary(
            op=ins.op, src1=ins.src1, src2=ins.src2, dst=dst,
        ))
    out: list = []
    for k, ins in enumerate(instrs):
        if k == at:
            out.extend(pair)
        if k in (i, j):
            if copies[k] is not None:
                out.append(copies[k])
            continue
        out.append(ins)
    if at == len(instrs):
        out.extend(pair)
    return tac_ast.Function(
        name=fn.name, is_global=fn.is_global,
        params=list(fn.params), instructions=out,
    )


class HoistDivmodPairs(PostFixedpointPass):
    name = "hoist_divmod_pairs"

    def run(self, fn, ctx):
        return hoist_divmod_pairs(fn, ctx)
