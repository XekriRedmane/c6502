"""Keep a local copy of a function's zero-page result in the result's
own bytes.

# What it does

A `static` function `F` that returns a pointer returns it in its
zero-page object `F@ret` (passes/return_in_zero_page.py), and each
call copies `F@ret` into the call's result. When the result is a local
that lives on past the next call of `F`, or one that the function
changes, the copy stays, 8 bytes at each call:

    JSR   next_string
    LDA   next_string@ret
    STA   __local_display_message__msg_0
    LDA   next_string@ret+1
    STA   __local_display_message__msg_1

`display_message` in `examples/ali_baba/text.c` steps through a
table of strings that way. This pass renames such a local pair to
`F@ret` itself, when the two never hold different values at once, so
the copies become `LDA F@ret; STA F@ret`, which the peepholes drop.
It's `passes/slot_coalescing.py`'s rule for a parameter's slot,
applied to the bytes `F` returns its result in.

# When it applies

In a function `G` of the unit, for `F@ret` and two of `G`'s own
locals `L0` and `L1`:

  * `G` calls `F`, directly or through other functions of the unit,
    so that `G` is already a function whose call may write `F@ret`.
    `memory_value_propagation` and every other pass that keeps a fact
    about `F@ret` across a call drops it at a call of `G` (`G`'s call
    summary includes `F`'s writes), so the writes of `F@ret` that the
    rename adds to `G` change nothing that a caller of `G` holds.
  * `G` is in the private-pool model (`compile`'s `pool_eligible`),
    isn't recursive, and makes no `save_hardware_stack` call, so the
    linker rejects any program in which one of `G`'s calls leads back
    to `G` (see the external calls below).
  * `L0` and `L1` are copied from `F@ret` and `F@ret+1` somewhere in
    `G` (`LDA F@ret; STA L0`, through any register, or a
    memory-to-memory `Mov`). Neither is reached by a pointer
    (`asm_aliasing.unreachable_by_pointer`), each is named only at
    offset 0 and only by instructions whose reads and writes the pass
    models (`slot_coalescing._excluded`), and neither is named by a
    `ZP` operand. A `(zp),Y` access through the pair names it by `L0`'s
    address, which the rename moves to `F@ret`'s, and then `L1` must
    be the byte after `L0`.
  * Neither byte of the pair interferes with its byte of `F@ret`: no
    instruction writes one while the other is live after it, except a
    copy of one into the other. `F@ret` is live where `G` reads it,
    explicitly or as a `(zp),Y` base, and dead at a return or a tail
    call, since a caller of `G` treats a call of `G` as a write of
    `F@ret`. A call writes `F@ret` when it's a call of `F` or of a
    function of the unit that calls `F`, and may write it when it
    leaves the unit (a function the unit doesn't define, or one of the
    unit's that calls one). From outside, a function of the unit is
    reached only through its entry points, the global functions and
    the ones whose address is taken. An entry point that calls `F`
    and that leads to `G` would make a cycle through `G`'s call, which
    the linker rejects, so a call that leaves the unit can write
    `F@ret` only when an entry point calls `F` without leading to `G`.
    Calls don't read `F@ret` or `G`'s locals.

# Where it runs

Once, in `compile.optimized_asm`, right after `slot_coalescing`, and
before the peephole fixed points that drop the self-copies it leaves.
"""

from __future__ import annotations

import dataclasses

import asm_ast
from passes.asm_aliasing import _instruction_operands, unreachable_by_pointer
from passes.return_in_zero_page import return_slot_name
from passes.slot_coalescing import _copy_sources, _excluded, _successors

_RMW = (
    asm_ast.Inc, asm_ast.Dec, asm_ast.ArithmeticShiftLeft,
    asm_ast.LogicalShiftRight, asm_ast.RotateLeft, asm_ast.RotateRight,
)
_ALU = (asm_ast.Add, asm_ast.Sub, asm_ast.And, asm_ast.Or, asm_ast.Xor)
_READS = (asm_ast.Compare, asm_ast.BitTest, asm_ast.Push)
_NO_OPERANDS = (
    asm_ast.Branch, asm_ast.Label, asm_ast.Return, asm_ast.Ret,
    asm_ast.SetCarry, asm_ast.ClearCarry,
)
# The four bytes the analysis follows, by the names it gives them, and
# each one's partner, the byte it would share storage with.
_CELLS = ("$L0", "$L1", "$R0", "$R1")
_PARTNER = {"$L0": "$R0", "$R0": "$L0", "$L1": "$R1", "$R1": "$L1"}


def apply_ret_slot_coalescing(
    prog: asm_ast.Program,
    *,
    slot_symbols: dict[str, int],
    zp_addrs: dict[str, int],
    eligible: set[str],
) -> asm_ast.Program:
    """`prog` with each local pair that holds a copy of an `F@ret`
    renamed to `F@ret` where that's sound (see the module docstring).
    `slot_symbols` gives each local slot's address, `zp_addrs` each
    zero-page object's, and `eligible` names the functions in the
    private-pool model that aren't recursive and make no
    `save_hardware_stack` call."""
    fns = {tl.name: tl for tl in prog.top_level
           if isinstance(tl, asm_ast.Function)}
    rets = {
        tl.name: tl.name[:-len("@ret")] for tl in prog.top_level
        if isinstance(tl, asm_ast.StaticVariable)
        and tl.name.endswith("@ret")
        and return_slot_name(tl.name[:-len("@ret")]) == tl.name
        and tl.name[:-len("@ret")] in fns and tl.name in zp_addrs
        and zp_addrs[tl.name] < 0xFF
    }
    if not rets:
        return prog
    graph = _CallGraph(prog, fns)
    unreachable = unreachable_by_pointer(prog)
    top: list[asm_ast.Type_top_level] = []
    for tl in prog.top_level:
        if isinstance(tl, asm_ast.Function) and tl.name in eligible:
            for ret, f in sorted(rets.items()):
                if tl.name != f and f in graph.reach[tl.name]:
                    tl = _coalesce_all(tl, ret, f, graph, unreachable,
                                       slot_symbols, zp_addrs[ret])
        top.append(tl)
    return asm_ast.Program(top_level=top)


class _CallGraph:
    """The unit's direct calls, what each function leads to, and its
    entry points."""

    def __init__(self, prog: asm_ast.Program, fns: dict):
        self.defined = set(fns)
        callees: dict[str, set[str]] = {}
        for name, fn in fns.items():
            labels = {i.name for i in fn.instructions
                      if isinstance(i, asm_ast.Label)}
            callees[name] = {
                i.name if isinstance(i, asm_ast.Call) else i.target
                for i in fn.instructions
                if isinstance(i, asm_ast.Call)
                or isinstance(i, asm_ast.Jump) and i.target not in labels
            }
        # Every name each function leads to by calls, the names the
        # unit doesn't define included.
        self.reach: dict[str, set[str]] = {}
        for name in fns:
            seen: set[str] = set()
            stack = list(callees[name])
            while stack:
                g = stack.pop()
                if g in seen:
                    continue
                seen.add(g)
                stack.extend(callees.get(g, ()))
            self.reach[name] = seen
        taken: set[str] = set()
        for tl in prog.top_level:
            if isinstance(tl, asm_ast.StaticVariable):
                taken |= {i.name for i in tl.init
                          if isinstance(i, (asm_ast.AddressInit,
                                            asm_ast.AddressByteInit))}
            elif isinstance(tl, asm_ast.Function):
                for ins in tl.instructions:
                    for op in _instruction_operands(ins):
                        if isinstance(op, (asm_ast.ImmLabelLow,
                                           asm_ast.ImmLabelHigh)):
                            taken.add(op.name)
                    if isinstance(ins, asm_ast.LoadAddress):
                        taken.add(getattr(ins.src, "name", None))
        self.entries = {
            n for n, fn in fns.items() if fn.is_global or n in taken
        }

    def leaves_unit(self, name: str) -> bool:
        """True when a call of `name` may run a function the unit
        doesn't define."""
        if name not in self.defined:
            return True
        return any(g not in self.defined for g in self.reach[name])

    def may_write_ret(self, callee: str, f: str, g: str) -> bool:
        """True when a call of `callee` from `g` may write `f@ret`."""
        if callee == f or f in self.reach.get(callee, ()):
            return True
        if not self.leaves_unit(callee):
            return False
        return any(
            e != g and g not in self.reach[e]
            for e in self.entries
            if e == f or f in self.reach[e]
        )


def _coalesce_all(fn, ret, f, graph, unreachable, slot_symbols, ret_addr):
    """`fn` with every local pair that can be `ret` renamed to it, one
    pair at a time."""
    for _ in range(8):
        new = _coalesce_one(fn, ret, f, graph, unreachable, slot_symbols,
                            ret_addr)
        if new is fn:
            return fn
        fn = new
    return fn


def _copies_of(instrs, ret: str, prefix: str) -> list[set[str]]:
    """The locals each byte of `ret` is copied into by `LDA ret+k; STA
    L` or a memory-to-memory `Mov`, for k = 0 and 1."""
    out: list[set[str]] = [set(), set()]
    for k, ins in enumerate(instrs):
        if not isinstance(ins, asm_ast.Mov) or ins.is_volatile:
            continue
        src, dst = ins.src, ins.dst
        if (isinstance(src, asm_ast.Reg) and isinstance(src.reg, asm_ast.A)
                and k and isinstance(instrs[k - 1], asm_ast.Mov)
                and instrs[k - 1].dst == src):
            src = instrs[k - 1].src
        if (isinstance(src, asm_ast.Data) and src.name == ret
                and src.offset in (0, 1)
                and isinstance(dst, asm_ast.Data) and dst.offset == 0
                and dst.name.startswith(prefix)):
            out[src.offset].add(dst.name)
    return out


def _coalesce_one(fn, ret, f, graph, unreachable, slot_symbols, ret_addr):
    instrs = fn.instructions
    lows, highs = _copies_of(instrs, ret, f"__local_{fn.name}__")
    for l0 in sorted(lows):
        for l1 in sorted(highs):
            if l0 == l1:
                continue
            renamed = _try_pair(fn, l0, l1, ret, f, graph, unreachable,
                                slot_symbols, ret_addr)
            if renamed is not None:
                return renamed
    return fn


def _try_pair(fn, l0, l1, ret, f, graph, unreachable, slot_symbols,
              ret_addr):
    """`fn` with `l0` and `l1` renamed to `ret`'s two bytes, or None
    when that isn't sound."""
    instrs = fn.instructions
    if not (l0 in unreachable and l1 in unreachable):
        return None
    if l0 not in slot_symbols or l1 not in slot_symbols:
        return None
    if _excluded(instrs, {l0, l1}):
        return None
    a0, a1 = slot_symbols[l0], slot_symbols[l1]
    cells = {a0: "$L0", a1: "$L1", ret_addr: "$R0", ret_addr + 1: "$R1"}
    names = {(l0, 0): "$L0", (l1, 0): "$L1", (ret, 0): "$R0",
             (ret, 1): "$R1"}
    canon = []
    for ins in instrs:
        c = _canonical(ins, names, cells, a0)
        if c is None:
            return None
        canon.append(c)
    rw = [_cell_reads_writes(c, cells, f, fn.name, graph) for c in canon]
    live = _live_out(canon, rw)
    copies = _copy_sources(canon, set(_CELLS))
    for k, (_, writes) in enumerate(rw):
        for w in writes:
            other = _PARTNER[w]
            if other in live[k] and copies.get(k) != other:
                return None
    renames = {(l0, 0): asm_ast.Data(name=ret, offset=0),
               (l1, 0): asm_ast.Data(name=ret, offset=1)}
    out = [_rename(ins, renames, a0, ret_addr) for ins in instrs]
    return asm_ast.Function(
        name=fn.name, is_global=fn.is_global, params=list(fn.params),
        instructions=out,
    )


def _canonical(ins, names, cells, a0):
    """`ins` with each operand that names one of the four bytes by name
    spelled `Data("$L0", 0)` and so on, or None when an operand reaches
    one of them in a way the pass doesn't follow: a `ZP` operand, an
    `IndexedData`, a `(zp),Y` base whose pair overlaps the bytes other
    than at `L0` with `L1` after it or at `F@ret`, or an instruction
    kind the pass doesn't model."""
    ops = list(_instruction_operands(ins))
    touched = False
    for op in ops:
        if isinstance(op, asm_ast.ZP) and op.address + op.offset in cells:
            return None
        if isinstance(op, asm_ast.IndexedData) and (
                (op.name, 0) in names or (op.name, 1) in names):
            return None
        if isinstance(op, (asm_ast.IndirectZp, asm_ast.IndirectZpY)):
            lo = cells.get(op.address)
            hi = cells.get(op.address + 1)
            if lo is None and hi is None:
                continue
            if (lo, hi) not in (("$L0", "$L1"), ("$R0", "$R1")):
                return None
            touched = True
        if isinstance(op, asm_ast.Data) and (op.name, op.offset) in names:
            touched = True
        elif isinstance(op, asm_ast.Data) and any(
                op.name == n for n, _ in names):
            return None     # another byte of `ret`
    if not touched:
        return ins
    if not isinstance(ins, (asm_ast.Mov, asm_ast.Pop, *_RMW, *_ALU,
                            *_READS)):
        return None
    if getattr(ins, "is_volatile", False):
        return None

    def rename(x):
        if isinstance(x, asm_ast.Data) and (x.name, x.offset) in names:
            return asm_ast.Data(name=names[(x.name, x.offset)], offset=0)
        return x
    return dataclasses.replace(ins, **{
        fld.name: rename(getattr(ins, fld.name))
        for fld in dataclasses.fields(ins)
    })


def _base_cells(op, cells) -> set[str]:
    """The bytes of the pair a `(zp),Y` operand reads its pointer from."""
    if isinstance(op, (asm_ast.IndirectZp, asm_ast.IndirectZpY)):
        return {c for c in (cells.get(op.address), cells.get(op.address + 1))
                if c is not None}
    return set()


def _cell_reads_writes(ins, cells, f, g, graph):
    """The cells the canonical instruction `ins` of `g` reads and
    writes. `cells` maps the four bytes' addresses to the cells, for
    the `(zp),Y` operands, which keep their addresses. A call reads
    none of them and writes `f@ret`'s when it may."""
    if isinstance(ins, asm_ast.Call):
        writes = ({"$R0", "$R1"} if graph.may_write_ret(ins.name, f, g)
                  else set())
        return set(), writes
    reads: set[str] = set()
    writes: set[str] = set()

    def name(op):
        if isinstance(op, asm_ast.Data) and op.name in _PARTNER:
            return op.name
        return None
    for op in _instruction_operands(ins):
        reads |= _base_cells(op, cells)
    if isinstance(ins, (asm_ast.Mov, asm_ast.Pop)):
        if isinstance(ins, asm_ast.Mov) and name(ins.src):
            reads.add(name(ins.src))
        if name(ins.dst):
            writes.add(name(ins.dst))
    elif isinstance(ins, _RMW):
        if name(ins.dst):
            reads.add(name(ins.dst))
            writes.add(name(ins.dst))
    else:
        for op in _instruction_operands(ins):
            if name(op):
                reads.add(name(op))
        if isinstance(ins, _ALU) and name(ins.dst):
            writes.add(name(ins.dst))
    return reads, writes


def _live_out(instrs, rw) -> list[set[str]]:
    """The cells live just after each instruction. Nothing is live at a
    return or at a jump out of the function."""
    label_at = {ins.name: k for k, ins in enumerate(instrs)
                if isinstance(ins, asm_ast.Label)}
    succ = [_successors(instrs, k, label_at) for k in range(len(instrs))]
    live_in: list[set[str]] = [set() for _ in instrs]
    live_out: list[set[str]] = [set() for _ in instrs]
    changed = True
    while changed:
        changed = False
        for k in range(len(instrs) - 1, -1, -1):
            out: set[str] = set()
            for s in succ[k]:
                out |= live_in[s]
            reads, writes = rw[k]
            new_in = reads | (out - writes)
            if out != live_out[k] or new_in != live_in[k]:
                live_out[k], live_in[k] = out, new_in
                changed = True
    return live_out


def _rename(node, renames, a0, ret_addr):
    """`node` with the locals' `Data` operands renamed to `ret`'s bytes
    and a `(zp),Y` base at `a0` moved to `ret_addr`."""
    if isinstance(node, asm_ast.Data):
        return renames.get((node.name, node.offset), node)
    if isinstance(node, asm_ast.IndirectZp) and node.address == a0:
        return asm_ast.IndirectZp(address=ret_addr, offset=node.offset)
    if isinstance(node, asm_ast.IndirectZpY) and node.address == a0:
        return asm_ast.IndirectZpY(address=ret_addr)
    if isinstance(node, list):
        return [_rename(x, renames, a0, ret_addr) for x in node]
    if dataclasses.is_dataclass(node) and not isinstance(node, type):
        changes = {}
        for fld in dataclasses.fields(node):
            val = getattr(node, fld.name)
            new = _rename(val, renames, a0, ret_addr)
            if new is not val:
                changes[fld.name] = new
        return dataclasses.replace(node, **changes) if changes else node
    return node
