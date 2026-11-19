"""TAC rule: a signed ordering compare of two values that can't be
negative is done unsigned.

`row + 1 < WNDBTM`, with `row` and `WNDBTM` bytes, compares two `int`s,
and a signed compare of 2-byte values is a subtract and a test of the
overflow flag before the sign (`BVC; EOR #$80; BMI`). The values are
from 0 to 256 and from 0 to 255, and two values from 0 to a type's
largest value compare the same way as signed and as unsigned numbers,
so an unsigned compare, which branches on the carry, gives the same
result in 4 bytes less.

`value_range.max_value` bounds each operand. The rule runs last in
the jump-fold rule set (`fold_producer_jumps` in `optimizer.py`), after
the rules that narrow a compare of extended bytes to a byte compare,
and applies to a `JumpIfCmp` and to a comparison kept as a value.
"""
from __future__ import annotations

import c99_ast
import tac_ast
from passes.optimization.framework import (
    Rule, PassContext, MatchResult, RuleEnv,
)
from passes.optimization.framework.patterns import Pattern


_ORDERING_OPS: tuple[type, ...] = (
    tac_ast.LessThan, tac_ast.GreaterThan,
    tac_ast.LessOrEqual, tac_ast.GreaterOrEqual,
)

# The unsigned constant of each signed integer constant's width.
_UNSIGNED_CONST = {
    tac_ast.ConstChar: tac_ast.ConstUChar,
    tac_ast.ConstInt: tac_ast.ConstUInt,
    tac_ast.ConstLong: tac_ast.ConstULong,
    tac_ast.ConstLongLong: tac_ast.ConstULongLong,
}

# The unsigned c99 type of each signed integer type's width.
_UNSIGNED_TYPE = {
    c99_ast.SChar: c99_ast.UChar, c99_ast.Int: c99_ast.UInt,
    c99_ast.Long: c99_ast.ULong, c99_ast.LongLong: c99_ast.ULongLong,
}


def _unsigned_operands(src1, src2, env: RuleEnv, ctx: PassContext):
    """`(pre, a, b)` for a signed ordering compare of `src1` and `src2`
    whose values `value_range.max_value` shows can't be negative, with
    `a` and `b` the operands as unsigned values and `pre` the copies
    that make them, or None.

    Two values from 0 to the type's largest compare the same way as
    signed and as unsigned numbers, and an unsigned compare has no
    test of the overflow flag (`BVC; EOR #$80`). A constant becomes
    the unsigned constant of its width, which makes the compare
    unsigned by itself (`tac_to_asm._is_unsigned_val`), and when both
    are Vars the first is copied into a new unsigned temporary of its
    width, a copy that copy propagation keeps since it changes the
    signedness and that the asm optimizer's copy propagation removes.
    `row + 1 < WNDBTM` with byte operands is the motivating case.

    A compare with the constant 0 is left alone, for the sign-bit
    lowering of a signed compare with 0, and so is any operand of a
    type other than a signed integer of 2 bytes or more: a byte
    compare narrows to an unsigned one by other rules when it can."""
    from passes.optimization.value_range import max_value
    if ctx.symbols is None or ctx.ssa_dsts is None:
        return None

    def signed_type(v):
        if isinstance(v, tac_ast.Constant):
            return type(v.const) in _UNSIGNED_CONST
        if not isinstance(v, tac_ast.Var):
            return False
        sym = ctx.symbols.get(v.name)
        if sym is None:
            return False
        t = sym.type
        return type(t) in _UNSIGNED_TYPE and type(t) is not c99_ast.SChar

    for v in (src1, src2):
        if not signed_type(v):
            return None
        if isinstance(v, tac_ast.Constant):
            if type(v.const) is tac_ast.ConstChar or v.const.value <= 0:
                return None
        elif max_value(v, env.def_of, ctx.symbols, ctx.ssa_dsts) is None:
            return None
    pre: list = []
    out = []
    for v in (src1, src2):
        if isinstance(v, tac_ast.Constant):
            out.append(tac_ast.Constant(
                const=_UNSIGNED_CONST[type(v.const)](value=v.const.value)))
        elif not pre and not any(isinstance(o, tac_ast.Constant)
                                 for o in (src1, src2)):
            from passes.type_checking import LocalAttr, Symbol
            k = 0
            while f"{v.name}.u{k}" in ctx.symbols:
                k += 1
            name = f"{v.name}.u{k}"
            t = _UNSIGNED_TYPE[type(ctx.symbols.get(v.name).type)]()
            ctx.symbols[name] = Symbol(type=t, attrs=LocalAttr())
            ctx.ssa_dsts.add(name)
            pre.append(tac_ast.Copy(src=v, dst=tac_ast.Var(name=name)))
            out.append(tac_ast.Var(name=name))
        else:
            out.append(v)
    return pre, out[0], out[1]


def _build_nonnegative_compare(
    m: MatchResult, env: RuleEnv, ctx: PassContext,
) -> list | None:
    """A signed ordering compare of two values that can't be negative
    is done unsigned (`_unsigned_operands`), as a `JumpIfCmp` or as a
    comparison kept as a value."""
    ins = m.bindings['cmp']
    if isinstance(ins, tac_ast.JumpIfCmp):
        if not isinstance(ins.op, _ORDERING_OPS):
            return None
        got = _unsigned_operands(ins.src1, ins.src2, env, ctx)
        if got is None:
            return None
        pre, a, b = got
        return pre + [tac_ast.JumpIfCmp(
            op=ins.op, src1=a, src2=b, target=ins.target)]
    if not isinstance(ins, tac_ast.Binary) or not isinstance(
            ins.op, _ORDERING_OPS):
        return None
    got = _unsigned_operands(ins.src1, ins.src2, env, ctx)
    if got is None:
        return None
    pre, a, b = got
    return pre + [tac_ast.Binary(op=ins.op, src1=a, src2=b, dst=ins.dst)]


class _m_Compare(Pattern):
    """Matches a `JumpIfCmp` or a `Binary` and binds it as `capture`."""

    def __init__(self, capture: str) -> None:
        self.capture = capture

    def match(self, node, m):
        if not isinstance(node, (tac_ast.JumpIfCmp, tac_ast.Binary)):
            return False
        m.bindings[self.capture] = node
        return True


NONNEGATIVE_COMPARE_RULE = Rule(
    name="nonnegative_compare",
    pattern=_m_Compare('cmp'),
    build=_build_nonnegative_compare,
)
