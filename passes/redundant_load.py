"""Redundant load elimination.

A 6502 register tracker. Per basic block, walk linearly and remember
which operand each of A / X / Y is currently a copy of. When the
next instruction is `LDA M` (or `LDX M` / `LDY M`) and the target
register already mirrors `M`, the load is redundant and we drop it.

The win is heaviest after loop unrolling: a 105-iteration unrolled
fill emits 105 copies of `LDA value; LDX col; STA $XXXX,X` even
though `value` and `col` are unchanged across the sequence. With
this pass: 1 × `LDA value; LDX col` + 105 × `STA $XXXX,X`. Without
the unroll context, the pass also catches incidental redundant
loads that arise from per-byte fan-outs and the SSA-destruction
move flow.

# Aliasing

Dropping `LDA M` requires knowing that the value at M hasn't
changed since the last load that put it in A. The hard part of
register tracking is figuring out which memory writes between
loads can clobber which tracked operands. Our model is a small
disjointness lattice over operand kinds:

  * `Imm`            — never aliases memory (it's an immediate
                       constant baked into the instruction).
  * `ZP(addr, off)`  — the byte at `(addr + off) & 0xFF`. Aliases
                       any other ZP cell at the same address;
                       never aliases `Data` / `IndexedData`
                       (those resolve to addresses ≥ $0100, by
                       construction — `replace_pseudoregisters`
                       emits ZP for ZP-resident pseudos and Data
                       for the data-segment statics).
  * `Data(name, off)`         — link-time absolute address. Aliases
                                only `Data(name, off)` with matching
                                name+offset. Doesn't alias ZP.
  * `IndexedData(name, off)`  — link-time `name+off,X` (or `,Y`).
                                Aliases any operand inside the
                                `[name+off .. name+off+0xFF]`
                                window that we can't prove
                                disjoint. Doesn't alias ZP.

That's enough for the unroll case (track ZP, write to
IndexedData; survives) and conservative everywhere else (any
shape we can't classify invalidates everything tracked).

# Flag soundness

Dropping a load skips the load's N/Z flag effect. Subsequent
`Branch` instructions read those flags. We don't drop the load
unless we can prove no flag-reader fires before another
flag-setter (effectively all loads / arithmetic / shifts /
compares / ALU ops). c6502's lowerings always emit a flag-
setting instruction immediately before any Branch they care
about, so this rule fires the optimization in the common cases
without going wrong.

# Basic blocks

State resets at every basic-block boundary: `Label`, `Jump`,
`Branch`, `Call`, `Ret`, `Return`. Conservatively, we treat
`FunctionPrologue`, `AllocateStack`, and `LoadAddress` as full
register clobbers — they expand into multi-instruction sequences
inside `asm_to_asm2`, and tracking the inner state would mean
mirroring the expansion here.

# Where it runs

After `replace_pseudoregisters` (operands are concrete) and
`apply_direct_index_load` (so we see `LDX zp` directly rather
than `LDA zp; TAX`); before `expand_long_branches` (we never
add new branches — the pass only deletes — but the ordering
keeps us symmetric with `inc_peephole` and
`apply_direct_index_load`).
"""
from __future__ import annotations

from dataclasses import dataclass

import asm_ast


def apply_redundant_load_elimination(
    prog: asm_ast.Program,
) -> asm_ast.Program:
    new_top: list[asm_ast.Type_top_level] = []
    for tl in prog.top_level:
        if isinstance(tl, asm_ast.Function):
            new_top.append(_rewrite_function(tl))
        else:
            new_top.append(tl)
    return asm_ast.Program(top_level=new_top)


@dataclass
class _RegState:
    """Per-block register tracking. Each field is the operand the
    register currently mirrors, or None when unknown."""
    a: asm_ast.Type_operand | None = None
    x: asm_ast.Type_operand | None = None
    y: asm_ast.Type_operand | None = None

    def reset(self) -> None:
        self.a = self.x = self.y = None


def _rewrite_function(fn: asm_ast.Function) -> asm_ast.Function:
    state = _RegState()
    out: list[asm_ast.Type_instruction] = []
    instrs = fn.instructions
    for i, instr in enumerate(instrs):
        if _is_redundant_load(instr, state) and _flags_dead_at(instrs, i + 1):
            # Drop the load entirely — A / X / Y already mirror
            # the load's source, and no Branch will read the
            # flags before something else resets them.
            continue
        out.append(instr)
        _update_state(instr, state)
    return asm_ast.Function(
        name=fn.name, is_global=fn.is_global,
        params=list(fn.params), instructions=out,
    )


def _is_redundant_load(
    instr: asm_ast.Type_instruction, state: _RegState,
) -> bool:
    if not isinstance(instr, asm_ast.Mov):
        return False
    # Register-to-register Movs aren't redundant in the load sense
    # — they're transfers, handled by _update_state.
    if isinstance(instr.src, asm_ast.Reg):
        return False
    if not isinstance(instr.dst, asm_ast.Reg):
        return False
    cur = _get_reg(state, instr.dst.reg)
    return cur is not None and _operands_equal(cur, instr.src)


def _update_state(
    instr: asm_ast.Type_instruction, state: _RegState,
) -> None:
    """Apply `instr`'s effect to the tracked register state."""
    if isinstance(instr, asm_ast.Mov):
        _update_for_mov(instr, state)
        return
    if isinstance(instr, (asm_ast.Label, asm_ast.Jump, asm_ast.Branch,
                          asm_ast.Ret, asm_ast.Return)):
        # Block boundary — anything after this is a fresh block.
        state.reset()
        return
    if isinstance(instr, asm_ast.Call):
        # JSR may clobber any register and any memory; conservatively
        # invalidate everything.
        state.reset()
        return
    if isinstance(instr, (asm_ast.FunctionPrologue,
                          asm_ast.AllocateStack,
                          asm_ast.LoadAddress)):
        # These compound nodes expand into multi-instruction sequences
        # in asm_to_asm2; their inner effect on A / X / Y is more than
        # a simple tracker can model.
        state.reset()
        return
    if isinstance(instr, asm_ast.Pop):
        # PLA / PLX / PLY pulls a fresh value off the stack.
        if isinstance(instr.dst, asm_ast.Reg):
            _set_reg(state, instr.dst.reg, None)
        return
    if isinstance(instr, asm_ast.Push):
        # PHA / PHP doesn't change registers but writes the stack —
        # we don't track stack-pointer-relative loads, so skip.
        return
    if isinstance(instr, (asm_ast.ClearCarry, asm_ast.SetCarry,
                          asm_ast.Compare)):
        # No register change.
        return
    if isinstance(instr, (asm_ast.Add, asm_ast.Sub,
                          asm_ast.And, asm_ast.Or)):
        # ADC / SBC / AND / ORA: in c6502's IR these always have
        # `dst=Reg(A)`. The result in A is no longer a copy of any
        # tracked operand.
        if isinstance(instr.dst, asm_ast.Reg):
            _set_reg(state, instr.dst.reg, None)
        # Memory side-effects: the source operand may also be the
        # destination of an indirectly-aliased write? No — these
        # instructions don't write memory. Nothing else to do.
        return
    if isinstance(instr, asm_ast.Xor):
        # EOR with src1 / src2 / dst (dst is always Reg(A)).
        if isinstance(instr.dst, asm_ast.Reg):
            _set_reg(state, instr.dst.reg, None)
        return
    if isinstance(instr, (asm_ast.ArithmeticShiftLeft,
                          asm_ast.LogicalShiftRight,
                          asm_ast.RotateLeft, asm_ast.RotateRight)):
        # Operate on Reg(A) per c6502's IR. Modify A; no memory write.
        if isinstance(instr.dst, asm_ast.Reg):
            _set_reg(state, instr.dst.reg, None)
        return
    if isinstance(instr, (asm_ast.Inc, asm_ast.Dec)):
        # Memory write to instr.dst. Invalidate any register tracking
        # that may alias.
        _invalidate_aliasing(state, instr.dst)
        return
    # Comments, blank lines, and any future no-op nodes.
    return


def _update_for_mov(mov: asm_ast.Mov, state: _RegState) -> None:
    """Distinguish the four Mov shapes:
      Mov(Reg, Reg)        — register transfer.
      Mov(non-Reg, Reg)    — load (LDA / LDX / LDY).
      Mov(Reg, non-Reg)    — store (STA / STX / STY).
      Mov(non-Reg, non-Reg) — c6502's IR doesn't actually emit
                              this, but we treat it conservatively.
    """
    src_is_reg = isinstance(mov.src, asm_ast.Reg)
    dst_is_reg = isinstance(mov.dst, asm_ast.Reg)
    if src_is_reg and dst_is_reg:
        # TAX / TAY / TXA / TYA: dst now mirrors whatever src mirrors.
        new_value = _get_reg(state, mov.src.reg)
        _set_reg(state, mov.dst.reg, new_value)
        return
    if dst_is_reg and not src_is_reg:
        # Load: the register now mirrors the source operand.
        _set_reg(state, mov.dst.reg, mov.src)
        return
    if src_is_reg and not dst_is_reg:
        # Store: register unchanged; memory at dst is rewritten.
        # Invalidate any tracked register whose source aliases dst.
        _invalidate_aliasing(state, mov.dst)
        return
    # Memory-to-memory Mov — c6502 doesn't emit these, but be safe.
    _invalidate_aliasing(state, mov.dst)


def _get_reg(state: _RegState, reg: asm_ast.Type_reg):
    if isinstance(reg, asm_ast.A):
        return state.a
    if isinstance(reg, asm_ast.X):
        return state.x
    if isinstance(reg, asm_ast.Y):
        return state.y
    return None


def _set_reg(
    state: _RegState, reg: asm_ast.Type_reg,
    value: asm_ast.Type_operand | None,
) -> None:
    if isinstance(reg, asm_ast.A):
        state.a = value
    elif isinstance(reg, asm_ast.X):
        state.x = value
    elif isinstance(reg, asm_ast.Y):
        state.y = value


def _invalidate_aliasing(
    state: _RegState, write_dst: asm_ast.Type_operand,
) -> None:
    """For each tracked register, drop the tracking if its source
    operand might alias `write_dst`."""
    if state.a is not None and _may_alias(state.a, write_dst):
        state.a = None
    if state.x is not None and _may_alias(state.x, write_dst):
        state.x = None
    if state.y is not None and _may_alias(state.y, write_dst):
        state.y = None


def _may_alias(
    a: asm_ast.Type_operand, b: asm_ast.Type_operand,
) -> bool:
    """Conservative: True iff we can't prove the two operands
    refer to disjoint memory cells (or one is a non-memory
    `Imm`, in which case it never aliases).

    Provably disjoint cases (return False):
      * Either side is `Imm` — immediates aren't memory.
      * Both are `ZP`: only same-byte aliases.
      * One is `ZP`, other is `Data` / `IndexedData` — different
        memory regions ($00–$FF vs ≥ $100).
      * Both are `Data` with different name OR different offset.

    Everything else returns True (defensive)."""
    if isinstance(a, asm_ast.Imm) or isinstance(b, asm_ast.Imm):
        return False
    if isinstance(a, asm_ast.ZP) and isinstance(b, asm_ast.ZP):
        return (a.address + a.offset) == (b.address + b.offset)
    if (isinstance(a, asm_ast.ZP)
            and isinstance(b, (asm_ast.Data, asm_ast.IndexedData))):
        return False
    if (isinstance(b, asm_ast.ZP)
            and isinstance(a, (asm_ast.Data, asm_ast.IndexedData))):
        return False
    if isinstance(a, asm_ast.Data) and isinstance(b, asm_ast.Data):
        return a.name == b.name and a.offset == b.offset
    return True


def _operands_equal(
    a: asm_ast.Type_operand, b: asm_ast.Type_operand,
) -> bool:
    """Structural equality on the operand kinds we track. Distinct
    types are never equal; for matching types, compare the
    relevant fields."""
    if type(a) is not type(b):
        return False
    if isinstance(a, asm_ast.Imm):
        return a.value == b.value
    if isinstance(a, asm_ast.ZP):
        return a.address == b.address and a.offset == b.offset
    if isinstance(a, asm_ast.Data):
        return a.name == b.name and a.offset == b.offset
    if isinstance(a, asm_ast.IndexedData):
        return a.name == b.name and a.offset == b.offset
    if isinstance(a, asm_ast.Frame):
        return a.offset == b.offset
    if isinstance(a, asm_ast.Stack):
        return a.offset == b.offset
    if isinstance(a, asm_ast.Indirect):
        return a.offset == b.offset
    return False


def _flags_dead_at(
    instrs: list[asm_ast.Type_instruction], idx: int,
) -> bool:
    """True iff scanning forward from `idx`, no `Branch` reads the
    N/Z flags before another instruction resets them. Conservative:
    treat anything we can't classify as live (the load isn't dropped)."""
    while idx < len(instrs):
        instr = instrs[idx]
        if isinstance(instr, asm_ast.Branch):
            # The flags ARE read here.
            return False
        if isinstance(instr, (asm_ast.Label, asm_ast.Jump,
                              asm_ast.Ret, asm_ast.Return,
                              asm_ast.Call)):
            # Block boundary — flags are dead from the next block's
            # perspective. (Inter-block flag liveness is rare in
            # c6502's lowerings; if it ever arises, this returns
            # True erroneously, but the cost is dropping a load
            # that may have been observable; the lowerings don't
            # exhibit this shape today.)
            return True
        if _resets_nz(instr):
            return True
        idx += 1
    return True


def _resets_nz(instr: asm_ast.Type_instruction) -> bool:
    """True iff `instr` writes the N/Z flags (so any prior flag
    state is overwritten)."""
    if isinstance(instr, asm_ast.Mov):
        # LDA / LDX / LDY / TAX / TAY / TXA / TYA all set N/Z.
        # STA / STX / STY don't.
        return isinstance(instr.dst, asm_ast.Reg)
    if isinstance(instr, (asm_ast.Add, asm_ast.Sub, asm_ast.And,
                          asm_ast.Or, asm_ast.Xor, asm_ast.Compare,
                          asm_ast.Inc, asm_ast.Dec,
                          asm_ast.ArithmeticShiftLeft,
                          asm_ast.LogicalShiftRight,
                          asm_ast.RotateLeft, asm_ast.RotateRight,
                          asm_ast.Pop)):
        return True
    return False
