"""Indirect-base copy propagation: bypass DPTR when an existing
ZP pointer can serve as the indirect base.

`tac_to_asm`'s indirect-load / indirect-store / indirect-indexed
lowerings always stage the pointer's two bytes into the runtime's
`DPTR` zero-page pair before issuing `(DPTR),Y` accesses. When
the pointer's source bytes already live at adjacent ZP addresses
(e.g., a `zp_abi` pointer parameter, or any non-address-taken
pointer that regalloc places in ZP), the staging is unnecessary —
the source pair itself is already a valid indirect base.

This pass detects the 4-instruction DPTR-stage shape

    LDA <ZP zp_lo> ; STA DPTR
    LDA <ZP zp_hi> ; STA DPTR+1

where `zp_lo` and `zp_hi` resolve to adjacent byte addresses
`(N, N+1)`, and records the equivalence `DPTR === zp_pair(N)`
within the current basic block. Subsequent `Indirect` /
`IndirectY` operands are rewritten to `IndirectZp(N, off)` /
`IndirectZpY(N)` — the 6502's `(zp),Y` mode reads the pointer
from any ZP pair, not just DPTR.

After the rewrite, nothing reads DPTR within the block. The
existing `asm_dead_store` pass (with `Data("DPTR", _)` now
classified as dead-at-exit) drops the redundant `STA DPTR` /
`STA DPTR+1` writes. The orphaned `LDA <zp_lo>` / `LDA <zp_hi>`
loads then become A-only dead computations, which
`dead_a_arith` drops on a subsequent fixed-point iteration.

# Invalidation

The equivalence `DPTR === zp_pair(N)` holds only while neither
the source bytes (`N`, `N+1`) nor the destination (`DPTR`,
`DPTR+1`) has been modified since the stage. The pass walks
forward per basic block and clears the equivalence on:

  * Any byte write whose destination is in `{DPTR, DPTR+1, N,
    N+1}` — including IndexedData writes whose range covers any
    of these addresses.
  * Indirect / Frame / Stack writes — runtime-pointer writes,
    addresses unknown; conservatively assume they could touch
    the protected cells.
  * Calls — opaque effect; clear all.
  * Block boundaries (Label / Jump / Branch / Ret / Return) —
    fresh state per block.

Reads through `(DPTR),Y` etc. don't modify DPTR or the source
ZP pair — they pass through cleanly.

# Where to run

Inside `_peephole_fixedpoint`, after `replace_pseudoregisters`
(operand addresses are concrete). The first iteration rewrites
the indirect operands; the second drops the dead staging.
"""

from __future__ import annotations

import asm_ast


# DPTR's runtime ZP address; mirrors `sim.assembler.DEFAULT_ZP_
# SYMBOLS["DPTR"]`.
_DPTR_ADDR = 0x24

# Pre-installed runtime ZP symbol addresses. Mirrors
# `passes.asm_aliasing._RUNTIME_ZP_ADDRS` — used here to resolve
# `Data(name, off)` operands whose name is a runtime symbol back
# to their byte addresses.
_RUNTIME_ZP_ADDRS = {
    "SSP": 0x00,
    "FP": 0x02,
    "HARGS": 0x04,
    "DPTR": _DPTR_ADDR,
}


def apply_indirect_base_prop(
    prog: asm_ast.Program,
    *,
    zp_symbol_addrs: dict[str, int] | None = None,
) -> asm_ast.Program:
    """Rewrite DPTR-staged indirect loads/stores to use the source
    ZP pair directly. `zp_symbol_addrs` extends the runtime-symbol
    table with caller-supplied `Data(name)` → byte-address bindings
    (typically the `__zpabi_*` slot symbols produced by
    `passes.zp_slot_allocation`) so a `Data(__zpabi_fn_p0)` whose
    resolved address is in zero page is recognized as a valid ZP
    pointer source for `(zp),Y` rewriting."""
    addrs = dict(_RUNTIME_ZP_ADDRS)
    if zp_symbol_addrs:
        addrs.update(zp_symbol_addrs)
    new_top: list[asm_ast.Type_top_level] = []
    for tl in prog.top_level:
        if isinstance(tl, asm_ast.Function):
            new_top.append(_rewrite_function(tl, addrs))
        else:
            new_top.append(tl)
    return asm_ast.Program(top_level=new_top)


def _rewrite_function(
    fn: asm_ast.Function, zp_addrs: dict[str, int],
) -> asm_ast.Function:
    """Walk `fn`'s instructions; track the `DPTR === zp_pair(N)`
    equivalence per basic block; rewrite Indirect / IndirectY
    operands while it holds."""
    instrs = fn.instructions
    base_zp: int | None = None
    out: list[asm_ast.Type_instruction] = []
    i = 0
    while i < len(instrs):
        stage = _match_dptr_stage(instrs, i, zp_addrs)
        if stage is not None:
            base_zp = stage
            out.extend(instrs[i:i + 4])
            i += 4
            continue
        instr = instrs[i]
        if _is_block_boundary(instr):
            base_zp = None
            out.append(instr)
            i += 1
            continue
        # Rewrite this instruction's operands first — they execute
        # with the equivalence still valid (DPTR holds zp_pair).
        # Then check if THIS instruction's writes invalidate the
        # equivalence for SUBSEQUENT instructions.
        if base_zp is not None:
            rewritten = _rewrite_operands(instr, base_zp)
            if _writes_invalidate(instr, base_zp):
                base_zp = None
            out.append(rewritten)
        else:
            out.append(instr)
        i += 1
    return asm_ast.Function(
        name=fn.name, is_global=fn.is_global,
        params=list(fn.params), instructions=out,
    )


def _match_dptr_stage(
    instrs: list[asm_ast.Type_instruction], i: int,
    zp_addrs: dict[str, int],
) -> int | None:
    """Match the 4-instruction DPTR-stage shape at `instrs[i:i+4]`.
    Returns the ZP byte address `N` of the source pair's low byte
    iff the source resolves to adjacent bytes `(N, N+1)` AND the
    destinations are `Data(DPTR, 0)` then `Data(DPTR, 1)`."""
    if i + 3 >= len(instrs):
        return None
    a, b, c, d = instrs[i:i + 4]
    if not all(isinstance(x, asm_ast.Mov) for x in (a, b, c, d)):
        return None
    if not (
        _is_reg_a(a.dst) and _is_reg_a(b.src)
        and _is_reg_a(c.dst) and _is_reg_a(d.src)
    ):
        return None
    if not _is_dptr_byte(b.dst, 0) or not _is_dptr_byte(d.dst, 1):
        return None
    addr_a = _resolved_zp_addr(a.src, zp_addrs)
    addr_c = _resolved_zp_addr(c.src, zp_addrs)
    if addr_a is None or addr_c is None:
        return None
    if addr_c != addr_a + 1:
        return None
    return addr_a


def _is_reg_a(op: asm_ast.Type_operand) -> bool:
    return isinstance(op, asm_ast.Reg) and isinstance(op.reg, asm_ast.A)


def _is_dptr_byte(op: asm_ast.Type_operand, offset: int) -> bool:
    return (
        isinstance(op, asm_ast.Data)
        and op.name == "DPTR"
        and op.offset == offset
    )


def _resolved_zp_addr(
    op: asm_ast.Type_operand, zp_addrs: dict[str, int],
) -> int | None:
    """ZP byte address that `op` references, if op resolves to a
    specific ZP byte. `ZP(addr, off)` resolves to `addr+off`;
    `Data(name, off)` resolves via `zp_addrs` (which extends the
    runtime-symbol table with any caller-supplied bindings like
    the `__zpabi_*` slot symbols). Returns None for non-ZP
    operands or symbols whose resolved address is above `$FF`."""
    if isinstance(op, asm_ast.ZP):
        return op.address + op.offset
    if isinstance(op, asm_ast.Data):
        base = zp_addrs.get(op.name)
        if base is not None and base + op.offset <= 0xFF:
            return base + op.offset
    return None


def _is_block_boundary(instr: asm_ast.Type_instruction) -> bool:
    return isinstance(instr, (
        asm_ast.Label, asm_ast.Jump, asm_ast.Branch,
        asm_ast.Ret, asm_ast.Return, asm_ast.Call,
        asm_ast.FunctionPrologue, asm_ast.AllocateStack,
        asm_ast.LoadAddress, asm_ast.Phi,
    ))


def _writes_invalidate(
    instr: asm_ast.Type_instruction, base_zp: int,
) -> bool:
    """True iff `instr`'s memory writes could touch any byte in
    `{DPTR, DPTR+1, base_zp, base_zp+1}` — the set of cells the
    `DPTR === zp_pair(base_zp)` equivalence depends on."""
    protected = {
        _DPTR_ADDR, _DPTR_ADDR + 1, base_zp, base_zp + 1,
    }
    for write_id in _memory_writes(instr):
        if write_id is None:
            return True
        if write_id[0] == 'byte':
            if write_id[1] in protected:
                return True
        elif write_id[0] == 'range':
            base, size = write_id[1], write_id[2]
            if base is None:
                return True
            if any(base <= p < base + size for p in protected):
                return True
        # ('static', name, off) — user-static link-time address.
        # User statics live in the data segment, disjoint from
        # ZP; no alias.
    return False


def _memory_writes(instr: asm_ast.Type_instruction):
    """Yield write-ids for each memory cell potentially written.
    Mirrors `redundant_store._memory_writes`'s shape — see that
    module's docstring for the encoding."""
    if isinstance(instr, asm_ast.Mov):
        dst = instr.dst
        if isinstance(dst, asm_ast.Reg):
            return
        if isinstance(dst, asm_ast.ZP):
            yield ('byte', dst.address + dst.offset)
            return
        if isinstance(dst, asm_ast.Data):
            base = _RUNTIME_ZP_ADDRS.get(dst.name)
            if base is not None:
                yield ('byte', base + dst.offset)
            else:
                yield ('static', dst.name, dst.offset)
            return
        if isinstance(dst, asm_ast.IndexedData):
            base = _indexed_base(dst)
            yield ('range', base, 256)
            return
        # Frame / Stack / Indirect / IndirectY / IndirectZp /
        # IndirectZpY — runtime-pointer writes; address unknown.
        yield None
        return
    if isinstance(instr, (
        asm_ast.Add, asm_ast.Sub, asm_ast.And, asm_ast.Or,
        asm_ast.Xor, asm_ast.ClearCarry, asm_ast.SetCarry,
        asm_ast.Compare,
    )):
        return
    if isinstance(instr, (asm_ast.Inc, asm_ast.Dec)):
        dst = instr.dst
        if isinstance(dst, asm_ast.Reg):
            return
        if isinstance(dst, asm_ast.ZP):
            yield ('byte', dst.address + dst.offset)
            return
        if isinstance(dst, asm_ast.Data):
            base = _RUNTIME_ZP_ADDRS.get(dst.name)
            if base is not None:
                yield ('byte', base + dst.offset)
            else:
                yield ('static', dst.name, dst.offset)
            return
        yield None
        return
    if isinstance(instr, (
        asm_ast.ArithmeticShiftLeft, asm_ast.LogicalShiftRight,
        asm_ast.RotateLeft, asm_ast.RotateRight,
    )):
        dst = instr.dst
        if isinstance(dst, asm_ast.Reg):
            return
        if isinstance(dst, asm_ast.ZP):
            yield ('byte', dst.address + dst.offset)
            return
        if isinstance(dst, asm_ast.Data):
            base = _RUNTIME_ZP_ADDRS.get(dst.name)
            if base is not None:
                yield ('byte', base + dst.offset)
            else:
                yield ('static', dst.name, dst.offset)
            return
        yield None
        return
    if isinstance(instr, asm_ast.Push):
        # PHA writes hardware stack ($0100-$01FF). Doesn't alias
        # ZP or static data.
        return
    if isinstance(instr, asm_ast.Pop):
        return
    yield None


def _indexed_base(op: asm_ast.IndexedData) -> int | None:
    if op.name == "":
        return op.offset
    base = _RUNTIME_ZP_ADDRS.get(op.name)
    if base is not None:
        return base + op.offset
    return None


def _rewrite_operands(
    instr: asm_ast.Type_instruction, base_zp: int,
) -> asm_ast.Type_instruction:
    """Rewrite Indirect / IndirectY operands within `instr` to use
    `base_zp` as the indirect base. Returns a new instruction
    instance only if at least one operand changed; otherwise the
    original."""
    if isinstance(instr, asm_ast.Mov):
        new_src = _rewrite_op(instr.src, base_zp)
        new_dst = _rewrite_op(instr.dst, base_zp)
        if new_src is instr.src and new_dst is instr.dst:
            return instr
        return asm_ast.Mov(src=new_src, dst=new_dst)
    if isinstance(instr, (
        asm_ast.Add, asm_ast.Sub, asm_ast.And, asm_ast.Or,
    )):
        new_src = _rewrite_op(instr.src, base_zp)
        if new_src is instr.src:
            return instr
        return type(instr)(src=new_src, dst=instr.dst)
    if isinstance(instr, asm_ast.Xor):
        new_s1 = _rewrite_op(instr.src1, base_zp)
        new_s2 = _rewrite_op(instr.src2, base_zp)
        if new_s1 is instr.src1 and new_s2 is instr.src2:
            return instr
        return asm_ast.Xor(src1=new_s1, src2=new_s2, dst=instr.dst)
    if isinstance(instr, asm_ast.Compare):
        new_right = _rewrite_op(instr.right, base_zp)
        if new_right is instr.right:
            return instr
        return asm_ast.Compare(left=instr.left, right=new_right)
    return instr


def _rewrite_op(
    op: asm_ast.Type_operand, base_zp: int,
) -> asm_ast.Type_operand:
    if isinstance(op, asm_ast.Indirect):
        return asm_ast.IndirectZp(address=base_zp, offset=op.offset)
    if isinstance(op, asm_ast.IndirectY):
        return asm_ast.IndirectZpY(address=base_zp)
    return op
