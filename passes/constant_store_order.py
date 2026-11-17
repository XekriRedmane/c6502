"""Groups a run of constant stores by value, so each value is loaded once.

# Pattern

Code that sets up several variables stores constants one after
another, and the order of the C statements decides the order of the
values loaded into A. Zork's `main` starts

    LDA   #$00
    STA   PRINTER_CSW
    LDA   #$C1
    STA   PRINTER_CSW+1
    LDA   #$00
    STA   ZCODE_PAGE_VALID
    ...

and loads 0 twice. The pass moves the stores of each value next to the
first load of it,

    LDA   #$00
    STA   PRINTER_CSW
    STA   ZCODE_PAGE_VALID
    ...
    LDA   #$C1
    STA   PRINTER_CSW+1

which drops each load after the first, 2 bytes and 2 cycles each.

# Conditions

A run is a straight sequence of `LDA #k` (an immediate or a label's
half) each followed by one or more `STA m`, with nothing else between
them. The asm IR's `Mov(#k, m)`, which `asm_emit` writes as `LDA #k;
STA m`, counts as a load and a store, and the pass writes the run back
as separate loads and stores. Every `m` is a `Data` byte that no other store in the run names,
and no store is volatile, so the order of the stores can't be seen:
nothing in the run reads memory. A run that stores into the slots of
two different functions is left alone, since the linker gives two
functions that can't be on the call stack together the same bytes.
The value loaded last stays last, so A and N/Z end the run as they
did. The pass changes a run only when it drops a load, so the peephole
fixed point it runs in still only shrinks the code.
"""
from __future__ import annotations

import asm_ast

_CONSTANTS = (asm_ast.Imm, asm_ast.ImmLabelLow, asm_ast.ImmLabelHigh)
_SLOT_PREFIXES = ("__zpabi_", "__local_")


def apply_constant_store_order(prog: asm_ast.Program) -> asm_ast.Program:
    top: list[asm_ast.Type_top_level] = []
    for tl in prog.top_level:
        if isinstance(tl, asm_ast.Function):
            top.append(asm_ast.Function(
                name=tl.name, is_global=tl.is_global,
                params=list(tl.params),
                instructions=_rewrite(tl.instructions),
            ))
        else:
            top.append(tl)
    return asm_ast.Program(top_level=top)


def _is_a(op) -> bool:
    return isinstance(op, asm_ast.Reg) and isinstance(op.reg, asm_ast.A)


def _is_load(instr) -> bool:
    return (isinstance(instr, asm_ast.Mov) and _is_a(instr.dst)
            and isinstance(instr.src, _CONSTANTS) and not instr.is_volatile)


def _is_store(instr) -> bool:
    return (isinstance(instr, asm_ast.Mov) and _is_a(instr.src)
            and isinstance(instr.dst, asm_ast.Data)
            and not instr.is_volatile)


def _slot_owner(name: str) -> str | None:
    """The function a `__zpabi_f__x` or `__local_f__x` slot belongs
    to, else None."""
    for prefix in _SLOT_PREFIXES:
        if name.startswith(prefix):
            return name[len(prefix):].split("__", 1)[0]
    return None


def _rewrite(instrs):
    out: list[asm_ast.Type_instruction] = []
    i = 0
    while i < len(instrs):
        groups, end = _run(instrs, i)
        merged = _merged(groups) if groups else None
        if merged is None:
            out.append(instrs[i])
            i += 1
            continue
        for load, stores in merged:
            out.append(load)
            out.extend(stores)
        i = end
    return out


def _is_constant_store(instr) -> bool:
    """A store of a constant to a byte, `Mov(Imm, Data)`, which
    `asm_emit` writes as `LDA #k; STA m`."""
    return (isinstance(instr, asm_ast.Mov)
            and isinstance(instr.src, _CONSTANTS)
            and isinstance(instr.dst, asm_ast.Data)
            and not instr.is_volatile)


def _run(instrs, i):
    """The `(load, stores)` groups of the run starting at `instrs[i]`
    and the index after it, or `([], i)` when no run starts there. A
    `Mov(Imm, Data)` counts as its load and its store."""
    a = asm_ast.Reg(reg=asm_ast.A())
    groups: list[tuple[asm_ast.Mov, list[asm_ast.Mov]]] = []
    j = i
    while j < len(instrs):
        instr = instrs[j]
        if _is_constant_store(instr):
            load = asm_ast.Mov(src=instr.src, dst=a)
            stores = [asm_ast.Mov(src=a, dst=instr.dst)]
            k = j + 1
        elif _is_load(instr):
            load, stores, k = instr, [], j + 1
        else:
            break
        while k < len(instrs) and _is_store(instrs[k]):
            stores.append(instrs[k])
            k += 1
        if not stores:
            break
        groups.append((load, stores))
        j = k
    return groups, j


def _merged(groups):
    """`groups` with each value's stores after its first load, the
    last value last, or None when that drops no load or the run
    breaks a condition."""
    keys = [repr(load.src) for load, _ in groups]
    if len(set(keys)) == len(keys):
        return None
    targets = [(s.dst.name, s.dst.offset) for _, ss in groups for s in ss]
    if len(set(targets)) != len(targets):
        return None
    owners = {_slot_owner(name) for name, _ in targets} - {None}
    if len(owners) > 1:
        return None
    order: list[str] = []
    by_key: dict[str, tuple[asm_ast.Mov, list[asm_ast.Mov]]] = {}
    for key, (load, stores) in zip(keys, groups):
        if key not in by_key:
            by_key[key] = (load, [])
            order.append(key)
        by_key[key][1].extend(stores)
    last = keys[-1]
    order.remove(last)
    order.append(last)
    return [by_key[k] for k in order]
