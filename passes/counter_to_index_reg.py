"""Move a pure count-down loop counter from memory into X (or Y).

# Motivating shape

    do { ... } while (--count != 0);

with `count` used for nothing else lowers to a counter in a ZP byte:

    LDA   n
    STA   count                 ; init
    .loop:
    ...
    DEC   count                 ; 5 cycles (zero page)
    BNE   .loop

When the loop doesn't use X, the hand-written form keeps the counter
in X instead: `LDX n` before the loop and `DEX` (2 cycles) at the
bottom. `loop_counter_to_x` does this for counters that are also an
index (reloaded with `LDX count`). This pass covers the counter that
is never read except by the `DEC` and its branch.

# Rewrite

`Mov(src, M)` (the init) becomes `Mov(src, Reg(X))` and `Dec(M)`
becomes `Dec(Reg(X))`. With `src = Reg(A)` that's `STA M` -> `TAX`,
and the peepholes later fold `LDA n; TAX` into `LDX n`. DEX sets N/Z
from the new value exactly as DEC M did, so the branch after it reads
the same flags.

# Eligibility

- `M` is a `Data` / `ZP` operand referenced by exactly two
  instructions in the function: one `Mov(src, M)` and one `Dec(M)`,
  and the `Dec` is directly followed by a `Branch(NE|EQ)` to a label
  at or before it (the loop's back edge). The init comes first.
- Between the init and that branch, no instruction from which the
  `DEC` can still be reached reads or writes the register (including
  through an indexed operand, or the `LDY` setup of a soft-stack or
  indirect operand when the register is Y), and there's no `Call`
  there, since a callee may clobber it, unless the callee is
  declared to preserve the register (`preserves_x` / `preserves_y`)
  and doesn't take it as an argument. An exit path (one that
  returns or leaves the loop for good) may use the register, since
  the counter is dead there.
- Nothing outside the loop jumps into it, so every path to the
  `DEC` passes through the init. The loop is every instruction the
  code after the init reaches and that reaches the `DEC` again, so a
  block of it placed out of line counts, as the `ssa_split` block
  that `divu16` (examples/zork/numbers.c) leaves after its
  `RTS` for the path where the divisor doesn't fit.
- The register is dead at the init (`x_dead_at` / `y_dead_at`), so
  overwriting it there can't change a later read. An init from the
  register itself, `STY M` with the counter going in Y, is dropped
  instead, when Y is dead after it. An init from the other index
  register is left alone, since the 6502 has no `TXY` or `TYX`, and
  `asm_emit` writes `Mov(Reg(Y), Reg(X))` as `TYA; TAX`, which
  changes A behind the backs of the passes that run after this one.

X is tried first, then Y.

# Where it runs

After `loop_counter_to_x` and before the second peephole fixedpoint,
which folds the `LDA n; TAX` the rewrite leaves.
"""

from __future__ import annotations

import asm_ast
from passes.asm_liveness import call_preserves, x_dead_at, y_dead_at

_SETUP_USES_Y = (
    asm_ast.Frame, asm_ast.Stack, asm_ast.Indirect, asm_ast.IndirectZp,
)


def apply_counter_to_index_reg(prog: asm_ast.Program) -> asm_ast.Program:
    out = []
    for tl in prog.top_level:
        if isinstance(tl, asm_ast.Function):
            instrs = list(tl.instructions)
            while True:
                new = _rewrite_one(instrs)
                if new is None:
                    break
                instrs = new
            tl = asm_ast.Function(
                name=tl.name, is_global=tl.is_global,
                params=list(tl.params), instructions=instrs,
            )
        out.append(tl)
    return asm_ast.Program(top_level=out)


def _operands(instr) -> list:
    match instr:
        case asm_ast.Mov(src=s, dst=d) | asm_ast.LoadAddress(src=s, dst=d):
            return [s, d]
        case asm_ast.Add(src=s, dst=d) | asm_ast.Sub(src=s, dst=d) \
                | asm_ast.And(src=s, dst=d) | asm_ast.Or(src=s, dst=d):
            return [s, d]
        case asm_ast.Xor(src1=s1, src2=s2, dst=d):
            return [s1, s2, d]
        case asm_ast.Compare(left=l, right=r):
            return [l, r]
        case asm_ast.Inc(dst=d) | asm_ast.Dec(dst=d) \
                | asm_ast.ArithmeticShiftLeft(dst=d) \
                | asm_ast.LogicalShiftRight(dst=d) \
                | asm_ast.RotateLeft(dst=d) | asm_ast.RotateRight(dst=d) \
                | asm_ast.Pop(dst=d):
            return [d]
        case asm_ast.Push(src=s) | asm_ast.BitTest(src=s):
            return [s]
    return []


def _key(op):
    if isinstance(op, asm_ast.Data):
        return ("data", op.name, op.offset)
    if isinstance(op, asm_ast.ZP):
        return ("zp", op.address + op.offset)
    return None


def _touches_reg(instr, reg_cls) -> bool:
    """True iff `instr` reads or writes the register, directly, as an
    index, or (for Y) through an operand's `LDY` setup. A call touches
    it unless the callee is declared to preserve it
    (`asm_liveness.call_preserves`) and doesn't take it as an
    argument."""
    if isinstance(instr, asm_ast.Call):
        reg = "X" if reg_cls is asm_ast.X else "Y"
        return not call_preserves(instr, reg) or reg in instr.reg_args
    if isinstance(instr, (
        asm_ast.FunctionPrologue, asm_ast.AllocateStack,
        asm_ast.Ret, asm_ast.Return, asm_ast.LoadAddress,
    )):
        return True
    if isinstance(instr, (asm_ast.Jump,)) and instr.reg_args:
        return True
    for op in _operands(instr):
        if isinstance(op, asm_ast.Reg) and isinstance(op.reg, reg_cls):
            return True
        if isinstance(op, asm_ast.IndexedData) and isinstance(op.index, reg_cls):
            return True
        if reg_cls is asm_ast.Y and isinstance(
            op, (asm_ast.IndirectY, asm_ast.IndirectZpY) + _SETUP_USES_Y,
        ):
            return True
    return False


def _target(instr):
    if isinstance(instr, (asm_ast.Jump, asm_ast.Branch)):
        return instr.target
    return None


def _reaches(instrs, start: int, goal: int, label_at) -> bool:
    """True iff control can get from `instrs[start]` to `goal`."""
    seen: set[int] = set()
    stack = [start]
    while stack:
        i = stack.pop()
        while i < len(instrs) and i not in seen:
            if i == goal:
                return True
            seen.add(i)
            ins = instrs[i]
            if isinstance(ins, (asm_ast.Return, asm_ast.Ret)):
                break
            t = _target(ins)
            if t is not None:
                if t not in label_at:
                    break
                if isinstance(ins, asm_ast.Jump):
                    i = label_at[t]
                    continue
                stack.append(label_at[t])
            i += 1
    return False


def _successors(instrs, i: int, label_at) -> list[int]:
    ins = instrs[i]
    if isinstance(ins, (asm_ast.Return, asm_ast.Ret)):
        return []
    t = _target(ins)
    if isinstance(ins, asm_ast.Jump):
        return [label_at[t]] if t in label_at else []
    out = [i + 1] if i + 1 < len(instrs) else []
    if t is not None and t in label_at:
        out.append(label_at[t])
    return out


def _loop_region(instrs, init_i: int, dec_i: int, label_at):
    """The instructions the code after the init reaches and that reach
    the DEC again without passing the init: the loop, with any block
    of it placed out of line (as `ssa_split` blocks are, after the
    function's return). None
    when an instruction outside it, other than the init, jumps or
    falls into it, since the counter would arrive there without its
    init."""
    succ = [_successors(instrs, i, label_at) for i in range(len(instrs))]
    forward: set[int] = set()
    stack = [init_i + 1]
    while stack:
        i = stack.pop()
        if i in forward or i >= len(instrs):
            continue
        forward.add(i)
        stack.extend(succ[i])
    if dec_i not in forward:
        return None
    preds: dict[int, list[int]] = {}
    for i in range(len(instrs)):
        for s2 in succ[i]:
            preds.setdefault(s2, []).append(i)
    loop: set[int] = set()
    stack = [dec_i]
    while stack:
        i = stack.pop()
        # The walk back stops at the init, which gives the counter its
        # value. An enclosing loop that runs the init again on each of
        # its passes is outside the counter's loop, so the counter
        # may take a register the enclosing loop uses, such as the
        # outer counter of a delay `do { for (y = n; --y;) {} } while
        # (--count);`.
        if i in loop or i not in forward or i == init_i:
            continue
        loop.add(i)
        stack.extend(preds.get(i, ()))
    for i in range(len(instrs)):
        if i in loop or i == init_i:
            continue
        if any(s2 in loop for s2 in succ[i]):
            return None
    return loop


def _rewrite_one(instrs):
    refs: dict[tuple, list[int]] = {}
    for i, ins in enumerate(instrs):
        for op in _operands(ins):
            k = _key(op)
            if k is not None:
                refs.setdefault(k, []).append(i)
    label_at = {
        ins.name: i for i, ins in enumerate(instrs)
        if isinstance(ins, asm_ast.Label)
    }
    for k, idxs in refs.items():
        idxs = sorted(set(idxs))
        if len(idxs) != 2:
            continue
        init_i, dec_i = idxs
        init, dec = instrs[init_i], instrs[dec_i]
        if not (
            isinstance(init, asm_ast.Mov) and not init.is_volatile
            and _key(init.dst) == k
            and isinstance(init.src, (
                asm_ast.Imm, asm_ast.Data, asm_ast.ZP, asm_ast.Reg,
                asm_ast.IndexedData,
            ))
            and isinstance(dec, asm_ast.Dec) and _key(dec.dst) == k
            and dec_i + 1 < len(instrs)
        ):
            continue
        br = instrs[dec_i + 1]
        if not (
            isinstance(br, asm_ast.Branch)
            and isinstance(br.cond, (asm_ast.NE, asm_ast.EQ))
            and br.target in label_at
            and init_i < label_at[br.target] <= dec_i
        ):
            continue
        loop = _loop_region(instrs, init_i, dec_i, label_at)
        if loop is None:
            continue
        for reg_cls, dead_at in ((asm_ast.X, x_dead_at), (asm_ast.Y, y_dead_at)):
            # The register may only be touched where the counter is
            # dead: at points from which the DEC can't be reached
            # again (an exit path out of the loop).
            if any(
                _touches_reg(instrs[i], reg_cls)
                for i in loop if i != dec_i
            ):
                continue
            # LDX / LDY must be able to load the init's source: no
            # `abs,X` into X (`abs,Y` into Y), and not the other index
            # register, since the 6502 has no TXY or TYX. `asm_emit`
            # would write `Mov(Y, X)` as `TYA; TAX`, which changes A
            # where the passes after this one take A to be unchanged.
            # The register itself already holds the init's value, and
            # its store goes, when nothing after the store reads the
            # value the register held, which the DEC now changes.
            src = init.src
            reg = asm_ast.Reg(reg=reg_cls())
            out = list(instrs)
            if isinstance(src, asm_ast.Reg) and isinstance(src.reg, reg_cls):
                if not dead_at(instrs, init_i + 1):
                    continue
                out[dec_i] = asm_ast.Dec(dst=reg)
                del out[init_i]
                return out
            if isinstance(src, asm_ast.Reg) and isinstance(
                src.reg, (asm_ast.X, asm_ast.Y),
            ):
                continue
            if isinstance(src, asm_ast.IndexedData) and isinstance(
                src.index, reg_cls,
            ):
                continue
            if not dead_at(instrs, init_i):
                continue
            out[init_i] = asm_ast.Mov(src=init.src, dst=reg, is_volatile=False)
            out[dec_i] = asm_ast.Dec(dst=reg)
            return out
    return None
