"""Steps an array element in place when a slot holds a copy of it.

# Pattern

`uint8_t timer = --BRICK_FILL_TIMERS[i];` in Lode Runner's
`handle_timers` reads the element into one of the function's own
slots, decrements the slot, and stores it back, and the code after it
reads the slot:

    LDA   BRICK_FILL_TIMERS,X
    STA   t
    BNE   .skip
    JMP   .next
.skip:
    DEC   t                     ; the site
    LDA   t
    STA   BRICK_FILL_TIMERS,X
    BEQ   .expired
    ...
    LDA   t                     ; a read of the new value
    CMP   #$14

When `t` holds the same value as the element at the `DEC`, the site
becomes `DEC BRICK_FILL_TIMERS,X; LDA BRICK_FILL_TIMERS,X`, and each
read of `t` that the site's new value reaches reads the element
instead:

.skip:
    DEC   BRICK_FILL_TIMERS,X
    LDA   BRICK_FILL_TIMERS,X
    BEQ   .expired
    ...
    LDA   BRICK_FILL_TIMERS,X
    CMP   #$14

`redundant_load_after_rmw` then drops the `LDA` after the `DEC` when A
is dead, and `asm_dead_store` drops the `STA t` that nothing reads any
more, which leaves the original's `DEC BRICK_FILL_TIMERS,X; BEQ`. The
site is `INC`, `DEC`, `ASL`, `LSR`, `ROL`, or `ROR` of `t`, then `LDA
t` and a non-volatile `STA e` of an element `e` indexed by X, the
three adjacent.

# When it applies

`t` is a `Data` slot of the function's own pool (`__local_<fn>__*`)
whose address the function never forms, so it's dead when the
function returns or jumps to another function, no callee reads it,
and only the function's own instructions can write it. Its address
must be in the slot map, so that a `(zp),Y` access through a pointer
in its bytes counts as a read of it.

A forward dataflow over the function's instructions follows which of
A, `t`, and `e` hold the same value (`_transfer`). A non-volatile
`LDA e` or `LDA t` makes A equal to that byte, and a store of A makes
the byte stored equal to whatever A equals. A write of X changes the
byte `e` names, so it ends what's known of `e`. So does a write that
may reach `e` (`_may_reach`), a call, and any instruction the pass
doesn't follow. At a label, what's known is what holds on every way
in. A site qualifies when `t` equals `e` before it.

From each qualifying site the pass walks forward along every path
until `t` is written without being read, or the function ends. Every
read of `t` it meets must be another qualifying site, or a load,
compare, or ALU operation with `t` as its source, before which `t`
equals `e`, so that it can read `e` instead (`_rewritten`). There's
no `LDX abs,X` or `CPX abs,X`, so a read into or against X doesn't
qualify. Otherwise the pass leaves `t` alone.

# Why it's sound

Outside the sites, the rewritten program runs the same instructions
on the same values, except that the reads it changes read `e` where
the original read `t`, which held the same value there. Inside a
site, `DEC e` writes the value the original's `STA e` wrote, and
`LDA e` loads the value the original's `LDA t` loaded. X doesn't
change within the three instructions, so `e` names the same byte at
each. The flags come out the same: the step sets N and Z (and for a
shift, C) from the same value in either place, the `LDA` sets N and
Z again, and the store sets none. Only `t` differs afterwards: it
keeps its old value where the original stepped it. Every read of `t`
that value can reach has become a read of `e`, and `t` is dead where
the walk stopped.

`e` is never volatile, since only non-volatile accesses make it equal
to anything.

# Cost

A site in zero page is 7 bytes and becomes 6, or 3 when A is dead
after it and the `LDA` goes. Each read it rewrites becomes a byte
longer, an absolute address in place of a zero-page one. The pass
rewrites a slot only when the sum is a gain, before counting the
store into `t` that dies with it. In `handle_timers` it saves 5
bytes: 4 at the site, minus 1 for the compare's load, plus 2 for the
store.

# Where to run

In the asm peephole fixed point, after `dec_peephole` and
`inc_peephole` have made the step an `INC` / `DEC` and before
`redundant_load_after_rmw` and `asm_dead_store`. It needs the slot
map, so it doesn't run in the unoptimized pipeline, whose locals are
on the soft stack.
"""
from __future__ import annotations

from typing import Optional

import asm_ast
from passes.asm_aliasing import (
    _is_zp_symbol, is_user_zp_global, may_alias, runtime_zp_addrs,
)
from passes.asm_dead_store import _reads, _successors
from passes.asm_liveness import a_dead_at, kills_a
from passes.inc_peephole import is_x_indexed

_A = asm_ast.Reg(reg=asm_ast.A())
_RMW = (
    asm_ast.Inc, asm_ast.Dec, asm_ast.ArithmeticShiftLeft,
    asm_ast.LogicalShiftRight, asm_ast.RotateLeft, asm_ast.RotateRight,
)
_ALU = (asm_ast.Add, asm_ast.Sub, asm_ast.And, asm_ast.Or)
# Instructions that change no register but A and no byte, and that
# the walk passes over.
_NO_EFFECT = (
    asm_ast.Label, asm_ast.Branch, asm_ast.Compare, asm_ast.BitTest,
    asm_ast.SetCarry, asm_ast.ClearCarry, asm_ast.Push,
)

# The cells whose equality the dataflow follows.
_CA, _CT, _CE = "A", "T", "E"
_ALL = frozenset({frozenset({_CA, _CT}), frozenset({_CA, _CE}),
                  frozenset({_CT, _CE})})
_TE = frozenset({_CT, _CE})
# Sources an `LDX` / `LDY` reads without going through A. A move into
# X or Y from anything else is taken to use A.
_DIRECT = (
    asm_ast.Imm, asm_ast.ImmLabelLow, asm_ast.ImmLabelHigh, asm_ast.Data,
    asm_ast.ZP, asm_ast.IndexedData, asm_ast.Reg,
)


def apply_element_in_place(
    prog: asm_ast.Program, *, zp_slot_symbols: dict[str, int] | None = None,
) -> asm_ast.Program:
    """Rewrites, in every function, the sites that step a slot holding a
    copy of an array element, and the reads of the slot they reach, to
    step and read the element (see the module docstring).
    `zp_slot_symbols` gives the slots' addresses; without it, the
    program is returned unchanged."""
    if not zp_slot_symbols:
        return prog
    return asm_ast.Program(top_level=[
        _rewrite_function(tl, zp_slot_symbols)
        if isinstance(tl, asm_ast.Function) else tl
        for tl in prog.top_level
    ])


def _rewrite_function(fn: asm_ast.Function, slots) -> asm_ast.Function:
    instrs = fn.instructions
    sites = _sites(instrs, fn, slots)
    if not sites:
        return fn
    by_pair: dict[tuple, list[int]] = {}
    for i in sites:
        t, e = instrs[i].dst, instrs[i + 2].dst
        by_pair.setdefault((t.name, t.offset, repr(e)), []).append(i)
    for indices in by_pair.values():
        new = _rewrite_pair(instrs, indices, slots)
        if new is not None:
            return asm_ast.Function(
                name=fn.name, is_global=fn.is_global,
                params=list(fn.params), instructions=new,
            )
    return fn


def _sites(instrs, fn, slots) -> list[int]:
    """The indexes of `<step> t; LDA t; STA e` with `t` an eligible slot
    and `e` a non-volatile element indexed by X."""
    prefix = f"__local_{fn.name}__"
    taken = _named_labels(instrs)
    out = []
    for i in range(len(instrs) - 2):
        step, load, store = instrs[i:i + 3]
        t = getattr(step, "dst", None)
        if not (isinstance(step, _RMW) and isinstance(t, asm_ast.Data)
                and t.name.startswith(prefix) and t.name not in taken
                and t.name in slots):
            continue
        if not (isinstance(load, asm_ast.Mov) and load.src == t
                and load.dst == _A and not load.is_volatile):
            continue
        if (isinstance(store, asm_ast.Mov) and store.src == _A
                and is_x_indexed(store.dst) and not store.is_volatile):
            out.append(i)
    return out


def _rewrite_pair(instrs, indices, slots):
    """The instructions with the sites at `indices` (one slot `t`, one
    element `e`) and the reads they reach rewritten, or None."""
    t = instrs[indices[0]].dst
    e = instrs[indices[0] + 2].dst
    facts = _facts(instrs, t, e, slots)
    good = [i for i in indices if facts[i] is not None and _TE in facts[i]]
    if not good:
        return None
    starts = set(good)
    labels = {ins.name: k for k, ins in enumerate(instrs)
              if isinstance(ins, asm_ast.Label)}
    reads: set[int] = set()
    seen: set[int] = set()
    stack = [i + 3 for i in good]
    while stack:
        j = stack.pop()
        if j >= len(instrs) or j in seen:
            continue
        seen.add(j)
        if j in starts:
            stack.append(j + 3)
            continue
        ins = instrs[j]
        if isinstance(ins, (asm_ast.Phi, asm_ast.FunctionPrologue,
                            asm_ast.AllocateStack, asm_ast.DecimalArith)):
            return None
        if _reads(ins, t, slots, private=True):
            if (facts[j] is None or _TE not in facts[j]
                    or _rewritten(ins, t, e) is None):
                return None
            reads.add(j)
        elif _writes_whole(ins, t):
            continue
        if isinstance(ins, asm_ast.JumpTable):
            stack.extend(labels[n] for n in ins.targets if n in labels)
            continue
        stack.extend(_successors(instrs, j, labels))
    gain = sum(1 + (3 if a_dead_at(instrs, i + 3) else 0) for i in good)
    if gain - len(reads) <= 0:
        return None
    out = []
    for k, ins in enumerate(instrs):
        if k in starts:
            out.append(type(ins)(dst=e))
            out.append(asm_ast.Mov(src=e, dst=_A, is_volatile=False))
        elif any(s < k <= s + 2 for s in starts):
            continue
        elif k in reads:
            out.append(_rewritten(ins, t, e))
        else:
            out.append(ins)
    return out


def _named_labels(instrs) -> set[str]:
    """The names whose address an operand of `instrs` forms."""
    out = set()
    for ins in instrs:
        for field in vars(ins).values():
            for op in field if isinstance(field, list) else (field,):
                if isinstance(op, (asm_ast.ImmLabelLow, asm_ast.ImmLabelHigh)):
                    out.add(op.name)
    return out


def _writes_whole(ins, t) -> bool:
    """True iff `ins` writes `t` without reading it."""
    return (isinstance(ins, (asm_ast.Mov, asm_ast.Pop))
            and ins.dst == t)


def _rewritten(ins, t, e):
    """`ins` reading `e` where it reads `t`, when `t` is its only use of
    `t` and the 6502 has the abs,X form, else None."""
    if isinstance(ins, asm_ast.Mov) and ins.src == t and ins.dst != t:
        if isinstance(ins.dst, asm_ast.Reg) and isinstance(
            ins.dst.reg, asm_ast.X,
        ):
            return None
        return asm_ast.Mov(src=e, dst=ins.dst, is_volatile=ins.is_volatile)
    if (isinstance(ins, asm_ast.Compare) and ins.left == _A
            and ins.right == t):
        return asm_ast.Compare(left=_A, right=e)
    if isinstance(ins, _ALU) and ins.dst == _A and ins.src == t:
        return type(ins)(src=e, dst=_A)
    if isinstance(ins, asm_ast.Xor) and ins.dst == _A and (
        (ins.src1 == t) != (ins.src2 == t)
    ):
        return asm_ast.Xor(
            src1=e if ins.src1 == t else ins.src1,
            src2=e if ins.src2 == t else ins.src2, dst=_A,
        )
    return None


# ---------------------------------------------------------------------------
# Which of A, t, and e hold the same value
# ---------------------------------------------------------------------------


def _facts(instrs, t, e, slots) -> list[Optional[frozenset]]:
    """For each instruction, the pairs of A, `t`, and `e` known to hold
    the same value before it, or None where the walk doesn't reach."""
    n = len(instrs)
    labels = {ins.name: k for k, ins in enumerate(instrs)
              if isinstance(ins, asm_ast.Label)}
    named = _named_labels(instrs)
    before: list[Optional[frozenset]] = [None] * n
    if n == 0:
        return before
    before[0] = frozenset()
    work = [0]
    while work:
        j = work.pop()
        ins = instrs[j]
        after = _transfer(ins, before[j], t, e, slots)
        if isinstance(ins, asm_ast.JumpTable):
            succ = [labels[x] for x in ins.targets if x in labels]
        else:
            succ = _successors(instrs, j, labels)
        for s in succ:
            # A label whose address is formed may be reached from
            # anywhere.
            state = after
            if isinstance(instrs[s], asm_ast.Label) and instrs[s].name in named:
                state = frozenset()
            old = before[s]
            new = state if old is None else old & state
            if new != old:
                before[s] = new
                work.append(s)
    return before


def _transfer(ins, s, t, e, slots) -> frozenset:
    if isinstance(ins, _NO_EFFECT):
        return s
    if isinstance(ins, asm_ast.Jump):
        return s if ins.target.startswith(".") else frozenset()
    if isinstance(ins, asm_ast.Mov):
        return _transfer_mov(ins, s, t, e, slots)
    if isinstance(ins, (*_RMW, asm_ast.Pop)):
        d = ins.dst
        if isinstance(d, asm_ast.Reg):
            if isinstance(d.reg, asm_ast.A):
                return _kill(s, _CA)
            if isinstance(d.reg, asm_ast.X):
                return _kill(s, _CE)
            return s
        return _write(s, d, None, t, e, slots, volatile=True)
    if isinstance(ins, (*_ALU, asm_ast.Xor)):
        if ins.dst == _A:
            return _kill(s, _CA)
        return frozenset()
    return frozenset()


def _transfer_mov(ins, s, t, e, slots) -> frozenset:
    src, dst = ins.src, ins.dst
    vol = ins.is_volatile
    if isinstance(dst, asm_ast.Reg):
        if isinstance(dst.reg, asm_ast.A):
            return _assign(s, _CA, None if vol else _key(src, t, e))
        if kills_a(ins) or not isinstance(src, _DIRECT):
            s = _kill(s, _CA)
        if isinstance(dst.reg, asm_ast.X):
            s = _kill(s, _CE)
        return s
    if src == _A:
        value = _CA
    elif isinstance(src, asm_ast.Reg):
        value = None
        if kills_a(ins):
            s = _kill(s, _CA)
    else:
        # `LDA src; STA dst`.
        s = _assign(s, _CA, None if vol else _key(src, t, e))
        value = _CA
    return _write(s, dst, value, t, e, slots, volatile=vol)


def _key(op, t, e) -> Optional[str]:
    if op == t:
        return _CT
    if op == e:
        return _CE
    return None


def _write(s, dst, value, t, e, slots, *, volatile) -> frozenset:
    """`s` after a write of `dst`, which ends up equal to `value` (one
    of the cells, or None)."""
    if dst == t:
        return _assign(s, _CT, value)
    if dst == e and not volatile:
        return _assign(s, _CE, value)
    if may_alias(dst, t, zp_slot_symbols=slots):
        s = _kill(s, _CT)
    if _may_reach(dst, e, slots):
        s = _kill(s, _CE)
    return s


def _kill(s, cell) -> frozenset:
    return frozenset(p for p in s if cell not in p)


def _assign(s, cell, value) -> frozenset:
    """`s` after `cell` takes the value of `value` (a cell, or None)."""
    s = _kill(s, cell)
    if value is None or value == cell:
        return s
    s = s | {frozenset({cell, value})}
    return _ALL if len(s) >= 2 else s


def _in_zero_page(name: str) -> bool:
    return (is_user_zp_global(name) or _is_zp_symbol(name)
            or name in runtime_zp_addrs())


def _may_reach(op, e, slots) -> bool:
    """True iff a write of `op` may change the element `e`. A byte of a
    zero-page object can't be an element of an object outside zero
    page indexed from its start or past it, which lies at `$100` or
    above, so the store of `GAME_COLNUM` between `handle_timers`'s
    site and its compare leaves the timer alone. `may_alias` treats
    two names as the same memory unless one is a compiler slot."""
    if not may_alias(op, e, zp_slot_symbols=slots):
        return False
    return not (
        isinstance(op, asm_ast.Data) and op.name != e.name and e.name
        and e.offset >= 0 and _in_zero_page(op.name)
        and not _in_zero_page(e.name)
    )
