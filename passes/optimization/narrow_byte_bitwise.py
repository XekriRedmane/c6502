"""TAC pass: an `&`, `|`, or `^` whose result has zero high bytes is
done on one byte and zero-extended.

# Motivating shape

C99 §6.3.1.1 promotes a byte to `int` before `&`, so

    uint8_t x;
    if ((x & 0x3F) < 3) ...

compares two `int`s:

    ZeroExtend(x -> %e:int)
    Binary(BitwiseAnd, %e, ConstInt(63) -> %m:int)
    JumpIfCmp(GreaterOrEqual, %m, ConstInt(3), ...)

`cmp_zero_jump_fold` narrows a comparison to one byte when each
operand is a ZeroExtend of a byte or a constant from 0 to 255, and
`%m` is neither, so the comparison was a signed 16-bit subtraction
with an overflow test. This pass rewrites the `&` as

    Binary(BitwiseAnd, x, ConstUChar(63) -> %n:uchar)
    ZeroExtend(%n -> %m:int)

after which the comparison narrows to `CMP #3; BCS`. Ali Baba's units
compared a masked byte this way in a dozen places.

# The rule

Each operand of the wide op is classified by its high bytes:

  - a ZeroExtend of a 1-byte Var has zero high bytes, and its low
    byte is the Var;
  - a SignExtend of a 1-byte Var has high bytes of unknown value,
    and its low byte is the Var;
  - a Constant from 0 to 255 has zero high bytes;
  - any other Constant has high bytes of known value, and its low
    byte is the value mod 256;
  - any other Var of the op's width has high bytes of unknown value,
    and its low byte is a Truncate of it.

A bitwise op works on each byte separately, so the low byte of the
result is the op of the low bytes. The high bytes of `a & b` are zero
when either operand's are, and those of `a | b` and `a ^ b` are zero
when both operands' are. Only then is the op rewritten. When neither
operand is a Var with zero high bytes nothing is gained (`w & 0x3F`
on an `int` w already writes its high byte as a constant 0), so at
least one operand must be a ZeroExtend or SignExtend.

# Soundness

The extend's source is read at the op in place of at the extend, so
it must hold the same value at both
(`cmp_zero_jump_fold.extend_source_unchanged`): an SSA-renamed name,
or a static read earlier in the same straight run with nothing
between that might write it. A Truncate of a Var other than an
extend reads it at the op, where the op read it already.
"""
from __future__ import annotations

import c99_ast
import tac_ast
from passes.optimization.cmp_zero_jump_fold import extend_source_unchanged
from passes.optimization.framework import (
    DefUsePass, DefUseEnv, PassContext, MatchResult, Rewrite,
    m_Binary, m_Var,
)

_OPS = (tac_ast.BitwiseAnd, tac_ast.BitwiseOr, tac_ast.BitwiseXor)

_INT_CONSTS = (
    tac_ast.ConstChar, tac_ast.ConstUChar, tac_ast.ConstInt,
    tac_ast.ConstUInt, tac_ast.ConstLong, tac_ast.ConstULong,
    tac_ast.ConstLongLong, tac_ast.ConstULongLong,
)


def _width(t) -> int | None:
    while isinstance(t, (c99_ast.Const, c99_ast.Volatile)):
        t = t.referenced_type
    if isinstance(t, (c99_ast.Char, c99_ast.SChar, c99_ast.UChar)):
        return 1
    if isinstance(t, (c99_ast.Int, c99_ast.UInt, c99_ast.Pointer)):
        return 2
    if isinstance(t, (c99_ast.Long, c99_ast.ULong)):
        return 4
    if isinstance(t, (c99_ast.LongLong, c99_ast.ULongLong)):
        return 8
    return None


def _is_volatile(t) -> bool:
    while isinstance(t, c99_ast.Const):
        t = t.referenced_type
    return isinstance(t, c99_ast.Volatile)


class NarrowByteBitwise(DefUsePass):
    """DefUsePass: matches a wide `Binary(&, |, ^)` and rewrites it as
    the op on the operands' low bytes into a new `uchar` temporary,
    zero-extended into the old dst, when the result's high bytes are
    zero. See the module docstring."""
    name = "narrow_byte_bitwise"
    pattern = m_Binary(op=_OPS, dst=m_Var(capture='dst'), capture='bin')

    def rewrite(self, m: MatchResult, env: DefUseEnv, ctx: PassContext):
        if ctx.symbols is None or ctx.ssa_dsts is None:
            return None
        binary = m.bindings['bin']
        dst = m.bindings['dst']
        dst_sym = ctx.symbols.get(dst.name)
        if dst_sym is None:
            return None
        width = _width(dst_sym.type)
        if width is None or width == 1:
            return None
        use_idx = next(
            (i for i, ins in enumerate(env.instructions) if ins is binary),
            None,
        )
        if use_idx is None:
            return None
        ops = [
            self._classify(v, width, env, ctx, use_idx)
            for v in (binary.src1, binary.src2)
        ]
        if any(o is None for o in ops):
            return None
        if not any(o[2] for o in ops):
            return None
        zero_high = [o[1] for o in ops]
        if isinstance(binary.op, tac_ast.BitwiseAnd):
            if not any(zero_high):
                return None
        elif not all(zero_high):
            return None
        pre: list[tac_ast.Type_instruction] = []
        narrow = []
        for v, (low, _, _) in zip((binary.src1, binary.src2), ops):
            if low is None:
                t = self._temp(ctx, dst.name, "lo")
                pre.append(tac_ast.Truncate(src=v, dst=t))
                low = t
            narrow.append(low)
        n = self._temp(ctx, dst.name, "n")
        return Rewrite(replacement=pre + [
            tac_ast.Binary(op=binary.op, src1=narrow[0], src2=narrow[1],
                           dst=n),
            tac_ast.ZeroExtend(src=n, dst=dst),
        ])

    def _classify(self, v, width, env, ctx, use_idx):
        """Returns `(low, zero_high, extended)` for an operand: `low` is
        its low byte as a 1-byte value, or None when it takes a
        Truncate; `zero_high` is true when its high bytes are zero;
        `extended` is true for an extend of a byte. None when the
        operand can't be narrowed."""
        if isinstance(v, tac_ast.Constant):
            if not isinstance(v.const, _INT_CONSTS):
                return None
            value = v.const.value
            byte = tac_ast.Constant(const=tac_ast.ConstUChar(value=value & 0xFF))
            return byte, 0 <= value <= 0xFF, False
        if not isinstance(v, tac_ast.Var):
            return None
        sym = ctx.symbols.get(v.name)
        if sym is None or _width(sym.type) != width or _is_volatile(sym.type):
            return None
        if v.name in ctx.ssa_dsts:
            ext = env.def_of(v)
            if isinstance(ext, (tac_ast.ZeroExtend, tac_ast.SignExtend)) \
                    and isinstance(ext.src, tac_ast.Var):
                src_sym = ctx.symbols.get(ext.src.name)
                if src_sym is not None and _width(src_sym.type) == 1 \
                        and not _is_volatile(src_sym.type) \
                        and extend_source_unchanged(
                            ext, env.def_idx[v.name], use_idx,
                            env.instructions, ctx.ssa_dsts):
                    return (ext.src, isinstance(ext, tac_ast.ZeroExtend),
                            True)
        return None, False, False

    def _temp(self, ctx, like: str, tag: str) -> tac_ast.Var:
        from passes.type_checking import LocalAttr, Symbol
        k = 0
        while f"{like}.{tag}{k}" in ctx.symbols:
            k += 1
        name = f"{like}.{tag}{k}"
        ctx.symbols[name] = Symbol(type=c99_ast.UChar(), attrs=LocalAttr())
        ctx.ssa_dsts.add(name)
        return tac_ast.Var(name=name)
