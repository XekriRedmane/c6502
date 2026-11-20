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

`z_reflects` holds several operands because multiple cells can be
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

  * Bottom = `()` (no equivalences known).
  * Per-block transfer ADDS equivalences (loads, stores tracked
    as new mirrors) and REMOVES them (clobbering writes via the
    aliasing lattice).
  * Join at multi-pred labels = list intersection on `_operands_
    equal`. Anything not present on *every* incoming path drops.

`Call`, `FunctionPrologue`, `AllocateStack`, and `LoadAddress`
are treated as full register clobbers inside the transfer —
they expand into multi-instruction sequences inside
`asm_to_asm2`, and tracking the inner state would mean mirroring
the expansion here. The one exception: a call to a routine
declared `preserves_x` / `preserves_y` keeps an immediate that X
(Y) holds, so `LDX #k; JSR f; LDX #k` loses its second load. A
memory equivalence still goes, since the callee may write the
memory.

# What is computed once

The dataflow visits a block again whenever its IN changes, and each
round of the first phase starts the analysis over, so the same
instructions are applied to the same states many times. Within one
call of `apply_redundant_load_elimination`, these are kept:

  * Each operand's `_Atom`, which every operand equal to it under
    `_operands_equal` shares (`_atom`), so equality is identity and
    the facts the updates read about an operand (its index register,
    whether it is stable memory or a constant) are found once.
  * Each instruction's facts (`_facts`), which are its effect on
    `z_tracks_a`, whether its addressing-mode setup writes Y, its
    operands' atoms, and every state it has been applied to with the
    state it gave.
    The update is a function of the instruction and the state's six
    fields alone, and the fields are tuples of atoms that no update
    changes in place, so a state seen before gives the same result.
  * `_may_alias` of each pair of atoms (`_without_aliasing`), which
    reads only the operands' fields and the `pointer_proof` set for
    the call.

The maps hold their instructions and operands, so no other object
takes one's `id` while they exist. Nothing changes an instruction or
an operand in place during the call, and the maps are emptied when it
returns, since later passes may and since `pointer_proof` changes.

# Where it runs

After `replace_pseudoregisters` (operands are concrete) and
`apply_direct_index_load` (so we see `LDX zp` directly rather
than `LDA zp; TAX`); before `expand_long_branches` (we never
add new branches — the pass only deletes — but the ordering
keeps us symmetric with `inc_peephole` and
`apply_direct_index_load`).
"""
from __future__ import annotations

from collections import Counter
from collections.abc import Iterable
from dataclasses import dataclass

import asm_ast
from passes.asm_aliasing import may_alias as _may_alias
from passes.asm_aliasing import pointer_proof, unreachable_by_pointer
from passes.asm_liveness import a_dead_at as _a_dead_at
from passes.asm_liveness import x_dead_at as _x_dead_at
from passes.asm_liveness import y_dead_at as _y_dead_at
from passes.asm_liveness import call_preserves as _call_preserves
from passes.asm_liveness import flags_dead_at as _flags_dead_at
from passes.asm_liveness import sets_up_y as _sets_up_y
from passes.asm_dead_store import mem_dead_after as _mem_dead_after
from passes.peephole_memo import FunctionMemo, rewrite_function
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


# How many times `_rewrite_function` runs its first phase again after
# a rewrite, at most.
_REORDER_ROUNDS = 64


# The zero-page address of each slot symbol and zero-page object for
# the run in progress, so that `_pointer_bytes` can tell whether a
# store writes a pointer's byte. Empty when the caller has no map.
_ZP_ADDRS: dict[str, int] = {}


def apply_redundant_load_elimination(
    prog: asm_ast.Program, *, zp_symbol_addrs: dict[str, int] | None = None,
    memo: FunctionMemo | None = None,
) -> asm_ast.Program:
    """`zp_symbol_addrs` maps slot symbols and zero-page objects to
    their addresses. Without it, a store to any slot may write the
    pointer of an `(zp),Y` access, which keeps the two stores of
    `_swap_pairs_ok` apart. Each function's rewrite reads the
    function, `zp_symbol_addrs`, and the names no pointer reaches,
    which are its context in `memo` (passes/peephole_memo.py)."""
    global _ZP_ADDRS
    new_top: list[asm_ast.Type_top_level] = []
    saved, _ZP_ADDRS = _ZP_ADDRS, dict(zp_symbol_addrs or {})
    try:
        unreachable = unreachable_by_pointer(prog)
        ctx = memo.context(unreachable) if memo is not None else None
        with pointer_proof(unreachable):
            for tl in prog.top_level:
                if isinstance(tl, asm_ast.Function):
                    new_top.append(
                        rewrite_function(memo, tl, ctx, _rewrite_function),
                    )
                else:
                    new_top.append(tl)
    finally:
        _ZP_ADDRS = saved
        # The maps hold what this run's `pointer_proof` gave, and the
        # instructions and operands of this run, which a later pass
        # may change.
        _FACTS.clear()
        _ALIAS.clear()
        _INTERN.clear()
        _ATOM_OF.clear()
    return asm_ast.Program(top_level=new_top)


@dataclass(slots=True)
class _RegState:
    """Per-block register tracking. Each field is the tuple of
    operands the corresponding register is known to mirror — empty
    when unknown. A register can simultaneously mirror multiple
    operands when, e.g., we LDA M (now A === M) and then STA N
    (now A === N as well, since the store wrote A's value to N
    while leaving A unchanged). Either equivalence is grounds for
    dropping a redundant LDA M / LDA N later.

    Each operand is held as its `_Atom`, which every operand equal to
    it under `_operands_equal` shares, so a membership test is `in`
    on atoms. Each field is a tuple in source order. The order
    decides which memory group a load takes (`_load_list`) and the
    order of the joined lists, and some operands (`IndirectZp`, the
    label immediates) equal no operand, themselves included, so a
    set wouldn't do. The tuples are never changed in place, so
    `_clone_state` copies only the references, an update replaces a
    field with a new tuple, and `_update_state` can look a state up
    by its fields (`_state_tuple`).

    `z_reflects` separately tracks which operands' current values
    the Z flag matches the zeroness of. See the module docstring's
    "Flag soundness" section — entry `M` in `z_reflects` means
    `Z is currently set to (M's current value == 0)`, regardless
    of what state.a/x/y look like. The two trackers are mostly
    parallel (after `LDA M`, both list M), but they can diverge
    when an instruction overwrites only one (e.g., `INC P` keeps
    A's value but sets Z to (P's new value == 0) — state.a stays,
    z_reflects becomes [P])."""
    a: tuple[_Atom, ...] = ()
    x: tuple[_Atom, ...] = ()
    y: tuple[_Atom, ...] = ()
    z_reflects: tuple[_Atom, ...] = ()
    # True when the last flag-setting instruction set N/Z from A's
    # current value (a load or computation into A, TAX / TAY). An
    # empty `z_reflects` alone doesn't say that: the flags may come
    # from an `LDY #off` setup, a compare, or an `LDX`.
    z_tracks_a: bool = False
    # Groups of at least two memory operands known to hold one value
    # that no register holds any more. A register's list becomes such
    # a group when the register is loaded with something else
    # (`_set_reg`), and a load of one of the group's operands makes
    # the register mirror all of them again (`_load_list`). After
    # `LDA t,X; STA p; LDA u,X; STA p+1; LDA (p),Y; ...; LDA t,X;
    # STA p`, the last store writes the value `p` holds, and goes.
    mem: tuple[tuple[_Atom, ...], ...] = ()

    def reset(self) -> None:
        self.a = ()
        self.x = ()
        self.y = ()
        self.z_reflects = ()
        self.z_tracks_a = False
        self.mem = ()


def _rewrite_function(
    fn: asm_ast.Function, reorder: int = _REORDER_ROUNDS,
) -> asm_ast.Function:
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

    Rewrites that change what a register or Z holds at the end of a
    block (`_swap_pairs_ok`, `_store_from_a`,
    `_store_const_via_index`) would make IN of a later block wrong,
    so they're a phase of their own, which stops at the end of the
    block where it first rewrites something. Then the analysis starts
    again on the rewritten code, at most `reorder` times more. A swap
    at the join before `for (i = 0; ...)` once left A holding another
    value where the next block's `i = 0` had become `STA i`.
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
    # The instructions of each block that change the state, found on
    # the first visit of the block.
    bodies: dict[int, tuple[asm_ast.Type_instruction, ...]] = {}
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
        body = bodies.get(bid)
        if body is None:
            body = bodies[bid] = _block_body(block)
        new_out = _transfer_block(body, new_in)
        old_out = out_state[bid]
        if old_out is None or not _state_equal(new_out, old_out):
            out_state[bid] = new_out
            for sid in block.successors:
                if sid == EXIT_ID:
                    continue
                if sid not in in_worklist:
                    worklist.append(sid)
                    in_worklist.add(sid)

    if reorder:
        reordered = _reorder_with_in_states(fn, cfg, in_state)
        if reordered is not None:
            # The phase stops at the end of the block it rewrote, so
            # it runs again on the rest with states that see the
            # rewrite.
            return _rewrite_function(reordered, reorder=reorder - 1)
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


_PASSIVE = frozenset((
    asm_ast.Label, asm_ast.Jump, asm_ast.Branch, asm_ast.Ret,
    asm_ast.Return,
))


def _block_body(block: BasicBlock) -> tuple[asm_ast.Type_instruction, ...]:
    """Returns the instructions of `block` that `_transfer_block`
    applies. It
    skips the block's leading `Label` (a marker, not a state-changing
    instruction) and its trailing terminator (`Jump` / `Branch` /
    `Ret` / `Return`), since terminators don't modify register state
    in our model and the CFG carries the state into successors
    directly."""
    return tuple(ins for ins in block.instructions
                 if type(ins) not in _PASSIVE)


def _transfer_block(
    body: tuple[asm_ast.Type_instruction, ...], in_state: _RegState,
) -> _RegState:
    """Applies a block's instructions (`_block_body`) to a copy of
    `in_state` and returns the resulting out-state."""
    state = _clone_state(in_state)
    for instr in body:
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
        # Terminators don't modify state in our model; emit them
        # as-is. (`_update_state` would reset on Jump/Ret/Return,
        # which is moot because the next instruction starts a new
        # block where we restore IN[block] anyway.)
        if type(instr) in _PASSIVE:
            out.append(instr)
            continue
        if type(instr) is not _MOV:
            # Every rewrite below is of a `Mov`.
            out.append(instr)
            _update_state(instr, state)
            continue
        if _reverses_transfer(instrs, i) or _repeats_transfer(instrs, i):
            # `TAX; TXA`: after the first, both registers and N/Z
            # hold the value already. `twice(v) { return add(v, v); }`
            # with `v` and both of add's arguments in registers. And
            # `TXA; BEQ l; TXA`, where only branches come between.
            continue
        redundant = _is_redundant_load(instr, state)
        if redundant and _flags_redundant_at(instrs, i, state):
            # Drop the load entirely — A / X / Y already mirror
            # the load's source, and either:
            #   - Z is already set to what the LDA would set it to
            #     (z_reflects covers src), so dropping doesn't
            #     disturb a downstream Branch, OR
            #   - No reachable Branch reads Z before another
            #     instruction overwrites it (`_flags_dead_at`).
            continue
        if (redundant
                and isinstance(instr.dst.reg, asm_ast.A)
                and not isinstance(instr.src, asm_ast.Reg)):
            # A holds M, but a branch reads the flags the `LDA M`
            # sets: `ORA #$00` sets them from A, which equals M, and
            # is never slower (`print_zstring`'s `STA c; BCC .l; RTS;
            # .l: LDA c; BNE` becomes `... .l: ORA #$00; BNE`). A is
            # unchanged, so every operand it mirrors is still there,
            # and Z now reflects all of them.
            out.append(asm_ast.Or(src=asm_ast.Imm(value=0), dst=instr.dst))
            state.z_reflects = state.a
            continue
        if _stores_held_value(instr, state):
            # The byte already holds the register's value: a group
            # of memory operands that held one value became the
            # register's list when it loaded one of them. A store
            # sets no flag, and the state is unchanged.
            continue
        redirected = _redirect_lda_to_transfer(instr, state)
        if redirected is not None:
            # Rewrite `LDA M` to `TXA` / `TYA` when X or Y already
            # mirrors M. Saves 1 byte (TXA/TYA = 1 byte vs LDA M = 2-3
            # bytes) and leaves A mirroring M too. The flag effect is
            # identical: LDA M and TXA both set N/Z to bit7(M)/(M==0)
            # since X == M under the mirror invariant. Pairs with
            # `via_a_store_fold` downstream, which then collapses
            # `TXA;STA N` to `STX N` when A is dead at N+1.
            out.append(redirected)
            _update_state(redirected, state)
            continue
        redirected = _redirect_index_load_to_transfer(
            instr, state, out[-1] if out else None,
        )
        if redirected is not None and (
            _named_object(instr.src) or _mem_dead_after(
                instrs, i, instr.src, fn_name=fn.name,
                zp_slot_symbols=_ZP_ADDRS,
            )
        ):
            out.append(redirected)
            _update_state(redirected, state)
            continue
        out.append(instr)
        _update_state(instr, state)

    return asm_ast.Function(
        name=fn.name, is_global=fn.is_global,
        params=list(fn.params), instructions=out,
    )


def _reorder_with_in_states(
    fn: asm_ast.Function, cfg: CFG,
    in_state: dict[int, _RegState | None],
) -> asm_ast.Function | None:
    """The first phase: walk as `_rewrite_with_in_states` does, but
    only to swap load-store pairs (`_swap_pairs_ok`), keeping the
    `LDA M` for the second phase to drop, to turn a store through
    A of a value A holds into the `STA` alone (`_store_from_a`), and
    to store a constant through Y or X when A holds a value worth
    keeping (`_store_const_via_index`).
    Returns None when neither applies anywhere."""
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
    changed = False
    i = 0
    while i < len(instrs):
        instr = instrs[i]
        cur_block = pos_to_block.get(i)
        if cur_block != last_block and changed:
            # IN of this block may no longer hold after the rewrite.
            out.extend(instrs[i:])
            break
        if cur_block != last_block:
            block_in = (
                in_state.get(cur_block) if cur_block is not None
                else None
            )
            state = _clone_state(block_in) if block_in is not None else _RegState()
            last_block = cur_block
        if type(instr) in _PASSIVE:
            out.append(instr)
            i += 1
            continue
        if type(instr) is not _MOV:
            # Each rewrite below starts at a `Mov`.
            out.append(instr)
            _update_state(instr, state)
            i += 1
            continue
        via = _store_const_via_index(instrs, i, state)
        if via is not None:
            # `LDA #k; STA M` with A holding a value from memory:
            # `LDY #k; STY M` keeps it, for the second phase to drop
            # its reload.
            replacement, consumed = via
            for ins in replacement:
                out.append(ins)
                _update_state(ins, state)
            changed = True
            i += consumed
            continue
        swap = _swap_pairs_ok(instrs, i, state)
        if swap is not None:
            # `LDA X; STA D1; LDA M; STA D2` with A already holding
            # M: `LDA M; STA D2; LDA X; STA D1`, whose `LDA M` the
            # second phase drops. Each load may have several stores.
            n1, n2 = swap
            second = list(range(i + 1 + n1, i + 2 + n1 + n2))
            first = list(range(i, i + 1 + n1))
            for k in second + first:
                out.append(instrs[k])
                _update_state(instrs[k], state)
            changed = True
            i += 2 + n1 + n2
            continue
        if _swap_loads_ok(instrs, i, state):
            # `LDX M1; LDA M2` with X already holding M2: `LDA M2;
            # LDX M1`, whose `LDA M2` the second phase makes `TXA`.
            for k in (i + 1, i):
                out.append(instrs[k])
                _update_state(instrs[k], state)
            changed = True
            i += 2
            continue
        stored = _store_from_a(instr, state)
        if stored is not None and _flags_redundant_at(instrs, i, state):
            # `Mov(M, D)` emits as `LDA M; STA D`; with A already
            # holding M, the `STA D` alone does it.
            out.append(stored)
            _update_state(stored, state)
            changed = True
            i += 1
            continue
        out.append(instr)
        _update_state(instr, state)
        i += 1
    if not changed:
        return None
    return asm_ast.Function(
        name=fn.name, is_global=fn.is_global,
        params=list(fn.params), instructions=out,
    )


def _clone_state(s: _RegState) -> _RegState:
    """Returns a copy of `s`. Its fields are tuples that no update
    changes in place, so the copy shares them."""
    return _RegState(s.a, s.x, s.y, s.z_reflects, s.z_tracks_a, s.mem)


def _memory_groups(s: _RegState) -> list[tuple[_Atom, ...]]:
    """Every group of operands `s` knows to hold one value: its
    memory groups and the lists of its registers."""
    return [*s.mem, s.a, s.x, s.y]


def _join_memory(
    result: _RegState, states: list[_RegState],
) -> tuple[tuple[_Atom, ...], ...]:
    """The memory groups that hold on every path in: each group of at
    least two operands that one group of every state contains, less
    the operands a register of the joined state still mirrors, which
    stay in that register's list."""
    groups = _memory_groups(states[0])
    for s in states[1:]:
        merged = []
        others = [g2 for g2 in _memory_groups(s) if g2]
        for g1 in groups:
            if not g1:
                continue
            for g2 in others:
                inter = _intersect_operands(g1, g2)
                if len(inter) >= 2:
                    merged.append(inter)
        groups = merged
    in_regs = {a for a in (*result.a, *result.x, *result.y)
               if a.key is not None}
    out: list[tuple[_Atom, ...]] = []
    for g in groups:
        g = tuple(a for a in g if a not in in_regs)
        if len(g) >= 2 and not any(_operand_list_equal(g, o) for o in out):
            out.append(g)
    return tuple(out)


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
        result.z_reflects = ()
        result.z_tracks_a = False
        result.mem = _join_memory(result, states)
    return result


def _intersect_operands(
    l1: tuple[_Atom, ...], l2: tuple[_Atom, ...],
) -> tuple[_Atom, ...]:
    """Multiset-style intersection on `_operands_equal`. Preserves
    `l1`'s element order so the dataflow is deterministic. An atom
    with a key is the only one with that key (`_atom`), so `in`
    compares keys; an atom without one equals nothing."""
    if not l1 or not l2:
        return ()
    return tuple(a for a in l1 if a.key is not None and a in l2)


def _state_equal(s1: _RegState, s2: _RegState) -> bool:
    return (
        s1.z_tracks_a == s2.z_tracks_a
        and _operand_list_equal(s1.a, s2.a)
        and _operand_list_equal(s1.x, s2.x)
        and _operand_list_equal(s1.y, s2.y)
        and _operand_list_equal(s1.z_reflects, s2.z_reflects)
        and len(s1.mem) == len(s2.mem)
        and all(any(_operand_list_equal(g1, g2) for g2 in s2.mem)
                for g1 in s1.mem)
    )


def _operand_list_equal(
    l1: tuple[_Atom, ...], l2: tuple[_Atom, ...],
) -> bool:
    """Order-insensitive equality on `_operands_equal`. Used by the
    dataflow fixed-point check — two states are equal iff their
    register equivalence lists contain the same operands (any
    order). An operand that equals nothing (an atom without a key)
    makes the lists unequal, even to themselves. Otherwise equality
    is identity of atoms, so a greedy matching of the two lists
    succeeds exactly when they are equal as multisets."""
    n = len(l1)
    if n != len(l2):
        return False
    if n == 0:
        return True
    if n == 1:
        a = l1[0]
        return a is l2[0] and a.key is not None
    for a in l1:
        if a.key is None:
            return False
    for a in l2:
        if a.key is None:
            return False
    if n == 2:
        a, b = l1
        c, d = l2
        return (a is c and b is d) or (a is d and b is c)
    return Counter(l1) == Counter(l2)


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
    if isinstance(instr.src, asm_ast.Reg):
        # Reg-to-reg transfer: the dropped TXA / TAX / TYA / TAY
        # would set Z to (src register's value == 0). z_reflects
        # already covers that when it intersects src's equivalence
        # class — Z is currently set to (some operand M's value
        # == 0), and src.mirrors contains M, so src equals M and
        # Z reflects (src == 0).
        src_cur = _get_reg(state, instr.src.reg)
        if _intersects(state.z_reflects, src_cur):
            return True
        return _flags_dead_at(instrs, i + 1)
    if _has(state.z_reflects, instr.src):
        return True
    return _flags_dead_at(instrs, i + 1)


def _reverses_transfer(
    instrs: list[asm_ast.Type_instruction], i: int,
) -> bool:
    """Whether `instrs[i]` moves a register back into the register it
    was just moved from (`TAX; TXA`, `TYA; TAY`, ...)."""
    if i == 0:
        return False
    prev, ins = instrs[i - 1], instrs[i]
    return (
        isinstance(prev, asm_ast.Mov) and isinstance(ins, asm_ast.Mov)
        and isinstance(prev.src, asm_ast.Reg)
        and isinstance(prev.dst, asm_ast.Reg)
        and prev.src != prev.dst
        and ins.src == prev.dst and ins.dst == prev.src
    )


def _repeats_transfer(
    instrs: list[asm_ast.Type_instruction], i: int,
) -> bool:
    """Whether `instrs[i]` is a transfer between registers that the
    instruction before a run of conditional branches ending at `i`
    already made, as in `TXA; BEQ two; TXA; BPL three`. A branch
    changes no register and no flag, and with no label in the run the
    second transfer is reached only from the first, so both registers
    and N and Z already hold what it would put there. A volatile
    transfer, which reads or writes a `register volatile` local, stays."""
    ins = instrs[i]
    if not (isinstance(ins, asm_ast.Mov) and not ins.is_volatile
            and isinstance(ins.src, asm_ast.Reg)
            and isinstance(ins.dst, asm_ast.Reg)
            and ins.src != ins.dst):
        return False
    k = i - 1
    while k >= 0 and isinstance(instrs[k], asm_ast.Branch):
        k -= 1
    return k < i - 1 and k >= 0 and instrs[k] == ins


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
    if not isinstance(instr.dst, asm_ast.Reg):
        return False
    if isinstance(instr.src, asm_ast.Reg):
        # Register-to-register transfer (TXA / TYA / TAX / TAY).
        # Droppable iff src's and dst's equivalence classes share
        # at least one operand — both registers currently hold the
        # same value, so the transfer wouldn't change dst. The
        # caller still has to check the N/Z flag effect separately
        # via `_flags_redundant_at`.
        src_cur = _get_reg(state, instr.src.reg)
        dst_cur = _get_reg(state, instr.dst.reg)
        return _intersects(src_cur, dst_cur)
    return _has(_get_reg(state, instr.dst.reg), instr.src)


def _store_const_via_index(
    instrs: list[asm_ast.Type_instruction], i: int, state: _RegState,
) -> tuple[list[asm_ast.Type_instruction], int] | None:
    """When `instrs[i:i+2]` is `LDA #k; STA M` (or `instrs[i]` the
    one atom `Mov(#k, M)`), A holds a value from
    memory (its list has a `Data` or `ZP` byte, say), nothing reads
    the `k` in A afterwards, and Y (or else X) is dead, return `LDY #k;
    STY M` (`LDX #k; STX M`), which leaves A alone, and how many
    instructions it replaces. Otherwise None.

    `get_alphabet` in examples/zork/zstring.c returns
    the alphabet it loaded after `SHIFT_ALPHABET = 0xFF`, which then
    needs no copy of it in a local: `LDY #$FF; STY SHIFT_ALPHABET`, as
    in Zork's code. The two forms are the same size and speed, so
    the rewrite costs nothing when no reload goes."""
    first = instrs[i]
    # Both forms start with a `Mov` of a constant, and both need A to
    # hold a byte of memory, so anything else returns None at once.
    if type(first) is not asm_ast.Mov or type(first.src) not in _IMMEDIATE:
        return None
    if not any(held.data_or_zp for held in state.a):
        return None
    a = asm_ast.Reg(reg=asm_ast.A())
    if (isinstance(first, asm_ast.Mov) and _immediates([first.src])
            and isinstance(first.dst, (asm_ast.Data, asm_ast.ZP))):
        # The one-atom form, `Mov(#k, M)`, which emits as the pair.
        load = asm_ast.Mov(src=first.src, dst=a, is_volatile=False)
        store = asm_ast.Mov(src=a, dst=first.dst,
                            is_volatile=first.is_volatile)
        after = i + 1
    elif i + 2 <= len(instrs):
        load, store = instrs[i], instrs[i + 1]
        after = i + 2
    else:
        return None
    if not (isinstance(load, asm_ast.Mov) and load.dst == a
            and not load.is_volatile and _immediates([load.src])):
        return None
    if not (isinstance(store, asm_ast.Mov) and store.src == a
            and isinstance(store.dst, (asm_ast.Data, asm_ast.ZP))):
        return None
    if not any(held.data_or_zp for held in state.a):
        return None
    # `_reloads_a` usually looks at one or two instructions, so it
    # goes before the liveness walk. Both are pure.
    if not _reloads_a(instrs, after, state.a, store.dst):
        return None
    if not _a_dead_at(instrs, after):
        return None
    for reg_cls, dead in ((asm_ast.Y, _y_dead_at), (asm_ast.X, _x_dead_at)):
        if dead(instrs, i):
            reg = asm_ast.Reg(reg=reg_cls())
            return [
                asm_ast.Mov(src=load.src, dst=reg, is_volatile=False),
                asm_ast.Mov(src=reg, dst=store.dst,
                            is_volatile=store.is_volatile),
            ], after - i
    return None


def _reloads_a(
    instrs: list[asm_ast.Type_instruction], start: int,
    held: list[asm_ast.Type_operand], stored: asm_ast.Type_operand,
) -> bool:
    """Whether the next instruction from `start` along the fall-through
    path that reads or writes A loads it from one of `held`, the
    operands A mirrors, unchanged since (and not `stored`). Branches and
    labels are passed over, so this is a guess at a reload the second
    phase can drop, not a proof; `_store_const_via_index` is sound
    either way."""
    written: list[asm_ast.Type_operand] = [stored]
    for ins in instrs[start:]:
        if isinstance(ins, (asm_ast.Label, asm_ast.Branch)):
            continue
        if isinstance(ins, asm_ast.Mov) and ins.dst == asm_ast.Reg(
            reg=asm_ast.A(),
        ):
            return (not ins.is_volatile
                    and _has(held, ins.src)
                    and not any(_may_alias(w, ins.src) for w in written))
        if isinstance(ins, (asm_ast.Inc, asm_ast.Dec)) and not isinstance(
            ins.dst, asm_ast.Reg,
        ):
            written.append(ins.dst)
            continue
        if isinstance(ins, asm_ast.Mov) and isinstance(
            ins.src, asm_ast.Reg,
        ) and not isinstance(ins.src.reg, asm_ast.A):
            written.append(ins.dst)
            continue
        return False
    return False


def _swap_pairs_ok(
    instrs: list[asm_ast.Type_instruction], i: int, state: _RegState,
) -> tuple[int, int] | None:
    """The numbers of stores `(n1, n2)` when `instrs[i:]` starts `LDA X;
    STA D1 (n1 of them); LDA M; STA D2 (n2 of them)` with A already
    mirroring M, and the two groups can trade places: `STA D2...; LDA
    X; STA D1...` does the same. None otherwise.

    No store of one group may change what the other group reads or
    writes (each `D1` against `M` and each `D2`, each `D2` against
    `X`), and A and N/Z, which end up holding X instead of M, must be
    dead after.

    That's the join of `floor_enemy_draw`'s dispatch: every case
    leaves the high byte of a pointer in A, and the join stores the
    low byte to a parameter first. Lode Runner's
    `enable_next_level_ladders` stores the row into a local and into
    `GAME_ROWNUM` and then copies the column, which A still holds,
    from its local into `GAME_COLNUM`."""
    if i >= len(instrs) or not _is_a_load(instrs[i]):
        return None
    n1 = _stores_of_a_after(instrs, i + 1)
    j = i + 1 + n1
    if n1 == 0 or j >= len(instrs) or not _is_a_load(instrs[j]):
        return None
    n2 = _stores_of_a_after(instrs, j + 1)
    if n2 == 0:
        return None
    load_x, load_m = instrs[i], instrs[j]
    d1s = [ins.dst for ins in instrs[i + 1:j]]
    d2s = [ins.dst for ins in instrs[j + 1:j + 1 + n2]]
    window = instrs[i:j + 1 + n2]
    if not _has(state.a, load_m.src):
        return None
    if _has(state.a, load_x.src):
        return None
    x, m = load_x.src, load_m.src
    for d1 in d1s:
        if _may_alias(d1, m):
            return None
        for d2 in d2s:
            if _may_alias(d1, d2) and not _same_index_apart(
                    d1, d2, window):
                return None
    for d2 in d2s:
        if _may_alias(d2, x):
            return None
    # Nor may a store of the first group write the pointer that a
    # store of the second group's address is read through, or a store
    # of the second group the one the first load's is: `LDA t,X; STA
    # p+1; LDA #7; STA (p),Y` with A holding 7 had stored through the
    # old `p`.
    for d1 in d1s:
        for d2 in d2s:
            if any(_may_alias(d1, b, zp_slot_symbols=_ZP_ADDRS)
                   for b in _pointer_bytes(d2)):
                return None
    for d2 in d2s:
        if any(_may_alias(d2, b, zp_slot_symbols=_ZP_ADDRS)
               for b in _pointer_bytes(x)):
            return None
    for d1 in d1s:
        # The first group's stores move after the second load, whose
        # address may be read through a pointer the stores write.
        if any(_may_alias(d1, b, zp_slot_symbols=_ZP_ADDRS)
               for b in _pointer_bytes(m)):
            return None
    end = j + 1 + n2
    if not (_a_dead_at(instrs, end) and _flags_dead_at(instrs, end)):
        return None
    return n1, n2


def _is_reg_a(op: asm_ast.Type_operand) -> bool:
    """Returns whether `op` is `Reg(A)`."""
    return type(op) is asm_ast.Reg and type(op.reg) is asm_ast.A


def _is_a_load(ins: asm_ast.Type_instruction) -> bool:
    """Returns whether `ins` is a non-volatile `LDA` of memory or a
    constant."""
    return (type(ins) is asm_ast.Mov and not ins.is_volatile
            and _is_reg_a(ins.dst) and type(ins.src) is not asm_ast.Reg)


def _stores_of_a_after(
    instrs: list[asm_ast.Type_instruction], k: int,
) -> int:
    """Counts the non-volatile stores of A into memory that come in a row
    from `instrs[k]`."""
    n = 0
    while k + n < len(instrs):
        ins = instrs[k + n]
        if not (type(ins) is asm_ast.Mov and not ins.is_volatile
                and _is_reg_a(ins.src) and type(ins.dst) is not asm_ast.Reg):
            break
        n += 1
    return n


def _swap_loads_ok(
    instrs: list[asm_ast.Type_instruction], i: int, state: _RegState,
) -> bool:
    """Whether `instrs[i:i+2]` is `LDX M1; LDA M2` (or `LDY M1`) with X
    (Y) already mirroring M2 and A not, so that `LDA M2; LDX M1` does
    the same and its `LDA M2` becomes `TXA` (`TYA`) in the second
    phase. Swapping a function's two register arguments, `g(b, a)` in
    `f(a, b)`, is `STA a; STX b; LDX a; LDA b` and becomes `STA a;
    STX b; TXA; LDX a`, and the store of `b` then goes when nothing
    reads it.

    A load writes only its register and N/Z, so after either order A
    holds M2 and X holds M1. The loads mustn't be volatile, since the
    order of volatile reads is the program's, and neither operand may
    be lowered with an `LDY #off` setup, which writes Y too. M2 may
    not be indexed by the register M1 goes into, which would read M2
    at another address once the load of M1 comes after it. N/Z,
    which end up set from M1 instead of M2, must be dead after."""
    if i + 2 > len(instrs):
        return False
    first, second = instrs[i:i + 2]
    # These register checks come first because they cost less than
    # `_sets_up_y`.
    if not (type(first) is asm_ast.Mov and type(second) is asm_ast.Mov
            and type(first.dst) is asm_ast.Reg
            and type(second.dst) is asm_ast.Reg
            and type(second.dst.reg) is asm_ast.A
            and type(first.dst.reg) in (asm_ast.X, asm_ast.Y)):
        return False
    for ins in (first, second):
        if (not isinstance(ins, asm_ast.Mov) or ins.is_volatile
                or isinstance(ins.src, asm_ast.Reg)
                or not isinstance(ins.dst, asm_ast.Reg)
                or _sets_up_y(ins)):
            return False
    if not isinstance(second.dst.reg, asm_ast.A):
        return False
    reg = first.dst.reg
    if isinstance(reg, asm_ast.X):
        mirror = state.x
    elif isinstance(reg, asm_ast.Y):
        mirror = state.y
    else:
        return False
    if (isinstance(second.src, asm_ast.IndexedData)
            and type(second.src.index) is type(reg)):
        return False
    if not _has(mirror, second.src):
        return False
    if _has(state.a, second.src):
        return False
    # `transfer_pm1_store` makes `LDA M1; SEC; SBC #k; STA M1; LDX M1`
    # a `DEX` of M1, as in Zork's `return_value`, so the load right
    # after `STA M1` stays.
    prev = instrs[i - 1] if i else None
    if (isinstance(prev, asm_ast.Mov)
            and isinstance(prev.src, asm_ast.Reg)
            and isinstance(prev.src.reg, asm_ast.A)
            and _operands_equal(prev.dst, first.src)):
        return False
    return _flags_dead_at(instrs, i + 2)


def _pointer_bytes(op: asm_ast.Type_operand) -> list[asm_ast.Type_operand]:
    """The bytes that hold the address `op` is reached through: a
    zero-page pointer's two bytes, DPTR's, FP's, or SSP's."""
    if isinstance(op, (asm_ast.IndirectZp, asm_ast.IndirectZpY)):
        return [asm_ast.ZP(address=op.address, offset=k) for k in (0, 1)]
    if isinstance(op, (asm_ast.Indirect, asm_ast.IndirectY)):
        name = "DPTR"
    elif isinstance(op, asm_ast.Frame):
        name = "FP"
    elif isinstance(op, asm_ast.Stack):
        name = "SSP"
    else:
        return []
    return [asm_ast.Data(name=name, offset=k) for k in (0, 1)]


def _same_index_apart(
    a: asm_ast.Type_operand, b: asm_ast.Type_operand,
    window: list[asm_ast.Type_instruction],
) -> bool:
    """Whether `a` and `b` are `name+k1,R` and `name+k2,R` with `k1`
    and `k2` different and less than 256 apart, and nothing in
    `window` writes R, so the two stores in it reach different bytes.
    The loads and stores `_swap_pairs_ok` looks at write no register
    but A, except the `LDY #off` of an operand lowered through Y.
    `hi, lo` stored to `LOCAL_ZVARS[x]` and `LOCAL_ZVARS[x + 1]` in
    Zork's `instr_call` are this."""
    if not (isinstance(a, asm_ast.IndexedData)
            and isinstance(b, asm_ast.IndexedData)
            and a.name == b.name and a.index == b.index
            and a.offset != b.offset and abs(a.offset - b.offset) < 256):
        return False
    if isinstance(a.index, asm_ast.Y):
        return not any(_sets_up_y(ins) for ins in window)
    return isinstance(a.index, asm_ast.X)


def _store_from_a(
    instr: asm_ast.Type_instruction, state: _RegState,
) -> asm_ast.Mov | None:
    """When `instr` is a store through A, `Mov(M, D)` with M memory or
    an immediate (emitted as `LDA M; STA D`), and A already mirrors M,
    return `Mov(Reg(A), D)`, the `STA D` alone. Otherwise None. The
    caller checks that the `LDA`'s N/Z effect isn't needed.

    That's the second store of `LDA #$FF; STA S; LDA #$FF; STA S+1`.
    A volatile `Mov` qualifies only with an immediate source, since
    the flag doesn't say which side is volatile."""
    if not isinstance(instr, asm_ast.Mov):
        return None
    if isinstance(instr.src, asm_ast.Reg) or isinstance(
        instr.dst, asm_ast.Reg,
    ):
        return None
    if _operands_equal(instr.src, instr.dst):
        return None
    if instr.is_volatile and not _immediates([instr.src]):
        return None
    if not _has(state.a, instr.src):
        return None
    return asm_ast.Mov(
        src=asm_ast.Reg(reg=asm_ast.A()), dst=instr.dst,
        is_volatile=instr.is_volatile,
    )


def _redirect_lda_to_transfer(
    instr: asm_ast.Type_instruction, state: _RegState,
) -> asm_ast.Mov | None:
    """When `instr` is `LDA M` (memory load into A) and X or Y
    already mirrors M, return a rewritten `Mov(Reg(X|Y), Reg(A))`
    — TXA or TYA. Otherwise return None.

    The original `LDA M` sets A := M and N/Z to bit7(M)/(M==0).
    The rewrite `TXA` sets A := X and N/Z to bit7(X)/(X==0). Under
    the X-mirrors-M invariant (X == M), both effects are
    identical, so no separate flag-soundness check is needed.

    Volatile loads are exempt — C99 §6.7.3.6 requires the read
    to actually happen against the memory cell. (`_is_redundant_
    load` also rejects volatile.)

    Saves 1-2 bytes / 2-3 cycles per occurrence and leaves A
    mirroring the same value, so downstream `STA M` becomes
    foldable to STX/STY via `via_a_store_fold` when A is dead.
    Reg-to-reg sources (TXA / TYA / TAX / TAY) skip: they're
    handled by `_is_redundant_load`'s transfer branch instead."""
    if not isinstance(instr, asm_ast.Mov):
        return None
    if instr.is_volatile:
        return None
    # dst must be Reg(A); src must be memory (not a register).
    if not (
        isinstance(instr.dst, asm_ast.Reg)
        and isinstance(instr.dst.reg, asm_ast.A)
    ):
        return None
    if isinstance(instr.src, asm_ast.Reg):
        return None
    # Already-mirrors-A is `_is_redundant_load`'s territory.
    if _has(state.a, instr.src):
        return None
    # Search X first, then Y. Both transfers are the same length;
    # X-first is arbitrary.
    for reg_cls, mirror in ((asm_ast.X, state.x), (asm_ast.Y, state.y)):
        if _has(mirror, instr.src):
            return asm_ast.Mov(
                src=asm_ast.Reg(reg=reg_cls()),
                dst=instr.dst,
                is_volatile=False,
            )
    return None


def _redirect_index_load_to_transfer(
    instr: asm_ast.Type_instruction, state: _RegState,
    prev: asm_ast.Type_instruction | None,
) -> asm_ast.Mov | None:
    """When `instr` is `LDX M` or `LDY M` (a non-volatile load from
    memory or an immediate) and A already mirrors M, returns `TAX` or
    `TAY`, which sets the register and N/Z to the same value, in one
    byte and two cycles. Otherwise returns None.

    A parameter arriving in A that a loop counts down in Y is `STA d;
    LDY d`, which becomes `STA d; TAY`, and the store goes once
    nothing reads `d` again.

    The caller rewrites a load of a byte the compiler allocated only
    when nothing reads M after it (`asm_dead_store.mem_dead_after`),
    so that the store of M goes too. A load of such a byte that's
    still read, such as a loop counter's `LDX i` at the top of a loop
    that tests `LDA i; CMP #n`, stays, since `loop_counter_to_x` and
    the other register promotions after the first peephole fixed
    point look for the counter's `LDX`. A load of a named object
    (`_named_object`), which those promotions leave alone, is
    rewritten either way: `A1 = A2 = C1; B1 = B2 = C2; pos(C1, C2)`
    passes C2 with `TAX` after the stores.

    The load is left alone right after `LDA M`, where
    `direct_index_load` makes `LDA M; TAX` the `LDX M` it was when A
    is dead after it, so the two passes don't undo each other.

    A volatile load qualifies only right after a store of A to M that
    isn't volatile. Every access of a volatile byte carries the flag,
    so M isn't volatile, and the flag comes from the register, which a
    `register volatile` local lives in (`wait`'s `y`). `TAY` writes
    that register once, as `LDY M` did, and keeps the flag."""
    if not isinstance(instr, asm_ast.Mov):
        return None
    if not (isinstance(instr.dst, asm_ast.Reg)
            and isinstance(instr.dst.reg, (asm_ast.X, asm_ast.Y))):
        return None
    if isinstance(instr.src, asm_ast.Reg) or _sets_up_y(instr):
        return None
    if instr.is_volatile and not (
        isinstance(prev, asm_ast.Mov) and not prev.is_volatile
        and isinstance(prev.src, asm_ast.Reg)
        and isinstance(prev.src.reg, asm_ast.A)
        and _operands_equal(prev.dst, instr.src)
    ):
        return None
    if not _has(state.a, instr.src):
        return None
    if (isinstance(prev, asm_ast.Mov)
            and isinstance(prev.dst, asm_ast.Reg)
            and isinstance(prev.dst.reg, asm_ast.A)
            and _operands_equal(prev.src, instr.src)):
        return None
    return asm_ast.Mov(
        src=asm_ast.Reg(reg=asm_ast.A()), dst=instr.dst,
        is_volatile=instr.is_volatile,
    )


def _named_object(op: asm_ast.Type_operand) -> bool:
    """True for a byte of an object the program names, a `Data`
    operand that isn't one of the compiler's zero-page slots
    (`__local_*`, `__zpabi_*`) or the runtime's bytes."""
    return (isinstance(op, asm_ast.Data)
            and not op.name.startswith(("__local_", "__zpabi_"))
            and op.name not in ("SSP", "FP", "HARGS", "DPTR"))


def _stores_held_value(
    instr: asm_ast.Type_instruction, state: _RegState,
) -> bool:
    """True iff `instr` is a non-volatile store of A, X, or Y into a
    `Data` / `ZP` byte the register is known to mirror, so the byte
    already holds the value: a store right after a load of the same
    byte, or one that a memory group made redundant."""
    if not (isinstance(instr, asm_ast.Mov) and not instr.is_volatile
            and isinstance(instr.src, asm_ast.Reg)
            and isinstance(instr.dst, (asm_ast.Data, asm_ast.ZP))):
        return False
    return _has(_get_reg(state, instr.src.reg), instr.dst)


_A, _X, _Y = asm_ast.A, asm_ast.X, asm_ast.Y
_MOV = asm_ast.Mov
_REG = asm_ast.Reg
_Y_INDEXED = frozenset((asm_ast.IndirectY, asm_ast.IndirectZpY))
_STABLE = frozenset(_STABLE_MEM_TYPES)
_GROUPABLE = frozenset((*_STABLE_MEM_TYPES, asm_ast.IndexedData))
_IMMEDIATE = frozenset((asm_ast.Imm, asm_ast.ImmLabelLow,
                        asm_ast.ImmLabelHigh))
_Y_SETUP = frozenset((asm_ast.Frame, asm_ast.Stack, asm_ast.Indirect,
                      asm_ast.IndirectZp))


class _Atom:
    """One operand as the state tracks it, with the facts about it
    that the updates read. `op` is the first operand met with this
    `key` (`_key`), and every operand with that key gets this atom
    (`_atom`), so two atoms with keys are equal under
    `_operands_equal` exactly when they are the same object. An
    operand without a key equals nothing, so its atom has `key` None
    and matches no atom, itself included.

    `dep` is the class of the register whose value the operand's
    address depends on. For `IndexedData(_, _, index=R)` it's R's
    class, since the address is `name + offset + R_value`, and for
    `IndirectY` / `IndirectZpY` it's Y, since they index with whatever
    Y holds. It's None for the rest."""
    __slots__ = ("op", "key", "dep", "stable", "groupable",
                 "immediate", "data_or_zp")

    def __init__(self, op: asm_ast.Type_operand, key) -> None:
        t = type(op)
        self.op = op
        self.key = key
        if t is asm_ast.IndexedData:
            self.dep = type(op.index)
        elif t in _Y_INDEXED:
            self.dep = _Y
        else:
            self.dep = None
        self.stable = t in _STABLE
        self.groupable = t in _GROUPABLE
        self.immediate = t in _IMMEDIATE
        self.data_or_zp = t is asm_ast.Data or t is asm_ast.ZP


# The atom of each key, and of each operand object by its `id`, for
# the run of `apply_redundant_load_elimination` in progress, which
# empties both when it returns. An entry of `_ATOM_OF` holds its
# operand, so no other object takes the operand's `id` while it
# exists, and no operand is changed in place during the run.
_INTERN: dict = {}
_ATOM_OF: dict[int, tuple] = {}


def _atom(op: asm_ast.Type_operand) -> _Atom:
    """Returns the atom that stands for `op` in a state."""
    e = _ATOM_OF.get(id(op))
    if e is not None and e[0] is op:
        return e[1]
    k = _key(op)
    if k is None:
        a = _Atom(op, None)
    else:
        a = _INTERN.get(k)
        if a is None:
            a = _INTERN[k] = _Atom(op, k)
    _ATOM_OF[id(op)] = (op, a)
    return a


def _drop_dependent_groups(
    mem: tuple[tuple[_Atom, ...], ...], rc: type,
) -> tuple[tuple[_Atom, ...], ...]:
    """Returns `mem` less the operands whose address depends on the
    register of class `rc`, and less the groups left with fewer than
    two. When nothing goes, it returns `mem` itself."""
    changed = False
    out = []
    for g in mem:
        g2 = _without_dependent(g, rc)
        if len(g2) >= 2:
            out.append(g2)
            if g2 is not g:
                changed = True
        else:
            changed = True
    return tuple(out) if changed else mem


def _without_dependent(
    atoms: tuple[_Atom, ...], rc: type,
) -> tuple[_Atom, ...]:
    """Returns `atoms` less those whose address depends on the
    register of class `rc` (`_Atom.dep`), or `atoms` itself when none
    does."""
    for a in atoms:
        if a.dep is rc:
            break
    else:
        return atoms
    return tuple(a for a in atoms if a.dep is not rc)


def _load_list(state: _RegState, src: _Atom) -> tuple[_Atom, ...]:
    """What a register mirrors after a load of `src`: the memory group
    that holds `src`, which leaves the groups, or `src` alone."""
    if src.key is not None:
        mem = state.mem
        for i, g in enumerate(mem):
            if src in g:
                state.mem = mem[:i] + mem[i + 1:]
                return g
    return (src,)


# What `_update_state` needs of each instruction object, found on its
# first update. Each entry is `(instr, z_effect, setup_writes_y,
# dst_setup_writes_y, src, dst, results)`, where `z_effect` is
# `_z_from_a(instr)`, `setup_writes_y` says whether any operand is
# lowered with an `LDY #off` setup, `dst_setup_writes_y` whether the
# destination of a `Mov` or `Pop` is, `src` and `dst` are the
# atoms of a `Mov`'s memory operands and of the memory operand a
# shift, `INC`, or `DEC` writes (None for a register or another
# instruction), and `results` maps each state the instruction has been
# applied to, as `_state_tuple` gives it, to the state it gave. The
# update is a function of the instruction and those six fields alone,
# and two tuples of atoms are equal only when they hold the same
# atoms, so a state found in `results` gives the same result again.
# The dataflow, the first phase, and the second phase apply most
# instructions to the same states. Each entry holds its instruction,
# so no other object takes its `id` while the entry exists, and
# `apply_redundant_load_elimination` empties the map when it returns.
# No instruction is changed in place during the pass, so an entry
# stays right for as long as it exists.
_FACTS: dict[int, tuple] = {}

_WRITES_DST = frozenset((
    asm_ast.ArithmeticShiftLeft, asm_ast.LogicalShiftRight,
    asm_ast.RotateLeft, asm_ast.RotateRight, asm_ast.Inc, asm_ast.Dec,
))


def _facts(instr: asm_ast.Type_instruction) -> tuple:
    """Returns the `_FACTS` entry of `instr`, making it on first use."""
    f = _FACTS.get(id(instr))
    if f is not None and f[0] is instr:
        return f
    t = type(instr)
    setup_y = any(type(op) in _Y_SETUP for op in _operands_of(instr))
    dst_y = (setup_y and t in (asm_ast.Mov, asm_ast.Pop)
             and type(instr.dst) in _Y_SETUP)
    src = dst = None
    if t is _MOV:
        if type(instr.src) is not _REG:
            src = _atom(instr.src)
        if type(instr.dst) is not _REG:
            dst = _atom(instr.dst)
    elif t in _WRITES_DST:
        # An `INC A` takes the memory path of `_core_step`, as it
        # always has.
        if not (type(instr.dst) is _REG
                and type(instr.dst.reg) in (_X, _Y)):
            dst = _atom(instr.dst)
    f = (instr, _z_from_a(instr), setup_y, dst_y, src, dst, {})
    _FACTS[id(instr)] = f
    return f


def _state_tuple(state: _RegState) -> tuple:
    """Returns the six fields of `state` as a tuple."""
    return (state.a, state.x, state.y, state.z_reflects,
            state.z_tracks_a, state.mem)


def _update_state(
    instr: asm_ast.Type_instruction, state: _RegState,
    branch_targets: set[str] | None = None,
) -> None:
    """Apply `instr`'s effect to the tracked register AND z_reflects
    state, including the side effects of its addressing-mode setup.

    `Frame` / `Stack` / `Indirect` / `IndirectZp` operands are
    emitted with an `LDY #off` in front of the access, which
    overwrites Y and sets N/Z from the offset. When the access is
    the instruction's last emitted opcode and doesn't itself set N/Z
    (a store, as in `LDY #off; STA (FP),Y`), the flags afterwards
    reflect the offset, so z_reflects is cleared."""
    f = _facts(instr)
    if branch_targets is None:
        before = _state_tuple(state)
        after = f[6].get(before)
        if after is not None:
            (state.a, state.x, state.y, state.z_reflects,
             state.z_tracks_a, state.mem) = after
            return
    if type(instr) is _MOV:
        _update_for_mov(instr, state, f[4], f[5])
    else:
        handler = _CORE.get(type(instr))
        if handler is not None:
            handler(instr, state, branch_targets, f)
    effect = f[1]
    if effect is not None:
        state.z_tracks_a = effect
    if f[2]:
        _set_reg(state, _Y, ())
        state.z_reflects = _without_dependent(state.z_reflects, _Y)
        if f[3]:
            state.z_reflects = ()
            state.z_tracks_a = False
    if branch_targets is None:
        f[6][before] = _state_tuple(state)


def _z_from_a(instr: asm_ast.Type_instruction) -> bool | None:
    """Whether `instr` leaves N/Z set from A's value (True), sets them
    from something else (False), or leaves them alone (None). The
    `LDY` of an addressing-mode setup is handled by the caller."""
    def is_a(op):
        return isinstance(op, asm_ast.Reg) and isinstance(op.reg, asm_ast.A)

    def is_reg(op):
        return isinstance(op, asm_ast.Reg)

    match instr:
        case asm_ast.Mov(src=s, dst=d):
            if is_a(d):
                return True              # LDA / TXA / TYA
            if is_reg(d):
                return is_a(s)           # TAX / TAY set Z from A; LDX not
            if not is_reg(s):
                return True              # mem-to-mem: LDA src; STA dst
            return None                  # STA / STX / STY
        case (asm_ast.Add(dst=d) | asm_ast.Sub(dst=d) | asm_ast.And(dst=d)
              | asm_ast.Or(dst=d) | asm_ast.Xor(dst=d)):
            return is_a(d)
        case (asm_ast.ArithmeticShiftLeft(dst=d)
              | asm_ast.LogicalShiftRight(dst=d)
              | asm_ast.RotateLeft(dst=d) | asm_ast.RotateRight(dst=d)):
            return is_a(d)
        case asm_ast.Pop(dst=d):
            return is_a(d)
        case asm_ast.Push() | asm_ast.SetCarry() | asm_ast.ClearCarry():
            return None
        case asm_ast.Branch() | asm_ast.Label():
            return None
    return False


def _operands_of(
    instr: asm_ast.Type_instruction,
) -> list[asm_ast.Type_operand]:
    """Every operand of `instr`, read or written."""
    match instr:
        case asm_ast.Mov(src=s, dst=d) | asm_ast.LoadAddress(src=s, dst=d):
            return [s, d]
        case asm_ast.Add(src=s, dst=d) | asm_ast.Sub(src=s, dst=d)                 | asm_ast.And(src=s, dst=d) | asm_ast.Or(src=s, dst=d):
            return [s, d]
        case asm_ast.Xor(src1=s1, src2=s2, dst=d):
            return [s1, s2, d]
        case asm_ast.Compare(left=l, right=r):
            return [l, r]
        case asm_ast.Inc(dst=d) | asm_ast.Dec(dst=d)                 | asm_ast.ArithmeticShiftLeft(dst=d)                 | asm_ast.LogicalShiftRight(dst=d)                 | asm_ast.RotateLeft(dst=d) | asm_ast.RotateRight(dst=d)                 | asm_ast.Pop(dst=d):
            return [d]
        case asm_ast.Push(src=s) | asm_ast.BitTest(src=s):
            return [s]
    return []


def _immediates(
    ops: list[asm_ast.Type_operand],
) -> list[asm_ast.Type_operand]:
    """The operands in `ops` that are constants, whose value no store
    can change."""
    return [op for op in ops if type(op) in _IMMEDIATE]


def _core_label(instr, state, branch_targets, f) -> None:
    # Reset at join points only — a fall-through-only label
    # preserves the prior block's register state.
    if branch_targets is None or instr.name in branch_targets:
        state.reset()


def _core_reset(instr, state, branch_targets, f) -> None:
    # Jump / Ret / Return end the block. FunctionPrologue,
    # AllocateStack, LoadAddress, and DecimalArith expand into
    # multi-instruction sequences in asm_to_asm2; their inner
    # effects exceed the tracker.
    state.reset()


def _core_none(instr, state, branch_targets, f) -> None:
    # Branch: fall-through preserves register state (Branch writes
    # neither registers nor flags). Push, SEC, and CLC leave both
    # alone too.
    return


def _core_call(instr, state, branch_targets, f) -> None:
    # JSR may clobber any register, memory, or flag. An X or Y
    # the callee preserves keeps its value, which still equals
    # an immediate it mirrored, but memory may have changed.
    x = (tuple(a for a in state.x if a.immediate)
         if _call_preserves(instr, "X") else ())
    y = (tuple(a for a in state.y if a.immediate)
         if _call_preserves(instr, "Y") else ())
    state.reset()
    state.x, state.y = x, y


def _core_pop(instr, state, branch_targets, f) -> None:
    # PLA/PLX/PLY: dest register loaded with unknown value;
    # N/Z set off that value.
    if type(instr.dst) is _REG:
        _set_reg(state, type(instr.dst.reg), ())
    state.z_reflects = ()


def _core_compare(instr, state, branch_targets, f) -> None:
    # CMP and BIT both set Z to a relation between A and the
    # operand, not to "operand's zeroness" — clear z_reflects.
    state.z_reflects = ()


def _core_alu(instr, state, branch_targets, f) -> None:
    # ADC/SBC/AND/ORA/EOR always write Reg(A) in our IR. A's
    # identity is lost; Z reflects A's new (untracked) value.
    if type(instr.dst) is _REG:
        _set_reg(state, type(instr.dst.reg), ())
    state.z_reflects = ()


def _core_shift(instr, state, branch_targets, f) -> None:
    if type(instr.dst) is _REG:
        _set_reg(state, type(instr.dst.reg), ())
        state.z_reflects = ()
        return
    # A shift of a memory byte sets N/Z from the byte's new
    # value, so Z reflects that byte and nothing it reflected
    # before. `LDA t; STA t2; LSR M; LDA t2; BEQ` must keep the
    # second load, whose Z the `BEQ` reads.
    dst = f[5]
    _invalidate_aliasing(state, dst)
    state.z_reflects = (dst,)


def _core_step(instr, state, branch_targets, f) -> None:
    dst = f[5]
    if dst is None:
        # INX/DEX/INY/DEY: drop the register's own equivalences
        # AND any other register's tracking of an
        # IndexedData(_, _, index=changed_reg) operand whose
        # address depended on the prior register value
        # (`_set_reg` drops both).
        _set_reg(state, type(instr.dst.reg), ())
        state.z_reflects = ()
        return
    # Inc/Dec on a memory operand: Z = (cell's new value == 0).
    _invalidate_aliasing(state, dst)
    state.z_reflects = (dst,)


_CORE = {
    asm_ast.Label: _core_label,
    asm_ast.Jump: _core_reset,
    asm_ast.Ret: _core_reset,
    asm_ast.Return: _core_reset,
    asm_ast.Branch: _core_none,
    asm_ast.Call: _core_call,
    asm_ast.FunctionPrologue: _core_reset,
    asm_ast.AllocateStack: _core_reset,
    asm_ast.LoadAddress: _core_reset,
    asm_ast.DecimalArith: _core_reset,
    asm_ast.Pop: _core_pop,
    asm_ast.Push: _core_none,
    asm_ast.SetCarry: _core_none,
    asm_ast.ClearCarry: _core_none,
    asm_ast.Compare: _core_compare,
    asm_ast.BitTest: _core_compare,
    asm_ast.Add: _core_alu,
    asm_ast.Sub: _core_alu,
    asm_ast.And: _core_alu,
    asm_ast.Or: _core_alu,
    asm_ast.Xor: _core_alu,
    asm_ast.ArithmeticShiftLeft: _core_shift,
    asm_ast.LogicalShiftRight: _core_shift,
    asm_ast.RotateLeft: _core_shift,
    asm_ast.RotateRight: _core_shift,
    asm_ast.Inc: _core_step,
    asm_ast.Dec: _core_step,
}


def _update_for_mov(
    mov: asm_ast.Mov, state: _RegState, s: _Atom | None, d: _Atom | None,
) -> None:
    """Apply a Mov's effect to state.a/x/y and z_reflects. Mov has
    four shapes: register-transfer, load, store, and mem-to-mem
    (emit lowers to `LDA src; STA dst`). `s` and `d` are the atoms of
    the source and destination, None for a register.
    """
    if mov.is_volatile:
        if d is None:
            # Volatile LDA M: clear state.<reg> so no future LDA M
            # can elide. z_reflects gets M because Z's current
            # value IS (M-at-read == 0) for the immediate downstream
            # Branch; a later LDA M wouldn't elide anyway (the
            # volatile gate rejects).
            dst_rc = type(mov.dst.reg)
            _set_reg(state, dst_rc, ())
            if s is not None:
                state.z_reflects = (s,)
            elif type(mov.src.reg) is _A and dst_rc is not _A:
                # A volatile `TAX` / `TAY` writes the register a
                # `register volatile` local lives in. A keeps its
                # value, and N/Z are set from it.
                state.z_reflects = state.a
            else:
                state.z_reflects = ()
            return
        if s is None:
            # Volatile STA M. Don't add M to the source register's
            # equivalence class — a future LDA M wouldn't be
            # elidable anyway, so the equivalence is unreachable.
            _invalidate_aliasing(state, d)
            _invalidate_z_aliasing(state, d)
            return
        # Volatile memory-to-memory Mov: emit lowers to
        # `LDA src; STA dst` (and drops a self-Mov, as below). The
        # LDA overwrites A, so A holds src's value and none of the
        # values it held before. The is_volatile bit often comes
        # from dst being volatile-typed even when src itself is a
        # stable non-volatile cell (e.g. `Copy(temp, volatile_y)`
        # in the sfx_tone case), so A === src is kept for a later
        # `LDA src`. A volatile `LDA src` doesn't elide anyway. The
        # same holds for dst: a non-volatile `LDA dst` exists only
        # when dst isn't the volatile side.
        if s is d and s.key is not None:
            return
        _invalidate_aliasing(state, d)
        _invalidate_z_aliasing(state, d)
        known = tuple(
            a for a in (s, d)
            if a.stable or type(a.op) is asm_ast.Imm
        )
        state.a = known
        state.z_reflects = known
        return
    if s is None and d is None:
        # TAX / TAY / TXA / TYA: dst now mirrors whatever src mirrors.
        # Z is set based on dst's new value (== src's value).
        # Conservative: if src's identity is tracked (state[src]
        # non-empty), z_reflects matches that list — both reflect
        # the same value. Otherwise clear.
        new_value = _get_reg(state, mov.src.reg)
        _set_reg(state, type(mov.dst.reg), new_value)
        # Transfers between registers DO set N/Z (except TXS, which
        # asm_ast doesn't model). For TAX/TAY/TXA/TYA, Z reflects
        # the transferred byte's zeroness — same as the source
        # register's identity.
        state.z_reflects = new_value
        return
    if d is None:
        # Load: the register now mirrors the source operand, and
        # the memory group that holds it (`_load_list`). Any prior
        # equivalences are wiped out (the previous value in the
        # register is gone). Z reflects (src == 0).
        #
        # When the load writes X or Y, `_set_reg` also drops any
        # tracking in the OTHER registers and the memory groups
        # that depends on the changed index register — e.g.
        # `IndexedData(_, _, index=X)` entries in state.a become
        # stale the moment X holds a different value. Without
        # this, a `LDA arr,X; LDX #N; LDA arr,X` chain would
        # incorrectly drop the second load on the assumption that
        # A still mirrors `arr,X`. Mirrors the INX/DEX/INY/DEY
        # invalidation in `_core_step`. The list the load gives
        # the register, and z_reflects, lose such operands too.
        rc = type(mov.dst.reg)
        _set_reg(state, rc, ())
        loaded = _load_list(state, s)
        if rc is _A:
            state.a = loaded
            state.z_reflects = (s,)
        else:
            loaded = _without_dependent(loaded, rc)
            if rc is _X:
                state.x = loaded
            elif rc is _Y:
                state.y = loaded
            state.z_reflects = (s,) if s.dep is not rc else ()
        return
    if s is None:
        # Store. STA/STX/STY don't touch N/Z. Add dst to the source
        # register's equivalence class so a later `LDA dst` can
        # elide. Add dst to z_reflects only when Z was already
        # tracking the source register's value — i.e. the existing
        # z_reflects entries intersect the existing register
        # equivalence class, OR both are empty (Z reflects A's
        # current unknown value, and dst now equals that value).
        #
        # A store that changes dst also ends Z's reflection of dst
        # and of every byte that may alias it. After `INC M; STA M`
        # with A not a copy of M, Z still holds the INC's test of the
        # old M + 1, so an `LDA M` before a `BEQ` must stay. A store
        # of a register that already mirrors dst writes the value dst
        # holds, and leaves Z's reflection of it alone.
        reg = mov.src.reg
        keyed = d.key is not None
        if not (keyed and d in _get_reg(state, reg)):
            _invalidate_z_aliasing(state, d)
        _invalidate_aliasing(state, d)
        if d.stable:
            cur = _get_reg(state, reg)
            cur_was_empty = not cur
            if d not in cur:
                cur = cur + (d,)
                _assign_reg(state, reg, cur)
            z = state.z_reflects
            # Whether Z reflects an operand of the class other than
            # dst. Every atom equal to dst is left out. An operand
            # equal to dst that Z reflects would mean that Z already
            # reflects dst, and then nothing is added either way.
            shared = any(
                c is not d and c.key is not None and c in z for c in cur
            )
            # Z reflects A's value either through an entry it shares
            # with A's class, or, when nothing is known about either,
            # because the last flag-setter set Z from A itself.
            if shared or (
                not z and cur_was_empty and state.z_tracks_a
            ):
                if d not in z:
                    state.z_reflects = z + (d,)
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
    if s is d and s.key is not None:
        return
    _invalidate_aliasing(state, d)
    _invalidate_z_aliasing(state, d)
    # The emitted `LDA src` sets N/Z from the moved value, so Z now
    # reflects dst (and nothing older). A constant source still
    # equals A too, whatever the store wrote.
    known = (d,) if d.stable else ()
    if s.immediate:
        known += (s,)
    state.a = known
    state.z_reflects = known


def _get_reg(state: _RegState, reg: asm_ast.Type_reg) -> tuple[_Atom, ...]:
    rc = type(reg)
    if rc is _A:
        return state.a
    if rc is _X:
        return state.x
    if rc is _Y:
        return state.y
    return ()


def _assign_reg(
    state: _RegState, reg: asm_ast.Type_reg, value: tuple[_Atom, ...],
) -> None:
    """Sets what `reg` mirrors, with none of `_set_reg`'s other
    updates."""
    rc = type(reg)
    if rc is _A:
        state.a = value
    elif rc is _X:
        state.x = value
    elif rc is _Y:
        state.y = value


def _set_reg(
    state: _RegState, rc: type, value: tuple[_Atom, ...],
) -> None:
    """Sets what the register of class `rc` mirrors. The memory
    operands of its old list still hold one value, so two or more of
    them become a memory group, less any whose address depends on the
    register itself."""
    if rc is _A:
        cur = state.a
    elif rc is _X:
        cur = state.x
    elif rc is _Y:
        cur = state.y
    else:
        return
    if len(cur) >= 2:
        old = tuple(a for a in cur if a.groupable and a.dep is not rc)
        if len(old) >= 2 and not any(
            _operand_list_equal(old, g) for g in state.mem
        ):
            state.mem = state.mem + (old,)
    if rc is _A:
        state.a = value
        return
    # An operand indexed by the register now names another byte, in
    # the memory groups, in the other registers' lists, and in the
    # value given, as `TAX` after `LDA t,X` gives.
    if state.mem:
        state.mem = _drop_dependent_groups(state.mem, rc)
    value = _without_dependent(value, rc)
    state.a = _without_dependent(state.a, rc)
    if rc is _X:
        state.y = _without_dependent(state.y, rc)
        state.x = value
    else:
        state.x = _without_dependent(state.x, rc)
        state.y = value


# `_may_alias(a.op, d.op)` for each pair of atoms `(a, d)` that
# `_without_aliasing` has asked about. Two operands with one key are
# the same in every field, so the atom's `op` gives the result its
# operands would. `_may_alias` reads only the operands' fields and the
# module state that `pointer_proof` sets for the whole pass, so an
# entry stays right until `apply_redundant_load_elimination` empties
# the map when it returns.
_ALIAS: dict[tuple[_Atom, _Atom], bool] = {}


def _without_aliasing(
    atoms: tuple[_Atom, ...], d: _Atom,
) -> tuple[_Atom, ...]:
    """Returns `atoms` less those that might alias `d`, or `atoms`
    itself when none might."""
    keep = []
    memo = _ALIAS
    for a in atoms:
        r = memo.get((a, d))
        if r is None:
            r = memo[(a, d)] = _may_alias(a.op, d.op)
        if not r:
            keep.append(a)
    return atoms if len(keep) == len(atoms) else tuple(keep)


def _invalidate_aliasing(state: _RegState, d: _Atom) -> None:
    """For each tracked register, filter out equivalence entries
    whose operand might alias `d`, the atom of a write's
    destination."""
    if state.a:
        state.a = _without_aliasing(state.a, d)
    if state.x:
        state.x = _without_aliasing(state.x, d)
    if state.y:
        state.y = _without_aliasing(state.y, d)
    if state.mem:
        changed = False
        out = []
        for g in state.mem:
            g2 = _without_aliasing(g, d)
            if len(g2) >= 2:
                out.append(g2)
                if g2 is not g:
                    changed = True
            else:
                changed = True
        if changed:
            state.mem = tuple(out)


def _invalidate_z_aliasing(state: _RegState, d: _Atom) -> None:
    """Drop z_reflects entries whose operand may alias `d`."""
    if state.z_reflects:
        state.z_reflects = _without_aliasing(state.z_reflects, d)


# The `_key` of each operand kind that `_operands_equal` compares,
# which is the kind and every field of it. An operand of a kind left
# out equals no operand, itself included.
_KEY_OF = {
    asm_ast.Imm: lambda o: (asm_ast.Imm, o.value),
    asm_ast.ZP: lambda o: (asm_ast.ZP, o.address, o.offset),
    asm_ast.Data: lambda o: (asm_ast.Data, o.name, o.offset),
    # `tab,X` and `tab,Y` name different bytes unless X == Y,
    # which the tracker doesn't know. Treating them as equal
    # dropped the `LDA tab,Y` in `j = tab[i]; ...; g = tab[j];`.
    asm_ast.IndexedData: lambda o: (
        asm_ast.IndexedData, o.name, o.offset, type(o.index)),
    asm_ast.Frame: lambda o: (asm_ast.Frame, o.offset),
    asm_ast.Stack: lambda o: (asm_ast.Stack, o.offset),
    asm_ast.Indirect: lambda o: (asm_ast.Indirect, o.offset),
}


def _key(op: asm_ast.Type_operand):
    """Returns a hashable value that two operands share exactly when
    `_operands_equal` holds between them, or None for an operand that
    equals nothing."""
    f = _KEY_OF.get(type(op))
    return None if f is None else f(op)


def _operands_equal(
    a: asm_ast.Type_operand, b: asm_ast.Type_operand,
) -> bool:
    """Structural equality on the operand kinds we track. Distinct
    types are never equal; for matching types, compare the
    relevant fields (`_KEY_OF`)."""
    k = _key(a)
    return k is not None and k == _key(b)


def _has(atoms: tuple[_Atom, ...], op: asm_ast.Type_operand) -> bool:
    """Returns whether some operand of `atoms` equals `op`
    (`_operands_equal`)."""
    a = _atom(op)
    return a.key is not None and a in atoms


def _intersects(l1: tuple[_Atom, ...], l2: tuple[_Atom, ...]) -> bool:
    """Returns whether some operand of `l1` equals some operand of
    `l2`."""
    if not l1 or not l2:
        return False
    for a in l1:
        if a.key is not None and a in l2:
            return True
    return False
