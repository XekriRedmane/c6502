"""TAC pass: fold `(x == 0) / (x != 0)` feeding a `JumpIfTrue` /
`JumpIfFalse` into a direct `JumpIfFalse(x) / JumpIfTrue(x)`.

The motivating pattern. C99 `if (a == 0) ...` for a `uint8_t a`
produces (after `c99_to_tac`):

    ZeroExtend(a, %0)              # a is uint8_t, promoted to int per
                                   # C99 §6.3.1.1.2
    Binary(Equal, %0, ConstInt(0), %cond)
    JumpIfFalse(%cond, .else)

Without this pass, `tac_to_asm` lowers the `Binary(Equal, ...)` as a
multi-byte CMP + zero/one-select sequence, then the JumpIfFalse
loads %cond and BEQ's. ~13 instructions for what should be a bare
`LDA a; BNE .else`.

This pass recognizes the chain and rewrites:

    Binary(Equal, x, 0, cond)
    JumpIfFalse(cond, t)           # → JumpIfTrue(x_narrow, t)

    Binary(Equal, x, 0, cond)
    JumpIfTrue(cond, t)            # → JumpIfFalse(x_narrow, t)

    Binary(NotEqual, x, 0, cond)
    JumpIfFalse(cond, t)           # → JumpIfFalse(x_narrow, t)

    Binary(NotEqual, x, 0, cond)
    JumpIfTrue(cond, t)            # → JumpIfTrue(x_narrow, t)

The `Binary` is dropped (its dst becomes dead); standard DSE picks
up any now-dead defs (including a preceding `ZeroExtend` whose dst
was only used by the comparison).

Narrowing through `ZeroExtend`. If `x` is a Var that's the
single-use dst of a `ZeroExtend(narrow, x)` upstream, substitute
`x` with the original `narrow` value. The resulting `JumpIfTrue` /
`JumpIfFalse` then operates at the narrow width — `tac_to_asm`'s
size-driven lowering reads the symbol table for the operand's c99
type and emits the appropriate byte-count zero-test (1 byte =
`LDA x; BEQ/BNE`; 2+ bytes = ORA chain).

Why "single-use" matters. If `cond` had additional uses (say, also
returned), removing the Binary would break those uses. SSA TAC has
single-def for free; we only need to verify single-use. The pass
runs inside the SSA-bracketed fixed-point loop so the SSA invariant
holds.

Pattern-matches strict adjacency only — `Binary` immediately
followed by the `JumpIf`. Most `if (x == 0)` / `while (x != 0)` /
`(x == 0) ? ...` constructs from `c99_to_tac` produce that exact
adjacency; non-adjacent cases would require copy-prop / DSE to
collapse the gap first, which the fixed-point loop does for us
in subsequent rounds.
"""
from __future__ import annotations

import tac_ast


def fold_cmp_zero_jump(
    fn: tac_ast.Function,
    *,
    symbols=None,
) -> tac_ast.Function:
    """Walk `fn.instructions`, find adjacent `Binary(==/!=, x, 0,
    cond); JumpIfTrue/False(cond, t)` pairs with single-use `cond`,
    and rewrite. `symbols` is unused today but accepted for
    consistency with the other SSA-aware passes' signatures."""
    use_count = _count_var_uses(fn)
    var_def_idx = _index_var_defs(fn)

    new_instrs: list[tac_ast.Type_instruction] = []
    skip_next = False
    for i, instr in enumerate(fn.instructions):
        if skip_next:
            skip_next = False
            continue
        rewrite = _try_fold(
            fn.instructions, i, use_count, var_def_idx,
        )
        if rewrite is None:
            new_instrs.append(instr)
            continue
        # Drop the Binary (i) and replace the JumpIf (i+1) with
        # `rewrite`. Skip the JumpIf in the next iteration.
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
    var_def_idx: dict[str, int],
) -> tac_ast.Type_instruction | None:
    """If `instrs[i:i+2]` is the foldable pattern, return the
    replacement JumpIf. Otherwise None."""
    if i + 1 >= len(instrs):
        return None
    binop = instrs[i]
    if not isinstance(binop, tac_ast.Binary):
        return None
    if not isinstance(binop.op, (tac_ast.Equal, tac_ast.NotEqual)):
        return None
    if not isinstance(binop.dst, tac_ast.Var):
        return None
    # One of the operands must be a zero-valued integer Constant.
    x = _zero_compare_other_operand(binop.src1, binop.src2)
    if x is None:
        return None
    # The next instruction must be a JumpIf reading binop.dst.
    jumpif = instrs[i + 1]
    if not isinstance(jumpif, (tac_ast.JumpIfTrue, tac_ast.JumpIfFalse)):
        return None
    if not isinstance(jumpif.condition, tac_ast.Var):
        return None
    if jumpif.condition.name != binop.dst.name:
        return None
    # The Binary's dst must be used exactly once — we're about to
    # drop the Binary, so any other reader would see a missing def.
    if use_count.get(binop.dst.name, 0) != 1:
        return None
    # Optionally narrow x by tracing through a ZeroExtend def.
    x = _trace_through_zero_extend(x, instrs, var_def_idx, use_count)
    # Build the replacement JumpIf with the right sense.
    return _build_replacement_jump(binop.op, jumpif, x)


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
    """True iff `val` is an integer Constant whose value is 0.
    Accepts any integer variant — Equal/NotEqual against 0 has the
    same semantics regardless of which signedness was assigned to
    the literal."""
    if not isinstance(val, tac_ast.Constant):
        return False
    c = val.const
    return isinstance(c, _INTEGER_CONSTS) and c.value == 0


_INTEGER_CONSTS: tuple[type, ...] = (
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
    upstream, return the ZeroExtend's source instead. Loops to
    handle chained ZeroExtends (rare, but cheap to support)."""
    while isinstance(x, tac_ast.Var):
        # Single-use: x is read exactly twice — once in the Binary
        # we're folding (which becomes dead) and once... wait, the
        # Binary's read counts as one use. After our rewrite drops
        # the Binary, x is read by the new JumpIf only. So we want
        # x's pre-rewrite use_count == 1 (only the Binary uses it).
        # Since after we substitute, the JumpIf will use the
        # narrowed source, x itself becomes dead and DSE drops the
        # ZeroExtend.
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
    """Return the Var defined by `instr`, or None if no def. Phi /
    FunctionCall / IndirectCall are excluded — Phi for now (we
    don't trace through them), Function calls because their dst is
    already a single-use destination of an effectful op (no
    benefit to tracing). Restrict to "value-producing" ops we
    might trace."""
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
    """Yield every Var read by `instr`. Mirrors the use-walking
    logic in `copy_propagation` and friends."""
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
        case tac_ast.JumpIfTrue(condition=c) | tac_ast.JumpIfFalse(condition=c):
            if isinstance(c, tac_ast.Var):
                yield c
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
