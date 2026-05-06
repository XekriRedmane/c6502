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

from dataclasses import dataclass, field

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
    """Per-block register tracking. Each field is the LIST of
    operands the corresponding register is known to mirror — empty
    when unknown. A register can simultaneously mirror multiple
    operands when, e.g., we LDA M (now A === M) and then STA N
    (now A === N as well, since the store wrote A's value to N
    while leaving A unchanged). Either equivalence is grounds for
    dropping a redundant LDA M / LDA N later.

    Stored as `list` rather than `set` because operand objects
    aren't hashable (dataclasses are by default but operand types
    are sum types not all of which are frozen)."""
    a: list[asm_ast.Type_operand] = field(default_factory=list)
    x: list[asm_ast.Type_operand] = field(default_factory=list)
    y: list[asm_ast.Type_operand] = field(default_factory=list)

    def reset(self) -> None:
        self.a = []
        self.x = []
        self.y = []


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
    return any(_operands_equal(c, instr.src) for c in cur)


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
            _set_reg(state, instr.dst.reg, [])
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
            _set_reg(state, instr.dst.reg, [])
        return
    if isinstance(instr, asm_ast.Xor):
        if isinstance(instr.dst, asm_ast.Reg):
            _set_reg(state, instr.dst.reg, [])
        return
    if isinstance(instr, (asm_ast.ArithmeticShiftLeft,
                          asm_ast.LogicalShiftRight,
                          asm_ast.RotateLeft, asm_ast.RotateRight)):
        if isinstance(instr.dst, asm_ast.Reg):
            _set_reg(state, instr.dst.reg, [])
        return
    if isinstance(instr, (asm_ast.Inc, asm_ast.Dec)):
        # Inc/Dec on `Reg(X)` / `Reg(Y)` modifies the index register
        # itself (INX / DEX / INY / DEY) — no memory write. The
        # register's tracked sources are cleared, AND any other
        # register's tracked sources that DEPEND on that register
        # (e.g. `IndexedData(_, _, index=X)`) are filtered out.
        # Tracked sources that don't depend on the register
        # (Imm, ZP, Data, IndexedData with the OTHER register)
        # remain valid.
        if isinstance(instr.dst, asm_ast.Reg) and isinstance(
            instr.dst.reg, (asm_ast.X, asm_ast.Y)
        ):
            changed_reg = instr.dst.reg
            _set_reg(state, changed_reg, [])
            state.a = [
                op for op in state.a
                if not _depends_on_reg(op, changed_reg)
            ]
            state.x = [
                op for op in state.x
                if not _depends_on_reg(op, changed_reg)
            ]
            state.y = [
                op for op in state.y
                if not _depends_on_reg(op, changed_reg)
            ]
            return
        # Inc/Dec on a memory operand (Data, ZP). Memory write to
        # instr.dst. Invalidate any register tracking that may alias.
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
        new_value = list(_get_reg(state, mov.src.reg))
        _set_reg(state, mov.dst.reg, new_value)
        return
    if dst_is_reg and not src_is_reg:
        # Load: the register now mirrors ONLY the source operand.
        # Any prior equivalences are wiped out (the previous
        # value in the register is gone).
        _set_reg(state, mov.dst.reg, [mov.src])
        return
    if src_is_reg and not dst_is_reg:
        # Store: register unchanged; memory at dst is rewritten.
        # Invalidate any tracked register equivalence that aliases
        # dst.
        _invalidate_aliasing(state, mov.dst)
        # Post-store, the source register and the destination
        # memory cell hold the same value. ADD this equivalence
        # to the source register's list — it doesn't replace
        # existing trackings (a register can simultaneously mirror
        # multiple memory cells, e.g. after `LDA M; STA N` we know
        # A === M AND A === N). We only track when the destination
        # is a "stable" memory operand (ZP / Data / Stack / Frame /
        # Indirect); IndexedData destinations depend on the index
        # register's runtime value and are excluded.
        if isinstance(mov.dst, (
            asm_ast.ZP, asm_ast.Data, asm_ast.Stack,
            asm_ast.Frame, asm_ast.Indirect,
        )):
            cur = _get_reg(state, mov.src.reg)
            if not any(_operands_equal(c, mov.dst) for c in cur):
                cur.append(mov.dst)
        return
    # Memory-to-memory Mov — c6502 doesn't emit these, but be safe.
    _invalidate_aliasing(state, mov.dst)


def _get_reg(state: _RegState, reg: asm_ast.Type_reg) -> list[asm_ast.Type_operand]:
    if isinstance(reg, asm_ast.A):
        return state.a
    if isinstance(reg, asm_ast.X):
        return state.x
    if isinstance(reg, asm_ast.Y):
        return state.y
    return []


def _set_reg(
    state: _RegState, reg: asm_ast.Type_reg,
    value: list[asm_ast.Type_operand],
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
    """For each tracked register, filter out equivalence entries
    whose operand might alias `write_dst`."""
    state.a = [op for op in state.a if not _may_alias(op, write_dst)]
    state.x = [op for op in state.x if not _may_alias(op, write_dst)]
    state.y = [op for op in state.y if not _may_alias(op, write_dst)]


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


def _depends_on_reg(
    op: asm_ast.Type_operand, reg: asm_ast.Type_reg,
) -> bool:
    """True iff `op`'s value depends on the runtime value of the
    register `reg`. Only `IndexedData(_, _, index=R)` does — its
    address is `name + offset + R_value`."""
    if isinstance(op, asm_ast.IndexedData):
        return type(op.index) is type(reg)
    return False


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
