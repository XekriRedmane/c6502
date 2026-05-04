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
    if len(new_instrs) == len(fn.instructions):
        return fn
    return tac_ast.Function(
        name=fn.name,
        is_global=fn.is_global,
        params=list(fn.params),
        instructions=new_instrs,
    )


_CMP_OPS: tuple[type, ...] = (
    tac_ast.Equal, tac_ast.NotEqual,
    tac_ast.LessThan, tac_ast.GreaterThan,
    tac_ast.LessOrEqual, tac_ast.GreaterOrEqual,
)


def _try_fold(
    instrs: list[tac_ast.Type_instruction],
    i: int,
    use_count: dict[str, int],
    var_def_idx: dict[str, int],
    symbols,
) -> tac_ast.Type_instruction | None:
    """If `instrs[i:i+2]` is the foldable pattern, return the
    replacement instruction. Otherwise None."""
    if i + 1 >= len(instrs):
        return None
    binop = instrs[i]
    if not isinstance(binop, tac_ast.Binary):
        return None
    if not isinstance(binop.op, _CMP_OPS):
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

    # Special case: ==/!= against zero. Lowering already produces
    # the optimal `LDA x; BEQ/BNE t` (1 byte) or `LDA x.b0; ORA
    # x.b1; ...; BEQ/BNE t` (multi-byte) for JumpIfTrue/False on x.
    if isinstance(binop.op, (tac_ast.Equal, tac_ast.NotEqual)):
        x = _zero_compare_other_operand(binop.src1, binop.src2)
        if x is not None:
            x = _trace_through_zero_extend(
                x, instrs, var_def_idx, use_count,
            )
            return _build_replacement_jump(binop.op, jumpif, x)

    # General case: rewrite as JumpIfCmp. Try to narrow both
    # operands to 1-byte unsigned via ZeroExtend tracing — that
    # turns a 16-bit SBC chain into a 1-byte CMP. Optional: even
    # without narrowing the JumpIfCmp form is still a win (skips
    # the 0/1 materialize).
    src1, src2 = binop.src1, binop.src2
    narrowed = _try_narrow_compare(
        src1, src2, instrs, var_def_idx, use_count, symbols,
    )
    if narrowed is not None:
        src1, src2 = narrowed
    new_op = _adjusted_op_for_jumpif(binop.op, jumpif)
    return tac_ast.JumpIfCmp(
        op=new_op, src1=src1, src2=src2, target=jumpif.target,
    )


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


def _count_var_uses(fn: tac_ast.Function) -> dict[str, int]:
    """Count Var uses by name across the whole function."""
    out: dict[str, int] = {}
    for instr in fn.instructions:
        for v in _vars_used_in(instr):
            out[v.name] = out.get(v.name, 0) + 1
    return out


def _index_var_defs(fn: tac_ast.Function) -> dict[str, int]:
    """Map each Var name to the index of its defining instruction.
    Assumes SSA single-def — multiple defs would overwrite."""
    out: dict[str, int] = {}
    for i, instr in enumerate(fn.instructions):
        d = _var_def_in(instr)
        if d is not None:
            out[d.name] = i
    return out


def _var_def_in(
    instr: tac_ast.Type_instruction,
) -> tac_ast.Var | None:
    """Return the Var defined by `instr`, or None if no def."""
    match instr:
        case tac_ast.Copy(dst=dst):
            return dst if isinstance(dst, tac_ast.Var) else None
        case tac_ast.SignExtend(dst=dst):
            return dst if isinstance(dst, tac_ast.Var) else None
        case tac_ast.ZeroExtend(dst=dst):
            return dst if isinstance(dst, tac_ast.Var) else None
        case tac_ast.Truncate(dst=dst):
            return dst if isinstance(dst, tac_ast.Var) else None
        case tac_ast.Unary(dst=dst):
            return dst if isinstance(dst, tac_ast.Var) else None
        case tac_ast.Binary(dst=dst):
            return dst if isinstance(dst, tac_ast.Var) else None
    return None


def _vars_used_in(instr: tac_ast.Type_instruction):
    """Yield every Var read by `instr`."""
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
        case tac_ast.JumpIfTrue(condition=c) | tac_ast.JumpIfFalse(condition=c):
            if isinstance(c, tac_ast.Var):
                yield c
        case tac_ast.JumpIfCmp(src1=s1, src2=s2):
            if isinstance(s1, tac_ast.Var):
                yield s1
            if isinstance(s2, tac_ast.Var):
                yield s2
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
