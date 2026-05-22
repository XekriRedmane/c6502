"""TAC pass that recognizes the indirect-(zp),Y access pattern.

For `ptr[i]` where `ptr` is a Pointer-typed local (or zp_abi parameter)
and `i` is a 1-byte typed integer, c99_to_tac lowers to:

    ZeroExtend(i,            %ext)        # i uchar → 2-byte
    Binary(Add, ptr, %ext,   %addr)       # 16-bit ptr arithmetic
    Load(%addr,              dst)         # read 1 byte

The 6502's indirect-indexed addressing mode `(zp),Y` does the same
`ptr + Y` arithmetic for free, with `Y = i`. So when `ptr` is (or
ends up at) a zero-page address, this collapses to:

    LDY i ; LDA (ptr_zp),Y ; STA dst        (~3 instructions / 5 bytes)

vs the original ~10 instructions / ~21 bytes (16-bit Add chain plus
DPTR setup plus indirect-Y read with Y=0). Even when ptr can't be
ZP-resident, lowering through DPTR with `Y=i` (instead of
`Y=0` and a precomputed sum) lets the asm-level forward copy
propagation hoist the loop-invariant DPTR setup out of an unrolled
body, which the current shape can't do because the DPTR contents
include the per-iteration index.

This pass detects the three-instruction TAC pattern and rewrites
to a single `IndirectIndexedLoad(ptr, index, dst)` (or the Store
mirror). The pattern recognizer doesn't need to know whether ptr
will end up in ZP — that decision happens in tac_to_asm and asm
regalloc.

# Eligibility

The fusion fires when:

  * The Load's `src_ptr` (or Store's `dst_ptr`) is a single-use
    Pseudo `%addr` defined by an `Add` of a Var `ptr` and another
    Pseudo `%ext`.
  * `ptr` is a Var (not a Constant — that pattern is the
    `recognize_indexed_load` / `_store` case).
  * `%ext` is itself a single-use Pseudo defined by
    `ZeroExtend(index_var, %ext)`.
  * `index_var`'s c99 type is a 1-byte type (Char / SChar / UChar).
    The high byte of the zero-extended index is provably zero, so
    setting `Y = index_var` is equivalent to adding `index_var` to
    the pointer's low byte and propagating any carry — i.e., the
    6502's `(zp),Y` semantics match exactly.
  * The Load's `dst` (or Store's `src`) is a 1-byte typed val. A
    multi-byte access would require multiple `LDA (zp),Y; INY` pairs
    with carry-safe iteration — deferred until a motivating case
    appears.

The c99 type of `ptr` is NOT checked here: the type checker
already verified the Binary is pointer arithmetic (because the
result feeds a Load/Store), and the only legal addends are a
pointer plus an integer. The Multiply that pointer arithmetic
would emit for `sizeof(pointee) > 1` would break the recognizer's
shape (it'd see `Binary(Multiply, ...)` instead of the ZeroExtend
result directly), so the by-1 scaling condition is implicit in
the structural match.

# Soundness

The single-use checks guarantee no other reader observes the
intermediate values, so removing the temps doesn't change
semantics. The high byte of `ZeroExtend(uchar, %ext)` is provably
zero, so `(ptr + ZeroExtend(i))` equals `(ptr + i)` as a 16-bit
sum, equals `*(ptr),Y` with `Y = i` on the 6502 (which adds Y to
the low byte and carries into the high byte). The 1-byte access
constraint matches the single LDA / STA the lowering emits.
"""

from __future__ import annotations

from collections import Counter

import c99_ast
import tac_ast
from passes.optimization.var_visit import uses_in
from passes.optimization.framework import (
    DefUsePass, DefUseEnv, Rewrite, PostFixedpointPass, PassContext,
    MatchResult, m_Store, m_Load, m_Var, m_Any, m_OneOf,
)


def recognize_indirect_indexed(
    fn: tac_ast.Function, *, symbols=None,
) -> tac_ast.Function:
    """Walk `fn`'s instructions; for each foldable triple
    (`ZeroExtend; Binary(Add); Load|Store`), splice in an
    `IndirectIndexedLoad` / `IndirectIndexedStore` and drop the
    two intermediate def instructions. Without `symbols`, the pass
    is a no-op (we need symbol-table types to verify operand widths)."""
    if symbols is None:
        return fn
    return _IMPL.run(
        fn,
        PassContext(ssa_dsts=_all_dsts(fn), symbols=symbols),
    )


def _all_dsts(fn: tac_ast.Function) -> set[str]:
    """Return the set of all Var dst names in `fn`. Used as ssa_dsts
    when calling from the free function — the free function is invoked
    from the PostFixedpointPass context where SSA has already been
    constructed, so all temps are uniquely defined."""
    out: set[str] = set()
    for instr in fn.instructions:
        if hasattr(instr, 'dst') and isinstance(instr.dst, tac_ast.Var):
            out.add(instr.dst.name)
    return out


def _count_uses(
    instrs: list[tac_ast.Type_instruction],
) -> Counter[str]:
    counts: Counter[str] = Counter()
    for instr in instrs:
        for v in uses_in(instr):
            counts[v.name] += 1
    return counts


def _split_var_var(
    a: tac_ast.Type_val,
    b: tac_ast.Type_val,
    def_idx: dict[str, int],
    all_instrs: list,
) -> tuple[tac_ast.Var | None, tac_ast.Var | None]:
    """Given the two operands of the address-computing Add, return
    `(ptr_var, ext_var)` where `ext_var` is the side defined by a
    `ZeroExtend` or `SignExtend` and `ptr_var` is the other side.
    The Add is commutative so we accept either argument order.
    Returns (None, None) if neither side fits."""
    if isinstance(a, tac_ast.Var) and isinstance(b, tac_ast.Var):
        if _defined_by_extend(b, def_idx, all_instrs):
            return (a, b)
        if _defined_by_extend(a, def_idx, all_instrs):
            return (b, a)
    return (None, None)


def _defined_by_extend(
    v: tac_ast.Var,
    def_idx: dict[str, int],
    all_instrs: list,
) -> bool:
    idx = def_idx.get(v.name)
    if idx is None:
        return False
    return isinstance(
        all_instrs[idx], (tac_ast.ZeroExtend, tac_ast.SignExtend),
    )


def _resolves_to_constant(
    v: tac_ast.Var,
    def_idx: dict[str, int],
    all_instrs: list,
) -> bool:
    """True iff `v`'s SSA def chain ends in a Constant. Follows
    `Copy` chains of arbitrary depth; gives up on non-Copy defs.
    Used to defer to the IndexedLoad/Store recognizers when the
    pointer-side operand is just a Var-wrapped Constant."""
    seen: set[str] = set()
    cur = v
    while True:
        if cur.name in seen:
            return False
        seen.add(cur.name)
        idx = def_idx.get(cur.name)
        if idx is None:
            return False
        d = all_instrs[idx]
        if not isinstance(d, tac_ast.Copy):
            return False
        src = d.src
        if isinstance(src, tac_ast.Constant):
            return True
        if isinstance(src, tac_ast.Var):
            cur = src
            continue
        return False


def _is_1_byte_var(v: tac_ast.Var, symbols) -> bool:
    sym = symbols.get(v.name) if hasattr(symbols, "get") else None
    if sym is None:
        return False
    t = sym.type
    while isinstance(t, (c99_ast.Const, c99_ast.Volatile)):
        t = t.referenced_type
    return isinstance(t, (c99_ast.Char, c99_ast.SChar, c99_ast.UChar))


def _is_1_byte_val(v: tac_ast.Type_val, symbols) -> bool:
    if isinstance(v, tac_ast.Constant):
        return isinstance(
            v.const, (tac_ast.ConstChar, tac_ast.ConstUChar),
        )
    if isinstance(v, tac_ast.Var):
        return _is_1_byte_var(v, symbols)
    return False


class _RecognizeIndirectIndexedDefUse(DefUsePass):
    """DefUsePass: matches Load(src_ptr=Var) or Store(dst_ptr=Var) at
    the use site. For each match, walks back twice through the SSA
    def-chain to find the ZeroExtend/SignExtend → Binary(Add, ptr,
    %ext) → Load/Store pattern. Uses the full def-idx so the use and
    its producers need not be index-adjacent.

    Returns Rewrite(replacement, drop_defs=(addr_var, ext_var)) so the
    two intermediate defs are dropped atomically in the same rebuild
    pass — no subsequent DSE pass is needed to clean them up."""
    name = "recognize_indirect_indexed"
    pattern = m_OneOf(
        m_Load(
            src_ptr=m_Var(capture='addr_var'),
            dst=m_Var(capture='load_dst'),
            capture='load_instr',
        ),
        m_Store(
            src=m_Any(capture='store_src'),
            dst_ptr=m_Var(capture='addr_var'),
            capture='store_instr',
        ),
    )

    def prepare_extra(self, fn, ctx):
        return _count_uses(fn.instructions)

    def rewrite(self, m: MatchResult, env: DefUseEnv, ctx: PassContext) -> object | None:
        if ctx.symbols is None:
            return None
        addr_var = m.bindings['addr_var']
        # addr_var must be SSA-renamed (single-def guarantee).
        if ctx.ssa_dsts is None or addr_var.name not in ctx.ssa_dsts:
            return None
        # Single-use gate on %addr.
        if env.extra.get(addr_var.name, 0) != 1:
            return None
        # Walk back to the Binary(Add) def.
        binary = env.def_of(addr_var)
        if not isinstance(binary, tac_ast.Binary) or not isinstance(binary.op, tac_ast.Add):
            return None
        # Both operands must be Vars (not Constant + Var — that's the
        # absolute,X pattern handled by recognize_indexed_store/load).
        ptr_var, ext_var = _split_var_var(
            binary.src1, binary.src2, env.def_idx, env.instructions,
        )
        if ptr_var is None or ext_var is None:
            return None
        # Defer to recognize_indexed_* when ptr resolves to a Constant.
        if _resolves_to_constant(ptr_var, env.def_idx, env.instructions):
            return None
        # ext_var must be SSA-renamed and single-use.
        if ext_var.name not in ctx.ssa_dsts:
            return None
        if env.extra.get(ext_var.name, 0) != 1:
            return None
        # Walk back to the ZeroExtend/SignExtend def.
        ext = env.def_of(ext_var)
        if not isinstance(ext, (tac_ast.ZeroExtend, tac_ast.SignExtend)):
            return None
        if not isinstance(ext.src, tac_ast.Var):
            return None
        idx_var = ext.src
        if not _is_1_byte_var(idx_var, ctx.symbols):
            return None
        # Determine Load or Store and check the access value's width.
        if 'load_instr' in m.bindings:
            load_dst = m.bindings['load_dst']
            if not _is_1_byte_var(load_dst, ctx.symbols):
                return None
            replacement: tac_ast.Type_instruction = tac_ast.IndirectIndexedLoad(
                ptr=ptr_var, index=idx_var, dst=load_dst,
                is_volatile=m.bindings['load_instr'].is_volatile,
            )
        else:
            store_src = m.bindings['store_src']
            if not _is_1_byte_val(store_src, ctx.symbols):
                return None
            replacement = tac_ast.IndirectIndexedStore(
                ptr=ptr_var, index=idx_var, src=store_src,
                is_volatile=m.bindings['store_instr'].is_volatile,
            )
        # Drop the two intermediate defs (Binary(Add) and ZeroExtend)
        # atomically — no subsequent DSE pass needed.
        return Rewrite(replacement=replacement, drop_defs=(addr_var, ext_var))


_IMPL = _RecognizeIndirectIndexedDefUse()


class RecognizeIndirectIndexed(PostFixedpointPass):
    """PostFixedpointPass wrapper. Delegates to _IMPL which uses
    Rewrite.drop_defs to atomically drop the two dead intermediate
    defs (Binary(Add) and ZeroExtend/SignExtend) in the same rebuild
    pass. This is sound even in PostFixedpointPass context — there
    is no need for a subsequent DSE pass."""
    name = "recognize_indirect_indexed"

    def run(self, fn, ctx):
        return _IMPL.run(fn, ctx)
