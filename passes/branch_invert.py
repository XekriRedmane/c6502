"""Branch-around-jump inversion peephole.

`tac_to_asm` emits the natural lowering of `if (cond) JumpIfFalse
target` and similar shapes as a conditional branch around an
unconditional jump:

    Branch(cond, L)
    Jump(target)
    Label(L)

The branch skips the jump when `cond` is true; otherwise control
falls into the jump and lands at `target`. The inverted form is
strictly shorter (no JMP) and strictly faster on the taken path
(branches are 2 bytes / 2-3 cycles; JMP is 3 bytes / 3 cycles):

    Branch(!cond, target)
    Label(L)

`Label(L)` is preserved — other instructions may jump to it. A
dead-label cleanup elsewhere can drop it if it ends up unused. The
inversion is sound regardless of L's other refs.

This pattern is what `tac_to_asm` emits for every C `if` whose
condition has the wrong polarity for a direct fall-through (i.e.
"branch around the JumpIfFalse-induced JMP"), so the peephole
fires across most non-trivial control flow.

Soundness:
  * `cond` and `!cond` describe disjoint sets of flag states (BCC
    fires iff C=0, BCS iff C=1; same for the other 3 pairs), so
    branching on `!cond` to `target` produces the same control
    flow as branching on `cond` past the JMP, then falling into
    the JMP.
  * The Branch atom doesn't read or write any operand state — only
    the flag bits, which are unchanged by the rewrite.

Where to run: in the asm-peephole fixed-point loop (after
`replace_pseudoregisters`, before `expand_long_branches`). The
inverted branch's target may be further away than the original
short `L` target, so `expand_long_branches` must run after — but
that's already the established ordering.

Doesn't grow code (Branch+Jump+Label is 3 atoms → Branch+Label is
2), so the fixed-point loop's monotone-shrinking invariant
holds."""

from __future__ import annotations

import asm_ast


_INVERT: dict[type, type] = {
    asm_ast.EQ: asm_ast.NE,
    asm_ast.NE: asm_ast.EQ,
    asm_ast.CC: asm_ast.CS,
    asm_ast.CS: asm_ast.CC,
    asm_ast.MI: asm_ast.PL,
    asm_ast.PL: asm_ast.MI,
    asm_ast.VC: asm_ast.VS,
    asm_ast.VS: asm_ast.VC,
}


def apply_branch_invert(prog: asm_ast.Program) -> asm_ast.Program:
    """Walk every Function and collapse Branch-around-Jump-to-Label
    triples into a single inverted-condition Branch."""
    new_top: list[asm_ast.Type_top_level] = []
    for tl in prog.top_level:
        if isinstance(tl, asm_ast.Function):
            new_top.append(_rewrite_function(tl))
        else:
            new_top.append(tl)
    return asm_ast.Program(top_level=new_top)


def _rewrite_function(fn: asm_ast.Function) -> asm_ast.Function:
    instrs = fn.instructions
    out: list[asm_ast.Type_instruction] = []
    i = 0
    while i + 2 < len(instrs):
        a, b, c = instrs[i], instrs[i + 1], instrs[i + 2]
        if (isinstance(a, asm_ast.Branch)
                and isinstance(b, asm_ast.Jump)
                and isinstance(c, asm_ast.Label)
                and a.target == c.name
                and type(a.cond) in _INVERT):
            inverted = _INVERT[type(a.cond)]()
            out.append(asm_ast.Branch(cond=inverted, target=b.target))
            out.append(c)
            i += 3
            continue
        out.append(a)
        i += 1
    while i < len(instrs):
        out.append(instrs[i])
        i += 1
    return asm_ast.Function(
        name=fn.name, is_global=fn.is_global,
        params=list(fn.params), instructions=out,
    )
