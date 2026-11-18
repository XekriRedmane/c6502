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
(examples/zork/numbers.c). Stores may come between the
two, since they leave the flags and the register alone, except one
lowered with an `LDY #off` setup.

A compare with the byte an add started from is the add's carry:

    LDA   Z_PC                LDA   Z_PC
    CLC                       CLC
    ADC   offset       ->     ADC   offset
    STA   new_lo              STA   new_lo
    CMP   Z_PC                BCC   .if_end
    BCS   .if_end

The sum `m + x` wraps below `m` exactly when the add carries out, so
`CMP m` sets C to the opposite of the add's carry, and the branch on C
takes the opposite condition with the compare gone
(`_carry_of_add`). Only stores from A may come between the add and
the compare, none to `m`, and N and Z must be dead after the branch
on both paths, since the add sets them from the sum. That's `if
(new_lo < lo)` after `new_lo = lo + offset`, the C for the carry of a
byte add, in Zork's `branch_to_offset`
(examples/zork/decode.c).

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
        for field in vars(ins).values():
            # `DecimalArith` keeps its operands in lists.
            for op in field if isinstance(field, list) else (field,):
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


def _flags_from(instrs, c: int, reg, refs=None, seen=None) -> bool:
    """True when N and Z at `instrs[c]` were set from `reg`'s current
    value by the instruction that last wrote it, with only stores in
    between.

    With `refs` (`_label_refs`), the walk also passes over conditional
    branches, which leave registers and flags alone, and follows a
    label to every way in: each branch or jump to it, and the
    fall-through from the instruction before it. Every one of them
    must have set the flags from `reg`. A label it meets twice, a
    label a `JumpTable` or a label operand names, and the function's
    start all count as not. In `je`, `LDA #$00; BEQ end` and `LDA
    #$01` both reach `end: STA equal`, so the `ORA #$00` after the
    store tests what the loads set."""
    from passes.asm_liveness import sets_up_y
    for k in range(c - 1, -1, -1):
        ins = instrs[k]
        dst = getattr(ins, "dst", None)
        if (isinstance(ins, asm_ast.Mov) and not isinstance(dst, asm_ast.Reg)
                and isinstance(ins.src, asm_ast.Reg) and not sets_up_y(ins)):
            continue
        if refs is not None and isinstance(ins, asm_ast.Branch):
            continue
        if refs is not None and isinstance(ins, asm_ast.Label):
            if seen is None:
                seen = set()
            if ins.name in seen or ins.name in refs.get(_TABLE_TARGETS, ()):
                return False
            seen.add(ins.name)
            preds = refs.get(ins.name, [])
            if any(p < 0 for p in preds):
                return False
            for p in preds:
                if not _flags_from(instrs, p, reg, refs, seen):
                    return False
            if k == 0:
                return False
            if isinstance(instrs[k - 1], (*_NO_FALL_THROUGH, asm_ast.JumpTable)):
                return bool(preds)
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
    """The indexes of the `CMP #$00` (or `CPX` / `CPY`) with C dead
    after, and of the `ORA #$00`, `AND #$FF`, and `EOR #$00`, whose N
    and Z the instruction that set the register already set. The
    `EOR #$00` comes from the low bytes of the mask that restores a
    signed bit-precise type's padding (bit_precise.py)."""
    from passes.asm_liveness import c_dead_at
    refs = _label_refs(instrs)
    refs[_TABLE_TARGETS] = {
        t for ins in instrs if isinstance(ins, asm_ast.JumpTable)
        for t in ins.targets
    }
    drop: set[int] = set()
    for c, ins in enumerate(instrs):
        if (isinstance(ins, asm_ast.Compare)
                and isinstance(ins.left, asm_ast.Reg)
                and isinstance(ins.right, asm_ast.Imm)
                and ins.right.value & 0xFF == 0
                and _flags_from(instrs, c, ins.left, refs)
                and c_dead_at(instrs, c + 1)):
            drop.add(c)
        elif (isinstance(ins, (asm_ast.Or, asm_ast.And))
                and _is_a(ins.dst) and isinstance(ins.src, asm_ast.Imm)
                and ins.src.value & 0xFF == (
                    0 if isinstance(ins, asm_ast.Or) else 0xFF)
                and _flags_from(instrs, c, ins.dst, refs)):
            drop.add(c)
        elif (isinstance(ins, asm_ast.Xor)
                and _is_a(ins.dst) and _is_a(ins.src1)
                and isinstance(ins.src2, asm_ast.Imm)
                and ins.src2.value & 0xFF == 0
                and _flags_from(instrs, c, ins.dst, refs)):
            drop.add(c)
    return drop


# The key in `_label_refs`' map under which `_drop_compares_with_zero`
# keeps the labels a `JumpTable` names. It isn't a label's name.
_TABLE_TARGETS = " jump table targets"


def _carry_of_add(instrs) -> dict[int, asm_ast.Type_instruction | None]:
    """The rewrites of `LDA m; CLC; ADC x; [STA r ...]; CMP m; BCC/BCS
    L`: the compare's index to None and the branch's to the branch on
    the opposite condition. See the module docstring."""
    from passes.asm_aliasing import may_alias
    from passes.asm_liveness import flags_dead_at
    labels = {
        ins.name: k for k, ins in enumerate(instrs)
        if isinstance(ins, asm_ast.Label)
    }
    out: dict[int, asm_ast.Type_instruction | None] = {}
    for i, load in enumerate(instrs):
        if not (isinstance(load, asm_ast.Mov) and not load.is_volatile
                and isinstance(load.src, (asm_ast.Data, asm_ast.ZP))
                and _is_a(load.dst)
                and i + 4 < len(instrs)
                and isinstance(instrs[i + 1], asm_ast.ClearCarry)
                and isinstance(instrs[i + 2], asm_ast.Add)
                and _is_a(instrs[i + 2].dst)
                and not isinstance(instrs[i + 2].src, asm_ast.Reg)):
            continue
        m = load.src
        c = i + 3
        while (c < len(instrs) and isinstance(instrs[c], asm_ast.Mov)
               and _is_a(instrs[c].src) and not instrs[c].is_volatile
               and not isinstance(instrs[c].dst, asm_ast.Reg)
               and not may_alias(instrs[c].dst, m)):
            c += 1
        if not (c + 1 < len(instrs)
                and instrs[c] == asm_ast.Compare(
                    left=asm_ast.Reg(reg=asm_ast.A()), right=m)
                and isinstance(instrs[c + 1], asm_ast.Branch)
                and isinstance(instrs[c + 1].cond, (asm_ast.CC, asm_ast.CS))
                and instrs[c + 1].target in labels
                and flags_dead_at(instrs, c + 2)
                and flags_dead_at(instrs, labels[instrs[c + 1].target])):
            continue
        branch = instrs[c + 1]
        flipped = asm_ast.CS() if isinstance(branch.cond, asm_ast.CC) else asm_ast.CC()
        out[c] = None
        out[c + 1] = asm_ast.Branch(cond=flipped, target=branch.target)
    return out


def _is_a(op) -> bool:
    return isinstance(op, asm_ast.Reg) and isinstance(op.reg, asm_ast.A)


def _rewrite_function(fn: asm_ast.Function) -> asm_ast.Function:
    instrs = fn.instructions
    carry = _carry_of_add(instrs)
    if carry:
        out = []
        for i, ins in enumerate(instrs):
            if i in carry:
                if carry[i] is not None:
                    out.append(carry[i])
            else:
                out.append(ins)
        return asm_ast.Function(
            name=fn.name, is_global=fn.is_global, params=list(fn.params),
            instructions=out,
        )
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
