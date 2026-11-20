"""Give a zero-page local that holds a copy of a parameter the
parameter's own slot.

# What it does

A function that changes a parameter works on a copy of it. The asm
SSA gives each assignment of the parameter a new version, and the
register allocator puts a version that doesn't go in a register in a
byte of the function's pool, so the function starts by copying the
parameter there:

    LDA   __zpabi_mulu16__a_0          ; the parameter's slot
    STA   __local_mulu16__a_0          ; a byte of mulu16's pool
    ...
    ASL   __local_mulu16__a_0
    ROL   __local_mulu16__a_1

When the parameter's slot and the local are never live at once with
different values, the local can be the slot itself. This pass renames
the local to the slot, and the copy becomes `LDA P; STA P`. The pass
drops such a store, a store from a register that already holds the
byte's value, here or on every path into a label, and the peepholes
then drop the load if nothing else reads it. Zork's `mulu16` (examples/zork/numbers.c)
copied both its parameters, 16 bytes, and `divu16` copied its
dividend into `quotient`, a local the C starts as a copy of it. A
version the allocator put in a register stays there.

# When it applies

  * The function is zp_abi, and the slot is one of its own parameter
    bytes. It isn't a register parameter's byte or a `zeropage(addr)`
    one, and it isn't in a share (`slot_sharing`), where a caller
    keeps a value of its own across the call.
  * The local is one of the function's `__local_F__*` bytes. Neither
    byte's address is taken in the unit
    (`asm_aliasing.unreachable_by_pointer`), so only the function's
    own instructions read or write them: a callee's zero page is
    disjoint from its caller's, and a store through a pointer can't
    reach them. Neither is reached by address, by a `ZP` operand or a
    `(zp),Y` pointer base, which is how a local used as a pointer is
    spelled. Each is named only at offset 0, never by an
    `IndexedData`, a `LoadAddress`, or a volatile access (a `volatile`
    local keeps bytes of its own), and only by instructions whose
    reads and writes the pass models.
  * The two are copy-related: one is written with the other's value,
    by `LDA P; STA L` (through any register) or a memory-to-memory
    `Mov`.
  * They don't interfere: no instruction writes one while the other
    is live after it, except a write of one with the other's value.
    Liveness is computed over the function's instructions, with
    nothing live at a return or a tail call: the caller doesn't read
    its arguments back. Which register holds which byte's value is
    followed forward through the function, keeping at a label what is
    the same on every path into it, and forgetting everything at any
    instruction other than a load, a transfer, a store, a compare, a
    branch, or a carry flag change.

Several locals can join one slot, one at a time, each checked against
everything already in it. A function's locals go into the slots of
one callee at most, the one whose slots take the most, and only along
a copy into the slot, which is the copy that goes.

The link metadata records each slot this pass writes as one its
function assigns, so a unit that declares the function with a `const`
parameter, and so would keep a value of its own in the slot, is
rejected by the linker (`slot_sharing.check_linked_shares`).

# A callee's parameter slot

The same goes for the bytes a function writes its callee's argument
into. A local whose value is copied into a callee's parameter slot
for the call can be that slot from where it's computed, as
`instr_mul` (examples/zork/numbers.c) passes the product
to `store_with_sign`:

    JSR   mulu16                        JSR   mulu16
    STA   __local_instr_mul__product    STA   __zpabi_store_with_sign__n_0
    ...                           ->    ...
    LDA   __local_instr_mul__product    JMP   store_with_sign
    STA   __zpabi_store_with_sign__n_0
    ...
    JMP   store_with_sign

A call reads its callee's slots, and a tail call's `JMP` does too. A
call may also write any callee's slots, since functions that are
never active together can share bytes, so the local can't be live
across any call. For the same reason, a store to a slot of another
function may write the same byte: the linker allocates the slots
again for the whole program, so two that are apart in this unit can
share a byte there. Only the same function's slots are certainly
apart. So the local can't be live across a store into another
function's slot either. `div`'s argument in
`tests/test_slot_coalescing.py` had overwritten a local kept in
`mul`'s slot, which in that unit was the same byte.

The slot is a parameter byte of a function this one calls, other than
a register or `zeropage(addr)` byte, outside every share, and out of
reach of any pointer. The callee may change its own
slot (a parameter the callee updates in place, above), since the
local is dead once the callee has it. The link metadata gets no
record of these: the function writes the callee's slot as it writes
any argument.

# Where it runs

Once, in `compile.optimized_asm`, after `synthesize_prologue`, when
the operands are the slots' `Data` names, and before the first
peephole fixed point, which drops the copies that become self-copies.
"""

from __future__ import annotations

import dataclasses

import asm_ast
from passes import decimal_arith
from passes.asm_aliasing import _instruction_operands, unreachable_by_pointer
from passes.dataclass_fields import fields_of

_RMW = (
    asm_ast.Inc, asm_ast.Dec, asm_ast.ArithmeticShiftLeft,
    asm_ast.LogicalShiftRight, asm_ast.RotateLeft, asm_ast.RotateRight,
)
_ALU = (asm_ast.Add, asm_ast.Sub, asm_ast.And, asm_ast.Or, asm_ast.Xor)
_READ_ONLY = (asm_ast.Compare, asm_ast.BitTest, asm_ast.Push)
_NO_OPERANDS = (
    asm_ast.Branch, asm_ast.Jump, asm_ast.Label, asm_ast.Return,
    asm_ast.Ret, asm_ast.Call, asm_ast.SetCarry, asm_ast.ClearCarry,
    asm_ast.FunctionPrologue, asm_ast.AllocateStack,
)
# Instructions that change no register and no memory, which a
# register's known contents survive.
_KEEPS_REGISTERS = (
    asm_ast.Compare, asm_ast.BitTest, asm_ast.Push, asm_ast.Branch,
    asm_ast.SetCarry, asm_ast.ClearCarry,
)


def apply_slot_coalescing(
    prog: asm_ast.Program, abi, shares, slot_symbols: dict[str, int],
) -> tuple[asm_ast.Program, dict[str, list[str]]]:
    """`prog` with locals renamed to the parameter slots they copy, and
    the slots each function now writes, by function. `slot_symbols`
    gives each slot symbol's address."""
    from passes.abi_selection import ZpLayout
    unreachable = unreachable_by_pointer(prog)
    shared = {s for share in shares for _, syms in share.members
              for s in syms}
    all_owners = {
        sym: f for f, layout in abi.items() if isinstance(layout, ZpLayout)
        for sym in layout.slot_symbols
    }
    written: dict[str, list[str]] = {}
    top: list[asm_ast.Type_top_level] = []
    for tl in prog.top_level:
        layout = abi.get(getattr(tl, "name", None))
        if isinstance(tl, asm_ast.Function) and isinstance(layout, ZpLayout):
            n = len(layout.slot_symbols)
            pinned = layout.pinned or [None] * n
            regs = layout.param_registers or [None] * n
            slots = [
                s for k, s in enumerate(layout.slot_symbols)
                if pinned[k] is None and regs[k] is None
                and s not in shared and s in unreachable
            ]
            callee = _callee_slots(tl, abi, shared, unreachable)
            tl, merged = _coalesce(tl, slots, callee, unreachable, slot_symbols,
                                   all_owners)
            if merged:
                written[tl.name] = merged
        top.append(tl)
    return asm_ast.Program(top_level=top), written


def _callee_slots(fn, abi, shared, unreachable) -> dict[str, str]:
    """The parameter bytes of the functions `fn` calls, or jumps to as
    a tail call, that `fn` names, each mapped to its function. A
    register or `zeropage(addr)` byte, a byte in a share, and a byte a
    pointer might reach are left out."""
    from passes.abi_selection import ZpLayout
    labels = {ins.name for ins in fn.instructions
              if isinstance(ins, asm_ast.Label)}
    called = {
        ins.name if isinstance(ins, asm_ast.Call) else ins.target
        for ins in fn.instructions
        if isinstance(ins, asm_ast.Call)
        or isinstance(ins, asm_ast.Jump) and ins.target not in labels
    }
    out: dict[str, str] = {}
    for g in sorted(called - {fn.name}):
        layout = abi.get(g)
        if not isinstance(layout, ZpLayout):
            continue
        n = len(layout.slot_symbols)
        pinned = layout.pinned or [None] * n
        regs = layout.param_registers or [None] * n
        for k, sym in enumerate(layout.slot_symbols):
            if (pinned[k] is None and regs[k] is None
                    and sym not in shared and sym in unreachable):
                out[sym] = g
    return out


def _coalesce(fn: asm_ast.Function, slots, callee, unreachable, slot_symbols,
              all_owners=None):
    instrs = fn.instructions
    prefix = f"__local_{fn.name}__"
    names = {
        op.name for ins in instrs for op in _instruction_operands(ins)
        if isinstance(op, asm_ast.Data)
        and (op.name in slots or op.name in callee
             or op.name.startswith(prefix))
        and op.name in unreachable
    }
    names -= _excluded(instrs, names)
    names -= _by_address(instrs, names, slot_symbols)
    owners = {s: g for s, g in callee.items() if s in names}
    params = [s for s in slots if s in names]
    targets = params + sorted(owners, key=lambda s: _first_use(instrs, s))
    if not targets:
        return fn, []
    ctx = _Ctx(owners, {ins.name for ins in instrs
                        if isinstance(ins, asm_ast.Label)}, all_owners)
    live = _live_out(instrs, names, ctx)
    copies = _copy_sources(instrs, names)
    # The copies, by the names they write directly.
    pairs = set()
    into: set[tuple[str, str]] = set()
    for k, src in copies.items():
        for dst in _direct_reads_writes(instrs[k], names)[1]:
            pairs.add(frozenset((dst, src)))
            into.add((src, dst))
    locals_ = sorted(n for n in names if n.startswith(prefix))

    def merge(slot, taken, renames):
        """Merges into `slot` the locals not in `taken` that join it,
        and returns how many did."""
        members = [slot]
        callee_slot = slot in owners
        grew = True
        while grew:
            grew = False
            for local in locals_:
                if local in taken or local in members:
                    continue
                # A callee's slot takes a local copied into it, since
                # that copy is what goes.
                if callee_slot:
                    related = (local, slot) in into
                else:
                    related = any(frozenset((local, m)) in pairs
                                  for m in members)
                if not related:
                    continue
                if any(_interferes(local, m, instrs, live, copies, ctx)
                       for m in members):
                    continue
                members.append(local)
                taken.add(local)
                renames[local] = slot
                grew = True
        return len(members) - 1

    renames: dict[str, str] = {}
    taken: set[str] = set()
    merged: list[str] = []
    for p in params:
        if merge(p, taken, renames):
            merged.append(p)
    # Locals go in the slots of one callee at most. One function's
    # slots are apart, but two functions' may share a byte, and a local
    # kept in one's slot would be overwritten by a store that a merge
    # into the other's makes of a store to a local. The callee whose
    # slots take the most is chosen.
    best = None
    for g in sorted(set(owners.values())):
        trial_renames, trial_taken = dict(renames), set(taken)
        count = sum(
            merge(slot, trial_taken, trial_renames)
            for slot in targets if owners.get(slot) == g
        )
        if count and (best is None or count > best[0]):
            best = (count, trial_renames)
    if best is not None:
        renames = best[1]
    if not renames:
        return fn, []
    renamed = [_rename(ins, renames) for ins in instrs]
    # A copy between two of the merged names now stores a byte into
    # itself, here or on another path into a label, which the
    # peepholes see only within a block.
    keep = set(names) - set(renames)
    drop = _self_stores(renamed, keep)
    return asm_ast.Function(
        name=fn.name, is_global=fn.is_global, params=list(fn.params),
        instructions=[ins for k, ins in enumerate(renamed) if k not in drop],
    ), merged


def _excluded(instrs, names: set[str]) -> set[str]:
    """The names an instruction refers to in a way the pass doesn't
    follow: at an offset other than 0, by `IndexedData`, in a
    `LoadAddress`, in a volatile access, or in an instruction of a
    kind it doesn't model."""
    out: set[str] = set()
    for ins in instrs:
        ops = list(_instruction_operands(ins))
        if getattr(ins, "is_volatile", False):
            out.update(op.name for op in ops
                       if isinstance(op, asm_ast.Data) and op.name in names)
        for op in ops:
            if isinstance(op, asm_ast.Data) and op.name in names and op.offset:
                out.add(op.name)
            if isinstance(op, asm_ast.IndexedData) and op.name in names:
                out.add(op.name)
        known = isinstance(ins, (asm_ast.Mov, asm_ast.Pop, *_RMW, *_ALU,
                                 *_READ_ONLY, *_NO_OPERANDS))
        if not known:
            out.update(op.name for op in ops
                       if isinstance(op, (asm_ast.Data, asm_ast.IndexedData))
                       and op.name in names)
    return out


def _by_address(instrs, names, slot_symbols) -> set[str]:
    """The names whose bytes an operand reaches by address: a `ZP`
    operand, or either byte of an `IndirectZp` / `IndirectZpY` pointer
    base. A local used as a `(zp),Y` base is spelled that way, and a
    rename of its `Data` name wouldn't reach those."""
    reached: set[int] = set()
    for ins in instrs:
        for op in _instruction_operands(ins):
            if isinstance(op, asm_ast.ZP):
                reached.add(op.address + op.offset)
            elif isinstance(op, (asm_ast.IndirectZp, asm_ast.IndirectZpY)):
                reached.update((op.address, op.address + 1))
    return {
        n for n in names
        if n not in slot_symbols or slot_symbols[n] in reached
    }


def _data(op, names) -> str | None:
    if isinstance(op, asm_ast.Data) and op.name in names:
        return op.name
    return None


class _Ctx:
    """What a call does to the callee slots among the names: `owners`
    maps each to its function, and `labels` are the function's own
    labels, which tell a tail call's `JMP` from a jump within it."""

    def __init__(self, owners: dict[str, str], labels: set[str],
                 all_owners: dict[str, str] | None = None):
        self.owners = owners
        self.labels = labels
        # Every parameter slot's function, to tell a write to one of
        # the same function's slots from another's.
        self.all_owners = all_owners or owners

    def slot_owner(self, name: str) -> str | None:
        return self.all_owners.get(name)


_NO_CTX = _Ctx({}, set())


def _first_use(instrs, name: str) -> int:
    for k, ins in enumerate(instrs):
        if any(isinstance(op, asm_ast.Data) and op.name == name
               for op in _instruction_operands(ins)):
            return k
    return len(instrs)


def _reads_writes(ins, names, ctx: _Ctx = _NO_CTX) -> tuple[set[str], set[str]]:
    """The names `ins` reads and the names it writes. A call reads its
    callee's slots and may write any callee's, since functions that
    are never active together can share bytes. A tail call reads its
    callee's slots."""
    reads: set[str] = set()
    writes: set[str] = set()
    if isinstance(ins, asm_ast.Call):
        reads = {s for s in names if ctx.owners.get(s) == ins.name}
        writes = {s for s in names if s in ctx.owners}
        return reads, writes
    if isinstance(ins, asm_ast.Jump) and ins.target not in ctx.labels:
        return {s for s in names if ctx.owners.get(s) == ins.target}, set()
    reads, writes = _direct_reads_writes(ins, names)
    # A write to a parameter slot of one function may write the byte of
    # another function's slot: functions that are never active together
    # can share bytes, and the linker allocates them again for the whole
    # program. Only a function's own slots are certainly apart.
    written = (
        decimal_arith.writes(ins) if isinstance(ins, asm_ast.DecimalArith)
        else [getattr(ins, "dst", None)]
    )
    for op in _instruction_operands(ins):
        if (isinstance(op, asm_ast.Data) and op.name.startswith("__zpabi_")
                and any(op is w for w in written)):
            owner = ctx.slot_owner(op.name)
            writes |= {s for s in names if s in ctx.owners
                       and ctx.owners[s] != owner}
    return reads, writes


def _direct_reads_writes(ins, names) -> tuple[set[str], set[str]]:
    """The names `ins` reads and writes by name."""
    reads: set[str] = set()
    writes: set[str] = set()
    if isinstance(ins, asm_ast.DecimalArith):
        # Its names are all excluded (`_excluded`), so this only has
        # to be right for liveness, where its reads come first.
        reads = {op.name for op in decimal_arith.reads(ins) if _data(op, names)}
        writes = {
            op.name for op in decimal_arith.writes(ins) if _data(op, names)
        }
        return reads, writes
    if isinstance(ins, (asm_ast.Mov, asm_ast.Pop)):
        if isinstance(ins, asm_ast.Mov) and _data(ins.src, names):
            reads.add(ins.src.name)
        if _data(ins.dst, names):
            writes.add(ins.dst.name)
    elif isinstance(ins, _RMW):
        if _data(ins.dst, names):
            reads.add(ins.dst.name)
            writes.add(ins.dst.name)
    else:
        for op in _instruction_operands(ins):
            if _data(op, names):
                reads.add(op.name)
        if isinstance(ins, _ALU) and _data(ins.dst, names):
            writes.add(ins.dst.name)
    return reads, writes


def _successors(instrs, k: int, label_at: dict[str, int]) -> list[int]:
    ins = instrs[k]
    if isinstance(ins, (asm_ast.Return, asm_ast.Ret)):
        return []
    if isinstance(ins, asm_ast.Jump):
        t = label_at.get(ins.target)
        return [] if t is None or ins.reg_args else [t]
    out = [k + 1] if k + 1 < len(instrs) else []
    if isinstance(ins, asm_ast.Branch):
        t = label_at.get(ins.target)
        if t is not None:
            out.append(t)
    return out


def _live_out(instrs, names, ctx: _Ctx = _NO_CTX) -> list[set[str]]:
    """The names live just after each instruction. Nothing is live at
    a return or a jump out of the function, except the slots a tail
    call reads."""
    label_at = {ins.name: k for k, ins in enumerate(instrs)
                if isinstance(ins, asm_ast.Label)}
    succ = [_successors(instrs, k, label_at) for k in range(len(instrs))]
    rw = [_reads_writes(ins, names, ctx) for ins in instrs]
    live_in: list[set[str]] = [set() for _ in instrs]
    live_out: list[set[str]] = [set() for _ in instrs]
    changed = True
    while changed:
        changed = False
        for k in range(len(instrs) - 1, -1, -1):
            out = set()
            for s in succ[k]:
                out |= live_in[s]
            reads, writes = rw[k]
            new_in = reads | (out - writes)
            if out != live_out[k] or new_in != live_in[k]:
                live_out[k], live_in[k] = out, new_in
                changed = True
    return live_out


def _copy_sources(instrs, names) -> dict[int, str]:
    """For each instruction that writes one of `names` with the value
    of another, the other's name. Which register holds which name's
    value is followed forward through the function, and at a label it's
    what is the same on every path into it."""
    label_at = {ins.name: k for k, ins in enumerate(instrs)
                if isinstance(ins, asm_ast.Label)}
    preds: list[list[int]] = [[] for _ in instrs]
    for k in range(len(instrs)):
        for s in _successors(instrs, k, label_at):
            preds[s].append(k)
    state_out: list[dict | None] = [None] * len(instrs)
    changed = True
    while changed:
        changed = False
        for k, ins in enumerate(instrs):
            ins_state = _meet([state_out[p] for p in preds[k]], k == 0)
            out, _ = _step(ins_state, ins, names)
            if out != state_out[k]:
                state_out[k] = out
                changed = True
    copies: dict[int, str] = {}
    for k, ins in enumerate(instrs):
        _, copy = _step(_meet([state_out[p] for p in preds[k]], k == 0),
                        ins, names)
        if copy is not None:
            copies[k] = copy
    return copies


def _self_stores(instrs, names) -> set[int]:
    """The indices of the stores from a register into one of `names`
    that the register already holds the value of."""
    label_at = {ins.name: k for k, ins in enumerate(instrs)
                if isinstance(ins, asm_ast.Label)}
    preds: list[list[int]] = [[] for _ in instrs]
    for k in range(len(instrs)):
        for s in _successors(instrs, k, label_at):
            preds[s].append(k)
    state_out: list[dict | None] = [None] * len(instrs)
    changed = True
    while changed:
        changed = False
        for k, ins in enumerate(instrs):
            out, _ = _step(_meet([state_out[p] for p in preds[k]], k == 0),
                           ins, names)
            if out != state_out[k]:
                state_out[k] = out
                changed = True
    drop: set[int] = set()
    for k, ins in enumerate(instrs):
        if (isinstance(ins, asm_ast.Mov) and not ins.is_volatile
                and isinstance(ins.src, asm_ast.Reg)
                and _data(ins.dst, names) is not None):
            holds = _meet([state_out[p] for p in preds[k]], k == 0)
            if holds.get(type(ins.src.reg).__name__) == ins.dst.name:
                drop.add(k)
    return drop


def _meet(states, entry: bool) -> dict:
    """What is the same in every known state in `states`. At the
    function's entry, no register's contents are known."""
    known = [s for s in states if s is not None]
    if entry or not known:
        return {}
    out = dict(known[0])
    for s in known[1:]:
        out = {r: n for r, n in out.items() if s.get(r) == n}
    return out


def _step(holds: dict, ins, names) -> tuple[dict, str | None]:
    """The register contents after `ins`, given `holds` before it, and
    the name whose value `ins` writes into another of `names`, if any."""
    new = dict(holds)
    copy = None
    if isinstance(ins, asm_ast.Label):
        return new, None
    if isinstance(ins, asm_ast.Mov):
        src, dst = ins.src, ins.dst
        if isinstance(dst, asm_ast.Reg):
            if isinstance(src, asm_ast.Reg):
                value = holds.get(type(src.reg).__name__)
            else:
                value = _data(src, names)
            if _sets_up_y(ins):
                new["Y"] = None
            new[type(dst.reg).__name__] = value
            return _clean(new), None
        target = _data(dst, names)
        if isinstance(src, asm_ast.Reg):
            reg = type(src.reg).__name__
            value = holds.get(reg)
        else:
            # A memory-to-memory or immediate move goes through A.
            reg = "A"
            value = _data(src, names)
        if target is not None:
            if value is not None and value != target:
                copy = value
            new = {r: n for r, n in new.items() if n != target}
            new[reg] = target
        else:
            new[reg] = value if not isinstance(src, asm_ast.Reg) else new.get(reg)
        if _sets_up_y(ins):
            new["Y"] = None
        return _clean(new), copy
    if isinstance(ins, _KEEPS_REGISTERS):
        if _sets_up_y(ins):
            new["Y"] = None
        return _clean(new), None
    return {}, None


def _clean(holds: dict) -> dict:
    return {r: n for r, n in holds.items() if n is not None}


def _sets_up_y(ins) -> bool:
    from passes.asm_liveness import sets_up_y
    return sets_up_y(ins)


def _interferes(a: str, b: str, instrs, live, copies, ctx=_NO_CTX) -> bool:
    """True when an instruction writes one of `a` and `b` while the
    other is live after it, other than a write of one with the other's
    value."""
    for k, ins in enumerate(instrs):
        _, writes = _reads_writes(ins, {a, b}, ctx)
        for w in writes:
            other = b if w == a else a
            if other in live[k] and copies.get(k) != other:
                return True
    return False


def _rename(node, renames: dict[str, str]):
    if isinstance(node, asm_ast.Data):
        new = renames.get(node.name)
        return node if new is None else asm_ast.Data(name=new, offset=node.offset)
    if isinstance(node, list):
        return [_rename(x, renames) for x in node]
    if dataclasses.is_dataclass(node) and not isinstance(node, type):
        changes = {}
        for f in fields_of(node):
            val = getattr(node, f.name)
            new = _rename(val, renames)
            if new is not val:
                changes[f.name] = new
        return dataclasses.replace(node, **changes) if changes else node
    return node
