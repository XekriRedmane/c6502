"""TAC pass: fold `Binary(BitwiseAnd, x, ConstInt(C), %res);
JumpIfTrue/False(%res, t)` (with single-use `%res`) into a single
`JumpIfMasked` when the operand can be narrowed to 1 byte.

# Motivating idiom

C99 `if (uchar_var & 0x80)` lowers through c99_to_tac as:

    ZeroExtend(uchar_var, %ext)               # uchar promoted to int
    Binary(BitwiseAnd, %ext, ConstInt(0x80), %res)
    JumpIfFalse(%res, .if_end)

Without this pass, `tac_to_asm` lowers the BitwiseAnd as a 2-byte
AND chain (writing both low and high bytes of `%res`) followed by
a JumpIfFalse that ORs the bytes and branches on Z. After the
existing const-prop / dead-store / redundant-load peepholes try
their best, what's typically left is:

    LDA uchar      ; AND #$80
    STA %res.lo     ; LDA #$00 ; STA %res.hi
    LDA %res.lo     ; BEQ .if_end

The `STA %res.hi` is a write of a constant zero into a slot
nothing reads (the test only consults the low byte). The
`LDA %res.lo` reload after `STA %res.lo` is the same byte we
just stored. Both should be DCE-able in principle, but the asm-
level passes don't catch them in every shape.

# What this pass does

When the AND operand traces back to a 1-byte unsigned value (via
`ZeroExtend(narrow, %ext)`) and the other operand is an integer
Constant fitting in 0..255 (with a single-use AND result), we
collapse the pair to:

    JumpIfMasked(narrow, mask=C, jump_when_nonzero=..., target)

which `tac_to_asm` lowers as:

    LDA narrow ; AND #C ; B(EQ|NE) target

— 3 instructions, no intermediate stores, no high-byte zero
stage. With `C == 0x80` the existing `and_sign_bit_branch` asm
peephole then folds `LDA; AND #$80; B(EQ|NE)` to
`LDA; B(PL|MI)`, leaving 2 instructions.

# Sense flip

JumpIfMasked encodes its sense via `jump_when_nonzero`:
  * `JumpIfTrue(%res, t)`  ⟹  `JumpIfMasked(..., jump_when_nonzero=True)`
    (jump iff `(narrow & C) != 0`)
  * `JumpIfFalse(%res, t)` ⟹  `JumpIfMasked(..., jump_when_nonzero=False)`
    (jump iff `(narrow & C) == 0`)

# Soundness

`(uchar)x & 0x80` always produces a value in `{0, 0x80}`. After
ZeroExtend the value is still in `{0, 0x80}` (the upper byte is
0). The widened AND `(int)x_ext & 0x80` is the same value.
JumpIfFalse fires iff this is zero; JumpIfTrue fires iff nonzero.

A 1-byte AND `narrow & 0x80` produces the same `{0, 0x80}`. The
1-byte branch on the result tests the same predicate. So the fold
is a pure type-narrowing of an operation whose semantics don't
depend on the extra width.

Generalizes to any mask fitting in 0..255: every bit position
beyond bit 7 of the narrow source is statically zero (because the
source is 1 byte unsigned), so a mask bit above bit 7 contributes
nothing. The pass requires `0 <= C <= 0xFF` so the mask matches
the narrowed AND's byte width exactly.

# Where it runs

In the SSA-bracketed fixed-point loop, alongside
`fold_cmp_zero_jump`. The two are complementary: cmp-zero handles
the `(uchar < N)` / `(uchar == 0)` shapes; this handles the
`(uchar & mask)` shape. Both rely on the same single-use SSA
invariant + ZeroExtend tracing helpers.

Strict adjacency: `Binary` immediately followed by the `JumpIf*`.
The c99_to_tac shapes for `if`, `while`, `?:` produce that exact
adjacency; non-adjacent cases would need copy-prop / DSE to
collapse the gap first, which the fixed-point loop handles in
subsequent rounds.
"""
from __future__ import annotations

import c99_ast
import tac_ast
from passes.optimization.cmp_zero_jump_fold import (
    _count_var_uses,
    _index_var_defs,
    _NARROW_UNSIGNED_TYPES,
)


def fold_narrow_and_jump(
    fn: tac_ast.Function,
    *,
    symbols=None,
) -> tac_ast.Function:
    """Walk `fn.instructions`, find adjacent
    `Binary(BitwiseAnd, ...); JumpIfTrue/False` pairs with
    single-use cond, and rewrite to `JumpIfMasked` when one
    operand traces to a 1-byte unsigned value and the other is a
    fitting integer constant. The `symbols` table is required for
    the narrowing path; without it the pass is a no-op."""
    if symbols is None:
        return fn
    use_count = _count_var_uses(fn)
    var_def_idx = _index_var_defs(fn)

    new_instrs: list[tac_ast.Type_instruction] = []
    skip_next = False
    for i, instr in enumerate(fn.instructions):
        if skip_next:
            skip_next = False
            continue
        rewrite = _try_fold(
            fn.instructions, i, use_count, var_def_idx, symbols,
        )
        if rewrite is None:
            new_instrs.append(instr)
            continue
        new_instrs.append(rewrite)
        skip_next = True
    return tac_ast.Function(
        name=fn.name,
        is_global=fn.is_global,
        params=list(fn.params),
        instructions=new_instrs,
    )


def _try_fold(
    instrs: list[tac_ast.Type_instruction],
    i: int,
    use_count: dict[str, int],
    var_def_idx: dict[str, int],
    symbols,
) -> tac_ast.Type_instruction | None:
    """Test whether `instrs[i:i+2]` matches the narrow-and-jump
    pattern. Returns the rewritten `JumpIfMasked` or None."""
    if i + 1 >= len(instrs):
        return None
    binop = instrs[i]
    if not isinstance(binop, tac_ast.Binary):
        return None
    if not isinstance(binop.op, tac_ast.BitwiseAnd):
        return None
    if not isinstance(binop.dst, tac_ast.Var):
        return None
    jumpif = instrs[i + 1]
    if not isinstance(jumpif, (tac_ast.JumpIfTrue, tac_ast.JumpIfFalse)):
        return None
    if not isinstance(jumpif.condition, tac_ast.Var):
        return None
    if jumpif.condition.name != binop.dst.name:
        return None
    if use_count.get(binop.dst.name, 0) != 1:
        return None

    # AND is commutative — try (narrow_arg, const_arg) in either
    # order. Exactly one operand must be a Constant in 0..255; the
    # other must trace through ZeroExtend to a 1-byte unsigned Var.
    narrow_arg = _try_narrow_pair(
        binop.src1, binop.src2, instrs, var_def_idx, use_count, symbols,
    )
    if narrow_arg is None:
        narrow_arg = _try_narrow_pair(
            binop.src2, binop.src1, instrs, var_def_idx, use_count, symbols,
        )
    if narrow_arg is None:
        return None
    narrow_val, mask = narrow_arg
    return tac_ast.JumpIfMasked(
        val=narrow_val,
        mask=mask,
        jump_when_nonzero=isinstance(jumpif, tac_ast.JumpIfTrue),
        target=jumpif.target,
    )


def _try_narrow_pair(
    maybe_narrow: tac_ast.Type_val,
    maybe_const: tac_ast.Type_val,
    instrs: list[tac_ast.Type_instruction],
    var_def_idx: dict[str, int],
    use_count: dict[str, int],
    symbols,
) -> tuple[tac_ast.Type_val, int] | None:
    """Check whether `(maybe_narrow, maybe_const)` matches
    `(narrow_traced_through_zero_extend, ConstInt(C) with C ≤ 0xFF)`.
    Returns `(narrow_source_val, mask_int)` on success, else None."""
    if not isinstance(maybe_const, tac_ast.Constant):
        return None
    c = maybe_const.const
    # Only integer-typed constants — FP makes no sense for bitwise
    # masks.
    if not isinstance(c, (
        tac_ast.ConstInt, tac_ast.ConstUInt,
        tac_ast.ConstLong, tac_ast.ConstULong,
        tac_ast.ConstLongLong, tac_ast.ConstULongLong,
        tac_ast.ConstChar, tac_ast.ConstUChar,
    )):
        return None
    if not (0 <= c.value <= 0xFF):
        return None
    if not isinstance(maybe_narrow, tac_ast.Var):
        return None
    if use_count.get(maybe_narrow.name, 0) != 1:
        return None
    def_idx = var_def_idx.get(maybe_narrow.name)
    if def_idx is None:
        return None
    defining = instrs[def_idx]
    if not isinstance(defining, tac_ast.ZeroExtend):
        return None
    src = defining.src
    if not isinstance(src, tac_ast.Var):
        return None
    sym = symbols.get(src.name)
    if sym is None:
        return None
    if not isinstance(sym.type, _NARROW_UNSIGNED_TYPES):
        return None
    return src, c.value
