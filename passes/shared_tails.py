"""Shares the code two functions of a unit end the same way with.

# Pattern

Two functions often end with the same instructions: the same call
with the same constant argument, or the same store of a byte into a
callee's parameter and a tail call. In Zork's `objects.c`,
`instr_get_parent`, `instr_get_next_prop`, and `instr_get_prop_len`
each end

    STA   __zpabi_store_result__value_0
    LDA   #$00
    STA   __zpabi_store_result__value_1
    JMP   store_result

[Cross jumping](../docs/optimizations/cross_jump.md) shares such code
inside one function. Across functions, a `JMP` can't reach a label
inside another function, since dasm and `binutils.assembler` scope a
`.` label to its function, and a label of any other kind starts an
item the linker may move. So the shared instructions become a function
of their own, `F@tail<n>`, internal to the unit, and each function that
ended with them ends with `JMP F@tail<n>` instead. The linker places
one of them right before it and drops that `JMP`
(`link_layout.fall_through`), as it does for any function ending in a
jump to another.

# Conditions

A *segment* is a run of instructions that ends with a `JMP` or an
`RTS`, doesn't start with a label, and whose branches and jumps to
`.` labels all go to labels inside it. No instruction outside a
segment may go to a label inside it. Two segments match when they're
the same instructions with their labels renamed consistently. Neither
function may have a soft-stack frame (a `FunctionPrologue`), whose
`Ret` depends on the function's own frame, and a segment may not hold
a `JumpTable`, whose table is named for its function.

The instructions are the same, so they do the same thing in either
function: they name only registers, globals, and other functions'
slots, since a function's own slots and labels are spelled with its
name. A `(zp),Y` pointer is the exception, since the operand carries
the pointer's address, and `asm_emit` names it by the slot the
function it's in keeps there. Two functions' slots can share those
bytes in a unit and be placed apart by the linker, so a segment may
not read or write through a pointer in a slot's bytes
(`asm_aliasing.through_slot_pointer`). The new function has no parameters or locals, and the link
metadata's call graph, built from the TAC, already has every call it
makes as a call of the functions it came from.

A segment of `s` bytes shared by `n` functions takes `s + 3n` bytes
in place of `n * s`, and the linker saves one of the `JMP`s. The pass
shares a segment when that's smaller before the link too, when `s +
3n < n * s`. It looks for the most bytes saved first, adds every
other function that ends with the same segment, and repeats.

# Cost of the search

The search compares every two ends of every two functions, and for
each match counts the other functions that end with the same segment.
It used to compute each segment's normal form again at every
comparison, and the check that no outside jump goes into a segment
scanned the whole function each time. Guards.c called `_form` 57,598
times, a quarter of its compile. The search now keeps a `_Fn` for each
function object, with its instructions' shapes, the positions that
name each label, and each segment's form and size once computed. A
pass that changes a function makes a new `asm_ast.Function`, so a
`_Fn` is reused only for the object it was made from, across the
rounds of `apply_shared_tails` too. Two ends are compared only when
their last instructions have the same shape, since segments that end
differently can't match, and the other functions that end with a
form are looked up in a table of each length's forms. The pairs are
taken in the same order and the first segment with the most bytes
saved still wins, so the pass chooses the same segments as before.

# Where it runs

Last in `compile.optimized_asm`, after `apply_short_jumps`, which
drops a label nothing in its own function names. Every change only
removes bytes from a function, so no branch falls out of reach.
"""
from __future__ import annotations

from dataclasses import replace

import asm_ast
from passes.asm_aliasing import through_slot_pointer
from sim.assembler import instruction_size

_ENDS = (asm_ast.Jump, asm_ast.Return)


def apply_shared_tails(
    prog: asm_ast.Program, symbols, *, zp_names=None,
    slot_addrs=frozenset(),
) -> asm_ast.Program:
    """`prog` with each shared segment in a function of its own (see
    the module docstring). Each new function's symbol, a copy of the
    first sharer's with internal linkage, is added to `symbols`.
    `zp_names` are the names of zero-page bytes, which take one-byte
    operands, for the sizes of the segments
    (`sim.assembler.instruction_size`). `slot_addrs` are the addresses
    of the unit's `__zpabi_*` and `__local_*` slots, whose bytes no
    `(zp),Y` pointer in a segment may have."""
    top = list(prog.top_level)
    count = 0
    infos: dict[int, _Fn] = {}
    while True:
        best = _best_share(top, zp_names, slot_addrs, infos)
        if best is None:
            return asm_ast.Program(top_level=top)
        owner, segments = best
        name = f"{owner}@tail{count}"
        count += 1
        body = None
        for idx, (fi, start, end) in sorted(
            segments.items(), key=lambda kv: kv[1][0],
        ):
            fn = top[fi]
            if body is None:
                body = fn.instructions[start:end + 1]
            instrs = (
                fn.instructions[:start]
                + [asm_ast.Jump(target=name, reg_args=["A", "X", "Y"])]
                + fn.instructions[end + 1:]
            )
            top[fi] = asm_ast.Function(
                name=fn.name, is_global=fn.is_global,
                params=list(fn.params), instructions=instrs,
            )
        sym = symbols.get(owner)
        if sym is not None:
            symbols[name] = replace(
                sym, attrs=replace(sym.attrs, is_global=False),
            )
        at = next(i for i, tl in enumerate(top)
                  if isinstance(tl, asm_ast.Function) and tl.name == owner)
        top.insert(at + 1, asm_ast.Function(
            name=name, is_global=False, params=[],
            instructions=[*body],
        ))


class _Fn:
    """What `_best_share` reads of one function, computed once per
    function object: each instruction's shape (`_shape`), which of them
    may not be in a segment, the positions that name each label, the
    positions of its ends, and the normal form (`_form`) and size of
    each segment as it's asked for. A function that changes is a new
    `asm_ast.Function`, so a `_Fn` stays valid for the object it was
    made from."""

    def __init__(self, fn, zp_names, slot_addrs) -> None:
        self.fn = fn
        instrs = fn.instructions
        self.instrs = instrs
        self.zp_names = zp_names
        self.eligible = not any(
            isinstance(i, asm_ast.FunctionPrologue) for i in instrs)
        self.shapes = [_shape(ins) for ins in instrs]
        self.blocked = [
            isinstance(ins, asm_ast.JumpTable)
            or through_slot_pointer(ins, slot_addrs)
            for ins in instrs
        ]
        self.ends = [k for k, ins in enumerate(instrs)
                     if isinstance(ins, _ENDS)]
        # The ends of each shape, in order.
        self.ends_by_shape: dict = {}
        for k in self.ends:
            self.ends_by_shape.setdefault(self.shapes[k], []).append(k)
        # The positions of the jumps and branches to each label.
        self.refs: dict[str, list[int]] = {}
        for k, ins in enumerate(instrs):
            if isinstance(ins, (asm_ast.Branch, asm_ast.Jump)):
                self.refs.setdefault(ins.target, []).append(k)
            elif isinstance(ins, asm_ast.JumpTable):
                for x in ins.targets:
                    self.refs.setdefault(x, []).append(k)
        self._forms: dict[tuple[int, int], tuple | None] = {}
        self._sizes: dict[tuple[int, int], int] = {}

    def form(self, s: int, t: int):
        """`_form(instrs, s, t, slot_addrs)`, computed once."""
        key = (s, t)
        try:
            return self._forms[key]
        except KeyError:
            pass
        out = self._forms[key] = self._form(s, t)
        return out

    def _form(self, s: int, t: int):
        instrs = self.instrs
        if s > t or s < 0 or t >= len(instrs):
            return None
        if isinstance(instrs[s], asm_ast.Label):
            return None
        if not isinstance(instrs[t], _ENDS):
            return None
        inside = {}
        n = 0
        for k in range(s, t + 1):
            ins = instrs[k]
            if isinstance(ins, asm_ast.Label):
                inside[ins.name] = n
                n += 1
        out = []
        shapes = self.shapes
        blocked = self.blocked
        for k in range(s, t + 1):
            if blocked[k]:
                return None
            ins = instrs[k]
            if isinstance(ins, asm_ast.Label):
                out.append(("label", inside[ins.name]))
            elif isinstance(ins, asm_ast.Branch):
                if ins.target not in inside:
                    return None
                out.append(("branch", shapes[k][1], inside[ins.target]))
            elif isinstance(ins, asm_ast.Jump) and ins.target.startswith("."):
                if ins.target not in inside:
                    return None
                out.append(("jump", inside[ins.target]))
            else:
                out.append(shapes[k])
        # Nothing outside the segment may go to a label inside it.
        for name in inside:
            for k in self.refs.get(name, ()):
                if k < s or k > t:
                    return None
        return tuple(out)

    def size(self, s: int, t: int) -> int:
        """The bytes of `instrs[s..t]`."""
        key = (s, t)
        size = self._sizes.get(key)
        if size is None:
            size = self._sizes[key] = sum(
                instruction_size(x, zp_names=self.zp_names)
                for x in self.instrs[s:t + 1])
        return size


def _info(infos, fn, zp_names, slot_addrs) -> _Fn:
    """The `_Fn` of `fn` in `infos`, made if it isn't there."""
    info = infos.get(id(fn))
    if info is None or info.fn is not fn:
        info = infos[id(fn)] = _Fn(fn, zp_names, slot_addrs)
    return info


def _best_share(top, zp_names=None, slot_addrs=frozenset(), infos=None):
    """`(owner, {key: (function index, start, end)})` for the segment
    that saves the most bytes, or None. `infos` keeps each function's
    `_Fn` between calls."""
    if infos is None:
        infos = {}
    fns = []
    for i, tl in enumerate(top):
        if isinstance(tl, asm_ast.Function):
            info = _info(infos, tl, zp_names, slot_addrs)
            if info.eligible:
                fns.append((i, info))
    # For each segment length, each normal form's first end in each
    # function, in the order of `fns`.
    by_length: dict[int, dict] = {}

    def ends_with(length: int, form) -> dict[int, int]:
        index = by_length.get(length)
        if index is None:
            index = by_length[length] = {}
            for fc, c in fns:
                for tc in c.ends:
                    f = c.form(tc - length + 1, tc)
                    if f is not None:
                        index.setdefault(f, {}).setdefault(fc, tc)
        return index.get(form, {})

    best = None
    best_gain = 0
    for ai, (fa, a) in enumerate(fns):
        for fb, b in fns[ai + 1:]:
            for ta in a.ends:
                # Segments that end in different shapes don't match.
                for tb in b.ends_by_shape.get(a.shapes[ta], ()):
                    seg = _common(a, ta, b, tb)
                    if seg is None:
                        continue
                    sa, sb, form = seg
                    size = a.size(sa, ta)
                    length = ta - sa + 1
                    others = ends_with(length, form)
                    n = 2 + len(others) - (fa in others) - (fb in others)
                    gain = n * size - (size + 3 * n)
                    if gain > best_gain:
                        best_gain = gain
                        members = {fa: (sa, ta), fb: (sb, tb)}
                        for fc, tc in others.items():
                            if fc not in members:
                                members[fc] = (tc - length + 1, tc)
                        best = (a.fn.name, {f: (f, s, t)
                                            for f, (s, t) in members.items()})
    return best


def _common(a: _Fn, ta: int, b: _Fn, tb: int):
    """`(start in a, start in b, normal form)` of the longest matching
    segments that end at `a[ta]` and `b[tb]`, or None."""
    sa_shapes, sb_shapes = a.shapes, b.shapes
    k = 0
    while (ta - k >= 0 and tb - k >= 0
           and sa_shapes[ta - k] == sb_shapes[tb - k]):
        k += 1
    for length in range(k, 0, -1):
        sa, sb = ta - length + 1, tb - length + 1
        form = a.form(sa, ta)
        if form is not None and form == b.form(sb, tb):
            return sa, sb, form
    return None


def _shape(ins):
    """An instruction with its local labels blanked, to find how far
    back two segments can match. An instruction with no label is its
    `repr`, which is also its entry in a normal form."""
    if isinstance(ins, asm_ast.Label):
        return ("label",)
    if isinstance(ins, asm_ast.Branch):
        return ("branch", repr(ins.cond))
    if isinstance(ins, asm_ast.Jump) and ins.target.startswith("."):
        return ("jump",)
    return repr(ins)
