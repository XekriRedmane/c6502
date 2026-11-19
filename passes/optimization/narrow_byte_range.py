"""TAC pass: a wide divide, remainder, or right shift of a value from
0 to 255 is done on one byte, and so is a left shift whose result
stays below 256, and a wide multiply of two such values reads them as
zero-extended bytes.

# Motivating shapes

C99 §6.3.1.1 promotes a byte to `int` before `/`, `%`, and `>>`, so

    uint8_t pos;
    uint8_t row = pos / 20;

divides two `int`s.

    ZeroExtend(pos -> %e:int)
    Binary(Divide, %e, ConstInt(20) -> %q:int)
    Truncate(%q -> row)

and `tac_to_asm` called the signed 16-bit helper `sdivmod16`. The
dividend is from 0 to 255 and the divisor from 1 to 255, so the
quotient is that of an unsigned byte divide, and so is the remainder.
This pass rewrites the divide as

    Binary(Divide, pos, ConstUChar(20) -> %n:uchar)
    ZeroExtend(%n -> %q:int)

after which `fold_truncate_extend` makes `row` the byte quotient,
strength reduction makes a power of two a shift or a mask, and
`tac_to_asm` lowers a byte divide by any other constant as a loop
of subtractions (`_inline_byte_divide`).

A right shift of such a value, `(style >> 5) & 3` with a `uint8_t
style`, is a shift of the byte for the same reason, whatever the
count, and the shifted-out high byte is zero. As an `int` it was a
signed 16-bit shift by 5, which called `asr16`.

A multiply of two such values, `price * (tier + 1)` with a `uint8_t
price` and a `tier` from 0 to 3, has a 16-bit product. `tac_to_asm`
lowers a 16-bit multiply whose operands each have one significant
byte (`passes/significant_bytes.py`) as an 8-by-8 shift-and-add loop
in place of a `mul16` call (`_inline_byte_product`). Here `tier + 1`
is an `int` add, whose high byte isn't known to be zero, so this pass
reads the operand as `ZeroExtend(Truncate(tier + 1))`, and
`narrow_widened_arith` then makes the add a byte add.

# The rule

`passes/optimization/value_range.py` bounds each operand. An operand
qualifies when its bound is at most 255, which also means it isn't
negative. A signed and an unsigned divide, remainder, or right shift
then give the same result, which is the unsigned byte one.

  - `Divide` / `Modulo`: the dividend qualifies, and the divisor is a
    constant from 1 to 255 or a Var that qualifies (a zero divisor
    has no defined result at either width).
  - `RightShift`: the shifted value qualifies. The lowering reads only
    the count's low byte, so a count that's a ZeroExtend of a byte is
    read as the byte, and any other count stays. A constant count
    of 8 or more gives 0 at either width, and the shift becomes a
    copy of 0. A variable count of 8 to 15 gives 0 at both widths,
    since the byte shift's loop shifts every bit out.
  - `LeftShift`: the count is a constant `k` from 0 to 7 and the
    shifted value's bound shifted left by `k` is at most 255, so the
    bits shifted out of the low byte are zeros and the high bytes of
    the result are 0. `0x28 * (row / 8)` on a byte `row`, which
    strength reduction makes `((row / 8) << 3) + ((row / 8) << 5)`,
    shifts `row / 8`, at most 31, by 3 as a byte.
  - `Multiply`: the destination is 2 bytes and both operands qualify,
    and at least one isn't already a byte constant or a ZeroExtend of
    a byte.

Each operand becomes a byte. A constant becomes a `ConstUChar`. An
SSA-renamed Var defined by a ZeroExtend of an unsigned byte becomes
that byte, when it holds the same value at the op
(`cmp_zero_jump_fold.extend_source_unchanged`). Any other Var gets a
new `Truncate` just before the op, which reads it where the op did.
"""
from __future__ import annotations

import c99_ast
import tac_ast
from passes.optimization.cmp_zero_jump_fold import extend_source_unchanged
from passes.optimization.framework import (
    DefUsePass, DefUseEnv, PassContext, MatchResult, Rewrite,
    m_Binary, m_Var,
)
from passes.optimization.value_range import (
    int_width, is_unsigned_type, max_value,
)

_OPS = (tac_ast.Divide, tac_ast.Modulo, tac_ast.RightShift, tac_ast.Multiply,
        tac_ast.LeftShift)


def _is_volatile(t) -> bool:
    while isinstance(t, c99_ast.Const):
        t = t.referenced_type
    return isinstance(t, c99_ast.Volatile)


class NarrowByteRange(DefUsePass):
    """DefUsePass: matches a wide `Binary(/, %, >>, *)` whose operands
    are bounded by 255 and does it on bytes. See the module
    docstring."""
    name = "narrow_byte_range"
    pattern = m_Binary(op=_OPS, dst=m_Var(capture='dst'), capture='bin')

    def rewrite(self, m: MatchResult, env: DefUseEnv, ctx: PassContext):
        if ctx.symbols is None or ctx.ssa_dsts is None:
            return None
        binary = m.bindings['bin']
        dst = m.bindings['dst']
        dst_sym = ctx.symbols.get(dst.name)
        if dst_sym is None:
            return None
        width = int_width(dst_sym.type)
        if width is None or width == 1:
            return None
        use_idx = next(
            (i for i, ins in enumerate(env.instructions) if ins is binary),
            None,
        )
        if use_idx is None:
            return None

        def bound(v):
            return max_value(v, env.def_of, ctx.symbols, ctx.ssa_dsts)

        a, b = binary.src1, binary.src2
        if not self._operand_ok(a, width, ctx):
            return None
        ma = bound(a)
        if ma is None or ma > 0xFF:
            return None
        if isinstance(binary.op, tac_ast.Multiply):
            return self._multiply(binary, dst, width, env, ctx, use_idx,
                                  bound)
        if isinstance(binary.op, tac_ast.LeftShift):
            # `(row / 8) << 3` is at most 248, so its high byte is 0.
            count = b.const.value if (
                isinstance(b, tac_ast.Constant)
                and isinstance(getattr(b.const, "value", None), int)
            ) else None
            if count is None or not 0 <= count <= 7 or ma << count > 0xFF:
                return None
            pre = []
            low = self._byte(a, env, ctx, use_idx, dst.name, pre)
            return self._narrowed(
                binary.op, low,
                tac_ast.Constant(const=tac_ast.ConstUChar(value=count)),
                dst, pre, ctx)
        if isinstance(binary.op, tac_ast.RightShift):
            count = b.const.value if (
                isinstance(b, tac_ast.Constant)
                and isinstance(getattr(b.const, "value", None), int)
            ) else None
            if count is not None and count >= 8:
                return tac_ast.Copy(src=_zero_like(dst_sym.type), dst=dst)
            if count is not None and count < 0:
                return None
            pre: list[tac_ast.Type_instruction] = []
            low = self._byte(a, env, ctx, use_idx, dst.name, pre)
            # The lowering reads only the count's low byte, so a count
            # that's a widened byte is read as the byte.
            b = self._extend_source(b, env, ctx, use_idx) or b
            return self._narrowed(binary.op, low, b, dst, pre, ctx)
        # Divide / Modulo.
        if isinstance(b, tac_ast.Constant):
            c = getattr(b.const, "value", None)
            if not isinstance(c, int) or not 1 <= c <= 0xFF:
                return None
        else:
            if not self._operand_ok(b, width, ctx):
                return None
            mb = bound(b)
            if mb is None or mb > 0xFF:
                return None
        pre = []
        low_a = self._byte(a, env, ctx, use_idx, dst.name, pre)
        low_b = self._byte(b, env, ctx, use_idx, dst.name, pre)
        return self._narrowed(binary.op, low_a, low_b, dst, pre, ctx)

    def _multiply(self, binary, dst, width, env, ctx, use_idx, bound):
        """Reads each operand of a 2-byte multiply of two bytes as a
        ZeroExtend of a byte, when one of them isn't already."""
        if width != 2:
            return None
        b = binary.src2
        if not self._operand_ok(b, width, ctx):
            return None
        mb = bound(b)
        if mb is None or mb > 0xFF:
            return None
        forms = [self._byte_form(v, env, ctx, use_idx)
                 for v in (binary.src1, binary.src2)]
        if all(forms):
            return None
        pre: list[tac_ast.Type_instruction] = []
        operands = []
        for v, ok in zip((binary.src1, binary.src2), forms):
            if ok:
                operands.append(v)
                continue
            low = self._byte(v, env, ctx, use_idx, dst.name, pre)
            wide = self._temp(ctx, dst.name, "nw", like=v.name)
            pre.append(tac_ast.ZeroExtend(src=low, dst=wide))
            operands.append(wide)
        return Rewrite(replacement=pre + [tac_ast.Binary(
            op=binary.op, src1=operands[0], src2=operands[1], dst=dst,
        )])

    def _narrowed(self, op, a, b, dst, pre, ctx):
        n = self._temp(ctx, dst.name, "nb")
        return Rewrite(replacement=pre + [
            tac_ast.Binary(op=op, src1=a, src2=b, dst=n),
            tac_ast.ZeroExtend(src=n, dst=dst),
        ])

    def _operand_ok(self, v, width, ctx) -> bool:
        """A constant, or a non-volatile Var of the op's width."""
        if isinstance(v, tac_ast.Constant):
            return True
        if not isinstance(v, tac_ast.Var):
            return False
        sym = ctx.symbols.get(v.name)
        return (sym is not None and int_width(sym.type) == width
                and not _is_volatile(sym.type))

    def _extend_source(self, v, env, ctx, use_idx):
        """The unsigned byte a Var is a ZeroExtend of, when it holds
        the same value at the op, else None."""
        if not isinstance(v, tac_ast.Var) or v.name not in ctx.ssa_dsts:
            return None
        ext = env.def_of(v)
        if not (isinstance(ext, tac_ast.ZeroExtend)
                and isinstance(ext.src, tac_ast.Var)):
            return None
        src_sym = ctx.symbols.get(ext.src.name)
        if (src_sym is None or int_width(src_sym.type) != 1
                or not is_unsigned_type(src_sym.type)
                or _is_volatile(src_sym.type)):
            return None
        if not extend_source_unchanged(
                ext, env.def_idx[v.name], use_idx, env.instructions,
                ctx.ssa_dsts):
            return None
        return ext.src

    def _byte_form(self, v, env, ctx, use_idx) -> bool:
        if isinstance(v, tac_ast.Constant):
            return 0 <= getattr(v.const, "value", -1) <= 0xFF
        return self._extend_source(v, env, ctx, use_idx) is not None

    def _byte(self, v, env, ctx, use_idx, base, pre):
        """`v` as an unsigned byte, from 0 to 255 by its bound."""
        if isinstance(v, tac_ast.Constant):
            return tac_ast.Constant(
                const=tac_ast.ConstUChar(value=v.const.value))
        src = self._extend_source(v, env, ctx, use_idx)
        if src is not None:
            return src
        t = self._temp(ctx, base, "lo")
        pre.append(tac_ast.Truncate(src=v, dst=t))
        return t

    def _temp(self, ctx, base: str, tag: str, like: str | None = None):
        """A new SSA local named after `base`, a `uchar`, or of `like`'s
        type."""
        from passes.type_checking import LocalAttr, Symbol
        k = 0
        while f"{base}.{tag}{k}" in ctx.symbols:
            k += 1
        name = f"{base}.{tag}{k}"
        t = c99_ast.UChar() if like is None else ctx.symbols.get(like).type
        ctx.symbols[name] = Symbol(type=t, attrs=LocalAttr())
        ctx.ssa_dsts.add(name)
        return tac_ast.Var(name=name)


def _zero_like(t) -> tac_ast.Constant:
    """A zero constant of the integer type `t`."""
    from passes.optimization.strength_reduction import _zero_for_c99_type
    return tac_ast.Constant(const=_zero_for_c99_type(t))
