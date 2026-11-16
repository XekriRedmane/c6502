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

The same is true of a compare reached by falling through one or more
branches right after an identical compare,

    Compare(A, M)          CMP M
    Branch(CC, L1)         BCC L1
    Branch(NE, L2)         BNE L2
    Compare(A, M)          CMP M       <- dropped

which `three_way_compare` leaves when it splits `key < e` and `key ==
e` into a compare of each byte, and the first compare may likewise be
followed by a run of branches before the one that reaches L.

L must have no other way in. The instruction before the run of labels
that ends at the compare has to be a `Jump`, `Return`, or `Ret`, so
nothing falls through, and exactly one `Branch` or `Jump` in the
function may target a label in the run. A label whose address is
taken (`ImmLabelLow` / `ImmLabelHigh`) disqualifies the run.

A compare of a register with 0 sets N and Z from the register's
value and sets C. When the instruction that wrote the register set N
and Z from the same value, as a load, a transfer, or an ALU operation
on A does, and C is dead after the compare, the compare goes too:

    LDA   b+1                 LDA   b+1
    CMP   #$00         ->     BNE   .if_else
    BNE   .if_else

That's the high byte of `b == 2` in Zork's `instr_mul`
(examples/zork/interpreter/numbers.c). Stores may come between the
two, since they leave the flags and the register alone, except one
lowered with an `LDY #off` setup.

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


def _compare_before(instrs, b: int):
    """The instruction before the run of branches that ends with the
    branch at `b`, or None at the start of the function."""
    while b >= 0 and isinstance(instrs[b], asm_ast.Branch):
        b -= 1
    return instrs[b] if b >= 0 else None


_A = asm_ast.Reg(reg=asm_ast.A())
# Instructions that set N and Z from the value they leave in their dst
# register.
_SETS_NZ_FROM_DST = (
    asm_ast.Mov, asm_ast.Add, asm_ast.Sub, asm_ast.And, asm_ast.Or,
    asm_ast.Xor, asm_ast.ArithmeticShiftLeft, asm_ast.LogicalShiftRight,
    asm_ast.RotateLeft, asm_ast.RotateRight, asm_ast.Inc, asm_ast.Dec,
    asm_ast.Pop,
)


def _flags_from(instrs, c: int, reg) -> bool:
    """True when N and Z at `instrs[c]` were set from `reg`'s current
    value by the instruction that last wrote it, with only stores in
    between."""
    from passes.asm_liveness import sets_up_y
    for k in range(c - 1, -1, -1):
        ins = instrs[k]
        dst = getattr(ins, "dst", None)
        if (isinstance(ins, asm_ast.Mov) and not isinstance(dst, asm_ast.Reg)
                and isinstance(ins.src, asm_ast.Reg) and not sets_up_y(ins)):
            continue
        if not isinstance(ins, _SETS_NZ_FROM_DST) or dst != reg:
            return False
        if isinstance(ins, asm_ast.Mov) and (
            isinstance(ins.src, asm_ast.Reg) and ins.src == reg
            or not isinstance(ins.src, (asm_ast.Reg, asm_ast.Imm))
            and sets_up_y(ins) and isinstance(reg.reg, asm_ast.Y)
        ):
            return False
        return True
    return False


def _drop_compares_with_zero(instrs) -> set[int]:
    from passes.asm_liveness import c_dead_at
    drop: set[int] = set()
    for c, ins in enumerate(instrs):
        if (isinstance(ins, asm_ast.Compare)
                and isinstance(ins.left, asm_ast.Reg)
                and isinstance(ins.right, asm_ast.Imm)
                and ins.right.value & 0xFF == 0
                and _flags_from(instrs, c, ins.left)
                and c_dead_at(instrs, c + 1)):
            drop.add(c)
    return drop


def _rewrite_function(fn: asm_ast.Function) -> asm_ast.Function:
    instrs = fn.instructions
    refs = _label_refs(instrs)
    # The compares with 0 go in a sweep of their own, since one of
    # them may be the compare that a later identical one relies on for
    # C. The peephole fixed point runs this again.
    drop: set[int] = _drop_compares_with_zero(instrs)
    if drop:
        return asm_ast.Function(
            name=fn.name, is_global=fn.is_global, params=list(fn.params),
            instructions=[ins for i, ins in enumerate(instrs)
                          if i not in drop],
        )
    for c, cmp in enumerate(instrs):
        if not isinstance(cmp, asm_ast.Compare):
            continue
        if c > 0 and isinstance(instrs[c - 1], asm_ast.Branch):
            if _compare_before(instrs, c - 1) == cmp:
                drop.add(c)
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
        if _compare_before(instrs, b) != cmp:
            continue
        drop.add(c)
    if not drop:
        return fn
    return asm_ast.Function(
        name=fn.name, is_global=fn.is_global, params=list(fn.params),
        instructions=[ins for i, ins in enumerate(instrs) if i not in drop],
    )
