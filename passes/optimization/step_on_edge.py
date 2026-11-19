"""Moves a step whose result is only copied on one edge onto that edge.

# What it does

After SSA destruction, a post-decrement in a loop's condition, as in
`do g(i); while (i-- != first);`, leaves

    Binary(Subtract, i, 1, %t)
    JumpIfCmp(NotEqual, i, first, .split)
    Ret(None)
  .split:
    Copy(%t, i)
    Jump(.loop)

The step is computed before the test, which reads the old `i`, and
its result reaches `i` only on the edge into `.split`. Both values are
live at the test, so `%t` takes a byte of its own, and the code is
`LDA i; SEC; SBC #1; STA t; LDA i; CMP first; BNE; ...; LDA t; STA i`.
This pass drops the `Binary` and puts the step in place of the
`Copy`:

    JumpIfCmp(NotEqual, i, first, .split)
    Ret(None)
  .split:
    Binary(Subtract, i, 1, i)
    Jump(.loop)

The step is then in place, which the DEC peephole turns into `DEC i`.
A step whose `Copy` is on the fall-through path moves the same way.

# When it applies

- The step is `Binary(Add | Subtract | the bitwise operators, X, c,
  T)` with a constant `c`, where `X` and `T` are SSA-renamed names,
  so no store through a pointer and no call can write either one.
- `T` is written by the step alone and read by one `Copy(T, Z)` alone.
- From the step to the end of its block, no instruction writes `X`,
  and no label comes between. The block ends at a jump of any kind.
- The `Copy` is in the block the jump goes to, or in the block the
  jump falls through to, and only instructions that don't write `X`
  and aren't labels or jumps come before it there. A block reached
  through a label must have that one jump as its only predecessor: no
  other jump names the label, and the instruction before the label
  is an unconditional jump or a return, so nothing falls into it.

On every path from the step to the `Copy`, `X` then holds the value
the step read, so `Binary(op, X, c, Z)` at the `Copy` writes `Z` the
value the `Copy` wrote. `T` is read nowhere else, so dropping it
changes no other value, and every other path no longer computes a
value that nothing read. The step has no side effects, since the
operators it matches can't trap.
"""

from __future__ import annotations

from collections import Counter

import tac_ast
from passes.optimization.framework import PassContext
from passes.optimization.framework.phases import PostDestructionPass
from passes.optimization.var_visit import defs_in, uses_in


_STEP_OPS = (
    tac_ast.Add, tac_ast.Subtract, tac_ast.BitwiseAnd, tac_ast.BitwiseOr,
    tac_ast.BitwiseXor,
)

_CONDITIONAL_JUMPS = (
    tac_ast.JumpIfTrue, tac_ast.JumpIfFalse, tac_ast.JumpIfCmp,
    tac_ast.JumpIfMasked,
)

_ENDS_FLOW = (tac_ast.Jump, tac_ast.Ret)


def _writes(instr, name: str) -> bool:
    return any(d.name == name for d in defs_in(instr))


def _copy_before_write(instrs, start: int, x: str, t: str) -> int | None:
    """Returns the index of `Copy(t, _)` at or after `start`, when
    only instructions that neither write `x` nor end the block come
    before it, and None otherwise."""
    k = start
    while k < len(instrs):
        ins = instrs[k]
        if (isinstance(ins, tac_ast.Copy) and isinstance(ins.src, tac_ast.Var)
                and ins.src.name == t):
            return k
        if isinstance(ins, (tac_ast.Label, tac_ast.Jump, tac_ast.Ret,
                            *_CONDITIONAL_JUMPS)):
            return None
        if _writes(ins, x):
            return None
        k += 1
    return None


def _jump_targets(ins) -> list[str]:
    target = getattr(ins, "target", None)
    return [target] if isinstance(target, str) else []


def move_steps_onto_edges(
    fn: tac_ast.Function, ssa_names: set[str],
) -> tac_ast.Function:
    """Applies the rewrite in the module docstring to every step in
    `fn` that meets its conditions. `ssa_names` holds the names that
    SSA construction renamed."""
    instrs = list(fn.instructions)
    uses: Counter = Counter()
    defs: Counter = Counter()
    for ins in instrs:
        for u in uses_in(ins):
            uses[u.name] += 1
        for d in defs_in(ins):
            defs[d.name] += 1
    label_refs: Counter = Counter()
    label_at: dict[str, int] = {}
    for k, ins in enumerate(instrs):
        if isinstance(ins, tac_ast.Label):
            label_at[ins.name] = k
        for target in _jump_targets(ins):
            label_refs[target] += 1
    changed = False
    i = 0
    while i < len(instrs):
        step = instrs[i]
        if not (isinstance(step, tac_ast.Binary)
                and isinstance(step.op, _STEP_OPS)
                and isinstance(step.src1, tac_ast.Var)
                and isinstance(step.src2, tac_ast.Constant)
                and isinstance(step.dst, tac_ast.Var)):
            i += 1
            continue
        x, t = step.src1.name, step.dst.name
        if (x == t or x not in ssa_names or t not in ssa_names
                or defs[t] != 1 or uses[t] != 1):
            i += 1
            continue
        j = i + 1
        while j < len(instrs) and not isinstance(
                instrs[j], (tac_ast.Label, tac_ast.Jump, tac_ast.Ret,
                            *_CONDITIONAL_JUMPS)):
            if _writes(instrs[j], x):
                break
            j += 1
        if (j >= len(instrs)
                or isinstance(instrs[j], (tac_ast.Label, tac_ast.Ret))
                or _writes(instrs[j], x)
                or not isinstance(instrs[j], (tac_ast.Jump,
                                              *_CONDITIONAL_JUMPS))):
            i += 1
            continue
        jump = instrs[j]
        found = None
        if isinstance(jump, _CONDITIONAL_JUMPS):
            found = _copy_before_write(instrs, j + 1, x, t)
        if found is None:
            target = jump.target
            at = label_at.get(target)
            if (at is not None and label_refs[target] == 1 and at > 0
                    and isinstance(instrs[at - 1], _ENDS_FLOW)):
                found = _copy_before_write(instrs, at + 1, x, t)
        if found is None:
            i += 1
            continue
        copy = instrs[found]
        instrs[found] = tac_ast.Binary(
            op=step.op, src1=step.src1, src2=step.src2, dst=copy.dst,
        )
        del instrs[i]
        changed = True
    if not changed:
        return fn
    return tac_ast.Function(
        name=fn.name, is_global=fn.is_global,
        params=list(fn.params), instructions=instrs,
    )


class MoveStepsOntoEdges(PostDestructionPass):
    name = "move_steps_onto_edges"

    def run(self, fn, ctx: PassContext):
        if ctx.ssa_dsts is None:
            return fn
        return move_steps_onto_edges(fn, ctx.ssa_dsts)
