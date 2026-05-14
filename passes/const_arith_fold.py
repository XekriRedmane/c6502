"""Asm-level constant-arithmetic folding for adjacent `LDA #imm`
followed by an `AND` / `ORA` against another immediate.

# Motivating case

When a `uchar` value is bit-tested against a small literal at the C
level, type promotion lifts both sides to `int` (per C99
§6.3.1.1.2). The high-byte result of the bitwise op then has the
shape `0 & 0`, `0 | 0`, etc. — i.e. a constant whose value is
trivially zero or unchanged. `tac_to_asm` emits a literal
`LDA #$00; AND #$00; STA <high>` pair for each such high byte.
This pass folds the pair to a single `LDA #$00; STA <high>`,
shrinking each occurrence by 2 bytes and 2 cycles.

# Pattern and equivalence

    Mov(Imm(c1), A)        # LDA #c1     ; A = c1, flags from c1
    <BinOp>(Imm(c2), A)    # AND/ORA #c2  ; A = c1 OP c2, flags from result

→

    Mov(Imm(c1 OP c2), A)  # LDA #(c1 OP c2) ; A = result, flags from result

The replacement preserves A's value AND the N/Z flag state — `LDA
#k` sets N=bit7(k) and Z=(k==0), which is exactly what the
original two-instruction sequence left behind. So the rewrite is
sound even when a subsequent instruction reads either A or the
flags. No liveness check needed.

# Identity drop after an A-writer

A second pattern handles dropping a no-op `Or(Imm(0), A)` or
`And(Imm(0xFF), A)` after any instruction that just wrote A:

    <writes A, sets N/Z from A's new value>   # e.g. `LDA M`, `AND #x`, ...
    Or(Imm(0), A)                              # A | 0 = A — no-op
or  And(Imm(0xFF), A)                          # A & $FF = A — no-op

→ drop the second instruction.

A's value is unchanged by the identity op, and the N/Z flags set
by the preceding write match what the identity op would have left
(both reflect A's value's sign-and-zeroness). C/V are unaffected
by AND/ORA, so they're also identical between the two forms. Any
subsequent reader of A or the flags sees the same state. The
"writes A" predicate covers every instruction whose dst is
`Reg(A)` — that's `Mov` (LDA / TXA / TYA) plus the post-fold
A-arithmetic atoms (`Add` / `Sub` / `And` / `Or`).

# Scope and ordering

Only handles `Reg(A)` as dst and `Imm` as both srcs — the common
shape `tac_to_asm` emits. `Add` / `Sub` are deliberately excluded:
they read the carry flag, which a preceding `LDA` doesn't set, so
their result depends on prior context that this pass can't see
locally. `Xor` is excluded because its IR shape differs (`src1`,
`src2`, `dst` rather than the `src`/`dst` shape `And`/`Or` use).

Runs inside the asm-peephole fixed-point loop. Shrinks code (two
atoms → one), so the loop's monotone-decreasing invariant holds.
After `replace_pseudoregisters` (operands concrete) and before
`expand_long_branches` (no branches involved here, but the
position is consistent with other peepholes)."""

from __future__ import annotations

import asm_ast


def apply_const_arith_fold(prog: asm_ast.Program) -> asm_ast.Program:
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
    while i + 1 < len(instrs):
        a, b = instrs[i], instrs[i + 1]
        folded = _try_fold(a, b)
        if folded is not None:
            out.append(folded)
            i += 2
            continue
        if _is_identity_after_a_write(a, b):
            # Drop `b` (the identity op); `a` stays.
            out.append(a)
            i += 2
            continue
        absorbed = _try_absorb_zero_load(a, b)
        if absorbed is not None:
            out.append(absorbed)
            i += 2
            continue
        out.append(a)
        i += 1
    while i < len(instrs):
        out.append(instrs[i])
        i += 1
    return asm_ast.Function(
        name=fn.name, is_global=fn.is_global,
        params=list(fn.params), instructions=out,
    )


def _try_absorb_zero_load(a, b):
    """Fold `Mov(Imm(0), A); Or(M, A)` → `Mov(M, A)`, and
    `Mov(Imm(0xFF), A); And(M, A)` → `Mov(M, A)`. The combined
    semantics are A = M, flags = bit7(M) / (M==0) — identical to a
    plain LDA M. Returns the replacement instruction or None."""
    if not (isinstance(a, asm_ast.Mov)
            and isinstance(a.src, asm_ast.Imm)
            and _is_reg_a(a.dst)):
        return None
    c1 = a.src.value & 0xFF
    if isinstance(b, asm_ast.Or) and _is_reg_a(b.dst) and c1 == 0:
        return asm_ast.Mov(src=b.src, dst=a.dst)
    if isinstance(b, asm_ast.And) and _is_reg_a(b.dst) and c1 == 0xFF:
        return asm_ast.Mov(src=b.src, dst=a.dst)
    return None


def _try_fold(
    a: asm_ast.Type_instruction, b: asm_ast.Type_instruction,
) -> asm_ast.Type_instruction | None:
    """If (a, b) is `Mov(Imm, A); <And|Or>(Imm, A)`, return the
    folded `Mov(Imm(c1 OP c2), A)`. Otherwise None."""
    if not (isinstance(a, asm_ast.Mov)
            and isinstance(a.src, asm_ast.Imm)
            and _is_reg_a(a.dst)):
        return None
    if not (isinstance(b, (asm_ast.And, asm_ast.Or))
            and isinstance(b.src, asm_ast.Imm)
            and _is_reg_a(b.dst)):
        return None
    c1 = a.src.value & 0xFF
    c2 = b.src.value & 0xFF
    if isinstance(b, asm_ast.And):
        result = c1 & c2
    else:
        result = c1 | c2
    return asm_ast.Mov(src=asm_ast.Imm(value=result), dst=a.dst)


def _is_reg_a(op: asm_ast.Type_operand) -> bool:
    return isinstance(op, asm_ast.Reg) and isinstance(op.reg, asm_ast.A)


def _is_identity_after_a_write(
    a: asm_ast.Type_instruction, b: asm_ast.Type_instruction,
) -> bool:
    """True iff `b` is a no-op bitwise op on A AND `a` is an
    instruction that wrote A AND left A's N/Z bits in the flags.

    `b` patterns: `Or(Imm(0), A)`, `And(Imm(0xFF), A)`.

    `a` predicates: `dst == Reg(A)` for any Mov / And / Or / Add /
    Sub / Xor. (Xor's IR shape is `src1, src2, dst` — handle the
    `dst` check uniformly.)"""
    if not (isinstance(b, (asm_ast.And, asm_ast.Or))
            and isinstance(b.src, asm_ast.Imm)
            and _is_reg_a(b.dst)):
        return False
    val = b.src.value & 0xFF
    if isinstance(b, asm_ast.Or) and val != 0:
        return False
    if isinstance(b, asm_ast.And) and val != 0xFF:
        return False
    # Does `a` write Reg(A)?
    if isinstance(a, (asm_ast.Mov, asm_ast.And, asm_ast.Or,
                       asm_ast.Add, asm_ast.Sub)):
        return _is_reg_a(a.dst)
    if isinstance(a, asm_ast.Xor):
        return _is_reg_a(a.dst)
    return False
