"""Branch threading on a known flag: a branch to a branch on the same
flag goes where that one would.

# Pattern

    Branch(c1, L)
    ...
    L:
    Branch(c2, M)        # c2 tests the flag c1 tested
    next:

A path that takes the first branch arrives at L with that flag known:
set for `BCS`, clear for `BCC`, and so on. If `c2` is `c1`, the second
branch is taken too, so the first can go straight to M. If `c2` is
`c1`'s opposite, it isn't, so the first can go to `next`, the
instruction after it (a label is added there when there isn't one).
Labels between L and the second branch don't change the flags.

That saves a branch, 2 cycles, on that path. `carry_inc` leaves the
shape in `do_rwts_on_sector` (`examples/zork/instr_save/disk.c`):

    BCS   .carry_inc@0       ; no borrow out of the low byte
    CPX   #$01
    DEX
    .carry_inc@0:
    BCC   .loop@0_break      ; never taken from the BCS
    INC   iob+4

becomes `BCS .thread@0` with `.thread@0:` in front of the `INC`, as
Zork's own `BCS .inc_track` does.

# Where to run

In the asm peephole fixed-point loop, after `branch_through_jump`.
The fall-through path still reaches L, so L stays; `dead_label_drop`
removes it if nothing else uses it.
"""
from __future__ import annotations

import re

import asm_ast

# Each condition, its opposite, and the flag both test.
_OPPOSITE: dict[type, type] = {
    asm_ast.CC: asm_ast.CS, asm_ast.CS: asm_ast.CC,
    asm_ast.EQ: asm_ast.NE, asm_ast.NE: asm_ast.EQ,
    asm_ast.MI: asm_ast.PL, asm_ast.PL: asm_ast.MI,
    asm_ast.VC: asm_ast.VS, asm_ast.VS: asm_ast.VC,
}
_LABEL_RE = re.compile(r"\.thread@(\d+)$")


def apply_branch_thread(prog: asm_ast.Program) -> asm_ast.Program:
    """Retargets every branch whose target is a branch on the same
    flag, in every function. Labels are numbered across the program,
    since the sim's assembler rejects one name in two functions."""
    counter = 1 + max(
        (int(m.group(1))
         for tl in prog.top_level if isinstance(tl, asm_ast.Function)
         for ins in tl.instructions
         if isinstance(ins, asm_ast.Label)
         and (m := _LABEL_RE.match(ins.name))),
        default=-1,
    )
    new_top: list[asm_ast.Type_top_level] = []
    for tl in prog.top_level:
        if isinstance(tl, asm_ast.Function):
            tl, counter = _rewrite_function(tl, counter)
        new_top.append(tl)
    return asm_ast.Program(top_level=new_top)


def _rewrite_function(
    fn: asm_ast.Function, counter: int,
) -> tuple[asm_ast.Function, int]:
    instrs = fn.instructions
    labels = {
        ins.name: k for k, ins in enumerate(instrs)
        if isinstance(ins, asm_ast.Label)
    }
    # New target of each branch that changes, and the labels to add
    # after the second branches that are passed over.
    retarget: dict[int, str] = {}
    added: dict[int, str] = {}
    for k, ins in enumerate(instrs):
        if not (isinstance(ins, asm_ast.Branch)
                and type(ins.cond) in _OPPOSITE):
            continue
        at = labels.get(ins.target)
        if at is None:
            continue
        j = at
        while j < len(instrs) and isinstance(instrs[j], asm_ast.Label):
            j += 1
        if j >= len(instrs) or j == k:
            continue
        second = instrs[j]
        if not isinstance(second, asm_ast.Branch):
            continue
        if type(second.cond) is type(ins.cond):
            if second.target != ins.target:
                retarget[k] = second.target
        elif type(second.cond) is _OPPOSITE[type(ins.cond)]:
            if j + 1 < len(instrs) and isinstance(
                instrs[j + 1], asm_ast.Label,
            ):
                retarget[k] = instrs[j + 1].name
            else:
                if j not in added:
                    added[j] = f".thread@{counter}"
                    counter += 1
                retarget[k] = added[j]
    if not retarget:
        return fn, counter
    out: list[asm_ast.Type_instruction] = []
    for k, ins in enumerate(instrs):
        if k in retarget:
            ins = asm_ast.Branch(cond=ins.cond, target=retarget[k])
        out.append(ins)
        if k in added:
            out.append(asm_ast.Label(name=added[k]))
    return asm_ast.Function(
        name=fn.name, is_global=fn.is_global,
        params=list(fn.params), instructions=out,
    ), counter
