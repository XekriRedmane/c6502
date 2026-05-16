"""TAC pass that recognizes the absolute,X-store pattern.

After the const-static-read fold, const-array-subscript fold, and
constant-Add reassociation, an expression like

    hires_page1[interlace_p1_offsets[2] + col] = value;

(with both `hires_page1` and `interlace_p1_offsets` const-qualified
statics) lowers to TAC

    ZeroExtend(col,         %ext)        # col uchar → 2-byte
    Binary(Add, C, %ext,    %addr)        # 16-bit ptr arithmetic
    Store(value,            %addr)        # write the byte

where C is a numeric Constant (the folded `hires_page1 +
interlace_p1_offsets[2]`). The 6502 has an addressing mode that
expresses exactly this: `STA $XXXX,X` (absolute,X), which adds
the X register to a compile-time-known 16-bit base before
storing — single instruction, 3 bytes.

This pass detects the three-instruction TAC pattern and rewrites
to the new `IndexedStore(address, index, src)` instruction, which
`tac_to_asm` lowers as

    Mov(uchar_var, A)               ; LDA val   (or via Reg(A))
    Mov(uchar_index, X)             ; LDX index
    Mov(A, IndexedAbs(C, X))        ; STA $C,X

— a 3-instruction / 7-byte sequence vs the original ~11
instructions / ~19 bytes for the same effect.

# Eligibility

The fusion fires when:

  * The Store's `dst_ptr` is a single-use Pseudo `%addr` defined
    by an `Add` of a Constant and another Pseudo `%ext`.
  * `%ext` is itself a single-use Pseudo defined by a
    `ZeroExtend(uchar_var, %ext)` (or `SignExtend` from a
    nonnegative-domain 1-byte source — but only ZeroExtend is
    handled here; signed 1-byte indices are rare in the
    addressing-mode role, and SignExtend would put a non-zero
    high byte for negative values which breaks the absolute,X
    invariant).
  * `uchar_var`'s c99 type is a 1-byte type (Char / SChar /
    UChar). The high byte of the index is zero, so `STA C,X`
    accesses `C + (X & 0xFF) = C + X`.
  * `C + 255 ≤ 0xFFFF`. The 6502's absolute,X addressing wraps
    modulo 0x10000, so a base above $FF00 with X near 255 would
    address into page zero, not what the C semantics want. The
    cap `C ≤ 0xFF00` keeps the access entirely within the
    16-bit address space.
  * The `Store.src` is a 1-byte typed Var (Char / SChar / UChar
    or a Pointer-typed Constant whose value fits in a byte). A
    multi-byte Store is currently not handled — it'd need
    multiple `STA $C+k,X` writes with carry-thread reasoning,
    deferred until a motivating case appears.

# Soundness

The single-use checks guarantee no other reader observes the
intermediate values, so removing the temps doesn't change
semantics. The C + 255 ≤ 0xFFFF check guarantees the absolute,X
addressing reaches the same byte the original
indirect-pointer write would have. The high byte of the
zero-extended index is provably zero (ZeroExtend produces
exactly that), so omitting the high-byte add is sound.
"""

from __future__ import annotations

from collections import Counter

import c99_ast
import tac_ast
from passes.optimization.var_visit import uses_in


def recognize_indexed_store(
    fn: tac_ast.Function, *, symbols=None,
) -> tac_ast.Function:
    """Walk `fn`'s instructions; for each foldable triple
    (`ZeroExtend; Binary(Add); Store`), splice in an
    `IndexedStore` and drop the three original instructions.
    Without `symbols`, the pass is a no-op (we need symbol-table
    types to verify the index is 1-byte and the value is 1-byte).

    Two passes: pass 1 scans for triples and records which
    instructions to drop / replace; pass 2 rebuilds the
    instruction list. The split is needed because the Store
    (where the rewrite materializes) comes AFTER the two defs
    it subsumes — a single-pass loop would emit those defs
    before reaching the Store."""
    if symbols is None:
        return fn
    use_counts = _count_uses(fn.instructions)
    # Build a map: instruction index → instruction. We need to find
    # the def of each Pseudo and check the chain.
    def_idx: dict[str, int] = {}
    for i, instr in enumerate(fn.instructions):
        for d in _defs(instr):
            def_idx[d.name] = i
    # Pass 1: identify rewrites.
    rewrites: dict[int, tac_ast.Type_instruction] = {}
    dropped: set[int] = set()
    for i, instr in enumerate(fn.instructions):
        rewritten = _try_recognize(
            instr, fn.instructions, def_idx, use_counts, symbols,
        )
        if rewritten is not None:
            replacement, dropped_indices = rewritten
            # Don't fold if any of the prereq instructions was
            # already consumed by an earlier fold.
            if dropped_indices & dropped:
                continue
            rewrites[i] = replacement
            dropped.update(dropped_indices)
    # Pass 2: rebuild.
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
    """Defs (single-dst writes). Used to build a name → defining-
    instruction-index map. IndexedStore has no def, so this just
    mirrors var_visit's defs_in but without the SSA-restricted
    behavior."""
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
    """If `instr` is the foldable `Store(val, %addr)` head of an
    eligible triple, return the IndexedStore replacement plus the
    set of original-instruction indices to drop. Otherwise None."""
    if not isinstance(instr, tac_ast.Store):
        return None
    if not isinstance(instr.dst_ptr, tac_ast.Var):
        return None
    addr_name = instr.dst_ptr.name
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
    addr_const, addr_other = _split_const_var(
        addr_def.src1, addr_def.src2,
    )
    if addr_const is None or addr_other is None:
        return None
    addr_value = addr_const.const.value
    if not (0 <= addr_value <= 0xFF00):
        return None
    if use_counts.get(addr_other.name, 0) != 1:
        return None
    ext_def_idx = def_idx.get(addr_other.name)
    if ext_def_idx is None:
        return None
    ext_def = all_instrs[ext_def_idx]
    if not isinstance(ext_def, tac_ast.ZeroExtend):
        return None
    if not isinstance(ext_def.src, tac_ast.Var):
        return None
    idx_var = ext_def.src
    # Verify the index source is 1-byte typed.
    if not _is_1_byte_var(idx_var, symbols):
        return None
    # Verify the value being stored is 1-byte typed (Var or
    # Constant). A multi-byte store would need multiple STA abs,X
    # writes — deferred.
    if not _is_1_byte_val(instr.src, symbols):
        return None
    # All checks pass. Build the IndexedStore and mark the three
    # source instructions for deletion. The collapsed instruction
    # inherits the original Store's `is_volatile` bit — folding a
    # volatile Store into an IndexedStore is fine; both lower to
    # the same logical access, just through different addressing.
    indexed = tac_ast.IndexedStore(
        address=addr_value, index=idx_var, src=instr.src,
        is_volatile=instr.is_volatile,
    )
    return (indexed, {addr_def_idx, ext_def_idx})


def _split_const_var(
    a: tac_ast.Type_val, b: tac_ast.Type_val,
) -> tuple[tac_ast.Constant | None, tac_ast.Var | None]:
    """If exactly one of (a, b) is Constant and the other is Var,
    return (constant, var). Otherwise return (None, None)."""
    if isinstance(a, tac_ast.Constant) and isinstance(b, tac_ast.Var):
        return (a, b)
    if isinstance(b, tac_ast.Constant) and isinstance(a, tac_ast.Var):
        return (b, a)
    return (None, None)


def _is_1_byte_var(
    v: tac_ast.Var, symbols,
) -> bool:
    """True iff the Var's symbol-table c99 type is one of the
    1-byte scalar types (Char / SChar / UChar)."""
    sym = symbols.get(v.name) if hasattr(symbols, "get") else None
    if sym is None:
        return False
    t = sym.type
    while isinstance(t, (c99_ast.Const, c99_ast.Volatile)):
        t = t.referenced_type
    return isinstance(t, (c99_ast.Char, c99_ast.SChar, c99_ast.UChar))


def _is_1_byte_val(
    v: tac_ast.Type_val, symbols,
) -> bool:
    """True iff the val is a 1-byte typed value: a Var with a
    1-byte c99 type, or a Constant with a 1-byte variant."""
    if isinstance(v, tac_ast.Constant):
        return isinstance(
            v.const, (tac_ast.ConstChar, tac_ast.ConstUChar),
        )
    if isinstance(v, tac_ast.Var):
        return _is_1_byte_var(v, symbols)
    return False
