"""How many low bytes of each local can be nonzero.

`tac_to_asm` lowers a shift of a byte widened to 16 bits as a shift
of all 16 bits, and calls `asl16` for one of more than 4 places,
though the high byte is zero. `64 * (c & 7)` in Zork's
`please_insert_save_diskette` is `ZeroExtend(c) & 7`, then `<< 6`.
`significant_bytes` works out, for each local of a function, how many
of its low bytes can be nonzero, so the lowering can shift just those
(`tac_to_asm._inline_left_shift_byte`).

A local's count is the largest over its definitions:

- `ZeroExtend(x)`, `Copy(x)`: x's count.
- `Truncate(x)`: x's count, at most the local's size.
- `x & y`: the smaller of the two counts.
- `x | y`, `x ^ y`: the larger.
- `x >> k` with a constant `k`, when x is unsigned or its top byte
  is known to be zero: x's count less the bytes `k` shifts out.
- A `Phi`: the largest over its arguments.
- Anything else: every byte.

A constant counts the bytes its value needs, a negative one every
byte of its type. A parameter, a static, a local whose address is
taken (a store through a pointer could write it), and anything not
defined in the function counts every byte. The counts start at 0 and
only grow, so the loop that recomputes them ends.
"""
from __future__ import annotations

from collections.abc import Callable

import tac_ast
from passes.optimization.var_visit import defs_in


def significant_bytes(
    fn: tac_ast.Function,
    symbols,
    size_of: Callable[[tac_ast.Type_val], int],
    is_unsigned: Callable[[tac_ast.Type_val], bool],
) -> dict[str, int]:
    """Each local of `fn` with fewer significant bytes than its size,
    mapped to that number. `size_of` and `is_unsigned` are
    `tac_to_asm`'s."""
    from passes.type_checking import LocalAttr

    addressed = {
        ins.operand.name for ins in fn.instructions
        if isinstance(ins, tac_ast.GetAddress)
        and isinstance(ins.operand, tac_ast.Var)
    }
    params = set(fn.params)
    defs: dict[str, list[tac_ast.Type_instruction]] = {}
    for ins in fn.instructions:
        for d in defs_in(ins):
            defs.setdefault(d.name, []).append(ins)
    tracked: dict[str, int] = {}
    for name in defs:
        sym = symbols.get(name) if symbols is not None else None
        if (sym is None or not isinstance(sym.attrs, LocalAttr)
                or name in addressed or name in params):
            continue
        tracked[name] = 0

    def count(val: tac_ast.Type_val) -> int:
        size = size_of(val)
        if isinstance(val, tac_ast.Constant):
            v = getattr(val.const, "value", None)
            if not isinstance(v, int) or v < 0:
                return size
            return min(size, (v.bit_length() + 7) // 8)
        if isinstance(val, tac_ast.Var) and val.name in tracked:
            return tracked[val.name]
        return size

    def of(ins: tac_ast.Type_instruction, size: int) -> int:
        match ins:
            case tac_ast.ZeroExtend(src=s) | tac_ast.Copy(src=s) \
                    | tac_ast.Truncate(src=s):
                return count(s)
            case tac_ast.Binary(op=tac_ast.BitwiseAnd(), src1=a, src2=b):
                return min(count(a), count(b))
            case tac_ast.Binary(op=tac_ast.BitwiseOr() | tac_ast.BitwiseXor(),
                                src1=a, src2=b):
                return max(count(a), count(b))
            case tac_ast.Binary(op=tac_ast.RightShift(), src1=a,
                                src2=tac_ast.Constant() as k):
                n = count(a)
                shift = getattr(k.const, "value", None)
                if not isinstance(shift, int) or shift < 0:
                    return size
                if n == size_of(a) and not is_unsigned(a):
                    return size
                return max(0, (8 * n - shift + 7) // 8)
            case tac_ast.Phi(args=args):
                return max((count(a.source) for a in args), default=size)
        return size

    changed = True
    while changed:
        changed = False
        for name in tracked:
            size = None
            n = 0
            for ins in defs[name]:
                dst = defs_in(ins)[0]
                size = size_of(dst)
                n = max(n, min(size, of(ins, size)))
            if n > tracked[name]:
                tracked[name] = n
                changed = True
    return {
        name: n for name, n in tracked.items()
        if n < size_of(tac_ast.Var(name=name))
    }
