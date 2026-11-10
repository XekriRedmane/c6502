"""Merges repeated return tails into one copy that branches reach.

A function with several early returns of the same value ends each
one with the same few instructions, such as `LDX #0; TXA; RTS` for
`return 0` from a function returning a 16-bit value in A and X. When
a copy is skipped over by a conditional branch,

    Branch(cond, L)          BCS L
    <tail>                   LDX #0 / TXA / RTS
    Label(L)               L:

it becomes a branch on the opposite condition to another copy of the
same tail:

    Branch(!cond, T)         BCC T
    Label(L)               L:

That saves the tail's instructions at each site. The copy that's kept
is the last one in the function. If it isn't already labeled, a fresh
label `.tail@N` goes in front of it, which changes nothing for the
code that falls into it.

A tail is a run of 2 to `_MAX_TAIL` instructions ending in a
`Return`, containing no labels, branches, or jumps, so it behaves the
same wherever it runs. Hand-written 6502 code shares a not-found exit
the same way. A lone `RTS` isn't merged, since the branch to it would
save at most one instruction.

A site is rewritten only when the new branch's estimated distance,
from the byte sizes `expand_long_branches` uses, is within
`_BRANCH_REACH` bytes. Otherwise `expand_long_branches` would turn it
into an inverted branch over a `JMP`, which is no shorter than the
tail it replaced.
"""

from __future__ import annotations

import asm_ast

_MAX_TAIL = 4
# A margin under the 127 bytes a branch reaches, since later passes
# can still move code between the branch and its target.
_BRANCH_REACH = 120

_INVERSE = {
    asm_ast.CC: asm_ast.CS, asm_ast.CS: asm_ast.CC,
    asm_ast.EQ: asm_ast.NE, asm_ast.NE: asm_ast.EQ,
    asm_ast.MI: asm_ast.PL, asm_ast.PL: asm_ast.MI,
    asm_ast.VC: asm_ast.VS, asm_ast.VS: asm_ast.VC,
}
_CONTROL = (
    asm_ast.Label, asm_ast.Branch, asm_ast.Jump, asm_ast.Return,
    asm_ast.Ret,
)


def apply_tail_merge(prog: asm_ast.Program) -> asm_ast.Program:
    """Merges repeated return tails in every Function. Other
    top-levels pass through unchanged."""
    # Every label in the program, so a new `.tail@N` is unique even
    # across functions (the sim's assembler keeps one label table) and
    # across the fixed point's repeated runs of this pass.
    labels = {
        ins.name for tl in prog.top_level
        if isinstance(tl, asm_ast.Function)
        for ins in tl.instructions if isinstance(ins, asm_ast.Label)
    }
    counter = 0
    new_top: list[asm_ast.Type_top_level] = []
    for tl in prog.top_level:
        if isinstance(tl, asm_ast.Function):
            tl, counter = _rewrite_function(tl, counter, labels)
        new_top.append(tl)
    return asm_ast.Program(top_level=new_top)


def _tail_at(instrs, i) -> tuple | None:
    """Returns the tail starting at `instrs[i]` as a tuple, or None
    when no run of plain instructions ending in a `Return` starts
    there. The tuple's `repr` serves as its key, since asm nodes
    aren't hashable."""
    for n in range(2, _MAX_TAIL + 1):
        if i + n > len(instrs):
            return None
        last = instrs[i + n - 1]
        if isinstance(last, asm_ast.Return):
            body = instrs[i:i + n - 1]
            if any(isinstance(x, _CONTROL) for x in body):
                return None
            return tuple(instrs[i:i + n])
        if isinstance(last, _CONTROL):
            return None
    return None


def _in_reach(instrs, b: int, keep: int) -> bool:
    """True iff a branch at index `b` to the instruction at `keep` is
    estimated to be within `_BRANCH_REACH` bytes."""
    from sim.assembler import instruction_size
    lo, hi = sorted((b + 1, keep))
    try:
        dist = sum(
            instruction_size(x) for x in instrs[lo:hi]
            if not isinstance(x, asm_ast.Label)
        )
    except Exception:
        return False
    return dist <= _BRANCH_REACH


def _rewrite_function(
    fn: asm_ast.Function, counter: int, labels: set[str],
) -> tuple[asm_ast.Function, int]:
    """Rewrites the sites in one function. `labels` holds every label
    in the program and gains the ones minted here."""
    instrs = list(fn.instructions)
    # Sites: `Branch(c, L); <tail>; Label(L)` at index b.
    sites: list[tuple[int, tuple]] = []
    for b, ins in enumerate(instrs):
        if not isinstance(ins, asm_ast.Branch):
            continue
        tail = _tail_at(instrs, b + 1)
        if tail is None:
            continue
        after = b + 1 + len(tail)
        if not (after < len(instrs)
                and isinstance(instrs[after], asm_ast.Label)
                and instrs[after].name == ins.target):
            continue
        sites.append((b, tail))
    if not sites:
        return fn, counter
    # The kept copy of each tail is its last occurrence outside every
    # site's tail (a site's own tail may be kept, at its start), since
    # a site's tail is what gets removed.
    inside: set[int] = set()
    for b, tail in sites:
        inside.update(range(b + 2, b + 1 + len(tail)))
    last_copy: dict[str, int] = {}
    for i in range(len(instrs)):
        if i in inside:
            continue
        tail = _tail_at(instrs, i)
        if tail is not None:
            last_copy[repr(tail)] = i
    insert_label: dict[int, str] = {}
    target_of: dict[str, str] = {}
    rewrite: dict[int, asm_ast.Branch] = {}
    for b, tail in sites:
        keep = last_copy.get(repr(tail))
        if keep is None:
            continue
        if keep == b + 1:
            continue
        key = repr(tail)
        if key not in target_of:
            prev = instrs[keep - 1] if keep > 0 else None
            if isinstance(prev, asm_ast.Label):
                target_of[key] = prev.name
            else:
                name = f".tail@{counter}"
                while name in labels:
                    counter += 1
                    name = f".tail@{counter}"
                counter += 1
                labels.add(name)
                insert_label[keep] = name
                target_of[key] = name
        if not _in_reach(instrs, b, keep):
            continue
        cond = type(instrs[b].cond)
        rewrite[b] = asm_ast.Branch(
            cond=_INVERSE[cond](), target=target_of[key],
        )
    if not rewrite:
        return fn, counter
    skip: set[int] = set()
    for b in rewrite:
        skip.update(range(b + 1, b + 1 + len(dict(sites)[b])))
    out: list[asm_ast.Type_instruction] = []
    for i, ins in enumerate(instrs):
        if i in insert_label:
            out.append(asm_ast.Label(name=insert_label[i]))
        if i in skip:
            continue
        out.append(rewrite.get(i, ins))
    return asm_ast.Function(
        name=fn.name, is_global=fn.is_global,
        params=list(fn.params), instructions=out,
    ), counter
