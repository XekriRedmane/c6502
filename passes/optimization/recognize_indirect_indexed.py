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
mirror).

A constant offset `Binary(Add, ptr, k)` with `0 <= k <= 255` feeding
the Load / Store is recognized too (`_const_offset_chain`), with the
index a `ConstUChar(k)`. That is the shape of `p[10]` and of a
byte-sized struct member read through a pointer, and it lowers to
`LDY #k; LDA (DPTR),Y`. Its address may be read by several 1-byte
Loads and Stores (`_all_byte_accesses`), as in `p[1]++`, and each is
rewritten. With a constant offset the value may be wider than a byte,
as a 2-byte member `p->gold` is, when its last byte is at most 255
from `ptr` (`_access_width`). It lowers as `LDY #k; LDA (DPTR),Y`,
then `LDY #k+1` (which the asm peepholes make `INY`), and so on, in
place of an Add that computes `ptr + k` into a temporary and stages
it in DPTR. The pattern recognizer doesn't need to know whether ptr
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
  * The Load's `dst` (or Store's `src`) is a 1-byte typed val, or,
    for a constant index `k`, a value of `w` bytes with `k + w - 1 <=
    255`, so that Y reaches every byte.

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

import tac_ast
from passes.optimization.var_visit import count_uses
from passes.optimization.framework import (
    DefUsePass, DefUseEnv, Rewrite, PostFixedpointPass, PassContext,
    MatchResult, m_Store, m_Load, m_Var, m_Any, m_OneOf,
)
from passes.optimization.extend_add_chain import (
    recognize_extend_add_chain, is_1_byte_var, is_1_byte_val, all_dsts,
)


def _access_width(v: tac_ast.Type_val, symbols) -> int | None:
    """The width in bytes of the scalar value `v`, a Var by its c99
    type or a Constant by its variant, or None."""
    from passes.optimization.split_tables import _width
    import c99_ast
    if isinstance(v, tac_ast.Constant):
        return _CONST_WIDTHS.get(type(v.const))
    if not isinstance(v, tac_ast.Var):
        return None
    sym = symbols.get(v.name) if hasattr(symbols, "get") else None
    if sym is None:
        return None
    t = sym.type
    while isinstance(t, (c99_ast.Const, c99_ast.Volatile)):
        t = t.referenced_type
    return _width(t)


_CONST_WIDTHS = {
    tac_ast.ConstChar: 1, tac_ast.ConstUChar: 1,
    tac_ast.ConstInt: 2, tac_ast.ConstUInt: 2,
    tac_ast.ConstLong: 4, tac_ast.ConstULong: 4,
    tac_ast.ConstLongLong: 8, tac_ast.ConstULongLong: 8,
    tac_ast.ConstFloat: 4, tac_ast.ConstDouble: 8,
}


def _fits_constant_offset(
    index: tac_ast.Type_val, v: tac_ast.Type_val, symbols,
) -> bool:
    """True iff `v` is wider than a byte and `index` is a constant
    offset that reaches all of its bytes in Y (C99 member access at a
    constant offset through a pointer)."""
    if not isinstance(index, tac_ast.Constant):
        return False
    k = getattr(index.const, "value", None)
    w = _access_width(v, symbols)
    return isinstance(k, int) and w is not None and w > 1 and k + w - 1 <= 0xFF


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
        PassContext(ssa_dsts=all_dsts(fn), symbols=symbols),
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


class _RecognizeIndirectIndexedDefUse(DefUsePass):
    """DefUsePass: matches Load(src_ptr=Var) or Store(dst_ptr=Var) at
    the use site. For each match, walks back twice through the SSA
    def-chain to find the ZeroExtend → Binary(Add, ptr,
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
        return count_uses(fn.instructions)

    def rewrite(self, m: MatchResult, env: DefUseEnv, ctx: PassContext) -> object | None:
        chain = recognize_extend_add_chain(m.bindings['addr_var'], env, ctx)
        if chain is None:
            chain = _shared_extend_add_chain(m.bindings['addr_var'], env, ctx)
            if chain is not None:
                # The Add and the extend stay for the readers not yet
                # rewritten. Once every one is, nothing reads them,
                # and the asm byte DCE drops them.
                return self._finish(
                    m, ctx, chain.base, chain.idx_var, (), [], env,
                )
        if chain is None:
            moved = _offset_from_extend_chain(m, env, ctx)
            if moved is not None:
                ptr_var, idx_var, pre = moved
                return self._finish(m, ctx, ptr_var, idx_var, (), pre, env)
        if chain is not None:
            ptr_var, idx_var = chain.base, chain.idx_var
            drop = chain.drop_defs()
        else:
            const_chain = _const_offset_chain(m.bindings['addr_var'], env, ctx)
            if const_chain is None:
                return None
            ptr_var, idx_var = const_chain
            # With several readers, the Add stays for the ones not yet
            # rewritten, which find their chain through it. Once all of
            # them are, nothing reads it, and the asm byte DCE drops it.
            addr = m.bindings['addr_var']
            drop = (addr,) if env.extra.get(addr.name, 0) == 1 else ()
            folded = _byte_index_sum(ptr_var, idx_var, m, env, ctx)
            if folded is not None:
                ptr_var, idx_var, pre = folded
                return self._finish(m, ctx, ptr_var, idx_var, drop, pre, env)
        return self._finish(m, ctx, ptr_var, idx_var, drop, [], env)

    def _finish(self, m, ctx, ptr_var, idx_var, drop, pre, env=None):
        """The `Rewrite` of the matched access as an indexed one through
        `ptr_var` at `idx_var`, after the instructions `pre`, or None."""
        # (zp),Y needs a pointer Var base — a Constant base is the
        # absolute,X case handled by recognize_indexed_store/load.
        if not isinstance(ptr_var, tac_ast.Var):
            return None
        # Defer to recognize_indexed_* when the ptr resolves to a Constant.
        if env is not None and _resolves_to_constant(
            ptr_var, env.def_idx, env.instructions,
        ):
            return None
        # Determine Load or Store and check the access value's width.
        if 'load_instr' in m.bindings:
            load_dst = m.bindings['load_dst']
            if not (is_1_byte_var(load_dst, ctx.symbols)
                    or _fits_constant_offset(idx_var, load_dst, ctx.symbols)):
                return None
            replacement: tac_ast.Type_instruction = tac_ast.IndirectIndexedLoad(
                ptr=ptr_var, index=idx_var, dst=load_dst,
                is_volatile=m.bindings['load_instr'].is_volatile,
            )
        else:
            store_src = m.bindings['store_src']
            if not (is_1_byte_val(store_src, ctx.symbols)
                    or _fits_constant_offset(idx_var, store_src, ctx.symbols)):
                return None
            replacement = tac_ast.IndirectIndexedStore(
                ptr=ptr_var, index=idx_var, src=store_src,
                is_volatile=m.bindings['store_instr'].is_volatile,
            )
        # Drop the intermediate defs (the Binary(Add), and the
        # ZeroExtend when there is one) atomically, so no subsequent
        # DSE pass is needed.
        if pre:
            return Rewrite(replacement=[*pre, replacement], drop_defs=drop)
        return Rewrite(replacement=replacement, drop_defs=drop)


def _shared_extend_add_chain(
    addr_var: tac_ast.Var, env: DefUseEnv, ctx: PassContext,
):
    """The `ExtendAddChain` of `addr_var = ptr + ZeroExtend(i)` when
    `addr_var` has several readers and each is a 1-byte `Load` from it
    or `Store` to it (`_all_byte_accesses`), as in `p[i] &= m`, which
    reads and writes `p + i`. Each reader becomes `(p),Y` with Y = `i`,
    so `i` must be SSA-renamed to hold the Add's value at each one.
    Else None."""
    if env.extra.get(addr_var.name, 0) < 2:
        return None
    chain = recognize_extend_add_chain(addr_var, env, ctx, single_use=False)
    if chain is None or chain.idx_var.name not in ctx.ssa_dsts:
        return None
    if not _all_byte_accesses(addr_var, chain.base, env, ctx, offsets=True):
        return None
    return chain


def _offset_from_extend_chain(m, env: DefUseEnv, ctx: PassContext):
    """`(t, i, [Binary(Add, p, k, t)])` when the access reads or writes
    through `q + k`, its one reader, with `k` an integer constant that
    doesn't fit in a byte and `q = p + ZeroExtend(i)`, whose other
    readers, if any, all become `(p),Y` accesses
    (`_shared_extend_add_chain`). The access is then `(t),Y` with Y =
    `i` and `t = p + k`, a pointer plus a constant, and `q` itself goes
    once its other readers are rewritten. A `k` of 0 to 255 is left to
    `_const_offset_chain`, which reads `(q),Y` with Y = `k`, since
    several such offsets share `q` where each would need a pointer of
    its own here. `background[x]` with
    `background = line + 0x2000`, which CSE had turned into
    `(line + x) + 0x2000` to share `line + x` with `line[x]`, reads
    page 2 through its own pointer. `p` must hold at the access the
    value `q`'s Add read. Else None."""
    from passes.optimization.narrow_widened_arith import (
        _register_temp, var_unchanged,
    )
    if ctx.symbols is None or ctx.ssa_dsts is None:
        return None
    addr = m.bindings['addr_var']
    if addr.name not in ctx.ssa_dsts or env.extra.get(addr.name, 0) != 1:
        return None
    outer = env.def_of(addr)
    if not (isinstance(outer, tac_ast.Binary)
            and isinstance(outer.op, tac_ast.Add)):
        return None
    for q, k in ((outer.src1, outer.src2), (outer.src2, outer.src1)):
        if not (isinstance(q, tac_ast.Var) and isinstance(k, tac_ast.Constant)
                and q.name in ctx.ssa_dsts):
            continue
        value = getattr(k.const, "value", None)
        if not isinstance(value, int) or isinstance(
            k.const, (tac_ast.ConstFloat, tac_ast.ConstDouble),
        ):
            continue
        if 0 <= value <= 0xFF:
            # `(q),Y` with Y = k reads it without a second pointer.
            continue
        if env.extra.get(q.name, 0) >= 2:
            chain = _shared_extend_add_chain(q, env, ctx)
        else:
            chain = recognize_extend_add_chain(q, env, ctx)
            if chain is not None and chain.idx_var.name not in ctx.ssa_dsts:
                chain = None
        if chain is None or not isinstance(chain.base, tac_ast.Var):
            continue
        if _resolves_to_constant(chain.base, env.def_idx, env.instructions):
            continue
        access = m.bindings.get('load_instr') or m.bindings.get('store_instr')
        at = next((i for i, ins in enumerate(env.instructions)
                   if ins is access), None)
        q_idx = env.def_idx.get(q.name)
        if at is None or q_idx is None or not var_unchanged(
            chain.base, q_idx, at, env, ctx,
        ):
            continue
        name = f"{addr.name}.base"
        _register_temp(ctx, like=chain.base.name, name=name)
        t = tac_ast.Var(name=name)
        return t, chain.idx_var, [tac_ast.Binary(
            op=tac_ast.Add(), src1=chain.base, src2=k, dst=t,
        )]
    return None


def _const_offset_chain(
    addr_var: tac_ast.Var, env: DefUseEnv, ctx: PassContext,
) -> tuple[tac_ast.Type_val, tac_ast.Constant] | None:
    """`(ptr, Constant(k))` when `addr_var` is an SSA name defined by
    `Binary(Add, ptr, k)` (either operand order) with an integer
    constant `0 <= k <= 255`, else None. The access then lowers as
    `LDY #k; LDA (DPTR),Y`, because `(zp),Y` adds Y to the pointer
    with a carry into the high byte, which is the same 16-bit sum the
    Add computed. The index comes back as a `ConstUChar` so it
    satisfies the 1-byte index contract of the indexed nodes.

    `addr_var` may have several readers when each one is a 1-byte
    `Load` from it or `Store` to it (`_all_byte_accesses`), since each
    is rewritten the same way. `p[1]++` reads and writes `p + 1`."""
    if ctx.symbols is None or ctx.ssa_dsts is None:
        return None
    if addr_var.name not in ctx.ssa_dsts:
        return None
    binary = env.def_of(addr_var)
    if not isinstance(binary, tac_ast.Binary) or not isinstance(
        binary.op, tac_ast.Add,
    ):
        return None
    for ptr, k in ((binary.src1, binary.src2), (binary.src2, binary.src1)):
        if not isinstance(k, tac_ast.Constant) or isinstance(
            ptr, tac_ast.Constant,
        ):
            continue
        value = getattr(k.const, "value", None)
        if not isinstance(value, int) or isinstance(
            k.const, (tac_ast.ConstFloat, tac_ast.ConstDouble),
        ):
            continue
        if not 0 <= value <= 0xFF:
            continue
        index = tac_ast.Constant(const=tac_ast.ConstUChar(value=value))
        if env.extra.get(addr_var.name, 0) != 1 and not _all_byte_accesses(
            addr_var, ptr, env, ctx, index=index,
        ):
            return None
        return ptr, index
    return None


def _byte_max(v: tac_ast.Type_val, env: DefUseEnv, depth: int = 0) -> int:
    """The largest value the 1-byte `v` can hold, from its definition:
    a constant, a left shift by a constant (whose low bits are 0), a
    logical right shift or an AND with a constant, or a copy of one of
    those. 255 for anything else."""
    if isinstance(v, tac_ast.Constant):
        value = getattr(v.const, "value", None)
        return value & 0xFF if isinstance(value, int) else 0xFF
    if not isinstance(v, tac_ast.Var) or depth > 8:
        return 0xFF
    d = env.def_of(v)
    if isinstance(d, tac_ast.Copy):
        return _byte_max(d.src, env, depth + 1)
    if not isinstance(d, tac_ast.Binary) or not isinstance(d.src2, tac_ast.Constant):
        return 0xFF
    k = getattr(d.src2.const, "value", None)
    if not isinstance(k, int) or not 0 <= k < 8:
        if isinstance(d.op, tac_ast.BitwiseAnd) and isinstance(k, int):
            return k & 0xFF
        return 0xFF
    if isinstance(d.op, tac_ast.LeftShift):
        return (0xFF << k) & 0xFF
    if isinstance(d.op, tac_ast.RightShift):
        return 0xFF >> k
    if isinstance(d.op, tac_ast.BitwiseAnd):
        return k & 0xFF
    return 0xFF


def _byte_index_sum(ptr, k, m, env: DefUseEnv, ctx: PassContext):
    """`(base, t, [Binary(Add, b, k, t)])` when `ptr`, the pointer an
    access reads at the constant offset `k`, is `base + ZeroExtend(b)`
    for a 1-byte `b` whose largest value plus `k` fits in a byte, and
    `base` still holds at the access the value the Add read. Then `ptr
    + k` is `base + (b + k)` with no carry out of the byte sum, and the
    access is `(base),Y` with Y = `b + k`. In Zork's abbreviation lookup
    `Z_ABBREV_TABLE[i + 1]` with `i = z << 1`, which is even, became
    `INY` after the read of `Z_ABBREV_TABLE[i]`, where it had added `i`
    to the table's address in a zero-page pair. Else None."""
    from passes.optimization.narrow_widened_arith import (
        _register_temp, var_unchanged,
    )
    if ctx.symbols is None or ctx.ssa_dsts is None:
        return None
    if not isinstance(ptr, tac_ast.Var) or ptr.name not in ctx.ssa_dsts:
        return None
    # Only when the access is the sum's one reader, so its 16-bit add
    # goes. `entry += stride; ... entry[0]` in the loop of
    # `match_dictionary_word` still needs the new `entry`, and reading
    # through the old one at Y = stride kept both alive.
    if k.const.value == 0 or env.extra.get(ptr.name, 0) != 1:
        return None
    add = env.def_of(ptr)
    if not (isinstance(add, tac_ast.Binary) and isinstance(add.op, tac_ast.Add)):
        return None
    for base, ext in ((add.src1, add.src2), (add.src2, add.src1)):
        if not (isinstance(base, tac_ast.Var) and isinstance(ext, tac_ast.Var)
                and ext.name in ctx.ssa_dsts):
            continue
        z = env.def_of(ext)
        if not (isinstance(z, tac_ast.ZeroExtend) and isinstance(z.src, tac_ast.Var)
                and z.src.name in ctx.ssa_dsts
                and is_1_byte_var(z.src, ctx.symbols)):
            continue
        if _byte_max(z.src, env) + k.const.value > 0xFF:
            continue
        add_idx = env.def_idx.get(ptr.name)
        access = m.bindings.get('load_instr') or m.bindings.get('store_instr')
        at = next((i for i, ins in enumerate(env.instructions)
                   if ins is access), None)
        if add_idx is None or at is None or not var_unchanged(
            base, add_idx, at, env, ctx,
        ):
            continue
        name = f"{ptr.name}.y{k.const.value}"
        _register_temp(ctx, like=z.src.name, name=name)
        t = tac_ast.Var(name=name)
        return base, t, [tac_ast.Binary(
            op=tac_ast.Add(), src1=z.src, src2=k, dst=t,
        )]
    return None


def _all_byte_accesses(
    addr_var: tac_ast.Var, ptr: tac_ast.Type_val, env: DefUseEnv,
    ctx: PassContext, offsets: bool = False,
    index: tac_ast.Type_val | None = None,
) -> bool:
    """True iff every read of `addr_var` is the pointer of a `Load`
    into a 1-byte Var or of a `Store` of a 1-byte value, and `ptr`
    still holds at each one the value the Add read. With the constant
    `index` the Add adds, a wider value whose bytes Y reaches counts
    too (`_fits_constant_offset`), as in `p->gold += 5`. Then every reader
    becomes an indexed access, and nothing reads the Add. A non-SSA `ptr`, such
    as a global, must be unchanged from the Add to each access
    (`narrow_widened_arith.var_unchanged`). With `offsets`, a reader
    may also be `addr_var + k` for a constant `k` outside 0 to 255
    whose one reader is such an access, which
    `_offset_from_extend_chain` rewrites."""
    from passes.optimization.narrow_widened_arith import var_unchanged
    from passes.optimization.var_visit import uses_in
    add_idx = env.def_idx.get(addr_var.name)
    if add_idx is None:
        return False
    seen = 0
    for i, instr in enumerate(env.instructions):
        reads = sum(1 for u in uses_in(instr) if u.name == addr_var.name)
        if not reads:
            continue
        if isinstance(instr, tac_ast.Load) and instr.src_ptr == addr_var:
            ok = (is_1_byte_var(instr.dst, ctx.symbols)
                  or (index is not None and _fits_constant_offset(
                      index, instr.dst, ctx.symbols)))
        elif isinstance(instr, tac_ast.Store) and instr.dst_ptr == addr_var:
            ok = (instr.src != addr_var
                  and (is_1_byte_val(instr.src, ctx.symbols)
                       or (index is not None and _fits_constant_offset(
                           index, instr.src, ctx.symbols))))
        elif offsets:
            at = _offset_access(instr, addr_var, env, ctx)
            ok = at is not None
            if ok and at >= 0 and isinstance(ptr, tac_ast.Var) and not var_unchanged(
                ptr, add_idx, at, env, ctx,
            ):
                return False
        else:
            ok = False
        if not ok or reads != 1:
            return False
        if isinstance(ptr, tac_ast.Var) and not var_unchanged(
            ptr, add_idx, i, env, ctx,
        ):
            return False
        seen += 1
    # `env.instructions` holds the rewrites made so far in this run,
    # so an access already rewritten no longer reads `addr_var`.
    return seen > 0


def _offset_access(
    instr, addr_var: tac_ast.Var, env: DefUseEnv, ctx: PassContext,
) -> int | None:
    """The index of the one reader of `instr`'s dst when `instr` is
    `Binary(Add, addr_var, k)` (either order) with an integer constant
    `k` outside 0 to 255, its dst is SSA-renamed, and that reader is a 1-byte `Load`
    from the dst or `Store` to it, or -1 when that access has been
    rewritten already in this run and nothing reads the dst. Else
    None."""
    from passes.optimization.var_visit import uses_in
    if not (isinstance(instr, tac_ast.Binary) and isinstance(instr.op, tac_ast.Add)):
        return None
    other = instr.src2 if instr.src1 == addr_var else instr.src1
    if not isinstance(other, tac_ast.Constant):
        return None
    value = getattr(other.const, "value", None)
    if not isinstance(value, int) or isinstance(
        other.const, (tac_ast.ConstFloat, tac_ast.ConstDouble),
    ) or 0 <= value <= 0xFF:
        return None
    dst = instr.dst
    if not (isinstance(dst, tac_ast.Var) and dst.name in ctx.ssa_dsts
            and env.extra.get(dst.name, 0) == 1):
        return None
    for i, use in enumerate(env.instructions):
        if not any(u.name == dst.name for u in uses_in(use)):
            continue
        if isinstance(use, tac_ast.Load) and use.src_ptr == dst:
            return i if is_1_byte_var(use.dst, ctx.symbols) else None
        if isinstance(use, tac_ast.Store) and use.dst_ptr == dst:
            return i if (use.src != dst
                         and is_1_byte_val(use.src, ctx.symbols)) else None
        return None
    # The access has been rewritten already, so nothing reads the Add.
    return -1


_IMPL = _RecognizeIndirectIndexedDefUse()


class RecognizeIndirectIndexed(PostFixedpointPass):
    """PostFixedpointPass wrapper. Delegates to _IMPL which uses
    Rewrite.drop_defs to atomically drop the two dead intermediate
    defs (Binary(Add) and ZeroExtend) in the same rebuild
    pass. This is sound even in PostFixedpointPass context — there
    is no need for a subsequent DSE pass."""
    name = "recognize_indirect_indexed"

    def run(self, fn, ctx):
        return _IMPL.run(fn, ctx)
