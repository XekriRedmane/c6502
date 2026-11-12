"""TAC peephole: fold `Truncate(Binary(safe_op, Extend(a), Extend(b)
| Constant), u)` → `Binary(safe_op, a, b | narrowed_Constant, u)`
at the narrow width.

# Motivating shape

C99 §6.3.1.1 integer-promotes 1-byte operands to `int` before any
arithmetic or bitwise op. For

    uint8_t a, b;
    uint8_t delta = (uint8_t)(a - b);

c99_to_tac emits:

    ZeroExtend(a   -> %ea:int)
    ZeroExtend(b   -> %eb:int)
    Binary(Subtract, %ea, %eb -> %r:int)    ; 16-bit subtract
    Truncate(%r -> %d:uchar)                ; cast back to 1 byte

`tac_to_asm` lowers the 16-bit subtract as a per-byte SBC chain.
The high byte's `LDA #$0; SBC #$0` is dead — its result feeds only
the dropped high byte of the Truncate — but the asm-level
`dead_a_arith` can't always see through the `LDA mem; BNE` shape
that follows because `all_flags_dead_at` bails at every Branch.
Killing the work at TAC level eliminates it at the source.

# Soundness

For an op whose result modulo 2^(8n) depends only on the operands
modulo 2^(8n) (the "wraparound-safe" ops), the low n bytes of
`op(extend_to_W(a), extend_to_W(b))` equal
`op(a_low_n, b_low_n) mod 2^(8n)`. So:

    Truncate(Binary(safe_op, Extend(a_n), Extend(b_n)), u_n)
        ≡ Binary(safe_op, a_n, b_n, u_n)

Wraparound-safe ops:
  - Add, Subtract — carry propagates strictly low→high.
  - Multiply — low n bytes of (a*b) depend only on low n bytes
    of a and b (two's-complement / unsigned modular arithmetic
    coincide for multiply).
  - BitwiseAnd, BitwiseOr, BitwiseXor — trivially per-byte.

NOT safe (high bytes contribute to low bytes of the result):
  - Divide, Modulo — long division pulls from high bytes.
  - RightShift — high bytes shift down into the low bytes.

LeftShift is safe in its first operand, since the low bytes of
`x << k` depend only on the low bytes of `x`. It's narrowed only
when the count is a Constant below the narrow width in bits. A
constant shift is inlined as ASL / ROL steps over every byte of the
operand, so `(uint8_t)(len << 4)` with `len` promoted to `int` would
otherwise shift a high byte that's thrown away.

For a Constant operand, narrowing reduces the value modulo
2^(8n). Modular arithmetic preserves equivalence regardless of
the constant's magnitude or sign — e.g. `Binary(Subtract,
ZE(uc), Constant(0x1234:int))` truncated to uchar is equivalent
to `Binary(Subtract, uc, Constant(0x34:uchar))`.

# Eligibility (per Truncate)

  - Truncate.src is an SSA-renamed Var whose def is a
    wraparound-safe Binary.
  - Each Binary operand is either:
      (a) an SSA-renamed Var whose def is `SignExtend` or
          `ZeroExtend` of a Var whose declared width equals the
          Truncate's dst width,
      (b) a Constant we can narrow to the dst width, or
      (c) any other integer Var wider than the dst width, such as
          a 16-bit global. Its low bytes come from a new
          `Truncate` just before the narrow Binary, so
          `(uint8_t)(w << 1)` on an `int w` becomes a 1-byte shift
          of `w`'s low byte. Only when the Truncate is the wide
          Binary's one reader, so the wide Binary goes away;
          otherwise the narrow one would repeat its work, as for
          `(uint8_t)(w + 3)` next to `(w + 3) >> 8`.
  - The Binary's dst width is strictly wider than the Truncate's
    dst width. (Otherwise `constant_fold` / `fold_truncate_extend`
    would already have caught the trivial case.)
  - The narrow Binary reads its operands where the Truncate was, so
    each Var it reads must still hold the value the wide Binary (or
    the Extend) read. An SSA-renamed Var always does. Any other Var
    has to be non-volatile, with the Extend or Binary in the same
    block as the Truncate and nothing in between that may write it
    (a definition of it, a call, or a store through a pointer or an
    index). Without this, `int t = g + 1; h(); (uint8_t)t` read `g`
    after `h` had changed it.

# Output

The Truncate is rewritten to a narrow Binary that writes
directly to the Truncate's dst. The original Binary and Extends
become dead; single-use Extend dsts are dropped eagerly via
Rewrite(drop_defs=...) and SSA-DCE collects any remaining
orphaned instructions on the next iteration.

# Iteration

Runs inside the TAC fixed-point loop. Composes with
`fold_truncate_extend` (which handles the no-Binary case),
constant folding (which sometimes resolves one Extend to a
Constant), and SSA-DCE (which collects the orphaned chain).
"""
from __future__ import annotations

import c99_ast
import tac_ast
from passes.optimization.framework import (
    DefUsePass, DefUseEnv, PassContext, MatchResult, Rewrite,
    m_Cast, m_Var,
)
from passes.optimization.var_visit import count_uses, defs_in


# Wraparound-safe ops: the low n bytes of op(W(a), W(b)) depend only
# on the low n bytes of W(a) and W(b). See module docstring.
_SAFE_OPS = (
    tac_ast.Add,
    tac_ast.Subtract,
    tac_ast.Multiply,
    tac_ast.BitwiseAnd,
    tac_ast.BitwiseOr,
    tac_ast.BitwiseXor,
)


class NarrowWidenedArith(DefUsePass):
    """DefUsePass: matches Truncate(src=Var, dst=Var). For each match,
    walks back via env.def_of to find a wraparound-safe Binary on
    widened operands; if found, rewrites the Truncate to a narrow
    Binary that produces the Truncate's dst directly.

    Drops the Binary's def eagerly via Rewrite(drop_defs=...) and
    also drops any single-use Extend defs that fed the Binary,
    so SSA-DCE doesn't need a separate sweep to clean them up."""
    name = "narrow_widened_arith"
    pattern = m_Cast(
        kind=tac_ast.Truncate,
        src=m_Var(capture='trunc_src'),
        dst=m_Var(capture='dst'),
        capture='trunc',
    )

    def prepare_extra(self, fn, ctx):
        return count_uses(fn.instructions)

    def rewrite(self, m: MatchResult, env: DefUseEnv, ctx: PassContext) -> object | None:
        if ctx.ssa_dsts is None or ctx.symbols is None:
            return None
        trunc_src = m.bindings['trunc_src']
        dst_var = m.bindings['dst']

        # trunc_src must be SSA-renamed (single-def guarantee).
        if trunc_src.name not in ctx.ssa_dsts:
            return None

        # Walk back: trunc_src must be defined by a wraparound-safe Binary.
        bin_instr = env.def_of(trunc_src)
        if not isinstance(bin_instr, tac_ast.Binary):
            return None
        is_shift = isinstance(bin_instr.op, tac_ast.LeftShift)
        if not (isinstance(bin_instr.op, _SAFE_OPS) or is_shift):
            return None
        if not isinstance(bin_instr.dst, tac_ast.Var):
            return None

        dst_width = _byte_width(dst_var, ctx.symbols)
        bin_width = _byte_width(bin_instr.dst, ctx.symbols)
        if dst_width is None or bin_width is None:
            return None
        if dst_width >= bin_width:
            # No widening to undo. fold_truncate_extend or constant_fold
            # handles the trivial cases.
            return None

        dst_unsigned = _is_unsigned(dst_var, ctx.symbols)
        if dst_unsigned is None:
            return None
        if is_shift:
            count = bin_instr.src2
            if not (isinstance(count, tac_ast.Constant)
                    and isinstance(getattr(count.const, "value", None), int)
                    and 0 <= count.const.value < 8 * dst_width):
                return None

        trunc_idx = env.def_idx.get(dst_var.name)
        bin_idx = env.def_idx.get(trunc_src.name)
        if trunc_idx is None or bin_idx is None:
            return None
        # Truncates of wide operands, placed just before the narrow
        # Binary. Their dsts are registered only once the rewrite is
        # certain. They're named after the wide result, which is an SSA
        # name, so they're unique: the Truncate's dst may be a global
        # that several Truncates write.
        pre: list[tac_ast.Truncate] = []
        narrow_s1 = _narrow_operand(
            bin_instr.src1, dst_width, dst_unsigned, env, bin_idx,
            trunc_idx, f"{trunc_src.name}.lo1", ctx, pre,
        )
        if narrow_s1 is None:
            return None
        narrow_s2 = _narrow_operand(
            bin_instr.src2, dst_width, dst_unsigned, env, bin_idx,
            trunc_idx, f"{trunc_src.name}.lo2", ctx, pre,
        )
        if narrow_s2 is None:
            return None
        for t in pre:
            _register_temp(ctx, like=dst_var.name, name=t.dst.name)

        narrow_binary = tac_ast.Binary(
            op=bin_instr.op, src1=narrow_s1, src2=narrow_s2,
            dst=dst_var,
        )

        # Eagerly drop the Binary's dst when the Truncate is its only
        # reader. Also drop each single-use Extend
        # dst that fed the Binary: use_counts[operand.name] == 1 means
        # only the Binary reads it, so after the Binary is gone it's dead.
        # (The Truncate is the current USE site; it's replaced, not dropped.)
        use_counts = env.extra
        replacement = [*pre, narrow_binary] if pre else narrow_binary
        # The wide Binary stays when anything besides this Truncate
        # reads its result (`l0 ^= f(); l0 += (int)l0;` reads the
        # wide XOR in the Add as well as through the Truncate). Its
        # Extend operands then stay alive too.
        if use_counts.get(bin_instr.dst.name, 0) != 1:
            if pre:
                # A wide operand isn't an extend that goes away, so the
                # narrow Binary would only repeat the wide one's work
                # (`(uint8_t)(w + 3)` next to `(w + 3) >> 8`).
                return None
            _count_new_uses(use_counts, replacement)
            return Rewrite(replacement=replacement)
        drop_vars: list[tac_ast.Var] = [bin_instr.dst]
        for operand in (bin_instr.src1, bin_instr.src2):
            if not isinstance(operand, tac_ast.Var):
                continue
            if operand.name not in ctx.ssa_dsts:
                continue
            ext = env.def_of(operand)
            if not isinstance(ext, (tac_ast.SignExtend, tac_ast.ZeroExtend)):
                continue
            # A new Truncate still reads the Extend's dst when the
            # operand was narrowed as a wide Var.
            if any(t.src == operand for t in pre):
                continue
            # Only drop the Extend if the Binary was its sole consumer.
            if use_counts.get(operand.name, 0) == 1:
                drop_vars.append(operand)

        _count_new_uses(use_counts, replacement)
        return Rewrite(replacement=replacement, drop_defs=tuple(drop_vars))


def _count_new_uses(use_counts, replacement) -> None:
    """Adds the reads of a rewrite's new instructions to the counts
    later rewrites in the same run decide their drops by. The counts
    are taken once per run, so without this a later rewrite could drop
    the definition of an extend source an earlier one now reads (the
    narrow Binary reads `p` where the wide one read `(int)p`)."""
    from passes.optimization.var_visit import uses_in
    for instr in replacement if isinstance(replacement, list) else [replacement]:
        for u in uses_in(instr):
            use_counts[u.name] += 1


def narrow_widened_arith(
    fn: tac_ast.Function, *,
    symbols=None, ssa_dsts: set[str] | None = None,
) -> tac_ast.Function:
    """Walk `fn`'s instructions; for each Truncate(t, u) whose src is
    defined by a wraparound-safe Binary on widened operands, rewrite
    the Truncate to a narrow Binary that produces u directly.

    `ssa_dsts` is the set of names introduced by `to_ssa`. The
    rewrite requires both the Binary's dst and each Extend's dst to
    be SSA-renamed so the def chain is unambiguous. Without it (or
    without `symbols`), the pass is a no-op."""
    ctx = PassContext(symbols=symbols, ssa_dsts=ssa_dsts)
    return NarrowWidenedArith().run(fn, ctx)


def _narrow_operand(
    operand: tac_ast.Type_val,
    target_width: int,
    target_unsigned: bool,
    env: DefUseEnv,
    bin_idx: int,
    trunc_idx: int,
    tmp_name: str,
    ctx: PassContext,
    pre: list[tac_ast.Truncate],
) -> tac_ast.Type_val | None:
    """Return a `Type_val` narrowed to `target_width` bytes (signed
    iff target_unsigned is False), or None if narrowing isn't safe.
    The narrow Binary sits where the Truncate is (`trunc_idx`), after
    the wide Binary (`bin_idx`).

    For a Var defined by a SignExtend / ZeroExtend of a Var whose
    declared width equals `target_width`: returns the cast's source
    Var, when it still holds the value the cast read.

    For any other integer Var wider than `target_width`: appends
    `Truncate(operand, tmp_name)` to `pre` and returns the temporary,
    when `operand` still holds the value the Binary read.

    For a Constant: returns a fresh Constant of the matching narrow
    variant, value masked to `target_width` bytes."""
    symbols, ssa_dsts = ctx.symbols, ctx.ssa_dsts
    if isinstance(operand, tac_ast.Constant):
        return _narrow_constant(operand, target_width, target_unsigned)
    if not isinstance(operand, tac_ast.Var):
        return None
    if operand.name in ssa_dsts:
        cast_idx = env.def_idx.get(operand.name)
        cast = env.instructions[cast_idx] if cast_idx is not None else None
        if (
            isinstance(cast, (tac_ast.SignExtend, tac_ast.ZeroExtend))
            and isinstance(cast.src, tac_ast.Var)
            and _byte_width(cast.src, symbols) == target_width
        ):
            if not _unchanged(cast.src, cast_idx, trunc_idx, env, ctx):
                return None
            return cast.src
    width = _byte_width(operand, symbols)
    if (
        width is None or width <= target_width
        or _is_unsigned(operand, symbols) is None
        or not _unchanged(operand, bin_idx, trunc_idx, env, ctx)
    ):
        return None
    tmp = tac_ast.Var(name=tmp_name)
    pre.append(tac_ast.Truncate(src=operand, dst=tmp))
    return tmp


# Instructions that may write a variable they don't name.
_MAY_WRITE_MEMORY = (
    tac_ast.FunctionCall, tac_ast.IndirectCall, tac_ast.Store,
    tac_ast.IndexedStore, tac_ast.IndexedSymbolStore,
    tac_ast.IndirectIndexedStore,
)

_ENDS_BLOCK = (
    tac_ast.Label, tac_ast.Ret, tac_ast.Jump, tac_ast.JumpIfTrue,
    tac_ast.JumpIfFalse, tac_ast.JumpIfCmp, tac_ast.JumpIfMasked,
)


def _unchanged(
    var: tac_ast.Var, start: int, end: int, env: DefUseEnv,
    ctx: PassContext,
) -> bool:
    """True when `var` holds at `end` the value it held at `start`.
    An SSA-renamed Var has one definition, so it always does. Any
    other Var must be non-volatile, with `start` and `end` in one
    block and nothing between them that may write it."""
    if var.name in ctx.ssa_dsts:
        return True
    sym = ctx.symbols.get(var.name)
    if sym is None or _is_volatile(sym.type) or start >= end:
        return False
    for k in range(start + 1, end):
        instr = env.instructions[k]
        if isinstance(instr, _ENDS_BLOCK + _MAY_WRITE_MEMORY):
            return False
        if any(d.name == var.name for d in defs_in(instr)):
            return False
    return True


def _is_volatile(t) -> bool:
    while isinstance(t, c99_ast.Const):
        t = t.referenced_type
    return isinstance(t, c99_ast.Volatile)


def _register_temp(ctx: PassContext, like: str, name: str) -> None:
    """Registers `name` as a local with `like`'s type, and as an SSA
    name."""
    from passes.type_checking import LocalAttr, Symbol
    if name not in ctx.symbols:
        ctx.symbols[name] = Symbol(
            type=ctx.symbols.get(like).type, attrs=LocalAttr(),
        )
    ctx.ssa_dsts.add(name)


def _narrow_constant(
    c: tac_ast.Constant, width: int, unsigned: bool,
) -> tac_ast.Constant | None:
    """Return a Constant of the matching narrow width / signedness,
    or None if the source is FP (FP narrowing isn't a modular
    reduction, and the safe-op list excludes FP-relevant cases
    anyway)."""
    inner = c.const
    if isinstance(inner, (tac_ast.ConstFloat, tac_ast.ConstDouble)):
        return None
    raw = inner.value
    mask = (1 << (8 * width)) - 1
    masked = raw & mask
    if width == 1:
        if unsigned:
            return tac_ast.Constant(const=tac_ast.ConstUChar(value=masked))
        signed = masked if masked < 0x80 else masked - 0x100
        return tac_ast.Constant(const=tac_ast.ConstChar(value=signed))
    if width == 2:
        if unsigned:
            return tac_ast.Constant(const=tac_ast.ConstUInt(value=masked))
        signed = masked if masked < 0x8000 else masked - 0x10000
        return tac_ast.Constant(const=tac_ast.ConstInt(value=signed))
    if width == 4:
        if unsigned:
            return tac_ast.Constant(const=tac_ast.ConstULong(value=masked))
        signed = masked if masked < 0x80000000 else masked - 0x100000000
        return tac_ast.Constant(const=tac_ast.ConstLong(value=signed))
    if width == 8:
        if unsigned:
            return tac_ast.Constant(
                const=tac_ast.ConstULongLong(value=masked),
            )
        signed = (
            masked if masked < 0x8000000000000000
            else masked - 0x10000000000000000
        )
        return tac_ast.Constant(
            const=tac_ast.ConstLongLong(value=signed),
        )
    return None


def _byte_width(v: tac_ast.Var, symbols) -> int | None:
    """Byte width of `v`'s declared type, or None if symbols missing
    or type is opaque (Array / Structure / Union)."""
    sym = symbols.get(v.name)
    if sym is None:
        return None
    t = sym.type
    while isinstance(t, (c99_ast.Const, c99_ast.Volatile)):
        t = t.referenced_type
    if isinstance(t, (c99_ast.Char, c99_ast.SChar, c99_ast.UChar)):
        return 1
    if isinstance(t, (c99_ast.Int, c99_ast.UInt, c99_ast.Pointer)):
        return 2
    if isinstance(t, (c99_ast.Long, c99_ast.ULong, c99_ast.Float)):
        return 4
    if isinstance(t, (
        c99_ast.LongLong, c99_ast.ULongLong, c99_ast.Double,
    )):
        return 8
    return None


def _is_unsigned(v: tac_ast.Var, symbols) -> bool | None:
    """True iff `v`'s declared type is an unsigned integer type
    (UChar / Char (c6502 plain char is unsigned) / UInt / ULong /
    ULongLong / Pointer). False for the signed integer types.
    None for FP / Array / Structure / Union (the narrowing
    rewrite isn't applicable)."""
    sym = symbols.get(v.name)
    if sym is None:
        return None
    t = sym.type
    while isinstance(t, (c99_ast.Const, c99_ast.Volatile)):
        t = t.referenced_type
    if isinstance(t, (c99_ast.Char, c99_ast.UChar)):
        return True
    if isinstance(t, c99_ast.SChar):
        return False
    if isinstance(t, c99_ast.Int):
        return False
    if isinstance(t, (c99_ast.UInt, c99_ast.Pointer)):
        return True
    if isinstance(t, c99_ast.Long):
        return False
    if isinstance(t, c99_ast.ULong):
        return True
    if isinstance(t, c99_ast.LongLong):
        return False
    if isinstance(t, c99_ast.ULongLong):
        return True
    return None
