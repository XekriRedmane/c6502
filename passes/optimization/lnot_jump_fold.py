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


def fold_lnot_jump(
    fn: tac_ast.Function,
    *,
    symbols=None,
) -> tac_ast.Function:
    """Walk `fn.instructions`, find adjacent `Unary(LogicalNot, ...);
    JumpIfTrue/False` pairs with single-use `%t`, and replace the
    pair with the sense-flipped JumpIf on the LogicalNot's source.
    `symbols` is accepted for signature uniformity with other folds
    in the fixed-point loop; this pass doesn't need it (width-
    agnostic rewrite)."""
    del symbols
    use_count = _count_var_uses(fn)

    new_instrs: list[tac_ast.Type_instruction] = []
    skip_next = False
    for i, instr in enumerate(fn.instructions):
        if skip_next:
            skip_next = False
            continue
        rewrite = _try_fold(fn.instructions, i, use_count)
        if rewrite is None:
            new_instrs.append(instr)
            continue
        new_instrs.append(rewrite)
        skip_next = True
    if len(new_instrs) == len(fn.instructions):
        return fn
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
) -> tac_ast.Type_instruction | None:
    """If `instrs[i:i+2]` is `Unary(LogicalNot, src, %t);
    JumpIf{True,False}(%t, target)` with single-use `%t`, return the
    sense-flipped JumpIf on `src`. Otherwise None."""
    if i + 1 >= len(instrs):
        return None
    unary = instrs[i]
    if not isinstance(unary, tac_ast.Unary):
        return None
    if not isinstance(unary.op, tac_ast.LogicalNot):
        return None
    if not isinstance(unary.dst, tac_ast.Var):
        return None
    jumpif = instrs[i + 1]
    if not isinstance(jumpif, (tac_ast.JumpIfTrue, tac_ast.JumpIfFalse)):
        return None
    if not isinstance(jumpif.condition, tac_ast.Var):
        return None
    if jumpif.condition.name != unary.dst.name:
        return None
    if use_count.get(unary.dst.name, 0) != 1:
        return None
    cls = (
        tac_ast.JumpIfTrue
        if isinstance(jumpif, tac_ast.JumpIfFalse)
        else tac_ast.JumpIfFalse
    )
    return cls(condition=unary.src, target=jumpif.target)


def _count_var_uses(fn: tac_ast.Function) -> dict[str, int]:
    """Count Var uses by name across the whole function."""
    out: dict[str, int] = {}
    for instr in fn.instructions:
        for v in _vars_used_in(instr):
            out[v.name] = out.get(v.name, 0) + 1
    return out


def _vars_used_in(instr: tac_ast.Type_instruction):
    """Yield every Var read by `instr`. Mirrors the use-site set in
    cmp_zero_jump_fold._vars_used_in."""
    match instr:
        case tac_ast.Ret(val=val):
            if isinstance(val, tac_ast.Var):
                yield val
        case tac_ast.SignExtend(src=s):
            if isinstance(s, tac_ast.Var):
                yield s
        case tac_ast.ZeroExtend(src=s):
            if isinstance(s, tac_ast.Var):
                yield s
        case tac_ast.Truncate(src=s):
            if isinstance(s, tac_ast.Var):
                yield s
        case tac_ast.IntToFloat(src=s) | tac_ast.IntToDouble(src=s):
            if isinstance(s, tac_ast.Var):
                yield s
        case tac_ast.FloatToInt(src=s) | tac_ast.DoubleToInt(src=s):
            if isinstance(s, tac_ast.Var):
                yield s
        case tac_ast.FloatToDouble(src=s) | tac_ast.DoubleToFloat(src=s):
            if isinstance(s, tac_ast.Var):
                yield s
        case tac_ast.Unary(src=s):
            if isinstance(s, tac_ast.Var):
                yield s
        case tac_ast.Binary(src1=s1, src2=s2):
            if isinstance(s1, tac_ast.Var):
                yield s1
            if isinstance(s2, tac_ast.Var):
                yield s2
        case tac_ast.Copy(src=s):
            if isinstance(s, tac_ast.Var):
                yield s
        case tac_ast.GetAddress():
            return
        case tac_ast.Load(src_ptr=p):
            if isinstance(p, tac_ast.Var):
                yield p
        case tac_ast.Store(src=s, dst_ptr=p):
            if isinstance(s, tac_ast.Var):
                yield s
            if isinstance(p, tac_ast.Var):
                yield p
        case tac_ast.IndexedLoad(index=i):
            if isinstance(i, tac_ast.Var):
                yield i
        case tac_ast.IndexedStore(index=i, src=s):
            if isinstance(i, tac_ast.Var):
                yield i
            if isinstance(s, tac_ast.Var):
                yield s
        case tac_ast.IndexedSymbolStore(index=i, src=s):
            if isinstance(i, tac_ast.Var):
                yield i
            if isinstance(s, tac_ast.Var):
                yield s
        case tac_ast.JumpIfTrue(condition=c) | tac_ast.JumpIfFalse(condition=c):
            if isinstance(c, tac_ast.Var):
                yield c
        case tac_ast.JumpIfCmp(src1=s1, src2=s2):
            if isinstance(s1, tac_ast.Var):
                yield s1
            if isinstance(s2, tac_ast.Var):
                yield s2
        case tac_ast.JumpIfMasked(val=v):
            if isinstance(v, tac_ast.Var):
                yield v
        case tac_ast.FunctionCall(args=args):
            for a in args:
                if isinstance(a, tac_ast.Var):
                    yield a
        case tac_ast.IndirectCall(ptr=p, args=args):
            if isinstance(p, tac_ast.Var):
                yield p
            for a in args:
                if isinstance(a, tac_ast.Var):
                    yield a
        case tac_ast.Phi(args=args):
            for a in args:
                if isinstance(a.source, tac_ast.Var):
                    yield a.source
