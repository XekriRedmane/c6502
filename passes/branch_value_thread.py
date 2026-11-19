"""Branch threading on known values: a branch whose target re-tests
what the path to it already decided goes past the test.

# Pattern

    LDA   b+1
    BNE   L               ; b+1 != 0 on this edge, and A holds it
    LDA   b
    CMP   #$02
    BNE   L               ; b+1 == 0 and A == b != 2 on this edge
    ...
L:
    LDA   b+1             ; A already holds b+1, and Z is the same
    BNE   M               ; decided on each edge into L
    LDA   b
    CMP   #$04
    BNE   M

Each edge into L knows something about the bytes the code at L tests.
On the first, A holds `b+1` and it isn't 0, so the code at L reloads
A with the same byte and takes `BNE M`: the first branch can go
straight to M. On the second, `b+1` is 0 from the first test, so
the `BNE M` at L isn't taken, and the `LDA b` after it leaves A as
it was: the second branch can go to the `CMP #$04`, where a label is
added. A test written as `if (b == 2) ... else if (b == 4) ...` on a
2-byte `b` leaves this shape, and Zork's `instr_mul` and `instr_div`
(examples/zork/numbers.c) test `b`'s high byte once,
where the C had tested it twice.

# How

For each branch, and each `JMP`, to a label in the function, a scan
of the straight-line code before it (from the last label) follows
what A holds (a byte of memory or a constant), what N and Z were set
from (a load of a byte, or a `CMP #k` of one), and what the branches
passed on the way decided about bytes of memory (equal or not equal
to a constant). The edge into the target adds what the branch itself
decides.

From the target, the pass then simulates loads of a byte or a
constant into A, `CMP #k` of A, and branches, following each branch
it can decide, and stops at anything else. A point it reaches is a
new target for the edge when A holds what it held on the edge, or is
dead there, and so are N/Z and C. X, Y, V, and memory don't change in
the code it simulates. The edge goes to the furthest such point.

A byte that's volatile is never followed, and a store, a call, or any
instruction the scan doesn't model forgets what it knew.

# Where to run

In the asm peephole fixed-point loop, after `branch_thread`. The
targets that lose their last branch go in `dead_label_drop`, and the
loads they pass over in `redundant_load` and `dead_a_arith`.
"""
from __future__ import annotations

import re

import asm_ast
from passes.asm_aliasing import may_alias
from passes.asm_liveness import a_dead_at, c_dead_at, flags_dead_at

_LABEL_RE = re.compile(r"\.vthread@(\d+)$")
# How many instructions the simulation from a target may take.
_STEPS = 24
_MEMORY = (asm_ast.Data, asm_ast.ZP)


def apply_branch_value_thread(prog: asm_ast.Program) -> asm_ast.Program:
    """Retargets every branch and `JMP` whose target's tests are
    decided on its edge, in every function. Labels are numbered across
    the program, since the sim's assembler rejects one name in two
    functions."""
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


class _State:
    """What's known at a point. `a` is the operand A holds (a memory
    byte or an `Imm`), or None. `nz` is what N and Z were set from:
    `("val", op)` for a load of `op` into A, `("cmp", op, k)` for a
    `CMP #k` of A holding `op`, or None. `facts` maps a memory byte
    (by `repr`) to its operand and what's known of its value, a set of
    `("eq", v)` / `("ne", v)` pairs."""

    def __init__(self):
        self.a = None
        self.nz = None
        self.facts: dict[str, tuple[object, set]] = {}

    def copy(self) -> "_State":
        s = _State()
        s.a, s.nz = self.a, self.nz
        s.facts = {k: (op, set(fs)) for k, (op, fs) in self.facts.items()}
        return s

    def forget(self, written) -> None:
        """Forgets what's known of every byte `written` may write, and
        A's and the flags' link to one."""
        for key in [k for k, (op, _) in self.facts.items()
                    if may_alias(op, written)]:
            del self.facts[key]
        if isinstance(self.a, _MEMORY) and may_alias(self.a, written):
            self.a = None
        if self.nz is not None and isinstance(self.nz[1], _MEMORY) \
                and may_alias(self.nz[1], written):
            self.nz = None

    def learn(self, op, fact) -> None:
        if isinstance(op, _MEMORY):
            self.facts.setdefault(repr(op), (op, set()))[1].add(fact)

    def value_of(self, op):
        """`op`'s value when it's known, else None."""
        if isinstance(op, asm_ast.Imm):
            return op.value
        if isinstance(op, _MEMORY):
            for kind, v in self.facts.get(repr(op), (None, set()))[1]:
                if kind == "eq":
                    return v
        return None

    def differs_from(self, op, v) -> bool | None:
        """True if `op` is known not to equal `v`, False if it's known
        to equal it, None if neither."""
        known = self.value_of(op)
        if known is not None:
            return (known & 0xFF) != (v & 0xFF)
        if isinstance(op, _MEMORY):
            if ("ne", v) in self.facts.get(repr(op), (None, set()))[1]:
                return True
        return None


def _decide(state: _State, cond) -> bool | None:
    """Whether a branch on `cond` is taken in `state`, or None."""
    nz = state.nz
    if nz is None:
        return None
    if nz[0] == "val":
        op, k = nz[1], 0
    else:
        op, k = nz[1], nz[2]
    if isinstance(cond, (asm_ast.EQ, asm_ast.NE)):
        differs = state.differs_from(op, k)
        if differs is None:
            return None
        return differs if isinstance(cond, asm_ast.NE) else not differs
    v = state.value_of(op)
    if v is None:
        return None
    v &= 0xFF
    if isinstance(cond, (asm_ast.MI, asm_ast.PL)):
        r = (v - k) & 0xFF
        neg = bool(r & 0x80)
        return neg if isinstance(cond, asm_ast.MI) else not neg
    if nz[0] == "cmp" and isinstance(cond, (asm_ast.CC, asm_ast.CS)):
        carry = v >= (k & 0xFF)
        return carry if isinstance(cond, asm_ast.CS) else not carry
    return None


def _learn_branch(state: _State, cond, taken: bool) -> None:
    """Adds what a branch on `cond`, taken or not, says about the byte
    N and Z were set from."""
    nz = state.nz
    if nz is None or not isinstance(cond, (asm_ast.EQ, asm_ast.NE)):
        return
    op, k = (nz[1], 0) if nz[0] == "val" else (nz[1], nz[2])
    equal = isinstance(cond, asm_ast.EQ) == taken
    state.learn(op, ("eq" if equal else "ne", k & 0xFF))


def _readable(op) -> bool:
    return isinstance(op, (asm_ast.Imm, asm_ast.Data, asm_ast.ZP))


def _scan_step(state: _State, ins) -> bool:
    """Updates `state` for `ins` in the straight-line scan before a
    branch. Returns False when the scan must start over (a label, or
    an instruction it doesn't model)."""
    if isinstance(ins, asm_ast.Mov):
        src, dst = ins.src, ins.dst
        if _is_a(dst):
            if _readable(src) and not ins.is_volatile:
                state.a = src
                state.nz = ("val", src)
            else:
                state.a = None
                state.nz = None
            return True
        if _is_reg(dst):
            # LDX / LDY / TAX / TAY set N and Z from the new value.
            state.nz = None
            return True
        if isinstance(dst, _MEMORY):
            state.forget(dst)
            if not _is_reg(src):
                # A move from memory or a constant is `LDA src; STA
                # dst`.
                if _readable(src) and not ins.is_volatile:
                    state.a, state.nz = src, ("val", src)
                else:
                    state.a, state.nz = None, None
            return True
        return False
    if isinstance(ins, asm_ast.Compare):
        if _is_a(ins.left) and isinstance(ins.right, asm_ast.Imm) \
                and isinstance(state.a, (asm_ast.Imm,) + _MEMORY):
            state.nz = ("cmp", state.a, ins.right.value)
        else:
            state.nz = None
        return True
    if isinstance(ins, (asm_ast.SetCarry, asm_ast.ClearCarry)):
        return True
    if isinstance(ins, (asm_ast.Inc, asm_ast.Dec,
                        asm_ast.ArithmeticShiftLeft,
                        asm_ast.LogicalShiftRight,
                        asm_ast.RotateLeft, asm_ast.RotateRight)):
        if isinstance(ins.dst, _MEMORY):
            state.forget(ins.dst)
        elif _is_a(ins.dst):
            state.a = None
        elif not _is_reg(ins.dst):
            # An element indexed by X (`DEC t,X`) may be any byte of
            # `t` and past it, as a store to one is.
            return False
        state.nz = None
        return True
    if isinstance(ins, (asm_ast.Add, asm_ast.Sub, asm_ast.And,
                        asm_ast.Or, asm_ast.Xor)):
        state.a, state.nz = None, None
        return True
    return False


def _is_a(op) -> bool:
    return isinstance(op, asm_ast.Reg) and isinstance(op.reg, asm_ast.A)


def _is_reg(op) -> bool:
    return isinstance(op, asm_ast.Reg)


def _same(x, y) -> bool:
    return x is not None and y is not None and repr(x) == repr(y)


def _new_target(instrs, labels, start: _State, at: int):
    """The furthest index the simulation from `instrs[at]` reaches with
    `start`'s registers and live flags, or None."""
    state = start.copy()
    # Which of A, N/Z, and C the simulated code has written.
    changed = {"a": False, "nz": False, "c": False}
    executed = False
    best = None
    i = at
    seen: set[int] = set()
    for _ in range(_STEPS):
        if i >= len(instrs) or i in seen:
            break
        seen.add(i)
        ins = instrs[i]
        if isinstance(ins, asm_ast.Label):
            i += 1
            continue
        if executed and _equivalent(instrs, i, start, state, changed):
            best = i
        executed = True
        if isinstance(ins, asm_ast.Mov) and _is_a(ins.dst) \
                and _readable(ins.src) and not ins.is_volatile:
            state.a = ins.src
            state.nz = ("val", ins.src)
            changed["a"] = changed["nz"] = True
            i += 1
            continue
        if isinstance(ins, asm_ast.Compare) and _is_a(ins.left) \
                and isinstance(ins.right, asm_ast.Imm) \
                and isinstance(state.a, (asm_ast.Imm,) + _MEMORY):
            state.nz = ("cmp", state.a, ins.right.value)
            changed["nz"] = changed["c"] = True
            i += 1
            continue
        if isinstance(ins, asm_ast.Branch):
            taken = _decide(state, ins.cond)
            if taken is None:
                break
            _learn_branch(state, ins.cond, taken)
            if taken:
                j = labels.get(ins.target)
                if j is None:
                    break
                i = j
            else:
                i += 1
            continue
        break
    return best


def _equivalent(instrs, i, start: _State, now: _State, changed) -> bool:
    """Whether arriving at `instrs[i]` in `now` is the same as arriving
    in `start` for everything read from there on. `changed` says which
    of A, N/Z, and C the simulated code wrote."""
    if changed["a"] and not (_same(start.a, now.a)
                             or a_dead_at(instrs, i)):
        return False
    if changed["nz"] and not (_same_nz(start.nz, now.nz)
                              or flags_dead_at(instrs, i)):
        return False
    if changed["c"] and not c_dead_at(instrs, i):
        return False
    return True


def _same_nz(x, y) -> bool:
    if x is None or y is None:
        return False
    return repr(x) == repr(y)


def _rewrite_function(
    fn: asm_ast.Function, counter: int,
) -> tuple[asm_ast.Function, int]:
    instrs = fn.instructions
    labels = {
        ins.name: k for k, ins in enumerate(instrs)
        if isinstance(ins, asm_ast.Label)
    }
    retarget: dict[int, str] = {}
    added: dict[int, str] = {}
    state = _State()
    for k, ins in enumerate(instrs):
        if isinstance(ins, asm_ast.Label):
            state = _State()
            continue
        if isinstance(ins, (asm_ast.Branch, asm_ast.Jump)):
            at = labels.get(ins.target)
            if at is not None:
                edge = state.copy()
                if isinstance(ins, asm_ast.Branch):
                    taken = _decide(edge, ins.cond)
                    if taken is False:
                        # Never taken; another pass's business.
                        edge = None
                    else:
                        _learn_branch(edge, ins.cond, True)
                if edge is not None:
                    dest = _new_target(instrs, labels, edge, at)
                    if dest is not None:
                        if dest > 0 and isinstance(
                            instrs[dest - 1], asm_ast.Label,
                        ):
                            retarget[k] = instrs[dest - 1].name
                        else:
                            if dest not in added:
                                added[dest] = f".vthread@{counter}"
                                counter += 1
                            retarget[k] = added[dest]
            if isinstance(ins, asm_ast.Branch):
                _learn_branch(state, ins.cond, False)
                continue
            state = _State()
            continue
        if not _scan_step(state, ins):
            state = _State()
    if not retarget:
        return fn, counter
    out: list[asm_ast.Type_instruction] = []
    for k, ins in enumerate(instrs):
        if k in added:
            out.append(asm_ast.Label(name=added[k]))
        if k in retarget:
            if isinstance(ins, asm_ast.Branch):
                ins = asm_ast.Branch(cond=ins.cond, target=retarget[k])
            else:
                ins = asm_ast.Jump(target=retarget[k])
        out.append(ins)
    return asm_ast.Function(
        name=fn.name, is_global=fn.is_global,
        params=list(fn.params), instructions=out,
    ), counter
