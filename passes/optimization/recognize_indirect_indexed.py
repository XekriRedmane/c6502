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


def recognize_indirect_indexed(
    fn: tac_ast.Function, *, symbols=None,
) -> tac_ast.Function:
    """Walk `fn`'s instructions; for each foldable triple
    (`ZeroExtend; Binary(Add); Load|Store`), splice in an
    `IndirectIndexedLoad` / `IndirectIndexedStore` and drop the
    three original instructions. Without `symbols`, the pass is a
    no-op (we need symbol-table types to verify operand widths)."""
    if symbols is None:
        return fn
    use_counts = _count_uses(fn.instructions)
    def_idx: dict[str, int] = {}
    for i, instr in enumerate(fn.instructions):
        for d in _defs(instr):
            def_idx[d.name] = i
    rewrites: dict[int, tac_ast.Type_instruction] = {}
    dropped: set[int] = set()
    for i, instr in enumerate(fn.instructions):
        rewritten = _try_recognize(
            instr, fn.instructions, def_idx, use_counts, symbols,
        )
        if rewritten is not None:
            replacement, dropped_indices = rewritten
            if dropped_indices & dropped:
                continue
            rewrites[i] = replacement
            dropped.update(dropped_indices)
    new_instrs: list[tac_ast.Type_instruction] = []
    for i, instr in enumerate(fn.instructions):
        if i in dropped:
            continue
        new_instrs.append(rewrites.get(i, instr))
    return tac_ast.Function(
        name=fn.name, is_global=fn.is_global,
        params=list(fn.params), instructions=new_instrs,
    )


def _count_uses(
    instrs: list[tac_ast.Type_instruction],
) -> Counter[str]:
    counts: Counter[str] = Counter()
    for instr in instrs:
        for v in uses_in(instr):
            counts[v.name] += 1
    return counts


def _defs(instr: tac_ast.Type_instruction):
    """Single-dst defs. Used to build a name → defining-instruction-
    index map. Mirrors the analogous helper in
    `recognize_indexed_store.py`."""
    match instr:
        case tac_ast.SignExtend(dst=d) | tac_ast.ZeroExtend(dst=d) \
                | tac_ast.Truncate(dst=d) \
                | tac_ast.IntToFloat(dst=d) | tac_ast.IntToDouble(dst=d) \
                | tac_ast.FloatToInt(dst=d) | tac_ast.DoubleToInt(dst=d) \
                | tac_ast.FloatToDouble(dst=d) | tac_ast.DoubleToFloat(dst=d) \
                | tac_ast.Unary(dst=d) | tac_ast.Binary(dst=d) \
                | tac_ast.Copy(dst=d) \
                | tac_ast.GetAddress(dst=d) \
                | tac_ast.Load(dst=d) \
                | tac_ast.IndexedLoad(dst=d) \
                | tac_ast.IndexedConstLoad(dst=d) \
                | tac_ast.IndirectIndexedLoad(dst=d) \
                | tac_ast.Phi(dst=d):
            if isinstance(d, tac_ast.Var):
                yield d
        case tac_ast.FunctionCall(dst=d) | tac_ast.IndirectCall(dst=d):
            if d is not None and isinstance(d, tac_ast.Var):
                yield d


def _try_recognize(
    instr: tac_ast.Type_instruction,
    all_instrs: list[tac_ast.Type_instruction],
    def_idx: dict[str, int],
    use_counts: Counter[str],
    symbols,
) -> tuple[tac_ast.Type_instruction, set[int]] | None:
    """If `instr` is the foldable `Load`/`Store` head of an
    eligible triple, return the IndirectIndexedLoad/Store
    replacement plus the set of original-instruction indices to
    drop. Otherwise None."""
    if isinstance(instr, tac_ast.Load):
        if not isinstance(instr.dst, tac_ast.Var):
            return None
        if not _is_1_byte_var(instr.dst, symbols):
            return None
        addr_val = instr.src_ptr
        head_kind = "load"
        head_value_for_check = instr.dst  # already verified 1-byte
        head_is_volatile = instr.is_volatile
    elif isinstance(instr, tac_ast.Store):
        if not _is_1_byte_val(instr.src, symbols):
            return None
        addr_val = instr.dst_ptr
        head_kind = "store"
        head_value_for_check = instr.src
        head_is_volatile = instr.is_volatile
    else:
        return None
    if not isinstance(addr_val, tac_ast.Var):
        return None
    addr_name = addr_val.name
    if use_counts.get(addr_name, 0) != 1:
        return None
    addr_def_idx = def_idx.get(addr_name)
    if addr_def_idx is None:
        return None
    addr_def = all_instrs[addr_def_idx]
    if not (
        isinstance(addr_def, tac_ast.Binary)
        and isinstance(addr_def.op, tac_ast.Add)
    ):
        return None
    ptr_var, ext_var = _split_var_var(addr_def.src1, addr_def.src2, def_idx, all_instrs)
    if ptr_var is None or ext_var is None:
        return None
    # Defer to `recognize_indexed_store` / `recognize_indexed_load`
    # when the pointer-side Var transitively holds a Constant.
    # Forcing the indirect-(zp),Y form here would lock in DPTR-
    # staging-via-IndirectIndexed for what's really an absolute,X
    # pattern, which IndexedStore lowers more cheaply
    # (`STA $C,X` vs. `STA (DPTR),Y` plus the staging).
    if _resolves_to_constant(ptr_var, def_idx, all_instrs):
        return None
    if use_counts.get(ext_var.name, 0) != 1:
        return None
    ext_def_idx = def_idx[ext_var.name]
    ext_def = all_instrs[ext_def_idx]
    # `_split_var_var` already verified that `ext_var` is defined
    # by a ZeroExtend or SignExtend; pull the source. SignExtend is
    # accepted under the same UB-permissive reasoning as in
    # `recognize_indexed_load` — (zp),Y addressing observes only
    # the index's low byte, and negative array indices are C99
    # §6.5.6 undefined.
    assert isinstance(ext_def, (tac_ast.ZeroExtend, tac_ast.SignExtend))
    if not isinstance(ext_def.src, tac_ast.Var):
        return None
    idx_var = ext_def.src
    if not _is_1_byte_var(idx_var, symbols):
        return None
    # Build the replacement. The collapsed instruction inherits
    # the original Load/Store's `is_volatile` bit.
    if head_kind == "load":
        replacement: tac_ast.Type_instruction = tac_ast.IndirectIndexedLoad(
            ptr=ptr_var, index=idx_var, dst=head_value_for_check,
            is_volatile=head_is_volatile,
        )
    else:
        replacement = tac_ast.IndirectIndexedStore(
            ptr=ptr_var, index=idx_var, src=head_value_for_check,
            is_volatile=head_is_volatile,
        )
    return (replacement, {addr_def_idx, ext_def_idx})


def _split_var_var(
    a: tac_ast.Type_val,
    b: tac_ast.Type_val,
    def_idx: dict[str, int],
    all_instrs: list[tac_ast.Type_instruction],
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
    all_instrs: list[tac_ast.Type_instruction],
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
    all_instrs: list[tac_ast.Type_instruction],
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
