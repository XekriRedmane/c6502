"""Drop adjacent `STA M; LDA M` round-trip when A's value already
reflects M, AND the preceding A-writer already set the flags to
A's value.

# Pattern

Three consecutive instructions:

    [i-1]   <writes Reg(A), sets N/Z to bit7(A) / (A==0)>
    [i]     Mov(Reg(A), <mem M>)    # STA M
    [i+1]   Mov(<mem M>, Reg(A))    # LDA M  (same M)

→ drop [i+1].

# Soundness

After [i-1], A = some value V, N = bit7(V), Z = (V==0).
After [i] (STA M), A = V, M = V, flags unchanged.
After [i+1] (LDA M), A = M = V (unchanged), N = bit7(V), Z = (V==0).

So [i+1] is observably a no-op: A is unchanged, and the flag
state was already what [i+1] would produce. Dropping is sound
regardless of whether a subsequent instruction reads A or the
flags.

The `redundant_load_elimination` pass recognizes the same
register-mirror state but bails when the flags are live downstream
(because in the GENERAL case, the LDA's flag effect could differ
from whatever the previous flag-setter left behind). Here we
specifically gate on the preceding A-writer having set the flags
to A's value, so the gap doesn't apply.

# Eligibility of the preceding A-writer ([i-1])

Any instruction whose `dst` is `Reg(A)` AND that sets N/Z to
bit7(A)/Zero(A) — that is, the flag effect is "result-based":

  * `Mov(<non-Reg>, Reg(A))` — LDA imm / LDA M / TXA / TYA.
  * `And(_, Reg(A))` — AND.
  * `Or(_, Reg(A))` — ORA.
  * `Add(_, Reg(A))` — ADC.
  * `Sub(_, Reg(A))` — SBC.
  * `Xor(_, _, Reg(A))` — EOR.
  * `Pop(dst=Reg(A))` — PLA (sets N/Z based on pulled value).
  * `ArithmeticShiftLeft(Reg(A))` — ASL A.
  * `LogicalShiftRight(Reg(A))` — LSR A.
  * `RotateLeft(Reg(A))` / `RotateRight(Reg(A))` — ROL/ROR A.

# Not eligible

  * `Compare` — leaves N/Z based on the subtraction, NOT A's
    value. Skip.
  * `Inc(M)` / `Dec(M)` / `ASL M` / etc. — flag-setters on
    memory, not on A.
  * `Branch` / `Jump` / `Label` / `Call` — block boundaries.
  * `ClearCarry` / `SetCarry` — don't touch N/Z.

# Where to run

Inside the asm-peephole fixed-point loop, before the
`asm_dead_store` step in the next iteration (so the now-isolated
`STA M` can be DSE'd if M isn't read elsewhere)."""

from __future__ import annotations

import asm_ast


def apply_round_trip_load_drop(prog: asm_ast.Program) -> asm_ast.Program:
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
    i = 0
    while i < len(instrs):
        # Look for the 3-instruction window [i, i+1, i+2].
        if (i + 2 < len(instrs)
                and _writes_a_with_flag_effect(instrs[i])
                and _is_sta(instrs[i + 1])
                and _is_lda_same_addr(instrs[i + 2], instrs[i + 1])):
            out.append(instrs[i])
            out.append(instrs[i + 1])
            # Drop instrs[i + 2].
            i += 3
            continue
        out.append(instrs[i])
        i += 1
    return asm_ast.Function(
        name=fn.name, is_global=fn.is_global,
        params=list(fn.params), instructions=out,
    )


_A = asm_ast.A


def _is_reg_a(op) -> bool:
    return isinstance(op, asm_ast.Reg) and isinstance(op.reg, _A)


def _writes_a_with_flag_effect(instr) -> bool:
    """True iff `instr` writes Reg(A) AND its N/Z flag effect
    reflects A's new value."""
    if isinstance(instr, asm_ast.Mov):
        # Mov to A: LDA / TXA / TYA — sets flags from loaded value.
        return _is_reg_a(instr.dst)
    if isinstance(instr, (asm_ast.And, asm_ast.Or,
                          asm_ast.Add, asm_ast.Sub)):
        return _is_reg_a(instr.dst)
    if isinstance(instr, asm_ast.Xor):
        return _is_reg_a(instr.dst)
    if isinstance(instr, asm_ast.Pop):
        return _is_reg_a(instr.dst)
    if isinstance(instr, (asm_ast.ArithmeticShiftLeft,
                          asm_ast.LogicalShiftRight,
                          asm_ast.RotateLeft,
                          asm_ast.RotateRight)):
        return _is_reg_a(instr.dst)
    return False


def _is_sta(instr) -> bool:
    """True iff `instr` is `Mov(Reg(A), <stable memory>)`. We
    only match stable memory operands (ZP / Data) because those
    are the ones a subsequent LDA can recognize as the same
    address."""
    return (isinstance(instr, asm_ast.Mov)
            and _is_reg_a(instr.src)
            and isinstance(instr.dst, (asm_ast.ZP, asm_ast.Data)))


def _is_lda_same_addr(instr, sta) -> bool:
    """True iff `instr` is `Mov(<mem>, Reg(A))` whose source is
    structurally identical to `sta.dst`."""
    if not isinstance(instr, asm_ast.Mov):
        return False
    if not _is_reg_a(instr.dst):
        return False
    return _operands_equal(instr.src, sta.dst)


def _operands_equal(a, b) -> bool:
    if isinstance(a, asm_ast.Data) and isinstance(b, asm_ast.Data):
        return a.name == b.name and a.offset == b.offset
    if isinstance(a, asm_ast.ZP) and isinstance(b, asm_ast.ZP):
        return a.address == b.address and a.offset == b.offset
    return False
