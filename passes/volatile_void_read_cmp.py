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
    The gate "N/Z and C are dead after the load" makes this
    difference unobservable.

The gates come from `asm_liveness`: A, N/Z, and C all have to be
dead after the load. An earlier hand-written gate accepted any
following instruction that writes A and the flags, including `AND`,
`ORA`, `ADC`, and `CMP`, which also read A. That turned
`while (!(*p & 0x80)) ;` into a loop that tested the stale A instead
of the byte it read.

Where to run: in the asm-peephole fixed-point loop, after
`replace_pseudoregisters` (Pseudos resolved). The rewrite is
monotone (1 atom → 1 atom of equal byte size at emit), but it
enables `apply_dead_reg_entry_stub_drop` and the param-pinning
path to keep A live across the read, which then cascades to more
upstream simplifications.
"""
from __future__ import annotations

import asm_ast
from passes.asm_liveness import a_dead_at, c_dead_at, flags_dead_at


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
        # The loaded value and the flags it sets must go unread. CMP
        # also writes C, which LDA leaves alone, so C has to be dead
        # too: a carry chain (`ADC; LDA vol; LDA x; ADC`) or a
        # carry-flag return can read the C from before the load.
        if not (a_dead_at(instrs, i + 1) and flags_dead_at(instrs, i + 1)
                and c_dead_at(instrs, i + 1)):
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

