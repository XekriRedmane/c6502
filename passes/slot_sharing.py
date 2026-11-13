"""Keeping a value in a callee's parameter bytes across calls.

Background
----------

`zp_slot_allocation` gives each zp_abi function's parameters bytes no
function on a common call path uses, and `zp_local_allocation` does the
same for body locals. So a caller that passes a value it still needs
afterwards keeps it in its own pool and copies it into the callee's
slot before every call:

    void instr_save(void) {
        uint8_t *page = Z_HEADER_ADDR;
        do {
            if (inc_sector_and_write(page)) goto fail;
            page += 0x100;
        } while (--pages != 0);
        ...

copies `page` into `inc_sector_and_write`'s slot each time round.
Zork's own code keeps the pointer in SCRATCH2, which the routines it
calls leave alone, and copies nothing. This pass finds the values that
can live in the callee's bytes the same way, and the allocators then
keep those bytes apart from everything that could change them.

Two kinds of value qualify:

- A local `v` of a function `E` passed as parameter `q` of `G`, and
  still needed after one of those calls. `E` then keeps `v` in `G`'s
  slot for `q`: it's a *holder* of the bytes. A local that a copy
  links to `v` (out of SSA, `page += 0x100` in a loop is `t = page +
  256; ...; page = t`) goes there too when the two are never live at
  once, so the copy goes.
- A parameter `p` of a zp_abi function `F` passed on as parameter `q`
  of `G`. `F`'s slot for `p` gets the same address as `G`'s slot for
  `q`, so `F`'s callers write the value where `G` reads it and `F`
  copies nothing. `F`'s slot is then a *member* of the bytes, as `G`'s
  is.

A set of such slots and holders is a `SlotShare`. The value goes in
one set of bytes for every function in it. `tac_to_asm` writes each
bound variable as the slot's `Data` operand (`bindings` says which), so
the copy before the call is a store of a byte to itself, which goes.

The conditions
--------------

Each function involved has a private pool
(`zp_local_allocation.pool_eligible`), so everything it calls is
bounded, and isn't on a cycle. Each member's owner never assigns the
parameter or takes its address, and isn't a routine with a fixed
address. The parameter isn't passed in a register or at a fixed
address, and is an integer or a pointer. The variable has the
parameter's size, isn't `volatile` or `register`, and isn't an object
whose address is taken or that's indexed by name.

A function *writes* the bytes when it calls a member's owner and
passes, for that member's parameter, anything but its own value in
the bytes, or when it's a holder and assigns its variable. Then, for
each bound variable `v` of a function `F`:

1. A call in `F` to a member's owner passes `v` (or a local sharing its
   home) for that parameter, or `v` is dead after the call and isn't
   one of the call's arguments (the call's argument writes would
   otherwise overwrite `v`, or read it after overwriting it).
2. A call that `v` is live across reaches no function that writes the
   bytes, other than `F`.
3. When any member's owner has external linkage, a call that `v` is
   live across reaches no function outside this unit (other than a
   routine with a fixed address, or an owner from another unit, below),
   since one in another unit might call the owner.

A binding that breaks a condition is dropped, and the check is
repeated, since dropping an alias can make its function a writer.
Each function appears at most once in a share, as a member or as a
holder: two of its values can't have one home.

Across units
------------

A member's owner may be a function this unit only declares, when the
declaration makes the parameter `const`, as in
`bool inc_sector_and_write(uint8_t *const buffer);`. The compiler
takes that as a promise that the function leaves its parameter's bytes
alone, and so does everything it calls; rule 2 can't be checked for
what it calls here. So each share records what the linker needs to
check it for the whole program: for each function in it, the callees
its value is live across (`live`); the callers whose every call to an
owner passes their own value in the bytes (`forwards`); and the holders
that assign their variable (`writers`). The linker merges the shares
of all units (`merge_shares`), and `check_linked_shares` repeats rule 2
with the program's call graph and each definition's record of the
parameters it assigns.

The allocators (`allocate_zp_slots`, `allocate_function_locals`) keep
a share's bytes off every other byte of every function that can be on
the call stack with any function in it, which rule 2 relies on.
"""
from __future__ import annotations

from dataclasses import dataclass, replace

import c99_ast
import tac_ast
from passes.abi_selection import ParamLayout, ZpLayout
from passes.recursion_saves import _live_out, defs_in


@dataclass(frozen=True)
class SlotShare:
    """Bytes several functions use as one value's home. `members` pairs
    each function whose parameter slots are in it with those slots, low
    byte first, all the same width. `holders` are the functions that
    keep a local there. `live`, `forwards`, and `writers` are for the
    linker (see "Across units"): each function with a value there
    paired with the callees it's live across, the (caller, owner) pairs
    whose every call passes the caller's own value, and the holders
    that assign theirs."""
    members: tuple[tuple[str, tuple[str, ...]], ...]
    holders: tuple[str, ...] = ()
    live: tuple[tuple[str, tuple[str, ...]], ...] = ()
    forwards: tuple[tuple[str, str], ...] = ()
    writers: tuple[str, ...] = ()

    @property
    def width(self) -> int:
        return len(self.members[0][1])

    def functions(self) -> set[str]:
        return {f for f, _ in self.members} | set(self.holders)

    def owners(self) -> set[str]:
        return {f for f, _ in self.members}

    def slots(self) -> set[str]:
        return {s for _, syms in self.members for s in syms}


def reach_sets(prog: tac_ast.Program) -> dict[str, set[str]]:
    """Each function defined in `prog` mapped to every function it can
    call, directly or not. A function without a body here is a leaf."""
    direct: dict[str, set[str]] = {}
    for tl in prog.top_level:
        if isinstance(tl, tac_ast.Function):
            direct[tl.name] = {
                i.name for i in tl.instructions
                if isinstance(i, tac_ast.FunctionCall)
            }
    return _closure(direct)


def _closure(direct: dict[str, set[str]]) -> dict[str, set[str]]:
    out: dict[str, set[str]] = {}
    for name in direct:
        seen: set[str] = set()
        stack = list(direct[name])
        while stack:
            c = stack.pop()
            if c in seen:
                continue
            seen.add(c)
            stack.extend(direct.get(c, ()))
        out[name] = seen
    return out


def coexist(a: str, b: str, reach: dict[str, set[str]]) -> bool:
    """True iff `a` and `b` can be on the call stack together: they're
    the same function, or one can call the other."""
    return a == b or b in reach.get(a, ()) or a in reach.get(b, ())


def share_conflicts(
    fn: str, shares, reach: dict[str, set[str]],
) -> list:
    """The shares whose bytes `fn`'s own bytes must avoid: those with a
    function that can be on the call stack with `fn`."""
    return [
        s for s in shares
        if any(coexist(fn, f, reach) for f in s.functions())
    ]


@dataclass
class _Binding:
    fn: str          # the function the variable belongs to
    var: str         # the variable
    owner: str       # the callee whose parameter slots it lives in
    index: int       # the callee's parameter's position
    alias: bool      # `var` is a parameter of `fn` (else a local)
    # For a local copied to or from a bound local (`primary`), and
    # never live at the same time, which shares its home.
    primary: str | None = None


def find_shares(
    prog: tac_ast.Program,
    abi: dict[str, ParamLayout],
    symbols,
    types=None,
    *,
    eligible: set[str],
    fixed: set[str] = frozenset(),
    recursion: set[str] = frozenset(),
) -> tuple[list[SlotShare], dict[str, dict[str, tuple[str, ...]]]]:
    """The shares for `prog` (see the module docstring), and for each
    function the variables it keeps in a share, each mapped to the slot
    symbols, low byte first, its operands become. `eligible` is the
    functions with private pools, `fixed` the routines with fixed
    addresses, and `recursion` the functions on a cycle."""
    fns = {
        tl.name: tl for tl in prog.top_level
        if isinstance(tl, tac_ast.Function)
    }
    reach = reach_sets(prog)
    ok = {n for n in fns if n in eligible and n not in recursion}
    changed = {n: _changed_names(fns[n]) for n in fns}
    slots = {
        n: _param_slots(n, abi, symbols, types, fns, changed)
        for n in set(fns) | {n for n, l in abi.items()
                             if isinstance(l, ZpLayout)}
    }
    live = {n: _live_out(fns[n]) for n in ok}

    def owner_ok(g: str) -> bool:
        if g in fixed:
            return False
        if g in fns:
            return g in ok
        return isinstance(abi.get(g), ZpLayout)

    cands: list[_Binding] = []
    bound: set[tuple[str, str]] = set()
    for fname in sorted(ok):
        fn = fns[fname]
        in_memory = _in_memory(fn)
        for i, instr in enumerate(fn.instructions):
            if not isinstance(instr, tac_ast.FunctionCall):
                continue
            g = instr.name
            if g == fname or not owner_ok(g):
                continue
            for j, arg in enumerate(instr.args):
                if not isinstance(arg, tac_ast.Var):
                    continue
                v = arg.name
                if (fname, v) in bound or j >= len(slots[g]):
                    continue
                target = slots[g][j]
                if target is None:
                    continue
                if v in fn.params:
                    k = fn.params.index(v)
                    own = slots[fname][k] if k < len(slots[fname]) else None
                    if own is None or len(own) != len(target):
                        continue
                    alias = True
                else:
                    if not _plain_local(v, symbols, in_memory):
                        continue
                    if _size(v, symbols, types) != len(target):
                        continue
                    if v not in live[fname][i]:
                        continue
                    alias = False
                cands.append(_Binding(fname, v, g, j, alias))
                bound.add((fname, v))
    for b in list(cands):
        if b.alias:
            continue
        group = _copy_group(
            fns[b.fn], b.var, live[b.fn], symbols, types, bound,
        )
        for t in sorted(group - {b.var}):
            cands.append(_Binding(b.fn, t, b.owner, b.index, False, b.var))
            bound.add((b.fn, t))

    def group_of(b: _Binding) -> set[str]:
        primary = b.primary or b.var
        return {
            c.var for c in cands
            if c.fn == b.fn and (c.primary or c.var) == primary
        }

    params = {n: list(fn.params) for n, fn in fns.items()}
    while True:
        shares, by_binding = _classes(cands, slots, params)
        cands = [b for b in cands if id(b) in by_binding]
        writers = _writers(fns, cands, by_binding, shares, slots)
        bad: set[int] = set()
        for b in cands:
            k = by_binding[id(b)]
            group = group_of(b)
            share = shares[k]
            owners = share.owners()
            extern_owners = {f for f in owners if f not in fns}
            global_owner = any(
                f not in fns or fns[f].is_global for f in owners
            )
            indexes = _member_indexes(share, slots)
            for i, instr in enumerate(fns[b.fn].instructions):
                if not isinstance(instr, tac_ast.FunctionCall):
                    continue
                v_live = b.var in live[b.fn][i]
                passed = tac_ast.Var(name=b.var) in instr.args
                for j in indexes.get(instr.name, ()):
                    arg = instr.args[j] if j < len(instr.args) else None
                    mine = isinstance(arg, tac_ast.Var) and arg.name in group
                    if not mine and (v_live or passed):
                        bad.add(id(b))
                if not v_live:
                    continue
                called = {instr.name} | reach.get(instr.name, set())
                if called & (writers[k] - {b.fn}):
                    bad.add(id(b))
                unknown = {
                    c for c in called if c not in fns and c not in fixed
                } - extern_owners
                if global_owner and unknown:
                    bad.add(id(b))
        if not bad:
            break
        # A binding that goes takes the locals sharing its home with it.
        gone = {(b.fn, b.var) for b in cands if id(b) in bad}
        cands = [
            b for b in cands
            if id(b) not in bad and (b.fn, b.primary) not in gone
        ]

    bindings: dict[str, dict[str, tuple[str, ...]]] = {}
    for b in cands:
        bindings.setdefault(b.fn, {})[b.var] = slots[b.owner][b.index]
    out: list[SlotShare] = []
    for k, share in enumerate(shares):
        mine = [b for b in cands if by_binding[id(b)] == k]
        if not mine:
            continue
        out.append(_with_link_records(
            share, mine, fns, live, slots, bindings, writers[k],
        ))
    return out, bindings


def _with_link_records(
    share: SlotShare, mine, fns, live, slots, bindings, writers,
) -> SlotShare:
    """`share` with the records the linker checks it by (see "Across
    units"); `mine` are its bindings."""
    across: dict[str, set[str]] = {}
    for b in mine:
        for i, instr in enumerate(fns[b.fn].instructions):
            if (isinstance(instr, tac_ast.FunctionCall)
                    and b.var in live[b.fn][i]):
                across.setdefault(b.fn, set()).add(instr.name)
    indexes = _member_indexes(share, slots)
    forwards = set()
    for name, fn in fns.items():
        own = {
            v for v, s in bindings.get(name, {}).items()
            if set(s) <= share.slots()
        }
        for g, js in indexes.items():
            calls = [
                i for i in fn.instructions
                if isinstance(i, tac_ast.FunctionCall) and i.name == g
            ]
            if calls and all(
                isinstance(c.args[j], tac_ast.Var) and c.args[j].name in own
                for c in calls for j in js if j < len(c.args)
            ):
                forwards.add((name, g))
    return replace(
        share,
        live=tuple(sorted((f, tuple(sorted(cs))) for f, cs in across.items())),
        forwards=tuple(sorted(forwards)),
        writers=tuple(sorted(w for w in writers if w in share.holders)),
    )


def _member_indexes(share: SlotShare, slots) -> dict[str, list[int]]:
    """Each member's owner mapped to the positions of its parameters in
    `share`."""
    out: dict[str, list[int]] = {}
    for f, syms in share.members:
        out.setdefault(f, []).append(slots[f].index(syms))
    return out


def _classes(cands, slots, params):
    """Groups the bindings' slots into shares. Returns the shares and
    each kept binding's share index (by `id`); a binding that would put
    a function in its share twice is left out."""
    parent: dict[tuple[str, int], tuple[str, int]] = {}

    def find(k):
        while parent.get(k, k) != k:
            k = parent[k]
        return k

    members: dict[tuple[str, int], set[tuple[str, int]]] = {}
    holders: dict[tuple[str, int], set[str]] = {}

    def fns_of(root) -> list[str]:
        return [f for f, _ in members.get(root, {root})] + sorted(
            holders.get(root, ()),
        )

    kept: list = []
    for b in cands:
        if b.primary is not None:
            continue
        target = find((b.owner, b.index))
        if b.alias:
            src = find((b.fn, params[b.fn].index(b.var)))
            if src == target:
                continue
            union = fns_of(src) + fns_of(target)
            if len(union) != len(set(union)):
                continue
            parent[src] = target
            members[target] = (
                members.get(target, {target}) | members.pop(src, {src})
            )
            holders[target] = holders.get(target, set()) | holders.pop(
                src, set(),
            )
        else:
            if b.fn in fns_of(target):
                continue
            members.setdefault(target, {target})
            holders.setdefault(target, set()).add(b.fn)
        kept.append(b)
    primaries = {(b.fn, b.var) for b in kept}
    kept += [
        b for b in cands
        if b.primary is not None and (b.fn, b.primary) in primaries
    ]
    roots = sorted({find((b.owner, b.index)) for b in kept})
    shares = []
    index: dict[tuple[str, int], int] = {}
    for r in roots:
        index[r] = len(shares)
        shares.append(SlotShare(
            members=tuple(sorted(
                (f, slots[f][k]) for f, k in members.get(r, {r})
            )),
            holders=tuple(sorted(holders.get(r, ()))),
        ))
    by_binding = {id(b): index[find((b.owner, b.index))] for b in kept}
    return shares, by_binding


def _writers(fns, cands, by_binding, shares, slots) -> dict[int, set[str]]:
    """For each share, the functions here that write its bytes (see the
    module docstring)."""
    self_bound: dict[tuple[str, str], int] = {
        (b.fn, b.var): by_binding[id(b)] for b in cands
    }
    owned: dict[str, list[tuple[int, int]]] = {}
    for k, share in enumerate(shares):
        for f, syms in share.members:
            owned.setdefault(f, []).append((slots[f].index(syms), k))
    out: dict[int, set[str]] = {k: set() for k in range(len(shares))}
    for name, fn in fns.items():
        for instr in fn.instructions:
            if isinstance(instr, tac_ast.FunctionCall):
                for j, k in owned.get(instr.name, ()):
                    arg = instr.args[j] if j < len(instr.args) else None
                    if not (
                        isinstance(arg, tac_ast.Var)
                        and self_bound.get((name, arg.name)) == k
                    ):
                        out[k].add(name)
    for b in cands:
        if b.alias:
            continue
        fn = fns[b.fn]
        if any(d.name == b.var for i in fn.instructions for d in defs_in(i)):
            out[by_binding[id(b)]].add(b.fn)
    return out


def _copy_group(
    fn: tac_ast.Function, var: str, live: list[set[str]], symbols, types,
    bound: set[tuple[str, str]],
) -> set[str]:
    """`var` and the locals of `fn` linked to it by copies (`a = b`)
    that can share its home: of its size, plain (see `_plain_local`),
    not bound elsewhere, and never live where another of them is
    defined, other than by a copy between them."""
    in_memory = _in_memory(fn)
    size = _size(var, symbols, types)
    copies = [
        (i.src.name, i.dst.name) for i in fn.instructions
        if isinstance(i, tac_ast.Copy) and isinstance(i.src, tac_ast.Var)
        and isinstance(i.dst, tac_ast.Var)
    ]

    def interferes(t: str, group: set[str]) -> bool:
        names = group | {t}
        for k, instr in enumerate(fn.instructions):
            if (isinstance(instr, tac_ast.Copy)
                    and isinstance(instr.src, tac_ast.Var)
                    and instr.src.name in names
                    and isinstance(instr.dst, tac_ast.Var)
                    and instr.dst.name in names):
                continue
            defined = {d.name for d in defs_in(instr)}
            if t in defined and live[k] & group:
                return True
            if defined & group and t in live[k]:
                return True
        return False

    group = {var}
    changed = True
    while changed:
        changed = False
        for a, c in copies:
            for t, m in ((c, a), (a, c)):
                if (m in group and t not in group
                        and (fn.name, t) not in bound
                        and t not in fn.params
                        and _plain_local(t, symbols, in_memory)
                        and _size(t, symbols, types) == size
                        and not interferes(t, group)):
                    group.add(t)
                    changed = True
    return group


def _param_slots(
    name: str, abi, symbols, types, fns, changed,
) -> list[tuple[str, ...] | None]:
    """`name`'s slot symbols, one tuple per parameter, or None for a
    parameter that can't be in a share: one passed in a register or at
    a fixed address, one that isn't an integer or a pointer, one the
    function assigns or takes the address of, or, for a function
    defined in another unit, one its declaration doesn't make
    `const`."""
    from passes.replace_pseudoregisters import sizeof
    layout = abi.get(name)
    sym = symbols.get(name) if symbols is not None else None
    ftype = getattr(sym, "type", None)
    if not isinstance(ftype, c99_ast.FunType):
        n = len(fns[name].params) if name in fns else 0
        return [None] * n
    if not isinstance(layout, ZpLayout):
        return [None] * len(ftype.params)
    out: list[tuple[str, ...] | None] = []
    flat = 0
    n_slots = len(layout.slot_symbols)
    pinned = layout.pinned or [None] * n_slots
    regs = layout.param_registers or [None] * n_slots
    for j, t in enumerate(ftype.params):
        n = sizeof(t, types)
        syms = tuple(layout.slot_symbols[flat:flat + n])
        ok = (
            len(syms) == n
            and not any(r is not None for r in regs[flat:flat + n])
            and not any(a is not None for a in pinned[flat:flat + n])
            and _scalar_type(t)
        )
        flat += n
        if ok and name in fns:
            p = fns[name].params[j] if j < len(fns[name].params) else None
            ok = p is not None and p not in changed[name]
        elif ok:
            ok = isinstance(t, c99_ast.Const) and not _volatile(t)
        out.append(syms if ok else None)
    return out


def _changed_names(fn: tac_ast.Function) -> set[str]:
    """The names `fn` assigns, takes the address of, or indexes by
    name."""
    out: set[str] = set()
    for instr in fn.instructions:
        out.update(d.name for d in defs_in(instr))
    return out | _in_memory(fn)


def _in_memory(fn: tac_ast.Function) -> set[str]:
    """The names whose address `fn` takes or that it indexes by name,
    which live in memory rather than in a value's home."""
    out: set[str] = set()
    for instr in fn.instructions:
        match instr:
            case tac_ast.GetAddress(operand=tac_ast.Var(name=n)):
                out.add(n)
            case tac_ast.IndexedLoad(name=n) | tac_ast.IndexedSymbolStore(
                name=n,
            ):
                out.add(n)
    return out


def _plain_local(v: str, symbols, in_memory: set[str]) -> bool:
    from passes.type_checking import LocalAttr
    sym = symbols.get(v) if symbols is not None else None
    if sym is None or not isinstance(sym.attrs, LocalAttr):
        return False
    if sym.attrs.register or v in in_memory or _volatile(sym.type):
        return False
    return _scalar_type(sym.type)


def _volatile(t) -> bool:
    while isinstance(t, (c99_ast.Const, c99_ast.Volatile)):
        if isinstance(t, c99_ast.Volatile):
            return True
        t = t.referenced_type
    return False


def _scalar_type(t) -> bool:
    while isinstance(t, (c99_ast.Const, c99_ast.Volatile)):
        t = t.referenced_type
    return isinstance(t, (
        c99_ast.Pointer, c99_ast.Char, c99_ast.SChar, c99_ast.UChar,
        c99_ast.Int, c99_ast.UInt, c99_ast.Long, c99_ast.ULong,
    ))


def _size(v: str, symbols, types) -> int:
    from passes.replace_pseudoregisters import size_of_name
    return size_of_name(v, symbols, types)


def changed_param_slots(
    prog: tac_ast.Program, abi: dict[str, ParamLayout], symbols, types=None,
) -> dict[str, list[str]]:
    """For each zp_abi function defined in `prog`, the slot symbols of
    the parameters it assigns, takes the address of, or indexes by name,
    for the link metadata: a unit that declares one of them `const`
    keeps a value there (see "Across units")."""
    from passes.replace_pseudoregisters import size_of_name
    out: dict[str, list[str]] = {}
    for tl in prog.top_level:
        layout = abi.get(getattr(tl, "name", None))
        if not (isinstance(tl, tac_ast.Function)
                and isinstance(layout, ZpLayout)):
            continue
        changed = _changed_names(tl)
        flat = 0
        syms: list[str] = []
        for p in tl.params:
            n = size_of_name(p, symbols, types)
            if p in changed:
                syms.extend(layout.slot_symbols[flat:flat + n])
            flat += n
        if syms:
            out[tl.name] = syms
    return out


def pinned_param_copies(
    prog: tac_ast.Program, abi: dict[str, ParamLayout], symbols, types,
    addressed: set[str],
) -> dict[str, set[str]]:
    """The `zeropage(addr)` parameters of the zp_abi functions defined
    in `prog` that their function copies into a local of its own as it
    starts (`tac_to_asm._pinned_param_locals`), by function. A function
    keeps such a parameter in its bytes unless something it calls might
    write them while it runs: a function with a `zeropage(addr)`
    parameter on any of the same bytes, since its callers store the
    argument there, or a function this unit doesn't define, other than
    a routine with an `address` (`addressed`), since it might call
    one."""
    reach = reach_sets(prog)
    bytes_of = {
        name: {a for a in layout.pinned if a is not None}
        for name, layout in abi.items() if isinstance(layout, ZpLayout)
    }
    out: dict[str, set[str]] = {}
    for tl in prog.top_level:
        layout = abi.get(getattr(tl, "name", None))
        if not (isinstance(tl, tac_ast.Function)
                and isinstance(layout, ZpLayout) and bytes_of[tl.name]):
            continue
        called = reach.get(tl.name, set()) - {tl.name}
        unknown = any(c not in reach and c not in addressed for c in called)
        flat = 0
        for p in tl.params:
            n = _size(p, symbols, types)
            mine = {a for a in layout.pinned[flat:flat + n] if a is not None}
            flat += n
            if mine and (unknown or any(
                mine & bytes_of.get(c, set()) for c in called
            )):
                out.setdefault(tl.name, set()).add(p)
    return out


# ---------------------------------------------------------------------------
# Linking.
# ---------------------------------------------------------------------------


def merge_shares(
    shares: list[SlotShare], canonical: dict[str, str],
) -> list[SlotShare]:
    """The shares of every unit as one program's: each slot symbol
    renamed by `canonical` (a declaration's spelling of a parameter's
    slot to the definition's), and shares with a slot in common joined.
    Raises `ValueError` when a function would be in a share twice."""
    parent = list(range(len(shares)))

    def find(k: int) -> int:
        while parent[k] != k:
            k = parent[k]
        return k

    renamed = [
        replace(s, members=tuple(
            (f, tuple(canonical.get(x, x) for x in syms))
            for f, syms in s.members
        ))
        for s in shares
    ]
    first: dict[str, int] = {}
    for k, s in enumerate(renamed):
        for sym in s.slots():
            if sym in first:
                parent[find(k)] = find(first[sym])
            else:
                first[sym] = k
    groups: dict[int, list[SlotShare]] = {}
    for k, s in enumerate(renamed):
        groups.setdefault(find(k), []).append(s)
    out = []
    for group in groups.values():
        members = sorted({m for s in group for m in s.members})
        holders = sorted({h for s in group for h in s.holders})
        fns = [f for f, _ in members] + holders
        twice = sorted({f for f in fns if fns.count(f) > 1})
        if twice:
            raise ValueError(
                f"`{twice[0]}` would keep two values in the parameter "
                f"bytes `{members[0][1][0]}` share"
            )
        live: dict[str, set[str]] = {}
        for s in group:
            for f, cs in s.live:
                live.setdefault(f, set()).update(cs)
        out.append(SlotShare(
            members=tuple(members), holders=tuple(holders),
            live=tuple(sorted((f, tuple(sorted(c))) for f, c in live.items())),
            forwards=tuple(sorted({x for s in group for x in s.forwards})),
            writers=tuple(sorted({w for s in group for w in s.writers})),
        ))
    return out


def check_linked_shares(
    shares: list[SlotShare],
    calls: list[tuple[str, str]],
    defined: set[str],
    changed: dict[str, set[str]],
) -> None:
    """Rule 2 of the module docstring for the whole program, and that
    every member's owner leaves its parameter alone. `calls` are the
    program's call edges, `defined` the functions some unit defines,
    and `changed` each one's assigned parameter slots. Raises
    `ValueError` with a message for the user."""
    direct: dict[str, set[str]] = {f: set() for f in defined}
    for a, b in calls:
        direct.setdefault(a, set()).add(b)
    reach = _closure(direct)
    for share in shares:
        what = f"the parameter bytes `{share.members[0][1][0]}`"
        for f, syms in share.members:
            if f not in defined:
                raise ValueError(
                    f"{what} hold a value across calls to `{f}`, which no "
                    f"unit defines, so nothing checks that it leaves its "
                    f"`const` parameter alone"
                )
            if set(syms) & changed.get(f, set()):
                raise ValueError(
                    f"a unit keeps a value in {what} across a call to "
                    f"`{f}` because its declaration makes the parameter "
                    f"`const`, but `{f}` changes it"
                )
        owners = share.owners()
        forwards = set(share.forwards)
        writers = {
            a for a, b in calls if b in owners and (a, b) not in forwards
        } | set(share.writers)
        for f, callees in share.live:
            for c in callees:
                hit = ({c} | reach.get(c, set())) & (writers - {f})
                if hit:
                    raise ValueError(
                        f"`{f}` keeps a value in {what} across its call to "
                        f"`{c}`, because a declaration makes the parameter "
                        f"`const`, but `{sorted(hit)[0]}`, which `{c}` "
                        f"reaches, writes those bytes; declare the "
                        f"parameter without `const`"
                    )


# ---------------------------------------------------------------------------
# After code generation.
# ---------------------------------------------------------------------------


def preferred_symbols(
    bindings: dict[str, dict[str, tuple[str, ...]]],
) -> dict[str, set[str]]:
    """For each function, the slot symbols its variables live in, which
    `asm_emit` names a `(zp),Y` pointer by in that function's body, so
    the linker, which binds each symbol again, keeps the pointer where
    the function keeps the value."""
    return {
        fn: {s for slots in vs.values() for s in slots}
        for fn, vs in bindings.items()
    }


def check_shares(
    prog, shares: list[SlotShare],
    bindings: dict[str, dict[str, tuple[str, ...]]],
) -> None:
    """Raises if the finished asm `prog` breaks what the shares rest on:
    a function writes one of its own parameter slots that's in a share.
    `find_shares` admits only owners that never assign the parameter,
    so a write would be one a pass after it made up."""
    import asm_ast
    own: dict[str, set[str]] = {}
    for share in shares:
        for f, syms in share.members:
            own.setdefault(f, set()).update(syms)
    for tl in prog.top_level:
        if not isinstance(tl, asm_ast.Function) or tl.name not in own:
            continue
        for instr in tl.instructions:
            for op in _memory_writes(instr):
                if isinstance(op, asm_ast.Data) and op.name in own[tl.name]:
                    raise AssertionError(
                        f"internal error: `{tl.name}` writes "
                        f"`{op.name}`, a parameter slot that holds a "
                        f"value its callers keep there "
                        f"(passes/slot_sharing.py)"
                    )


def _memory_writes(instr):
    import asm_ast
    match instr:
        case asm_ast.Mov(dst=d) | asm_ast.Inc(dst=d) | asm_ast.Dec(dst=d) \
                | asm_ast.ArithmeticShiftLeft(dst=d) \
                | asm_ast.LogicalShiftRight(dst=d) \
                | asm_ast.RotateLeft(dst=d) | asm_ast.RotateRight(dst=d) \
                | asm_ast.Pop(dst=d) | asm_ast.LoadAddress(dst=d):
            yield d
