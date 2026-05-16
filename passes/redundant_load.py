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

# Flag soundness — the Z-reflects tracker

Dropping `LDA M` skips re-setting Z to "(M == 0)". A subsequent
`Branch(EQ|NE)` then reads whatever Z was left at by the prior
flag-affecting instruction — which may not match what `LDA M`
would have produced. Two independent paths to soundness:

  1. **Flag dead.** No reachable instruction reads N/Z before
     another instruction overwrites them: dropping is safe
     regardless of Z's current value. `_flags_dead_at` answers
     this with a forward CFG scan.

  2. **Flag already correct.** Some earlier instruction already
     set Z to "(M == 0)" and nothing since has touched it:
     `LDA M`'s flag effect is redundant. `z_reflects` tracks
     which operands' zeroness Z currently reflects.

`z_reflects` is a LIST because multiple cells can be
simultaneously zero-equivalent: after `LDA M; STA N; STA P`,
Z reflects M === N === P. Same shape as `state.a/x/y`. Update
rules track every flag-affecting opcode (LDA, ADC, SBC, AND,
ORA, EOR, INC, DEC, shifts, CMP, BIT, mem-to-mem Movs).

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
from passes.asm_aliasing import may_alias as _may_alias
from passes.asm_liveness import flags_dead_at as _flags_dead_at


# Operand kinds with stable (non-index-dependent) addresses — the
# kinds we're willing to add to a register's equivalence class or
# to z_reflects after a write. `IndexedData` is excluded because
# its address depends on the index register's runtime value.
_STABLE_MEM_TYPES = (
    asm_ast.ZP, asm_ast.Data, asm_ast.Stack,
    asm_ast.Frame, asm_ast.Indirect,
)


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
    are sum types not all of which are frozen).

    `z_reflects` separately tracks which operands' current values
    the Z flag matches the zeroness of. See the module docstring's
    "Flag soundness" section — entry `M` in `z_reflects` means
    `Z is currently set to (M's current value == 0)`, regardless
    of what state.a/x/y look like. The two trackers are mostly
    parallel (after `LDA M`, both list M), but they can diverge
    when an instruction overwrites only one (e.g., `INC P` keeps
    A's value but sets Z to (P's new value == 0) — state.a stays,
    z_reflects becomes [P])."""
    a: list[asm_ast.Type_operand] = field(default_factory=list)
    x: list[asm_ast.Type_operand] = field(default_factory=list)
    y: list[asm_ast.Type_operand] = field(default_factory=list)
    z_reflects: list[asm_ast.Type_operand] = field(default_factory=list)

    def reset(self) -> None:
        self.a = []
        self.x = []
        self.y = []
        self.z_reflects = []


def _rewrite_function(fn: asm_ast.Function) -> asm_ast.Function:
    state = _RegState()
    instrs = fn.instructions
    # Set of label names that are the target of some Branch or
    # Jump in this function. A `Label` whose name is NOT in this
    # set has only the immediate fall-through as a predecessor —
    # its register-mirror state at entry equals the state at the
    # preceding instruction's exit, so we don't need to reset.
    branch_targets = _collect_branch_targets(instrs)
    out: list[asm_ast.Type_instruction] = []
    for i, instr in enumerate(instrs):
        if _is_redundant_load(instr, state) and _flags_redundant_at(
            instrs, i, state,
        ):
            # Drop the load entirely — A / X / Y already mirror
            # the load's source, and either:
            #   - The Z flag is already set to what the LDA would
            #     set it to (z_reflects covers the LDA's src), so
            #     dropping doesn't disturb a downstream Branch, OR
            #   - No reachable Branch reads Z before another
            #     instruction overwrites it (`_flags_dead_at`).
            continue
        out.append(instr)
        _update_state(instr, state, branch_targets)
    return asm_ast.Function(
        name=fn.name, is_global=fn.is_global,
        params=list(fn.params), instructions=out,
    )


def _flags_redundant_at(
    instrs: list[asm_ast.Type_instruction],
    i: int,
    state: _RegState,
) -> bool:
    """Decide whether the N/Z flag effect of `instrs[i]` (a load
    we're considering dropping) is safe to skip.

    Two ways for it to be safe — the disjunction matters because
    either one alone wouldn't catch all the cases the inner-loop
    optimization needs:

      1. The flags are dead at `i+1` (`_flags_dead_at`). No
         reachable `Branch` reads N/Z before another instruction
         overwrites them, so the load's flag effect is unobservable.

      2. `state.z_reflects` already covers the load's source M.
         Z is currently set to "is M zero?" — the same state the
         dropped LDA would set it to. Subsequent Branch reads see
         the right value.

    Both checks are sound on their own; either suffices. We try
    the cheap structural-equality check (`z_reflects`) first, then
    fall back to the CFG walk (`_flags_dead_at`).
    """
    instr = instrs[i]
    if not isinstance(instr, asm_ast.Mov):
        # `_is_redundant_load` already gates on Mov, so this branch
        # is dead in practice — defensive only.
        return _flags_dead_at(instrs, i + 1)
    if any(_operands_equal(z, instr.src) for z in state.z_reflects):
        return True
    return _flags_dead_at(instrs, i + 1)


def _collect_branch_targets(instrs) -> set[str]:
    """Set of label names referenced by any `Jump` or `Branch` in
    `instrs`. Used to decide whether a `Label` introduces a new
    block: a label that nothing branches/jumps to has only the
    fall-through predecessor, so the prior instruction's register
    state still applies at the label."""
    out: set[str] = set()
    for instr in instrs:
        if isinstance(instr, asm_ast.Jump):
            out.add(instr.target)
        elif isinstance(instr, asm_ast.Branch):
            out.add(instr.target)
    return out


def _is_redundant_load(
    instr: asm_ast.Type_instruction, state: _RegState,
) -> bool:
    if not isinstance(instr, asm_ast.Mov):
        return False
    # A volatile load must re-read the memory cell every time per
    # C99 §6.7.3.6 — never elide even if A already mirrors the
    # value from an earlier read.
    if instr.is_volatile:
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
    branch_targets: set[str] | None = None,
) -> None:
    """Apply `instr`'s effect to the tracked register AND z_reflects
    state. SBC/ADC/AND/OR/XOR/shift-of-A clear state.a and
    z_reflects in parallel, which is what the STA equivalence-class
    code in `_update_for_mov` relies on for the "both empty" branch.
    """
    if isinstance(instr, asm_ast.Mov):
        _update_for_mov(instr, state)
        return
    if isinstance(instr, asm_ast.Label):
        # Reset at join points only — a fall-through-only label
        # preserves the prior block's register state.
        if branch_targets is None or instr.name in branch_targets:
            state.reset()
        return
    if isinstance(instr, (asm_ast.Jump,
                          asm_ast.Ret, asm_ast.Return)):
        state.reset()
        return
    if isinstance(instr, asm_ast.Branch):
        # Fall-through preserves register state (Branch writes
        # neither registers nor flags).
        return
    if isinstance(instr, asm_ast.Call):
        # JSR may clobber any register, memory, or flag.
        state.reset()
        return
    if isinstance(instr, (asm_ast.FunctionPrologue,
                          asm_ast.AllocateStack,
                          asm_ast.LoadAddress)):
        # These expand into multi-instruction sequences in
        # asm_to_asm2; their inner effects exceed the tracker.
        state.reset()
        return
    if isinstance(instr, asm_ast.Pop):
        # PLA/PLX/PLY: dest register loaded with unknown value;
        # N/Z set off that value.
        if isinstance(instr.dst, asm_ast.Reg):
            _set_reg(state, instr.dst.reg, [])
        state.z_reflects = []
        return
    if isinstance(instr, asm_ast.Push):
        return
    if isinstance(instr, (asm_ast.SetCarry, asm_ast.ClearCarry)):
        return
    if isinstance(instr, (asm_ast.Compare, asm_ast.BitTest)):
        # CMP and BIT both set Z to a relation between A and the
        # operand, not to "operand's zeroness" — clear z_reflects.
        state.z_reflects = []
        return
    if isinstance(instr, (asm_ast.Add, asm_ast.Sub,
                          asm_ast.And, asm_ast.Or, asm_ast.Xor)):
        # ADC/SBC/AND/ORA/EOR always write Reg(A) in our IR. A's
        # identity is lost; Z reflects A's new (untracked) value.
        if isinstance(instr.dst, asm_ast.Reg):
            _set_reg(state, instr.dst.reg, [])
        state.z_reflects = []
        return
    if isinstance(instr, (asm_ast.ArithmeticShiftLeft,
                          asm_ast.LogicalShiftRight,
                          asm_ast.RotateLeft, asm_ast.RotateRight)):
        if isinstance(instr.dst, asm_ast.Reg):
            _set_reg(state, instr.dst.reg, [])
            state.z_reflects = []
            return
        _invalidate_aliasing(state, instr.dst)
        _invalidate_z_aliasing(state, instr.dst)
        state.z_reflects.append(instr.dst)
        return
    if isinstance(instr, (asm_ast.Inc, asm_ast.Dec)):
        if isinstance(instr.dst, asm_ast.Reg) and isinstance(
            instr.dst.reg, (asm_ast.X, asm_ast.Y)
        ):
            # INX/DEX/INY/DEY: drop the register's own equivalences
            # AND any other register's tracking of an
            # IndexedData(_, _, index=changed_reg) operand whose
            # address depended on the prior register value.
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
            state.z_reflects = []
            return
        # Inc/Dec on a memory operand: Z = (cell's new value == 0).
        _invalidate_aliasing(state, instr.dst)
        state.z_reflects = [instr.dst]
        return
    return


def _update_for_mov(mov: asm_ast.Mov, state: _RegState) -> None:
    """Apply a Mov's effect to state.a/x/y and z_reflects. Mov has
    four shapes: register-transfer, load, store, and mem-to-mem
    (emit lowers to `LDA src; STA dst`).
    """
    if mov.is_volatile:
        if isinstance(mov.dst, asm_ast.Reg):
            # Volatile LDA M: clear state.<reg> so no future LDA M
            # can elide. z_reflects gets M because Z's current
            # value IS (M-at-read == 0) for the immediate downstream
            # Branch; a later LDA M wouldn't elide anyway (the
            # volatile gate rejects).
            _set_reg(state, mov.dst.reg, [])
            if not isinstance(mov.src, asm_ast.Reg):
                state.z_reflects = [mov.src]
            else:
                state.z_reflects = []
            return
        if isinstance(mov.src, asm_ast.Reg):
            # Volatile STA M. Don't add M to the source register's
            # equivalence class — a future LDA M wouldn't be
            # elidable anyway, so the equivalence is unreachable.
            _invalidate_aliasing(state, mov.dst)
            _invalidate_z_aliasing(state, mov.dst)
            return
        # Volatile memory-to-memory Mov: emit lowers to
        # `LDA src; STA dst`. The is_volatile bit often comes from
        # dst being volatile-typed even when src itself is a stable
        # non-volatile cell (e.g. `Copy(temp, volatile_y)` in the
        # sfx_tone case); in that case A === src is still a valid
        # equivalence post-Mov, so we add src to state.a when src
        # is a stable-address operand.
        _invalidate_aliasing(state, mov.dst)
        _invalidate_z_aliasing(state, mov.dst)
        if isinstance(mov.src, _STABLE_MEM_TYPES):
            if not any(_operands_equal(c, mov.src) for c in state.a):
                state.a.append(mov.src)
            if not any(
                _operands_equal(z, mov.src) for z in state.z_reflects
            ):
                state.z_reflects.append(mov.src)
        return
    src_is_reg = isinstance(mov.src, asm_ast.Reg)
    dst_is_reg = isinstance(mov.dst, asm_ast.Reg)
    if src_is_reg and dst_is_reg:
        # TAX / TAY / TXA / TYA: dst now mirrors whatever src mirrors.
        # Z is set based on dst's new value (== src's value).
        # Conservative: if src's identity is tracked (state[src]
        # non-empty), z_reflects matches that list — both reflect
        # the same value. Otherwise clear.
        new_value = list(_get_reg(state, mov.src.reg))
        _set_reg(state, mov.dst.reg, new_value)
        # Transfers between registers DO set N/Z (except TXS, which
        # asm_ast doesn't model). For TAX/TAY/TXA/TYA, Z reflects
        # the transferred byte's zeroness — same as the source
        # register's identity.
        state.z_reflects = list(new_value)
        return
    if dst_is_reg and not src_is_reg:
        # Load: the register now mirrors ONLY the source operand.
        # Any prior equivalences are wiped out (the previous
        # value in the register is gone). Z reflects (src == 0).
        _set_reg(state, mov.dst.reg, [mov.src])
        state.z_reflects = [mov.src]
        # When the load writes X or Y, also drop any tracking
        # in the OTHER registers that depends on the changed
        # index register — e.g. `IndexedData(_, _, index=X)`
        # entries in state.a become stale the moment X holds a
        # different value. Without this, a `LDA arr,X; LDX
        # #N; LDA arr,X` chain would incorrectly drop the
        # second load on the assumption that A still mirrors
        # `arr,X`. Mirrors the INX/DEX/INY/DEY invalidation
        # done lower in `_update_state`.
        if isinstance(mov.dst.reg, (asm_ast.X, asm_ast.Y)):
            changed_reg = mov.dst.reg
            if isinstance(changed_reg, asm_ast.X):
                state.a = [
                    op for op in state.a
                    if not _depends_on_reg(op, changed_reg)
                ]
                state.y = [
                    op for op in state.y
                    if not _depends_on_reg(op, changed_reg)
                ]
            else:  # Y
                state.a = [
                    op for op in state.a
                    if not _depends_on_reg(op, changed_reg)
                ]
                state.x = [
                    op for op in state.x
                    if not _depends_on_reg(op, changed_reg)
                ]
            state.z_reflects = [
                op for op in state.z_reflects
                if not _depends_on_reg(op, changed_reg)
            ]
        return
    if src_is_reg and not dst_is_reg:
        # Store. STA/STX/STY don't touch N/Z. Add dst to the source
        # register's equivalence class so a later `LDA dst` can
        # elide. Add dst to z_reflects only when Z was already
        # tracking the source register's value — i.e. the existing
        # z_reflects entries intersect the existing register
        # equivalence class, OR both are empty (Z reflects A's
        # current unknown value, and dst now equals that value).
        _invalidate_aliasing(state, mov.dst)
        if isinstance(mov.dst, _STABLE_MEM_TYPES):
            cur = _get_reg(state, mov.src.reg)
            cur_was_empty = not cur
            if not any(_operands_equal(c, mov.dst) for c in cur):
                cur.append(mov.dst)
            cur_pre_existing = [c for c in cur if c is not mov.dst]
            shared = any(
                _operands_equal(z, c)
                for z in state.z_reflects
                for c in cur_pre_existing
            )
            if shared or (not state.z_reflects and cur_was_empty):
                if not any(
                    _operands_equal(z, mov.dst)
                    for z in state.z_reflects
                ):
                    state.z_reflects.append(mov.dst)
        return
    # Memory-to-memory Mov: emit lowers to `LDA src; STA dst`. The
    # emit-time LDA sets Z to (src == 0); dst now holds that value
    # too. We don't add src to state.a or z_reflects because src
    # may be an index-dependent operand whose value future code
    # can change; the dst-side tracking is sufficient for the
    # common `LDA M; STA N; LDA N` shape.
    _invalidate_aliasing(state, mov.dst)
    _invalidate_z_aliasing(state, mov.dst)
    if isinstance(mov.dst, _STABLE_MEM_TYPES):
        state.a = [mov.dst]
        state.z_reflects.append(mov.dst)


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


def _invalidate_z_aliasing(
    state: _RegState, write_dst: asm_ast.Type_operand,
) -> None:
    """Drop z_reflects entries whose operand may alias `write_dst`."""
    state.z_reflects = [
        op for op in state.z_reflects if not _may_alias(op, write_dst)
    ]


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


