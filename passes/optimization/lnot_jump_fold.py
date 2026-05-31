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
