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

# Flag soundness — and the Z-reflects tracker

The 6502 has a tiny one-bit lightbulb called the Z flag. It turns
ON whenever the CPU just produced a zero result, and OFF
otherwise. Most data-affecting instructions touch Z: every LDA
turns Z ON iff the loaded byte was zero; every ADC / SBC / AND /
ORA / EOR turns Z ON iff the new value of A is zero; INC / DEC
turn Z ON iff the bumped memory cell is now zero.

The downstream consumer of Z is a `Branch(EQ, ...)` (BEQ) or
`Branch(NE, ...)` (BNE) — these jump iff Z is the corresponding
state. So if we drop an `LDA M`, we skip the act of *re-setting*
the lightbulb to "is M zero?". If a BEQ / BNE follows, the
branch will read whatever state the lightbulb was left in by
the instruction BEFORE the LDA. That might be the wrong state.

There are two clean ways to know dropping the LDA is safe for
the branch:

  1. **The flag is dead.** No reachable instruction reads Z
     before another instruction overwrites it. Then it doesn't
     matter what Z is — we can drop the LDA without changing
     observable behavior. This is the `_flags_dead_at` check —
     it scans forward and answers "is Z dead at position N+1".

  2. **The flag is already what the LDA would set it to.** Maybe
     an earlier instruction already set Z to "is M zero?", and
     no intervening instruction has touched Z since. Then the
     LDA's flag effect is *redundant* — it would set Z to the
     same state it already has. Dropping is safe; the branch
     sees the same Z either way.

The first check is the simple one; the second is what the
`z_reflects` tracker does. Each entry in `z_reflects` is an
operand whose current value's zeroness equals the current state
of Z. After `LDA M`, `z_reflects = [M]` — Z reflects M's
zeroness. After `STA N` (M's current value into N), N now has
the same value as M did when the LDA ran, AND Z still reflects
that value, so `z_reflects = [M, N]`. After `SBC #1`, Z reflects
A's new value — but A's identity is no longer tracked (state.a
is cleared too), so `z_reflects = []`. After `STA M`, A's
current (unknown) value is in M, and Z reflects A's current
value, so `z_reflects = [M]` again.

When `_is_redundant_load` sees `LDA M`, it checks BOTH:
  * `state.<reg>` contains M (the register already holds M's
    value — no need to load again).
  * `z_reflects` contains M OR the flag is dead (the lightbulb
    is already in the state the LDA would set, OR nobody cares).

If both hold, drop the LDA.

# The example the new check fixes

For `volatile uint8_t y = pitch; while (--y != 0) {}`, the inner
loop in asm IR looks like:

    LDA b1   (volatile read of y)         ; A = y, Z = (y == 0)
    SEC
    SBC #1                                ; A = y - 1, Z = (A == 0)
    STA b0                                ; b0 = A
    Mov(b0, b1) (volatile)                ; emit: LDA b0; STA b1
    LDA b0                                ; <-- redundant
    Branch(EQ, .break)

After `STA b0`: A === b0, Z still reflects (post-SBC == 0) which
equals (b0 == 0). So `state.a = [b0]`, `z_reflects = [b0]`.

The volatile mem-to-mem `Mov(b0, b1)` emits `LDA b0; STA b1`.
The LDA b0 in emit doesn't change A's value (A === b0 already),
and sets Z to (b0 == 0) — which is what Z was. So
`state.a = [b0]`, `z_reflects = [b0]` survive the Mov.

At the candidate `LDA b0`: state.a contains b0 (value redundant)
AND z_reflects contains b0 (flag redundant). Drop the LDA.

Without the `z_reflects` extension, the conservative
`_flags_dead_at` would refuse the drop here (the Branch reads
Z), and the LDA would survive — bloating the inner loop by a
3-cycle no-op every iteration.

# Why z_reflects is a LIST, not a single operand

After `LDA M; STA N; STA P`, A's value === M === N === P. Z
still reflects "the value of A". So z_reflects tracks every
operand currently equivalent for the purpose of the Z flag.
Either of `LDA M`, `LDA N`, `LDA P` is redundant given the
current state — the list captures all three.

This is the same shape `state.a` already uses for tracking
value-equivalence. `z_reflects` follows the same update rules:
add to the list when a STA copies A's value to a new cell;
clear when an instruction overwrites A's value (SBC / ADC etc.)
or directly overwrites Z (CMP / BIT / etc.).

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
    state.

    The Z flag is touched by a wide variety of instructions; each
    branch below handles its own update. The rough categorization:

      - Instructions that DON'T touch Z (SetCarry / ClearCarry /
        Push / Compare-doesn't-fit-our-model / pure stores / etc.):
        z_reflects unchanged.
      - Instructions that set Z based on a specific operand's
        zeroness (LDA M, INC M, DEC M, shift-of-M):
        z_reflects = [M].
      - Instructions that set Z based on A's new value, where A's
        identity isn't tracked anymore (SBC / ADC / AND / OR / XOR
        / shift-of-A / PLA): z_reflects = []. (After the
        instruction state.a is also empty, so the lists stay in
        sync — which is what the STA-into-A's-equivalence-class
        machinery in `_update_for_mov` relies on.)
      - Block-boundary instructions (Label-as-target, Jump, Ret,
        Call, FunctionPrologue / AllocateStack / LoadAddress):
        full reset (state.reset() clears z_reflects too).
    """
    if isinstance(instr, asm_ast.Mov):
        _update_for_mov(instr, state)
        return
    if isinstance(instr, asm_ast.Label):
        # A label that something else can branch / jump to is a
        # join point — incoming control might bring an unrelated
        # register state, so we must reset. A label that ONLY the
        # fall-through reaches (no jump / branch targets it) leaves
        # the prior register state intact at this point.
        if branch_targets is None or instr.name in branch_targets:
            state.reset()
        return
    if isinstance(instr, (asm_ast.Jump,
                          asm_ast.Ret, asm_ast.Return)):
        # No fall-through. The next instruction starts a new block
        # reached only via its Label-as-target.
        state.reset()
        return
    if isinstance(instr, asm_ast.Branch):
        # Conditional branch: the fall-through preserves register
        # state (the condition only affects PC, not registers). DON'T
        # reset. The next instruction's state == this instruction's
        # exit state, which == this instruction's entry state since
        # Branch doesn't write registers OR flags. If the next
        # instruction is a Label that's also a Jump/Branch target,
        # the Label update will reset.
        return
    if isinstance(instr, asm_ast.Call):
        # JSR may clobber any register, any memory, and the flags;
        # conservatively invalidate everything.
        state.reset()
        return
    if isinstance(instr, (asm_ast.FunctionPrologue,
                          asm_ast.AllocateStack,
                          asm_ast.LoadAddress)):
        # These compound nodes expand into multi-instruction sequences
        # in asm_to_asm2; their inner effect on A / X / Y / flags is
        # more than a simple tracker can model.
        state.reset()
        return
    if isinstance(instr, asm_ast.Pop):
        # PLA / PLX / PLY pulls a fresh value off the stack. Sets
        # the destination register, sets N/Z based on the pulled
        # value. We don't track stack-pointer-relative values, so
        # the pulled value's identity is unknown.
        if isinstance(instr.dst, asm_ast.Reg):
            _set_reg(state, instr.dst.reg, [])
        # Z reflects the pulled value, whose identity is unknown.
        state.z_reflects = []
        return
    if isinstance(instr, asm_ast.Push):
        # PHA / PHP doesn't change registers OR flags — only the
        # stack pointer and stack memory. We don't track either.
        return
    if isinstance(instr, asm_ast.SetCarry):
        # SEC: sets C only. N/Z untouched.
        return
    if isinstance(instr, asm_ast.ClearCarry):
        # CLC: clears C only. N/Z untouched.
        return
    if isinstance(instr, asm_ast.Compare):
        # CMP: sets N/Z/C based on A - operand. Z is set to "A
        # equals operand", which isn't an "operand's zeroness"
        # relation — clear z_reflects.
        state.z_reflects = []
        return
    if isinstance(instr, asm_ast.BitTest):
        # BIT M: sets N to bit 7 of M, V to bit 6, and Z to
        # (A & M) == 0. The Z meaning here ("A and M have no
        # bits in common") doesn't fit our "operand's zeroness"
        # model — clear conservatively.
        state.z_reflects = []
        return
    if isinstance(instr, (asm_ast.Add, asm_ast.Sub,
                          asm_ast.And, asm_ast.Or)):
        # ADC / SBC / AND / ORA: in c6502's IR these always have
        # `dst=Reg(A)`. The result in A is no longer a copy of any
        # tracked operand. Z reflects A's new value, but A's
        # identity is unknown, so z_reflects collapses to [] —
        # parallel with state.a. A subsequent STA M can then
        # repopulate both lists by adding M to each.
        if isinstance(instr.dst, asm_ast.Reg):
            _set_reg(state, instr.dst.reg, [])
        state.z_reflects = []
        return
    if isinstance(instr, asm_ast.Xor):
        # EOR: same Z behavior as ADC/SBC/etc.
        if isinstance(instr.dst, asm_ast.Reg):
            _set_reg(state, instr.dst.reg, [])
        state.z_reflects = []
        return
    if isinstance(instr, (asm_ast.ArithmeticShiftLeft,
                          asm_ast.LogicalShiftRight,
                          asm_ast.RotateLeft, asm_ast.RotateRight)):
        if isinstance(instr.dst, asm_ast.Reg):
            # ASL A / LSR A / ROL A / ROR A: A's value changes,
            # Z = (A's new value == 0). Both lists empty.
            _set_reg(state, instr.dst.reg, [])
            state.z_reflects = []
            return
        # Shift/rotate on a memory operand (zp / abs). This is a
        # read-modify-write — the cell's value changes, so any
        # tracking that mirrors this cell must be invalidated.
        # Z = (cell's new value == 0).
        _invalidate_aliasing(state, instr.dst)
        # z_reflects now only contains operands that ARE the
        # shifted cell (now reflects its new value). Other
        # entries had values that haven't changed.
        state.z_reflects = [
            op for op in state.z_reflects if not _may_alias(op, instr.dst)
        ]
        state.z_reflects.append(instr.dst)
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
            # INX/DEX/INY/DEY sets Z to (X/Y's new value == 0).
            # We don't track X/Y values structurally as operands,
            # so the new Z meaning isn't representable here —
            # clear conservatively. The other z_reflects entries
            # (representing memory cells) survive only if their
            # addresses don't depend on the changed register.
            state.z_reflects = [
                op for op in state.z_reflects
                if not _depends_on_reg(op, changed_reg)
            ]
            # And drop everything anyway — the old z_reflects
            # entries represented prior Z state, but Z has been
            # overwritten by the INX/DEX result.
            state.z_reflects = []
            return
        # Inc/Dec on a memory operand (Data, ZP). Memory write to
        # instr.dst. Invalidate any register tracking that may alias.
        # Z = (cell's new value == 0). z_reflects becomes [dst].
        _invalidate_aliasing(state, instr.dst)
        state.z_reflects = [instr.dst]
        return
    # Comments, blank lines, and any future no-op nodes — neither
    # registers nor flags change.
    return


def _update_for_mov(mov: asm_ast.Mov, state: _RegState) -> None:
    """Distinguish the four Mov shapes:
      Mov(Reg, Reg)        — register transfer.
      Mov(non-Reg, Reg)    — load (LDA / LDX / LDY).
      Mov(Reg, non-Reg)    — store (STA / STX / STY).
      Mov(non-Reg, non-Reg) — memory-to-memory; emit lowers to
                              `LDA src; STA dst`.

    Each path updates BOTH the value tracker (`state.a/x/y`) AND
    the flag tracker (`state.z_reflects`) per the corresponding
    instruction's effect on N/Z.
    """
    # A volatile Mov reads or writes memory whose contents can
    # change outside the function's control. The redundant_load
    # pass refuses to elide volatile Movs (`_is_redundant_load`
    # gates on `is_volatile`), so the question here is what the
    # tracker should record for what comes AFTER. The volatile
    # Mov itself executes — its emit-time effects on A / flags
    # are real — and the subsequent instruction sees that state.
    if mov.is_volatile:
        if isinstance(mov.dst, asm_ast.Reg):
            # Volatile LDA M (or LDX/LDY): the load happened, A
            # now holds M's value AT THAT READ, and Z reflects
            # the same. But the next time we'd want to LDA M
            # we MUSTN'T elide (M might have changed). So clear
            # state.<reg> — no future LDA can rely on A still
            # mirroring M. z_reflects is more subtle: Z's
            # current value IS (M's value-at-read == 0). For
            # the IMMEDIATE downstream Branch, that's correct.
            # And no subsequent LDA M would be elidable (volatile
            # check rejects), so including M in z_reflects can't
            # cause an incorrect drop — keep it.
            _set_reg(state, mov.dst.reg, [])
            if not isinstance(mov.src, asm_ast.Reg):
                state.z_reflects = [mov.src]
            else:
                state.z_reflects = []
            return
        if isinstance(mov.src, asm_ast.Reg):
            # Volatile STA M (Reg, Mem). Memory at M is rewritten
            # (and observable). A unchanged, Z unchanged.
            _invalidate_aliasing(state, mov.dst)
            # state.<src.reg>'s entries that aliased M were just
            # invalidated; the source register's other entries
            # (and the prior z_reflects) survive. Don't ADD M to
            # the source's list — M is volatile, so a subsequent
            # LDA M wouldn't be elidable anyway, and adding
            # would suggest equivalence we can't rely on for
            # future reads.
            state.z_reflects = [
                op for op in state.z_reflects
                if not _may_alias(op, mov.dst)
            ]
            return
        # Volatile memory-to-memory Mov: emit lowers to
        # `LDA src; STA dst`. The LDA src reads src into A AND
        # sets Z to (src == 0). The STA dst writes A to dst (a
        # volatile write — observable, can't be elided).
        #
        # Post-Mov: A holds src's just-read value. Z reflects
        # src's value. The dst's value equals src's value AT
        # THIS WRITE, but dst is treated as volatile (the
        # is_volatile bit on the Mov often comes from dst being
        # volatile-typed), so we don't trust dst's value for
        # future reads.
        #
        # For state.a: src may carry the dst-volatile-derived
        # is_volatile=True even when src ITSELF is a non-volatile
        # stable cell (e.g., `Copy(non_volatile_temp,
        # volatile_y)` in our motivating sfx_tone case). In that
        # case, A === src is a valid equivalence: src is stable,
        # so A still mirrors src after this Mov. We add src to
        # state.a UNLESS src is a register-indexed operand
        # (IndexedData) whose value depends on an index register
        # (the safety bar set by the non-volatile path below).
        _invalidate_aliasing(state, mov.dst)
        if isinstance(mov.src, (
            asm_ast.ZP, asm_ast.Data, asm_ast.Stack,
            asm_ast.Frame, asm_ast.Indirect,
        )):
            if not any(
                _operands_equal(c, mov.src) for c in state.a
            ):
                state.a.append(mov.src)
            state.z_reflects = [
                op for op in state.z_reflects
                if not _may_alias(op, mov.dst)
            ]
            if not any(
                _operands_equal(z, mov.src) for z in state.z_reflects
            ):
                state.z_reflects.append(mov.src)
        else:
            # src is volatile-typed-and-its-value-can-change
            # (IndirectY, IndexedData with a runtime index, etc.).
            # Z reflects the just-read value but we can't name a
            # stable operand for it. Clear z_reflects for any
            # entry that may alias the dst write; the rest stay.
            state.z_reflects = [
                op for op in state.z_reflects
                if not _may_alias(op, mov.dst)
            ]
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
        # Store: register unchanged; memory at dst is rewritten.
        # Invalidate any tracked register equivalence that aliases
        # dst. STA / STX / STY do NOT modify N/Z (the only register
        # values they touch are written to memory; flags are
        # untouched).
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
            # z_reflects: the store doesn't change Z, but it
            # makes dst's value EQUAL to the source register's
            # current value. If Z was previously set by an
            # instruction whose result was (the source register's
            # value's zeroness) — captured by `state.z_reflects`
            # being parallel with `state.<src.reg>` — then Z
            # also reflects (dst's new value's zeroness). Add
            # dst to z_reflects IFF z_reflects already contained
            # an operand from the source register's equivalence
            # class (or z_reflects is empty AND state.<src.reg>
            # is empty, meaning Z reflects A's-current-value-but-
            # we-don't-know-which-cell — STA M then makes Z
            # reflect M too).
            #
            # Easier rule: if z_reflects ∩ state.<src.reg> is
            # non-empty, OR (z_reflects is empty AND state.
            # <src.reg> is empty), add dst to z_reflects. This
            # covers both the after-LDA case (lists agree) and
            # the after-SBC-then-STA case (both empty before STA,
            # dst becomes the first entry).
            cur_after = _get_reg(state, mov.src.reg)
            cur_for_check = [c for c in cur_after if c is not mov.dst]
            shared = any(
                _operands_equal(z, c)
                for z in state.z_reflects
                for c in cur_for_check
            )
            if shared or (not state.z_reflects and not cur_for_check):
                if not any(
                    _operands_equal(z, mov.dst)
                    for z in state.z_reflects
                ):
                    state.z_reflects.append(mov.dst)
        return
    # Memory-to-memory Mov: c6502 DOES emit these (e.g.
    # `Mov(IndexedData, Data)`, `Mov(Data, Data)`), and asm_emit
    # lowers them to `LDA src; STA dst` — using A as the staging
    # register. So post-Mov, A's value equals BOTH src and dst,
    # and Z reflects (src == 0) (set by the emit-time LDA src).
    #
    # Invalidate any prior trackings that aliased the dst write,
    # then ADD A === dst (when dst is a stable-address memory
    # operand). We don't add A === src: the src may carry an
    # index register that future code could change, invalidating
    # the equivalence — and the dst-side tracking is sufficient
    # for catching the common `LDA M; STA N; LDA N` shape.
    _invalidate_aliasing(state, mov.dst)
    if isinstance(mov.dst, (
        asm_ast.ZP, asm_ast.Data, asm_ast.Stack,
        asm_ast.Frame, asm_ast.Indirect,
    )):
        state.a = [mov.dst]
    # z_reflects after a mem-to-mem emit: the LDA src set Z to
    # (src == 0); the STA dst doesn't touch Z. So Z reflects src.
    # And dst now equals src in value, so dst also reflects.
    # Filter out any prior entries that may alias the dst (their
    # cell's value may have just changed), then include dst.
    # We DON'T include src for the same index-register-stability
    # reason `state.a` excludes it.
    state.z_reflects = [
        op for op in state.z_reflects if not _may_alias(op, mov.dst)
    ]
    if isinstance(mov.dst, (
        asm_ast.ZP, asm_ast.Data, asm_ast.Stack,
        asm_ast.Frame, asm_ast.Indirect,
    )):
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


