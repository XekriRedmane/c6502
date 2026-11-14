"""Computing a member of a static object in its own bytes.

`c99_to_tac` reads a member of a static struct, or an element of a
static array at a constant index, with `IndexedLoad` and writes it
with `IndexedSymbolStore`, so `P.lo++` on a static `P` is

    IndexedLoad(P, 0, t)
    Binary(+, t, 1, u)
    IndexedSymbolStore(P, 0, u)

and `tac_to_asm` lowers it as written: copy the member's bytes into
`t`, add into `u`, and copy `u` back. A plain static `w++` is
`Binary(+, w, 1, w)`, which lowers to `LDA w; CLC; ADC #1; STA w` per
byte, and the INC peephole (`passes/inc_peephole.py`) makes that
`INC w; BNE; INC w+1`. The member's add never gets that shape, so it
costs a copy in and a copy out, and the asm optimizer puts the bytes
in registers or temporaries around the add. Likewise `P.lo = w << 1`
shifts into a temporary and then copies it into the member.

`plan_in_place_updates` finds the values that can be computed in the
member's own bytes. `tac_to_asm` then names them by the member's bytes
(`Data(P, k)`) and skips the store, so the instruction that computes
the value writes the member. For `P.lo++` it skips the load too, so
the add reads the member in place and the INC and DEC peepholes see
their pattern.

The rewrite moves the member's write from the store back to the
instruction that computes `u`, and for an update it makes every read
of `t` a read of the member. That's the same program when, in one
function's instructions:

- the store is the instruction right after the one that defines `u`
  (a `Binary`, `Unary`, `Copy`, extend, or truncate), it isn't
  volatile, and `u` is defined only there and read only by the store;
- for an update, the definition is `t + c` or `t - c` with `c` a
  constant, `t` is loaded from the bytes the store writes by a load
  that isn't volatile, `t` and `u` are the same size, and `t` is
  defined only by the load and read only after it and up to the add.
  Between the load and the add there's no label, jump, call, store,
  or other write to the object (only instructions that compute into
  locals and read memory), so the member still holds `t`'s value
  wherever `t` is read;
- otherwise, the definition reads nothing of the object, so writing
  the member while computing `u` can't change what it reads;
- `t` and `u` are locals of the function whose address isn't taken,
  not parameters or statics.

`tac_to_asm` also requires the object to have static storage and not
to be renamed already, and `u` to need every byte.
"""
from __future__ import annotations

from collections import Counter
from collections.abc import Callable
from dataclasses import dataclass

import tac_ast
from passes.optimization.var_visit import defs_in, uses_in


# Instructions whose result can be computed in the member's bytes.
_COMPUTES = (
    tac_ast.Binary, tac_ast.Unary, tac_ast.Copy,
    tac_ast.SignExtend, tac_ast.ZeroExtend, tac_ast.Truncate,
)

# Instructions that may sit between an update's load and its add: they
# compute into locals and read memory, but don't write memory or
# change the flow of control.
_PASSIVE = (
    tac_ast.SignExtend, tac_ast.ZeroExtend, tac_ast.Truncate,
    tac_ast.Unary, tac_ast.Binary, tac_ast.Copy, tac_ast.GetAddress,
    tac_ast.Load, tac_ast.IndexedLoad, tac_ast.IndexedConstLoad,
    tac_ast.IndirectIndexedLoad,
)


@dataclass(frozen=True)
class InPlaceUpdate:
    """One member computed in place: the index of its store, the value
    `u` stored, the object and first byte of the member, and for an
    update, the index of the load and the loaded local `t`."""
    store: int
    u: str
    obj: str
    base: int
    load: int | None = None
    t: str | None = None


def plan_in_place_updates(
    instrs: list[tac_ast.Type_instruction],
    params: list[str],
    is_local: Callable[[str], bool],
    size_of: Callable[[tac_ast.Type_val], int],
) -> list[InPlaceUpdate]:
    """The members of `instrs` that can be computed in place.
    `is_local` says whether a name is a local of the function (not a
    static); `size_of` is `tac_to_asm`'s."""
    defs: Counter[str] = Counter()
    uses: Counter[str] = Counter()
    addressed: set[str] = set()
    for ins in instrs:
        for v in defs_in(ins):
            defs[v.name] += 1
        for v in uses_in(ins):
            uses[v.name] += 1
        if (isinstance(ins, tac_ast.GetAddress)
                and isinstance(ins.operand, tac_ast.Var)):
            addressed.add(ins.operand.name)

    def renamable(name: str) -> bool:
        return (name not in params and name not in addressed
                and defs[name] == 1 and is_local(name))

    plan: list[InPlaceUpdate] = []
    taken: set[str] = set()
    for j, ins in enumerate(instrs[:-1]):
        store = instrs[j + 1]
        if not (isinstance(ins, _COMPUTES)
                and isinstance(ins.dst, tac_ast.Var)
                and isinstance(store, tac_ast.IndexedSymbolStore)
                and not store.is_volatile
                and isinstance(store.index, tac_ast.Constant)
                and store.src == ins.dst):
            continue
        u = ins.dst.name
        obj = store.name
        base = (store.index.const.value & 0xFF) + store.offset
        if not renamable(u) or uses[u] != 1 or u in taken:
            continue
        update = _update_of(instrs, j, base, obj, renamable, uses, size_of)
        if update is not None and update[1] not in taken:
            i, t = update
            plan.append(InPlaceUpdate(j + 1, u, obj, base, i, t))
            taken.update((t, u))
            continue
        if any(v.name == obj or v.name in taken for v in uses_in(ins)):
            continue
        plan.append(InPlaceUpdate(j + 1, u, obj, base))
        taken.add(u)
    return plan


def _update_of(
    instrs: list[tac_ast.Type_instruction], j: int, base: int, obj: str,
    renamable: Callable[[str], bool], uses: Counter[str],
    size_of: Callable[[tac_ast.Type_val], int],
) -> tuple[int, str] | None:
    """When instruction `j` adds a constant to, or subtracts one from,
    a local `t` loaded from byte `base` of `obj`, and the member can
    stand for `t`, the index of the load and `t`, else None."""
    ins = instrs[j]
    if not (isinstance(ins, tac_ast.Binary)
            and isinstance(ins.op, (tac_ast.Add, tac_ast.Subtract))
            and isinstance(ins.src1, tac_ast.Var)
            and isinstance(ins.src2, tac_ast.Constant)):
        return None
    t = ins.src1.name
    if (t == ins.dst.name or not renamable(t)
            or size_of(ins.src1) != size_of(ins.dst)):
        return None
    i = _load_of(instrs, j, t, obj)
    if i is None:
        return None
    load = instrs[i]
    if (load.is_volatile
            or not isinstance(load.index, tac_ast.Constant)
            or (load.index.const.value & 0xFF) + load.offset != base):
        return None
    # Every read of t is between the load and the add.
    if sum(
        v.name == t for k in range(i + 1, j + 1)
        for v in uses_in(instrs[k])
    ) != uses[t]:
        return None
    return i, t


def _load_of(
    instrs: list[tac_ast.Type_instruction], j: int, t: str, obj: str,
) -> int | None:
    """The index of the `IndexedLoad` of `obj` into `t` before
    instruction `j`, when only instructions that leave `obj`'s bytes
    alone come between them, or None."""
    for i in range(j - 1, -1, -1):
        ins = instrs[i]
        if (isinstance(ins, tac_ast.IndexedLoad) and ins.dst == tac_ast.Var(t)
                and ins.name == obj):
            return i
        if not isinstance(ins, _PASSIVE):
            return None
        if any(v.name in (t, obj) for v in defs_in(ins)):
            return None
    return None
