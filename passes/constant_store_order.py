"""Groups a run of stores by the value stored, so each value is loaded once.

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

# Values loaded from memory

The same applies to a value loaded from memory that the run doesn't
change. Lode Runner's `fill_hole` copies a brick's row and column into
locals and then into `GAME_ROWNUM` and `GAME_COLNUM`, and
rematerialization (`asm_remat`, `memory_value_propagation`) can turn each
copy of a local into a second load of the table,

    LDA   BRICK_DIG_ROWS,X
    STA   __local_handle_timers__2
    LDA   BRICK_DIG_COLS,X
    STA   __local_handle_timers__saved_guard_num
    LDA   BRICK_DIG_ROWS,X
    STA   GAME_ROWNUM
    LDA   BRICK_DIG_COLS,X
    STA   GAME_COLNUM

which the pass writes with one load of each table byte,

    LDA   BRICK_DIG_ROWS,X
    STA   __local_handle_timers__2
    STA   GAME_ROWNUM
    LDA   BRICK_DIG_COLS,X
    STA   __local_handle_timers__saved_guard_num
    STA   GAME_COLNUM

# Copies within the run

A load of a `Data` byte that an earlier group of the run stored gives
A the value that group loaded, since nothing else in the run writes
the byte, so it joins that group. The rematerializations above leave a
copy of a local in place when the local is read again later, because a
load of `BRICK_DIG_ROWS,X` takes a byte more than a load of the local
(`asm_remat._costs_more`), and

    LDA   BRICK_DIG_ROWS,X
    STA   __local_handle_timers__2
    LDA   BRICK_DIG_COLS,X
    STA   __local_handle_timers__saved_guard_num
    STA   GAME_COLNUM
    LDA   __local_handle_timers__2
    STA   GAME_ROWNUM

becomes the same code as above. Such a load reads nothing once the
run is rewritten, so the condition below on the bytes the loads read
leaves it out.

# Conditions

A run is a straight sequence of loads into A, each followed by one or
more `STA m`, with nothing else between them. A load is `LDA #k` (an
immediate or a label's half), `LDA n` of a `Data` byte, or `LDA
n+k,X` / `LDA n+k,Y` (`IndexedData`), and none is volatile. The asm
IR's `Mov(#k, m)`, which `asm_emit` writes as `LDA #k; STA m`, counts
as a load and a store, and the pass writes the run back as separate
loads and stores. Two loads load the same value when their operands
are equal, which for an indexed load includes the index register.
Nothing in the run writes X or Y, so `n,X` names the same byte at
each load in it.

Every `m` is a `Data` byte that no other store in the run names, and
no store is volatile. The pass moves stores earlier, past loads, and
moves the load of the last value later, past stores, so no store in
the run may write a byte that a load in it reads. A store to `m`
can't write the byte of a load of `Data` byte `n` unless `m` and `n`
are the same name and offset. A load of `n+k,X` is taken to stay
inside the object `n`, which C requires of a subscript (C99
§6.5.6p8), so no store in the run may name `n`. The
rematerializations that produced the second loads are sound only on
the same premise. The premise isn't applied to an indexed load of a
compiler slot (`__zpabi_*`, `__local_*`) or a runtime symbol (`HARGS`
and the rest), whose bytes other names may cover, so a run with such
a load is left alone. With those conditions met, the order of the
stores can't be seen, and each load reads the same value wherever it
goes.

A run that stores into or loads from the slots of two different
functions is left alone, since the linker gives two functions that
can't be on the call stack together the same bytes. The value loaded last stays
last, so A and N/Z end the run as they did, and the loads and stores
leave X, Y, C, and V alone. Every byte the run stores holds the
value it held before at the end of the run. The pass changes a run
only when it drops a load, so the peephole
fixed point it runs in still only shrinks the code.
"""
from __future__ import annotations

import asm_ast

_CONSTANTS = (asm_ast.Imm, asm_ast.ImmLabelLow, asm_ast.ImmLabelHigh)
_MEMORY = (asm_ast.Data, asm_ast.IndexedData)
_SLOT_PREFIXES = ("__zpabi_", "__local_")
_RUNTIME_NAMES = frozenset({"SSP", "FP", "HARGS", "DPTR"})


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
    """A non-volatile load into A of a constant, a `Data` byte, or an
    `IndexedData` byte."""
    return (isinstance(instr, asm_ast.Mov) and _is_a(instr.dst)
            and isinstance(instr.src, (*_CONSTANTS, *_MEMORY))
            and not instr.is_volatile)


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


def _value_keys(groups):
    """The value each group's load gives A, as a key, and whether the
    load reads a byte an earlier group of the run stored, in which
    case it gives the value that group loaded."""
    keys: list[str] = []
    copies: list[bool] = []
    stored: dict[tuple[str, int], str] = {}
    for load, stores in groups:
        src = load.src
        byte = ((src.name, src.offset)
                if isinstance(src, asm_ast.Data) else None)
        if byte in stored:
            keys.append(stored[byte])
            copies.append(True)
        else:
            keys.append(repr(src))
            copies.append(False)
        for s in stores:
            stored[(s.dst.name, s.dst.offset)] = keys[-1]
    return keys, copies


def _merged(groups):
    """`groups` with each value's stores after its first load, the
    last value last, or None when that drops no load or the run
    breaks a condition."""
    keys, copies = _value_keys(groups)
    if len(set(keys)) == len(keys):
        return None
    targets = [(s.dst.name, s.dst.offset) for _, ss in groups for s in ss]
    if len(set(targets)) != len(targets):
        return None
    # A load of a byte the run stored earlier goes with the load whose
    # value was stored there, so it reads nothing once the run is
    # rewritten, and only the other loads' bytes must stay unwritten.
    sources = [load.src for (load, _), copy in zip(groups, copies)
               if not copy]
    names = [name for name, _ in targets] + [
        load.src.name for load, _ in groups
        if isinstance(load.src, asm_ast.Data)]
    owners = {_slot_owner(name) for name in names} - {None}
    if len(owners) > 1:
        return None
    if any(_store_may_write(src, name, offset)
           for src in sources for name, offset in targets):
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


def _store_may_write(src, name: str, offset: int) -> bool:
    """Whether a store to `Data(name, offset)` may write the byte the
    load operand `src` reads. An `IndexedData` load stays inside its
    object (C99 §6.5.6p8), except one of a compiler slot or a runtime
    symbol, which counts as reaching every byte."""
    if isinstance(src, asm_ast.Data):
        return src.name == name and src.offset == offset
    if isinstance(src, asm_ast.IndexedData):
        return (src.name == name or src.name in _RUNTIME_NAMES
                or src.name.startswith(_SLOT_PREFIXES))
    return False
