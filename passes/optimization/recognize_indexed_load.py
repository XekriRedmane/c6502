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
from passes.optimization.var_visit import count_uses, defs_in
from passes.optimization.framework import (
    DefUsePass, DefUseEnv, PassContext, MatchResult,
    m_Load, m_Var, m_Any,
)


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
    use_counts = count_uses(fn.instructions)
    def_idx: dict[str, int] = {}
    for i, instr in enumerate(fn.instructions):
        for d in defs_in(instr):
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
    # Accept both ZeroExtend (unsigned-uchar promotion) and
    # SignExtend (signed-int8_t promotion). The 6502's absolute,X
    # addressing reads only the index's low byte, so the high-byte
    # portion of either extension is irrelevant to the byte-address
    # computation. SignExtend is sound iff the index is non-negative
    # at runtime — guaranteed by C99 §6.5.6, which leaves arr[i]
    # undefined when i would address outside arr's bounds, including
    # negative i for arrays declared at their natural base.
    #
    # The dead high-byte computation in the SignExtend's lowering
    # (LDA src.high → A; BMI sx_neg; LDA #$00; JMP sx_done; sx_neg:;
    # LDA #$FF; sx_done:; STA dst.byte_1) becomes residue that
    # byte_dce + dead_a_arith eliminate once the dst is no longer
    # referenced — the recognizer rewrites away the chain's only
    # consumer (the Add + Load), so SSA-DCE drops the SignExtend
    # entirely on the next iteration.
    if not isinstance(ext_def, (tac_ast.ZeroExtend, tac_ast.SignExtend)):
        return None
    if not isinstance(ext_def.src, tac_ast.Var):
        return None
    idx_var = ext_def.src
    if not _is_1_byte_var(idx_var, symbols):
        return None
    indexed = tac_ast.IndexedConstLoad(
        address=addr_value, index=idx_var, dst=instr.dst,
        is_volatile=instr.is_volatile,
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
    while isinstance(t, (c99_ast.Const, c99_ast.Volatile)):
        t = t.referenced_type
    return isinstance(t, (c99_ast.Char, c99_ast.SChar, c99_ast.UChar))


class RecognizeIndexedLoad(DefUsePass):
    """DefUsePass: matches Load(src_ptr=Var, dst=Var). For each match,
    walks back twice through the SSA def-chain to find the
    ZeroExtend/SignExtend → Binary(Add, Const, %ext) → Load
    pattern. Uses the full def-idx so the Load and its producers
    need not be index-adjacent."""
    name = "recognize_indexed_load"
    pattern = m_Load(
        src_ptr=m_Var(capture='addr_var'),
        dst=m_Var(capture='load_dst'),
        capture='load_instr',
    )

    def prepare_extra(self, fn, ctx):
        return count_uses(fn.instructions)

    def rewrite(self, m: MatchResult, env: DefUseEnv, ctx: PassContext) -> object | None:
        if ctx.symbols is None:
            return None
        load_instr = m.bindings['load_instr']
        addr_var = m.bindings['addr_var']
        load_dst = m.bindings['load_dst']
        # dst must be 1-byte typed.
        if not _is_1_byte_var(load_dst, ctx.symbols):
            return None
        # addr_var must be SSA-renamed (single-def guarantee).
        if ctx.ssa_dsts is None or addr_var.name not in ctx.ssa_dsts:
            return None
        # Single-use gate on %addr.
        if env.extra.get(addr_var.name, 0) != 1:
            return None
        # Walk back to the Binary(Add, Const, %ext) def.
        binary = env.def_of(addr_var)
        if not isinstance(binary, tac_ast.Binary) or not isinstance(binary.op, tac_ast.Add):
            return None
        addr_const, ext_var = _split_const_var(binary.src1, binary.src2)
        if addr_const is None or ext_var is None:
            return None
        addr_value = addr_const.const.value
        if not (0 <= addr_value <= 0xFF00):
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
        # All checks pass. The two intermediate defs (%ext, %addr)
        # become dead after this rewrite; SSA-DCE drops them on the
        # next fixedpoint iteration.
        return tac_ast.IndexedConstLoad(
            address=addr_value, index=idx_var, dst=load_dst,
            is_volatile=load_instr.is_volatile,
        )
