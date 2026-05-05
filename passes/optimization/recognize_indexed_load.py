"""TAC pass that recognizes the absolute,X-load pattern.

Mirror of `recognize_indexed_store.py`, but for the analogous Load
shape. After const-static fold + reassoc, an expression like

    pixels = hud_strip_src[y];   // hud_strip_src is `T * const`

(with `hud_strip_src` a const-pointer static folding to a numeric
address) lowers to

    ZeroExtend(y,           %ext)        # y uchar → 2-byte
    Binary(Add, C, %ext,    %addr)        # 16-bit ptr arithmetic
    Load(%addr,             pixels)       # read 1 byte

where C is the folded numeric base. The 6502 expresses this with
`LDA $XXXX,X` (absolute,X) — single instruction, 3 bytes.

This pass detects the three-instruction TAC pattern and rewrites
to the new `IndexedConstLoad(address, index, dst)` instruction,
which `tac_to_asm` lowers as

    Mov(uchar_idx, A)               ; LDA index   (or via Reg(A))
    Mov(A, X)                        ; TAX
    Mov(IndexedAbs(C, X), A)        ; LDA $C,X
    Mov(A, dst)                      ; STA dst

— a 4-instruction / 8-byte sequence vs the original ~10
instructions for the same effect (DPTR setup + indirect-Y load).

The eligibility checks are the same as `recognize_indexed_store`
modulo direction:
  * The Load's `src_ptr` is a single-use Pseudo `%addr` defined
    by an `Add` of a Constant and another Pseudo `%ext`.
  * `%ext` is itself a single-use Pseudo defined by
    `ZeroExtend(uchar_var, %ext)`.
  * `uchar_var`'s c99 type is 1 byte (Char / SChar / UChar).
  * `0 ≤ C ≤ 0xFF00` (so `C + 255` doesn't wrap past $FFFF).
  * The Load's `dst` is a 1-byte typed Var. Multi-byte loads
    would need multiple `LDA $C+k,X` reads — deferred until a
    motivating case appears.
"""
from __future__ import annotations

from collections import Counter

import c99_ast
import tac_ast
from passes.optimization.var_visit import uses_in


def recognize_indexed_load(
    fn: tac_ast.Function, *, symbols=None,
) -> tac_ast.Function:
    """Walk `fn`'s instructions; for each foldable triple
    (`ZeroExtend; Binary(Add); Load`), splice in an
    `IndexedConstLoad` and drop the three original instructions.
    Without `symbols`, the pass is a no-op (we need symbol-table
    types to verify the index and dst are 1-byte)."""
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
    if not isinstance(instr, tac_ast.Load):
        return None
    if not isinstance(instr.src_ptr, tac_ast.Var):
        return None
    if not isinstance(instr.dst, tac_ast.Var):
        return None
    if not _is_1_byte_var(instr.dst, symbols):
        return None
    addr_name = instr.src_ptr.name
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
    if not _is_1_byte_var(idx_var, symbols):
        return None
    indexed = tac_ast.IndexedConstLoad(
        address=addr_value, index=idx_var, dst=instr.dst,
    )
    return (indexed, {addr_def_idx, ext_def_idx})


def _split_const_var(
    a: tac_ast.Type_val, b: tac_ast.Type_val,
) -> tuple[tac_ast.Constant | None, tac_ast.Var | None]:
    if isinstance(a, tac_ast.Constant) and isinstance(b, tac_ast.Var):
        return (a, b)
    if isinstance(b, tac_ast.Constant) and isinstance(a, tac_ast.Var):
        return (b, a)
    return (None, None)


def _is_1_byte_var(v: tac_ast.Var, symbols) -> bool:
    sym = symbols.get(v.name) if hasattr(symbols, "get") else None
    if sym is None:
        return False
    t = sym.type
    while isinstance(t, c99_ast.Const):
        t = t.referenced_type
    return isinstance(t, (c99_ast.Char, c99_ast.SChar, c99_ast.UChar))
