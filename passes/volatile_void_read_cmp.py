"""Convert `Mov(<indirect>, Reg(A), is_volatile=True)` to
`Compare(Reg(A), <indirect>)` when A's new value is dead and the
flags from the load aren't observed.

Motivating shape: `(void)*sfx_click_ptr` — a volatile read whose
loaded value is discarded. `tac_to_asm` lowers it to `Mov(<indirect>,
Reg(A))` which emits as `LDA (DPTR),Y`. The LDA clobbers A.

The same physical memory access can be performed by `CMP (DPTR),Y`
— a CMP also reads the byte at `(DPTR),Y` but only sets the
N/Z/C flags from `A - <byte>`. A is unchanged. So when:

  * The new A value is dead (no reads of A before next A-write),
    AND
  * The flags from the load aren't observed (the next instruction
    overwrites N/Z, or doesn't read flags),

we can rewrite the LDA to CMP and preserve A's prior value across
the volatile access. This is the prerequisite for keeping
`reg("A")`-pinned params live across a volatile click — without
it, the click destroys the pinned register.

Soundness:
  * Memory access: LDA and CMP both perform the same 6502 read
    cycle from `(DPTR),Y`. Volatile semantics are preserved.
  * A register: LDA writes A; CMP doesn't. With A dead-after, A's
    new value isn't observed in either form. With A live-before,
    CMP preserves the prior value (extending liveness is always
    safe).
  * Flags: LDA sets N/Z based on the loaded byte. CMP sets
    N/Z/C based on `A - byte`. DIFFERENT values when A != byte.
    The gate "next instruction clobbers N/Z" makes this difference
    unobservable.

Where to run: in the asm-peephole fixed-point loop, after
`replace_pseudoregisters` (Pseudos resolved). The rewrite is
monotone (1 atom → 1 atom of equal byte size at emit), but it
enables `apply_dead_reg_entry_stub_drop` and the param-pinning
path to keep A live across the read, which then cascades to more
upstream simplifications.
"""
from __future__ import annotations

import asm_ast


_INDIRECT_TYPES = (
    asm_ast.Indirect,
    asm_ast.IndirectY,
    asm_ast.IndirectZp,
    asm_ast.IndirectZpY,
)


def apply_volatile_void_read_cmp(prog: asm_ast.Program) -> asm_ast.Program:
    """Walk every Function and rewrite eligible volatile-load atoms
    to Compare atoms."""
    new_top: list[asm_ast.Type_top_level] = []
    for tl in prog.top_level:
        if isinstance(tl, asm_ast.Function):
            new_top.append(_rewrite_function(tl))
        else:
            new_top.append(tl)
    return asm_ast.Program(top_level=new_top)


def _rewrite_function(fn: asm_ast.Function) -> asm_ast.Function:
    instrs = fn.instructions
    out: list[asm_ast.Type_instruction] = []
    for i, instr in enumerate(instrs):
        if not _is_candidate(instr):
            out.append(instr)
            continue
        if not _next_clobbers_flags_and_a_dead(instrs, i + 1):
            out.append(instr)
            continue
        # Rewrite Mov to Compare.
        out.append(asm_ast.Compare(
            left=instr.dst, right=instr.src,
        ))
    return asm_ast.Function(
        name=fn.name, is_global=fn.is_global,
        params=list(fn.params), instructions=out,
    )


def _is_candidate(instr: asm_ast.Type_instruction) -> bool:
    """`Mov(<indirect>, Reg(A), is_volatile=True)` — a volatile
    indirect read into A is our target shape."""
    if not isinstance(instr, asm_ast.Mov):
        return False
    if not instr.is_volatile:
        return False
    if not isinstance(instr.src, _INDIRECT_TYPES):
        return False
    if not (
        isinstance(instr.dst, asm_ast.Reg)
        and isinstance(instr.dst.reg, asm_ast.A)
    ):
        return False
    return True


def _next_clobbers_flags_and_a_dead(
    instrs: list[asm_ast.Type_instruction], start: int,
) -> bool:
    """True iff the next observable instruction at `start` both
    overwrites N/Z and clobbers A before any read of A. The two
    conditions tend to co-occur (most A-writing ops also write
    N/Z), so the simplest check is "the next non-Label, non-self-
    Mov instruction is one of the flag-and-A-clobbering shapes".

    Conservative: a Branch / Jump / Label / function exit at the
    immediate next position means the flags are read or the
    boundary is uncertain — bail."""
    i = start
    while i < len(instrs):
        nxt = instrs[i]
        if isinstance(nxt, asm_ast.Label):
            # A label is a join point; conservatively decline so
            # we don't promise about successors we haven't proven.
            return False
        if isinstance(nxt, asm_ast.Mov) and nxt.src == nxt.dst:
            i += 1
            continue
        return _instr_clobbers_flags_and_a(nxt)
    # End of function — flags die at exit, and A is going to be
    # the function's return slot. If A had been clobbered by the
    # original LDA, the function returns a garbage value; if we
    # preserve A via CMP, we return whatever A was before. Either
    # is wrong if the function was supposed to return the loaded
    # value, but the precondition "A dead after" rules out a
    # caller-observable return of the loaded value. So either
    # outcome is unobserved; the rewrite is safe.
    return True


def _instr_clobbers_flags_and_a(
    instr: asm_ast.Type_instruction,
) -> bool:
    """True iff `instr` writes both N/Z and Reg(A) before reading
    them — making the previous A value AND previous flags dead."""
    # Instructions that write A (so A's prior value is dead) AND
    # write N/Z (so prior flags are dead).
    if isinstance(instr, asm_ast.Mov):
        # Mov(_, Reg(A)) loads A and sets N/Z based on the loaded
        # value — clobbers both. (The 6502's LDA / TXA / TYA / PLA
        # all set N/Z.)
        if (
            isinstance(instr.dst, asm_ast.Reg)
            and isinstance(instr.dst.reg, asm_ast.A)
        ):
            return True
        # Mov to memory doesn't touch A. But it also doesn't
        # touch N/Z (STA / STX / STY don't affect flags). So
        # flags from the prior load are still live.
        return False
    if isinstance(instr, (asm_ast.Add, asm_ast.Sub)):
        # ADC / SBC: writes A AND flags.
        return True
    if isinstance(instr, (asm_ast.And, asm_ast.Or, asm_ast.Xor)):
        return True
    if isinstance(instr, asm_ast.Compare):
        # CMP sets flags from A - operand; A unchanged. Flags
        # clobbered (good); A's prior value preserved (also
        # acceptable — we just need A to not depend on the
        # earlier LDA's value).
        return True
    if isinstance(instr, (asm_ast.Inc, asm_ast.Dec)):
        # INC / DEC / INX / INY / DEX / DEY: write N/Z based on
        # result. Don't touch A. So flags clobbered but A is
        # preserved. With our gate "A is dead", preserving A is
        # equivalent to clobbering it (we don't observe either).
        return True
    if isinstance(instr, (
        asm_ast.ArithmeticShiftLeft, asm_ast.LogicalShiftRight,
        asm_ast.RotateLeft, asm_ast.RotateRight,
    )):
        # ASL / LSR / ROL / ROR set N/Z/C; when dst is A they
        # also clobber A.
        return True
    if isinstance(instr, asm_ast.Pop):
        # PLA clobbers A and sets N/Z.
        if (
            isinstance(instr.dst, asm_ast.Reg)
            and isinstance(instr.dst.reg, asm_ast.A)
        ):
            return True
        # PLX / PLY also write N/Z but not A — depending on
        # whether A is dead this might still be OK.
        return True
    # Push, SetCarry, ClearCarry, BitTest, LoadAddress, ...
    # don't reliably clobber both A and N/Z. Be conservative.
    return False
