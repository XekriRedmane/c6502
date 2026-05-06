"""SBC-1-fused-with-test-zero peephole.

A typical post-decrement loop test like

    for (uint8_t i = N; i-- > 0; ) { ... }

lowers via TAC's `JumpIfCmp(LessOrEqual, i, 0)` to an asm sequence
that computes `i - 1` via SBC and SEPARATELY tests `i` against 0
via `LDA #0; CMP i`:

    LDA M       ; load i (== M, the iv's storage)
    SEC
    SBC #$01    ; A = i - 1
    Mov(A, dst) ; store new iv (typically TAX or STA tmp)
    LDA #$00
    CMP M       ; test i against 0 (CMP sets C=1 iff 0 >= M, i.e. M==0)
    Branch      ; BCC .skip / JMP .break (long-form), or BCS .break

The SBC already sets C with the inverse information needed for the
test:

  * After SBC: C=1 iff M >= 1; C=0 (borrow) iff M == 0.
  * After CMP-against-0: C=1 iff 0 >= M, i.e. M == 0.

So we can drop the `LDA #0; CMP M` pair and use the SBC's C
directly, **flipping the branch sense** (CMP's "C=1 iff M==0"
maps to SBC's "C=0 iff M==0"). Rewrite rules:

  * `Branch(CC, target)` (skip-break-when-positive long-form
    pattern) → `Branch(CS, target)` (skip-break-when-positive
    using SBC's C).
  * `Branch(CS, target)` (break-when-zero unexpanded form) →
    `Branch(CC, target)` (break-when-zero using SBC's C).
  * Same flipping for the rare `Branch(EQ/NE, target)` post-CMP
    forms (Z is set iff M == 0 after CMP-against-0; SBC sets Z
    iff `M-1 == 0`, i.e. M == 1 — DIFFERENT condition, so we
    don't flip Z-based branches; skip those).

# Eligibility

Pattern (consecutive instructions):

    Mov(M, Reg(A))         # i0: load iv
    SetCarry               # i1
    Sub(Imm(1), Reg(A))    # i2: A = M - 1
    Mov(Reg(A), <dst>)     # i3: stash new value somewhere
    Mov(Imm(0), Reg(A))    # i4: LDA #0
    Compare(Reg(A), M)     # i5: CMP M (same M)
    Branch(CC|CS, target)  # i6

Constraints:
  * `M` in i0 and i5 are the same operand (Data / ZP).
  * `<dst>` in i3 is NOT M (the in-place form is `dec_peephole`'s
    job; skip here).
  * Branch cond is `CC` or `CS` only — the C flag has the inverse
    relationship between CMP-against-0 and SBC, but Z and N don't
    map cleanly.

Replacement: drop i4 (`LDA #0`) and i5 (`CMP M`); flip the Branch
condition. Net: 7 → 5 instructions.

# Where to run

After `replace_pseudoregisters` (so M is concrete) and `inc_peephole`
/ `dec_peephole` (so we know the in-place case isn't here). Before
`expand_long_branches` (the rewrite shrinks code, never grows).
"""

from __future__ import annotations

import asm_ast


_REG_A = asm_ast.Reg(reg=asm_ast.A())


def apply_sub1_test_zero_peephole(prog: asm_ast.Program) -> asm_ast.Program:
    """Walk every Function top-level and rewrite SBC-1-then-test-
    against-zero patterns. `StaticVariable`s and other top-levels
    pass through unchanged."""
    new_top: list[asm_ast.Type_top_level] = []
    for tl in prog.top_level:
        if isinstance(tl, asm_ast.Function):
            new_top.append(_rewrite_function(tl))
        else:
            new_top.append(tl)
    return asm_ast.Program(top_level=new_top)


def _rewrite_function(fn: asm_ast.Function) -> asm_ast.Function:
    instrs = fn.instructions
    out: list[asm_ast.Type_instruction] = []
    i = 0
    while i < len(instrs):
        match = _try_match(instrs, i)
        if match is None:
            out.append(instrs[i])
            i += 1
            continue
        replacement, n_consumed = match
        out.extend(replacement)
        i += n_consumed
    return asm_ast.Function(
        name=fn.name, is_global=fn.is_global,
        params=list(fn.params), instructions=out,
    )


def _try_match(
    instrs: list[asm_ast.Type_instruction], start: int,
) -> tuple[list[asm_ast.Type_instruction], int] | None:
    """Match the 7-instruction pattern at `instrs[start]` and return
    the 4-instruction replacement + number of original instructions
    consumed (always 7 on a match), or None on failure."""
    if start + 7 > len(instrs):
        return None
    i0, i1, i2, i3, i4, i5, i6 = instrs[start:start + 7]
    # i0: Mov(M, Reg(A))
    if not (
        isinstance(i0, asm_ast.Mov)
        and i0.dst == _REG_A
        and _is_compareable_operand(i0.src)
    ):
        return None
    M = i0.src
    # i1: SetCarry
    if not isinstance(i1, asm_ast.SetCarry):
        return None
    # i2: Sub(Imm(1), Reg(A))
    if not (
        isinstance(i2, asm_ast.Sub)
        and i2.src == asm_ast.Imm(value=1)
        and i2.dst == _REG_A
    ):
        return None
    # i3: Mov(Reg(A), <dst>) where dst != M (in-place is dec_peephole's job).
    if not (isinstance(i3, asm_ast.Mov) and i3.src == _REG_A):
        return None
    if _operands_equal(i3.dst, M):
        return None
    # i4: Mov(Imm(0), Reg(A))
    if not (
        isinstance(i4, asm_ast.Mov)
        and i4.src == asm_ast.Imm(value=0)
        and i4.dst == _REG_A
    ):
        return None
    # i5: Compare(Reg(A), M) — same M as i0.src.
    if not (
        isinstance(i5, asm_ast.Compare)
        and i5.left == _REG_A
        and _operands_equal(i5.right, M)
    ):
        return None
    # i6: Branch(CC|CS, target). Other conditions (EQ/NE/MI/PL/
    # VC/VS) don't have the same inverse mapping between CMP-vs-0
    # and SBC's flags; only C flips cleanly.
    if not isinstance(i6, asm_ast.Branch):
        return None
    if isinstance(i6.cond, asm_ast.CC):
        flipped_cond = asm_ast.CS()
    elif isinstance(i6.cond, asm_ast.CS):
        flipped_cond = asm_ast.CC()
    else:
        return None
    target = i6.target
    # Replacement: i0, i1, i2, i3, then Branch(flipped, target).
    replacement = [
        i0, i1, i2, i3,
        asm_ast.Branch(cond=flipped_cond, target=target),
    ]
    return (replacement, 7)


def _is_compareable_operand(op: asm_ast.Type_operand) -> bool:
    """True iff `op` can appear as both a Mov src AND a Compare
    right operand. CMP supports zp / abs / # — we accept Data,
    ZP, and `Pseudo` (so the peephole can fire BEFORE pseudo-
    register replacement, where Pseudo operands haven't yet been
    lowered to concrete ZP / Frame slots). Same operand both
    times is what matters for the peephole's soundness."""
    return isinstance(op, (asm_ast.Data, asm_ast.ZP, asm_ast.Pseudo))


def _operands_equal(
    a: asm_ast.Type_operand, b: asm_ast.Type_operand,
) -> bool:
    if isinstance(a, asm_ast.Data) and isinstance(b, asm_ast.Data):
        return a.name == b.name and a.offset == b.offset
    if isinstance(a, asm_ast.ZP) and isinstance(b, asm_ast.ZP):
        return a.address == b.address and a.offset == b.offset
    if isinstance(a, asm_ast.Pseudo) and isinstance(b, asm_ast.Pseudo):
        return a.name == b.name and a.offset == b.offset
    return False
