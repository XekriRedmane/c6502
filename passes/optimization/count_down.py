"""Counts a loop down to 0 when its counter is used only to count.

# What it does

`for (uint8_t i = 0; i < 20; i++) out(0xA0);` counts `i` up and
compares it with 20 on each pass, `INC i; LDA i; CMP #$14; BCC`. Its
counter is read only by its own test and its own step, so the loop
can count the passes left instead, from 20 down to 0, and test that
for 0, which is `DEC i; BNE`, 3 bytes and 2 cycles a pass fewer.

In SSA form the loop is

    Phi(i.2, [(entry, 0uc), (continue, i.3)])
    JumpIfCmp(GreaterOrEqual, i.2, 20uc, .break)
    ...
    Binary(Add, i.2, 1uc, i.3)

and the pass rewrites it, keeping the names, as

    Phi(i.2, [(entry, 20uc), (continue, i.3)])
    JumpIfCmp(Equal, i.2, 0uc, .break)
    ...
    Binary(Subtract, i.2, 1uc, i.3)

so that `i.2` holds `20 - i` where it held `i`.

# When it applies

- `i.2` is a `Phi` of an unsigned byte with two arguments, a constant
  `c0` and a variable `i.3`, and `Binary(Add, i.2, 1, i.3)` is the
  only definition of `i.3`, which only the `Phi` reads.
- `i.2` is read by that `Add` and by one `JumpIfCmp`, the first
  instruction after the `Phi`s of their block `H`, and by nothing else.
- The jump compares `i.2` with a bound `n`, by `>=` or `==` to leave
  the loop when it's taken or by `<` or `!=` to stay in it. The bound
  may be on the left with the comparison mirrored.
- The bound is a constant `c1` with `c0 <= c1`, or `c0` is 0 and the
  bound is a variable that SSA construction renamed (or a parameter)
  defined in a block that strictly dominates `H`, so the loop doesn't
  change it.
- The block the loop stays in after the test, `S`, has `H` as its only
  predecessor, and `S` dominates the block of the `Add`.

Each pass through the `Add` then comes from `H` with the test showing
`i < n`, so `i` goes from `c0` up to at most `n` and never wraps, and
`i >= n`, `i == n`, `n - i == 0` all say the same thing. `n - i`
starts at `c1 - c0` (or the variable's value) and drops by 1 at each
step, so the test of it for 0 takes the same branch as the test of
`i` on every pass.
"""

from __future__ import annotations

from collections import Counter

import c99_ast
import tac_ast
from passes.optimization.cfg import build_cfg, immediate_dominators
from passes.optimization.framework import PassContext, PostFixedpointPass
from passes.optimization.var_visit import defs_in, uses_in


# The comparison with the counter on the left, and whether a taken
# jump leaves the loop.
_LEAVES = {
    tac_ast.GreaterOrEqual: True, tac_ast.Equal: True,
    tac_ast.LessThan: False, tac_ast.NotEqual: False,
}
# The comparison with the counter on the right, written with it on the
# left.
_MIRROR = {
    tac_ast.LessOrEqual: tac_ast.GreaterOrEqual,
    tac_ast.Equal: tac_ast.Equal,
    tac_ast.GreaterThan: tac_ast.LessThan,
    tac_ast.NotEqual: tac_ast.NotEqual,
}

_JUMPS = (
    tac_ast.Jump, tac_ast.JumpIfTrue, tac_ast.JumpIfFalse,
    tac_ast.JumpIfCmp, tac_ast.JumpIfMasked,
)


def _unsigned_byte(name: str, symbols) -> bool:
    sym = symbols.get(name)
    t = getattr(sym, "type", None)
    return isinstance(t, (c99_ast.UChar, c99_ast.Char))


def _const_value(v) -> int | None:
    if isinstance(v, tac_ast.Constant):
        value = getattr(v.const, "value", None)
        if isinstance(value, int):
            return value
    return None


def count_loops_down(fn: tac_ast.Function, ctx: PassContext):
    """Applies the rewrite in the module docstring to every loop of
    `fn` that meets its conditions."""
    if ctx.symbols is None or ctx.ssa_dsts is None:
        return fn
    instrs = list(fn.instructions)
    cfg = build_cfg(fn)
    idom = immediate_dominators(cfg)
    block_of: list[int] = []
    for bid in cfg.block_order:
        block_of.extend([bid] * len(cfg.blocks[bid].instructions))
    uses: Counter = Counter()
    def_at: dict[str, int] = {}
    for k, ins in enumerate(instrs):
        for u in uses_in(ins):
            uses[u.name] += 1
        for d in defs_in(ins):
            def_at[d.name] = k
    label_block = {
        cfg.blocks[b].instructions[0].name: b for b in cfg.block_order
        if isinstance(cfg.blocks[b].instructions[0], tac_ast.Label)
    }

    def dominates(a: int, b: int) -> bool:
        while b in idom:
            if a == b:
                return True
            if idom[b] == b:
                return False
            b = idom[b]
        return False

    changed = False
    for ip, phi in enumerate(instrs):
        if not (isinstance(phi, tac_ast.Phi) and len(phi.args) == 2
                and isinstance(phi.dst, tac_ast.Var)):
            continue
        p = phi.dst.name
        if (p not in ctx.ssa_dsts or not _unsigned_byte(p, ctx.symbols)
                or uses[p] != 2):
            continue
        consts = [a for a in phi.args if _const_value(a.source) is not None]
        steps = [a for a in phi.args if isinstance(a.source, tac_ast.Var)]
        if len(consts) != 1 or len(steps) != 1:
            continue
        c0 = _const_value(consts[0].source)
        s = steps[0].source.name
        if s not in def_at or uses[s] != 1:
            continue
        ia = def_at[s]
        add = instrs[ia]
        if not (isinstance(add, tac_ast.Binary)
                and isinstance(add.op, tac_ast.Add)
                and _const_value(add.src2) == 1
                and add.src1 == phi.dst):
            continue
        h = block_of[ip]
        if h not in idom:
            continue
        ij = ip + 1
        while ij < len(instrs) and isinstance(instrs[ij], tac_ast.Phi):
            ij += 1
        if ij >= len(instrs) or block_of[ij] != h:
            continue
        jump = instrs[ij]
        if not isinstance(jump, tac_ast.JumpIfCmp):
            continue
        if jump.src1 == phi.dst and type(jump.op) in _LEAVES:
            op, bound = type(jump.op), jump.src2
        elif jump.src2 == phi.dst and type(jump.op) in _MIRROR:
            op, bound = _MIRROR[type(jump.op)], jump.src1
        else:
            continue
        c1 = _const_value(bound)
        if c1 is not None:
            if not 0 <= c0 <= c1 <= 0xFF:
                continue
            start = tac_ast.Constant(
                const=type(consts[0].source.const)(value=c1 - c0))
        else:
            if c0 != 0 or not isinstance(bound, tac_ast.Var):
                continue
            k = bound.name
            if k in fn.params:
                pass
            elif k in ctx.ssa_dsts and k in def_at:
                kb = block_of[def_at[k]]
                if kb == h or not dominates(kb, h):
                    continue
            else:
                continue
            if not _unsigned_byte(k, ctx.symbols):
                continue
            start = bound
        leaves = _LEAVES[op]
        # The block the loop stays in after the test.
        succs = cfg.blocks[h].successors
        taken = label_block.get(jump.target)
        if taken is None or len(succs) != 2:
            continue
        fall = next((b for b in succs if b != taken), None)
        stay = fall if leaves else taken
        if (stay is None or stay == h
                or cfg.blocks[stay].predecessors != [h]
                or not dominates(stay, block_of[ia])):
            continue
        zero = tac_ast.Constant(const=type(consts[0].source.const)(value=0))
        instrs[ip] = tac_ast.Phi(dst=phi.dst, args=[
            tac_ast.PhiArg(pred_label=a.pred_label,
                           source=start if a is consts[0] else a.source)
            for a in phi.args
        ])
        instrs[ij] = tac_ast.JumpIfCmp(
            op=tac_ast.Equal() if leaves else tac_ast.NotEqual(),
            src1=phi.dst, src2=zero, target=jump.target,
        )
        instrs[ia] = tac_ast.Binary(
            op=tac_ast.Subtract(), src1=add.src1, src2=add.src2,
            dst=add.dst,
        )
        changed = True
    if not changed:
        return fn
    return tac_ast.Function(
        name=fn.name, is_global=fn.is_global,
        params=list(fn.params), instructions=instrs,
    )


class CountLoopsDown(PostFixedpointPass):
    name = "count_loops_down"

    def run(self, fn, ctx):
        return count_loops_down(fn, ctx)
