"""`SEC` and `CLC` where the carry already has that value.

# What it does

A forward dataflow over the function finds, before each instruction,
whether C is known set, known clear, or unknown. `SEC` and `CLC` set
it, `BCS` and `BCC` tell each of their two edges which way it went,
and an instruction that writes C (`ADC`, `SBC`, a compare, a shift or
rotate, a call, and anything else not known to leave C alone) makes
it unknown. Then:

  * A `SEC` where C is known set, or a `CLC` where it's known clear,
    goes, and so does one whose carry nothing reads
    (`asm_liveness.c_dead_at`), as when dead-code elimination has
    removed the add it was for.
  * A `SEC` (or `CLC`) that starts a block, after its labels, moves in
    front of those labels when every path into the block other than
    the one falling into it already has C that way. Only the
    fall-through path, the way into a loop, then runs it.

The second is the shape of `do_rwts_on_sector`'s loop
(`examples/zork/instr_save/disk.c`) once its high byte is in X:

    LDX   SCRATCH1+1
    .loop@0_continue:
    SEC
    SBC   SECTORS_PER_TRACK
    BCS   .thread@0
    CPX   #$01
    DEX
    BCC   .loop@0_break
    .thread@0:
    INC   iob+4
    JMP   .loop@0_continue

Both ways to `.thread@0` have C set, so the `JMP` back does too, and
the `SEC` moves in front of `.loop@0_continue`, as in Zork's code.
That's 2 cycles a pass.

# Where to run

Once, after the third peephole fixed point (`compile.optimized_asm`).
Several peepholes match a SEC or CLC: `transfer_pm1_store` turns
`TXA; CLC; ADC #1; STA M` into `INX; STX M`, and run inside the
fixed point this pass dropped the CLC first, leaving `TXA; ADC #1`.
"""
from __future__ import annotations

import asm_ast
from passes.asm_liveness import c_dead_at

# Instructions that leave C alone. Anything else is taken to change it.
_KEEP_C = (
    asm_ast.Mov, asm_ast.Inc, asm_ast.Dec, asm_ast.And, asm_ast.Or,
    asm_ast.Xor, asm_ast.Push, asm_ast.Pop, asm_ast.BitTest,
    asm_ast.Label, asm_ast.Branch, asm_ast.Jump,
)
_UNSEEN = object()


def apply_carry_known(prog: asm_ast.Program) -> asm_ast.Program:
    new_top: list[asm_ast.Type_top_level] = []
    for tl in prog.top_level:
        if isinstance(tl, asm_ast.Function):
            tl = _rewrite_function(tl)
        new_top.append(tl)
    return asm_ast.Program(top_level=new_top)


def _after(instr, c):
    """C after `instr` falls through, given C before it."""
    if isinstance(instr, asm_ast.SetCarry):
        return 1
    if isinstance(instr, asm_ast.ClearCarry):
        return 0
    if isinstance(instr, _KEEP_C):
        return c
    return None


def _edges(instrs, labels, i, c):
    """`(successor, C on that edge)` for each way out of `instrs[i]`,
    given C before it."""
    ins = instrs[i]
    if isinstance(ins, (asm_ast.Return, asm_ast.Ret)):
        return []
    if isinstance(ins, asm_ast.Jump):
        at = labels.get(ins.target)
        return [] if at is None else [(at, c)]
    if isinstance(ins, asm_ast.Branch):
        taken = fall = c
        if isinstance(ins.cond, asm_ast.CS):
            taken, fall = 1, 0
        elif isinstance(ins.cond, asm_ast.CC):
            taken, fall = 0, 1
        out = [(i + 1, fall)] if i + 1 < len(instrs) else []
        at = labels.get(ins.target)
        if at is not None:
            out.append((at, taken))
        return out
    if i + 1 >= len(instrs):
        return []
    return [(i + 1, _after(ins, c))]


def _carry_states(instrs, labels):
    """C before each instruction (1, 0, or None for unknown), and the
    edges into each instruction with the C each brings."""
    before: list = [_UNSEEN] * len(instrs)
    if not instrs:
        return before, {}
    before[0] = None
    work = [0]
    while work:
        i = work.pop()
        for j, c in _edges(instrs, labels, i, before[i]):
            old = before[j]
            if old is _UNSEEN:
                before[j] = c
                work.append(j)
            elif old is not None and old != c:
                before[j] = None
                work.append(j)
    incoming: dict[int, list[tuple[int, object]]] = {}
    for i in range(len(instrs)):
        if before[i] is _UNSEEN:
            continue
        for j, c in _edges(instrs, labels, i, before[i]):
            incoming.setdefault(j, []).append((i, c))
    return before, incoming


def _setter_value(ins) -> int | None:
    if isinstance(ins, asm_ast.SetCarry):
        return 1
    if isinstance(ins, asm_ast.ClearCarry):
        return 0
    return None


def _labels(instrs) -> dict[str, int]:
    return {
        ins.name: k for k, ins in enumerate(instrs)
        if isinstance(ins, asm_ast.Label)
    }


def _rewrite_function(fn: asm_ast.Function) -> asm_ast.Function:
    """Three steps, each on the result of the one before, since each
    changes what the next sees: dropping the setters C already
    matches (which changes no value of C), moving setters in front of
    loops, and dropping the setters nothing reads. Done in one step,
    `CLC; LDA x; CLC; ADC y` lost both: the first as dead, since the
    second overwrites C, and the second as redundant after the
    first."""
    instrs = list(fn.instructions)
    instrs = _drop_known(instrs)
    instrs = _move_into_loops(instrs)
    instrs = [
        ins for i, ins in enumerate(instrs)
        if _setter_value(ins) is None or not c_dead_at(instrs, i + 1)
    ]
    if instrs == fn.instructions:
        return fn
    return asm_ast.Function(
        name=fn.name, is_global=fn.is_global,
        params=list(fn.params), instructions=instrs,
    )


def _drop_known(instrs):
    """Without the SEC / CLC where C is known to have that value."""
    before, _ = _carry_states(instrs, _labels(instrs))
    return [
        ins for i, ins in enumerate(instrs)
        if _setter_value(ins) is None or before[i] is _UNSEEN
        or before[i] != _setter_value(ins)
    ]


def _move_into_loops(instrs):
    """Each SEC / CLC that starts a block moved in front of its labels
    when every edge in but the fall-through already has C that way."""
    before, incoming = _carry_states(instrs, _labels(instrs))
    drop: set[int] = set()
    move: dict[int, int] = {}   # where a setter goes -> the setter
    for i, ins in enumerate(instrs):
        want = _setter_value(ins)
        if want is None or before[i] is _UNSEEN:
            continue
        p = i
        while p > 0 and isinstance(instrs[p - 1], asm_ast.Label):
            p -= 1
        if p == i or p == 0:
            continue
        prev = instrs[p - 1]
        if isinstance(prev, (asm_ast.Jump, asm_ast.Return, asm_ast.Ret)):
            continue
        # The edges into the run of labels and the setter, less those
        # from one label to the next.
        edges = [
            e for k in range(p, i + 1) for e in incoming.get(k, ())
            if not p <= e[0] < i
        ]
        fall = [c for src, c in edges if src == p - 1]
        others = [c for src, c in edges if src != p - 1]
        if len(fall) == 1 and others and all(c == want for c in others):
            drop.add(i)
            move[p] = i
    if not drop:
        return instrs
    out: list[asm_ast.Type_instruction] = []
    for k, ins in enumerate(instrs):
        if k in move:
            out.append(instrs[move[k]])
        if k not in drop:
            out.append(ins)
    return out
