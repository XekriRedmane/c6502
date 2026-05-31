"""TAC pass: fold `Binary(cmp_op, src1, src2, cond);
JumpIfTrue/False(cond, t)` (with single-use cond) into a single
direct conditional jump.

Two flavors of fold, depending on the comparison shape:

  * **Equal/NotEqual against zero** — the pattern that motivates the
    pass. C99 `if (a == 0) ...` for a `uint8_t a` produces:

        ZeroExtend(a, %0)              # a is uint8_t, promoted to int
        Binary(Equal, %0, ConstInt(0), %cond)
        JumpIfFalse(%cond, .else)

    Without folding, `tac_to_asm` lowers the Binary as a multi-byte
    CMP + zero/one-select, then JumpIfFalse loads %cond and BEQs.
    ~13 instructions for what should be `LDA a; BNE .else`. We
    rewrite to `JumpIfTrue(a, .else)` (sense flip), tracing through
    the ZeroExtend so the test happens at the source's narrow width.

  * **Anything else** — other comparison ops, or `==`/`!=` against a
    non-zero constant. We rewrite as the new TAC instruction
    `JumpIfCmp(op, src1, src2, t)`, which `tac_to_asm` lowers as a
    per-byte compare chain ending in a single `Branch` (no 0/1
    materialize). The op is inverted when the original JumpIf was a
    JumpIfFalse — `<` becomes `>=`, `==` becomes `!=`, etc., so the
    JumpIfCmp always means "jump if op is true".

    Operand narrowing through ZeroExtend: when one operand is the
    single-use dst of a `ZeroExtend(narrow_var)` upstream and
    `narrow_var` has a 1-byte unsigned type (Char / UChar), and the
    other operand either traces to the same kind of narrow Var or is
    an integer constant whose value fits in 0..255, both operands
    narrow to 1 byte. A signed-int `(int)uint8 < 105` then lowers as
    a 1-byte unsigned CMP — `LDA a; CMP #105; BCS .end` for the
    JumpIfFalse sense — instead of the 16-bit SBC chain.

    Operand narrowing through SignExtend: for ordering against zero
    (`>= 0` / `< 0` only), if one operand is the single-use dst of a
    `SignExtend(narrow_var)` whose source is a 1-byte signed type
    (`SChar`) and the other operand is a literal zero, both narrow
    to 1 byte. The transformation is sound because SignExtend
    preserves the sign bit — `(int)schar >= 0 ⇔ schar >= 0` — and
    the resulting `JumpIfCmp(GE/LT, schar_var, ConstChar(0), t)`
    lowers via the dedicated zero-relational asm path (no SBC, no
    V-correction) into a bare `LDA schar_var; B<PL|MI> t`. The
    motivating case is the rotated signed-countdown for-loop's
    tail test, which becomes `DEC mem; LDA mem; BPL .top`.

The cmp's dst becomes dead in both cases; standard DSE picks it up
along with any preceding ZeroExtend whose dst was only used by it.

"Single-use" gating. SSA TAC has single-def for free; the use-count
of `cond` must be exactly 1 (the JumpIf's read). If it had additional
uses (e.g. also returned), removing the Binary would break those.
The pass runs inside the SSA-bracketed fixed-point loop, so the SSA
invariant holds.

Strict adjacency only — `Binary` immediately followed by the JumpIf.
The c99_to_tac shapes for `if (...)`, `while (...)`, `?:` produce
that exact adjacency; non-adjacent cases would need copy-prop / DSE
to collapse the gap first, which the fixed-point loop handles in
subsequent rounds.
"""
from __future__ import annotations

import c99_ast
import tac_ast
from passes.optimization.framework import (
    RuleSet, Rule, producer_then_jump, single_use,
    PassContext, MatchResult, RuleEnv,
    m_Binary, m_Var,
)


_CMP_OPS: tuple[type, ...] = (
    tac_ast.Equal, tac_ast.NotEqual,
    tac_ast.LessThan, tac_ast.GreaterThan,
    tac_ast.LessOrEqual, tac_ast.GreaterOrEqual,
)


def _build_cmp_zero_jump(
    m: MatchResult, env: RuleEnv, ctx: PassContext,
) -> list | None:
    """Fold `Binary(cmp_op, ...); JumpIf{True,False}(cond, T)` to a
    direct conditional jump. `== 0` / `!= 0` trace through ZeroExtend to
    a bare sense-flipped JumpIf; everything else emits a `JumpIfCmp`
    (op inverted for the JumpIfFalse sense), narrowing both operands to
    1 byte where the ZeroExtend / SignExtend chain permits."""
    use_count, var_def_idx, instrs = env.use_count, env.def_idx, env.instructions
    binop = m.bindings['binop']
    jmp = m.bindings['jmp']

    if isinstance(binop.op, (tac_ast.Equal, tac_ast.NotEqual)):
        x = _zero_compare_other_operand(binop.src1, binop.src2)
        if x is not None:
            x = _trace_through_zero_extend(x, instrs, var_def_idx, use_count)
            return [_build_replacement_jump(binop.op, jmp, x)]

    src1, src2 = binop.src1, binop.src2
    narrowed = _try_narrow_compare(
        src1, src2, instrs, var_def_idx, use_count, ctx.symbols,
    )
    if narrowed is not None:
        src1, src2 = narrowed
    else:
        narrowed_signed = _try_narrow_signed_against_zero(
            binop.op, src1, src2, instrs, var_def_idx, use_count, ctx.symbols,
        )
        if narrowed_signed is not None:
            src1, src2 = narrowed_signed
    new_op = _adjusted_op_for_jumpif(binop.op, jmp)
    return [tac_ast.JumpIfCmp(
        op=new_op, src1=src1, src2=src2, target=jmp.target,
    )]


CMP_ZERO_RULE = Rule(
    name="fold_cmp_zero_jump",
    pattern=producer_then_jump(
        m_Binary(op=_CMP_OPS, dst=m_Var(capture='cond_dst'), capture='binop'),
        on='cond_dst',
    ),
    where=[single_use('cond_dst')],
    build=_build_cmp_zero_jump,
)


def fold_cmp_zero_jump(
    fn: tac_ast.Function,
    *,
    symbols=None,
) -> tac_ast.Function:
    """Walk `fn.instructions`, find adjacent `Binary(cmp_op, ...);
    JumpIfTrue/False` pairs with single-use cond, and rewrite. The
    `symbols` table is needed for the narrowing path (we read each
    Var's c99 type to decide if a 1-byte unsigned narrowing is sound);
    without it the pass falls back to non-narrowing rewrites."""
    return RuleSet(CMP_ZERO_RULE).run(fn, PassContext(symbols=symbols))


def _adjusted_op_for_jumpif(
    op: tac_ast.Type_binary_operator,
    jumpif: tac_ast.Type_instruction,
) -> tac_ast.Type_binary_operator:
    """JumpIfCmp's contract is "jump if op(src1, src2) is true". When
    the source pattern is JumpIfFalse, we invert op so the new
    instruction still means "jump if true (under the inverted op)"."""
    if isinstance(jumpif, tac_ast.JumpIfTrue):
        return op
    inverter: dict[type, type] = {
        tac_ast.Equal: tac_ast.NotEqual,
        tac_ast.NotEqual: tac_ast.Equal,
        tac_ast.LessThan: tac_ast.GreaterOrEqual,
        tac_ast.GreaterOrEqual: tac_ast.LessThan,
        tac_ast.GreaterThan: tac_ast.LessOrEqual,
        tac_ast.LessOrEqual: tac_ast.GreaterThan,
    }
    return inverter[type(op)]()


# --- Narrowing -------------------------------------------------------

_NARROW_UNSIGNED_TYPES: tuple[type, ...] = (
    c99_ast.Char,   # plain char is unsigned in c6502
    c99_ast.UChar,
)


# 1-byte signed types eligible for the SignExtend-against-zero
# narrowing. `Char` is unsigned in c6502; only `SChar` has the
# sign-bit-preserved property we exploit here.
_NARROW_SIGNED_TYPES: tuple[type, ...] = (
    c99_ast.SChar,
)


# Ordering ops where narrowing through SignExtend against zero
# preserves the comparison's truth value. Strictly `>= 0` and `< 0`:
# only the sign bit matters, so the underlying byte's sign bit is
# the answer regardless of width. `> 0` and `<= 0` would also need
# a zero check (more than one branch), so they're out of scope.
_GE_LT_OPS: tuple[type, ...] = (
    tac_ast.GreaterOrEqual, tac_ast.LessThan,
)


def _try_narrow_through_zero_extend(
    val: tac_ast.Type_val,
    instrs: list[tac_ast.Type_instruction],
    var_def_idx: dict[str, int],
    use_count: dict[str, int],
    symbols,
) -> tuple[tac_ast.Type_val, object] | None:
    """If `val` is a Var that's the single-use dst of a ZeroExtend
    upstream and the source has a c99 type, return (source, type).
    Otherwise None."""
    if not isinstance(val, tac_ast.Var):
        return None
    if use_count.get(val.name, 0) != 1:
        return None
    def_idx = var_def_idx.get(val.name)
    if def_idx is None:
        return None
    defining = instrs[def_idx]
    if not isinstance(defining, tac_ast.ZeroExtend):
        return None
    src = defining.src
    if not isinstance(src, tac_ast.Var):
        return None
    if symbols is None:
        return None
    sym = symbols.get(src.name)
    if sym is None:
        return None
    return src, sym.type


def _narrow_const_to_unsigned_byte(
    val: tac_ast.Type_val,
) -> tac_ast.Constant | None:
    """If `val` is an integer Constant whose value fits in 0..255,
    return a `ConstUChar(value)` rewrap. Otherwise None."""
    if not isinstance(val, tac_ast.Constant):
        return None
    c = val.const
    if not isinstance(c, _INTEGER_CONSTS):
        return None
    if not (0 <= c.value <= 255):
        return None
    return tac_ast.Constant(const=tac_ast.ConstUChar(value=c.value))


def _try_narrow_through_sign_extend(
    val: tac_ast.Type_val,
    instrs: list[tac_ast.Type_instruction],
    var_def_idx: dict[str, int],
    use_count: dict[str, int],
    symbols,
) -> tuple[tac_ast.Type_val, object] | None:
    """If `val` is a Var that's the single-use dst of a SignExtend
    upstream and the source has a c99 type, return (source, type).
    Otherwise None. Mirror of `_try_narrow_through_zero_extend` for
    the signed case."""
    if not isinstance(val, tac_ast.Var):
        return None
    if use_count.get(val.name, 0) != 1:
        return None
    def_idx = var_def_idx.get(val.name)
    if def_idx is None:
        return None
    defining = instrs[def_idx]
    if not isinstance(defining, tac_ast.SignExtend):
        return None
    src = defining.src
    if not isinstance(src, tac_ast.Var):
        return None
    if symbols is None:
        return None
    sym = symbols.get(src.name)
    if sym is None:
        return None
    return src, sym.type


def _try_narrow_signed_against_zero(
    op: tac_ast.Type_binary_operator,
    src1: tac_ast.Type_val,
    src2: tac_ast.Type_val,
    instrs: list[tac_ast.Type_instruction],
    var_def_idx: dict[str, int],
    use_count: dict[str, int],
    symbols,
) -> tuple[tac_ast.Type_val, tac_ast.Type_val] | None:
    """Narrow `Binary(GreaterOrEqual | LessThan, ?, ?, _)` whose
    one operand is the single-use dst of `SignExtend(SChar_var)`
    and the other is a literal zero. Returns
    `(SChar_var, ConstChar(0))` in the appropriate slot ordering,
    or None if no narrowing applies. The rewrite preserves truth
    because SignExtend keeps the sign bit, and `>= 0` / `< 0` are
    sign-bit tests."""
    if not isinstance(op, _GE_LT_OPS):
        return None
    if symbols is None:
        return None
    info1 = _try_narrow_through_sign_extend(
        src1, instrs, var_def_idx, use_count, symbols,
    )
    info2 = _try_narrow_through_sign_extend(
        src2, instrs, var_def_idx, use_count, symbols,
    )
    n1_ok = info1 is not None and isinstance(
        info1[1], _NARROW_SIGNED_TYPES,
    )
    n2_ok = info2 is not None and isinstance(
        info2[1], _NARROW_SIGNED_TYPES,
    )
    zero_const = tac_ast.Constant(const=tac_ast.ConstChar(value=0))
    if n1_ok and _is_constant_zero(src2):
        return info1[0], zero_const
    if n2_ok and _is_constant_zero(src1):
        return zero_const, info2[0]
    return None


def _try_narrow_compare(
    src1: tac_ast.Type_val,
    src2: tac_ast.Type_val,
    instrs: list[tac_ast.Type_instruction],
    var_def_idx: dict[str, int],
    use_count: dict[str, int],
    symbols,
) -> tuple[tac_ast.Type_val, tac_ast.Type_val] | None:
    """Try to narrow both compare operands to 1-byte unsigned. The
    rule: at least one operand is a single-use ZeroExtend dst whose
    source has type Char / UChar (1-byte unsigned); the other is
    either also such a Var, or an integer constant fitting 0..255.
    Returns the narrowed pair, or None if narrowing isn't applicable."""
    if symbols is None:
        return None
    info1 = _try_narrow_through_zero_extend(
        src1, instrs, var_def_idx, use_count, symbols,
    )
    info2 = _try_narrow_through_zero_extend(
        src2, instrs, var_def_idx, use_count, symbols,
    )

    n1_ok = info1 is not None and isinstance(
        info1[1], _NARROW_UNSIGNED_TYPES,
    )
    n2_ok = info2 is not None and isinstance(
        info2[1], _NARROW_UNSIGNED_TYPES,
    )

    if n1_ok and n2_ok:
        return info1[0], info2[0]
    if n1_ok:
        nc = _narrow_const_to_unsigned_byte(src2)
        if nc is not None:
            return info1[0], nc
    if n2_ok:
        nc = _narrow_const_to_unsigned_byte(src1)
        if nc is not None:
            return nc, info2[0]
    return None


# --- Helpers shared with the zero-fold path --------------------------

def _zero_compare_other_operand(
    a: tac_ast.Type_val, b: tac_ast.Type_val,
) -> tac_ast.Type_val | None:
    """If exactly one of `a`/`b` is an integer Constant with value 0,
    return the OTHER operand. Otherwise None."""
    a_is_zero = _is_constant_zero(a)
    b_is_zero = _is_constant_zero(b)
    if a_is_zero and not b_is_zero:
        return b
    if b_is_zero and not a_is_zero:
        return a
    return None


def _is_constant_zero(val: tac_ast.Type_val) -> bool:
    """True iff `val` is an integer Constant whose value is 0."""
    if not isinstance(val, tac_ast.Constant):
        return False
    c = val.const
    return isinstance(c, _INTEGER_CONSTS) and c.value == 0


_INTEGER_CONSTS: tuple[type, ...] = (
    tac_ast.ConstChar, tac_ast.ConstUChar,
    tac_ast.ConstInt, tac_ast.ConstLong, tac_ast.ConstLongLong,
    tac_ast.ConstUInt, tac_ast.ConstULong, tac_ast.ConstULongLong,
)


def _trace_through_zero_extend(
    x: tac_ast.Type_val,
    instrs: list[tac_ast.Type_instruction],
    var_def_idx: dict[str, int],
    use_count: dict[str, int],
) -> tac_ast.Type_val:
    """If `x` is a Var that's the single-use dst of a ZeroExtend
    upstream, return the ZeroExtend's source. Loops to handle chained
    ZeroExtends."""
    while isinstance(x, tac_ast.Var):
        if use_count.get(x.name, 0) != 1:
            break
        def_idx = var_def_idx.get(x.name)
        if def_idx is None:
            break
        defining = instrs[def_idx]
        if not isinstance(defining, tac_ast.ZeroExtend):
            break
        x = defining.src
    return x


def _build_replacement_jump(
    op: tac_ast.Type_binary_operator,
    outer: tac_ast.Type_instruction,
    x: tac_ast.Type_val,
) -> tac_ast.Type_instruction:
    """Choose JumpIfTrue / JumpIfFalse based on (Equal/NotEqual) ×
    (outer's class):
        Equal    + JumpIfFalse → JumpIfTrue(x)
        Equal    + JumpIfTrue  → JumpIfFalse(x)
        NotEqual + JumpIfFalse → JumpIfFalse(x)
        NotEqual + JumpIfTrue  → JumpIfTrue(x)
    NotEqual preserves the outer sense; Equal flips it."""
    is_equal = isinstance(op, tac_ast.Equal)
    outer_is_true = isinstance(outer, tac_ast.JumpIfTrue)
    new_is_true = outer_is_true if not is_equal else not outer_is_true
    cls = tac_ast.JumpIfTrue if new_is_true else tac_ast.JumpIfFalse
    return cls(condition=x, target=outer.target)
