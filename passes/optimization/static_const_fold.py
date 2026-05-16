"""TAC-level scalar const-static read fold.

Replace `Var(name)` USE positions with `Constant(value)` when `name`
is a static-storage object that's:

  * const-qualified at the top level (`Const(...)` wrapper on its
    symbol-table type),
  * scalar (one of the integer types, Float, Double, or Pointer —
    not Array, Structure, or Union),
  * initialized with a literal `Initial(int_or_float)` value (NOT
    `AddressInit`, which is link-time-resolved and can't be a
    Constant; NOT a tuple, which is an aggregate).

The c99 type system already rejects writes to `const` lvalues, so
in single-TU c6502 the value is genuinely fixed at link time. Both
internal- and external-linkage const statics qualify (other TUs
could read but not write an external-linkage const). The
asm-level `fold_const_statics` (`passes/optimization_asm/const_
static_fold.py`) drops the `StaticVariable` storage when nobody
references it; this TAC-level pass eliminates the runtime reads
that previously kept those references alive.

The bigger win, though, is enabling downstream constant folding:
once `Var(hires_page1)` becomes `Constant(0x2000)` and
`IndexedLoad(interlace_p1_offsets, Constant(2))` becomes
`Constant(0x01D0)` (handled by the const-array-subscript fold in
`constant_folding.py`), `Binary(Add, Constant(0x2000),
Constant(0x01D0))` collapses to `Constant(0x21D0)` via the
existing constant_fold pass — turning a multi-step runtime
address computation into a single immediate.

Runs once before the TAC fixed-point loop. The replacement is
purely USE-position; defs (which for these statics shouldn't
exist anyway, since they're const) and `IndexedLoad.name`
references (which name an array, not read its value) are left
alone.
"""

from __future__ import annotations

import c99_ast
import tac_ast
from c99_to_tac import _tac_const_for
from passes.type_checking import (
    AddressInit, Initial, StaticAttr, SymbolTable,
)


def fold_static_const_reads(
    fn: tac_ast.Function, symbols: SymbolTable,
) -> tac_ast.Function:
    """Walk `fn`'s instructions, replace `Var(name)` USE operands
    with `Constant(value)` where `name` is a foldable scalar const
    static (per the module docstring's eligibility rules). Returns
    a new Function; doesn't mutate the input."""
    cache = _build_cache(symbols)
    if not cache:
        return fn
    new_instrs = [_rewrite_instr(i, cache) for i in fn.instructions]
    return tac_ast.Function(
        name=fn.name, is_global=fn.is_global,
        params=list(fn.params), instructions=new_instrs,
    )


def _build_cache(
    symbols: SymbolTable,
) -> dict[str, tac_ast.Constant]:
    """Pre-compute the foldable-name → Constant map. Walks the
    symbol table once; the per-instruction rewriter just looks up
    each Var's name."""
    cache: dict[str, tac_ast.Constant] = {}
    for name, sym in symbols.items():
        if not isinstance(sym.attrs, StaticAttr):
            continue
        if not isinstance(sym.attrs.initial_value, Initial):
            continue
        v = sym.attrs.initial_value.value
        if isinstance(v, (AddressInit, tuple)):
            # AddressInit: link-time symbol. Tuple: aggregate. Neither
            # can be folded to a TAC Constant.
            continue
        if not isinstance(v, (int, float)):
            continue
        scalar_t = _scalar_type(sym.type)
        if scalar_t is None:
            continue
        cache[name] = tac_ast.Constant(
            const=_tac_const_for(scalar_t, v),
        )
    return cache


def _scalar_type(t):
    """Return the underlying scalar type if `t` is a const-qualified
    (and NOT volatile-qualified) scalar, else None. The const wrapper
    is the eligibility gate — we don't fold non-const statics (they
    could legally be modified at runtime, even if the program doesn't
    happen to). Volatile statics are also rejected even when const-
    qualified: per C99 §6.7.3.6, every access to a volatile object
    is a side effect, so folding two `Var(...)` reads to a single
    `Constant` would erase those side effects."""
    has_const = False
    inner = t
    while isinstance(inner, (c99_ast.Const, c99_ast.Volatile)):
        if isinstance(inner, c99_ast.Volatile):
            return None
        has_const = True
        inner = inner.referenced_type
    if not has_const:
        return None
    if isinstance(inner, (
        c99_ast.Char, c99_ast.SChar, c99_ast.UChar,
        c99_ast.Int, c99_ast.UInt,
        c99_ast.Long, c99_ast.ULong,
        c99_ast.LongLong, c99_ast.ULongLong,
        c99_ast.Float, c99_ast.Double,
        c99_ast.Pointer,
    )):
        return inner
    return None


def _rewrite_instr(
    instr: tac_ast.Type_instruction,
    cache: dict[str, tac_ast.Constant],
) -> tac_ast.Type_instruction:
    """Rewrite USE-position Var operands in `instr` to their
    cached Constant. DEF-position operands and operand fields that
    aren't Var values (e.g. `IndexedLoad.name`) are left alone."""

    def sub(v: tac_ast.Type_val) -> tac_ast.Type_val:
        if isinstance(v, tac_ast.Var) and v.name in cache:
            return cache[v.name]
        return v

    match instr:
        case tac_ast.Ret(val=v) if v is not None:
            return tac_ast.Ret(val=sub(v))
        case tac_ast.SignExtend(src=s, dst=d):
            return tac_ast.SignExtend(src=sub(s), dst=d)
        case tac_ast.ZeroExtend(src=s, dst=d):
            return tac_ast.ZeroExtend(src=sub(s), dst=d)
        case tac_ast.Truncate(src=s, dst=d):
            return tac_ast.Truncate(src=sub(s), dst=d)
        case tac_ast.IntToFloat(src=s, dst=d):
            return tac_ast.IntToFloat(src=sub(s), dst=d)
        case tac_ast.IntToDouble(src=s, dst=d):
            return tac_ast.IntToDouble(src=sub(s), dst=d)
        case tac_ast.FloatToInt(src=s, dst=d):
            return tac_ast.FloatToInt(src=sub(s), dst=d)
        case tac_ast.DoubleToInt(src=s, dst=d):
            return tac_ast.DoubleToInt(src=sub(s), dst=d)
        case tac_ast.FloatToDouble(src=s, dst=d):
            return tac_ast.FloatToDouble(src=sub(s), dst=d)
        case tac_ast.DoubleToFloat(src=s, dst=d):
            return tac_ast.DoubleToFloat(src=sub(s), dst=d)
        case tac_ast.GetAddress(operand=o, dst=d):
            # GetAddress.operand names a storage cell (its address
            # is what we want); folding its value would be wrong.
            return instr
        case tac_ast.Load(src_ptr=p, dst=d, is_volatile=v):
            return tac_ast.Load(src_ptr=sub(p), dst=d, is_volatile=v)
        case tac_ast.Store(src=s, dst_ptr=p, is_volatile=v):
            return tac_ast.Store(
                src=sub(s), dst_ptr=sub(p), is_volatile=v,
            )
        case tac_ast.IndexedLoad(name=n, index=idx, dst=d, is_volatile=v):
            # IndexedLoad.name is the array's symbol identifier, not
            # a value — leave it alone. Only the index is a USE.
            return tac_ast.IndexedLoad(
                name=n, index=sub(idx), dst=d, is_volatile=v,
            )
        case tac_ast.IndexedStore(address=a, index=idx, src=s, is_volatile=v):
            return tac_ast.IndexedStore(
                address=a, index=sub(idx), src=sub(s), is_volatile=v,
            )
        case tac_ast.IndexedSymbolStore(name=n, index=idx, src=s, is_volatile=v):
            return tac_ast.IndexedSymbolStore(
                name=n, index=sub(idx), src=sub(s), is_volatile=v,
            )
        case tac_ast.Unary(op=op, src=s, dst=d):
            return tac_ast.Unary(op=op, src=sub(s), dst=d)
        case tac_ast.Binary(op=op, src1=s1, src2=s2, dst=d):
            return tac_ast.Binary(
                op=op, src1=sub(s1), src2=sub(s2), dst=d,
            )
        case tac_ast.Copy(src=s, dst=d):
            return tac_ast.Copy(src=sub(s), dst=d)
        case tac_ast.JumpIfTrue(condition=c, target=t):
            return tac_ast.JumpIfTrue(condition=sub(c), target=t)
        case tac_ast.JumpIfFalse(condition=c, target=t):
            return tac_ast.JumpIfFalse(condition=sub(c), target=t)
        case tac_ast.JumpIfCmp(op=op, src1=s1, src2=s2, target=t):
            return tac_ast.JumpIfCmp(
                op=op, src1=sub(s1), src2=sub(s2), target=t,
            )
        case tac_ast.JumpIfMasked(
            val=v, mask=m, jump_when_nonzero=jnz, target=t,
        ):
            return tac_ast.JumpIfMasked(
                val=sub(v), mask=m,
                jump_when_nonzero=jnz, target=t,
            )
        case tac_ast.FunctionCall(name=n, args=args, dst=d):
            return tac_ast.FunctionCall(
                name=n, args=[sub(a) for a in args], dst=d,
            )
        case tac_ast.IndirectCall(ptr=p, args=args, dst=d):
            return tac_ast.IndirectCall(
                ptr=sub(p), args=[sub(a) for a in args], dst=d,
            )
        case tac_ast.Phi(dst=d, args=args):
            return tac_ast.Phi(
                dst=d,
                args=[
                    tac_ast.PhiArg(
                        pred_label=a.pred_label, source=sub(a.source),
                    )
                    for a in args
                ],
            )
    return instr
