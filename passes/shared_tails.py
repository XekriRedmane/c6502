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
name. The new function has no parameters or locals, and the link
metadata's call graph, built from the TAC, already has every call it
makes as a call of the functions it came from.

A segment of `s` bytes shared by `n` functions takes `s + 3n` bytes
in place of `n * s`, and the linker saves one of the `JMP`s. The pass
shares a segment when that's smaller before the link too, when `s +
3n < n * s`. It looks for the most bytes saved first, adds every
other function that ends with the same segment, and repeats.

# Where it runs

Last in `compile.optimized_asm`, after `apply_short_jumps`, which
drops a label nothing in its own function names. Every change only
removes bytes from a function, so no branch falls out of reach.
"""
from __future__ import annotations

from dataclasses import replace

import asm_ast
from sim.assembler import instruction_size

_ENDS = (asm_ast.Jump, asm_ast.Return)


def apply_shared_tails(
    prog: asm_ast.Program, symbols, *, zp_names=None,
) -> asm_ast.Program:
    """`prog` with each shared segment in a function of its own (see
    the module docstring). Each new function's symbol, a copy of the
    first sharer's with internal linkage, is added to `symbols`.
    `zp_names` are the names of zero-page bytes, which take one-byte
    operands, for the sizes of the segments
    (`sim.assembler.instruction_size`)."""
    top = list(prog.top_level)
    count = 0
    while True:
        best = _best_share(top, zp_names)
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


def _eligible(tl) -> bool:
    return (isinstance(tl, asm_ast.Function)
            and not any(isinstance(i, asm_ast.FunctionPrologue)
                        for i in tl.instructions))


def _best_share(top, zp_names=None):
    """`(owner, {key: (function index, start, end)})` for the segment
    that saves the most bytes, or None."""
    fns = [(i, tl) for i, tl in enumerate(top) if _eligible(tl)]
    ends = {i: [k for k, ins in enumerate(tl.instructions)
                if isinstance(ins, _ENDS)] for i, tl in fns}
    best = None
    best_gain = 0
    for ai, (fa, a) in enumerate(fns):
        for fb, b in fns[ai + 1:]:
            for ta in ends[fa]:
                for tb in ends[fb]:
                    seg = _common(a.instructions, ta, b.instructions, tb)
                    if seg is None:
                        continue
                    sa, sb, form = seg
                    size = sum(instruction_size(x, zp_names=zp_names)
                               for x in a.instructions[sa:ta + 1])
                    members = {fa: (sa, ta), fb: (sb, tb)}
                    for fc, c in fns:
                        if fc in members:
                            continue
                        for tc in ends[fc]:
                            sc = tc - (ta - sa)
                            if sc >= 0 and _form(c.instructions, sc, tc) == form:
                                members[fc] = (sc, tc)
                                break
                    n = len(members)
                    gain = n * size - (size + 3 * n)
                    if gain > best_gain:
                        best_gain = gain
                        best = (a.name, {f: (f, s, t)
                                         for f, (s, t) in members.items()})
    return best


def _common(a, ta, b, tb):
    """`(start in a, start in b, normal form)` of the longest matching
    segments that end at `a[ta]` and `b[tb]`, or None."""
    k = 0
    while (ta - k >= 0 and tb - k >= 0
           and _shape(a[ta - k]) == _shape(b[tb - k])):
        k += 1
    for length in range(k, 0, -1):
        sa, sb = ta - length + 1, tb - length + 1
        form = _form(a, sa, ta)
        if form is not None and form == _form(b, sb, tb):
            return sa, sb, form
    return None


def _shape(ins):
    """An instruction with its local labels blanked, to find how far
    back two segments can match."""
    if isinstance(ins, asm_ast.Label):
        return ("label",)
    if isinstance(ins, asm_ast.Branch):
        return ("branch", repr(ins.cond))
    if isinstance(ins, asm_ast.Jump) and ins.target.startswith("."):
        return ("jump",)
    return repr(ins)


def _form(instrs, s, t):
    """The segment `instrs[s..t]` with its labels numbered in order,
    or None when it isn't a segment (see the module docstring)."""
    seg = instrs[s:t + 1]
    if not seg or isinstance(seg[0], asm_ast.Label):
        return None
    if not isinstance(seg[-1], _ENDS):
        return None
    inside = {ins.name: n for n, ins in enumerate(
        x for x in seg if isinstance(x, asm_ast.Label))}
    out = []
    for ins in seg:
        if isinstance(ins, asm_ast.JumpTable):
            return None
        if isinstance(ins, asm_ast.Label):
            out.append(("label", inside[ins.name]))
        elif isinstance(ins, asm_ast.Branch):
            if ins.target not in inside:
                return None
            out.append(("branch", repr(ins.cond), inside[ins.target]))
        elif isinstance(ins, asm_ast.Jump) and ins.target.startswith("."):
            if ins.target not in inside:
                return None
            out.append(("jump", inside[ins.target]))
        else:
            out.append(repr(ins))
    # Nothing outside the segment may go to a label inside it.
    for k, ins in enumerate(instrs):
        if s <= k <= t:
            continue
        targets = []
        if isinstance(ins, (asm_ast.Branch, asm_ast.Jump)):
            targets = [ins.target]
        elif isinstance(ins, asm_ast.JumpTable):
            targets = list(ins.targets)
        if any(x in inside for x in targets):
            return None
    return tuple(out)
