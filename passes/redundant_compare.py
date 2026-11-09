"""Drops a compare whose flags are already set by an identical compare
on the only path into it.

    Compare(A, M)          CMP M
    Branch(cond, L)        BCS L
    ...                    ...         (no fall-through into L)
    Label(L)             L:
    Compare(A, M)          CMP M       <- dropped

When L is reached only by that branch, A and M are the values the
first compare saw, and a branch changes no flags, so the second
compare sets every flag the same way. This is the shape of two tests
of the same operands in a row, such as

    if (key < entry) return 0;
    if (key == entry) ...

which `tac_to_asm` lowers as two `LDA key; CMP entry` pairs, and
whose second `LDA key` is already dropped as redundant.

L must have no other way in. The instruction before the run of labels
that ends at the compare has to be a `Jump`, `Return`, or `Ret`, so
nothing falls through, and exactly one `Branch` or `Jump` in the
function may target a label in the run. A label whose address is
taken (`ImmLabelLow` / `ImmLabelHigh`) disqualifies the run.

A compare of a volatile read is never followed by a branch
(`volatile_void_read_cmp` makes that rewrite only when the flags
are dead), so dropping one of a pair never drops a volatile access.
"""

from __future__ import annotations

import asm_ast

_NO_FALL_THROUGH = (asm_ast.Jump, asm_ast.Return, asm_ast.Ret)


def apply_redundant_compare(prog: asm_ast.Program) -> asm_ast.Program:
    """Drops every redundant compare in every Function. Other
    top-levels pass through unchanged."""
    return asm_ast.Program(top_level=[
        _rewrite_function(tl) if isinstance(tl, asm_ast.Function) else tl
        for tl in prog.top_level
    ])


def _label_refs(instrs) -> dict[str, list[int]]:
    """Maps each label name to the indices of the Branch / Jump
    instructions that target it. A label named by an `ImmLabelLow` /
    `ImmLabelHigh` operand gets a -1 entry, which no caller accepts."""
    refs: dict[str, list[int]] = {}
    for i, ins in enumerate(instrs):
        if isinstance(ins, (asm_ast.Branch, asm_ast.Jump)):
            refs.setdefault(ins.target, []).append(i)
        for op in vars(ins).values():
            if isinstance(op, (asm_ast.ImmLabelLow, asm_ast.ImmLabelHigh)):
                refs.setdefault(op.name, []).append(-1)
    return refs


def _rewrite_function(fn: asm_ast.Function) -> asm_ast.Function:
    instrs = fn.instructions
    refs = _label_refs(instrs)
    drop: set[int] = set()
    for c, cmp in enumerate(instrs):
        if not isinstance(cmp, asm_ast.Compare):
            continue
        s = c
        while s > 0 and isinstance(instrs[s - 1], asm_ast.Label):
            s -= 1
        if s == c or s == 0:
            continue
        if not isinstance(instrs[s - 1], _NO_FALL_THROUGH):
            continue
        sources = [
            b for ins in instrs[s:c] for b in refs.get(ins.name, [])
        ]
        if len(sources) != 1:
            continue
        b = sources[0]
        if b < 1 or not isinstance(instrs[b], asm_ast.Branch):
            continue
        if instrs[b - 1] != cmp:
            continue
        drop.add(c)
    if not drop:
        return fn
    return asm_ast.Function(
        name=fn.name, is_global=fn.is_global, params=list(fn.params),
        instructions=[ins for i, ins in enumerate(instrs) if i not in drop],
    )
