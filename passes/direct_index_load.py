"""Direct-into-X/Y peephole.

`tac_to_asm` always stages a value into `Reg(X)` or `Reg(Y)` via
`Reg(A)`:

    Mov(M, Reg(A))            ; LDA M
    Mov(Reg(A), Reg(X))       ; TAX

This is conservatively right at lowering time — `M` is still a
`Pseudo` and could resolve to `Frame` / `Stack` / `Indirect`,
which use indirect-Y addressing. `LDX` / `LDY` don't support
`(ind),Y`, so a direct load wouldn't work for those operands.

After `replace_pseudoregisters` resolves Pseudos to concrete
operand types, we can short-circuit the round trip when `M` is
addressable by `LDX` / `LDY` directly:

  * `Imm`   — `LDX #imm`   (2 bytes, same as `LDA #imm; TAX` but
                            without the TAX).
  * `Data`  — `LDX abs`    (3 bytes / 4 cycles vs `LDA abs; TAX`
                            = 4 bytes / 6 cycles).
  * `ZP`    — `LDX zp`     (2 bytes / 3 cycles vs `LDA zp; TAX`
                            = 3 bytes / 5 cycles).

Saves 1 byte / 2 cycles per occurrence.

# Eligibility

The fusion fires when:

  * Two consecutive instructions match the pattern
    `Mov(src=M, dst=Reg(A)); Mov(src=Reg(A), dst=Reg(X|Y))`.
  * `M` is one of `Data` / `ZP` / `Imm` — the addressing modes
    `LDX` / `LDY` support directly.
  * `Reg(A)` is dead immediately after the second `Mov`. If the
    next instruction reads A (or could observe A's value before
    a redefinition), we'd be losing the load.

# Flag soundness

`LDA M; TAX` sets N/Z twice — first based on M's value (LDA),
then again based on the transferred value (TAX), which IS M's
value. The combined N/Z state is exactly "based on M's value".

After the rewrite, `LDX M` sets N/Z based on M's value. Same
state. So the rewrite preserves the flags any subsequent
`Branch` would observe.

# Where to run

After `replace_pseudoregisters` (Pseudos are resolved, so we can
recognize Data / ZP) and before `expand_long_branches` (no new
branches are introduced — the pass shrinks code, never expands —
so order with that pass doesn't strictly matter; we go before
to keep the pass ordering symmetric with `inc_peephole`).
"""

from __future__ import annotations

import asm_ast


def apply_direct_index_load(prog: asm_ast.Program) -> asm_ast.Program:
    """Walk every Function top-level and apply the fusion. Other
    top-levels (`StaticVariable`) pass through unchanged."""
    new_top: list[asm_ast.Type_top_level] = []
    for tl in prog.top_level:
        if isinstance(tl, asm_ast.Function):
            new_top.append(_rewrite_function(tl))
        else:
            new_top.append(tl)
    return asm_ast.Program(top_level=new_top)


def _rewrite_function(fn: asm_ast.Function) -> asm_ast.Function:
    """Two-pass: pass 1 records (rewrite, drop) for each fusable
    pair; pass 2 rebuilds the instruction list. Single-pass would
    work too here (the rewrite happens at the FIRST instruction's
    position, not the second), but the two-pass shape mirrors the
    other peepholes and is robust if a future extension extends
    the matched window."""
    instrs = fn.instructions
    rewrites: dict[int, asm_ast.Type_instruction] = {}
    skipped: set[int] = set()
    for i in range(len(instrs) - 1):
        if i in skipped:
            continue
        first = instrs[i]
        second = instrs[i + 1]
        fused = _try_fuse(first, second, instrs, i + 2)
        if fused is not None:
            rewrites[i] = fused
            skipped.add(i + 1)
    out: list[asm_ast.Type_instruction] = []
    for i, instr in enumerate(instrs):
        if i in skipped:
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
    """If `first` is `Mov(M, Reg(A))` with M ∈ Data/ZP/Imm, and
    `second` is `Mov(Reg(A), Reg(X|Y))`, and Reg(A)'s value is
    dead at `after_idx`, return the fused `Mov(M, Reg(X|Y))`.
    Otherwise None."""
    if not isinstance(first, asm_ast.Mov):
        return None
    if not _is_reg_a(first.dst):
        return None
    if not isinstance(
        first.src, (asm_ast.Data, asm_ast.ZP, asm_ast.Imm),
    ):
        return None
    if not isinstance(second, asm_ast.Mov):
        return None
    if not _is_reg_a(second.src):
        return None
    if not _is_reg_x_or_y(second.dst):
        return None
    if not _a_dead_at(instrs, after_idx):
        return None
    return asm_ast.Mov(src=first.src, dst=second.dst)


def _is_reg_a(op: asm_ast.Type_operand) -> bool:
    return isinstance(op, asm_ast.Reg) and isinstance(op.reg, asm_ast.A)


def _is_reg_x_or_y(op: asm_ast.Type_operand) -> bool:
    return (
        isinstance(op, asm_ast.Reg)
        and isinstance(op.reg, (asm_ast.X, asm_ast.Y))
    )


def _a_dead_at(
    instrs: list[asm_ast.Type_instruction], idx: int,
) -> bool:
    """True iff `Reg(A)`'s value at position `idx` is dead — every
    forward path through the instruction stream encounters a
    write-without-read of A before any read of A.

    Conservative within a single basic block: stops at any
    intra-block control-flow boundary (`Branch` / `Jump` / `Label`)
    and returns False (we don't track inter-block A liveness).
    Mirrors `backward_copy_propagation._a_dead_at`."""
    while idx < len(instrs):
        instr = instrs[idx]
        if isinstance(instr, (asm_ast.Ret, asm_ast.Return)):
            return not instr.save_a
        if isinstance(instr, (asm_ast.Label, asm_ast.Jump, asm_ast.Branch)):
            return False
        if _reads_a(instr):
            return False
        if _kills_a(instr):
            return True
        idx += 1
    return True


def _reads_a(instr: asm_ast.Type_instruction) -> bool:
    """True iff `instr` reads `Reg(A)` (uses it as a source or
    read-modify-writes via `dst=A`)."""
    if isinstance(instr, asm_ast.Mov):
        return _is_reg_a(instr.src)
    if isinstance(instr, asm_ast.Push):
        return _is_reg_a(instr.src)
    if isinstance(instr, (
        asm_ast.Add, asm_ast.Sub, asm_ast.And, asm_ast.Or,
    )):
        return _is_reg_a(instr.src) or _is_reg_a(instr.dst)
    if isinstance(instr, asm_ast.Xor):
        return (
            _is_reg_a(instr.src1) or _is_reg_a(instr.src2)
            or _is_reg_a(instr.dst)
        )
    if isinstance(instr, asm_ast.Compare):
        return _is_reg_a(instr.left) or _is_reg_a(instr.right)
    if isinstance(instr, (
        asm_ast.Inc, asm_ast.Dec,
        asm_ast.ArithmeticShiftLeft, asm_ast.LogicalShiftRight,
        asm_ast.RotateLeft, asm_ast.RotateRight,
    )):
        return _is_reg_a(instr.dst)
    return False


def _kills_a(instr: asm_ast.Type_instruction) -> bool:
    """True iff `instr` writes `Reg(A)` without reading it first."""
    if isinstance(instr, asm_ast.Mov):
        if _is_reg_a(instr.dst) and not _is_reg_a(instr.src):
            return True
    if isinstance(instr, asm_ast.Pop):
        if _is_reg_a(instr.dst):
            return True
    if isinstance(instr, asm_ast.Call):
        # Callees clobber A — 1-byte returns leave the result
        # there, HARGS-returning calls leave A undefined.
        return True
    if isinstance(instr, asm_ast.LoadAddress):
        # The compound expansion routes A through immediates.
        return True
    return False
