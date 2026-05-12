"""Shared `Reg(A)` liveness predicate for asm-level peepholes.

Three peepholes (`direct_index_load`, the SSA-time
`backward_copy_propagation`, the CPX/CPY rewriter) need to ask
"is the current value of `Reg(A)` observable past this point?".
The answer drives whether transferring through A (`LDA M; TAX`,
`LDA M; STA N`, `TXA; CMP K`) can be collapsed: if A's post-
sequence value is never read before being overwritten, the
intermediate-A version is observably equivalent to a direct
version that bypasses A.

# `a_dead_at(instrs, idx)` — CFG-wide forward search

Forward DFS from `idx`. Each instruction either:

  * Reads `Reg(A)` — A is live on this path → return False.
  * Kills `Reg(A)` (writes A without reading it first) — A is
    dead from this point on this path; terminate the path.
  * `Ret` / `Return` — A is dead iff `save_a=False` (a
    `save_a=True` epilogue does `PHA` first, which is a read);
    if False, terminate the path; if True, return False.
  * `Jump(target)` — follow target only.
  * `Branch(_, target)` — follow both fall-through and target.
  * `Label` — pass through (a label is a marker; the walk
    continues on the same path).
  * End of instructions — terminate the path (no observers
    possible).

A's dead at `idx` iff EVERY path terminates without observing a
read. Visited indices are tracked to keep the walk linear in
program size.

The CFG walk catches the common loop-tail shape

    TXA ; CMP K ; Branch cond, t   ; (idx points past Branch)
    ... fall-through path starts with TYA / LDA / similar (kills A)
    t: LDA M / TYA / similar       (kills A on the taken path)

which a within-block walk that bails at the Branch misses.

# Phi handling

Within asm-level SSA, `Phi` nodes may take `Reg(A)` as a source
(rare but possible during regalloc transitions). The conservative
treatment is "Phi reads its sources" — a Phi with `Reg(A)` source
counts as a read.

# Soundness notes

- `LoadAddress` is treated as a kill: its expansion in
  `asm_to_asm2` always writes A through immediates, never
  preserving the prior A value.
- `Call` is treated as a kill: callees clobber A (1-byte returns
  leave the result there; HARGS-returning calls leave A
  unspecified).
- `Pop(dst=A)` is a kill — fresh value from the hw stack.
- The ALU instructions (Add/Sub/And/Or/Xor) RMW A: they read A
  before writing it, so they're a read, not a kill.
- Shift/rotate on A (`ASL A` etc.) likewise RMW A.
"""

from __future__ import annotations

import asm_ast


def is_reg_a(op: asm_ast.Type_operand) -> bool:
    return isinstance(op, asm_ast.Reg) and isinstance(op.reg, asm_ast.A)


def a_dead_at(
    instrs: list[asm_ast.Type_instruction], idx: int,
) -> bool:
    """True iff `Reg(A)`'s current value is dead at `instrs[idx]` —
    every forward path through the instruction stream encounters a
    kill of A before any read of A. Uses a CFG-wide forward DFS to
    handle inter-block paths."""
    label_to_index = _build_label_map(instrs)
    visited: set[int] = set()
    stack: list[int] = [idx]
    while stack:
        j = stack.pop()
        if j in visited:
            continue
        visited.add(j)
        if not _path_dead_from(instrs, j, label_to_index, stack):
            return False
    return True


def _path_dead_from(
    instrs: list[asm_ast.Type_instruction],
    start: int,
    label_to_index: dict[str, int],
    stack: list[int],
) -> bool:
    """Walk forward from `start` along the straight-line successor
    chain. Returns False if this path observes a read of A before
    a kill. Returns True if the path terminates without observing
    one (kill, Ret-with-save_a=False, end of instructions). At
    branches / jumps, push successors onto `stack` and return True
    (the dispatching loop checks each successor)."""
    idx = start
    while idx < len(instrs):
        instr = instrs[idx]
        if isinstance(instr, (asm_ast.Ret, asm_ast.Return)):
            return not instr.save_a
        if isinstance(instr, asm_ast.Jump):
            tgt = label_to_index.get(instr.target)
            if tgt is not None:
                stack.append(tgt)
            return True
        if isinstance(instr, asm_ast.Branch):
            if idx + 1 < len(instrs):
                stack.append(idx + 1)
            tgt = label_to_index.get(instr.target)
            if tgt is not None:
                stack.append(tgt)
            return True
        if isinstance(instr, asm_ast.Label):
            idx += 1
            continue
        if reads_a(instr):
            return False
        if kills_a(instr):
            return True
        idx += 1
    return True


def _build_label_map(
    instrs: list[asm_ast.Type_instruction],
) -> dict[str, int]:
    out: dict[str, int] = {}
    for i, ins in enumerate(instrs):
        if isinstance(ins, asm_ast.Label):
            out[ins.name] = i
    return out


def reads_a(instr: asm_ast.Type_instruction) -> bool:
    """True iff `instr` reads `Reg(A)` (uses it as a source or
    read-modify-writes via `dst=A`)."""
    if isinstance(instr, asm_ast.Mov):
        return is_reg_a(instr.src)
    if isinstance(instr, asm_ast.Push):
        return is_reg_a(instr.src)
    if isinstance(instr, (
        asm_ast.Add, asm_ast.Sub, asm_ast.And, asm_ast.Or,
    )):
        return is_reg_a(instr.src) or is_reg_a(instr.dst)
    if isinstance(instr, asm_ast.Xor):
        return (
            is_reg_a(instr.src1) or is_reg_a(instr.src2)
            or is_reg_a(instr.dst)
        )
    if isinstance(instr, asm_ast.Compare):
        return is_reg_a(instr.left) or is_reg_a(instr.right)
    if isinstance(instr, (
        asm_ast.Inc, asm_ast.Dec,
        asm_ast.ArithmeticShiftLeft, asm_ast.LogicalShiftRight,
        asm_ast.RotateLeft, asm_ast.RotateRight,
    )):
        return is_reg_a(instr.dst)
    if isinstance(instr, asm_ast.Phi):
        return any(is_reg_a(a.source) for a in instr.args)
    return False


def kills_a(instr: asm_ast.Type_instruction) -> bool:
    """True iff `instr` writes `Reg(A)` without reading it first."""
    if isinstance(instr, asm_ast.Mov):
        if is_reg_a(instr.dst) and not is_reg_a(instr.src):
            return True
    if isinstance(instr, asm_ast.Pop):
        if is_reg_a(instr.dst):
            return True
    if isinstance(instr, asm_ast.Call):
        return True
    if isinstance(instr, asm_ast.LoadAddress):
        return True
    return False
