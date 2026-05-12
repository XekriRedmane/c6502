"""Asm-level redundant memory-to-memory store elimination.

After loop unrolling, the same loop-invariant memory-to-memory
transfer can appear many times in a single basic block. The headline
case from c6502's lowerings is the DPTR staging sequence:

    LDA  ptr_lo            ; (M0)
    STA  DPTR              ; (M1)
    LDA  ptr_hi            ; (M2)
    STA  DPTR+1            ; (M3)
    ...do an indirect-Y access via (DPTR),Y...
    LDA  ptr_lo            ; redundant — DPTR still holds ptr_lo
    STA  DPTR              ; redundant
    LDA  ptr_hi            ; redundant
    STA  DPTR+1            ; redundant
    ...next access...

When neither `ptr_lo`/`ptr_hi` nor `DPTR`/`DPTR+1` is written
between the two stagings, the second 4-instruction block is dead.
The existing `redundant_load` pass can't catch this because it
tracks A's content (which gets clobbered by the intervening
`LDA (DPTR),Y`); the equivalence we need to remember is
`memory[DPTR] === memory[ptr_lo]`, a memory-to-memory fact that
survives A getting clobbered.

This pass tracks per-basic-block: for each stable-address memory
cell `D`, the last memory cell `S` whose value was stored into
`D`. A `Mov(S, A); Mov(A, D)` pair is dropped when `known[D] = S`
and no intervening instruction has invalidated either side.

# Aliasing model

Tracked cells are stable-address operands — operands whose
byte address doesn't depend on runtime register or pointer
values:
  * `ZP(addr, off)` — fixed zero-page byte at `addr + off`.
  * `Data(name, off)` with `name` ∈ `{SSP, FP, HARGS, DPTR}` —
    runtime-symbol references, which resolve to known ZP byte
    addresses (mapped per `_RUNTIME_ZP_ADDRS`).
  * `Data(name, off)` with `name` a user static — link-time
    absolute address, opaque at this stage. Tracked by
    `(name, off)` identity; assumed to not overlap any other
    statics or any ZP cell.

Cells that are NOT tracked:
  * `Frame(off)` / `Stack(off)` / `Indirect(off)` / `IndirectY()`
    — all use indirect-Y addressing through a runtime ZP pointer
    (`FP` / `SSP` / `DPTR`). The byte they refer to depends on
    that pointer's runtime value, which we don't model.
  * `IndexedData(name, off, X|Y)` — writes the byte at
    `name + off + index_reg`; the actual write address spans a
    256-byte range.

# Invalidation

For each instruction other than a matched LDA/STA pair, the pass
computes the cells potentially written and drops every state
entry whose key or value could alias such a write. A write that
the pass can't classify (e.g. `STA (DPTR),Y`) clears the whole
map.

Range writes through `IndexedData` are handled precisely when the
base address is known: the range is `[base, base+255]`. For
tracked cells with known byte addresses, this is a simple
`base <= addr <= base + 255` containment check. For tracked
`('static', name, off)` cells whose absolute address is unknown,
the pass is conservative (any IndexedData write could alias).

Hardware-stack pushes (`PHA`) write into page 1 ($0100-$01FF)
which doesn't alias the zero page or the static-data segment c6502
uses ($0800+). Treated as no-op.

# Flag soundness

Dropping the LDA/STA pair removes the LDA's effect on N/Z. A
later Branch reading the flags would observe whatever the
previous flag-setter wrote. The pass conservatively keeps the
pair when a Branch follows before another flag-setter — same
guard as `redundant_load`. (In practice the DPTR-stage case is
nowhere near a Branch — staging happens at the top of a hot
loop body, branches come at the bottom.)

# Where it runs

Inside `compile._peephole_fixedpoint`, after `redundant_load`.
The two passes are complementary: `redundant_load` catches
register-vs-memory redundancies; `redundant_store` catches
memory-vs-memory redundancies.
"""

from __future__ import annotations

import asm_ast


# Pre-installed runtime ZP symbol addresses (matches
# `sim.assembler.DEFAULT_ZP_SYMBOLS`). Lets us resolve
# `Data(name=runtime_symbol, offset=k)` operands to specific ZP byte
# addresses for aliasing analysis.
_RUNTIME_ZP_ADDRS = {
    "SSP": 0x00,
    "FP": 0x02,
    "HARGS": 0x04,
    "DPTR": 0x24,
}


def apply_redundant_store_elimination(
    prog: asm_ast.Program,
) -> asm_ast.Program:
    new_top: list[asm_ast.Type_top_level] = []
    for tl in prog.top_level:
        if isinstance(tl, asm_ast.Function):
            new_top.append(_rewrite_function(tl))
        else:
            new_top.append(tl)
    return asm_ast.Program(top_level=new_top)


def _rewrite_function(fn: asm_ast.Function) -> asm_ast.Function:
    """Walk `fn`'s instructions; track memory-to-memory
    equivalences per basic block; drop LDA/STA pairs that
    re-establish a known one."""
    known: dict[tuple, tuple] = {}
    out: list[asm_ast.Type_instruction] = []
    instrs = fn.instructions
    i = 0
    while i < len(instrs):
        instr = instrs[i]
        nxt = instrs[i + 1] if i + 1 < len(instrs) else None
        pair = _match_lda_sta(instr, nxt)
        if pair is not None and _flags_dead_at(instrs, i + 2):
            src_op, dst_op = pair
            src_id = _addr_id(src_op)
            dst_id = _addr_id(dst_op)
            if (
                src_id is not None
                and dst_id is not None
                and known.get(dst_id) == src_id
            ):
                # Redundant — drop both instructions.
                i += 2
                continue
            # Otherwise: the new STA establishes the equivalence.
            # First, invalidate any existing entries whose value
            # was the dst's PRE-store content (we're about to
            # overwrite it).
            _drop_entries_using(known, dst_id)
            known[dst_id] = src_id
            out.append(instr)
            out.append(nxt)
            i += 2
            continue
        _invalidate(known, instr)
        out.append(instr)
        i += 1
    return asm_ast.Function(
        name=fn.name, is_global=fn.is_global,
        params=list(fn.params), instructions=out,
    )


def _match_lda_sta(
    a: asm_ast.Type_instruction,
    b: asm_ast.Type_instruction | None,
) -> tuple[asm_ast.Type_operand, asm_ast.Type_operand] | None:
    """If `a; b` is `Mov(src=mem, A); Mov(A, dst=mem)` with both
    src and dst stable-address memory operands, return
    `(src, dst)`. Otherwise None."""
    if b is None:
        return None
    if not (isinstance(a, asm_ast.Mov) and isinstance(b, asm_ast.Mov)):
        return None
    if not (_is_reg_a(a.dst) and _is_reg_a(b.src)):
        return None
    if not (_is_stable_mem(a.src) and _is_stable_mem(b.dst)):
        return None
    return (a.src, b.dst)


def _is_reg_a(op: asm_ast.Type_operand) -> bool:
    return isinstance(op, asm_ast.Reg) and isinstance(op.reg, asm_ast.A)


def _is_stable_mem(op: asm_ast.Type_operand) -> bool:
    """Memory operands whose byte address is determinate at this
    stage (no runtime-pointer indirection, no index-register range).
    Tracked: ZP, Data. Not tracked: Frame, Stack, Indirect,
    IndirectY, IndexedData, Reg, Imm."""
    return isinstance(op, (asm_ast.ZP, asm_ast.Data))


def _addr_id(op: asm_ast.Type_operand) -> tuple | None:
    """Canonical identity of a stable-address memory operand.
    Returns ('byte', abs_addr) when the operand resolves to a
    specific byte address (ZP, or Data with a runtime-symbol
    name); returns ('static', name, offset) for Data with a
    user-symbol name whose address is opaque here."""
    if isinstance(op, asm_ast.ZP):
        return ('byte', op.address + op.offset)
    if isinstance(op, asm_ast.Data):
        base = _RUNTIME_ZP_ADDRS.get(op.name)
        if base is not None:
            return ('byte', base + op.offset)
        return ('static', op.name, op.offset)
    return None


def _drop_entries_using(known: dict, id_: tuple) -> None:
    """Drop entries whose VALUE is `id_` — their tracked source
    is about to be overwritten by a new STA."""
    if id_ is None:
        return
    for k in [k for k, v in known.items() if v == id_]:
        del known[k]


def _invalidate(known: dict, instr: asm_ast.Type_instruction) -> None:
    """Update `known` for an instruction that's NOT a matched
    LDA/STA pair. Clear on block boundaries; for each write,
    drop entries whose key or value could alias the written
    cell."""
    if isinstance(instr, (
        asm_ast.Label, asm_ast.Jump, asm_ast.Branch,
        asm_ast.Ret, asm_ast.Return, asm_ast.Call,
        asm_ast.FunctionPrologue, asm_ast.AllocateStack,
        asm_ast.LoadAddress, asm_ast.Phi,
    )):
        known.clear()
        return
    for write_id in _memory_writes(instr):
        if write_id is None:
            known.clear()
            return
        to_drop = [
            k for k, v in known.items()
            if _aliases(k, write_id) or _aliases(v, write_id)
        ]
        for k in to_drop:
            del known[k]


def _memory_writes(instr: asm_ast.Type_instruction):
    """Yield each write-id (an ('byte', addr) / ('static', name,
    off) / ('range', base, size) tuple) for a memory cell
    potentially written by `instr`. Yield `None` to signal an
    unknown-effect write that should clear the entire map.
    Instructions with no memory side effect yield nothing."""
    if isinstance(instr, asm_ast.Mov):
        dst = instr.dst
        if isinstance(dst, asm_ast.Reg):
            return
        if isinstance(dst, (asm_ast.ZP, asm_ast.Data)):
            yield _addr_id(dst)
            return
        if isinstance(dst, asm_ast.IndexedData):
            base = _indexed_base(dst)
            yield ('range', base, 256)
            return
        # Frame / Stack / Indirect / IndirectY: indirect-Y through a
        # ZP pointer; runtime address.
        yield None
        return
    if isinstance(instr, (
        asm_ast.Add, asm_ast.Sub, asm_ast.And, asm_ast.Or,
        asm_ast.Xor, asm_ast.ClearCarry, asm_ast.SetCarry,
        asm_ast.Compare,
    )):
        # Result in A or flags; no memory write.
        return
    if isinstance(instr, (asm_ast.Inc, asm_ast.Dec)):
        dst = instr.dst
        if isinstance(dst, asm_ast.Reg):
            return  # INX / INY / DEX / DEY
        if isinstance(dst, (asm_ast.ZP, asm_ast.Data)):
            yield _addr_id(dst)
            return
        yield None
        return
    if isinstance(instr, (
        asm_ast.ArithmeticShiftLeft, asm_ast.LogicalShiftRight,
        asm_ast.RotateLeft, asm_ast.RotateRight,
    )):
        dst = instr.dst
        if isinstance(dst, asm_ast.Reg):
            return  # ASL A / LSR A / ROL A / ROR A
        if isinstance(dst, (asm_ast.ZP, asm_ast.Data)):
            yield _addr_id(dst)
            return
        yield None
        return
    if isinstance(instr, asm_ast.Push):
        # PHA: writes hardware stack ($0100-$01FF). Doesn't alias
        # ZP (page 0) or the static-data segment (≥ $0800 by
        # c6502 convention).
        return
    if isinstance(instr, asm_ast.Pop):
        # PLA: reads hardware stack into A; no memory write.
        return
    # Unknown instruction kind — conservative.
    yield None


def _indexed_base(op: asm_ast.IndexedData) -> int | None:
    """The absolute numeric base of an IndexedData operand, if
    knowable. Empty-name encodes a raw 16-bit base in `offset`;
    runtime-symbol names resolve via `_RUNTIME_ZP_ADDRS`; user-
    static names have link-time addresses opaque here (return
    None to signal 'unknown range')."""
    if op.name == "":
        return op.offset
    base = _RUNTIME_ZP_ADDRS.get(op.name)
    if base is not None:
        return base + op.offset
    return None


def _aliases(addr_id: tuple, write_id: tuple) -> bool:
    """True iff a write described by `write_id` could write to
    the cell described by `addr_id`. Conservative: returns True
    for any case we can't prove disjoint."""
    if addr_id is None or write_id is None:
        return True
    if write_id[0] == 'range':
        _, base, size = write_id
        if base is None:
            return True  # unknown range, conservative
        if addr_id[0] == 'byte':
            return base <= addr_id[1] < base + size
        # ('static', name, off) with unknown link-time address —
        # could be anywhere; assume might alias.
        return True
    if write_id[0] == 'byte' and addr_id[0] == 'byte':
        return write_id[1] == addr_id[1]
    if write_id[0] == 'static' and addr_id[0] == 'static':
        return (
            write_id[1] == addr_id[1] and write_id[2] == addr_id[2]
        )
    # Cross-namespace: one is byte-addressed (known address), the
    # other is static (link-time address). User statics live in
    # the data segment (≥ $0800 by c6502 convention), disjoint
    # from zero page and from runtime symbols. Don't alias.
    return False


def _flags_dead_at(
    instrs: list[asm_ast.Type_instruction], idx: int,
) -> bool:
    """True iff dropping a load at index `idx - 2` is sound from
    a flag-liveness standpoint. Scans forward from `idx`; returns
    False if a Branch reads the flags before another instruction
    overwrites them. Mirrors `redundant_load._flags_dead_at`."""
    while idx < len(instrs):
        instr = instrs[idx]
        if isinstance(instr, asm_ast.Branch):
            return False
        if isinstance(instr, (
            asm_ast.Label, asm_ast.Jump,
            asm_ast.Ret, asm_ast.Return, asm_ast.Call,
        )):
            return True
        if _resets_nz(instr):
            return True
        idx += 1
    return True


def _resets_nz(instr: asm_ast.Type_instruction) -> bool:
    """True iff `instr` overwrites the N/Z flags."""
    if isinstance(instr, asm_ast.Mov):
        # Loads (LDA / LDX / LDY / transfers) set N/Z; stores
        # don't.
        return isinstance(instr.dst, asm_ast.Reg)
    return isinstance(instr, (
        asm_ast.Add, asm_ast.Sub, asm_ast.And, asm_ast.Or,
        asm_ast.Xor, asm_ast.Compare,
        asm_ast.Inc, asm_ast.Dec,
        asm_ast.ArithmeticShiftLeft, asm_ast.LogicalShiftRight,
        asm_ast.RotateLeft, asm_ast.RotateRight,
        asm_ast.Pop,
    ))
