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

# A wide value with a mask in one byte

`if (zchars & 0x8000)` on a 16-bit `zchars` has no extend to trace.
When the mask's set bits all fall in one byte `k` of the value (here
`0x80 << 8`), only that byte of the AND can be nonzero, so the test
is of that byte:

    Binary(RightShift, zchars, 8 * k, %s)   # the byte moved down
    Truncate(%s, %b)                         # and taken alone
    JumpIfMasked(%b, mask=0x80, ...)

`tac_to_asm` lowers the shift by a whole byte as a byte move, and the
asm peepholes leave `LDA zchars+1; BPL` (or `BIT zchars+1; BPL`)
where the 16-bit AND stored both bytes and ORed them together. The
value may be any integer variable of 2 or more bytes, signed or not:
byte `k` is the same either way. `%s` and `%b` are new temporaries,
named after the AND's result.

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
    _NARROW_UNSIGNED_TYPES, extend_source_unchanged,
)
from passes.optimization.framework import (
    RuleSet, Rule, producer_then_jump, single_use, have_symbols,
    PassContext, MatchResult, RuleEnv,
    m_Binary, m_Var,
)


def _build_narrow_and_jump(
    m: MatchResult, env: RuleEnv, ctx: PassContext,
) -> list | None:
    """Fold `Binary(BitwiseAnd, ...); JumpIf{True,False}(%res, T)` to a
    single `JumpIfMasked` when one operand traces back (through a
    ZeroExtend) to a 1-byte unsigned value and the other is an integer
    constant fitting in 0..255. AND is commutative, so try both
    operand orders."""
    use_count, var_def_idx, instrs = env.use_count, env.def_idx, env.instructions
    binop = m.bindings['binop']
    jmp = m.bindings['jmp']
    # The AND's own position, where the narrowed source is read in
    # place of the extend's dst.
    use_idx = var_def_idx[m.bindings['and_dst'].name]
    if instrs[use_idx] is not binop:
        # A non-SSA AND dst written again later. With -1, only an
        # SSA-renamed extend source qualifies.
        use_idx = -1

    narrow_arg = _try_narrow_pair(
        binop.src1, binop.src2, instrs, var_def_idx, use_count, ctx.symbols,
        use_idx, ctx.ssa_dsts,
    )
    if narrow_arg is None:
        narrow_arg = _try_narrow_pair(
            binop.src2, binop.src1, instrs, var_def_idx, use_count, ctx.symbols,
            use_idx, ctx.ssa_dsts,
        )
    if narrow_arg is None:
        wide = _try_wide_byte(binop, ctx)
        if wide is None:
            return None
        return wide + [tac_ast.JumpIfMasked(
            val=wide[-1].dst,
            mask=_one_byte_mask(binop)[2],
            jump_when_nonzero=isinstance(jmp, tac_ast.JumpIfTrue),
            target=jmp.target,
        )]
    narrow_val, mask = narrow_arg
    return [tac_ast.JumpIfMasked(
        val=narrow_val,
        mask=mask,
        jump_when_nonzero=isinstance(jmp, tac_ast.JumpIfTrue),
        target=jmp.target,
    )]


AND_ZERO_RULE = Rule(
    name="fold_narrow_and_jump",
    pattern=producer_then_jump(
        m_Binary(op=tac_ast.BitwiseAnd, dst=m_Var(capture='and_dst'), capture='binop'),
        on='and_dst',
    ),
    where=[single_use('and_dst'), have_symbols()],
    build=_build_narrow_and_jump,
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
    return RuleSet(AND_ZERO_RULE).run(fn, PassContext(symbols=symbols))


_INT_CONSTS = (
    tac_ast.ConstInt, tac_ast.ConstUInt, tac_ast.ConstLong,
    tac_ast.ConstULong, tac_ast.ConstLongLong, tac_ast.ConstULongLong,
)


def _one_byte_mask(binop):
    """`(value, k, mask_byte)` when one operand of the AND is a Var
    and the other an integer constant whose set bits all fall in byte
    `k`, else None."""
    for val, const in ((binop.src1, binop.src2), (binop.src2, binop.src1)):
        if not (isinstance(val, tac_ast.Var)
                and isinstance(const, tac_ast.Constant)
                and isinstance(const.const, _INT_CONSTS)):
            continue
        c = const.const.value
        if c <= 0:
            continue
        k = (c.bit_length() - 1) // 8
        if c >> (8 * k) > 0xFF or c & ((1 << (8 * k)) - 1):
            continue
        return val, k, c >> (8 * k)
    return None


def _try_wide_byte(binop, ctx: PassContext) -> list | None:
    """The instructions that put byte `k` of the AND's wide operand in
    a 1-byte temporary, for "A wide value with a mask in one byte",
    or None. The last one's dst is the temporary."""
    from passes.type_checking import LocalAttr, Symbol
    found = _one_byte_mask(binop)
    if found is None or ctx.symbols is None:
        return None
    val, k, _mask = found
    sym = ctx.symbols.get(val.name)
    if sym is None or not isinstance(
        sym.type, (c99_ast.Int, c99_ast.UInt, c99_ast.Long, c99_ast.ULong,
                   c99_ast.LongLong, c99_ast.ULongLong),
    ):
        return None
    size = {c99_ast.Int: 2, c99_ast.UInt: 2, c99_ast.Long: 4,
            c99_ast.ULong: 4, c99_ast.LongLong: 8,
            c99_ast.ULongLong: 8}[type(sym.type)]
    if k >= size:
        return None
    base = binop.dst.name
    out: list = []
    src = val
    if k:
        shifted = tac_ast.Var(name=f"{base}.byte")
        ctx.symbols[shifted.name] = Symbol(type=sym.type, attrs=LocalAttr())
        out.append(tac_ast.Binary(
            op=tac_ast.RightShift(), src1=val,
            src2=tac_ast.Constant(const=tac_ast.ConstInt(value=8 * k)),
            dst=shifted,
        ))
        src = shifted
    byte = tac_ast.Var(name=f"{base}.b")
    ctx.symbols[byte.name] = Symbol(type=c99_ast.UChar(), attrs=LocalAttr())
    out.append(tac_ast.Truncate(src=src, dst=byte))
    if ctx.ssa_dsts is not None:
        ctx.ssa_dsts.update(i.dst.name for i in out)
    return out


def _try_narrow_pair(
    maybe_narrow: tac_ast.Type_val,
    maybe_const: tac_ast.Type_val,
    instrs: list[tac_ast.Type_instruction],
    var_def_idx: dict[str, int],
    use_count: dict[str, int],
    symbols,
    use_idx: int,
    ssa_dsts: set[str] | None,
) -> tuple[tac_ast.Type_val, int] | None:
    """Check whether `(maybe_narrow, maybe_const)` matches
    `(narrow_traced_through_zero_extend, ConstInt(C) with C ≤ 0xFF)`,
    with the extend's source holding the same value at
    `instrs[use_idx]` as at the extend. Returns
    `(narrow_source_val, mask_int)` on success, else None."""
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
    if not extend_source_unchanged(
        defining, def_idx, use_idx, instrs, ssa_dsts,
    ):
        return None
    return src, c.value
