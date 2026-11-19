"""TAC pass: send an edge into a join straight to where the join's test
sends the constant that the edge brings.

# The shape

An inlined function that returns a constant on some paths, such as

    static bool find(uint8_t v) {
        ...
        do { if (a[i] == v) { hits++; return true; } } while (--i != 0);
        return false;
    }
    void f(uint8_t v) { if (find(v)) return; hits += 2; }

leaves its result in a `Phi` at the copy's end label, and the caller's
test of the result right after it:

    .f@ssa_block@1:
        hits = hits + 1
        Jump(.i0@end)
    ...
        JumpIfTrue(%23, .i0_loop@0_start)
    .i0_loop@0_break:
    .i0@end:
        %11.3 = Phi(1 from .f@ssa_block@1, 0 from .i0_loop@0_break)
        JumpIfFalse(%11.3, .if_end@1)
        Ret

Each edge into `.i0@end` brings a constant, so the test is decided on
each of them. The pass sends the first edge to the `Ret` (under a new
label) and the second to `.if_end@1`, and `.i0@end` is left with no
way in, which unreachable-code elimination then removes. The 6502 code
had loaded the constant and branched on it, `LDA #$00; BNE`, on every
such path. `&&` and `||` whose result is tested leave the same shape,
with a 0 or a 1 from each edge.

# The rule

A block B made of its label, one `Phi` with an SSA destination `t`,
and a `JumpIfTrue` or `JumpIfFalse` on `t`, where nothing else reads
`t`, is a candidate. For each argument of the `Phi` that is an integer
constant, from a predecessor P with one edge into B, the constant
decides the jump, and the edge goes to the successor S it picks
(`JumpIfTrue` takes its target when the constant isn't 0). P's jump to
B goes to S instead. When P falls into B without a jump of its own, a
`Jump` to S goes at P's end, and when P ends with a conditional jump to
somewhere else, the `Jump` goes in a new block after it, under a new
label, so that it can be named as a predecessor in S's `Phi`s. S gets
a label when it starts without one, which can only happen when B falls
through to it. The argument leaves B's `Phi`, and each `Phi` in S that
has an argument from B gets the same argument from P (or from the new
block).

When P holds nothing but its label, or its label and a `Jump` to B,
and every way into P is a jump to its label, the jumps into P go to S
instead, and P is left with no way in. `&&` and `||` leave such a P
for the edges their tests short-circuit (`.and_false@N`, `.or_true@N`),
and a `Jump` to S at P's end would only be a second jump on the way.
Each block that jumps into P must then meet the conditions below in
P's place, and start with a label when S's `Phi`s name B, so that they
can name it instead. Otherwise P's own edge goes to S as above.

A block that an earlier step left with no way in still has its edges
in the graph until unreachable-code elimination drops it, so the pass
doesn't count them.

An edge is left alone when

  - S is B itself, or B's two successors are the same block, or S is
    the exit (B is the last block and falls off the end);
  - P has two edges into B, or P is B;
  - P already has an edge to S, since S's `Phi`s would then need two
    arguments from P;
  - S is inside a natural loop, other than as its header, that P is
    outside of. The new edge would enter that loop's body without
    passing its header, and the loop would have two entries.

# Why it's sound

The jump in B reads only `t`, and on the edge from P, `t` is the
constant, so control leaves B for S on that edge whatever else holds.
B computes nothing other than `t`, which nothing else reads, so
skipping B skips no effect and no value that a later instruction
reads. An empty P whose jumps go to S skips nothing either. A `Phi`
in S that took a value `v` from B takes the same `v` from the block
whose edge moved. `v` isn't `t`, which only the jump reads, so it's
defined outside B (and outside an empty P), in a block D that
dominates B. A path from the entry to that block, followed by its edge
into B, is a path to B, which passes through D before B, so D
dominates the block too (or is the block), and `v` is defined on every
path to the block's end.

A loop that holds the edge's new source and not S (an exit) or that
holds both is unchanged in its entries. A loop with S as its header
gains an edge into the header. Any new cycle through a moved edge
Q → S was already a cycle Q → (P →) B → S → ... → Q, whose header
dominated each of its blocks, so the new edge doesn't make the graph
irreducible.

# Where to run

In the fixed point, right before unreachable-code elimination, which
drops B once its last edge is gone (and with it B's `Phi`, left with
no arguments) and drops the stale arguments from B in S's `Phi`s.
"""
from __future__ import annotations

import dataclasses

import tac_ast
from passes.optimization.cfg import (
    ENTRY_ID, EXIT_ID, build_cfg, cfg_to_function, natural_loops,
)
from passes.optimization.framework import FixedpointPass, PassContext
from passes.optimization.var_visit import count_uses

_CONDITIONAL = (
    tac_ast.JumpIfTrue, tac_ast.JumpIfFalse,
    tac_ast.JumpIfCmp, tac_ast.JumpIfMasked,
)
_JUMPS = (tac_ast.Jump, *_CONDITIONAL)


def thread_constant_jumps(
    fn: tac_ast.Function, ctx: PassContext,
) -> tac_ast.Function:
    """Returns `fn` with every edge that brings a join a constant that
    decides the join's test sent to the successor the test picks (see
    the module docstring), or `fn` itself when no edge qualifies."""
    if ctx.ssa_dsts is None:
        return fn
    labels = {i.name for i in fn.instructions if isinstance(i, tac_ast.Label)}
    counter = 0

    def fresh_label() -> str:
        nonlocal counter
        while True:
            name = f".{fn.name}@jthread@{counter}"
            counter += 1
            if name not in labels:
                labels.add(name)
                return name

    # One join at a time, since threading one changes the blocks the
    # next one sees.
    while True:
        new_fn = _thread_one_join(fn, ctx.ssa_dsts, fresh_label)
        if new_fn is None:
            return fn
        fn = new_fn


def _leading_label(block) -> str | None:
    if block.instructions and isinstance(block.instructions[0], tac_ast.Label):
        return block.instructions[0].name
    return None


def _constant_value(v) -> int | None:
    from passes.optimization.constant_folding import _is_integer_const
    if isinstance(v, tac_ast.Constant) and _is_integer_const(v.const):
        return v.const.value
    return None


@dataclasses.dataclass
class _Thread:
    """One edge to send to `target`: the `Phi` argument `arg` it brings,
    and the blocks whose jumps change, with the label each jumps to
    now. `sources` is `[(P, join label)]` for the predecessor P itself,
    or `[(Q, P's label), ...]` for the blocks that jump into an empty
    P."""
    arg: tac_ast.PhiArg
    sources: list[tuple[int, str]]
    target: int


def _thread_one_join(fn, ssa_dsts, fresh_label):
    """Threads the constant edges into the first join that has one,
    and returns the new function, or None when no join has one."""
    cfg = build_cfg(fn)
    uses = count_uses(fn.instructions)
    label_block = {
        _leading_label(b): bid for bid, b in cfg.blocks.items()
        if _leading_label(b) is not None
    }
    loops = None

    def enters_loop_body(source: int, target: int) -> bool:
        nonlocal loops
        if loops is None:
            loops = natural_loops(cfg)
        return any(target in body and target != header and source not in body
                   for header, body in loops)

    # A block that an earlier threading left with no way in still has
    # its edges in the graph, until UCE drops it. They aren't edges.
    reachable = _reachable(cfg)
    for b in cfg.blocks.values():
        b.predecessors = [p for p in b.predecessors if p in reachable]
    for bid in cfg.block_order:
        block = cfg.blocks[bid]
        ins = block.instructions
        if len(ins) != 3 or bid not in reachable:
            continue
        label, phi, jump = ins
        if not (isinstance(label, tac_ast.Label)
                and isinstance(phi, tac_ast.Phi)
                and isinstance(jump, (tac_ast.JumpIfTrue, tac_ast.JumpIfFalse))
                and isinstance(phi.dst, tac_ast.Var)
                and jump.condition == phi.dst
                and phi.dst.name in ssa_dsts
                and uses.get(phi.dst.name, 0) == 1):
            continue
        taken, fall = block.successors
        if taken == fall:
            continue
        threads = []
        for arg in phi.args:
            value = _constant_value(arg.source)
            pred = label_block.get(arg.pred_label)
            if value is None or pred is None or pred == bid:
                continue
            if block.predecessors.count(pred) != 1:
                continue
            target = taken if (value != 0) == isinstance(
                jump, tac_ast.JumpIfTrue) else fall
            if target in (bid, EXIT_ID):
                continue
            named = any(
                isinstance(i, tac_ast.Phi)
                and any(a.pred_label == label.name for a in i.args)
                for i in cfg.blocks[target].instructions
            )
            through = _jumps_through(
                cfg, pred, label.name, target, named, enters_loop_body,
            )
            if through is not None:
                threads.append(_Thread(arg, through, target))
                continue
            if target in cfg.blocks[pred].successors:
                continue
            if enters_loop_body(pred, target):
                continue
            threads.append(_Thread(arg, [(pred, label.name)], target))
        if not threads:
            continue
        _apply(cfg, bid, threads, fresh_label)
        return cfg_to_function(fn, cfg)
    return None


def _reachable(cfg) -> set[int]:
    seen = {ENTRY_ID}
    stack = [ENTRY_ID]
    while stack:
        for s in cfg.blocks[stack.pop()].successors:
            if s not in seen:
                seen.add(s)
                stack.append(s)
    return seen


def _jumps_through(cfg, pred, join_label, target, named, enters_loop_body):
    """Returns the blocks, each with `pred`'s label, whose jumps into
    `pred` can go straight to `target`, when `pred` holds its label and
    nothing else but a `Jump` to the join, and is reached only by
    jumps, or None. Each of them must jump into `pred` once, have no
    edge to `target` already, not enter a loop's body by the new edge,
    and start with a label when `target`'s `Phi`s name the join
    (`named`), so that they can name it too."""
    block = cfg.blocks[pred]
    rest = block.instructions[1:]
    if rest != [] and rest != [tac_ast.Jump(target=join_label)]:
        return None
    if not block.predecessors:
        return None
    name = block.instructions[0].name
    sources = []
    for q in block.predecessors:
        q_block = cfg.blocks.get(q)
        if q == pred or q_block is None or not q_block.instructions:
            return None
        last = q_block.instructions[-1]
        if not (isinstance(last, _JUMPS)
                and last.target == name
                and q_block.successors.count(pred) == 1
                and target not in q_block.successors
                and not enters_loop_body(q, target)
                and (not named or _leading_label(q_block) is not None)):
            return None
        sources.append((q, name))
    return sources


def _apply(cfg, bid, threads, fresh_label) -> None:
    """Sends each edge of `threads` to its target, with the bookkeeping
    the module docstring describes. Builds new instructions and `Phi`s
    rather than changing the ones the function holds, since the driver
    compares the result with its input."""
    join = cfg.blocks[bid]
    join_label = join.instructions[0].name
    threaded = {t.arg.pred_label for t in threads}
    phi = join.instructions[1]
    join.instructions[1] = tac_ast.Phi(
        dst=phi.dst,
        args=[a for a in phi.args if a.pred_label not in threaded],
    )
    for t in threads:
        target_block = cfg.blocks[t.target]
        target_label = _leading_label(target_block)
        if target_label is None:
            target_label = fresh_label()
            target_block.instructions = [
                tac_ast.Label(name=target_label), *target_block.instructions,
            ]
        from_labels = [
            _redirect(cfg.blocks[q], into, target_label, fresh_label)
            for q, into in t.sources
        ]
        target_block.instructions = [
            _with_args_from(i, join_label, from_labels)
            if isinstance(i, tac_ast.Phi) else i
            for i in target_block.instructions
        ]


def _redirect(block, into: str, target: str, fresh_label) -> str | None:
    """Sends `block`'s edge into the block labeled `into` to the block
    labeled `target`, and returns the label that names `block` (or the
    new block holding its jump) as a predecessor of `target`."""
    last = block.instructions[-1]
    if isinstance(last, _JUMPS) and last.target == into:
        block.instructions = [
            *block.instructions[:-1], dataclasses.replace(last, target=target),
        ]
        return _leading_label(block)
    if isinstance(last, _CONDITIONAL):
        # The edge is the fall-through after a conditional jump, so the
        # jump to `target` goes in a block of its own.
        new = fresh_label()
        block.instructions = [
            *block.instructions,
            tac_ast.Label(name=new), tac_ast.Jump(target=target),
        ]
        return new
    block.instructions = [*block.instructions, tac_ast.Jump(target=target)]
    return _leading_label(block)


def _with_args_from(
    phi: tac_ast.Phi, old: str, new: list[str | None],
) -> tac_ast.Phi:
    """Returns `phi` with a copy of its argument from `old`, if it has
    one, coming from each label of `new` as well."""
    extra = [tac_ast.PhiArg(pred_label=n, source=a.source)
             for a in phi.args if a.pred_label == old for n in new]
    if not extra:
        return phi
    return tac_ast.Phi(dst=phi.dst, args=[*phi.args, *extra])


class ThreadConstantJumps(FixedpointPass):
    name = "thread_constant_jumps"

    def run(self, fn, ctx):
        return thread_constant_jumps(fn, ctx)
