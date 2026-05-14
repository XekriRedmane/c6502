"""Fold `DEC M / INC M; LDA M; B<cond>` to `DEC M / INC M; B<cond>`.

# Pattern

Three consecutive instructions:

    Dec(M) or Inc(M)        ; sets N/Z based on M's new value
    Mov(M, Reg(A))           ; LDA M — A = M, sets N/Z based on M
    Branch(<cond>, target)   ; B<cond> reads N/Z

→

    Dec(M) or Inc(M)
    Branch(<cond>, target)

# Soundness

`Dec(M)` / `Inc(M)` on the 6502 perform the read-modify-write in
place and set N/Z based on the new value at M. `Mov(M, Reg(A))`
reads that same value and writes it to A, setting N/Z to the
same bits. So the flag state at the Branch is identical whether
or not the LDA is dropped.

The LDA's write of A is the only observable difference. Dropping
is sound when A is dead at the branch's two successor positions
— which is checked with `a_dead_at`.

The branch's condition must only depend on N/Z (so the C and V
flags can carry any value). That excludes BCC / BCS / BVC / BVS;
the allowed set is BEQ / BNE / BMI / BPL.

# Motivating case

Loop tail after a counter decrement:

    DEC counter         ; counter--, set N/Z
    LDA counter         ; A = counter, set N/Z to same thing
    BPL .loop_start     ; loop while counter >= 0

→ `DEC counter; BPL .loop_start`. Saves a 2-byte / 3-cycle LDA
per iteration. The hand-written form `DEX; BPL .loop` would be
shorter still (1-byte DEX), but that requires the counter to
live in X — a regalloc decision beyond this pass's scope.

# Where to run

Inside the asm-peephole fixed-point loop, after the other
arithmetic-shrinking passes have produced the DEC/INC form."""

from __future__ import annotations

import asm_ast
from passes.asm_liveness import a_dead_at


_FLAG_NZ_BRANCHES: tuple[type, ...] = (
    asm_ast.EQ, asm_ast.NE, asm_ast.MI, asm_ast.PL,
)


def apply_dec_inc_branch_fold(prog: asm_ast.Program) -> asm_ast.Program:
    new_top: list[asm_ast.Type_top_level] = []
    for tl in prog.top_level:
        if isinstance(tl, asm_ast.Function):
            new_top.append(_rewrite_function(tl))
        else:
            new_top.append(tl)
    return asm_ast.Program(top_level=new_top)


def _is_reg_a(op) -> bool:
    return isinstance(op, asm_ast.Reg) and isinstance(op.reg, asm_ast.A)


def _stable_mem_eq(a, b) -> bool:
    if isinstance(a, asm_ast.Data) and isinstance(b, asm_ast.Data):
        return a.name == b.name and a.offset == b.offset
    if isinstance(a, asm_ast.ZP) and isinstance(b, asm_ast.ZP):
        return a.address == b.address and a.offset == b.offset
    return False


def _rewrite_function(fn: asm_ast.Function) -> asm_ast.Function:
    instrs = fn.instructions
    branch_targets = _collect_branch_targets(instrs)
    out: list[asm_ast.Type_instruction] = []
    i = 0
    while i < len(instrs):
        match = _try_match(instrs, i, branch_targets)
        if match is not None:
            lda_idx, branch_idx = match
            # Keep everything except the LDA at `lda_idx`.
            for j in range(i, branch_idx + 1):
                if j == lda_idx:
                    continue
                out.append(instrs[j])
            i = branch_idx + 1
            continue
        out.append(instrs[i])
        i += 1
    return asm_ast.Function(
        name=fn.name, is_global=fn.is_global,
        params=list(fn.params), instructions=out,
    )


def _collect_branch_targets(instrs) -> set[str]:
    out: set[str] = set()
    for instr in instrs:
        if isinstance(instr, (asm_ast.Jump, asm_ast.Branch)):
            out.add(instr.target)
    return out


def _try_match(
    instrs, i: int, branch_targets: set[str],
) -> tuple[int, int] | None:
    """If `instrs[i]` is an `Inc(M)`/`Dec(M)` followed (after any
    number of passive Labels — labels with no Jump/Branch incoming)
    by `Mov(M, A)` and a flag-NZ Branch with A dead afterward,
    return `(lda_idx, branch_idx)`. Otherwise None."""
    if not isinstance(instrs[i], (asm_ast.Inc, asm_ast.Dec)):
        return None
    mem = instrs[i].dst
    if not isinstance(mem, (asm_ast.ZP, asm_ast.Data)):
        return None
    j = i + 1
    # Skip passive labels (no incoming branches/jumps).
    while j < len(instrs) and isinstance(instrs[j], asm_ast.Label):
        if instrs[j].name in branch_targets:
            return None
        j += 1
    # Next must be `Mov(M, A)`.
    if j >= len(instrs):
        return None
    lda = instrs[j]
    if not (isinstance(lda, asm_ast.Mov)
            and _is_reg_a(lda.dst)
            and _stable_mem_eq(lda.src, mem)):
        return None
    lda_idx = j
    j += 1
    # Next must be a flag-NZ Branch.
    if j >= len(instrs):
        return None
    br = instrs[j]
    if not (isinstance(br, asm_ast.Branch)
            and isinstance(br.cond, _FLAG_NZ_BRANCHES)):
        return None
    # A must be dead afterward.
    if not a_dead_at(instrs, j + 1):
        return None
    return (lda_idx, j)
