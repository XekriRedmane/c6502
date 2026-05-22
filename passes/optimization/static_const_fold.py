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
from passes.optimization.framework import (
    OperandRewritePass, PreFixedpointPass, PassContext,
    m_Var,
)
from passes.optimization.framework.patterns import MatchResult


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


class _FoldStaticConstReadsImpl(OperandRewritePass):
    name = "fold_static_const_reads"
    operand_pattern = m_Var(capture='var')

    def prepare(self, fn, ctx):
        if ctx.symbols is None:
            return {}
        return _build_cache(ctx.symbols)

    def rewrite_operand(self, m: MatchResult, cache, ctx: PassContext):
        var = m.bindings['var']
        return cache.get(var.name)  # Constant or None


_IMPL = _FoldStaticConstReadsImpl()


def fold_static_const_reads(
    fn: tac_ast.Function, symbols: SymbolTable,
) -> tac_ast.Function:
    """Walk `fn`'s instructions, replace `Var(name)` USE operands
    with `Constant(value)` where `name` is a foldable scalar const
    static (per the module docstring's eligibility rules). Returns
    a new Function; doesn't mutate the input."""
    return _IMPL.run(fn, PassContext(symbols=symbols))


class FoldStaticConstReads(PreFixedpointPass):
    name = "fold_static_const_reads"

    def run(self, fn, ctx):
        return _IMPL.run(fn, ctx)
