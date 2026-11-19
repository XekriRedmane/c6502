"""TAC pass: fold `Unary(LogicalNot, src, %t);
JumpIfTrue/False(%t, target)` (with single-use `%t`, adjacent)
into a sense-flipped direct JumpIf on `src`.

# Motivating idiom

C99 `if (!cond) ...` where `cond` is any scalar lowers through
c99_to_tac as:

    Unary(LogicalNot, cond_val, %not)
    JumpIfFalse(%not, .if_end)

Without folding, `tac_to_asm` lowers the LogicalNot as a
materialize-0-or-1 sequence (BEQ over `LDA #0`, `LDA #1` on the
true side) and then the JumpIfFalse re-tests the just-materialized
value with an `ORA #$00; BEQ`. For a 1-byte source returned in A
from a JSR, the post-call `BEQ .lnot_true` already exploits the Z
flag for free, but the 0/1 select + re-test downstream is pure
waste — the same A holds the original predicate value that the
LogicalNot was about to invert.

# What this pass does

When `Unary(LogicalNot, src, %t)` is immediately followed by
`JumpIf{True,False}(%t, target)` and `%t` is used exactly once
across the function, rewrite as `JumpIf{False,True}(src, target)`
— the inverted sense passes through the LogicalNot's semantic
inversion, so the meaning is preserved.

The downstream lowering of `JumpIfTrue(src, t)` is `Mov(src.b0, A);
[ORA src.bk]; Branch(NE, t)` — for a 1-byte src that's a bare
`LDA src; BNE t`, and after the existing asm-level redundant-load
elimination drops the LDA when A already holds src (the
JSR-returns-in-A case), the whole sequence collapses to a single
post-JSR `BNE t`.

# Width-agnostic

Soundness doesn't depend on src's width. For multi-byte src the
JumpIf*'s own lowering walks every byte (ORA chain across the
high bytes, EQ/NE on the final result), which is strictly cheaper
than materializing 0/1 and then ORing-and-branching.

# Single-use gate

`%t` must be used exactly once across the function (the JumpIf's
condition read). Any additional use would mean the materialized
0/1 value flows somewhere else — e.g. assigned to a variable —
and dropping the LogicalNot would change those reads' values.
Standard DSE reaps the now-dead Unary on the next sweep.

Strict adjacency only — `Unary(LogicalNot, ...)` immediately
followed by the JumpIf. The c99_to_tac shape for `if (!x) ...`
produces that exact adjacency; non-adjacent cases would need
copy-prop / DSE to collapse the gap first, which the fixed-point
loop handles in subsequent rounds.
"""
from __future__ import annotations

import tac_ast
from passes.optimization.framework import (
    RuleSet, Rule, producer_then_jump, single_use,
    PassContext, MatchResult, RuleEnv,
    m_Unary, m_Any, m_Var,
)


def _build_lnot_jump(
    m: MatchResult, env: RuleEnv, ctx: PassContext,
) -> list[tac_ast.Type_instruction] | None:
    """Replace `LogicalNot(src, %t); JumpIf{True,False}(%t, T)` with the
    sense-flipped `JumpIf{False,True}(src, T)`. Pure structural rewrite —
    `!x` jumped-on is `x` jumped-on with the opposite sense."""
    jmp = m.bindings['jmp']
    src = m.bindings['src']
    cls = (
        tac_ast.JumpIfFalse
        if isinstance(jmp, tac_ast.JumpIfTrue)
        else tac_ast.JumpIfTrue
    )
    return [cls(condition=src, target=jmp.target)]


LNOT_RULE = Rule(
    name="fold_lnot_jump",
    pattern=producer_then_jump(
        m_Unary(
            op=tac_ast.LogicalNot,
            src=m_Any(capture='src'),
            dst=m_Var(capture='not_dst'),
        ),
        on='not_dst',
    ),
    where=[single_use('not_dst')],
    build=_build_lnot_jump,
)


_COMPARE_OPS = (
    tac_ast.Equal, tac_ast.NotEqual, tac_ast.LessThan,
    tac_ast.GreaterThan, tac_ast.LessOrEqual, tac_ast.GreaterOrEqual,
)

# The constant 1 of each integer constant class's type.
_ONE = {
    tac_ast.ConstChar: tac_ast.ConstChar, tac_ast.ConstUChar: tac_ast.ConstUChar,
    tac_ast.ConstInt: tac_ast.ConstInt, tac_ast.ConstUInt: tac_ast.ConstUInt,
}


def _zero_or_one(v, env: RuleEnv, ctx: PassContext, depth: int = 0) -> bool:
    """True when the Var `v` can hold only 0 or 1: an object that was a
    `bool` (`LocalAttr.was_bool` / `StaticAttr.was_bool`), or an SSA
    name defined by a comparison, a `!`, or an extend or copy of such
    a value."""
    if not isinstance(v, tac_ast.Var) or ctx.symbols is None:
        return False
    sym = ctx.symbols.get(v.name)
    if sym is None:
        return False
    if getattr(sym.attrs, "was_bool", False):
        return True
    if depth > 4 or ctx.ssa_dsts is None or v.name not in ctx.ssa_dsts:
        return False
    ins = env.def_of(v)
    match ins:
        case tac_ast.Binary(op=op) if isinstance(op, _COMPARE_OPS):
            return True
        case tac_ast.Unary(op=tac_ast.LogicalNot()):
            return True
        case tac_ast.ZeroExtend(src=s) | tac_ast.Copy(src=s):
            return _zero_or_one(s, env, ctx, depth + 1)
    return False


def _byte_type(t):
    """The c99 type `t` names, without `const` and `volatile`."""
    import c99_ast
    while isinstance(t, (c99_ast.Const, c99_ast.Volatile)):
        t = t.referenced_type
    return t


def _build_lnot_of_zero_or_one(
    m: MatchResult, env: RuleEnv, ctx: PassContext,
) -> list[tac_ast.Type_instruction] | None:
    """`!x` of a value that is 0 or 1 (`_zero_or_one`) is `x ^ 1`, which
    `tac_to_asm` lowers as `EOR #1` where `!` takes a branch to load 0
    or 1. The `^` is done at x's width, a byte for a `bool`, into a new
    temporary that the result's `ZeroExtend` (or `Truncate`) reads, or
    into the result itself at the same width. A `!` whose result a
    jump tests is left to `LNOT_RULE`, which drops the `!` and flips
    the jump, and that test is the result's one use."""
    import c99_ast
    from passes.optimization.value_range import int_width
    from passes.type_checking import LocalAttr, Symbol
    unary = m.bindings['unary']
    src, dst = unary.src, unary.dst
    if ctx.symbols is None or not isinstance(dst, tac_ast.Var):
        return None
    if not _zero_or_one(src, env, ctx):
        return None
    if env.use_count.get(dst.name, 0) == 1:
        uses = [
            ins for ins in env.instructions
            if isinstance(ins, (tac_ast.JumpIfTrue, tac_ast.JumpIfFalse))
            and ins.condition == dst
        ]
        if uses:
            return None
    s_t = _byte_type(ctx.symbols.get(src.name).type)
    d_sym = ctx.symbols.get(dst.name)
    if d_sym is None:
        return None
    s_w, d_w = int_width(s_t), int_width(d_sym.type)
    one_cls = {
        c99_ast.UChar: tac_ast.ConstUChar, c99_ast.Char: tac_ast.ConstUChar,
        c99_ast.SChar: tac_ast.ConstChar, c99_ast.Int: tac_ast.ConstInt,
        c99_ast.UInt: tac_ast.ConstUInt,
    }
    if s_w is None or d_w is None:
        return None
    if s_w == d_w:
        cls = one_cls.get(type(_byte_type(d_sym.type)))
        if cls is None:
            return None
        return [tac_ast.Binary(
            op=tac_ast.BitwiseXor(), src1=src,
            src2=tac_ast.Constant(const=cls(value=1)), dst=dst,
        )]
    narrow, extend = (src, True) if s_w < d_w else (None, False)
    t_type = s_t if extend else _byte_type(d_sym.type)
    cls = one_cls.get(type(t_type))
    if cls is None:
        return None
    k = 0
    while f"{dst.name}.not{k}" in ctx.symbols:
        k += 1
    tmp = tac_ast.Var(name=f"{dst.name}.not{k}")
    ctx.symbols[tmp.name] = Symbol(type=t_type, attrs=LocalAttr())
    if ctx.ssa_dsts is not None:
        ctx.ssa_dsts.add(tmp.name)
    one = tac_ast.Constant(const=cls(value=1))
    if extend:
        return [
            tac_ast.Binary(op=tac_ast.BitwiseXor(), src1=narrow, src2=one,
                           dst=tmp),
            tac_ast.ZeroExtend(src=tmp, dst=dst),
        ]
    return [
        tac_ast.Truncate(src=src, dst=tmp),
        tac_ast.Binary(op=tac_ast.BitwiseXor(), src1=tmp, src2=one, dst=dst),
    ]


LNOT_OF_ZERO_OR_ONE_RULE = Rule(
    name="lnot_of_zero_or_one",
    pattern=m_Unary(
        op=tac_ast.LogicalNot, dst=m_Var(capture='not_dst'),
        capture='unary',
    ),
    build=_build_lnot_of_zero_or_one,
)


def fold_lnot_jump(
    fn: tac_ast.Function,
    *,
    symbols: object | None = None,
) -> tac_ast.Function:
    """Walk `fn.instructions`, find adjacent `Unary(LogicalNot, ...);
    JumpIfTrue/False` pairs with single-use `%t`, and replace the
    pair with the sense-flipped JumpIf on the LogicalNot's source.
    `symbols` is accepted for signature uniformity with other folds
    in the fixed-point loop; this pass doesn't need it (width-
    agnostic rewrite)."""
    del symbols
    return RuleSet(LNOT_RULE).run(fn, PassContext())
