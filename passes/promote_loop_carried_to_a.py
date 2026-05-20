"""Promote a function-entry-saved reg-attributed parameter from a
ZP body-local to live in A across the loop body.

Motivating shape (snd_delay_up's outer loop, after the prior
peepholes settle):

    STA __local_..._pitch     ; save A (=pitch) to body local
    LDA sfx_click_ptr
    STA DPTR                   ; DPTR staging clobbers A but not Y
    LDA sfx_click_ptr+1
    STA DPTR+1
.loop@0_start:
    LDY __local_..._pitch      ; Y = pitch (for inner counter)
    INC __local_..._pitch      ; pitch++
    …                          ; inner DEY/BNE, then CMP (DPTR),Y, DEX/BNE
    BNE .loop@0_start

The body-local serves only as a "pitch register" — written at
entry, read at the top of each iteration, modified in place,
never observed by anything outside the function. With A unused
across the loop body (CMP reads A but its flags are dead, DEY/DEX
don't touch A), we can park pitch in Y during the DPTR staging
and keep it in A for the rest of the function:

    TAY                        ; save pitch in Y
    LDA sfx_click_ptr
    STA DPTR
    LDA sfx_click_ptr+1
    STA DPTR+1
    TYA                        ; restore pitch to A
.loop@0_start:
    TAY                        ; Y = pitch (init inner counter)
    CLC
    ADC #$01                   ; A = pitch + 1 (next iter's pitch)
    …
    BNE .loop@0_start          ; A carries pitch+1 around the edge

The local symbol is unused after the rewrite and is dropped from
the EQU table by downstream `prune_unused_locals`.

Eligibility (all must hold):

  * The function's FIRST non-label instruction is `Mov(Reg(A),
    Data(__local_<fn>__*, 0))` — the body-local save of an
    A-resident param.
  * Somewhere later, exactly one `LDY` from the same slot, paired
    with an immediately-following `Inc` on that slot.
  * The Label immediately before the `LDY` is the loop-start
    label (no intervening non-label instructions).
  * The slot has no other access sites in the function.
  * Y is unused between the entry STA and the loop-start label
    (preserves the TAY-parked value across the preamble).
  * No instruction between the in-loop INC site and the back-edge
    Branch writes A. A reads are allowed only via
    `Compare(Reg(A), _)` whose flags are clobbered before any
    subsequent Branch (the dead-flag CMP that the volatile-void-
    read rewrite produces).
  * The body contains a `Branch` whose target is the loop-start
    label — the back-edge that closes the carry path.

When all checks pass: entry STA → TAY, insert TYA just before the
loop-start label, in-loop LDY → TAY, in-loop INC → ClearCarry +
Add(Imm(1), Reg(A)).

Where to run: one-shot after the asm-peephole fixed-point loop,
alongside `apply_loop_counter_to_x`. The transformation is
structural (no iteration needed) and the rewritten IR re-enters
the second peephole sweep to mop up redundant Y / dead loads
the rewrite exposes (e.g., a now-redundant `LDY #$00` before
`CMP (DPTR),Y` when Y is already 0 from the inner DEY loop).
"""
from __future__ import annotations

import asm_ast


def apply_promote_loop_carried_to_a(
    prog: asm_ast.Program,
) -> asm_ast.Program:
    """Walk every Function and apply the loop-carried-A promotion
    when the structural pattern matches. Functions that don't
    match pass through unchanged."""
    new_top: list[asm_ast.Type_top_level] = []
    for tl in prog.top_level:
        if isinstance(tl, asm_ast.Function):
            new_top.append(_promote_function(tl))
        else:
            new_top.append(tl)
    return asm_ast.Program(top_level=new_top)


def _promote_function(fn: asm_ast.Function) -> asm_ast.Function:
    instrs = fn.instructions
    # Find the entry STA-to-local. Skip past Labels and any
    # zp_abi entry stubs (`STA __zpabi_<fn>__*` — these are
    # calling-convention saves emitted by tac_to_asm before the
    # body-local save we're looking for; the
    # `apply_dead_reg_entry_stub_drop` pass collects them later).
    entry_sta_idx = _find_entry_local_save_idx(instrs, fn.name)
    if entry_sta_idx is None:
        return fn
    entry_sta = instrs[entry_sta_idx]
    slot_name = entry_sta.dst.name
    # Find the LDY-from-slot + Inc-on-slot pair somewhere later.
    pair_idx = _find_ldy_inc_pair(instrs, entry_sta_idx + 1, slot_name)
    if pair_idx is None:
        return fn
    # The Label immediately preceding the pair must be the loop
    # start — no other instructions between the Label and the LDY
    # (just consecutive labels OK).
    loop_start_idx = _find_loop_start_label(
        instrs, pair_idx, lower_bound=entry_sta_idx,
    )
    if loop_start_idx is None:
        return fn
    loop_label = instrs[loop_start_idx].name
    # The slot must have no other access sites.
    for k, instr in enumerate(instrs):
        if k in (entry_sta_idx, pair_idx, pair_idx + 1):
            continue
        if _touches_slot(instr, slot_name):
            return fn
    # Y must be unused between the entry STA and the loop-start
    # label (the TAY/TYA save range).
    for k in range(entry_sta_idx + 1, loop_start_idx):
        if _touches_reg_y(instrs[k]):
            return fn
    # Find the back-edge Branch / Jump to the loop label.
    back_edge_idx = _find_back_edge(instrs, pair_idx + 2, loop_label)
    if back_edge_idx is None:
        return fn
    # A must be unwritten between the in-loop INC (which becomes
    # CLC+ADC, writing A as the "intended" update) and the back-
    # edge. A reads are allowed only via Compare(Reg(A), _) with
    # dead flags.
    if not _a_safe_across_carry(instrs, pair_idx + 2, back_edge_idx):
        return fn
    # All checks pass. Transform.
    return _rewrite(
        fn, entry_sta_idx, loop_start_idx, pair_idx,
    )


# ---------------------------------------------------------------------------
# Pattern matchers.
# ---------------------------------------------------------------------------


def _find_first_non_label(
    instrs: list[asm_ast.Type_instruction],
) -> int | None:
    for i, instr in enumerate(instrs):
        if not isinstance(instr, asm_ast.Label):
            return i
    return None


def _find_entry_local_save_idx(
    instrs: list[asm_ast.Type_instruction], fn_name: str,
) -> int | None:
    """Walk past Labels and zp_abi entry stubs to find the first
    body-local STA from Reg(A). Returns its index, or None if the
    function doesn't open with that shape.

    A zp_abi entry stub is `Mov(Reg(A|X|Y), Data(__zpabi_<fn>__*))`
    — emitted by tac_to_asm to copy the calling-convention's
    incoming register into the param's ZP slot. These are dead
    after our pin and get dropped downstream, but they're still
    present in the IR when we run (we sit before
    `apply_dead_reg_entry_stub_drop`)."""
    zpabi_prefix = f"__zpabi_{fn_name}__"
    local_prefix = f"__local_{fn_name}__"
    for i, instr in enumerate(instrs):
        if isinstance(instr, asm_ast.Label):
            continue
        if not isinstance(instr, asm_ast.Mov):
            return None
        if instr.is_volatile:
            return None
        # Source must be a register at function entry.
        if not isinstance(instr.src, asm_ast.Reg):
            return None
        if not isinstance(instr.dst, asm_ast.Data):
            return None
        if instr.dst.offset != 0:
            return None
        # zp_abi entry stub — skip.
        if instr.dst.name.startswith(zpabi_prefix):
            continue
        # Body-local save — must be from Reg(A) specifically.
        if (
            instr.dst.name.startswith(local_prefix)
            and isinstance(instr.src.reg, asm_ast.A)
        ):
            return i
        return None
    return None


def _is_entry_local_save(
    instr: asm_ast.Type_instruction, fn_name: str,
) -> bool:
    if not isinstance(instr, asm_ast.Mov):
        return False
    if instr.is_volatile:
        return False
    if not (
        isinstance(instr.src, asm_ast.Reg)
        and isinstance(instr.src.reg, asm_ast.A)
    ):
        return False
    if not isinstance(instr.dst, asm_ast.Data):
        return False
    if instr.dst.offset != 0:
        return False
    prefix = f"__local_{fn_name}__"
    return instr.dst.name.startswith(prefix)


def _find_ldy_inc_pair(
    instrs: list[asm_ast.Type_instruction],
    start: int, slot_name: str,
) -> int | None:
    for i in range(start, len(instrs) - 1):
        if (
            _is_ldy_from_slot(instrs[i], slot_name)
            and _is_inc_slot(instrs[i + 1], slot_name)
        ):
            return i
    return None


def _is_ldy_from_slot(
    instr: asm_ast.Type_instruction, slot_name: str,
) -> bool:
    """Match `Mov(Data(slot, 0), Reg(Y))` — the `LDY slot` that
    spills the body-local into the inner-loop counter. The Mov
    may be volatile-marked when Y is pinned to a volatile C-level
    local (e.g., `volatile uint8_t y __attribute__((reg("Y")))` in
    snd_delay_up) — that flag carries from the source-level
    volatile of the dst register, not from the slot itself.
    Rewriting to TAY preserves the same volatile-of-Y semantics
    (TAY still writes Y), so we don't reject on is_volatile."""
    if not isinstance(instr, asm_ast.Mov):
        return False
    if not (
        isinstance(instr.src, asm_ast.Data)
        and instr.src.name == slot_name
        and instr.src.offset == 0
    ):
        return False
    return (
        isinstance(instr.dst, asm_ast.Reg)
        and isinstance(instr.dst.reg, asm_ast.Y)
    )


def _is_inc_slot(
    instr: asm_ast.Type_instruction, slot_name: str,
) -> bool:
    if not isinstance(instr, asm_ast.Inc):
        return False
    return (
        isinstance(instr.dst, asm_ast.Data)
        and instr.dst.name == slot_name
        and instr.dst.offset == 0
    )


def _find_loop_start_label(
    instrs: list[asm_ast.Type_instruction],
    pair_idx: int,
    lower_bound: int,
) -> int | None:
    """Walk back from `pair_idx - 1` past consecutive Labels; the
    closest Label is the loop start. Anything other than Labels
    between the LDY and the Label disqualifies."""
    j = pair_idx - 1
    last_label = None
    while j > lower_bound:
        instr = instrs[j]
        if isinstance(instr, asm_ast.Label):
            last_label = j
            j -= 1
            continue
        # Non-label between LDY and Label.
        if last_label is None:
            return None
        return last_label
    return last_label


def _touches_slot(
    instr: asm_ast.Type_instruction, slot_name: str,
) -> bool:
    """True iff any operand of `instr` references the slot."""
    for op in _operands(instr):
        if (
            isinstance(op, asm_ast.Data)
            and op.name == slot_name
        ):
            return True
    return False


def _touches_reg_y(instr: asm_ast.Type_instruction) -> bool:
    """True iff `instr` reads or writes Reg(Y)."""
    def is_y(op):
        return (
            isinstance(op, asm_ast.Reg)
            and isinstance(op.reg, asm_ast.Y)
        )
    for op in _operands(instr):
        if is_y(op):
            return True
    return False


def _find_back_edge(
    instrs: list[asm_ast.Type_instruction],
    start: int, target_label: str,
) -> int | None:
    for i in range(start, len(instrs)):
        instr = instrs[i]
        if isinstance(instr, (asm_ast.Branch, asm_ast.Jump)):
            if instr.target == target_label:
                return i
    return None


def _a_safe_across_carry(
    instrs: list[asm_ast.Type_instruction],
    start: int, end: int,
) -> bool:
    """No instruction in `instrs[start:end]` writes Reg(A). A
    reads are allowed only if they're inside a `Compare(Reg(A),
    _)` whose flags are clobbered before any later Branch."""
    for k in range(start, end):
        instr = instrs[k]
        if _writes_reg_a(instr):
            return False
        if _reads_reg_a(instr):
            if not _is_compare_with_dead_flags(instrs, k, end):
                return False
    return True


def _is_compare_with_dead_flags(
    instrs: list[asm_ast.Type_instruction],
    idx: int, end: int,
) -> bool:
    """True iff `instrs[idx]` is `Compare(Reg(A), _)` and there's a
    flag-clobbering instruction before the next Branch (within
    `instrs[idx+1:end]` AND before `instrs[end]` if it's a
    branch)."""
    instr = instrs[idx]
    if not isinstance(instr, asm_ast.Compare):
        return False
    if not (
        isinstance(instr.left, asm_ast.Reg)
        and isinstance(instr.left.reg, asm_ast.A)
    ):
        return False
    # Walk forward until we hit either a flag-clobberer (good) or
    # a Branch (bad — flags read).
    j = idx + 1
    while j <= end:
        if j == len(instrs):
            return True
        nxt = instrs[j]
        if isinstance(nxt, asm_ast.Branch):
            return False
        if _overwrites_n_z(nxt):
            return True
        j += 1
    return True


# ---------------------------------------------------------------------------
# A-register liveness helpers (mirrors `dec_register_peephole`).
# ---------------------------------------------------------------------------


def _writes_reg_a(instr: asm_ast.Type_instruction) -> bool:
    def is_a(op):
        return (
            isinstance(op, asm_ast.Reg)
            and isinstance(op.reg, asm_ast.A)
        )
    match instr:
        case asm_ast.Mov(dst=d):
            if is_a(d):
                return True
        case asm_ast.Add(dst=d) | asm_ast.Sub(dst=d):
            if is_a(d):
                return True
        case asm_ast.And(dst=d) | asm_ast.Or(dst=d) | asm_ast.Xor(dst=d):
            if is_a(d):
                return True
        case asm_ast.Pop(dst=d):
            if is_a(d):
                return True
        case asm_ast.ArithmeticShiftLeft(dst=d) | asm_ast.LogicalShiftRight(dst=d):
            if is_a(d):
                return True
        case asm_ast.RotateLeft(dst=d) | asm_ast.RotateRight(dst=d):
            if is_a(d):
                return True
        case asm_ast.Call() | asm_ast.LoadAddress():
            # Both clobber A in their lowering.
            return True
    return False


def _reads_reg_a(instr: asm_ast.Type_instruction) -> bool:
    def is_a(op):
        return (
            isinstance(op, asm_ast.Reg)
            and isinstance(op.reg, asm_ast.A)
        )
    match instr:
        case asm_ast.Mov(src=s):
            if is_a(s):
                return True
        case asm_ast.Add(src=s, dst=d) | asm_ast.Sub(src=s, dst=d):
            if is_a(s) or is_a(d):
                return True
        case asm_ast.And(src1=a, src2=b, dst=d) | asm_ast.Or(src1=a, src2=b, dst=d) | asm_ast.Xor(src1=a, src2=b, dst=d):
            if is_a(a) or is_a(b) or is_a(d):
                return True
        case asm_ast.Compare(left=l, right=r):
            if is_a(l) or is_a(r):
                return True
        case asm_ast.Push(src=s):
            if is_a(s):
                return True
        case asm_ast.ArithmeticShiftLeft(dst=d) | asm_ast.LogicalShiftRight(dst=d):
            if is_a(d):
                return True
        case asm_ast.RotateLeft(dst=d) | asm_ast.RotateRight(dst=d):
            if is_a(d):
                return True
    return False


def _overwrites_n_z(instr: asm_ast.Type_instruction) -> bool:
    """True iff `instr` overwrites N/Z (so any prior N/Z is dead).
    Matches the simpler block-local check used by the dead-flag
    peepholes — Add/Sub/And/Or/Xor/Inc/Dec/Compare and Mov-into-
    register all set N/Z based on their result."""
    if isinstance(instr, (
        asm_ast.Add, asm_ast.Sub, asm_ast.And, asm_ast.Or, asm_ast.Xor,
        asm_ast.Inc, asm_ast.Dec, asm_ast.Compare,
        asm_ast.ArithmeticShiftLeft, asm_ast.LogicalShiftRight,
        asm_ast.RotateLeft, asm_ast.RotateRight,
        asm_ast.BitTest,
    )):
        return True
    if isinstance(instr, asm_ast.Mov):
        # Mov-into-register sets N/Z (LDA / LDX / LDY / TAX / TXA
        # / TAY / TYA all do); Mov-to-memory (STA / STX / STY)
        # leaves flags alone. Be conservative: only register
        # destinations count.
        return isinstance(instr.dst, asm_ast.Reg)
    if isinstance(instr, asm_ast.Pop):
        return isinstance(instr.dst, asm_ast.Reg)
    return False


def _operands(instr: asm_ast.Type_instruction):
    """Yield every operand of `instr`. Used for slot / register
    touch detection."""
    for fld in instr.__dataclass_fields__:
        v = getattr(instr, fld, None)
        if isinstance(v, list):
            for x in v:
                if hasattr(x, "__dataclass_fields__"):
                    # PhiArg or similar — recurse one level.
                    for fld2 in x.__dataclass_fields__:
                        yield getattr(x, fld2, None)
                else:
                    yield x
        else:
            yield v


# ---------------------------------------------------------------------------
# Transformation.
# ---------------------------------------------------------------------------


def _rewrite(
    fn: asm_ast.Function,
    entry_sta_idx: int,
    loop_start_idx: int,
    pair_idx: int,
) -> asm_ast.Function:
    instrs = fn.instructions
    reg_a = asm_ast.Reg(reg=asm_ast.A())
    reg_y = asm_ast.Reg(reg=asm_ast.Y())
    # Preserve the volatile bit from the in-loop LDY M when
    # building the replacement TAY (the LDY was volatile because
    # the dst Y is volatile-pinned at the C level; TAY writes
    # the same Y and inherits the same observable semantics).
    pair_ldy = instrs[pair_idx]
    ldy_volatile = isinstance(pair_ldy, asm_ast.Mov) and pair_ldy.is_volatile
    out: list[asm_ast.Type_instruction] = []
    for k, instr in enumerate(instrs):
        if k == entry_sta_idx:
            # STA __local_..._x → TAY
            out.append(asm_ast.Mov(src=reg_a, dst=reg_y))
        elif k == loop_start_idx:
            # Insert TYA just before the loop-start label.
            out.append(asm_ast.Mov(src=reg_y, dst=reg_a))
            out.append(instr)
        elif k == pair_idx:
            # LDY __local_..._x → TAY (preserving volatile if set)
            out.append(asm_ast.Mov(
                src=reg_a, dst=reg_y, is_volatile=ldy_volatile,
            ))
        elif k == pair_idx + 1:
            # INC __local_..._x → CLC; ADC #$01
            out.append(asm_ast.ClearCarry())
            out.append(asm_ast.Add(src=asm_ast.Imm(value=1), dst=reg_a))
        else:
            out.append(instr)
    return asm_ast.Function(
        name=fn.name, is_global=fn.is_global,
        params=list(fn.params), instructions=out,
    )
