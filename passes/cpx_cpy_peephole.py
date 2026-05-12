"""Direct CPX / CPY peephole.

`tac_to_asm`'s comparison lowerings stage every left operand into
`Reg(A)` before the `Compare` atom, because comparisons against
`Stack` / `Frame` / `Indirect` operands need `CMP (zp),Y` which
only `CMP` supports (`CPX` / `CPY` have no indirect-Y addressing
mode). For comparisons whose left operand is already in `Reg(X)`
or `Reg(Y)` — typically the loop-induction-variable check shape

    Mov(Reg(X), Reg(A))         ; TXA
    Compare(Reg(A), R)          ; CMP imm/abs/zp
    Branch(cond, target)        ; BCC / BCS / BEQ / BNE / ...

— the TXA round trip is unnecessary when `R` is an addressing
mode `CPX` supports (immediate, Data, ZP) AND `Reg(A)`'s value
isn't observed after the Compare. In that case we rewrite to

    Compare(Reg(X), R)          ; CPX imm/abs/zp
    Branch(cond, target)

saving 1 byte / 2 cycles per occurrence. Same shape for `TYA;
CMP; ...` → `CPY ...`.

# Flag soundness

`TXA; CMP R` first sets N/Z based on X (TXA), then overwrites
N/Z/C with the comparison result (CMP). The final flag state is
"CMP X vs R", which is exactly what `CPX R` produces. So any
subsequent `Branch` sees the same condition.

# `Reg(A)` liveness

After the rewrite, A keeps its prior value (the TXA in the
original would have overwritten it with X's value). For
soundness we require A to be dead after the Compare — no
instruction reads A before redefining it. The shared
`asm_liveness.a_dead_at` helper handles the forward scan with
the same rules `direct_index_load` and `backward_copy_propagation`
use.

# Right-operand eligibility

`CPX` / `CPY` support immediate, absolute, and zero-page
addressing — `Imm` / `Data` / `ZP` operands all work. `Stack` /
`Frame` / `Indirect` / `IndirectY` need indirect-Y addressing,
which CPX / CPY don't have; those cases must stay routed through
`CMP` via Reg(A).

# Where to run

After `replace_pseudoregisters` (so operands are concrete) and
inside `_peephole_fixedpoint`. The pass shrinks code, never
grows.
"""

from __future__ import annotations

import asm_ast
from passes.asm_liveness import a_dead_at, is_reg_a


def apply_cpx_cpy_peephole(prog: asm_ast.Program) -> asm_ast.Program:
    new_top: list[asm_ast.Type_top_level] = []
    for tl in prog.top_level:
        if isinstance(tl, asm_ast.Function):
            new_top.append(_rewrite_function(tl))
        else:
            new_top.append(tl)
    return asm_ast.Program(top_level=new_top)


def _rewrite_function(fn: asm_ast.Function) -> asm_ast.Function:
    """Two-pass: pass 1 records the fusion at the second
    instruction's index (the Compare); pass 2 rebuilds, dropping
    the first instruction (the TXA / TYA) and replacing the
    Compare."""
    instrs = fn.instructions
    rewrites: dict[int, asm_ast.Type_instruction] = {}
    drop_first: set[int] = set()
    for i in range(len(instrs) - 1):
        first = instrs[i]
        second = instrs[i + 1]
        fused = _try_fuse(first, second, instrs, i + 2)
        if fused is not None:
            drop_first.add(i)
            rewrites[i + 1] = fused
    out: list[asm_ast.Type_instruction] = []
    for i, instr in enumerate(instrs):
        if i in drop_first:
            continue
        out.append(rewrites.get(i, instr))
    return asm_ast.Function(
        name=fn.name, is_global=fn.is_global,
        params=list(fn.params), instructions=out,
    )


def _try_fuse(
    first: asm_ast.Type_instruction,
    second: asm_ast.Type_instruction,
    instrs: list[asm_ast.Type_instruction],
    after_idx: int,
) -> asm_ast.Type_instruction | None:
    """If `first` is `Mov(Reg(X|Y), Reg(A))` and `second` is
    `Compare(Reg(A), R)` with R ∈ Imm/Data/ZP, and `Reg(A)` is
    dead at `after_idx`, return the rewritten `Compare(Reg(X|Y),
    R)`. Otherwise None."""
    if not isinstance(first, asm_ast.Mov):
        return None
    if not is_reg_a(first.dst):
        return None
    if not _is_reg_x_or_y(first.src):
        return None
    if not isinstance(second, asm_ast.Compare):
        return None
    if not is_reg_a(second.left):
        return None
    if not isinstance(
        second.right, (asm_ast.Imm, asm_ast.Data, asm_ast.ZP),
    ):
        return None
    if not a_dead_at(instrs, after_idx):
        return None
    return asm_ast.Compare(left=first.src, right=second.right)


def _is_reg_x_or_y(op: asm_ast.Type_operand) -> bool:
    return (
        isinstance(op, asm_ast.Reg)
        and isinstance(op.reg, (asm_ast.X, asm_ast.Y))
    )
