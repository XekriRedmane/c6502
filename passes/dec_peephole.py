"""Single-byte DEC peephole — collapse `mem -= 1` SBC chains into
`DEC` instructions.

`tac_to_asm` lowers `Binary(Subtract, x, 1, x)` (in-place sub-1) on
a 1-byte value as a 4-instruction SBC chain:

    Mov(M, A); SetCarry; Sub(Imm(1), A); Mov(A, M)

For `Data(name, k)` (static-storage operands), `ZP(addr+k, 0)`
operands (regalloc-assigned locals), and `Reg(X|Y)` (HwReg-pinned
counters), the 6502 offers a much shorter encoding: `DEC m` is a
2-byte (zp) / 3-byte (abs) read-modify-write that decrements the
addressed byte in place; `DEX` / `DEY` is a 1-byte implicit-mode
opcode that decrements the corresponding index register. Using
that, the 1-byte sub-1 collapses to a single instruction:

    DEC M    (or DEX / DEY)

# Multi-byte case

Multi-byte sub-1 ALSO lowers as an SBC chain (one Sub #1 on byte 0,
Sub #0 on continuation bytes with carry threading), but unlike the
INC chain — where BNE cleanly tests "this byte didn't wrap to
zero" — there's no single-flag DEC test that equates to "this byte
borrowed". After DEC, Z=1 iff result is 0 (input was 1, no borrow);
N=1 iff result has bit 7 set (input was either 0 → 0xFF, or in
0x80..0xFF → 0x7F..0xFE — only the first case borrows). So BMI on
DEC's result conflates the borrow case with normal large-value
results.

Because of this, we DON'T extend the peephole to multi-byte. The
multi-byte SBC chain stays as-is (it's still correct, just longer).
For the 1-byte case the peephole is a pure win.

# Eligibility

  * Memory operand is `Data(name, k)`, `ZP(addr, 0)`, `Reg(X)`,
    or `Reg(Y)`. DEX / DEY exist for the registers; DEC supports
    zp / abs / zp,X / abs,X for memory. `Frame` / `Stack` /
    `Indirect` use indirect-Y, which DEC doesn't address — skip.
  * Pattern is in-place: `LDA M; SEC; SBC #1; STA M` with the
    same M throughout.

# Where to run

After `replace_pseudoregisters` (so operands are concrete `Data`/
`ZP` / `Reg`) and before `expand_long_branches` (no new branches
introduced — single Inc/Dec emit). Same slot as `inc_peephole`,
which it composes with naturally.
"""

from __future__ import annotations

import asm_ast


def apply_dec_peephole(prog: asm_ast.Program) -> asm_ast.Program:
    """Walk every Function top-level and rewrite 1-byte sub-1 chains
    into Dec instructions where eligible. `StaticVariable`s and
    other top-levels pass through unchanged."""
    new_top: list[asm_ast.Type_top_level] = []
    for tl in prog.top_level:
        if isinstance(tl, asm_ast.Function):
            new_top.append(_rewrite_function(tl))
        else:
            new_top.append(tl)
    return asm_ast.Program(top_level=new_top)


def _rewrite_function(fn: asm_ast.Function) -> asm_ast.Function:
    """Walk the function's instruction list. At each position try
    to match the 4-instruction sub-1 pattern; on a match, splice in
    the single Dec and skip past the matched window. Otherwise copy
    one instruction and advance."""
    instrs = fn.instructions
    out: list[asm_ast.Type_instruction] = []
    i = 0
    while i < len(instrs):
        match = _try_match_sub1(instrs, i)
        if match is None:
            out.append(instrs[i])
            i += 1
            continue
        n_consumed, mem_op = match
        out.append(asm_ast.Dec(dst=mem_op))
        i += n_consumed
    return asm_ast.Function(
        name=fn.name, is_global=fn.is_global,
        params=list(fn.params), instructions=out,
    )


_REG_A = asm_ast.Reg(reg=asm_ast.A())


def _try_match_sub1(
    instrs: list[asm_ast.Type_instruction], start: int,
) -> tuple[int, asm_ast.Type_operand] | None:
    """Match the 4-instruction sub-1 sequence:

        Mov(M, A); SetCarry; Sub(Imm(1), A); Mov(A, M)

    Returns (4, M) on success, or None on failure. M must be a
    DEC-eligible operand (Data / ZP / Reg(X|Y)).

    Crucially, we MUST NOT fire when the matched 4-instruction
    sequence is the FIRST byte of a multi-byte SBC chain. The
    continuation bytes (Mov(M[k+1], A); Sub(Imm 0, A); Mov(A,
    M[k+1])) read the carry threaded through from the prior
    SBC — replacing the first chunk with `Dec(M[0])` would lose
    the carry, and the high-byte computations would borrow
    spuriously. So if the immediately-following 3 instructions
    form a continuation-byte pattern, skip this match."""
    if start + 4 > len(instrs):
        return None
    i0, i1, i2, i3 = instrs[start:start + 4]
    if not (
        isinstance(i0, asm_ast.Mov)
        and _is_dec_eligible_operand(i0.src)
        and i0.dst == _REG_A
    ):
        return None
    if not isinstance(i1, asm_ast.SetCarry):
        return None
    if not (
        isinstance(i2, asm_ast.Sub)
        and i2.src == asm_ast.Imm(value=1)
        and i2.dst == _REG_A
    ):
        return None
    if not (
        isinstance(i3, asm_ast.Mov)
        and i3.src == _REG_A
        and _operands_equal(i3.dst, i0.src)
    ):
        return None
    # Reject if followed by a continuation-byte SBC pattern:
    #   Mov(M[k+1], A); Sub(Imm 0, A); Mov(A, M[k+1])
    if _is_sbc_continuation(instrs, start + 4):
        return None
    # Reject if followed by a Branch that reads C or V — DEC
    # doesn't touch C/V, but SBC sets them, so dropping the SBC
    # would leave C/V at a stale value and BCC/BCS/BVC/BVS would
    # observe wrong flags. The N/Z flags ARE set by DEC and match
    # what the SBC would have set (DEC's N/Z reflect the result),
    # so BMI/BPL/BEQ/BNE remain correct.
    if _next_branch_reads_c_or_v(instrs, start + 4):
        return None
    return (4, i0.src)


def _next_branch_reads_c_or_v(
    instrs: list[asm_ast.Type_instruction], pos: int,
) -> bool:
    """True iff the next flag-relevant instruction at or after
    `pos` is a Branch on C (BCC/BCS) or V (BVC/BVS). Walks past
    instructions that don't read flags (LDA / STA / etc. — these
    set N/Z but don't read prior flag state) until hitting a
    Branch or another instruction that resets all flags."""
    j = pos
    n = len(instrs)
    while j < n:
        instr = instrs[j]
        if isinstance(instr, asm_ast.Branch):
            return isinstance(
                instr.cond, (asm_ast.CC, asm_ast.CS, asm_ast.VC, asm_ast.VS),
            )
        # Any flag-setting instruction other than a Branch: assume
        # we're not the relevant flag source anymore. Conservative:
        # reset all flags means the SBC's flags don't reach the
        # branch, so the peephole is safe regardless.
        if isinstance(instr, (
            asm_ast.Mov, asm_ast.Add, asm_ast.Sub, asm_ast.And,
            asm_ast.Or, asm_ast.Xor, asm_ast.Compare, asm_ast.Inc,
            asm_ast.Dec, asm_ast.ArithmeticShiftLeft,
            asm_ast.LogicalShiftRight, asm_ast.RotateLeft,
            asm_ast.RotateRight, asm_ast.Pop,
            asm_ast.SetCarry, asm_ast.ClearCarry,
        )):
            return False
        # Labels, Jumps, Calls, etc. — control flow boundary; we
        # can't reason cross-block here, so be safe.
        if isinstance(instr, (asm_ast.Label, asm_ast.Jump,
                              asm_ast.Call, asm_ast.Ret,
                              asm_ast.Return)):
            return False
        j += 1
    return False


def _is_sbc_continuation(
    instrs: list[asm_ast.Type_instruction], pos: int,
) -> bool:
    """True iff `instrs[pos:pos+3]` is the 3-instruction continuation
    byte of a multi-byte SBC chain: `Mov(M, A); Sub(Imm 0, A);
    Mov(A, M)`. The carry from the prior byte's SBC threads into
    this Sub through C, so the first byte's chain can't be replaced
    by `Dec` (which doesn't touch C) without breaking the chain."""
    if pos + 3 > len(instrs):
        return False
    j0, j1, j2 = instrs[pos:pos + 3]
    if not (
        isinstance(j0, asm_ast.Mov)
        and j0.dst == _REG_A
    ):
        return False
    if not (
        isinstance(j1, asm_ast.Sub)
        and j1.src == asm_ast.Imm(value=0)
        and j1.dst == _REG_A
    ):
        return False
    if not (
        isinstance(j2, asm_ast.Mov)
        and j2.src == _REG_A
        and _operands_equal(j2.dst, j0.src)
    ):
        return False
    return True


def _is_dec_eligible_operand(op: asm_ast.Type_operand) -> bool:
    """True iff `op` is a memory operand DEC / DEX / DEY can address
    directly: `Data(name, k)`, `ZP(addr, 0)`, `Reg(X)`, or `Reg(Y)`.
    Frame / Stack / Indirect / IndexedData use addressing modes DEC
    doesn't support."""
    if isinstance(op, (asm_ast.Data, asm_ast.ZP)):
        return True
    if isinstance(op, asm_ast.Reg):
        return isinstance(op.reg, (asm_ast.X, asm_ast.Y))
    return False


def _operands_equal(
    a: asm_ast.Type_operand, b: asm_ast.Type_operand,
) -> bool:
    """True iff `a` and `b` denote the same byte / register."""
    if isinstance(a, asm_ast.Data) and isinstance(b, asm_ast.Data):
        return a.name == b.name and a.offset == b.offset
    if isinstance(a, asm_ast.ZP) and isinstance(b, asm_ast.ZP):
        return a.address == b.address and a.offset == b.offset
    if isinstance(a, asm_ast.Reg) and isinstance(b, asm_ast.Reg):
        return type(a.reg) is type(b.reg)
    return False
