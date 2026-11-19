"""TAC pass: a jump whose target starts with a test that the jump's own
path has already decided goes past the test.

# What it does

`if (t == 20 || t == 10) f(t == 20 ? a : b);` reaches this pass, after
the short-circuit fold, as

    JumpIfCmp(Equal, t, 20, .scfold@0)
    JumpIfCmp(Equal, t, 10, .scfold@0)
    Jump(.if_end@2)
  .scfold@0:
    JumpIfCmp(NotEqual, t, 20, .cond_else@3)
  .ssa_block@4:
    Copy(a, %r)
    ...
  .cond_else@3:
    Copy(b, %r)

Each jump to `.scfold@0` has decided the test there. On the first, t
is 20, so the `!= 20` jump isn't taken and the jump can go to
`.ssa_block@4`. On the second, t is 10, so the `!= 20` jump is taken
and the jump can go to `.cond_else@3`. Nothing else reaches
`.scfold@0` after that, and the pass drops its test. Lode Runner's `handle_timers` (examples/lode_runner/timers.c)
has this shape where it picks the brick sprite to draw.

A conditional jump that the straight-line code before it has decided
becomes a `Jump` when it's always taken, and goes when it never is.

# How

A scan through the function keeps, for each Var, the integer
constants it's known to equal or to differ from. A conditional jump
on `x == k` or `x != k` (a `JumpIfCmp` with an integer constant on
either side, or a `JumpIfFalse` or `JumpIfTrue`, which compare with 0)
gives the fact `x == k` on one of its edges and `x != k` on the other.
A fact is dropped when something writes x. A `Label` drops them all,
since another jump may reach it, and so does any instruction other
than the conditional jumps, `Copy`, `Unary`, `Binary`, the casts,
`GetAddress`, and the loads, which write their `dst` and nothing else.
A `Jump` ends the straight-line code.

From each jump's target, with the facts the scan holds on that edge, the
pass follows the labels and the equality tests it can decide, taking
the decided edge each time and adding its fact, for up to `_STEPS`
tests. The jump goes to the furthest point reached after at least one
test, where a label is added when there's none. The tests skipped
read only Vars and constants and write nothing, so skipping them
changes nothing else.

The walk also passes a `Copy` into a local whose address isn't taken,
from a constant or a non-volatile Var, as SSA destruction leaves for
`r` in `r = 0; if (...) r = t == 20 ? a : b;`. It drops the facts about
the local, and a point counts only when every local it passed this
way is dead there (`_liveness`), so the copy's value is never read on
the new edge.

After threading, a run of labels that no jump names, after a `Jump`
or `Ret`, goes with the code up to the next label, since nothing
reaches it. That's the test every jump went past.

# Conditions

  * The Var is an integer of 1, 2, 4, or 8 bytes, and isn't volatile,
    since a volatile Var may change between two reads and each read
    has to happen.
  * The constant's class has the Var's width. Values are compared
    modulo 2 to the width, so a signed and an unsigned spelling of one
    bit pattern agree.
  * Only `Equal` and `NotEqual` are decided. An ordering compare
    neither gives a fact nor is decided.

# Placing the target

Threading leaves the conditional jump's target one block further on
than the code that the jump falls into:

    JumpIfCmp(Equal, t, 10, .cond_else@3)
    Jump(.if_end@2)
  .ssa_block@4:
    Copy(a, %r)
    Jump(.cond_end@4)
  .cond_else@3:
    Copy(b, %r)
  .cond_end@4:

The pass swaps the two blocks, so that `.cond_else@3` comes right
after the `Jump`, ends with `Jump(.cond_end@4)`, and `.ssa_block@4`
falls into `.cond_end@4`. The asm `branch_invert` then makes the
conditional jump and the `Jump` one branch, `BNE .if_end@2`, and the
two arms keep one jump between them. The swap is made when

  * a conditional jump to E is followed by a `Jump`, with only labels
    between them,
  * then a block A that starts with a label, has no label, `Jump`, or
    `Ret` in it, and ends with a `Jump` to C,
  * then a block whose labels include E, with no label, jump, or
    `Ret` in it,
  * then a label run that includes C.

Nothing falls into A or into E's block, since each follows a `Jump`,
so the order of the two changes only which of them jumps to C. In
`handle_timers` the threading and the swap save 4 bytes.

# Where to run

Runs after the short-circuit fold, in the post-destruction fixed-point
loop, since the fold makes the shape above, and before SSA
destruction a Var's facts would have to follow its versions.
"""
from __future__ import annotations

import dataclasses

import c99_ast
import tac_ast
from passes.optimization.framework import (
    PassContext, PostDestructionFixedpointPass,
)
from passes.optimization.value_range import int_width
from passes.optimization.var_visit import defs_in, uses_in
from passes.type_checking import LocalAttr

# The bit width of each integer constant class.
_CONST_BITS = {
    tac_ast.ConstChar: 8, tac_ast.ConstUChar: 8,
    tac_ast.ConstInt: 16, tac_ast.ConstUInt: 16,
    tac_ast.ConstLong: 32, tac_ast.ConstULong: 32,
    tac_ast.ConstLongLong: 64, tac_ast.ConstULongLong: 64,
}

# Instructions that write their `dst` and nothing else.
_WRITES_DST_ONLY = (
    tac_ast.Copy, tac_ast.Unary, tac_ast.Binary,
    tac_ast.SignExtend, tac_ast.ZeroExtend, tac_ast.Truncate,
    tac_ast.IntToFloat, tac_ast.IntToDouble, tac_ast.FloatToInt,
    tac_ast.DoubleToInt, tac_ast.FloatToDouble, tac_ast.DoubleToFloat,
    tac_ast.GetAddress, tac_ast.Load, tac_ast.IndexedLoad,
    tac_ast.IndexedConstLoad, tac_ast.IndirectIndexedLoad,
)

_JUMPS = (tac_ast.Jump, tac_ast.JumpIfTrue, tac_ast.JumpIfFalse,
          tac_ast.JumpIfCmp, tac_ast.JumpIfMasked)

# How many tests the walk from a target may decide.
_STEPS = 16

_LABEL_PREFIX = "cthread@"


class ThreadKnownCompares(PostDestructionFixedpointPass):
    name = "thread_known_compares"

    def run(self, fn: tac_ast.Function, ctx: PassContext):
        return thread_known_compares(fn, ctx.symbols)


def thread_known_compares(fn: tac_ast.Function, symbols) -> tac_ast.Function:
    """Returns `fn` with each jump sent past the equality tests at its
    target that its path has decided, and each conditional jump that
    the code before it has decided made a `Jump` or removed. Without
    `symbols` the Vars' types are unknown, and `fn` comes back
    unchanged."""
    if symbols is None:
        return fn
    fn = _thread(fn, symbols)
    instrs = _place_branch_targets(_drop_unreached(fn.instructions))
    if instrs is fn.instructions:
        return fn
    return tac_ast.Function(
        name=fn.name, is_global=fn.is_global, params=list(fn.params),
        instructions=instrs,
    )


def _thread(fn: tac_ast.Function, symbols) -> tac_ast.Function:
    instrs = fn.instructions
    info = _Function(instrs, symbols)
    width_of = info.width_of
    retarget: dict[int, str] = {}
    replaced: dict[int, tac_ast.Type_instruction | None] = {}
    added: dict[int, str] = {}
    counter = _next_label_number(instrs, fn.name)
    facts: dict[str, set] = {}
    for k, ins in enumerate(instrs):
        if isinstance(ins, tac_ast.Label):
            facts = {}
            continue
        if isinstance(ins, _JUMPS):
            test = _equality_test(ins, width_of)
            if test is not None:
                decided = _decide(facts, test)
                if decided is False:
                    replaced[k] = None
                    continue
                if decided is True:
                    ins = tac_ast.Jump(target=ins.target)
                    replaced[k] = ins
            edge = _copy(facts)
            if test is not None and not isinstance(ins, tac_ast.Jump):
                _learn(edge, test, True)
            dest = _walk(info, edge, ins.target)
            if dest is not None:
                if dest > 0 and isinstance(instrs[dest - 1], tac_ast.Label):
                    retarget[k] = instrs[dest - 1].name
                else:
                    if dest not in added:
                        added[dest] = f".{fn.name}@{_LABEL_PREFIX}{counter}"
                        counter += 1
                    retarget[k] = added[dest]
            if isinstance(ins, tac_ast.Jump):
                facts = {}
            elif test is not None:
                _learn(facts, test, False)
            continue
        if isinstance(ins, _WRITES_DST_ONLY):
            dst = ins.dst
            if isinstance(dst, tac_ast.Var):
                facts.pop(dst.name, None)
            continue
        facts = {}
    if not retarget and not replaced:
        return fn
    out: list[tac_ast.Type_instruction] = []
    for k, ins in enumerate(instrs):
        if k in added:
            out.append(tac_ast.Label(name=added[k]))
        if k in replaced:
            ins = replaced[k]
            if ins is None:
                continue
        if k in retarget:
            ins = dataclasses.replace(ins, target=retarget[k])
        out.append(ins)
    return tac_ast.Function(
        name=fn.name, is_global=fn.is_global, params=list(fn.params),
        instructions=out,
    )


def _fact_width(sym) -> int | None:
    """The byte width of a Var whose values the pass follows, or None
    for a Var it doesn't (not an integer, or volatile)."""
    if sym is None:
        return None
    t = sym.type
    while isinstance(t, (c99_ast.Const, c99_ast.Volatile)):
        if isinstance(t, c99_ast.Volatile):
            return None
        t = t.referenced_type
    return int_width(t)


def _equality_test(ins, width_of):
    """`(name, value, equal_when_taken)` when `ins` jumps on `x == k`
    or `x != k` for a Var x the pass follows and an integer constant k
    of x's width, with k reduced modulo 2 to the width, else None."""
    if isinstance(ins, (tac_ast.JumpIfTrue, tac_ast.JumpIfFalse)):
        cond = ins.condition
        if isinstance(cond, tac_ast.Var) and width_of(cond.name):
            return cond.name, 0, isinstance(ins, tac_ast.JumpIfFalse)
        return None
    if not isinstance(ins, tac_ast.JumpIfCmp) or not isinstance(
            ins.op, (tac_ast.Equal, tac_ast.NotEqual)):
        return None
    for v, c in ((ins.src1, ins.src2), (ins.src2, ins.src1)):
        if not (isinstance(v, tac_ast.Var)
                and isinstance(c, tac_ast.Constant)):
            continue
        w = width_of(v.name)
        bits = _CONST_BITS.get(type(c.const))
        if w is None or bits != 8 * w:
            continue
        value = c.const.value & ((1 << bits) - 1)
        return v.name, value, isinstance(ins.op, tac_ast.Equal)
    return None


def _decide(facts, test) -> bool | None:
    """Whether the jump of `test` is taken, given `facts`, or None."""
    name, value, equal_when_taken = test
    known = facts.get(name)
    if not known:
        return None
    eq = next((v for kind, v in known if kind == "eq"), None)
    if eq is not None:
        equal = eq == value
    elif ("ne", value) in known:
        equal = False
    else:
        return None
    return equal == equal_when_taken


def _learn(facts, test, taken: bool) -> None:
    name, value, equal_when_taken = test
    equal = equal_when_taken == taken
    facts.setdefault(name, set()).add(("eq" if equal else "ne", value))


def _copy(facts):
    return {name: set(known) for name, known in facts.items()}


class _Function:
    """What the walks from the jumps of one function read: its
    instructions, its labels' indices, the widths of its Vars, and,
    computed the first time a walk passes a `Copy`, the Vars live at
    each instruction."""

    def __init__(self, instrs, symbols):
        self.instrs = instrs
        self.symbols = symbols
        self.labels = {
            ins.name: k for k, ins in enumerate(instrs)
            if isinstance(ins, tac_ast.Label)
        }
        self._widths: dict[str, int | None] = {}
        self._live: list[set[str]] | None = None
        self._addressed = {
            ins.operand.name for ins in instrs
            if isinstance(ins, tac_ast.GetAddress)
            and isinstance(ins.operand, tac_ast.Var)
        }

    def width_of(self, name: str) -> int | None:
        if name not in self._widths:
            self._widths[name] = _fact_width(self.symbols.get(name))
        return self._widths[name]

    def skippable(self, ins) -> str | None:
        """The destination of a `Copy` that a walk may pass without
        running it, if it's dead where the walk lands: a local whose
        address isn't taken, from a constant or a non-volatile Var.
        Else None."""
        if not (isinstance(ins, tac_ast.Copy)
                and isinstance(ins.dst, tac_ast.Var)):
            return None
        name = ins.dst.name
        sym = self.symbols.get(name)
        if (sym is None or not isinstance(sym.attrs, LocalAttr)
                or name in self._addressed):
            return None
        if isinstance(ins.src, tac_ast.Var):
            src = self.symbols.get(ins.src.name)
            if src is None or _is_volatile(src.type):
                return None
        return name

    def live_at(self, i: int) -> set[str]:
        """The Vars that some path from `instrs[i]` reads before writing
        them."""
        if self._live is None:
            self._live = _liveness(self.instrs, self.labels)
        return self._live[i] if i < len(self.instrs) else set()


def _is_volatile(t) -> bool:
    while isinstance(t, (c99_ast.Const, c99_ast.Volatile)):
        if isinstance(t, c99_ast.Volatile):
            return True
        t = t.referenced_type
    return False


def _liveness(instrs, labels) -> list[set[str]]:
    """The Vars live before each instruction, by the usual backward
    dataflow over the instructions."""
    n = len(instrs)
    succ: list[list[int]] = []
    for k, ins in enumerate(instrs):
        nxt = [k + 1] if k + 1 < n else []
        if isinstance(ins, tac_ast.Jump):
            nxt = []
        elif isinstance(ins, tac_ast.Ret):
            nxt = []
        if isinstance(ins, _JUMPS) and ins.target in labels:
            nxt = nxt + [labels[ins.target]]
        succ.append(nxt)
    uses = [{v.name for v in uses_in(ins)} for ins in instrs]
    defs = [{v.name for v in defs_in(ins)} for ins in instrs]
    live: list[set[str]] = [set() for _ in instrs]
    changed = True
    while changed:
        changed = False
        for k in range(n - 1, -1, -1):
            out: set[str] = set()
            for s in succ[k]:
                out |= live[s]
            new = uses[k] | (out - defs[k])
            if new != live[k]:
                live[k] = new
                changed = True
    return live


def _walk(fn: _Function, facts, target: str) -> int | None:
    """The furthest index that control reaches from `target` through
    labels, tests decided by `facts`, and `Copy`s into locals that are
    dead there, after at least one test, or None."""
    instrs, labels = fn.instrs, fn.labels
    i = labels.get(target)
    if i is None:
        return None
    facts = _copy(facts)
    best = None
    skipped: set[str] = set()
    seen: set[int] = set()
    steps = 0
    while i < len(instrs) and steps < _STEPS:
        ins = instrs[i]
        if isinstance(ins, tac_ast.Label):
            i += 1
            continue
        if i in seen:
            break
        seen.add(i)
        dst = fn.skippable(ins)
        if dst is not None:
            # A test of `dst` after this isn't decided.
            facts.pop(dst, None)
            skipped.add(dst)
            i += 1
            continue
        test = (_equality_test(ins, fn.width_of)
                if isinstance(ins, _JUMPS) else None)
        if test is None:
            break
        taken = _decide(facts, test)
        if taken is None:
            break
        steps += 1
        _learn(facts, test, taken)
        if taken:
            j = labels.get(ins.target)
            if j is None:
                break
            i = j
        else:
            i += 1
        # Past the labels, so that the last of them names the place.
        while i < len(instrs) and isinstance(instrs[i], tac_ast.Label):
            i += 1
        if not skipped & fn.live_at(i):
            best = i
    return best


def _drop_unreached(instrs):
    """Returns `instrs` without the blocks that nothing reaches: a run
    of labels that no jump names, after a `Jump` or `Ret`, and the code
    up to the next label. Threading leaves such a block where every
    jump to a test went past it. Returns `instrs` itself when there's
    none."""
    while True:
        named = {ins.target for ins in instrs if isinstance(ins, _JUMPS)}
        out = []
        dropping = False
        for ins in instrs:
            if isinstance(ins, tac_ast.Label):
                # Unreached unless a jump names it, when it follows a
                # terminator or a label dropped for the same reason.
                unreached = dropping or (
                    out and isinstance(out[-1], _TERMINATORS))
                if unreached and ins.name not in named:
                    dropping = True
                    continue
                dropping = False
            elif dropping:
                continue
            out.append(ins)
        if len(out) == len(instrs):
            return instrs
        instrs = out


_TERMINATORS = (tac_ast.Jump, tac_ast.Ret)

_CONDITIONAL = (tac_ast.JumpIfTrue, tac_ast.JumpIfFalse,
                tac_ast.JumpIfCmp, tac_ast.JumpIfMasked)


def _place_branch_targets(instrs):
    """Returns `instrs` with each pair of blocks after a conditional
    jump and a `Jump` swapped when the second block is the conditional
    jump's target, or `instrs` itself when there's none. See "Placing
    the target" in the module docstring."""
    out = list(instrs)
    changed = False
    k = 0
    while k < len(out):
        swapped = _swap_at(out, k)
        if swapped is not None:
            start, end, replacement = swapped
            out[start:end] = replacement
            changed = True
        k += 1
    return out if changed else instrs


def _label_run(instrs, i):
    """The index past the labels starting at `i`, and their names."""
    names = []
    while i < len(instrs) and isinstance(instrs[i], tac_ast.Label):
        names.append(instrs[i].name)
        i += 1
    return i, names


def _swap_at(instrs, k):
    """`(end, replacement)` for `instrs[k + 2:end]` when `instrs[k]`
    and `instrs[k + 1]` start the shape of "Placing the target", else
    None."""
    cond = instrs[k]
    if not isinstance(cond, _CONDITIONAL):
        return None
    # Labels between the two jumps don't matter, since the `Jump` stays.
    j, _ = _label_run(instrs, k + 1)
    if j >= len(instrs) or not isinstance(instrs[j], tac_ast.Jump):
        return None
    a_start = j + 1
    a_body, a_labels = _label_run(instrs, a_start)
    if not a_labels:
        return None
    a_end = a_body
    while a_end < len(instrs) and not isinstance(
            instrs[a_end], tac_ast.Label):
        a_end += 1
    last = instrs[a_end - 1]
    if a_end == a_body or not isinstance(last, tac_ast.Jump):
        return None
    if any(isinstance(ins, (tac_ast.Jump, tac_ast.Ret))
           for ins in instrs[a_body:a_end - 1]):
        return None
    e_body, e_labels = _label_run(instrs, a_end)
    if cond.target not in e_labels:
        return None
    e_end = e_body
    while e_end < len(instrs) and not isinstance(
            instrs[e_end], tac_ast.Label):
        if isinstance(instrs[e_end], _JUMPS + (tac_ast.Ret,)):
            return None
        e_end += 1
    if e_end == e_body:
        return None
    _, c_labels = _label_run(instrs, e_end)
    if last.target not in c_labels:
        return None
    replacement = (
        instrs[a_end:e_end] + [tac_ast.Jump(target=last.target)]
        + instrs[a_start:a_end - 1]
    )
    return a_start, e_end, replacement


def _next_label_number(instrs, fn_name: str) -> int:
    prefix = f".{fn_name}@{_LABEL_PREFIX}"
    numbers = [
        int(ins.name[len(prefix):]) for ins in instrs
        if isinstance(ins, tac_ast.Label) and ins.name.startswith(prefix)
        and ins.name[len(prefix):].isdigit()
    ]
    return 1 + max(numbers, default=-1)
