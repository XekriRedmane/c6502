"""Loads a loop's one constant in A once, before the loop.

In a loop where every write of A is `LDA #k` of one constant `k`,
except for `TXA` / `TYA` transfers whose value nothing reads, A holds
`k` wherever the loop reads it once the transfers are gone. The pass
removes the loads and the transfers and puts one `LDA #k` in front of
the loop's header.

The transfers are what a test of an index register leaves, as in the
loop that clears an automatic aggregate's bytes
(`c99_to_tac._emit_zero_bytes`):

    LDY #n                    LDY #n
    .clear:                   LDA #0
    DEY                       .clear:
    LDA #0          ->        DEY
    STA v,Y                   STA v,Y
    TYA                       BNE .clear
    BNE .clear

`dead_a_arith` drops a `TYA` whose flags `DEY` has already set, but
here the `LDA #0` sets the flags between them, and the `LDA #0` can't
leave the loop while the `TYA` writes A. So this pass takes the two
steps together. The loop is 4 cycles a byte shorter and a byte
smaller.

# What the loop must look like

The loop runs from its header label to a branch or jump back to it,
which comes later in the code, and nothing outside it branches to the
header or into it. The instruction before the header doesn't end its
block, so the code before the loop falls into the new `LDA #k`.
Every instruction in the loop is one of these.

  * `LDA #k`, all with the same `k`, where N and Z are dead after
    it, since the load is gone.
  * `TXA` or `TYA`, where A is dead after it and N and Z are dead or
    already set from the register (`dead_a_arith._flags_reflect_src_reg`,
    asked of the code without the loads).
  * An instruction that leaves A alone and that `asm_emit` writes
    without going through A (`_leaves_a`).

A is dead at the header, so no read in the loop sees the value A had
before it, and N and Z are dead there, since the new `LDA #k` sets
them.

# Why the values stay the same

After the change A holds `k` from the header on. A read of A in the
loop or after it got its value from one of the loads, since A is
dead after each transfer and at the header, and that value was `k`.
The flags are unchanged wherever they're read. A removed load's N and
Z are dead, a removed transfer's are what the earlier instruction set
from the same register, and the new load's are dead at the header.
"""

from __future__ import annotations

import asm_ast
from passes.asm_licm import _branch_or_jump_target, _is_single_entry
from passes.asm_liveness import a_dead_at, flags_dead_at, is_reg_a
from passes.dead_a_arith import _flags_reflect_src_reg


def apply_loop_constant_a(prog: asm_ast.Program) -> asm_ast.Program:
    """Loads each qualifying loop's constant in A once, before the
    loop, in every function of `prog`."""
    new_top: list[asm_ast.Type_top_level] = []
    for tl in prog.top_level:
        if isinstance(tl, asm_ast.Function):
            instrs = list(tl.instructions)
            while True:
                changed = _one_loop(instrs)
                if changed is None:
                    break
                instrs = changed
            if instrs != tl.instructions:
                tl = asm_ast.Function(
                    name=tl.name, is_global=tl.is_global,
                    params=list(tl.params), instructions=instrs,
                )
        new_top.append(tl)
    return asm_ast.Program(top_level=new_top)


def _one_loop(
    instrs: list[asm_ast.Type_instruction],
) -> list[asm_ast.Type_instruction] | None:
    """Rewrites the innermost loop that qualifies and returns the new
    instructions, or None when no loop does."""
    label_idx = {
        ins.name: k for k, ins in enumerate(instrs)
        if isinstance(ins, asm_ast.Label)
    }
    loops = []
    for k, ins in enumerate(instrs):
        h = label_idx.get(_branch_or_jump_target(ins))
        if h is not None and h < k:
            loops.append((h, k))
    loops.sort(key=lambda hk: hk[1] - hk[0])
    for h, b in loops:
        out = _rewrite_loop(instrs, h, b, label_idx)
        if out is not None:
            return out
    return None


def _rewrite_loop(
    instrs: list[asm_ast.Type_instruction], h: int, b: int,
    label_idx: dict[str, int],
) -> list[asm_ast.Type_instruction] | None:
    header = instrs[h].name
    body = range(h, b + 1)
    if not _is_single_entry(instrs, body, header, label_idx):
        return None
    for k, ins in enumerate(instrs):
        if k not in body and _branch_or_jump_target(ins) == header:
            return None
    if h > 0 and isinstance(instrs[h - 1], (
        asm_ast.Jump, asm_ast.Ret, asm_ast.Return, asm_ast.JumpTable,
    )):
        return None
    loads: list[int] = []
    transfers: list[int] = []
    value = None
    for k in body:
        ins = instrs[k]
        if _is_constant_load(ins):
            if value is not None and ins.src != value:
                return None
            value = ins.src
            loads.append(k)
        elif _is_transfer(ins):
            transfers.append(k)
        elif not _leaves_a(ins):
            return None
    if not loads:
        return None
    if not (a_dead_at(instrs, h) and flags_dead_at(instrs, h)):
        return None
    if not all(flags_dead_at(instrs, k + 1) for k in loads):
        return None
    without_loads = [
        ins for k, ins in enumerate(instrs) if k not in set(loads)
    ]
    for k in transfers:
        if not a_dead_at(instrs, k + 1):
            return None
        if flags_dead_at(instrs, k + 1):
            continue
        shift = sum(1 for j in loads if j < k)
        if not _flags_reflect_src_reg(without_loads, k - shift):
            return None
    gone = set(loads) | set(transfers)
    return (
        instrs[:h]
        + [asm_ast.Mov(src=value, dst=asm_ast.Reg(reg=asm_ast.A()))]
        + [ins for k, ins in enumerate(instrs[h:], h) if k not in gone]
    )


def _is_constant_load(ins: asm_ast.Type_instruction) -> bool:
    """True iff `ins` is `LDA #k`, of a number or a label's half."""
    return (isinstance(ins, asm_ast.Mov) and is_reg_a(ins.dst)
            and isinstance(ins.src, (asm_ast.Imm, asm_ast.ImmLabelLow,
                                     asm_ast.ImmLabelHigh)))


def _is_transfer(ins: asm_ast.Type_instruction) -> bool:
    """True iff `ins` is `TXA` or `TYA`."""
    return (isinstance(ins, asm_ast.Mov) and is_reg_a(ins.dst)
            and _is_xy(ins.src))


def _is_xy(op: asm_ast.Type_operand) -> bool:
    return isinstance(op, asm_ast.Reg) and isinstance(
        op.reg, (asm_ast.X, asm_ast.Y),
    )


# Operands that `LDX`, `LDY`, `STX`, `STY`, `INC`, `DEC`, and the
# shifts take directly, with no `LDY #off` and no trip through A.
_DIRECT = (asm_ast.Data, asm_ast.ZP)
_DIRECT_OR_IMM = _DIRECT + (
    asm_ast.Imm, asm_ast.ImmLabelLow, asm_ast.ImmLabelHigh,
)


def _leaves_a(ins: asm_ast.Type_instruction) -> bool:
    """True iff `ins` doesn't write A, as `asm_emit` writes it. Kinds
    not listed here, such as calls, pulls, and the compound nodes,
    count as writing A."""
    if isinstance(ins, (asm_ast.Label, asm_ast.Branch, asm_ast.SetCarry,
                        asm_ast.ClearCarry, asm_ast.BitTest)):
        return True
    if isinstance(ins, asm_ast.Jump):
        return not ins.reg_args
    if isinstance(ins, asm_ast.Compare):
        return is_reg_a(ins.left)
    if isinstance(ins, asm_ast.Push):
        return is_reg_a(ins.src)
    if isinstance(ins, asm_ast.Mov):
        if is_reg_a(ins.src):
            # STA, TAX, TAY.
            return not is_reg_a(ins.dst)
        if _is_xy(ins.dst):
            # LDX, LDY.
            return isinstance(ins.src, _DIRECT_OR_IMM + (asm_ast.IndexedData,))
        if _is_xy(ins.src):
            # STX, STY.
            return isinstance(ins.dst, _DIRECT)
        return False
    if isinstance(ins, (asm_ast.Inc, asm_ast.Dec)):
        return _is_xy(ins.dst) or _read_modify_write(ins.dst)
    if isinstance(ins, (asm_ast.ArithmeticShiftLeft,
                        asm_ast.LogicalShiftRight,
                        asm_ast.RotateLeft, asm_ast.RotateRight)):
        return _read_modify_write(ins.dst)
    return False


def _read_modify_write(op: asm_ast.Type_operand) -> bool:
    """True iff `INC`, `DEC`, and the shifts take `op` directly: a
    byte, or a byte indexed by X (there's no `INC abs,Y`)."""
    if isinstance(op, asm_ast.IndexedData):
        return isinstance(op.index, asm_ast.X)
    return isinstance(op, _DIRECT)
