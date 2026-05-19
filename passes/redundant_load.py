"""Redundant load elimination.

A 6502 register tracker. Tracks which operand each of A / X / Y
is currently a copy of. When the next instruction is `LDA M` (or
`LDX M` / `LDY M`) and the target register already mirrors `M`,
the load is redundant and we drop it.

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

# Cross-block must-availability dataflow

The tracker runs as a forward must-analysis over the function's
CFG: at each basic block's entry, the state is the intersection
of every predecessor's exit state. A register equivalence (A ===
M, say) is preserved into a successor block iff every path from
ENTRY to that successor's entry leaves the equivalence intact.

The headline shape this catches that a per-block tracker misses
is the diamond merge:

    LDX __zpabi_..._slot       ; entry sets X = slot
    ...
    BCC .merge                  ; both preds carry X = slot through —
    ...                         ; neither branch path modifies X or
    BCS .merge                  ; slot
.merge:
    LDX __zpabi_..._slot       ; redundant on both incoming paths

Per-block tracking forgets X's equivalence at .merge (multiple
preds → reset). The intersection-based join recovers it whenever
*every* incoming path agreed on the value.

State lattice (per register / per z_reflects):

  * Bottom = `[]` (no equivalences known).
  * Per-block transfer ADDS equivalences (loads, stores tracked
    as new mirrors) and REMOVES them (clobbering writes via the
    aliasing lattice).
  * Join at multi-pred labels = list intersection on `_operands_
    equal`. Anything not present on *every* incoming path drops.

`Call`, `FunctionPrologue`, `AllocateStack`, and `LoadAddress`
are treated as full register clobbers inside the transfer —
they expand into multi-instruction sequences inside
`asm_to_asm2`, and tracking the inner state would mean mirroring
the expansion here.

# Where it runs

After `replace_pseudoregisters` (operands are concrete) and
`apply_direct_index_load` (so we see `LDX zp` directly rather
than `LDA zp; TAX`); before `expand_long_branches` (we never
add new branches — the pass only deletes — but the ordering
keeps us symmetric with `inc_peephole` and
`apply_direct_index_load`).
"""
from __future__ import annotations

from collections.abc import Iterable
from dataclasses import dataclass, field

import asm_ast
from passes.asm_aliasing import may_alias as _may_alias
from passes.asm_liveness import flags_dead_at as _flags_dead_at
from passes.optimization_asm.cfg import (
    CFG, ENTRY_ID, EXIT_ID, BasicBlock, build_cfg,
)


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
    """CFG-based forward must-availability dataflow.

    Two-phase: (1) iterate IN / OUT register-equivalence states
    over the function's CFG to a fixed point, joining at multi-pred
    labels by list intersection on `_operands_equal`; (2) walk the
    original instruction list with IN[block] as the starting state
    at each block boundary and drop redundant loads as we go.

    Soundness: an equivalence in `in_state[B]` means every path
    from ENTRY to B's entry leaves that equivalence intact. So a
    load whose dst register's IN-state list contains the load's
    src is genuinely redundant.
    """
    if not fn.instructions:
        return fn

    cfg = _build_cfg_tolerant(fn)

    # Forward must-availability dataflow.
    # in_state[bid] = state at block entry (after its leading
    #                 Label, if any).
    # out_state[bid] = state at block exit (just before its
    #                  terminator, if any).
    # `None` = the block hasn't been reached from ENTRY yet; treat
    # as "skip in the join" so loops with not-yet-computed back
    # edges don't immediately collapse to empty.
    in_state: dict[int, _RegState | None] = {
        bid: None for bid in cfg.blocks
    }
    out_state: dict[int, _RegState | None] = {
        bid: None for bid in cfg.blocks
    }
    in_state[ENTRY_ID] = _RegState()
    out_state[ENTRY_ID] = _RegState()

    # Seed the worklist with every reachable-from-entry block id in
    # source order. Subsequent iterations re-add successors as
    # their predecessors' out-states change. Bounded by lattice
    # height × block count.
    worklist: list[int] = list(cfg.block_order)
    in_worklist: set[int] = set(worklist)
    max_iters = 1000 * (len(cfg.blocks) + 1)
    iters = 0
    while worklist and iters < max_iters:
        iters += 1
        bid = worklist.pop(0)
        in_worklist.discard(bid)
        if bid in (ENTRY_ID, EXIT_ID):
            continue
        block = cfg.blocks[bid]
        # Compute new IN as the intersection over initialized
        # predecessors only. Uninitialized predecessors (back
        # edges on the first sweep, unreachable preds) are skipped
        # — the lattice only narrows, so they get folded in later
        # when they get computed.
        init_pred_outs = [
            out_state[pid] for pid in block.predecessors
            if out_state[pid] is not None
        ]
        if not init_pred_outs:
            continue
        new_in = _join_states(init_pred_outs)
        old_in = in_state[bid]
        if old_in is not None and _state_equal(new_in, old_in):
            continue
        in_state[bid] = new_in
        new_out = _transfer_block(block, new_in)
        old_out = out_state[bid]
        if old_out is None or not _state_equal(new_out, old_out):
            out_state[bid] = new_out
            for sid in block.successors:
                if sid == EXIT_ID:
                    continue
                if sid not in in_worklist:
                    worklist.append(sid)
                    in_worklist.add(sid)

    return _rewrite_with_in_states(fn, cfg, in_state)


def _build_cfg_tolerant(fn: asm_ast.Function) -> CFG:
    """Wrap `build_cfg` to tolerate Branch / Jump targets that
    aren't defined as Labels in the function. Real compiled
    functions always have every target resolve (the assembler
    would reject otherwise), but synthetic test inputs sometimes
    use a target as a stand-in placeholder. We append a synthetic
    `Label + Return` to the (temporary) instruction list for each
    such target so `build_cfg` produces a well-formed CFG; the
    final rewrite walks only the ORIGINAL instructions, so the
    synthetics never appear in the output.
    """
    labels_in_fn = {
        instr.name for instr in fn.instructions
        if isinstance(instr, asm_ast.Label)
    }
    unresolved: list[str] = []
    seen: set[str] = set()
    for instr in fn.instructions:
        if isinstance(instr, (asm_ast.Branch, asm_ast.Jump)):
            if instr.target not in labels_in_fn and instr.target not in seen:
                seen.add(instr.target)
                unresolved.append(instr.target)
    if not unresolved:
        return build_cfg(fn)
    new_instrs = list(fn.instructions)
    for target in unresolved:
        new_instrs.append(asm_ast.Label(name=target))
        new_instrs.append(asm_ast.Return(save_a=False))
    synthetic = asm_ast.Function(
        name=fn.name, is_global=fn.is_global,
        params=list(fn.params), instructions=new_instrs,
    )
    return build_cfg(synthetic)


def _transfer_block(
    block: BasicBlock, in_state: _RegState,
) -> _RegState:
    """Apply `block`'s instructions to a copy of `in_state` and
    return the resulting out-state. Skips the block's leading
    `Label` (a marker, not a state-changing instruction) and its
    trailing terminator (`Jump` / `Branch` / `Ret` / `Return`)
    — terminators don't modify register state in our model; the
    CFG carries the state into successors directly."""
    state = _clone_state(in_state)
    for instr in block.instructions:
        if isinstance(instr, asm_ast.Label):
            continue
        if isinstance(instr, (asm_ast.Jump, asm_ast.Branch,
                              asm_ast.Ret, asm_ast.Return)):
            continue
        _update_state(instr, state)
    return state


def _rewrite_with_in_states(
    fn: asm_ast.Function, cfg: CFG,
    in_state: dict[int, _RegState | None],
) -> asm_ast.Function:
    """Final walk: visit each instruction in source order with
    IN[block] as the starting state at each block boundary. Drop
    redundant loads as the per-instruction tracker discovers them.
    Unreachable blocks (IN still `None`) fall back to an empty
    state — conservative but correct."""
    # Map each instruction's source position to its owning block id.
    pos_to_block: dict[int, int] = {}
    pos = 0
    for bid in cfg.block_order:
        for _ in cfg.blocks[bid].instructions:
            pos_to_block[pos] = bid
            pos += 1

    instrs = fn.instructions
    state = _RegState()
    last_block: int | None = None
    out: list[asm_ast.Type_instruction] = []
    for i, instr in enumerate(instrs):
        cur_block = pos_to_block.get(i)
        if cur_block != last_block:
            block_in = (
                in_state.get(cur_block) if cur_block is not None
                else None
            )
            state = _clone_state(block_in) if block_in is not None else _RegState()
            last_block = cur_block
        # Labels are block markers — skip the per-instruction
        # update (no state change) but keep them in the output.
        if isinstance(instr, asm_ast.Label):
            out.append(instr)
            continue
        # Terminators don't modify state in our model; emit them
        # as-is. (`_update_state` would reset on Jump/Ret/Return,
        # which is moot because the next instruction starts a new
        # block where we restore IN[block] anyway.)
        if isinstance(instr, (asm_ast.Jump, asm_ast.Branch,
                              asm_ast.Ret, asm_ast.Return)):
            out.append(instr)
            continue
        if _is_redundant_load(instr, state) and _flags_redundant_at(
            instrs, i, state,
        ):
            # Drop the load entirely — A / X / Y already mirror
            # the load's source, and either:
            #   - Z is already set to what the LDA would set it to
            #     (z_reflects covers src), so dropping doesn't
            #     disturb a downstream Branch, OR
            #   - No reachable Branch reads Z before another
            #     instruction overwrites it (`_flags_dead_at`).
            continue
        out.append(instr)
        _update_state(instr, state)

    return asm_ast.Function(
        name=fn.name, is_global=fn.is_global,
        params=list(fn.params), instructions=out,
    )


def _clone_state(s: _RegState) -> _RegState:
    return _RegState(
        a=list(s.a), x=list(s.x), y=list(s.y),
        z_reflects=list(s.z_reflects),
    )


def _join_states(states: Iterable[_RegState]) -> _RegState:
    """Intersection over `_RegState`s on `_operands_equal`. Caller
    guarantees at least one input — empty iterable would yield TOP,
    which we don't represent.

    `z_reflects` is dropped (not intersected) at any block with more
    than one predecessor. The two state kinds differ in how they're
    *produced*: A/X/Y equivalences are produced by the load itself
    (e.g., `LDA M` makes A === M, observable at the load), so a
    later drop based on cross-block agreement doesn't disturb the
    producer. `z_reflects` entries record that Z was set by some
    *upstream* flag-affecting instruction — possibly a different
    one on each incoming path. Dropping a consumer load based on
    "Z is already (M == 0)" relies on at least one producer per
    path surviving, but downstream DSE / dead-A passes don't see
    the cross-block Z dependency and may delete an upstream STA /
    LDA that was the only thing keeping Z set on one path. The
    safe rule: only carry `z_reflects` across an edge with no
    other incoming alternative."""
    states = list(states)
    assert states, "join over zero predecessors"
    result = _clone_state(states[0])
    for s in states[1:]:
        result.a = _intersect_operands(result.a, s.a)
        result.x = _intersect_operands(result.x, s.x)
        result.y = _intersect_operands(result.y, s.y)
    if len(states) > 1:
        result.z_reflects = []
    return result


def _intersect_operands(
    l1: list[asm_ast.Type_operand],
    l2: list[asm_ast.Type_operand],
) -> list[asm_ast.Type_operand]:
    """Multiset-style intersection on `_operands_equal`. Preserves
    `l1`'s element order so the dataflow is deterministic."""
    return [
        op for op in l1
        if any(_operands_equal(op, b) for b in l2)
    ]


def _state_equal(s1: _RegState, s2: _RegState) -> bool:
    return (
        _operand_list_equal(s1.a, s2.a)
        and _operand_list_equal(s1.x, s2.x)
        and _operand_list_equal(s1.y, s2.y)
        and _operand_list_equal(s1.z_reflects, s2.z_reflects)
    )


def _operand_list_equal(
    l1: list[asm_ast.Type_operand],
    l2: list[asm_ast.Type_operand],
) -> bool:
    """Order-insensitive equality on `_operands_equal`. Used by the
    dataflow fixed-point check — two states are equal iff their
    register equivalence lists contain the same operands (any
    order)."""
    if len(l1) != len(l2):
        return False
    matched = [False] * len(l2)
    for op in l1:
        for j, op2 in enumerate(l2):
            if not matched[j] and _operands_equal(op, op2):
                matched[j] = True
                break
        else:
            return False
    return True


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
    #
    # Self-Mov `Mov(M, M)` is a no-op: `asm_emit` drops both the
    # LDA and STA (`src == dst` peephole at `asm_emit.py:513`), so
    # A is NOT loaded and state shouldn't pretend it was. This
    # arises from SSA destruction emitting an intra-color copy
    # when a Phi src and dst land at the same byte.
    if _operands_equal(mov.src, mov.dst):
        return
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


